#ifndef UICS_SOCKETS_H
#define UICS_SOCKETS_H

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h> // for close
#include <arpa/inet.h>
#include "inst_log.h"
#include "b64.h"

#define h_addr h_addr_list[0]//to be posix complient


extern Log instLog;

inline void log_socket(std::string txt){instLog.writeto("[socket] "+txt,3 );}
inline void Perror(std::string txt){log_socket(txt);perror(txt.c_str());}

int create_socket(uint16_t port);

int read_socket(std::string *buffer,int sockfd);
int read_socket(uint16_t port,std::string *buffer);
int read_socket(uint16_t port,std::string *buffer,std::string addr);
int read_image(unsigned int *im,size_t length,int sockfd);
int recv_image(int sockfd,uint16_t **im);
int write_image(uint16_t port, unsigned int *im,size_t length,std::string addr);
int write_socket(uint16_t port, std::string command,std::string addr);
int write_socket(uint16_t port, std::string command);
int write_socket(std::string command,int sock);

#endif // UICS_SOCKETS_H

