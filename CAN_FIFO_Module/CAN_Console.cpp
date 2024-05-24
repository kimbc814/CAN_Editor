#include "stdafx.h"
#include "CAN_DLL/CAN_Access.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

#define MAX_FRAMES 100
typedef struct msg_frame {
    long _rid;
    int _rlen;
    char _rdata[8] = { 0, };
} msgFrame;
msgFrame* frames[MAX_FRAMES] = { nullptr, };
std::mutex frames_mutex;

void SendCanMessage(CAN_HANDLE h) {
    long sid = 0;
    unsigned char sdata[8] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11 };

    int ret1 = CAN_Send(h, sid, 8, (char*)sdata, 0, 0);
    if (!ret1) {
        printf("Send failed.\n");
    }
}

void RecvCanMessage(CAN_HANDLE h) {
    char rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    long rid;
    int rlen, ext, rtr;

    int ret = CAN_Recv(h, &rid, &rlen, rdata, &ext, &rtr);
    
    if (ret) {     
        std::lock_guard<std::mutex> lock(frames_mutex);
        int idx_frame=0;
        for (msgFrame* frame : frames) {
            if (frame == nullptr) {
                break;
            }
            if (frame->_rid == rid) {
                for (int i = 0; i < 8; i++) {
                    frame->_rdata[i] = rdata[i];
                }
                return;
            }
            idx_frame ++;
        }
        frames[idx_frame] = new msgFrame();
        frames[idx_frame]->_rid = rid;
        frames[idx_frame]->_rlen = rlen;
        for (int i = 0; i < 8; i++) {
            frames[idx_frame]->_rdata[i] = rdata[i];
        } 
    }
    else { printf("CAN READ FAIL\n"); }
}
void threadReadCAN() {
    // CAN 핸들 설정
    int noDevice = CAN_Fifo_ScanSerialNumber();
    if (noDevice <= 0) {
        printf("Can't find CAN Module\n");
        exit(0);
    }

    CAN_HANDLE h = CAN_OpenFifo(CAN_Fifo_GetSerialNumber(0));
    if (h < 0) {
        printf("USB2CAN open failed.\n");
        exit(0);
    }

    HANDLE hEvent = CreateEvent(NULL, false, false, "");
    CAN_SetRxEventNotification(h, hEvent);
    CAN_SetTransferMode(h, 1);

    while (1) {
        if (CAN_CountRxQueue(h) > 0) {
            RecvCanMessage(h);
        }
        else { /*printf("Read Fail\n");*/ }
    }
    // 정리
    CAN_Close(h);
    CloseHandle(hEvent);
}
void threadSendSock(SOCKET sock) {
    while (1) {
        /*
        send(sock, "316 8 35 99 EC 18 99 19 20 7D", strlen("316 8 35 99 EC 18 99 19 20 7D"), 0);
        Sleep(10);
        send(sock, "329 8 87 B0 80 14 11 58 3B 18", strlen("329 8 87 B0 80 14 11 58 3B 18"), 0);
        Sleep(10);
        send(sock, "2B0 5 17 00 00 07 98", strlen("2B0 5 17 00 00 07 98"), 0);
        Sleep(10);
        send(sock, "316 8 35 99 A0 0F 99 19 20 7D", strlen("316 8 35 99 EC 18 99 19 20 7D"), 0);
        Sleep(10);
        send(sock, "329 8 87 B0 80 14 11 58 3B 18", strlen("329 8 87 B0 80 14 11 58 3B 18"), 0);
        Sleep(10);
        send(sock, "2B0 5 17 00 00 07 98", strlen("2B0 5 17 00 00 07 98"), 0);
        Sleep(10);
        send(sock, "316 8 35 99 68 10 99 19 20 7D", strlen("316 8 35 99 EC 18 99 19 20 7D"), 0);
        Sleep(10);
        */
        //DWORD dwWaitResult = WaitForSingleObject(hEvent, 1);
        std::vector<msgFrame> local_frames;
        {
            std::lock_guard<std::mutex> lock(frames_mutex);
            for (msgFrame* frame : frames) {
                if (frame == nullptr) {
                    break;
                }
                local_frames.push_back(*frame);
            }
        }
        puts("==================================================");
        for (const msgFrame& frame : local_frames) {
            // 수신된 메시지 포맷 지정
            char recv_message[100];
            sprintf(recv_message,
                "%03X %d %02X %02X %02X %02X %02X %02X %02X %02X",
                frame._rid, frame._rlen,
                (int)(unsigned char)frame._rdata[0],
                (int)(unsigned char)frame._rdata[1],
                (int)(unsigned char)frame._rdata[2],
                (int)(unsigned char)frame._rdata[3],
                (int)(unsigned char)frame._rdata[4],
                (int)(unsigned char)frame._rdata[5],
                (int)(unsigned char)frame._rdata[6],
                (int)(unsigned char)frame._rdata[7]
            );

            printf("%s\n", recv_message);

            // 소켓을 통해 메시지를 전송
            if (send(sock, recv_message, strlen(recv_message), 0) < 0) {
                printf("Send to server failed: %d\n", WSAGetLastError());
            }

            // 10ms 대기
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
    }
}
int _tmain(int argc, _TCHAR* argv[]) {
    // Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return -1;
    }

    // 소켓 생성 및 서버 연결
    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    while (1) {
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) >= 0) {
            printf("Connect Success\n");
            break;
        }
        else {
            printf("Connect error : %d\n", WSAGetLastError());
            Sleep(1000);
        }
    }
    
    std::thread read_CAN(threadReadCAN);
    std::thread send_sock(threadSendSock, sock);
    read_CAN.join();
    send_sock.join();

    
    closesocket(sock);
    WSACleanup();

    getchar();

    return 0;
}