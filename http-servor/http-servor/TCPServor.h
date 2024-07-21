#pragma once
#ifdef WIN32
#include <basetsd.h>
#endif // WIN32

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/System/Clock.hpp>
#include "extern_lib/logger/SubLogger.h"
#include "TCPServorControler.h"
#include <string>
#include <vector>
#include <thread>


class TCPServor
{
public:
	TCPServor();
	~TCPServor();
	void init();
	void start();

	virtual void resive(sf::TcpSocket * socket, char* data,size_t size) = 0;

	void setLogger(Logger* logger);
	SubLogger* getSublogger();
	TCPServorControler* getControler();

	void setPort(unsigned short number);
	void setBufferSize(size_t size);
	void setTimeout(unsigned int timeout);
	void setWaitTime(unsigned int waitTime);

private:
	void removeConnectedClient(std::vector<std::pair<sf::TcpSocket*, sf::Time>*>* connectedClient, sf::SocketSelector* selector, size_t i);

	void newClientAcceptThread();

	TCPServorControler tCPServorControler;

	SubLogger * subLogger;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<std::pair<sf::TcpSocket*, sf::Time>*> connectedClient;

	sf::Clock serverStart;
	unsigned short port;
	size_t bufferSize;
	unsigned int timeout;
	unsigned int waitTime;
};

