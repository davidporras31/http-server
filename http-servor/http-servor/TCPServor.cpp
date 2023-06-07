#include "TCPServor.h"

TCPServor::TCPServor()
{
	this->subLogger = nullptr;
	this->tCPServorControler = TCPServorControler();
	this->port = 80;
	this->bufferSize = 512;
	this->timeout = 500;
	this->waitTime = 50;
}

TCPServor::~TCPServor()
{
	this->subLogger->dump();
}

void TCPServor::init()
{
	this->subLogger->log(LoggerGravity::INFO, "init");

	// Setup the TCP listening socket
	this->subLogger->log(LoggerGravity::INFO, "Setup the TCP listener");
	if (this->listener.listen(this->port) != sf::Socket::Done) {
		this->subLogger->log(LoggerGravity::FATAL, "Setup failed with error");
		throw new std::exception("Setup failed with error");
	}
	this->listener.setBlocking(false);
}

void TCPServor::start()
{
	this->subLogger->log(LoggerGravity::INFO, "Sarting servor");
	sf::SocketSelector selector;
	sf::Time ttimeout = sf::milliseconds(sf::Int64(this->timeout));
	sf::Time twaittime = sf::microseconds(sf::Int64(this->waitTime));
	sf::Clock serverStart;
	std::vector<std::pair<sf::TcpSocket*,sf::Time>*> connectedClient;

	while (!this->tCPServorControler.isClosingServor())
	{
		// Wait for a client to connect
		sf::TcpSocket client;
		switch(listener.accept(client))
		{
		case sf::Socket::NotReady:
			//no
			break;
		case sf::Socket::Error:
			this->getSublogger()->log(ERROR, "Failed to accept connection");
			break;
		case sf::Socket::Done:
			selector.add(client);
			connectedClient.push_back(new std::pair<sf::TcpSocket*, sf::Time>(& client,serverStart.getElapsedTime()));
			break;
		}
		for (size_t i = 0; i < connectedClient.size(); i++)
		{
			if(serverStart.getElapsedTime() < connectedClient.at(i)->second + ttimeout)
			{
				this->removeConnectedClient(&connectedClient, &selector, i);
			}
		}
		if (selector.wait(twaittime))
		{
			for (size_t connectedClientId = 0; connectedClientId < connectedClient.size(); connectedClientId++)
			{
				if (selector.isReady(*connectedClient.at(connectedClientId)->first))
				{
					connectedClient.at(connectedClientId)->second = serverStart.getElapsedTime();
					// Receive until the peer shuts down the connection
					size_t bytes_received;
					//do {
						char* buffer = new char[this->bufferSize];
						size_t curent_size = this->bufferSize;
						sf::Socket::Status clientStatus = connectedClient.at(connectedClientId)->first->receive(buffer, this->bufferSize, bytes_received);
						while (clientStatus == sf::Socket::Partial)
						{
							char* new_buffer = new char[this->bufferSize];
							clientStatus = connectedClient.at(connectedClientId)->first->receive(new_buffer, this->bufferSize, bytes_received);
							size_t temp_size = curent_size;
							curent_size += bytes_received;
							char* temp_buffer = buffer;
							buffer = new char[curent_size];
							strcpy_s(buffer, curent_size, temp_buffer);
							strcat_s(buffer,bytes_received,new_buffer);
							delete []temp_buffer;
							delete []new_buffer;
						}
						switch (clientStatus)
						{
						case sf::Socket::Error:
							// Handle errors
							this->removeConnectedClient(&connectedClient, &selector, connectedClientId);
							this->getSublogger()->log(ERROR, "Failed to receive data from client");
							break;
						case sf::Socket::Disconnected:
							this->removeConnectedClient(&connectedClient, &selector, connectedClientId); 
							break;
						case sf::Socket::Done:
							try {
								this->resive(connectedClient.at(connectedClientId)->first, buffer, bytes_received);
							}
							catch (std::exception e) {
								this->getSublogger()->log(ERROR, "error on resive function:" + std::string(e.what()));
							}
							catch (...) {
								this->getSublogger()->log(ERROR, "unkhnown error on resive function");
							}
							break;
						}
						delete[] buffer;
					//} while (bytes_received > 0 && !this->tCPServorControler.isClosingServor());
				}
			}
		}
	}
}

void TCPServor::setLogger(Logger* logger)
{
	this->subLogger = new SubLogger(logger, "TCPServor");
}

SubLogger* TCPServor::getSublogger()
{
	return this->subLogger;
}

TCPServorControler* TCPServor::getControler()
{
	return &this->tCPServorControler;
}

void TCPServor::setPort(unsigned short number)
{
	this->port = number;
}

void TCPServor::setBufferSize(size_t size)
{
	this->bufferSize = size;
}

void TCPServor::setTimeout(unsigned int timeout)
{
	this->timeout = timeout;
}

void TCPServor::setWaitTime(unsigned int waitTime)
{
	this->waitTime = waitTime;
}

void TCPServor::removeConnectedClient(std::vector<std::pair<sf::TcpSocket*, sf::Time>*>* connectedClient, sf::SocketSelector* selector, size_t i)
{
	connectedClient->at(i)->first->disconnect();
	selector->remove(*connectedClient->at(i)->first);
	delete connectedClient->at(i);
	connectedClient->erase(connectedClient->begin() + i);
}
