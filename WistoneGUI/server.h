#pragma once

#include <windows.h>

#define DATA_BUF_LEN 500
#define CNSL_MSG_LEN 500
#define PING_MSG_LEN 4

namespace WistoneGUI {

/*****CLIENT RECORD*******************************************************************************/

	struct ClientRecord {
		bool				isConnected;
		SOCKET				socket;
		struct sockaddr_in* address;
		HANDLE				threadHandle;

		ClientRecord() :	isConnected(false), 
							socket(INVALID_SOCKET), 
							address(nullptr), 
							threadHandle(INVALID_HANDLE_VALUE) {}
	};

/*****SERVER**************************************************************************************/

	class Server {
	public:
		void	serverConnect(void);
		void	serverDisconnect(void);
		void	serverReconnect(void); // YL 19.6
		bool	serverCheckConnection(void); // YL 20.6
		void	serverSend(char* command);
				Server(void* thisGUI);
				~Server(void);
	private:
		// methods:
		static void WINAPI	serverThread(LPVOID parameter);
		static void WINAPI	clientThread(LPVOID parameter);
		void				clearClientRecord(void);
		int					sendHelloToClient(int resultReceive);
		int					receiveHelloFromClient(int* resultReceive);
		void				pingTheClient(void);
		void				printMessage(void);
		void				printDebug(void);
		inline const char*	eol(void);	
		// constants: 
		static const short	serverPortNumber			= 5555;
		static const int	serverQueueMax				= 1;
		static const int	connectionRetriesMax		= 5;
		static const int	receiveRetriesMax			= 5;
		static const int	sendRetriesMax				= 3;
		static const int	connectionRetriesMilliSec	= 2000;
		static const int	receiveRetriesMilliSec		= 1000;
		static const int	sendRetriesMilliSec			= 500;
		static const int	clientThreadSleepMilliSec	= 1000;
		// data members:
		SOCKET				serverSocket;
		HANDLE				serverThreadHandle;
		ClientRecord*		clientRecord;
		void*				gui;
		// IO:
		char				incomingData[DATA_BUF_LEN];
		char				outgoingData[DATA_BUF_LEN];
		char				consoleMessage[CNSL_MSG_LEN];
		char				pingMessage[PING_MSG_LEN];
		int					pingLength;
		// utilities:
		int					result;			
	};
}