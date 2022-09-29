#include "http_request.h"

HTTPRequest::HTTPRequest(string request){

    HTTPRequest::request = request;

}

int HTTPRequest::setURL(){
    
    string request = HTTPRequest::request;

    vector<string> strs = tokenize(request, "://");

    if(strs.size() != 2)
        return -1;

    if(strs[0] != "http" && strs[0] != "https")
        return -1;

    string link = strs[1];

    strs = tokenize(link, "/");
    
    if(strs.size() == 1) {

        vector<string> host = tokenize(strs[0], ":");

        getchar();

        if(host.size() == 1){

            HTTPRequest::hostname = strs[0];
            HTTPRequest::port = HTTP_DEFAULT_PORT;
        }
        else{
            HTTPRequest::hostname = host[0];
            HTTPRequest::port = host[1];
        }

        HTTPRequest::object = "/";

    }
    else {

        vector<string> host = tokenize(strs[0], ":");

        if(host.size() == 1){
            HTTPRequest::hostname = strs[0];
            HTTPRequest::port = HTTP_DEFAULT_PORT;
        }
        else{
            HTTPRequest::hostname = host[0];
            HTTPRequest::port = host[1];
        }

        for(int i = 1; i < (int) strs.size(); i++)
            object = object + "/" + strs[i];

        HTTPRequest::object = object;
    }

    HTTPRequest::hostname = HTTPRequest::getIP(HTTPRequest::hostname) + (HTTPRequest::hostname == "localhost" ? (":" + HTTPRequest::port) : "");

    // std::cout << "hostname: " << HTTPRequest::hostname << "\n";
    // std::cout << "port: " << HTTPRequest::port << "\n";
    // std::cout << "object: " << HTTPRequest::object << "\n";

    return 0; //success
}

vector<string> HTTPRequest::tokenize(string s, string del){

    vector<string> strs;
    
    int start, end = -1*del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        strs.push_back(s.substr(start, end - start));
    } while (end != -1);

    return strs;
}

string HTTPRequest::prepareRequest(){

    string request;

    request = "GET " + HTTPRequest::object + " HTTP/1.0\\r\\n" +
              "Host: " + HTTPRequest::hostname + "\\r\\n";

    return request;
}

void HTTPRequest::readResponse(char *response){

    string str_response(response);
    HTTPRequest::response = str_response;

    vector<string> strs = tokenize(str_response, "\\r\\n");

    string status = tokenize(strs[0], " ")[1];

    if(status == "200"){

        std::cout << "200 OK\n";

        vector<string> aux = tokenize(HTTPRequest::object, "/");
        string file = aux[aux.size() - 1];

        if(file == "")
            file = "index.html";

        ofstream f(file);

        f << strs[strs.size() - 1];

        f.close();

    }
    else if(status == "400"){
        std::cout << "400 Bad Request\n";
    }
    else if(status == "404"){
        std::cout << "404 Not found";
    }    

}

string HTTPRequest::getIP(string hostname){

    string ip;

    struct addrinfo hints;
    struct addrinfo* res;

    // hints - modo de configurar o socket para o tipo  de transporte
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // funcao de obtencao do endereco via DNS - getaddrinfo 
    // funcao preenche o buffer "res" e obtem o codigo de resposta "status" 
    int status = 0;
    if ((status = getaddrinfo(hostname.c_str(), "80", &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return "";
    }

    //std::cout << "IP addresses for " << hostname << ": " << std::endl;

    for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
        // a estrutura de dados eh generica e portanto precisa de type cast
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

        // e depois eh preciso realizar a conversao do endereco IP para string
        char ipstr[INET_ADDRSTRLEN] = {'\0'};
        inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        //std::cout << "  " << ipstr << std::endl;
        string aux(ipstr);
        ip = aux;
    }

    freeaddrinfo(res); // libera a memoria alocada dinamicamente para "res"
    return ip;
}