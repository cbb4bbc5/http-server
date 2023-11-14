#include "http_functions.hpp"

std::string content_type(std::string extension) {
  // based on:
  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
  std::string type = "application/octet-stream";
  
  if (EXTENSIONS.count(extension) != 0)
    type = EXTENSIONS.at(extension);

  return type;
}

std::pair<std::string, std::string> split_string(std::string str, std::string delim) {
  
  //int pos = str.find(delim);
  int pos = str.find(delim);
  std::string first_part = "";
  std::string second_part = "";

  if (pos != -1) {
    first_part = str.substr(0, pos);
    second_part = str.substr(pos + delim.size(), str.size());
  }

  return {first_part, second_part};
}

void parse_request(std::string buf, std::map<std::string, std::string> &request) {
  // maybe store port as well

    int msg_end = buf.find("\r\n\r\n");
    if (msg_end != -1) {

      std::string first_line = buf.substr(0, buf.find("\r\n"));

      if (first_line.substr(0, buf.find(" ")) == "GET") {
	first_line.erase(0, buf.find(" ") + 1);
	request["get"] = first_line.substr(0, first_line.find(" "));
      }

      int connection = buf.find("Connection");

      if (connection != -1) {

	int connection_end = buf.substr(connection).find("\r\n");

	if (connection_end != -1) {
	  request["Connection"] = split_string(buf.substr(connection,
							  connection_end), ": ").second;
	  
	}

      }

      int host = buf.find("Host");

      if (host != -1) {

	int host_end = buf.substr(host).find("\r\n");

	if (host_end != -1) {
	  request["Host"] = split_string(buf.substr(host, host_end), ": ").second;
	}

      }

    }

}

void response(int code, std::string &whatever,
	      const std::map<std::string, std::string> &request,
	      const std::string &directory, std::stringstream &buffer) {

  std::string host = split_string(request.at("Host"), ":").first;
  std::string path = directory + "/" + host + request.at("get");
  std::string con_type;
  std::string message = "";
  int length;
  switch (code) {
    case 301:
      whatever = "HTTP/1.1 301 Moved Permanently\r\n";
      whatever += "Location: http://" + host + ":" + request.at("port") +
	"/index.html\r\n\r\n";
      break;
      
    case 200:
      con_type = content_type(split_string(request.at("get"), ".").second);
      length = process_file(path, buffer);

      whatever = std::string("HTTP/1.1 200 OK\r\n") +
	std::string("Content-Type: " + con_type + "\r\n") +
	std::string("Content-Length: " + std::to_string(length) + "\r\n\r\n");
      whatever += buffer.str();
      break;

    case 404:
      con_type = content_type("html");
      message = "<html><body><h1>Error 404 Not Found</h1></body></html>";
      whatever = "HTTP/1.1 404 Not Found\r\n";
      whatever += "Content-Type: " + con_type + "\r\n";
      whatever += "Content-Length: " + std::to_string(message.size()) + "\r\n\r\n";
      whatever += message;
      break;

    case 403:
      con_type = content_type("html");
      message = "<html><body><h1>Error 403 Forbidden</h1></body></html>";
      whatever = "HTTP/1.1 403 Forbidden\r\n";
      whatever += "Content-Type: " + con_type + "\r\n";
      whatever += "Content-Length: " + std::to_string(message.size()) + "\r\n\r\n";
      whatever += message;
      break;

    case 501:
      con_type = content_type("html");
      message = "<html><body><h1>Error 501 Not Implemented</h1></body></html>";
      whatever = "HTTP/1.1 501 Not Implemented\r\n";
      whatever += "Content-Type: " + con_type + "\r\n";
      whatever += "Content-Length: " + std::to_string(message.size()) + "\r\n\r\n";
      whatever += message;
      break;

  }

}
