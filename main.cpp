#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <memory>

#include <curl/curl.h>

#include "APILoadTester.h"


static constexpr size_t MAX_PARALLEL = 10;


static bool isUInt( const std::string& s ) 
{
    return !s.empty() && std::all_of( s.begin(), s.end(), ::isdigit );
}

bool checkParams( const int argc, char* argv[], 
                  std::string& url, size_t& requestsCount )
{
    if ( argc < 3 )
    {
        std::cerr << "Very few input arguments. There should be 1. URL-string; 2. Number of all requests to send.\n";
        return false;
    }
    else 
    {
        url = argv[1];
        if ( isUInt( argv[2] ) )
        {
            requestsCount = atoi( argv[2] );
        }
        else
        {
            std::cerr << "\nIncorrect 2nd parameter.\n";
            return false;            
        }

        return true;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        std::string url {};
        size_t requestsCount;
        if ( !checkParams( argc, argv, url, requestsCount ) )
        {
            return 0;
        }

        auto tester = std::make_unique<APILoadTester>( url, requestsCount, MAX_PARALLEL );
        tester->run();
        const APILoadTestResult& testResult = tester->getResult();

        std::cout << "Status codes: \n";
        for ( const auto& [code, count] : testResult.codesMap )
        {
            std::cout << std::to_string( code ) + " - " + std::to_string( count ) + " req" << std::endl;
        }

        const int avgIntTimeInMS = int( 1000. * testResult.time / requestsCount );
        std::cout << "\nAvg req time: " + std::to_string( avgIntTimeInMS ) + " ms" << std::endl;
    }
    catch ( const char* errorStr )
    {
        std::cout << errorStr << std::endl;
    }
    catch ( const std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
