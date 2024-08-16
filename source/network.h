#include <3ds.h>
#include <curl/curl.h>

bool initSocket();
void initcurl();
void initform();
void discordhandleentry(const char* handle);
void essentialdataentry();
void fileentry(const char* filepath);
CURLcode submittourl(const char* url);
void exiteverything();