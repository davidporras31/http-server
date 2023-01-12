#pragma once
#include "extern_lib/logger/include/SubLogger.h"
#include "TCPServorControler.h"
#if defined(_WIN64) || defined(_WIN32)
namespace win {
#include <winsock2.h>
#include <ws2tcpip.h>
}
#undef INVALID_SOCKET
#define INVALID_SOCKET		(win::SOCKET)(~0)
#undef MAKEWORD
#define MAKEWORD(a, b)      ((win::WORD)(((win::BYTE)(((win::DWORD_PTR)(a)) & 0xff)) | ((win::WORD)((win::BYTE)(((win::DWORD_PTR)(b)) & 0xff))) << 8))
#include <stdio.h>
#undef ERROR
#undef DELETE

#pragma comment(lib, "Ws2_32.lib")
#endif // Windows


class TCPServor
{
public:
	TCPServor();
	void init();
	void start();

	int send(win::SOCKET socket, char* data, std::size_t size);
	virtual void resive(win::SOCKET socket, char* data,size_t size) = 0;

	void setLogger(Logger* logger);
	SubLogger* getSublogger();
	TCPServorControler* getControler();

private:
	TCPServorControler tCPServorControler;
	win::WSADATA wsaData;
	SubLogger * subLogger;
	win::SOCKET ListenSocket;
};

