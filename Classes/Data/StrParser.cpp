//
//  StrParser.cpp
//  magicheroes
//
//  Created by mac022 on 13-1-30.
//
//

#include "StrParser.h"
#include "../Storage/Storage.h"

StrParser::StrParser():finish(false)
{
}

StrParser::~StrParser()
{
}

bool    StrParser::parseSavedFile(char *fileName, cocos2d::CCDictionary *dic)
{   
    if( !fileName || !dic )
    {
        return false;
    }
    
    unsigned long fileSize;
    unsigned char *fileContent = STORAGE->readSavedFile(fileName, &fileSize);
    bool ret = parseContent(fileContent, fileSize, dic);
    CC_SAFE_DELETE_ARRAY(fileContent);
    
    return ret;
}

bool    StrParser::parsePackageFile(char *fileName, cocos2d::CCDictionary *dic)
{
    if( !fileName || !dic )
    {
        return false;
    }
    
    unsigned long fileSize;
    unsigned char *fileContent = STORAGE->readPackageFile(fileName, &fileSize);
    bool ret = parseContent(fileContent, fileSize, dic);
    CC_SAFE_DELETE_ARRAY(fileContent);
    
    return ret;
}

bool    StrParser::parseContent(unsigned char *content, unsigned long contentSize, cocos2d::CCDictionary *dic)
{
    if( !content || !dic )
    {
        return false;
    }
        
    finish = false;
    
    bool ret = false;
    bool readKey = true;
    bool escape = false;
    unsigned char ch;
    unsigned long keyStartIndex = 0;
    unsigned long valueStartIndex = 0;
    
    std::string    *key;
    
    for( unsigned long i = 0; i < contentSize; i ++ )
    {
        ch = content[i];
        
        if (escape == false && ch == '\\')
        {
            escape = true;
            continue;
        }
        
        if (readKey && ch == ':')
        {
            
            key = new std::string((const char*)(content+keyStartIndex), i - keyStartIndex);
            
            readKey = false;
            valueStartIndex = i + 1;
            
            continue;
        }
        
        if (escape == true)
        {
            switch (ch)
            {
                case 'n':
                    ch = '\n';
                    break;
                    
                case 'r':
                    ch = '\r';
                    break;
                    
                case 'f':
                    ch = '\f';
                    break;
                    
                case '\\':
                    ch = '\\';
                    break;
                    
                case 't':
                    ch = '\t';
                    break;
                    
                case 'b':
                    ch = '\b';
                    break;
                    
                case '/':
                    ch = '/';
                    break;
                    
                case '"':
                    ch = '"';
                    break;
                    
                case 'u':
                {
                    int uffff = 0;
                    for (int j=0; j<4; j++)
                    {
                        i ++;
                        unsigned char chr = content[i];
                        int hex = hexToInt (chr);
                        if (hex < 0)
                        {
                            uffff = (int)chr;
                            break;
                        }
                        uffff = uffff * 16 + hex;
                    }
                    ch = (char)uffff;
                    break;
                }
                    
                default:
                    escape = false;
                    break;
                    
            }
        }
        
        if (escape == false)
        {
            if ((ch == '\r' || ch == '\n'))
            {
                if (!readKey)
                {
                    dic->setObject(CCString::createWithData(content+valueStartIndex, i - valueStartIndex),*key );
                    delete key;
                }
                
                readKey = true;
                keyStartIndex = i + 1;
                continue;
            }
        } else {
            escape = false;
        }
    }
    finish = true;
    
    return ret;
}

int StrParser::hexToInt(char h)
{
    if ((h >= '0') && (h <= '9')) {
        return (h - '0');
    }else if ((h >= 'a') && (h <= 'f')) {
        return ((h - 'a') + 10);
    }else if ((h >= 'A') && (h <= 'F')) {
        return ((h - 'A') + 10);
    }
    return -1;
}