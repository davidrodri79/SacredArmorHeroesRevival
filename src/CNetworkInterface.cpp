
#include "platform.h"

#if defined SAHWINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <crtdbg.h>

#include <winsock2.h>   // the networking library
#pragma comment(lib, "ws2_32.lib")      // links WinSock2 with MS Visual Studio
#elif defined SAHLINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <errno.h>
#define SOCKET_ERROR -1
#endif
//#include <vector>       // for the server's client list and input buffer
#include <stdio.h>

#include<allegro5/allegro.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


#include "CNetworkInterface.hpp"



#define NETWORK_LOG
//#define NETWORK_LOG_VERBOSE

#define DISCONNECT_TIMEOUT 30.0f//10.0f

float getCurrentTime()
{
	return al_get_time();
}

#if defined SAHWINDOWS
int getReadStatus(const SOCKET a_socket)
#elif defined SAHLINUX
int getReadStatus(const int a_socket)
#endif
{
        // zero seconds, zero milliseconds. max time select call allowd to block
        static const timeval instantSpeedPlease = {0,0};
#if defined SAHWINDOWS
        fd_set a = {1, {a_socket}};
#elif defined SAHLINUX
        fd_set a;
        FD_ZERO(&a);
        FD_SET(a_socket, &a);
#endif


/*
select returns the number of ready socket handles in the fd_set structure, zero if the time limit expired, or SOCKET_ERROR if an error occurred. WSAGetLastError can be used to retrieve a specific error code.
*/
        int result = select(a_socket+1,&a,0,0,&instantSpeedPlease);
        if(result == SOCKET_ERROR)
        {
#if defined SAHWINDOWS
                result = WSAGetLastError();
#elif defined SAHLINUX
                result = errno;
#endif
        }
        if(result != 0 && result != 1)
        {
#ifdef NETWORK_LOG
                fprintf(stderr,"select() error %d\n", result);
#endif
                return SOCKET_ERROR;
        }
        return result;
}

#if defined SAHWINDOWS
int endSocket(SOCKET socket)
#elif defined SAHLINUX
int endSocket(int socket)
#endif
{
#if defined SAHWINDOWS
        int result = shutdown(socket, SD_BOTH);
#elif defined SAHLINUX
        int result = shutdown(socket, SHUT_RDWR);
#endif
        if (result != 0)
        {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                fprintf(stderr,"socket shutdown() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
                fprintf(stderr,"socket shutdown() error %d\n", errno);
#endif
#endif
        }
#if defined SAHWINDOWS
        result = closesocket(socket);
#elif defined SAHLINUX
        result = close(socket);
#endif
        if (result != 0)
        {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                fprintf(stderr,"socket closesocket() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
                fprintf(stderr,"socket closesocket() error %d\n", errno);
#endif
#endif
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
}

#if defined SAHWINDOWS
int getWriteStatus(const SOCKET a_socket)
#elif defined SAHLINUX
int getWriteStatus(const int a_socket)
#endif
{
        // zero seconds, zero milliseconds. max time select call allowd to block
        static const timeval instantSpeedPlease = {0,0};
#if defined SAHWINDOWS
        fd_set a = {1, {a_socket}};
#elif defined SAHLINUX
        fd_set a;
        FD_ZERO(&a);
        FD_SET(a_socket,&a);
#endif
/*
select returns the number of ready socket handles in the fd_set structure, zero if the time limit expired, or SOCKET_ERROR if an error occurred. WSAGetLastError can be used to retrieve a specific error code.
*/
        int result = select(a_socket+1,0,&a,0,&instantSpeedPlease);
        if(result == SOCKET_ERROR)
        {
#if defined SAHWINDOWS
                result = WSAGetLastError();
#elif defined SAHLINUX
                result = errno;
#endif
        }
        if(result != 0 && result != 1)
        {
#ifdef NETWORK_LOG
                fprintf(stderr,"select() error %d\n", result);
#endif
                return SOCKET_ERROR;
        }
        return result;
}

bool CNetworkInterface::createSocket()
{
	int result;
#if defined SAHWINDOWS
    WSADATA wsaData; // gets populated w/ info explaining this sockets implementation
    // load Winsock 2.0 DLL. initiates use of the Winsock DLL by a process
    if ((result = WSAStartup(MAKEWORD(1, 1),&wsaData)) != 0)
    {
#ifdef NETWORK_LOG
            fprintf(stderr,"WSAStartup() error %d\n", result);
#endif
            return false;
    }
#endif

	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 1 to set non-blocking, 0 to set re-usable
    unsigned long argp = 1;

    result = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&argp, sizeof(argp));
    if (result != 0)
    {
#ifdef NETWORK_LOG
            fprintf(stderr,"setsockopt() error %d\n", result);
#endif
            return false;
    }

	// 1 to set non-blocking, 0 to set blocking
    argp = 1;
    // attempt to setup the socket as non-blocking
#if defined SAHWINDOWS
    if(ioctlsocket(mSocket, FIONBIO, &argp) == SOCKET_ERROR)
#elif defined SAHLINUX
    //if(ioctl(mSocket, FIONBIO, &argp) < 0)
    if(fcntl(mSocket, F_SETFL, O_NONBLOCK) < 0)
#endif
    {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                fprintf(stderr,"ioctlsocket() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
                fprintf(stderr,"fcntl() error %d\n", errno);
#endif
#endif
            return false;
    }

	return true;
}

bool CNetworkInterface::end()
{
#if defined SAHWINDOWS
	int result = WSACleanup();
    if (result != 0)
    {
#ifdef NETWORK_LOG
         fprintf(stderr,"WSACleanup() error %d\n", result);
#endif
		 return false;
    }
#endif
	return true;
}

CNetworkServer::CNetworkServer()
{
}

CNetworkServer::~CNetworkServer()
{
	end();
}

bool CNetworkServer::init(unsigned short port)
{
	mNextLobbyId = 100;

	if (!createSocket()) return false;

	sockaddr_in connectionAddress;
    unsigned short myPort = port;
    // initialize the socket's address
    memset(&connectionAddress, 0, sizeof(sockaddr_in)); // initialize to zero
    connectionAddress.sin_family = AF_INET;
    // host-to-network-short: big-endian conversion of a 16 byte value
    connectionAddress.sin_port = htons(myPort);
    connectionAddress.sin_addr.s_addr = INADDR_ANY;

	// start listening on the server
    int result = bind(mSocket, (sockaddr*)&connectionAddress, sizeof(sockaddr_in));
    if (result == SOCKET_ERROR)
    {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
            fprintf(stderr,"socket bind() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
            fprintf(stderr,"socket bind() error %d\n", errno);
#endif
#endif
            return false;
    }
    result = listen(mSocket, /* size of connection queue */10);
    if (result == SOCKET_ERROR)
    {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
            fprintf(stderr,"listen() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
            fprintf(stderr,"listen() error %d\n", errno);
#endif
#endif
            return false;
    }

	// Get Local IP Adress
#if defined SAHWINDOWS
	HOSTENT *hp=NULL;
	char hostname[128];

	gethostname(hostname, 128);
	hp = gethostbyname(hostname);

	if(hp == NULL)
	{
#ifdef NETWORK_LOG
		fprintf(stderr,"Local gethostbyname failed: %d\n", WSAGetLastError());
#endif
		return 0;
	}

	memcpy(&(hostnameAddr.sin_addr),hp->h_addr,hp->h_length);
	hostnameAddr.sin_family = hp->h_addrtype;
#elif defined SAHLINUX

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[32];

    if (getifaddrs(&ifaddr) == -1)
    {
        fprintf(stderr,"getifaddrs");
        return true;
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        bool found = false;
        if (ifa->ifa_addr == NULL)
            continue;

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, 32, NULL, 0, NI_NUMERICHOST);

        //fprintf(stderr, "ifa->ifa_name: %s\n", ifa->ifa_name );

        if(/*(strcmp(ifa->ifa_name,"wlan0")==0)&&*/(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
                //exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr,"\tInterface : <%s>\n",ifa->ifa_name );
                fprintf(stderr,"\t  Address : <%s>\n", host);
                if(strncmp(host,"127",3) != 0 && !found)
                {
                    inet_aton(host, &hostnameAddr.sin_addr);
                    hostnameAddr.sin_family = ifa->ifa_addr->sa_family;
                    found = true;
                }
            }
        }
    }

#endif

	return true;
}

char* CNetworkServer::getServerIPAdress()
{
	return inet_ntoa(hostnameAddr.sin_addr);
}

void CNetworkServer::acceptClients()
{
	int result;
	result = getReadStatus(mSocket);
	if (result != SOCKET_ERROR)
	{
		if(result == 1)
        {
#ifdef NETWORK_LOG
                fprintf(stderr,"connecting client...");
#endif
                int client_size = sizeof(sockaddr_in);
                TClientInfo lClient;
				lClient.mSocket = accept(mSocket, (struct sockaddr*)&lClient.mAddress, &client_size);
#if defined SAHWINDOWS
                if(lClient.mSocket == INVALID_SOCKET)
#elif defined SAHLINUX
                if(lClient.mSocket < 0)
#endif
                {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                    fprintf(stderr,"accept() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
                    fprintf(stderr,"accept() error %d\n", errno);
#endif
#endif
                        return;
                }
				lClient.mLastAliveTime = getCurrentTime();
                mClients.push_back(lClient);

#if defined SAHWINDOWS
				sockaddr_in *lAddrIn = (sockaddr_in*)&lClient.mAddress;
				char *ip = inet_ntoa(lAddrIn->sin_addr);
#elif defined SAHLINUX
                struct sockaddr_in *lAddrIn = (sockaddr_in*)&lClient.mAddress;
                socklen_t len;

                len = sizeof lAddrIn;
                getpeername(lClient.mSocket, (struct sockaddr*)lAddrIn, &len);
                char *ip = inet_ntoa(lAddrIn->sin_addr);
#endif
#ifdef NETWORK_LOG
                fprintf(stderr,"\rconnected client %d with IP %s\n", mClients.size()-1, ip);
#endif
        }
	}
}

void CNetworkServer::stopAccepting()
{
#if defined SAHWINDOWS
	closesocket(mSocket);
#elif defined SAHLINUX
    close(mSocket);
#endif
}

bool CNetworkServer::isClientConnected(tbyte aLobbyId)
{
	for(int i = mClients.size() - 1; i >= 0; i--)
	{
		if(mClients[i].mLobbyId == aLobbyId)
			return true;
	}
	return false;
}

char* CNetworkServer::getClientIPAdress(tbyte aLobbyId)
{

	for(int i = 0; i < mClients.size(); i++)
	{
		if(mClients[i].mLobbyId == aLobbyId)
		{
			sockaddr_in *lAddrIn = (sockaddr_in*)&mClients[i].mAddress;
			char *ip = inet_ntoa(lAddrIn->sin_addr);
			return ip;
		}
	}
}

void CNetworkServer::assignPlayerMaskToClient(tbyte aLobbyId, tbyte aMask)
{
	for(int i = 0; i < mClients.size(); i++)
	{
		if(mClients[i].mLobbyId == aLobbyId)
		{
			mClients[i].mPlayerMask = aMask;
			return;
		}
	}
#ifdef NETWORK_LOG
	fprintf(stderr,"CNetworkServer::Error: Can't find player with lobbyId %d!!!\n", aLobbyId);
#endif
}

void CNetworkServer::receiveMessages(CMessageQueue *aQueue)
{
	char *lBuffer;

	for(unsigned int i = 0; i < mClients.size(); ++i)
	{
        int status = getReadStatus(mClients[i].mSocket);
        if(status == 1)
        {
#if defined SAHWINDOWS
                if(mClients[i].mSocket == INVALID_SOCKET)
#elif defined SAHLINUX
                if(mClients[i].mSocket < 0)
#endif
                {
                        continue;
                }

                unsigned long howMuchInBuffer = 0;
#if defined SAHWINDOWS
                ioctlsocket(mClients[i].mSocket, FIONREAD, &howMuchInBuffer);
#elif defined SAHLINUX
                ioctl(mClients[i].mSocket, FIONREAD, &howMuchInBuffer);
#endif
#ifdef NETWORK_LOG_VERBOSE
                fprintf(stderr, "ioctl(): Must read %d bytes\n", howMuchInBuffer);
#endif
                if(howMuchInBuffer > 0)
                {
						mClients[i].mLastAliveTime = getCurrentTime();

						lBuffer = DBG_NEW char[howMuchInBuffer];

						//fprintf(stderr,"Message received from %d: must read %d bytes\n",i,howMuchInBuffer);
                        int result = recv(mClients[i].mSocket, lBuffer, howMuchInBuffer,0);
                        // NOTE: for multi-byte values sent across the network
                        // ntohl or ntohs should be used to convert 4-byte and
                        // 2-byte values respectively.
                        if(result == SOCKET_ERROR)
                        {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                                fprintf(stderr, "server %d recv() error %d\n", i, WSAGetLastError());
#elif defined SAHLINUX
                                fprintf(stderr, "server %d recv() error %d\n", i, errno);
#endif
#endif
								delete lBuffer;
                                return;
                        }


#ifdef NETWORK_LOG_VERBOSE
                        fprintf(stderr, "Received %d bytes\n", result);
                        for(int x=0; x < result; x++)
                        {
                            fprintf(stderr,"%d,",(int)lBuffer[x]);
                        }
                        fprintf(stderr,"\n");
#endif
						char *lReadPtr = lBuffer;
						int lProcessed = 0;

						while(lProcessed < howMuchInBuffer)
						{
							TMessage *lMsg = 0;
#if defined SAHWINDOWS
							EMessage lType = (EMessage)lReadPtr[4];
#elif defined SAHLINUX
                            EMessage lType = (EMessage)lReadPtr[8];
#endif

							switch(lType)
							{
								case eMsgProfile	: lMsg = DBG_NEW TMsgProfile; break;
								case eMsgPlUpdate	: lMsg = DBG_NEW TMsgPlUpdate; break;
								case eMsgPlSound	: lMsg = DBG_NEW TMsgPlSound; break;
								case eMsgOpenDoor	: lMsg = DBG_NEW TMsgOpenDoor; break;
								case eMsgConsoleLog	: lMsg = DBG_NEW TMsgConsoleLog; break;
								case eMsgRequestItem: lMsg = DBG_NEW TMsgRequestItem; break;
								case eMsgRemoveItem : lMsg = DBG_NEW TMsgRemoveItem; break;
								case eMsgConcealItem: lMsg = DBG_NEW TMsgConcealItem; break;
								case eMsgDamage		: lMsg = DBG_NEW TMsgDamage; break;
								case eMsgNewBullet  : lMsg = DBG_NEW TMsgNewBullet; break;
								case eMsgEndBullet  : lMsg = DBG_NEW TMsgEndBullet; break;
								case eMsgNewExplos  : lMsg = DBG_NEW TMsgNewExplos; break;
								case eMsgNewBarrier : lMsg = DBG_NEW TMsgNewBarrier; break;
								case eMsgNewItem	: lMsg = DBG_NEW TMsgNewItem; break;
								case eMsgTurnSwitch : lMsg = DBG_NEW TMsgTurnSwitch; break;
								case eMsgTalkToChar : lMsg = DBG_NEW TMsgTalkToChar; break;
								case eMsgIncreaseKills : lMsg = DBG_NEW TMsgIncreaseKills; break;
								case eMsgEndMatch	: lMsg = DBG_NEW TMsgEndMatch; break;
								case eMsgKeepAlive	: lMsg = DBG_NEW TMsgKeepAlive; break;
								case eMsgPlDisconnect: lMsg = DBG_NEW TMsgPlDisconnect; break;
							}

							if(lMsg)
							{
#if defined SAHWINDOWS
								memcpy( (char*)lMsg + 4, (char*)lReadPtr + 4, lMsg->size() - 4 );
#elif defined SAHLINUX
                                memcpy( (char*)lMsg + 8, (char*)lReadPtr + 8, lMsg->size() - 8 );
#endif
								if(lType == eMsgProfile)
								{
									mClients[i].mLobbyId = mNextLobbyId;
									TMsgProfile *lProf = (TMsgProfile*)lMsg;
									lProf->mLobbyId = mNextLobbyId;
									mNextLobbyId++;
								}
							}

#ifdef NETWORK_LOG
							fprintf( stderr, "Mensaje del cliente %d: Tipo %s\n", i, lMsg->typeName());
#endif

							aQueue->queueMessage(lMsg);

							lReadPtr += lMsg->size();
							lProcessed += lMsg->size();
						};

						delete lBuffer;

                }
        }
		else if (status == SOCKET_ERROR)
		{
			endSocket(mClients[i].mSocket);
			mClients.erase(mClients.begin()+i);
			i--;
		}

		if(getCurrentTime()  > mClients[i].mLastAliveTime + DISCONNECT_TIMEOUT)
		{
#ifdef NETWORK_LOG
			fprintf(stderr, "Client %s disconnected for timeout.\n", getClientIPAdress(mClients[i].mLobbyId));
#endif
			endSocket(mClients[i].mSocket);
			mClients.erase(mClients.begin()+i);
			i--;
		}
	}
}

void CNetworkServer::sendMessages(CMessageQueue *aQueue)
{
	int result;

	char buffer[512];

	TMessage *lMessage;

	while((lMessage = aQueue->extractFirst()) != 0)
	{
		unsigned int lSize = lMessage->size();

		memcpy(buffer, lMessage, lMessage->size());

		for(unsigned int i = 0; i < mClients.size(); ++i)
		{
#if defined SAHWINDOWS
			if(mClients[i].mSocket == INVALID_SOCKET)
#elif defined SAHLINUX
            if(mClients[i].mSocket < 0)
#endif
			{
				continue;
			}

			// Do not send messages if is not intended to receive it, nor if he is the sender
			if((!(mClients[i].mPlayerMask & lMessage->mReceiver)) || (mClients[i].mPlayerMask & lMessage->mSender))
			{
				continue;
			}

			result = send(mClients[i].mSocket, buffer, lMessage->size(),0);
			if(result == SOCKET_ERROR) // if the socket failed
			{
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
					fprintf(stderr,"\rserver send error %d\n",
							WSAGetLastError());
#elif defined SAHLINUX
                    fprintf(stderr,"\rserver send error\n");
#endif
#endif

					endSocket(mClients[i].mSocket);
					mClients.erase(mClients.begin()+i);
					i--;
			}
			/*else
				fprintf(stderr,"Message sent!!  %d bytes\n", lSize);*/
		}

		delete lMessage;
	}
}

CNetworkClient::CNetworkClient()
	: mConnected(false)
{
}

CNetworkClient::~CNetworkClient()
{
	end();
}

bool CNetworkClient::init()
{
	if (!createSocket()) return false;

	return true;
}

bool CNetworkClient::connectToServer(char *targetIP, unsigned short port)
{
	sockaddr_in connectionAddress;
    unsigned short myPort = port;
    // initialize the socket's address
    memset(&connectionAddress, 0, sizeof(sockaddr_in)); // initialize to zero
    connectionAddress.sin_family = AF_INET;
    // host-to-network-short: big-endian conversion of a 16 byte value
    connectionAddress.sin_port = htons(myPort);

    unsigned long raw_ip_nbo = inet_addr(targetIP);
    if (raw_ip_nbo == INADDR_NONE)
    {
#ifdef NETWORK_LOG
            fprintf(stderr,"inet_addr() error \"%s\"\n", targetIP);
#endif
            return false;
    }
    connectionAddress.sin_addr.s_addr = raw_ip_nbo;

	int result = connect(mSocket, (sockaddr*)&connectionAddress, sizeof(sockaddr_in));
    if(result == SOCKET_ERROR)
    {
#if defined SAHWINDOWS
            int errorCode = WSAGetLastError();
            switch(errorCode)
            {

            case WSAEISCONN:
#ifdef NETWORK_LOG
				fprintf(stderr,"Is connected!\n");
#endif
				break;
            case WSAEWOULDBLOCK:
#ifdef NETWORK_LOG
				fprintf(stderr,"Would block!\n");
#endif
				break;
            case WSAEALREADY:
#ifdef NETWORK_LOG
				fprintf(stderr,"already connected!\n");
#endif
				break;
                break;

            default:
#ifdef NETWORK_LOG
                    fprintf(stderr,"client connect() error %d\n", errorCode);
#endif

                    return false;
            }
#elif defined SAHLINUX
            switch(errno)
            {

            case EINPROGRESS:
#ifdef NETWORK_LOG
				fprintf(stderr,"Connection in progress...\n");
#endif
				break;

            default:
#ifdef NETWORK_LOG
                    fprintf(stderr,"client connect() error %d\n", errno);
#endif

                    return false;
            }
#endif
    }

	bool lWait = true, lSuccess = false;
	float lStartTime = al_get_time();
	while(lWait)
	{
		result = getWriteStatus(mSocket);

		if (result != SOCKET_ERROR)
		{
			if(result == 1)
			{
				lWait = false;
				lSuccess = true;
				mConnected = true;
			}
		}
		else
		{
			lWait = false;
			lSuccess = false;
		}

		// Time out
		if (al_get_time() > lStartTime + 5.0f)
		{
			lWait = false;
			lSuccess = false;
		}

		/*result = send(mSocket,0, 0, 0);

		if(result == SOCKET_ERROR)
		{
			lWait = false;
			lSuccess = false;
		}*/
	}

#ifdef NETWORK_LOG
	if(lSuccess)
		fprintf(stderr,"Cliente conectado!!!!!\n");
#endif
	return lSuccess;
}

bool CNetworkClient::isConnected()
{
	return mConnected;
}

void CNetworkClient::receiveMessages(CMessageQueue *aQueue)
{
	if(getReadStatus(mSocket) != SOCKET_ERROR)
    {
		if(getReadStatus(mSocket) == 1)
        {
            unsigned long howMuchInBuffer = 0;
            unsigned long numBytesRead = 0;

#if defined SAHWINDOWS
            ioctlsocket(mSocket, FIONREAD, &howMuchInBuffer);
#elif defined SAHLINUX
            ioctl(mSocket, FIONREAD, &howMuchInBuffer);
#endif
            char *lBuffer = DBG_NEW char[howMuchInBuffer];
            int result = recv(mSocket, lBuffer, howMuchInBuffer, 0);
            if(result == SOCKET_ERROR)
            {
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
                    fprintf(stderr,"client recv() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
                    fprintf(stderr,"client recv() error %d\n", errno);
#endif
#endif
					mConnected = false;
                    return;
            }

			char *lReadPtr = lBuffer;
			int lProcessed = 0;

			while(lProcessed < howMuchInBuffer)
			{
				TMessage *lMsg = 0;
#if defined SAHWINDOWS
				EMessage lType = (EMessage)lReadPtr[4];
#elif defined SAHLINUX
                EMessage lType = (EMessage)lReadPtr[8];
#endif


				switch(lType)
				{
					case eMsgStartMatch: lMsg = DBG_NEW TMsgStartMatch; break;
					case eMsgPlUpdate: lMsg = DBG_NEW TMsgPlUpdate; break;
					case eMsgPlSound: lMsg = DBG_NEW TMsgPlSound; break;
					case eMsgOpenDoor: lMsg = DBG_NEW TMsgOpenDoor; break;
					case eMsgConsoleLog: lMsg = DBG_NEW TMsgConsoleLog; break;
					case eMsgRequestItem: lMsg = DBG_NEW TMsgRequestItem; break;
					case eMsgRemoveItem: lMsg = DBG_NEW TMsgRemoveItem; break;
					case eMsgConcealItem: lMsg = DBG_NEW TMsgConcealItem; break;
					case eMsgDamage: lMsg = DBG_NEW TMsgDamage; break;
					case eMsgNewBullet: lMsg = DBG_NEW TMsgNewBullet; break;
					case eMsgEndBullet: lMsg = DBG_NEW TMsgEndBullet; break;
					case eMsgNewExplos: lMsg = DBG_NEW TMsgNewExplos; break;
					case eMsgNewBarrier: lMsg = DBG_NEW TMsgNewBarrier; break;
					case eMsgNewItem: lMsg = DBG_NEW TMsgNewItem; break;
					case eMsgTurnSwitch: lMsg = DBG_NEW TMsgTurnSwitch; break;
					case eMsgTalkToChar: lMsg = DBG_NEW TMsgTalkToChar; break;
					case eMsgIncreaseKills: lMsg = DBG_NEW TMsgIncreaseKills; break;
					case eMsgEndMatch: lMsg = DBG_NEW TMsgEndMatch; break;
					case eMsgKeepAlive: lMsg = DBG_NEW TMsgKeepAlive; break;
					case eMsgPlDisconnect: lMsg = DBG_NEW TMsgPlDisconnect; break;

				}

				if(lMsg)
				{
#if defined SAHWINDOWS
					memcpy( (char*)lMsg + 4, (char*)lReadPtr + 4, lMsg->size() - 4 );
#elif defined SAHLINUX
                    memcpy( (char*)lMsg + 8, (char*)lReadPtr + 8, lMsg->size() - 8 );
#endif
				}

#ifdef NETWORK_LOG
				fprintf( stderr, "Mensaje del server: Tipo %s\n", lMsg->typeName());
#endif

				aQueue->queueMessage(lMsg);

				lReadPtr += lMsg->size();
				lProcessed += lMsg->size();
			};
		}
	}
	else
	{
		mConnected = false;
	}
}

void CNetworkClient::sendMessages(CMessageQueue *aQueue)
{
	char buffer[1024];


	TMessage *lMessage;

	while((lMessage = aQueue->extractFirst()) != 0)
	{
		unsigned int lSize = lMessage->size();

		memcpy(buffer, lMessage, lMessage->size());

		int result = send(mSocket,(const char *)buffer, lSize, 0);
		if(result == SOCKET_ERROR)
		{
			mConnected = false;
#ifdef NETWORK_LOG
#if defined SAHWINDOWS
			fprintf(stderr,"client send() error %d\n", WSAGetLastError());
#elif defined SAHLINUX
            fprintf(stderr,"client send() error %d\n", errno);
#endif
#endif
			return;
		}

#ifdef NETWORK_LOG_VERBOSE
		fprintf(stderr,"Message sent!!  %d bytes\n", lSize);
#endif

		delete lMessage;
	}

}
