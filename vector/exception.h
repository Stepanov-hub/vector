#pragma once

#include <string>

class exception
{
private:
	std::string str;
public:
	exception() :str{"unknown exception"} {};
	exception(std::string& r) :str{r} {};
	exception(std::string&& r) :str{r} {};
	std::string what() {
		return str;
	}
};




