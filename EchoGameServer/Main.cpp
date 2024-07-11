#include "EchoGameServer.h"
#include <conio.h>

int main() {
	EchoGameServer echoGameServer(
		ECHO_GAME_SERVER_IP, ECHO_GAME_SERVER_PORT, ECHO_GAME_SERVER_MAX_OF_CONNECTIONS,
		MONT_SERVER_PROTOCOL_CODE, ECHO_GAME_SERVER_PROTOCOL_CODE, ECHO_GAME_SERVER_PACKET_KEY,
		true,
		ECHO_GAME_SERVER_MAX_OF_CONNECTIONS,
		ECHO_GAME_SERVER_IOCP_CONCURRENT_THREAD, ECHO_GAME_SERVER_IOCP_WORKER_THREAD,
		ECHO_GAME_SERVER_TLS_MEM_POOL_UNIT_CNT, ECHO_GAME_SERVER_TLS_MEM_POOL_UNIT_CAPACITY,
		false, false,
		ECHO_GAME_SERVER_SERIAL_BUFFER_SIZE, 
		ECHO_GAME_SERVER_SESSION_RECV_BUFF_SIZE
	);

	echoGameServer.Start();

	char ctr;
	clock_t ct = 0;
	while (true) {
		if (_kbhit()) {
			ctr = _getch();
			if (ctr == 'q' || ctr == 'Q') {
				break;
			}
		}

		clock_t now = clock();
		if (now - ct > 1000) {
			echoGameServer.PrintServerInfoOnConsole();
			ct = now;
		}
	}

	echoGameServer.Stop();
}