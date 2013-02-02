//
//  StrParser.h
//  magicheroes
//
//  Created by mac022 on 13-1-30.
//
//

#ifndef __magicheroes__StrParser__
#define __magicheroes__StrParser__

#include "cocos2d.h"

USING_NS_CC;

class   StrParser
{
private:
    bool            finish;
    
private:
    int hexToInt (char h);
    
public:
    StrParser();
    ~StrParser();
    bool    finished(){ return finish; }
    bool    parsePackageFile( char *fileName, CCDictionary *dic );
    bool    parseSavedFile( char *fileName, CCDictionary *dic );
    bool    parseContent( unsigned char *content, unsigned long contentSize, CCDictionary *dic );
    
};

#endif /* defined(__magicheroes__StrParser__) */
