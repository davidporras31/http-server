
#include "HttpServor.h"
class ImpServor : public HttpServor
{
public:

	// Hérité via HttpServor
	virtual void GET(win::SOCKET socket, std::string* path, std::string* data) override
	{
		this->sendData(socket, new std::string("<title>Exemple</title>\n\
<p> Ceci est une page d'exemple.</p>"));
		//win::closesocket(socket);
		win::shutdown(socket, SD_SEND);
	}
	virtual void HEAD(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void POST(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void OPTIONS(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void CONNECT(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void TRACE(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void PUT(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void PATCH(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
	virtual void DELETE(win::SOCKET socket, std::string* path, std::string* data) override
	{
	}
};
int main()
{
	Logger logger;
	logger.log(LoggerGravity::INFO, "application starting");
	ImpServor currentServor;
	currentServor.setServorDescritor(ServorDescritor("HTTPServor","1.0"));
	currentServor.setLogger(&logger);
	currentServor.init();
	currentServor.start();
	logger.log(LoggerGravity::INFO, "application exit");
}
