#include "APILoadTester.h"

#include <regex>


APILoadTester::APILoadTester( const std::string& url, const size_t requestsCount, 
               				  const size_t maxParallelRequestsCount ) : 
    url_( url ), 
    requestsCount_( requestsCount ),
    maxParallelRequestsCount_( maxParallelRequestsCount )
{
    checkInput_();
    curl_global_init( CURL_GLOBAL_ALL );
    mcHandler_ = std::make_unique<MultiCurlHandler>();
}

void APILoadTester::checkInput_() const
{
    if ( !requestsCount_ || !maxParallelRequestsCount_ )
    {
        throw std::invalid_argument( "Invalid number of requests value. Should be more than 0" );
    }

    const std::string& regexPattern = "((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)";
    const std::regex urlRegex( regexPattern );

    if ( !std::regex_match( url_, urlRegex ) )
    {
        throw std::invalid_argument( "Ill-formed URL" );
    }
}

void APILoadTester::run() const
{
    mcHandler_->setMaxConnects( maxParallelRequestsCount_ );

    size_t reqIt = 0;
    for ( ; reqIt < std::min( requestsCount_, maxParallelRequestsCount_ ); ++reqIt )
    {
        mcHandler_->addHandle( url_.c_str() );
    }

    CURLMsg* msg = nullptr;
    int msgsLeft = -1;
    int stillAlive = 1;

    do {
        auto mc = mcHandler_->getMC();
        curl_multi_perform( mc, &stillAlive );
        
        while ( ( msg = curl_multi_info_read( mc, &msgsLeft ) ) ) 
        {
            if ( msg && ( msg->msg == CURLMSG_DONE ) ) 
            {
                CURL* const e = msg->easy_handle;

                long httpCode;
                curl_easy_getinfo( e, CURLINFO_RESPONSE_CODE,&httpCode );
                result_.codesMap[httpCode]++;

                double transferTime;
                curl_easy_getinfo( e, CURLINFO_TOTAL_TIME, &transferTime );
                result_.time += transferTime;

                mcHandler_->removeHandle( e );
            }

            if ( reqIt < requestsCount_ )
            {
                mcHandler_->addHandle( url_.c_str() );
                ++reqIt;
            }
        }
        
        if ( stillAlive )
        {
            curl_multi_wait( mc, NULL, 0, 1000, NULL );
        }

    } while ( stillAlive || ( reqIt < requestsCount_ ) );
}
