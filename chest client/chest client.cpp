#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <WinSock2.h>
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
int main() {
	WSADATA wsd;
	string input = "";
	string name = "";
	string answer = "";
	while (input != "quit") {
		if (FAILED(WSAStartup(MAKEWORD(2, 2), &wsd)))
			//Инициализация Winsock, функция WSAStartupиспользуется для указания под версии интерфейса Winsoсk
			cout << "Error with initializint winsock" << endl; //Ошибка инициализации winsock

		SOCKET sockk = socket(AF_INET, SOCK_STREAM, 0); //Создаем сокет
		if (sockk == INVALID_SOCKET)
			cout << "Error with creating socket" << endl; //Ошибка создания сокета

		sockaddr_in addr; //Создаем и заполняем переменную для хранения адреса
		addr.sin_family = AF_INET; //Семейство адресов, которые будет обрабатывать наш сервер, у нас это TCP/IP адреса
		addr.sin_port = htons(3128); //Используем функцию htons для перевода номера порта в TCP/IP представление
		addr.sin_addr.s_addr = inet_addr("192.168.1.240");//inet_addr("192.168.0.102");//htonl(INADDR_LOOPBACK);

		//INADDR_LOOPBACK позволяет рабтать с сервером, расположенном на том же компе что и клиент
		string msg = "";
		if (SOCKET_ERROR == connect(sockk, (struct sockaddr*)&addr, sizeof(addr))) //Связываем сокет с адресом
			cout << "Error with binding socket";
		
		while (input != "quit") {
			
			cin >> input;
			msg = input;
			answer = TalkToServer(msg, sockk);
			cout << "ans: " << answer << endl;
		}
		closesocket(sockk);
	}
	
	return 0;
}
