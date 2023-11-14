#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>

#include "system_functions.hpp"
#include "http_functions.hpp"

int main(int argc, char** argv) {

  if (argc != ARGS) {
    std::cerr << "Incorrect number of arguments!\n"
	      << "Expected " << ARGS - 1 << " and got "
	      << argc - 1 << "\n";
    return -1;
  }

  int port;
  std::string directory = argv[2];
  
  try {
    port = check_port(argv[1]);
  }

  catch(std::invalid_argument const &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }

  try {

    bool is_directory = check_directory(directory);
    if (!is_directory) {
      std::cerr << directory << " is not a directory\n";
      return -1;
    }
    
    if (directory.back() != '/') {
      directory += "/";
    }

  }

  catch(std::invalid_argument const &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }

  // if parameters from stdin are correct, proceed to prepare variables for server

  std::vector<std::string> files;

  // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
  for (auto &file : std::filesystem::directory_iterator(directory)) {
    std::string f = file.path();
    files.emplace_back(f.substr(f.find_last_of("/") + 1));
  }

  int socket;

  try {
    socket = create_socket();
  }

  catch(std::runtime_error const &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  int opt = 1;
  setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

  try {
    bind_socket(socket, addr);
  }

  catch(std::runtime_error const &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }

  try {
    listen_on_socket(socket, BACKLOG);
  }
  
  catch(std::runtime_error const &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }
  
  socklen_t len = sizeof(addr);
  std::map<std::string, std::string> request;
  std::chrono::steady_clock::time_point begin, end;
  begin = std::chrono::steady_clock::now();
  end = std::chrono::steady_clock::now();
  int other = accept(socket, (struct sockaddr *)&addr, &len);;

  while(true) {

    int time_diff = std::chrono::duration_cast<std::chrono::microseconds>
      (end - begin).count() / TIMEOUT;

    if (time_diff > 1 || request["Connection"] == "close") {

      std::chrono::steady_clock::now();
      close(other);
      other = accept(socket, (struct sockaddr *)&addr, &len);

    }

    if (other < 0) {
      std::cerr << "Failed to update the socket\n";
      return -1;
    }

    std::string buf;
    int mslen = 1000;
    buf.resize(mslen);

    try {
      read_from_socket(other, buf, mslen);
    }
    
    catch (std::runtime_error const &e) {
      std::cerr << e.what() << "\n";
    }

    request.clear();
    parse_request(buf, request);

    std::string host = split_string(request["Host"], ":").first;
    request["port"] = split_string(request["Host"], ":").second;
    std::stringstream buffer;
    std::string path = directory + host + request["get"];
    std::string long_enough, whatever;
    
    int code = 200;
    bool bp;
    
    if (request["get"].find("..") != std::string::npos) {
      code = 403;
    }

    if (request["get"].size() == 0) {
      code = 501;
    }
    
    try {

      bp = check_directory(path);
      if (bp && code != 501) {
	code = 301;
      }

    }

    catch (std::invalid_argument const &e) {

      if (split_string(request["get"], ".").second != "ico") {
	code = 404;
      }

      std::string p = path.substr(path.find_last_of("/") + 1);

      if (code == 404 && std::find(files.begin(), files.end(), p) != files.end()) {
	code = 403;
      }

    }

    response(code, whatever, request, directory, buffer);

    int s = send(other, whatever.c_str(), whatever.size(), 0);

    if (s < 0) {
      std::cerr << "Send error\n";
    }

    end = std::chrono::steady_clock::now();

  }

  return 0;

}
