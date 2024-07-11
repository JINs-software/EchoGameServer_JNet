#pragma once
#include "JNetCore.h"

using namespace jnet;
using namespace jgroup;

class AuthThread : public JNetGroupThread
{
public:
	//virtual void ConsoleLog() override {
	//	std::cout << "[AuthThread] Total Login Query Cnt  : " << m_TotalLoginQueryCnt << std::endl;
	//	std::cout << "[AuthThread] Total Auth Success Cnt : " << m_TotalAuthSuccessCnt << std::endl;
	//	std::cout << "[AuthThread] Total Auth Fail Cnt    : " << m_TotalAuthFailCnt << std::endl;
	//
	//	std::cout << "[AuthThread] Total Auth Thread Message Enqueue Cnt: " << GetMessageQueueEnqueueCount() << std::endl;
	//	std::cout << "[AuthThread] Total Auth Thread Message Dequeue Cnt: " << GetMessageQueueDequeueCount() << std::endl;
	//}

private:
	uint64	m_TotalLoginQueryCnt = 0;
	uint64	m_TotalAuthSuccessCnt = 0;
	uint64	m_TotalAuthFailCnt = 0;


private:
	void	ProcessAuth(SessionID64 sessionID, INT64 accountNo, char SessionKey[64]);
	bool	AuthQueryToRedis(INT64 accountNo, char SessionKey[64]);
	void	SendLoginResponse(SessionID64 sessionID, INT64 accountNo, BYTE status);

	virtual void OnStart() override;
	virtual void OnStop()  override {}
	virtual void OnEnterClient(SessionID64 sessionID) override;
	virtual void OnLeaveClient(SessionID64 sessionID) override;
	virtual void OnMessage(SessionID64 sessionID, JBuffer& recvData) override;
};

