#pragma once
#include <minwindef.h>

#define dfPACKET_CODE		119
#define dfPACKET_KEY		50

#pragma pack(push, 1)
struct stMSG_GAME_REQ_LOGIN {
	WORD	Type;
	INT64	AccountNo;
	char	SessionKey[64];
	int		Version;
};
struct stMSG_GAME_RES_LOGIN {
	WORD	Type;
	BYTE	Status;	// (0: 실패 / 1 : 성공)
	INT64	AccountNo;
};
struct stMSG_GAME_REQ_ECHO {
	WORD		Type;
	INT64		AccountoNo;
	LONGLONG	SendTick;
};
struct stMSG_GAME_RES_ECHO {
	WORD		Type;
	INT64		AccountoNo;
	LONGLONG	SendTick;
};
struct stMSG_MONITOR_DATA_UPDATE {
	WORD	Type;
	BYTE	DataType;
	int		DataValue;
	int		TimeStamp;

};
#pragma pack(pop)

enum en_PACKET_TYPE
{
	//------------------------------------------------------
	// Game Server
	//------------------------------------------------------
	en_PACKET_CS_GAME_SERVER = 1000,

	//------------------------------------------------------------
	// 로그인 요청
	//
	//	{
	//		WORD	Type
	//
	//		INT64	AccountNo
	//		char	SessionKey[64]
	//
	//		int		Version			// 1 
	//	}
	//
	//------------------------------------------------------------
	en_PACKET_CS_GAME_REQ_LOGIN,

	//------------------------------------------------------------
	// 로그인 응답
	//
	//	{
	//		WORD	Type
	//
	//		BYTE	Status (0: 실패 / 1: 성공)
	//		INT64	AccountNo
	//	}
	//
	//	지금 더미는 무조건 성공으로 판단하고 있음
	//	Status 결과를 무시한다는 이야기
	//
	//  en_PACKET_CS_GAME_RES_LOGIN define 값 사용.
	//------------------------------------------------------------
	en_PACKET_CS_GAME_RES_LOGIN,



	//------------------------------------------------------------
	// 테스트용 에코 요청
	//
	//	{
	//		WORD		Type
	//
	//		INT64		AccountoNo
	//		LONGLONG	SendTick
	//	}
	//
	//------------------------------------------------------------	
	en_PACKET_CS_GAME_REQ_ECHO = 5000,

	//------------------------------------------------------------
	// 테스트용 에코 응답 (REQ 를 그대로 돌려줌)
	//
	//	{
	//		WORD		Type
	//
	//		INT64		AccountoNo
	//		LONGLONG	SendTick
	//	}
	//
	//------------------------------------------------------------
	en_PACKET_CS_GAME_RES_ECHO,

	//------------------------------------------------------------
	// 하트비트
	//
	//	{
	//		WORD		Type
	//	}
	//
	//
	// 클라이언트는 이를 30초마다 보내줌.
	// 서버는 40초 이상동안 메시지 수신이 없는 클라이언트를 강제로 끊어줘야 함.
	//------------------------------------------------------------	
	en_PACKET_CS_GAME_REQ_HEARTBEAT,

	//------------------------------------------------------
	// Monitor Server  Protocol
	//------------------------------------------------------
	en_PACKET_SS_MONITOR = 20000,
	//------------------------------------------------------
	// Server -> Monitor Protocol
	//------------------------------------------------------
	//------------------------------------------------------------
	// LoginServer, GameServer , ChatServer  가 모니터링 서버에 로그인 함
	//
	// 
	//	{
	//		WORD	Type
	//
	//		int		ServerNo		//  각 서버마다 고유 번호를 부여하여 사용
	//	}
	//
	//------------------------------------------------------------
	en_PACKET_SS_MONITOR_LOGIN,

	//------------------------------------------------------------
	// 서버가 모니터링서버로 데이터 전송
	// 각 서버는 자신이 모니터링중인 수치를 1초마다 모니터링 서버로 전송.
	//
	// 서버의 다운 및 기타 이유로 모니터링 데이터가 전달되지 못할떄를 대비하여 TimeStamp 를 전달한다.
	// 이는 모니터링 클라이언트에서 계산,비교 사용한다.
	// 
	//	{
	//		WORD	Type
	//
	//		BYTE	DataType				// 모니터링 데이터 Type 하단 Define 됨.
	//		int		DataValue				// 해당 데이터 수치.
	//		int		TimeStamp				// 해당 데이터를 얻은 시간 TIMESTAMP  (time() 함수)
	//										// 본래 time 함수는 time_t 타입변수이나 64bit 로 낭비스러우니
	//										// int 로 캐스팅하여 전송. 그래서 2038년 까지만 사용가능
	//	}
	//
	//------------------------------------------------------------
	en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE
};

enum en_PACKET_SS_MONITOR_DATA_UPDATE {
	dfMONITOR_DATA_TYPE_GAME_SERVER_RUN = 10,		// GameServer 실행 여부 ON / OFF
	dfMONITOR_DATA_TYPE_GAME_SERVER_CPU = 11,		// GameServer CPU 사용률
	dfMONITOR_DATA_TYPE_GAME_SERVER_MEM = 12,		// GameServer 메모리 사용 MByte
	dfMONITOR_DATA_TYPE_GAME_SESSION = 13,			// 게임서버 세션 수 (컨넥션 수)
	dfMONITOR_DATA_TYPE_GAME_AUTH_PLAYER = 14,		// 게임서버 AUTH MODE 플레이어 수
	dfMONITOR_DATA_TYPE_GAME_GAME_PLAYER = 15,		// 게임서버 GAME MODE 플레이어 수
	dfMONITOR_DATA_TYPE_GAME_ACCEPT_TPS = 16,		// 게임서버 Accept 처리 초당 횟수
	dfMONITOR_DATA_TYPE_GAME_PACKET_RECV_TPS = 17,		// 게임서버 패킷처리 초당 횟수
	dfMONITOR_DATA_TYPE_GAME_PACKET_SEND_TPS = 18,		// 게임서버 패킷 보내기 초당 완료 횟수
	dfMONITOR_DATA_TYPE_GAME_DB_WRITE_TPS = 19,		// 게임서버 DB 저장 메시지 초당 처리 횟수
	dfMONITOR_DATA_TYPE_GAME_DB_WRITE_MSG = 20,		// 게임서버 DB 저장 메시지 큐 개수 (남은 수)
	dfMONITOR_DATA_TYPE_GAME_AUTH_THREAD_FPS = 21,		// 게임서버 AUTH 스레드 초당 프레임 수 (루프 수)
	dfMONITOR_DATA_TYPE_GAME_GAME_THREAD_FPS = 22,		// 게임서버 GAME 스레드 초당 프레임 수 (루프 수)
	dfMONITOR_DATA_TYPE_GAME_PACKET_POOL = 23,		// 게임서버 패킷풀 사용량
};


enum en_SERVER_TYPE {
	dfSERVER_LOGIN_SERVER = 0,
	dfSERVER_ECHO_GAME_SERVER,
	dfSERVER_CHAT_SERVER,
	dfSERVER_SYSTEM
};