#include "Request.hpp"
#include "Pages.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "Game.hpp"

int gameIndex = 0;

std::vector<Game> games;

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
		bool bk = false;
		for (int i = 0; i < games.size(); i++)
		{
			if (games[i].active == false)
			{
				responce = std::to_string(i);
				games[i].active = true;
				bk = true;
				break;
			}
		}
		if (!bk)
		{
			responce = std::to_string(gameIndex++);
			games.push_back({});
		}
		std::cout << "new game " << responce << '\n';
	}
	else if (path[1] == 'c')
	{
		games[std::stoi(request.arg)].active = false;
		std::cout << "close game " << request.arg << '\n';
	}
	else if (path[1] == 'g')
	{
		int gameInd = request.arg[0] - '0';
		int p = request.arg.find('%') + 1;
		games[gameInd].board = request.arg.substr(p, request.arg.size() - p);
		responce = "0";
		int gameCount = 0;
		for (int i = 0; i < games.size(); i++)
		{
			if (i != gameInd && games[i].active)
			{
				gameCount++;
				responce += games[i].board;
			}
		}
		responce[0] = gameCount + '0';
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
