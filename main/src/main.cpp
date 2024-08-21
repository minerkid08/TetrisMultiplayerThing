#include "Pages.hpp"
#include "Request.hpp"
#include <iostream>
#include <thread>
#include <winsock2.h>

#include <windows.h>

int main(int argc, const char** argv)
{
	loadPages();

	SOCKET wsocket;
	SOCKET newWsocket;
	WSADATA data;
	struct sockaddr_in server;
	int serverLen;
	int BUFFERSIZE = 30720;
	if (WSAStartup(MAKEWORD(2, 2), &data))
	{
		std::cout << "could not create server\n";
		return 1;
	}

	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (wsocket == INVALID_SOCKET)
	{
		std::cout << "could not create socket\n";
		return 1;
	}

	server.sin_family = AF_INET;
	if (argc > 1)
	{
		server.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else
	{
		server.sin_addr.s_addr = inet_addr("0.0.0.0");
	}
	server.sin_port = htons(8080);
	serverLen = sizeof(server);

	if (bind(wsocket, (SOCKADDR*)&server, serverLen))
	{
		std::cout << "could not bind socket\n";
	}

	if (listen(wsocket, 20))
	{
		std::cout << "could not start listening\n";
	}

	std::cout << "server started\n";

	int bytes = 0;
	while (true)
	{
		newWsocket = accept(wsocket, (SOCKADDR*)&server, &serverLen);
		if (wsocket == INVALID_SOCKET)
		{
			std::cout << "could not accept\n";
		}
		char buffer[30720] = {0};
		bytes = recv(newWsocket, buffer, BUFFERSIZE, 0);
		if (bytes < 0)
		{
			std::cout << "could not read request\n";
			continue;
		}

		buffer[bytes] = '\0';
		std::string request = buffer;

		if (request[0] == 'G')
		{
			request = request.substr(4, request.find('\n') - 4);
		}

		std::string path = request.substr(0, request.find(' '));

		/*SOCKADDR name;
		int size = sizeof(sockaddr);
		ZeroMemory(&name, sizeof(name));
		getpeername(newWsocket, &name, &size);
		std::cout << "sending: ";
		unsigned char a;
		for (int i = 2; i < 6; i++)
		{
			a = name.sa_data[i];
			unsigned int b = a;
			std::cout << b << (i == 5 ? "" : ".");
		}
		std::cout << ", " << name.sa_family << " : " << path << "\n";*/

		std::string arg = path.substr(path.find_last_of('/') + 1);
		path = path.substr(0, path.find_last_of('/'));

		if (path == "")
		{
			path = arg;
		}
		Request r;
		r.arg = arg;
		r.path = path;
		r.socket = newWsocket;
		std::thread t(handleRequest, r);
		t.detach();
	}
	closesocket(wsocket);
	return 0;
}
