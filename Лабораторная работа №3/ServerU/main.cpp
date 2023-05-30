#include <iostream>
#include <conio.h>
#include "Winsock2.h"

#pragma warning(disable:4996)
#pragma comment(lib, "WS2_32.lib")

using namespace std;

string GetErrorMsgText(int errorCode)
{
	string msgText;
	switch (errorCode)
	{
		case WSAEINTR:
			msgText = "WSAEINTR";
			break;
		case WSAEACCES:
			msgText = "WSAEACCES";
			break;
		case WSASYSCALLFAILURE:
			msgText = "WSASYSCALLFAILURE";
			break;
		default:
			msgText = "ERROR";
			break;
	};
	return msgText;
};

string SetErrorMsgText(string msgText, int errorCode)
{
	return msgText + GetErrorMsgText(errorCode);
}

void PrintClientMessage(SOCKADDR_IN clnt, char* ibuf)
{
	cout << "MESSAGE FROM CLIENT\n";
	cout << "-------------------\n";
	cout << "Client: " << inet_ntoa(clnt.sin_addr) << ":" << htons(clnt.sin_port) << endl;
	cout << "Message: " << ibuf << "\n\n";
}

int main()
{
	try
	{
		cout << "Server started...\n\n";

		WSADATA wsaData;
		SOCKET sS;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup error: ", WSAGetLastError());
		}

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket error: ", WSAGetLastError());
		}

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.1");

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Bind error: ", WSAGetLastError());
		}

		SOCKADDR_IN clnt;
		memset(&clnt, 0, sizeof(clnt));
		int lc = sizeof(clnt),
			lb = 0;
		char ibuf[50];

		if ((lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Recvfrom error: ", WSAGetLastError());
		}
		else
		{
			PrintClientMessage(clnt, ibuf);
		}

		int lobuf = 0;

		if ((lobuf = sendto(sS, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Sendto error: ", WSAGetLastError());
		}

		if (closesocket(sS) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Close socket error: ", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Cleanup error: ", WSAGetLastError());
		}

	}
	catch (string errorMsgText)
	{
		cout << "WSAGetLastError:\n\t" << errorMsgText << endl;
	}

	_getch();

	return 0;
}