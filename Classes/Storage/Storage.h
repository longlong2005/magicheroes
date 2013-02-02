//
//  Storage.h
//  magicheroes
//
//  Created by mac022 on 12-12-19.
//
//

#ifndef __magicheroes__Storage__
#define __magicheroes__Storage__

#include "cocos2d.h"
#include "Singleton.h"

USING_NS_CC;

class Storage : public Singleton<Storage>
{

public:
    
    bool    saveFile(const char *fileName, const char *data, int dataLen);
    bool    removeSavedFile(const char *fileName);
    
    unsigned    char*   readSavedFile(const char* fileName, unsigned long *size);
    unsigned    char*   readPackageFile(const char* fileName, unsigned long *size);
    
    DECLARE_SINGLETON(Storage)
};

#define STORAGE Storage::instance()
#endif /* defined(__magicheroes__Storage__) */
