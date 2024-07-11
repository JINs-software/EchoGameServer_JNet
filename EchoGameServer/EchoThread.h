#pragma once
#include "JNetCore.h"

#define ON_MESSAGE_BUFFERING

using namespace jnet;
using namespace jgroup;

// Àü¿ªÀÇ ½Ì±ÛÅæ ¸ð´ÏÅÍ¸µ °´Ã¼
class EchoGameServerMont;
extern EchoGameServerMont* g_ServerMont;

class EchoThread : public JNetGroupThread
{
public:
	//virtual void ConsoleLog() override {
	//	std::cout << "[EchoThread] Message Queuue Size : " << GetGroupThreadMessageQueueSize() << std::endl;
	//
	//	std::cout << "[EchoThread] Total Echo Thread Message Enqueue Cnt: " << GetMessageQueueEnqueueCount() << std::endl;
	//	std::cout << "[EchoThread] Total Echo Thread Message Dequeue Cnt: " << GetMessageQueueDequeueCount() << std::endl;
	//}

private:
	void ProcessEchoMessage(SessionID64 sessionID, INT64 accountNo, LONGLONG sendTick);

	virtual void OnStart() override;
	virtual void OnStop()  override {}
	virtual void OnEnterClient(SessionID64 sessionID) override;
	virtual void OnLeaveClient(SessionID64 sessionID) override;
	virtual void OnMessage(SessionID64 sessionID, JBuffer& recvData) override;
};

