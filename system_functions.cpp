#include "system_functions.hpp"

int string_to_int(std::string str) {

  int res;

  try {
    res = std::stoi(str, nullptr);
  }

  catch(std::invalid_argument const &e) {
    std::cerr << "std::invalid_argument: " << e.what() << "\n";
  }

  catch(std::out_of_range const &e) {
    std::cerr << "std::out_of_range: " << e.what() << "\n";
  }

  return res;

}

int check_port(std::string port) {

  int p = string_to_int(port);

  if (!(p >= MIN_PORT_NUMBER && p <= MAX_PORT_NUMBER)) {
    throw std::invalid_argument(
				"Port number outside of range" +
				std::to_string(MIN_PORT_NUMBER) + "-" +
				std::to_string(MAX_PORT_NUMBER));

  }

  return p;

}

bool check_directory(std::string directory) {

  struct stat s;
  bool is_directory = true;
  
  // breaks another function
  if (stat(directory.c_str(), &s) != 0) {
    throw std::invalid_argument("Invalid directory");
  }

  else if(!(s.st_mode & S_IFDIR)) {
    is_directory = false;
  }
  
  return is_directory;
  
}

int create_socket() {

  int s = socket(AF_INET, SOCK_STREAM, 0);

  if (s < 0) {
    throw std::runtime_error("Cannot create socket");
  }

  return s;
}

void bind_socket(int socket, sockaddr_in &addr) {

  int res = bind(socket, (sockaddr*)&addr, sizeof(addr));
  
  if (res < 0) {
    throw std::runtime_error("Cannot bind socket");
  }

}

void listen_on_socket(int socket, int backlog) {

  int listen_res = listen(socket, backlog);
  
  if (listen_res < 0) {
    throw std::runtime_error("Cannot listen on socket");
  }

}

void close_socket(int socket) {

  int close_res = close(socket);

  if (close_res < 0) {
    throw std::runtime_error("Cannot terminate connection");
  }

}

int accept_connection(int socket, sockaddr_in &addr) {
  
  socklen_t len = sizeof(addr);
  int accept_res = accept(socket, (sockaddr*)&addr, &len);
  
  if (accept_res < 0) {
    throw std::runtime_error("Cannot create connection on socket");
  }
  
  return accept_res;

}

int process_file(std::string path, std::stringstream &buffer) {

    // based on https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  
  std::ifstream f;
  f.open(path);

  if (f.bad()) {
    throw std::runtime_error("Failed to open " + path);
  }

  f.seekg(0, std::ios::end);
  int length = f.tellg();

  f.seekg(0, std::ios::beg);
  buffer << f.rdbuf();
  f.close();
  
  return length;

}

void read_from_socket(int socket, std::string &buffer, int length) {

  int read_res = read(socket, buffer.data(), length);
  
  if (read_res < 0) {
    throw std::runtime_error("Failure to read from socket");
  }

}
