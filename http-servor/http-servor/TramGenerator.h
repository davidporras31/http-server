#pragma once
#include <string>
#include <time.h>
#include <iomanip>
#include <sstream>

#include "ServorDescritor.h"

std::string sendTram(ServorDescritor servorDescritor, size_t length);
std::string redirectTram(ServorDescritor servorDescritor, std::string path);


