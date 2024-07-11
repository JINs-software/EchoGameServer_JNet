#pragma once

#define ECHO_THREAD_PRIORITY_BOOST						true
#define RECV_PACKET_BUFFERING_MODE						true

#define ECHO_GAME_SERVER_IP								NULL //"127.0.0.1"
#define ECHO_GAME_SERVER_PORT							12120

#define ECHO_GAME_SERVER_IOCP_CONCURRENT_THREAD			0
#define ECHO_GAME_SERVER_IOCP_WORKER_THREAD				4
#define ECHO_GAME_SERVER_MAX_OF_CONNECTIONS				8000

#define ECHO_GAME_SERVER_TLS_MEM_POOL_UNIT_CNT			0
#define ECHO_GAME_SERVER_TLS_MEM_POOL_UNIT_CAPACITY		1000

#define ECHO_GAME_SERVER_SERIAL_BUFFER_SIZE				5000	// 에코 패킷(헤더(5) + 바디(2+8+8)) * 오버랩 갯수(200) == 4600
																// 수신 버퍼링 대비

#define ECHO_GAME_SERVER_SESSION_RECV_BUFF_SIZE			10000

#define ECHO_GAME_SERVER_PROTOCOL_CODE					119
#define ECHO_GAME_SERVER_PACKET_KEY						50

#define AUTH_SESSION_GROUP			1000
#define ECHO_SESSION_GROUP_0		2000


#define IOCP_WORKER_THREAD_CNT				4


#define CONNECT_TO_MONITORING_SERVER
#if defined(CONNECT_TO_MONITORING_SERVER)
#define MONT_SERVER_IP							"127.0.0.1"
#define	MONT_SERVER_PORT						12121
#define MONT_SERVER_PROTOCOL_CODE				109
#define MONT_SERVER_PACKET_KEY					30
#define MONT_CLIENT_IOCP_CONCURRENT_THRD		0
#define MONT_CLIENT_IOCP_WORKER_THRD_CNT		1
#define MONT_CLIENT_MEM_POOL_UNIT_CNT			0
#define MONT_CLIENT_MEM_POOL_UNIT_CAPACITY		10
#define MONT_CLIENT_MEM_POOL_BUFF_ALLOC_SIZE	200
#define MONT_CLIENT_RECV_BUFF_SIZE				100
#endif