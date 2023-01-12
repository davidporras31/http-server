#pragma once
class TCPServorControler
{
public:
	TCPServorControler();
	bool isClosingServor();
	void closeServor();
private:
	bool closing;
};

