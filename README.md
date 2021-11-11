
Utility for the API load testing (`./task.txt`).
Only GET requests so far.

Three options to run a program:
1. 
- `g++ main.cpp MultiCurlHandler.cpp APILoadTester.cpp --std=c++17 -lcurl -Wextra -Wall -Werror -pedantic-errors -o APILoadTestApp`
- `./APILoadTestApp <URL> <REQUESTS_NUM>`

2. Using CMake 
- `mkdir build`
- `cd build`
- `cmake ..`
- `cmake --build .`
- `./APILoadTestApp <URL> <REQUESTS_NUM>`

3. Using Docker
- `docker build . -t apiloadtest:latest`
- `docker run apiloadtest:latest <URL> <REQUESTS_NUM>`
