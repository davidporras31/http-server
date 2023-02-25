#include "TramGenerator.h"
std::string sendTram(ServorDescritor servorDescritor, size_t length)
{
	auto format = "%D, %d %M %a %h : %m : %s";
	std::time_t now = std::time(0);
	std::tm gmt;
	gmtime_s(&gmt, &now);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &gmt);
	std::string snow(buffer);

	return std::string() + "HTTP / 1.0 200 OK\n" +
		"Date : " + snow + " GMT\n" +
		"Server : " + servorDescritor.name + " / " + servorDescritor.version + "\n" +
		"Content - Type : text / html\n" + //make it variabe or erase
		"Content - Length : " + std::to_string(length) + "\n" +
		//"Expires : Sat, 01 Jan 2030 00 : 59 : 59 GMT\n" +
		//"Last - modified : Fri, 09 Aug 1996 14:21 : 40 GMT\n" +
		"\n";
}

std::string redirectTram(ServorDescritor servorDescritor, std::string path)
{
	return std::string() + "HTTP/1.0 301 Moved Permanently\n" +
		"Location: " + path;
}
