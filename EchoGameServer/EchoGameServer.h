#pragma once
#include "JNetCore.h"
#include "Configuration.h"
#include "CommonProtocol.h"
#include "AuthThread.h"
#include "EchoThread.h"
#include "EchoGameServerMont.h"

using namespace jnet;
using namespace jgroup;

extern EchoGameServerMont* g_ServerMont;

class EchoGameServer : public JNetGroupServer
{
public:
	EchoGameServer(
		const char* serverIP, uint16 serverPort, uint16 maximumOfConnections,
		BYTE packetCode_LAN, BYTE packetCode, BYTE packetSymmetricKey,
		bool recvBufferingMode,
		uint16 maximumOfSessions,
		uint32 numOfIocpConcurrentThrd, uint16 numOfIocpWorkerThrd,
		size_t tlsMemPoolUnitCnt, size_t tlsMemPoolUnitCapacity,
		bool tlsMemPoolMultiReferenceMode, bool tlsMemPoolPlacementNewMode,
		uint32 memPoolBuffAllocSize,
		uint32 sessionRecvBuffSize
	);
	~EchoGameServer();

private:
	virtual void OnPrintLogOnConsole();
	virtual void OnClientJoin(SessionID64 sessionID, const SOCKADDR_IN& clientSockAddr) override;
	virtual void OnClientLeave(SessionID64 sessionID) override;
};
