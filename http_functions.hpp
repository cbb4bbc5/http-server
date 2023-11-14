#ifndef HTTP_FUNCTIONS_H_
#define HTTP_FUNCTIONS_H_

#include <map>
#include <string>
#include <stdexcept>
#include <utility>
#include <sstream>
#include <iostream>

#include <sys/socket.h>

#include "system_functions.hpp"

std::string content_type(std::string extension);
std::pair<std::string, std::string> split_string(std::string str, std::string delim);
void parse_request(std::string buf, std::map<std::string, std::string> &request);
void response(int code, std::string &whatever,
	      const std::map<std::string, std::string> &request,
	      const std::string &directory, std::stringstream &buffer);

const std::string DEFAULT_PAGE = "index.html";

const std::map<std::string, std::string> EXTENSIONS = {
  // assume that text files are in utf8 and we do not have to check that
  {"txt", "text/plain; charset=utf-8"},
  {"html", "text/html; charset=utf-8"},
  {"css", "text/css; charset=utf-8"},
  {"min.css", "text/css; charset=utf-8"},
  {"jpeg", "image/jpeg"},
  {"jpg", "image/jpeg"},
  {"png", "image/png"},
  {"pdf","application/pdf"},
};

#endif // HTTP_FUNCTIONS_H_
