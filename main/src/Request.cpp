#include "Request.hpp"
#include "Pages.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "Game.hpp"

int gameIndex = 0;

std::vector<Game> games = {{}, {}};

std::string responce;

void handleRequest(Request request)
{
	std::string& path = request.path;

	if (hasPage(path))
	{
		responce = getPage(path);
	}
	else if (path[1] == 'd')
	{
		responce = std::to_string(gameIndex++);
		games.push_back({});
		std::cout << responce << '\n';
	}
	else if (path[1] == 'g')
	{
		if (responce.size() < 201)
		{
			responce.resize(201);
		}
		int gameInd = request.arg[0] - '0';
		int p = request.arg.find('%') + 1;
		std::string data = request.arg.substr(p, request.arg.size() - p);
		memcpy(&(games[gameInd].board), data.c_str(), 200);
		memcpy((char*)responce.c_str(), &(games[gameInd].board), 200);
		std::cout << responce << '\n';
	}
	std::string serverMessage = "HTTP/1.1 200 OK\n Content-Type: text/html\nContent-Length: ";
	serverMessage.append(std::to_string(responce.size()));
	serverMessage.append("\n\n");
	serverMessage.append(responce);
	int bytesSent = 0;
	int totalBytesSent = 0;
	while (totalBytesSent < serverMessage.size())
	{
		bytesSent = send(request.socket, serverMessage.c_str(), serverMessage.size(), 0);
		if (bytesSent < 0)
		{
			std::cout << "could not send\n";
			break;
		}
		totalBytesSent += bytesSent;
	}
	closesocket(request.socket);
}
