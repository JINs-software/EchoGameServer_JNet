#pragma once
#include "JNetCore.h"
#include "PerformanceCounter.h"
#include "Configuration.h"
#include "CommonProtocol.h"

using namespace jnet;

class EchoGameServerMont : public JNetClient {
private:
	bool					m_MontConnected;

	PerformanceCounter*		m_PerfCounter;

	bool					m_Stop;
	HANDLE					m_CounterThread;

	jgroup::JNetGroupServer*				m_EchoGameServer;
	jgroup::JNetGroupThread*				m_AuthGroupThread;
	jgroup::JNetGroupThread*				m_EchoGroupThread;

public:
	EchoGameServerMont(
		jgroup::JNetGroupServer* echogameServer, jgroup::JNetGroupThread* authThread, jgroup::JNetGroupThread* echoThread,
		const char* serverIP, uint16 serverPort,
		BYTE packetCode_LAN,
		uint32 numOfIocpConcurrentThrd, uint16 numOfIocpWorkerThrd,
		size_t tlsMemPoolUnitCnt, size_t tlsMemPoolUnitCapacity,
		bool tlsMemPoolMultiReferenceMode, bool tlsMemPoolPlacementNewMode,
		uint32 memPoolBuffAllocSize,
		uint32 sessionRecvBuffSize
	)
		:
		JNetClient(
			serverIP, serverPort,
			packetCode_LAN,
			numOfIocpConcurrentThrd, numOfIocpWorkerThrd,
			tlsMemPoolUnitCnt, tlsMemPoolUnitCapacity,
			tlsMemPoolMultiReferenceMode, tlsMemPoolPlacementNewMode,
			memPoolBuffAllocSize,
			sessionRecvBuffSize
		),
		m_EchoGameServer(echogameServer), m_AuthGroupThread(authThread), m_EchoGroupThread(echoThread), 
		m_MontConnected(false),
		m_PerfCounter(NULL),
		m_Stop(false)
	{}

	bool Start() {
		if (!JNetClient::Start(false)) {
			return false;
		}
		m_CounterThread = (HANDLE)_beginthreadex(NULL, 0, PerformanceCountFunc, this, 0, NULL);

		return true;
	}
	void Stop() {
		m_Stop = true;
	}

private:
	// 모니터링 항목
	uint64		m_TotalEnterCountInAuthGroup = 0;
	uint64		m_TotalEnterCountInEchoGroup = 0;
	uint64		m_TotalLeaveCountInAuthGroup = 0;
	uint64		m_TotalLeaveCountInEchoGroup = 0;
	int			m_PlayerInAuthGroup = 0;
	int			m_PlayerInEchoGroup = 0;

public:
	void ConsoleLog() {
		std::cout << "[Echo Game Server Mont]" << std::endl;
		std::cout << "GameSessionCnt               : " << m_EchoGameServer->GetCurrentSessions() << std::endl;
		std::cout << "Total Enter Count(AuthGroup) : " << m_TotalEnterCountInAuthGroup << std::endl;
		std::cout << "Total Leave Count(AuthGroup) : " << m_TotalLeaveCountInAuthGroup << std::endl;
		std::cout << "Player In AuthGroup          : " << m_PlayerInAuthGroup << std::endl;
		std::cout << "Total Enter Count(EchoGroup) : " << m_TotalEnterCountInEchoGroup << std::endl;
		std::cout << "Total Leave Count(EchoGroup) : " << m_TotalLeaveCountInEchoGroup << std::endl;
		std::cout << "Player In EchoGroup          : " << m_PlayerInEchoGroup << std::endl;
		std::cout << "Accept TPS                   : " << m_EchoGameServer->GetAcceptTPS() << std::endl;
		std::cout << "Recv TPS                     : " << m_EchoGameServer->GetRecvTPS() << std::endl;
		std::cout << "Send TPS                     : " << m_EchoGameServer->GetSendTPS() << std::endl;
		std::cout << "Auth Thread FPS              : " << m_AuthGroupThread->GetGroupThreadLoopFPS() << std::endl;
		std::cout << "Echo Thread FPS              : " << m_EchoGroupThread->GetGroupThreadLoopFPS() << std::endl;
		std::cout << "MemPool Unit Usage           : " << m_EchoGameServer->GetCurrentAllocatedMemUnitCnt() << std::endl;
		std::cout << "[EchoGameServerMont::JNetCore Info]" << std::endl;
		JNetCore::PrintLibraryInfoOnConsole();
	}
	
	inline void Increment_EnterInAuthGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement64((int64*)&m_TotalEnterCountInAuthGroup);
		}
		else {
			m_TotalEnterCountInAuthGroup++;
		}
	}
	inline void Increment_EnterInEchoGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement64((int64*)&m_TotalEnterCountInEchoGroup);
		}
		else {
			m_TotalEnterCountInEchoGroup++;
		}
	}
	inline void Increment_LeaveInAuthGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement64((int64*)&m_TotalLeaveCountInAuthGroup);
		}
		else {
			m_TotalLeaveCountInAuthGroup++;
		}
	}
	inline void Increment_LeaveInEchoGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement64((int64*)&m_TotalLeaveCountInEchoGroup);
		}
		else {
			m_TotalLeaveCountInEchoGroup++;
		}
	}

	inline void	Increment_PlayerInAuthGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement((LONG*)&m_PlayerInAuthGroup);
		}
		else {
			m_PlayerInAuthGroup++;
		}
	}
	inline void	Decrement_PlayerInAuthGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedDecrement((LONG*)&m_PlayerInAuthGroup);
		}
		else {
			m_PlayerInAuthGroup--;
		}
	}
	inline void	Increment_PlayerInEchoGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedIncrement((LONG*)&m_PlayerInEchoGroup);
		}
		else {
			m_PlayerInEchoGroup++;
		}
	}
	inline void	Decrement_PlayerInEchoGroup(bool threadSafe = false) {
		if (threadSafe) {
			InterlockedDecrement((LONG*)&m_PlayerInEchoGroup);
		}
		else {
			m_PlayerInEchoGroup--;
		}
	}

	virtual void OnConnectionToServer() override {
		// 모니터링 서버에 접속 성공 시 로그인 요청 메시지 송신
		JBuffer* loginMsg = AllocSerialSendBuff(sizeof(WORD) + sizeof(int));
		(*loginMsg) << (WORD)en_PACKET_SS_MONITOR_LOGIN;
		(*loginMsg) << (int)dfSERVER_ECHO_GAME_SERVER;

		if (!SendPacket(loginMsg)) {
			FreeSerialBuff(loginMsg);
		}
		else {
			m_MontConnected = true;
		}
	}
	virtual void OnDisconnectionFromServer() override {
		m_MontConnected = false;
	}
	virtual void OnRecv(JBuffer& recvBuff) override {
		// 모니터링 서버로부터 수신 받을 패킷 없음
	}

	static UINT __stdcall PerformanceCountFunc(void* arg);
	void SendCounterToMontServer();
};