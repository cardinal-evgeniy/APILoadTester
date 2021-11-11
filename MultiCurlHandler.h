#pragma once

#include <curl/curl.h>


class MultiCurlHandler
{
public:
    explicit MultiCurlHandler() : mc_( curl_multi_init() ) 
    {
        if ( !mc_ )
        {
            throw "Invalid Multi Curl Handler";
        }
    }

    MultiCurlHandler( const MultiCurlHandler& ) = delete;
    MultiCurlHandler& operator=( const MultiCurlHandler& ) = delete;
    MultiCurlHandler( MultiCurlHandler&& ) = delete;
    MultiCurlHandler& operator=( MultiCurlHandler&& ) = delete;
    ~MultiCurlHandler() noexcept { curl_multi_cleanup( mc_ ); }

    void setMaxConnects( const long N );
    void addHandle( const char* url );
    void removeHandle( CURL* const e );
    CURLM* getMC() const { return mc_; }

private:
    CURLM* const mc_ = nullptr;
};
