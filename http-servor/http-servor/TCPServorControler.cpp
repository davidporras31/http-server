#include "TCPServorControler.h"

TCPServorControler::TCPServorControler()
{
	this->closing = false;
}

bool TCPServorControler::isClosingServor()
{
	return this->closing;
}

void TCPServorControler::closeServor()
{
	this->closing = true;
}
