#include "TCPServor.h"

TCPServor::TCPServor()
{
	this->wsaData =  win::WSADATA();
	this->ListenSocket = INVALID_SOCKET;
	this->subLogger = nullptr;
	this->tCPServorControler = TCPServorControler();
	this->port = 80;
	this->bufferSize = 512;
}

TCPServor::~TCPServor()
{
	this->subLogger->dump();
}

void TCPServor::init()
{
	this->subLogger->log(LoggerGravity::INFO, "init");
	int iResult;

	// Initialize Winsock
	this->subLogger->log(LoggerGravity::INFO, "WSAStartup");
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		this->subLogger->log(LoggerGravity::FATAL,"WSAStartup failed");
		throw new std::exception("WSAStartup failed");
	}
	// Initialize Addrinfo
	this->subLogger->log(LoggerGravity::INFO, "get local address");

	struct win::addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = win::IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = win::getaddrinfo(NULL, std::to_string(this->port).data(), &hints, &result);
	if (iResult != 0) {
		this->subLogger->log(LoggerGravity::FATAL, "getaddrinfo failed");
		win::WSACleanup();
		throw new std::exception("getaddrinfo failed");
	}
	// Create Socket
	this->subLogger->log(LoggerGravity::INFO, "Create Socket");

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = win::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		this->subLogger->log(LoggerGravity::FATAL, "Error Creating Socket");
		win::freeaddrinfo(result);
		win::WSACleanup();
		throw new std::exception("Error Creating Socket");
	}

	// Setup the TCP listening socket
	this->subLogger->log(LoggerGravity::INFO, "Setup the TCP listening socket");
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		this->subLogger->log(LoggerGravity::FATAL, "bind failed with error");
		win::freeaddrinfo(result);
		win::closesocket(ListenSocket);
		win::WSACleanup();
		throw new std::exception("bind failed with error");
	}

	win::freeaddrinfo(result);

	//Listening socket
	this->subLogger->log(LoggerGravity::INFO, "listening socket");
	if (win::listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		this->subLogger->log(LoggerGravity::FATAL, "Listen failed with error");
		win::closesocket(ListenSocket);
		win::WSACleanup();
		throw new std::exception("Listen failed with error");
	}
}

void TCPServor::start()
{
	this->subLogger->log(LoggerGravity::INFO, "Sarting servor");
	win::SOCKET ClientSocket;
	win::SOCKADDR csin;
	while (!this->tCPServorControler.isClosingServor())
	{
		int sinsize = sizeof(csin);
		if ((ClientSocket = win::accept(ListenSocket, (win::SOCKADDR*)&csin, &sinsize)) != INVALID_SOCKET)
		{
			char * recvbuf = new char[this->bufferSize];
			int iResult;// , iSendResult;
			int recvbuflen = int(this->bufferSize);

			// Receive until the peer shuts down the connection
			do {

				iResult = win::recv(ClientSocket, recvbuf, recvbuflen, 0);
				/*if (win::WSAGetLastError() == WSAEMSGSIZE) {
					this->subLogger->log(LoggerGravity::ERROR, "packet overflow");
				}*/
				
				if (iResult > 0) {
					this->resive(ClientSocket, recvbuf, iResult);
				}
				else if (iResult == 0)
				{ }//this->subLogger->log(LoggerGravity::INFO, "Connection closing...");
				else {
					this->subLogger->log(LoggerGravity::ERROR, "recv failed");
					win::closesocket(ClientSocket);
					win::WSACleanup();
					return ;
				}

			} while (iResult > 0 && !this->tCPServorControler.isClosingServor());
		}
	}
	win::closesocket(ListenSocket);
	win::WSACleanup();
}

int TCPServor::send(win::SOCKET socket, char* data, std::size_t size)
{
	return win::send(socket,(const char *)data,int(size),0);
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

void TCPServor::setPort(uint16_t number)
{
	this->port = number;
}

void TCPServor::setBufferSize(size_t size)
{
	this->bufferSize = size;
}
