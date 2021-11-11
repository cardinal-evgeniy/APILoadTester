#include "MultiCurlHandler.h"

// to disable printing a response body to console
static size_t writeCallback( void* /*data*/, size_t size, size_t nmemb, void* /*userp*/ )
{
    return size * nmemb;
}

void MultiCurlHandler::setMaxConnects( const long N ) 
{
    curl_multi_setopt( mc_, CURLMOPT_MAXCONNECTS, N );
}

void MultiCurlHandler::addHandle( const char* url )
{ 
    CURL* const e = curl_easy_init();
    curl_easy_setopt( e, CURLOPT_WRITEFUNCTION, writeCallback );
    curl_easy_setopt( e, CURLOPT_URL, url );
    curl_easy_setopt( e, CURLOPT_PRIVATE, url );
    curl_multi_add_handle( mc_, e );
}

void MultiCurlHandler::removeHandle( CURL* const e )
{
    curl_multi_remove_handle( mc_, e );
    curl_easy_cleanup( e );
}
