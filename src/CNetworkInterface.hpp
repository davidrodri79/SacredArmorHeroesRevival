

#include <vector>
#include "CMessageQueue.hpp"

class CNetworkInterface
{

	public:

	protected:
#if defined SAHWINDOWS
		SOCKET mSocket;
#elif defined SAHLINUX
        int mSocket;
#endif // defined

		bool createSocket();
		bool end();

		virtual void sendMessages(CMessageQueue*) = 0;
		virtual void receiveMessages(CMessageQueue*) = 0;

};

class CNetworkServer : public CNetworkInterface
{
	private:

		struct sockaddr_in hostnameAddr;

		struct TClientInfo
		{
#if defined SAHWINDOWS
             SOCKET mSocket;
#elif defined SAHLINUX
             int mSocket;
#endif // defined
             sockaddr mAddress;
			 tbyte mLobbyId;
			 tbyte mPlayerMask;
			 float mLastAliveTime;
		};

		std::vector<TClientInfo> mClients;

		unsigned char mNextLobbyId;

	public:
		CNetworkServer();
		~CNetworkServer();

		bool init(unsigned short);
		char* getServerIPAdress();
		void acceptClients();
		bool isClientConnected(tbyte);
		char* getClientIPAdress(tbyte aLobbyId);
		void stopAccepting();
		void assignPlayerMaskToClient(tbyte,tbyte);
		void sendMessages(CMessageQueue*);
		void receiveMessages(CMessageQueue*);
};

class CNetworkClient : public CNetworkInterface
{
	private:
		bool mConnected;

	public:
		CNetworkClient();
		~CNetworkClient();

		bool init();
		bool connectToServer(char*, unsigned short);
		bool isConnected();
		void sendMessages(CMessageQueue*);
		void receiveMessages(CMessageQueue*);
};
