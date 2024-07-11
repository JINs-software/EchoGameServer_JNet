#include "EchoGameServer.h"

EchoGameServerMont* g_ServerMont;

EchoGameServer::EchoGameServer(
	const char* serverIP, uint16 serverPort, uint16 maximumOfConnections,
	BYTE packetCode_LAN, BYTE packetCode, BYTE packetSymmetricKey,
	bool recvBufferingMode,
	uint16 maximumOfSessions,
	uint32 numOfIocpConcurrentThrd, uint16 numOfIocpWorkerThrd,
	size_t tlsMemPoolUnitCnt, size_t tlsMemPoolUnitCapacity,
	bool tlsMemPoolMultiReferenceMode, bool tlsMemPoolPlacementNewMode,
	uint32 memPoolBuffAllocSize,
	uint32 sessionRecvBuffSize
)
	: JNetGroupServer(
		serverIP, serverPort, maximumOfConnections,
		packetCode_LAN, packetCode, packetSymmetricKey,
		recvBufferingMode,
		maximumOfSessions,
		numOfIocpConcurrentThrd, numOfIocpWorkerThrd,
		tlsMemPoolUnitCnt, tlsMemPoolUnitCapacity,
		tlsMemPoolMultiReferenceMode, tlsMemPoolPlacementNewMode,
		memPoolBuffAllocSize,
		sessionRecvBuffSize
	)
{
	// 그룹 스레드 생성 및 셋팅
	JNetGroupThread* authThread = new AuthThread();
	JNetGroupThread* echoThread = new EchoThread();
	CreateGroup(AUTH_SESSION_GROUP, authThread);
	CreateGroup(ECHO_SESSION_GROUP_0, echoThread, ECHO_THREAD_PRIORITY_BOOST);

#if defined(CONNECT_TO_MONITORING_SERVER)
	g_ServerMont = new EchoGameServerMont(
		this, authThread, echoThread,
		MONT_SERVER_IP, MONT_SERVER_PORT,
		MONT_SERVER_PROTOCOL_CODE,
		MONT_CLIENT_IOCP_CONCURRENT_THRD, MONT_CLIENT_IOCP_WORKER_THRD_CNT,
		MONT_CLIENT_MEM_POOL_UNIT_CNT, MONT_CLIENT_MEM_POOL_UNIT_CAPACITY,
		false, false, 
		MONT_CLIENT_MEM_POOL_BUFF_ALLOC_SIZE,
		MONT_CLIENT_RECV_BUFF_SIZE
	);
	g_ServerMont->Start();
#endif
}

EchoGameServer::~EchoGameServer()
{
#if defined(CONNECT_TO_MONITORING_SERVER)
	if (g_ServerMont != NULL) {
		g_ServerMont->Stop();
		delete g_ServerMont;
	}
#endif
}

void EchoGameServer::OnPrintLogOnConsole()
{
	g_ServerMont->ConsoleLog();
}

void EchoGameServer::OnClientJoin(SessionID64 sessionID, const SOCKADDR_IN& clientSockAddr)
{
	EnterSessionGroup(sessionID, AUTH_SESSION_GROUP);
}

void EchoGameServer::OnClientLeave(SessionID64 sessionID)
{
	LeaveSessionGroup(sessionID);
}
