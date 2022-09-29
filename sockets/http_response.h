#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <fstream>
#include <iostream>
using namespace std;

class HTTPResponse {

    public:
        HTTPResponse(char *request, string root_directory);

        string sendResponse(); // será convetida em um vetor de bytes posteriormente

        string prepareResponse(string status, string content, string content_type); // será convetida em um vetor de bytes posteriormente

        void setMethod(); // sempre GET nesse lab

        string getResquetedFile();

        vector<string> tokenize(string s, string del);

        enum METHODS {
            GET
        };

        int method = GET; // sempre GET nesse lab

        string request;

        string response;

        string file;

        string file_type;

        string content;

        string root_directory;

};

#endif