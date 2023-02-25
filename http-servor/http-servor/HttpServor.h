#pragma once
#include "./TCPServor.h"
#include "./ServorDescritor.h"
#include "./TramGenerator.h"
class HttpServor : public TCPServor
{
public :
	HttpServor();
	virtual void GET(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void HEAD(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void POST(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void OPTIONS(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void CONNECT(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void TRACE(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void PUT(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void PATCH(win::SOCKET socket, std::string* path, std::string* data) = 0;
	virtual void DELETE(win::SOCKET socket, std::string* path, std::string* data) = 0;

	ServorDescritor getServorDescritor();
	void setServorDescritor(ServorDescritor servorDescritor);

	static bool asDataFild(std::string methode);
	std::string generateTram(size_t length);

	int sendData(win::SOCKET socket, std::string* data);
	// Hérité via TCPServor
	virtual void resive(win::SOCKET socket, char* data, size_t size) override;
private:
	ServorDescritor servorDescritor;
};

