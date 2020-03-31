// winsocket_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{

	// iniitialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "can't initialize winsock! quitting" << endl;
		return -1;
	}

	// create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "can't create a socket! quitting" << endl;
	}

	// bind the IP address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); // host to network short (htons)
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// wait for connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << " invalid client socket! quitting" << endl;
		return -1;
	}
	char host[NI_MAXHOST];    // client's remote name
	char service[NI_MAXHOST]; // service (i.e., port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);    // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port " << ntohs(client.sin_port) << endl;
	}

	// close listening socket
	closesocket(listening);

	// while loop : accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		/*		if (bytesReceived == SOCKET_ERROR)
				{
					cerr << "Error in recv(). quitting" << endl;
					return -1;
				}*/
		if (bytesReceived == 0)
		{
			cout << "client disconnected " << endl;
		}

		// echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);

	}

	// close the socket
	closesocket(clientSocket);

	// Shutdown winsock
	WSACleanup();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
