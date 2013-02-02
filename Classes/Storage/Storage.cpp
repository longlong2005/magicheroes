//
//  Storage.cpp
//  magicheroes
//
//  Created by mac022 on 12-12-19.
//
//

#include "Storage.h"
//#include "stdio.h"

DELCARE_SINGLETON_MEMBER(Storage);

Storage::Storage()
{
    
}

Storage::~Storage()
{

}

bool    Storage::saveFile(const char *fileName, const char *data, int dataLen)
{
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    std::string path= fileUtils->getWriteablePath();
    path += fileName;
    const  char *cPath = path.c_str();
	FILE *fp = fopen(cPath, "w");
    
	if (!fp)
	{
		CCLOG("can not create file %s", cPath);
		return false;
	}
    
	size_t len = fwrite(data, 1, dataLen, fp);
	fclose(fp);
    
    if( len != dataLen )
    {
        remove(cPath);
        return false;
    }
    return true;
}

unsigned char*   Storage::readSavedFile(const char* fileName, unsigned long *size)
{
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    std::string path= fileUtils->getWriteablePath();
    path += fileName;
    const  char *cPath = path.c_str();
	
    return fileUtils->getFileData(cPath, "r", size);
}

bool    Storage::removeSavedFile(const char *fileName)
{
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    std::string path= fileUtils->getWriteablePath();
    path += fileName;
    const  char *cPath = path.c_str();
    
    return remove(cPath) == 0;
    
}

unsigned    char*   Storage::readPackageFile(const char* fileName, unsigned long *size)
{
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    const char *path = fileUtils->fullPathFromRelativePath(fileName);
    return fileUtils->getFileData(path, "r", size);
}
