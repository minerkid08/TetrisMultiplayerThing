#pragma once
#include <string>
#include <windows.h>

struct Request{
	std::string path;
	std::string arg;
	SOCKET socket;
};

void handleRequest(Request request);
