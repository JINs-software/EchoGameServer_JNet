#include "AuthThread.h"
#include "Configuration.h"
#include "CommonProtocol.h"
#include "EchoGameServerMont.h"

extern EchoGameServerMont* g_ServerMont;

void AuthThread::ProcessAuth(SessionID64 sessionID, INT64 accountNo, char sessionKey[64])
{
	m_TotalLoginQueryCnt++;

	// 레디스에 요청
	// 요청에 대한 반환에 따라
	if (AuthQueryToRedis(accountNo, sessionKey)) {
		// - 인증 성공
		// (1) EnterGroup(세션, 에코 그룹)
#if defined(MULTI_ECHO_SESSION_GROUP)
		GroupID gid;
		if (sessionID % 4 == 0) {
			gid = ECHO_SESSION_GROUP_0;
		}
		else if (sessionID % 4 == 1) {
			gid = ECHO_SESSION_GROUP_1;
		}
		else if (sessionID % 4 == 2) {
			gid = ECHO_SESSION_GROUP_2;
		}
		else if (sessionID % 4 == 3) {
			gid = ECHO_SESSION_GROUP_3;
		}
		ForwardSessionGroup(sessionID, gid);
#else
		ForwardSessionToGroup(sessionID, ECHO_SESSION_GROUP_0);
		//g_ServerMont->Decrement_PlayerInAuthGroup(true);
		//g_ServerMont->Increment_PlayerInEchoGroup(true);
		
#endif
		// (2) 성공 반환 메시지 전달
		SendLoginResponse(sessionID, accountNo, 1);
		m_TotalAuthSuccessCnt++;
	}
	else {
		// - 인증 실패
		// (1) 실패 반환 메시지 전달
		SendLoginResponse(sessionID, accountNo, 0);
		m_TotalAuthFailCnt++;
	}
}

bool AuthThread::AuthQueryToRedis(INT64 accountNo, char SessionKey[64])
{
	return true;
}

void AuthThread::SendLoginResponse(SessionID64 sessionID, INT64 accountNo, BYTE status)
{

	JBuffer* resMsg = AllocSerialBuff();
	stMSG_HDR* hdr = resMsg->DirectReserve<stMSG_HDR>();
	hdr->code = dfPACKET_CODE;
	hdr->len = sizeof(stMSG_GAME_RES_LOGIN);
	hdr->randKey = -1;

	*resMsg << (WORD)en_PACKET_CS_GAME_RES_LOGIN;
	*resMsg << status;
	*resMsg << accountNo;

	if (!SendPacket(sessionID, resMsg, false, false)) {
		FreeSerialBuff(resMsg);
	}
}

void AuthThread::OnStart()
{
	// 레디스 연동
}

void AuthThread::OnEnterClient(SessionID64 sessionID)
{
#if defined(CONNECT_TO_MONITORING_SERVER)
	g_ServerMont->Increment_PlayerInAuthGroup(false);
	g_ServerMont->Increment_EnterInAuthGroup(false);
#endif
}

void AuthThread::OnLeaveClient(SessionID64 sessionID)
{
#if defined(CONNECT_TO_MONITORING_SERVER)
	g_ServerMont->Decrement_PlayerInAuthGroup(false);
	g_ServerMont->Increment_LeaveInAuthGroup(false);
#endif
}

void AuthThread::OnMessage(SessionID64 sessionID, JBuffer& recvData)
{
	// 로그인 요청 메시지 수신
	LONG recvMsgCnt = 0;
	while (recvData.GetUseSize() >= sizeof(WORD)) {
		WORD type;
		recvData.Peek(&type);
		if (type != en_PACKET_CS_GAME_REQ_LOGIN) {
			DebugBreak();
		}
		else {
			stMSG_GAME_REQ_LOGIN msg;
			recvData >> msg;

			// 1. 버전 확인
			//	skip..

			// 2. 인증
			ProcessAuth(sessionID, msg.AccountNo, msg.SessionKey);
		}
		recvMsgCnt++;
	}

	if (recvData.GetUseSize() != 0) {
		DebugBreak();
	}
}
