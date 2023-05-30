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

void PrintServerMessage(SOCKADDR_IN serv, char* ibuf)
{
	cout << "MESSAGE FROM SERVER\n";
	cout << "-------------------\n";
	cout << "Server: " << inet_ntoa(serv.sin_addr) << ":" << htons(serv.sin_port) << endl;
	cout << "Message: " << ibuf << "\n\n";
}

int main()
{
	try
	{
		cout << "Client started...\n\n";

		WSADATA wsaData;
		SOCKET cC;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup error: ", WSAGetLastError());
		}

		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Socket error: ", WSAGetLastError());
		}

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.1");

		char obuf[50] = "Hello from ClientU!";
		int lobuf = 0;

		if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("recv:", WSAGetLastError());
		}

		int lc = sizeof(serv),
			lb = 0;
		char ibuf[50];
		
		if ((lb = recvfrom(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &lc)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Recvfrom error: ", WSAGetLastError());
		}
		else
		{
			PrintServerMessage(serv, ibuf);
		}

		if (closesocket(cC) == SOCKET_ERROR)
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