#ifndef SYSTEM_FUNCTIONS_H_
#define SYSTEM_FUNCTIONS_H_

#include <iostream>
#include <fstream>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <unistd.h>

const int ARGS = 3;
const int MIN_PORT_NUMBER = 1024;
const int MAX_PORT_NUMBER = (1 << 16) - 1;
const int BACKLOG = 4096; // default on modern Linux
const int TIMEOUT = 1000000;

// input validation
int string_to_int(std::string str);
int check_port(std::string port);
bool check_directory(std::string directory);
// socket operations with added error checking
int create_socket();
void bind_socket(int socket, sockaddr_in &addr); 
void listen_on_socket(int socket, int backlog);
int accept_connection(int socket, sockaddr_in &addr);
void close_socket(int socket);
void read_from_socket(int socket, std::string &buffer, int length);
// file_operations
int process_file(std::string path, std::stringstream &buffer);

#endif // SYSTEM_FUNCTIONS_H_
