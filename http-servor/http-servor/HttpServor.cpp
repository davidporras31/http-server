#include "HttpServor.h"

HttpServor::HttpServor()
{
}

ServorDescritor HttpServor::getServorDescritor()
{
	return this->servorDescritor;
}

void HttpServor::setServorDescritor(ServorDescritor servorDescritor)
{
	this->servorDescritor = servorDescritor;
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
	return sendTram(this->servorDescritor, length);
}

int HttpServor::sendData(win::SOCKET socket, std::string* data)
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
