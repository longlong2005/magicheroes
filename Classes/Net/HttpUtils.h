//
//  Http.h
//  magicheroes
//
//  Created by mac022 on 12-12-18.
//
//

#ifndef __magicheroes__HttpUtils__
#define __magicheroes__HttpUtils__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Singleton.h"
#include "libjson.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef void (CCObject::*SEL_HTTPOKCALLBACK)(CCDictionary*);
typedef void (CCObject::*SEL_HTTPERRORCALLBACK)(const char*);

#define httpOk_selector(_selector) (SEL_HTTPOKCALLBACK)(&_selector)
#define httpError_selector(_selector) (SEL_HTTPERRORCALLBACK)(&_selector)

struct PostParam
{
    char    *url;
    char    *data;
    CCObject*                   target;
    SEL_HTTPOKCALLBACK          ok;
    SEL_HTTPERRORCALLBACK       error;
};

struct GetParam
{
    char    *url;
    CCObject*                   target;
    SEL_HTTPOKCALLBACK          ok;
    SEL_HTTPERRORCALLBACK       error;
};

class HttpReq: public CCHttpRequest
{
private:
    SEL_HTTPOKCALLBACK    ok;
    SEL_HTTPERRORCALLBACK    error;
    
    std::string         downloadFileName;
    
public:
    inline void    setOkCallback(SEL_HTTPOKCALLBACK ok)
    {
        this->ok = ok;
    }
    
    inline void    setErrorCallback(SEL_HTTPERRORCALLBACK error)
    {
        this->error = error;
    }
    
    inline  SEL_HTTPOKCALLBACK    getOkCallback()
    {
        return ok;
    }
    
    inline  SEL_HTTPERRORCALLBACK    getErrorCallback()
    {
        return error;
    }
    
    inline  void    setDownloadFileName( char * fileName)
    {
        downloadFileName = fileName;
    }
    
    inline  const   char*   getDownloadFileName()
    {
        return downloadFileName.c_str();
    }
};

class HttpUtils: public CCObject, public Singleton<HttpUtils>
{
private:
    enum{
        URL_MAX_SIZE=256,
    };
    CCHttpClient    *httpClient;
    int             baseUrlLen;
    char            url[URL_MAX_SIZE];
    
public:
    void    setBaseUrl(const char * url);
    
    void    setTimeoutForConnect(int value);
    int     getTimeoutForConnect();
    
    void    setTimeoutForRead(int value);
    int     getTimeoutForRead();
    
    void    post(PostParam *p);
    void    get(GetParam *g);
    void    send(CCHttpRequest *request);
    
    void    download(GetParam *g, char *fileName);
    
    void    ParseJSON(const JSONNode &n, CCDictionary &dic);
    
    void    testHttp();
    
    
private:
    //Http Response Callback
    void    onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
    void    onDownloadCompleted(cocos2d::CCNode *sender, void *data);
    
    void    ParseJSON(const JSONNode &n, CCArray &array);
    DECLARE_SINGLETON(HttpUtils)
};

#define HTTP HttpUtils::instance()

#endif /* defined(__magicheroes__Http__) */
