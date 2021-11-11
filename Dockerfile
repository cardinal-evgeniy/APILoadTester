FROM gcc:latest

COPY . /usr/src/APILoadTest

WORKDIR /usr/src/APILoadTest

RUN apt-get -y update & apt-get -y install libcurl4-openssl-dev
RUN g++ main.cpp MultiCurlHandler.cpp APILoadTester.cpp --std=c++17 -lcurl -Wextra -Wall -Werror -pedantic-errors -o APILoadTestApp

ENTRYPOINT [ "./APILoadTestApp" ]
#CMD [ "http://www.google.com", "100" ]
