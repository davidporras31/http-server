#include "HttpServor.h"

HttpServor::HttpServor()
{
}

bool HttpServor::asDataFild(std::string methode)
{
	bool ret = false;
	if (methode == "POST") {
		ret = true;
	}
	else if (methode == "OPTIONS") {
		ret = true;
	}
	/*else if (methode == "CONNECT") {
		ret = true;
	}*/
	else if (methode == "TRACE") {
		ret = true;
	}
	else if (methode == "PUT") {
		ret = true;
	}
	else if (methode == "PATCH") {
		ret = true;
	}
	return ret;
}

std::string HttpServor::generateTram(size_t length)
{
	auto format = "%D, %d %M %a %h : %m : %s";
	std::time_t now = std::time(0);
	std::tm gmt;
	gmtime_s(&gmt,&now);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &gmt);
	std::string snow(buffer);

	return std::string() + "HTTP / 1.0 200 OK\n" +
		"Date : " + snow + " GMT\n" +
		"Server : " + ServerName + " / " + Version + "\n" +
		"Content - Type : text / html\n" +
		"Content - Length : " + std::to_string(length) + "\n" +
		"Expires : Sat, 01 Jan 2000 00 : 59 : 59 GMT\n" +
		"Last - modified : Fri, 09 Aug 1996 14:21 : 40 GMT\n" +
		"\n";
}

int HttpServor::send(win::SOCKET socket, std::string* data)
{
	std::string tram = generateTram(data->length()) + *data;
	char* cdata = new char[tram.length() + 1];
	strcpy_s(cdata, sizeof(char) * (tram.length() + 1), tram.c_str());
	return TCPServor::send(socket, cdata, tram.length());
}

void HttpServor::resive(win::SOCKET socket, char* data, size_t size)
{
	std::string val(data,size);
	std::string methode = val.substr(0, val.find(" "));
	std::string path = val.substr(methode.length() + 1, val.length()).substr(0, val.find(" ") - 2);
	std::string sdata;

	if (asDataFild(methode)) {
		sdata = val.substr(val.find(" \n"), val.length());
	}
	if (methode == "GET") {
		this->GET(socket, &path, &sdata);
	}else if (methode == "HEAD") {
		this->HEAD(socket, &path, &sdata);
	}else if (methode == "POST") {
		this->POST(socket, &path, &sdata);
	}else if (methode == "OPTIONS") {
		this->OPTIONS(socket, &path, &sdata);
	}else if (methode == "CONNECT") {
		this->CONNECT(socket, &path, &sdata);
	}else if (methode == "TRACE") {
		this->TRACE(socket, &path, &sdata);
	}else if (methode == "PUT") {
		this->PUT(socket, &path, &sdata);
	}else if (methode == "PATCH") {
		this->PATCH(socket, &path, &sdata);
	}else if (methode == "DELETE") {
		this->DELETE(socket, &path, &sdata);
	}
}
