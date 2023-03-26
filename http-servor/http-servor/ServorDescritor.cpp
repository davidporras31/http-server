#include "ServorDescritor.h"

ServorDescritor::ServorDescritor(std::string name, std::string version)
{
	this->name = name;
	this->version = version;
}

ServorDescritor::ServorDescritor()
{
	this->name = "name";
	this->version = "version";
}
