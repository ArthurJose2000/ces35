#include "http_response.h"
#include <iostream>

using namespace std;

HTTPResponse::HTTPResponse(char *request, string root_directory){

    string str_request(request);

    HTTPResponse::request = str_request;
    HTTPResponse::root_directory = root_directory;

}

string HTTPResponse::sendResponse(){

    vector<string> strs = tokenize(HTTPResponse::request, "\\r\\n");

    if(strs.size() <= 1)
        return prepareResponse("400 Bad Request", "", "");

    string file = tokenize(strs[0], " ")[1];

    if(file == "/")
        file = "/index.html";
    

    HTTPResponse::file = file;
    HTTPResponse::file_type = tokenize(file, ".")[1];

    ifstream f(HTTPResponse::root_directory  + file);

    string content;
    string aux;
    bool exist = false;
    while (getline(f, aux)) {
        exist = true;
        content = content + aux + "\n";
    }

    f.close();

    HTTPResponse::content = content;

    if(!exist)
        return prepareResponse("404 Not Found", "", "");


    std::cout << "directory: " << HTTPResponse::file << "\n";
    std::cout << "file: " << HTTPResponse::file << "\n";
    std::cout << "file type: " << HTTPResponse::file_type << "\n";
    std::cout << "content: " << HTTPResponse::content << "\n";

    return prepareResponse("200 OK", HTTPResponse::content, HTTPResponse::file_type);
}

string HTTPResponse::prepareResponse(string status, string content, string content_type){

    string response = "HTTP/1.0 " + status + "\\r\\n" +
                      "Content-Length: " + std::to_string(content.size()) + "\\r\\n" +
                      "Content-Type: " + content_type + "\\r\\n" +
                      content;

    return response;

}

vector<string> HTTPResponse::tokenize(string s, string del){

    vector<string> strs;
    
    int start, end = -1*del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        strs.push_back(s.substr(start, end - start));
    } while (end != -1);

    return strs;
}