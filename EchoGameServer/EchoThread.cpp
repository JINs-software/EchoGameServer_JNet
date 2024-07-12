#include "EchoThread.h"
#include "CommonProtocol.h"
#include "EchoGameServerMont.h"

extern EchoGameServerMont* g_ServerMont;

void EchoThread::ProcessEchoMessage(SessionID64 sessionID, INT64 accountNo, LONGLONG sendTick)
{
	JBuffer* resMsg = AllocSerialBuff();
	stMSG_HDR* hdr = resMsg->DirectReserve<stMSG_HDR>();
	hdr->code = dfPACKET_CODE;
	hdr->len = sizeof(stMSG_GAME_RES_ECHO);
	hdr->randKey = -1;

	*resMsg << (WORD)en_PACKET_CS_GAME_RES_ECHO;
	*resMsg << accountNo;
	*resMsg << sendTick;

	if (!SendPacket(sessionID, resMsg, true, false)) {
		FreeSerialBuff(resMsg);
	}
}

void EchoThread::OnStart()
{
	HANDLE hThread = GetCurrentThread();
	
	// ���� �������� �켱������ �������� ����
	if (!SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST)) {
		std::cerr << "Failed to set thread priority. Error: " << GetLastError() << "\n";
		DebugBreak();
	}
}

void EchoThread::OnEnterClient(SessionID64 sessionID)
{
#if defined(CONNECT_TO_MONITORING_SERVER)
	g_ServerMont->Increment_PlayerInEchoGroup(false);
	g_ServerMont->Increment_EnterInEchoGroup(false);
#endif
}

void EchoThread::OnLeaveClient(SessionID64 sessionID)
{
#if defined(CONNECT_TO_MONITORING_SERVER)
	g_ServerMont->Decrement_PlayerInEchoGroup(false);
	g_ServerMont->Increment_LeaveInEchoGroup(false);
#endif
}

void EchoThread::OnMessage(SessionID64 sessionID, JBuffer& recvData)
{
#if defined(ON_MESSAGE_BUFFERING)
	while (true) {
		JBuffer* resMsg = AllocSerialBuff();

		LONG recvMsgCnt = 0;
		while (recvData.GetUseSize() >= sizeof(stMSG_GAME_REQ_ECHO)) {
			//recvData.Peek(&type);
			// => Peek���� �޸� ���� ������� ����
			//    (���޹��� ���� ����ȭ ���۴� �� ���·� �޽����� ������ ������ ����޴´�.)
			
			if (resMsg->GetFreeSize() < sizeof(stMSG_HDR) + sizeof(stMSG_GAME_REQ_ECHO::Type) + sizeof(stMSG_GAME_REQ_ECHO::AccountoNo) + sizeof(stMSG_GAME_REQ_ECHO::SendTick)) {
				break;
			}
			
			stMSG_GAME_REQ_ECHO msg;
			//recvData >> msg;
			// => �޸� ���� ������� ����
			msg = *(stMSG_GAME_REQ_ECHO*)recvData.GetDequeueBufferPtr();

			stMSG_HDR* hdr = resMsg->DirectReserve<stMSG_HDR>();
			hdr->code = dfPACKET_CODE;
			hdr->len = sizeof(stMSG_GAME_REQ_ECHO::Type) + sizeof(stMSG_GAME_REQ_ECHO::AccountoNo) + sizeof(stMSG_GAME_REQ_ECHO::SendTick);
			hdr->randKey = -1;		// SendPacket�� ���ڵ� ��û

			stMSG_GAME_RES_ECHO* body = resMsg->DirectReserve<stMSG_GAME_RES_ECHO>();
			body->Type = en_PACKET_CS_GAME_RES_ECHO;
			body->AccountoNo = msg.AccountoNo;
			body->SendTick = msg.SendTick;

			recvData.DirectMoveDequeueOffset(sizeof(stMSG_GAME_REQ_ECHO));
			
			recvMsgCnt++;
		}

		if (recvMsgCnt > 0) {
			SendPacket(sessionID, resMsg, true, false);		// ��Ŀ �����忡 �۽� å�� �Ҵ�
#if defined(CONNECT_TO_MONITORING_SERVER)
			//m_Server->Increment(false, recvMsgCnt);;
			m_Server->IncrementSendTransactions(false, recvMsgCnt);
#endif
		}
		else {
			FreeSerialBuff(resMsg);
			break;
		}

	}
#else
	// ���� ��û �޽��� ����
	LONG recvMsgCnt = 0;
	while (recvData.GetUseSize() >= sizeof(WORD)) {
		WORD type;
		recvData.Peek(&type);
		if (type != en_PACKET_CS_GAME_REQ_ECHO) {
			DebugBreak();
		}
		else {
			stMSG_GAME_REQ_ECHO msg;
			recvData >> msg;
			ProcessEchoMessage(sessionID, msg.AccountoNo, msg.SendTick);
		}
		recvMsgCnt++;
	}
#if defined(CALCULATE_TRANSACTION_PER_SECOND)
	IncrementRecvTransactionNoGuard(recvMsgCnt);
#endif
#endif

	if (recvData.GetUseSize() != 0) {
		DebugBreak();
	}
}
