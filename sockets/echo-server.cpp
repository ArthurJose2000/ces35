#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include "http_response.h"

void readRequests(char *buf, int *clientSockfd);

int main(int argc, char *argv[]) {

  if (argc < 3){
    std::cout << "Erro: Indique a porta e o diretório que será servido\n";
    return 0;
  }

  // server port
  int port = atoi(argv[1]);

  // cria um socket para IPv4 e usando protocolo de transporte TCP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // Opções de configuração do SOCKETs
  // No sistema Unix um socket local TCP fica preso e indisponível 
  // por algum tempo após close, a não ser que configurado SO_REUSEADDR
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  // struct sockaddr_in {
  //  short            sin_family;   // e.g. AF_INET, AF_INET6
  //  unsigned short   sin_port;     // e.g. htons(3490)
  //  struct in_addr   sin_addr;     // see struct in_addr, below
  //  char             sin_zero[8];  // zero this if you want to
  // };

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);     // porta tem 16 bits, logo short, network byte order
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
  std::cout << addr.sin_port << " #\n";

  // realizar o bind (registrar a porta para uso com o SO) para o socket
  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }

  // colocar o socket em modo de escuta, ouvindo a porta 
  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return 3;
  }

  // aceitar a conexao TCP
  // verificar que sockfd e clientSockfd sao sockets diferentes
  // sockfd eh a "socket de boas vindas"
  // clientSockfd eh a "socket diretamente com o cliente"
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("accept");
    return 4;
  }

  // usa um vetor de caracteres para preencher o endereço IP do cliente
  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Accept a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

  // faz leitura e escrita dos dados da conexao 
  // utiliza um buffer de 1MB (char)
  bool isEnd = false;
  char buf[1024*1024] = {0};
  std::stringstream ss;

  while (!isEnd) {
    // zera a memoria do buffer
    memset(buf, '\0', sizeof(buf));

    // recebe ate 1 MB bytes do cliente remoto
    if (recv(clientSockfd, buf, 1024*1024, 0) == -1) {
      perror("recv");
      return 5;
    }


    //thread(readRequests, &buf, &clientSockfd).detach();

    HTTPResponse response(buf);
    string str_response = response.sendResponse();

    // Imprime o valor recebido no servidor antes de reenviar
    // para o cliente de volta
    ss << buf << std::endl;
    std::cout << buf << std::endl;

    // envia de volta o buffer recebido como um echo
    if (send(clientSockfd, str_response.c_str(), 1024*1024, 0) == -1) {
      perror("send");
      return 6;
    }

    // o conteudo do buffer convertido para string pode 
    // ser comparado com palavras-chave
    if (ss.str() == "close\n")
      break;

    // zera a string para receber a proxima
    ss.str("");
  }

  // fecha o socket
  close(clientSockfd);

  return 0;
}

void readRequests(char *buf, int *clientSockfd){

    std::stringstream ss;

    HTTPResponse response(buf);
    string str_response = response.sendResponse();

    // Imprime o valor recebido no servidor antes de reenviar
    // para o cliente de volta
    ss << buf << std::endl;
    std::cout << buf << std::endl;

    // envia de volta o buffer recebido como um echo
    if (send(*clientSockfd, str_response.c_str(), 1024*1024, 0) == -1) {
      perror("send");
      return;
    }

    // o conteudo do buffer convertido para string pode 
    // ser comparado com palavras-chave
    if (ss.str() == "close\n")
      return;

    // zera a string para receber a proxima
    ss.str("");
}