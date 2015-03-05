#include "stdafx.h"
#include "server.h"
#include "GUI.h"

#include <assert.h>
#include <stdio.h>
#include <vcclr.h> // for gcroot

using namespace WistoneGUI;

typedef enum {
	STARTING,
	RECEIVED_HELLO,
	SENT_HELLO
} ConnectionEstablishmentStage;

/*************************************************************************************************/
void Server::serverConnect(void) {

	serverSocket = INVALID_SOCKET;

	// initialize Windows Socket:
	WSADATA wsaData; // for Winsock initialization info
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		sprintf_s(consoleMessage, CNSL_MSG_LEN, "WSAStartup() failed with error %d %s", WSAGetLastError(), eol());
		printMessage();
		exit(1);
	}

	// create a socket for connecting the server:
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		sprintf_s(consoleMessage, CNSL_MSG_LEN, "socket() failed with error %d %s", WSAGetLastError(), eol());
		printMessage();
		WSACleanup();
		exit(1);
	}

	// set server address info:
	struct sockaddr_in serverAddress = {
		serverAddress.sin_family = AF_INET,
		serverAddress.sin_port = htons(serverPortNumber),
		serverAddress.sin_addr.S_un.S_addr = INADDR_ANY
	};

	// assign the server address to the socket:
	result = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (result == SOCKET_ERROR) {
		sprintf_s(consoleMessage, CNSL_MSG_LEN, "bind() failed with error %d %s", WSAGetLastError(), eol());
		printMessage();
		closesocket(serverSocket);
		WSACleanup();
		exit(1);
	}

	// define max queue size:
	listen(serverSocket, serverQueueMax);

	// create the main thread:
	serverThreadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)serverThread, (LPVOID)this, 0, nullptr);
	sprintf_s(consoleMessage, CNSL_MSG_LEN, "Server Connect - OK %s", eol());
	printMessage();
}

/*************************************************************************************************/
void Server::serverDisconnect(void) {

	// the client:
	assert(clientRecord != nullptr);
	if ((clientRecord->threadHandle != nullptr) && (clientRecord->threadHandle != INVALID_HANDLE_VALUE)) {
		CloseHandle(clientRecord->threadHandle);
		clientRecord->threadHandle = INVALID_HANDLE_VALUE;
	}
	clearClientRecord();

	// the server:
	if ((serverThreadHandle != nullptr) && (serverThreadHandle != INVALID_HANDLE_VALUE)) {
		CloseHandle(serverThreadHandle);
		serverThreadHandle = INVALID_HANDLE_VALUE;
	}
	closesocket(serverSocket);
	WSACleanup();
	memset(incomingData, '\0' ,DATA_BUF_LEN);
	memset(outgoingData, '\0' ,DATA_BUF_LEN);
	gcroot<GUI^>& thisGUI = *((gcroot<GUI^>*)gui);
	thisGUI->setServerConnected(false);
	sprintf_s(consoleMessage, CNSL_MSG_LEN, "Server Disconnect - OK %s", eol());
	printMessage();
}

/*************************************************************************************************/
void Server::serverReconnect(void) {

	serverDisconnect();
	serverConnect();
}

/*************************************************************************************************/
void Server::serverSend(char* command)
{
	assert(clientRecord != nullptr);
	if (clientRecord->isConnected == false) {
		serverReconnect(); // TODO - the message is lost
		return;
	}
	sprintf_s(outgoingData, DATA_BUF_LEN, "%s%s", command, eol());
	int commandLength = strlen(outgoingData);
	result = send(clientRecord->socket, outgoingData, commandLength, 0);
	if (result == SOCKET_ERROR) {
		sprintf_s(consoleMessage, CNSL_MSG_LEN, "serverSend() failed with error %d %s", WSAGetLastError(), eol());
		printDebug(); 
	}
	else {
		gcroot<GUI^>& thisGUI = *((gcroot<GUI^>*)gui);
		thisGUI->resetServerCounter();
		if (result != commandLength) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "serverSend() sent %d out of %d %s", result, commandLength, eol());
			printDebug();
		}
	}
	memset(outgoingData, '\0' ,DATA_BUF_LEN);
}

/*************************************************************************************************/
Server::Server(void* thisGUI) {

	serverSocket = INVALID_SOCKET;
	serverThreadHandle = INVALID_HANDLE_VALUE;
	clientRecord = new ClientRecord();
	gui = thisGUI;
	memset(incomingData,	'\0' ,DATA_BUF_LEN);
	memset(outgoingData,	'\0' ,DATA_BUF_LEN);
	memset(consoleMessage,	'\0' ,CNSL_MSG_LEN);
	memset(pingMessage,		'\0' ,PING_MSG_LEN);
	strcpy_s(pingMessage, "!");
	pingLength = strlen(pingMessage);
}

/*************************************************************************************************/
Server::~Server(void) {

	delete clientRecord;
}

/*************************************************************************************************/
void WINAPI Server::serverThread(LPVOID parameter) {

	Server* s = (Server*)parameter;
	sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "Starting server thread %s", s->eol());
	s->printDebug();
	assert(s->clientRecord != nullptr);
	assert(s->clientRecord->isConnected == false);
	sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "Waiting for a new connection on port %d %s", s->serverPortNumber, s->eol());
	s->printDebug();
	s->clientRecord->socket = accept(s->serverSocket, (sockaddr*)s->clientRecord->address, nullptr);
	if (s->clientRecord->socket == INVALID_SOCKET) {
		sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "accept() failed with error %d %s", WSAGetLastError(), s->eol());
		s->printMessage();
		return;
	}
	sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "accept() - OK %s", s->eol());
	s->printDebug();
	// create a client thread to handle the new connection:
	s->clientRecord->threadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)clientThread, parameter, 0, nullptr);
}

/*************************************************************************************************/
void WINAPI Server::clientThread(LPVOID parameter) {

	int resultReceive = 0;
	int receiveRetries = 0;
	Server* s = (Server*)parameter;
	gcroot<GUI^>& thisGUI = *((gcroot<GUI^>*)s->gui);
	ConnectionEstablishmentStage stage = STARTING; 

	sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "Starting client thread %s", s->eol());
	s->printDebug();
	assert(s->clientRecord->isConnected == false);
	switch (stage) {
		case STARTING:
			s->result = s->receiveHelloFromClient(&resultReceive);
			if (s->result != 0) {
				sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "Too many connection retries, reconnecting the server %s", s->eol());
				s->printMessage();
				s->serverReconnect();
			}
			stage = RECEIVED_HELLO;
		case RECEIVED_HELLO:
			s->result = s->sendHelloToClient(resultReceive);
			if (s->result != 0) {
				s->serverReconnect();
				sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "Too many connection retries, reconnecting the server %s", s->eol());
				s->printMessage();
			}
			stage = SENT_HELLO;
		case SENT_HELLO:
			sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "The connection is active %s", s->eol());
			s->printMessage();
			s->clientRecord->isConnected = true;
			thisGUI->setServerConnected(true);
	}
	while (s->clientRecord->isConnected == true) {
		s->result = recv(s->clientRecord->socket, s->incomingData, DATA_BUF_LEN, 0);
		if (s->result > 0) {	
			sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "recv() received: %s %s", s->incomingData, s->eol());
			s->printDebug();
			receiveRetries = 0;
			if (strcmp(s->incomingData, s->pingMessage) == 0) {
				s->pingTheClient();
			}
		}
		else if (s->result == 0) {
			sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "recv() - closing connection %s", s->eol());
			s->printDebug();
			s->serverReconnect();
		}
		else {
			sprintf_s(s->consoleMessage, CNSL_MSG_LEN, "recv() failed with error %d %s", WSAGetLastError(), s->eol());
			s->printDebug();
			receiveRetries++;
			Sleep(s->receiveRetriesMilliSec);
			if (receiveRetries >= s->receiveRetriesMax) {
				s->serverReconnect();
			}
		}
		memset(s->incomingData, '\0' ,DATA_BUF_LEN);
		thisGUI->resetServerCounter();
		Sleep(clientThreadSleepMilliSec);
	}
}

/*************************************************************************************************/
void Server::clearClientRecord(void) {
	
	assert(clientRecord != nullptr);
	clientRecord->isConnected = false;
	closesocket(clientRecord->socket);
	clientRecord->socket = INVALID_SOCKET;
	clientRecord->address = nullptr;
}

/*************************************************************************************************/
int Server::sendHelloToClient(int resultReceive) {

	int resultSend = 0;
	int sendRetries = 0;

	strcpy_s(outgoingData, incomingData);
	do {
		resultSend = send(clientRecord->socket, outgoingData, resultReceive, 0);
		if (resultSend == SOCKET_ERROR) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "sendHelloToClient() failed with error %d %s", WSAGetLastError(), eol());
			printDebug();
			sendRetries++;
			Sleep(sendRetriesMilliSec);
			continue; 
		}
		if (resultSend != resultReceive) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "sendHelloToClient() sent %d out of %d %s", resultSend, resultReceive, eol());
			printDebug();
			sendRetries++;
			Sleep(sendRetriesMilliSec);
			continue;
		}
		memset(incomingData, '\0' ,DATA_BUF_LEN);
		memset(outgoingData, '\0' ,DATA_BUF_LEN);
		return 0;
	} while (sendRetries < sendRetriesMax);
	memset(incomingData, '\0' ,DATA_BUF_LEN);
	memset(outgoingData, '\0' ,DATA_BUF_LEN);
	return 1;
}

/*************************************************************************************************/
int Server::receiveHelloFromClient(int* resultReceive) {

	int receiveRetries = 0;

	do {
		(*resultReceive) = recv(clientRecord->socket, incomingData, DATA_BUF_LEN, 0);
		if ((*resultReceive) == SOCKET_ERROR) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "receiveHelloFromClient() failed with error %d %s", WSAGetLastError(), eol());
			printDebug();
			receiveRetries++;
			Sleep(receiveRetriesMilliSec);
			continue;
		}
		sprintf_s(consoleMessage, CNSL_MSG_LEN, "receiveHelloFromClient() received: %s %s", incomingData, eol());
		printDebug();
		return 0;
	} while (receiveRetries < receiveRetriesMax);
	return 1;
}

/*************************************************************************************************/
void Server::pingTheClient(void) {

	int sendRetries = 0;
	gcroot<GUI^>& thisGUI = *((gcroot<GUI^>*)gui);

	do {
		result = send(clientRecord->socket, pingMessage, pingLength, 0);
		if (result == SOCKET_ERROR) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "sendPingToClient() failed with error %d %s", WSAGetLastError(), eol());
			printDebug();
			sendRetries++;
			Sleep(sendRetriesMilliSec);
			continue; 
		}
		if (result != pingLength) {
			sprintf_s(consoleMessage, CNSL_MSG_LEN, "sendPingToClient() sent %d out of %d %s", result, pingLength, eol());
			printDebug();
			sendRetries++;
			Sleep(sendRetriesMilliSec);
			continue; 
		}
		thisGUI->resetServerCounter();
		return;
	} while (sendRetries < sendRetriesMax);
	sprintf_s(consoleMessage, CNSL_MSG_LEN, "Too many ping retries, reconnecting the server %s", eol());
	printMessage();
	serverReconnect();
}

/*************************************************************************************************/
void Server::printMessage(void) {
	
	String^ consoleMessageString = gcnew String(consoleMessage);
	gcroot<GUI^>& thisGUI = *((gcroot<GUI^>*)gui);
	thisGUI->SetText(consoleMessageString);
	memset(consoleMessage, '\0' ,CNSL_MSG_LEN); // YL 12.6
}

/*************************************************************************************************/

#define PRINT_DEBUG

void Server::printDebug(void) {

#if defined PRINT_DEBUG
	printMessage();
#endif
}

/*************************************************************************************************/
const char* Server::eol(void) {

	char* endOfLine = "\n";
	return endOfLine;
}