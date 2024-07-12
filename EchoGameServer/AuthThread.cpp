#include "AuthThread.h"
#include "Configuration.h"
#include "CommonProtocol.h"
#include "EchoGameServerMont.h"

extern EchoGameServerMont* g_ServerMont;

void AuthThread::ProcessAuth(SessionID64 sessionID, INT64 accountNo, char sessionKey[64])
{
	m_TotalLoginQueryCnt++;

	// ���𽺿� ��û
	// ��û�� ���� ��ȯ�� ����
	if (AuthQueryToRedis(accountNo, sessionKey)) {
		// - ���� ����
		// (1) EnterGroup(����, ���� �׷�)
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
		// (2) ���� ��ȯ �޽��� ����
		SendLoginResponse(sessionID, accountNo, 1);
		m_TotalAuthSuccessCnt++;
	}
	else {
		// - ���� ����
		// (1) ���� ��ȯ �޽��� ����
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
	// ���� ����
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
	// �α��� ��û �޽��� ����
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

			// 1. ���� Ȯ��
			//	skip..

			// 2. ����
			ProcessAuth(sessionID, msg.AccountNo, msg.SessionKey);
		}
		recvMsgCnt++;
	}

	if (recvData.GetUseSize() != 0) {
		DebugBreak();
	}
}
