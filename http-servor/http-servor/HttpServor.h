#pragma once
#include "./TCPServor.h"
#include "./ServorDescritor.h"
#include "./TramGenerator.h"
class HttpServor : public TCPServor
{
public :
	HttpServor();
	virtual void GET(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void HEAD(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void POST(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void OPTIONS(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void CONNECT(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void TRACE(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void PUT(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void PATCH(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;
	virtual void DELETE(sf::TcpSocket * socket, std::string* path, std::string* data) = 0;

	ServorDescritor getServorDescritor();
	void setServorDescritor(ServorDescritor servorDescritor);

	static bool asDataFild(std::string methode);
	std::string generateTram(size_t length);

	int sendData(sf::TcpSocket * socket, std::string* data);
	// Hérité via TCPServor
	virtual void resive(sf::TcpSocket * socket, char* data, size_t size) override;
private:
	ServorDescritor servorDescritor;
};

