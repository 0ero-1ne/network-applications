#include <iostream>
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

int main()
{
	try
	{
		WSADATA wsaData;
		SOCKET cC;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup error: ", WSAGetLastError());
		}

		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Socket error: ", WSAGetLastError());
		}

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.1");

		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Connect error:", WSAGetLastError());
		}

		char obuf[50] = "Hello Server! I am client :) ";
		int lobuf = 0;

		for (int i = 0; i < 10; i++)
		{
			if ((lobuf = send(cC, obuf + i, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("Send error: ", WSAGetLastError());
			}
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

	system("pause");

	return 0;
}