#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include "CardClass.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;
using namespace std;
string TalkToServer(string msg, SOCKET sockk) {
	char buf[2000];
	memset(buf, 0, 2000);
	send(sockk, msg.c_str(), msg.size(), 0);
	recv(sockk, buf, 2000, 0);
	return buf;
}
vector <int> decodestatus(string answer) {
	string data = "";
	vector <int> status;
	for (int i = 0; i < answer.size(); ++i) {
		while (answer[i] != ' ') {
			data += answer[i];
			++i;
		}
		status.push_back(STI(data));
		data = "";
	}
	return status;
}
int main() {
	WSADATA wsd;
	string input = "";
	string name = "";
	string answer = "";
	while (input != "quit") {
		if (FAILED(WSAStartup(MAKEWORD(2, 2), &wsd)))
			//������������� Winsock, ������� WSAStartup������������ ��� �������� ��� ������ ���������� Winso�k
			cout << "Error with initializint winsock" << endl; //������ ������������� winsock

		SOCKET sockk = socket(AF_INET, SOCK_STREAM, 0); //������� �����
		if (sockk == INVALID_SOCKET)
			cout << "Error with creating socket" << endl; //������ �������� ������

		sockaddr_in addr; //������� � ��������� ���������� ��� �������� ������
		addr.sin_family = AF_INET; //��������� �������, ������� ����� ������������ ��� ������, � ��� ��� TCP/IP ������
		addr.sin_port = htons(3128); //���������� ������� htons ��� �������� ������ ����� � TCP/IP �������������
		addr.sin_addr.s_addr = inet_addr("192.168.1.240");//inet_addr("192.168.0.102");//htonl(INADDR_LOOPBACK);

		//INADDR_LOOPBACK ��������� ������� � ��������, ������������� �� ��� �� ����� ��� � ������
		string msg = "";
		string data = "";
		vector <int> status;
		vector <int> mycards;
		if (SOCKET_ERROR == connect(sockk, (struct sockaddr*)&addr, sizeof(addr))) { //��������� ����� � �������
			cout << "Error with binding socket" << endl;
		}
		else {
			cout << "Connected, you can enter your name now" << endl;
			do
			{
				cin >> input;
				msg = input;
				data = TalkToServer(msg, sockk);
				data = TalkToServer("0", sockk);
			} while (data == "the game is not started yet");
		}
		cout << "game is ready" << endl;
		while (input != "quit") {
			cin >> input;
			input = to_lower(input);
			if (input == "help") {
				cout << "commands avaible:" << endl;
				cout << "help - show this menu" << endl;
				cout << "0 or status - to look compactly at your cards" << endl;
				cout << "1 or getcards - to get list of your cards" << endl;
				cout << "2 3 4 5 6 7 8 9 10 J Q K A - ask enemy for a card, only legal move will be send to the server!!!" << endl;
				cout << "quit - quit, this finishes the game for everyone!!!" << endl;
			}
			else if (input == "0" || input == "status") {
				msg = "0";
				answer = TalkToServer(msg, sockk);
				status.clear();
				status = decodestatus(answer);
				for (int i = 0; i < status.size(); ++i) {
					cout << status[i] << " ";
				}
				cout << endl << "2 3 4 5 6 7 8 9 10 J Q K A" << endl;
			}
			else if (input == "1" || input == "mycards") {
				msg = "1";
				answer = TalkToServer(msg, sockk);
				mycards.clear();
				for (int i = 0; i < answer.size(); ++i) {
					while (answer[i] != ' ') {
						data += answer[i];
						++i;
					}
					mycards.push_back(STI(data));
					data = "";
				}
				for (int i = 0; i < mycards.size(); ++i) {
					cout << Card(mycards[i]);
				}
			}
			else if ((input >= "2" && input <= "9") || input == "10" || input == "j" || input == "q" || input == "k" || input == "a") {
				status.clear();
				status = decodestatus(TalkToServer("0", sockk));
				msg = input;
				int num = 0;
				if (input == "j")
					num = 9;
				else if (input == "q")
					num = 10;
				else if (input == "k")
					num = 11;
				else if (input == "a")
					num = 12;
				else
					num = STI(input);
				if (status[num] == 0)
					cout << "this move is illigal, you don't have this card" << endl;
				else
					cout << TalkToServer(msg, sockk) << endl;
			}
			else {
				cout << "invalid command, type help to see avaible commands" << endl;
			}
		}
		closesocket(sockk);
	}
	return 0;
}
