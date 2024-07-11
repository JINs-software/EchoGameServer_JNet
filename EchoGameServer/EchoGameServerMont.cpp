#include "EchoGameServerMont.h"

UINT __stdcall EchoGameServerMont::PerformanceCountFunc(void* arg)
{
	EchoGameServerMont* serverMont = (EchoGameServerMont*)arg;

	serverMont->AllocTlsMemPool();

	serverMont->m_PerfCounter = new PerformanceCounter();
	serverMont->m_PerfCounter->SetCpuUsageCounter();
	serverMont->m_PerfCounter->SetProcessCounter(dfMONITOR_DATA_TYPE_GAME_SERVER_MEM, dfQUERY_PROCESS_USER_VMEMORY_USAGE, L"EchoGameServer");

	clock_t timestamp = clock();
	while (!serverMont->m_Stop) {
		if (serverMont->m_MontConnected) {
			// Connected to Mont Server..
			serverMont->SendCounterToMontServer();
		}
		else  {
			serverMont->ConnectToServer();
		}

		Sleep(1000);
	}

	return 0;
}

void EchoGameServerMont::SendCounterToMontServer()
{
	time_t now = time(NULL);
	m_PerfCounter->ResetPerfCounterItems();

	JBuffer* perfMsg = AllocSerialSendBuff((sizeof(WORD) + sizeof(BYTE) + sizeof(int) + sizeof(int)) * 12);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_SERVER_RUN);
	(*perfMsg) << static_cast<int>(1);
	(*perfMsg) << static_cast<int>(now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_SERVER_CPU);
	(*perfMsg) << static_cast<int>( m_PerfCounter->ProcessTotal());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_SERVER_MEM);
	(*perfMsg) << static_cast<int>( m_PerfCounter->GetPerfCounterItem(dfMONITOR_DATA_TYPE_GAME_SERVER_MEM) / (1024 * 1024));
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_SESSION);
	(*perfMsg) << static_cast<int>(m_EchoGameServer->GetCurrentSessions());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_AUTH_PLAYER);
	(*perfMsg) << static_cast<int>( m_PlayerInAuthGroup);
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_GAME_PLAYER);
	(*perfMsg) << static_cast<int>(m_PlayerInEchoGroup);
	(*perfMsg) << static_cast<int>(now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_ACCEPT_TPS);
	(*perfMsg) << static_cast<int>(m_EchoGameServer->GetAcceptTPS());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_PACKET_RECV_TPS);
	(*perfMsg) << static_cast<int>(m_EchoGameServer->GetRecvTPS());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_PACKET_SEND_TPS);
	(*perfMsg) << static_cast<int>(m_EchoGameServer->GetSendTPS());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_AUTH_THREAD_FPS);
	(*perfMsg) << static_cast<int>(m_AuthGroupThread->GetGroupThreadLoopFPS());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_GAME_THREAD_FPS);
	(*perfMsg) << static_cast<int>(m_EchoGroupThread->GetGroupThreadLoopFPS());
	(*perfMsg) << static_cast<int>( now);

	(*perfMsg) << static_cast<WORD>(en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE);
	(*perfMsg) << static_cast<BYTE>(dfMONITOR_DATA_TYPE_GAME_PACKET_POOL);
	(*perfMsg) << static_cast<int>(m_EchoGameServer->GetCurrentAllocatedMemUnitCnt());
	(*perfMsg) << static_cast<int>( now);

	SendPacket(perfMsg);
}
