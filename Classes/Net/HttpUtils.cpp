//
//  Http.cpp
//  magicheroes
//
//  Created by mac022 on 12-12-18.
//
//

#include "HttpUtils.h"
#include "Storage/Storage.h"

DELCARE_SINGLETON_MEMBER(HttpUtils);

HttpUtils::HttpUtils()
{
    httpClient = CCHttpClient::getInstance();
    CCAssert(httpClient,"httpClient is NULL");
    
    memset(this->url, 0, URL_MAX_SIZE);
    baseUrlLen = 0;
}

HttpUtils::~HttpUtils()
{
    CCHttpClient::destroyInstance();
}

void    HttpUtils::setBaseUrl(const char * url)
{
    baseUrlLen = strlen(url);
    
    strcpy(this->url, url);
}

void    HttpUtils::setTimeoutForConnect(int value)
{
    httpClient->setTimeoutForConnect(value);
}

int     HttpUtils::getTimeoutForConnect()
{
    return httpClient->getTimeoutForConnect();
}

void    HttpUtils::setTimeoutForRead(int value)
{
    httpClient->setTimeoutForRead(value);
}

int     HttpUtils::getTimeoutForRead()
{
    return  httpClient->getTimeoutForRead();
}

void    HttpUtils::post(PostParam *p)
{
    HttpReq *request = new HttpReq();
    CCAssert(request, "Get request is NULL");
    
    url[baseUrlLen]='\0';
    strcat(url, p->url);
    request->setUrl(url);
    request->setOkCallback(p->ok);
    request->setErrorCallback(p->error);
    request->setResponseCallback(this, callfuncND_selector(HttpUtils::onHttpRequestCompleted));
    request->setRequestData(p->data, strlen(p->data));
    request->setRequestType(CCHttpRequest::kHttpPost);
    httpClient->send(request);
    
    request->release();
}

void    HttpUtils::get(GetParam *g)
{
    HttpReq *request = new HttpReq();
    CCAssert(request, "Get request is NULL");
    
    url[baseUrlLen]='\0';
    strcat(url, g->url);
    request->setUrl(url);
    request->setOkCallback(g->ok);
    request->setErrorCallback(g->error);
    request->setResponseCallback(this, callfuncND_selector(HttpUtils::onHttpRequestCompleted));
    request->setRequestType(CCHttpRequest::kHttpGet);
    httpClient->send(request);
    
    request->release();
}

void    HttpUtils::send(CCHttpRequest *request)
{
    request->retain();
    request->setResponseCallback(this, callfuncND_selector(HttpUtils::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void    HttpUtils::download(GetParam *g, char *fileName)
{
    HttpReq *request = new HttpReq();
    CCAssert(request, "Get request is NULL");
    
    url[baseUrlLen]='\0';
    strcat(url, g->url);
    request->setUrl(url);
    request->setOkCallback(g->ok);
    request->setErrorCallback(g->error);
    request->setDownloadFileName(fileName);
    request->setResponseCallback(this, callfuncND_selector(HttpUtils::onDownloadCompleted));
    request->setRequestType(CCHttpRequest::kHttpGet);
    httpClient->send(request);
    
    request->release();
}

void    HttpUtils::onDownloadCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    HttpReq     *req = (HttpReq*)response->getHttpRequest();
    CCObject    *target = req->getTarget();
    SEL_HTTPOKCALLBACK      okCallback = req->getOkCallback();
    SEL_HTTPERRORCALLBACK    errorCallback = req->getErrorCallback();
    
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        
        if( target && errorCallback )
        {
            (target->*errorCallback)(response->getErrorBuffer());
        }
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string downloadData;
    downloadData.assign(buffer->begin(),buffer->end());
    bool ok = STORAGE->saveFile(req->getDownloadFileName(), downloadData.c_str(), downloadData.size());
    if( ok ){
        
        if( target && okCallback )
        {
            (target->*okCallback)(NULL);
        }
    }else{
        SEL_HTTPERRORCALLBACK    errorCallback = req->getErrorCallback();
        if( target && errorCallback )
        {
            (target->*errorCallback)("Save file error!");
        }
    }
}

void    HttpUtils::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    HttpReq     *req = (HttpReq*)response->getHttpRequest();
    CCObject    *target = req->getTarget();
    if( !target )
    {
        return;
    }
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        SEL_HTTPERRORCALLBACK    errorCallback = req->getErrorCallback();
        if( errorCallback )
        {
            (target->*errorCallback)(response->getErrorBuffer());
        }
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string json;
    json.assign(buffer->begin(),buffer->end());
    JSONNode n;
    try{
        n = libjson::parse(json);
    }catch(std::exception e){
        CCLOG("Json parse error, content:%s",json.c_str());
        throw e;
    }

    CCDictionary dic;
    ParseJSON(n, dic);
    SEL_HTTPOKCALLBACK    okCallback = req->getOkCallback();
    if( okCallback )
    {
        (target->*okCallback)(&dic);
    }
}

void    HttpUtils::testHttp()
{
//    std::string json="{\"RootA\":[1,2]}";
//    JSONNode n = libjson::parse(json);
//    CCDictionary dic;
//    ParseJSON(n,dic);
}

void HttpUtils::ParseJSON(const JSONNode &n, CCDictionary &dic)
{
    JSONNode::const_iterator i = n.begin();
    
    while (i != n.end())
    {
        switch (i->type()) {
            case JSON_NODE:
            {
                CCDictionary *subDic = CCDictionary::create();
                dic.setObject(subDic, i->name());
                ParseJSON(*i, *subDic);
                break;
            }
                
            case JSON_STRING:
            case JSON_BOOL:
            case JSON_NUMBER:
            case JSON_NULL:
            {
                CCString    *value = new CCString( i->as_string() );
                CCLOG("name:%s value:%s", i->name().c_str(),value->getCString());
                dic.setObject(value, i->name());
                value->release();
                break;
            }
                
            case JSON_ARRAY:
            {
                CCArray *array = new CCArray();
                dic.setObject(array, i->name());
                ParseJSON(*i,*array);
                array->release();
                break;
            }
                
                
            default:
                CCAssert(false, "JSON parse error!");
                break;
        }

        //increment the iterator
        ++i;
    }
}

void    HttpUtils::ParseJSON(const JSONNode &n, CCArray &array)
{
    JSONNode::const_iterator i = n.begin();
    
    while (i != n.end())
    {
        switch (i->type()) {
            case JSON_NODE:
            {
                CCDictionary *subDic = CCDictionary::create();
                array.addObject(subDic);
                ParseJSON(*i, *subDic);
                break;
            }
                
            case JSON_STRING:
            case JSON_BOOL:
            case JSON_NUMBER:
            case JSON_NULL:
            {
                CCString    *value = new CCString( i->as_string() );
                array.addObject(value);
                value->release();
                break;
            }
                
            case JSON_ARRAY:
            {
                CCArray *arr = new CCArray();
                array.addObject(arr);
                ParseJSON(*i,*arr);
                arr->release();
                break;
            }
                
                
            default:
                CCAssert(false, "JSON parse error!");
                break;
        }
        
        //increment the iterator
        ++i;
    }

}