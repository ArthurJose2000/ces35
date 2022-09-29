#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <cstring>
#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream> 

using namespace std;

#define HTTP_DEFAULT_PORT "80"

class HTTPRequest {

    public:

        HTTPRequest(string request);

        int setURL();

        void setMethod(); // sempre GET nesse lab

        void readResponse(char *response);

        string prepareRequest(); // será convetida em um vetor de bytes posteriormente

        string getIP(string hostname);

        vector<string> tokenize(string s, string del);

        enum METHODS {
            GET
        };

        int method = GET; // sempre GET nesse lab

        string request;

        string response;

        string hostname;

        string port;

        string object;

};

#endif