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
		SOCKET sS;


		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup error: ", WSAGetLastError());
		}

		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket error: ", WSAGetLastError());
		}

		SOCKADDR_IN serv; // параметры сокета sS
		serv.sin_family = AF_INET; // используется IP-адресация
		serv.sin_port = htons(2000); // порт 2000
		serv.sin_addr.s_addr = inet_addr("127.0.0.1");
		
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Bind error: ", WSAGetLastError());
		}

		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Listen error: ", WSAGetLastError());
		}

		SOCKET cS;
		SOCKADDR_IN clnt;
		memset(&clnt, 0, sizeof(clnt));
		int lclnt = sizeof(clnt);


		if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Accept error: ", WSAGetLastError());
		}
		else
		{
			cout << "Client: " << inet_ntoa(clnt.sin_addr) << ":" << htons(clnt.sin_port) << endl;
		}

		int libuf = 0;
		char ibuf[50];
		int i = 0;

		while (true)
		{
			if (i == 10)
			{
				break;
			}

			if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("Recv error: ", WSAGetLastError());
			}

			cout << "Client message: " << ibuf << " " << i << endl;
			i++;
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

	return 0;
}