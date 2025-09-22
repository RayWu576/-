#include <string>
#include <stdio.h>
#include <winsock.h>
#include <map>
#include <conio.h>

int main(){
	SOCKET sd1;
	WSADATA wsadata;
	struct sockaddr_in serv, clnt1, clnt2;
	std::map<int, std::string> USER_LIST;
	//std::map<, intstd::string> USER_PORT;  //
	
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(12345);
	
	char str1[100],name[100];
    char *ip; 
    int port;
	int n, i, n1;
	
	n = WSAStartup(0x101,(WSADATA *) &wsadata);
    //n2 = WSAStartup(0x101,(WSADATA *) &wsadata);

	sd1 = socket(AF_INET,SOCK_DGRAM,0);

    int len_clnt1 = sizeof(clnt1);
	int len_clnt2 = sizeof(clnt2);
	bind( sd1,(struct sockaddr*) &serv, len_clnt1 );//n = bind( sd,(struct sockaddr*) &serv, sizeof(serv) );

    printf("Waiting.....\n");

	//n1 = recvfrom (sd1, name ,100, 0,(struct sockaddr*) &clnt1, &len_clnt1 );
	
	char buffer[1024];
    sockaddr_in clientAAddress;
    sockaddr_in clientBAddress;
    int clientAAddressSize = sizeof(clientAAddress);
    int clientBAddressSize = sizeof(clientBAddress);
    //int bytesRead;
	
	while(1){
		/*
		if(n1 = recvfrom(sd1, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAAddress, &clientAAddressSize)){
			ip = inet_ntoa(clnt1.sin_addr);
			port = ntohs(clnt1.sin_port);
			if (USER_LIST.find(port) == USER_LIST.end())
			{
				USER_LIST[port] = str1;
            	printf("%s welcome to chatroom! \n",str1);
			}
			else
				printf("recv:%s from:%s port:%d, ip: %s\n", str1, USER_LIST[port].c_str(), port, ip); 
			sendto(sd1, buffer, n1, 0, (struct sockaddr*)&clientBAddress, clientBAddressSize);
		}

		else {//
		n1 = recvfrom(sd1, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientBAddress, &clientBAddressSize);
			ip = inet_ntoa(clnt1.sin_addr);
			port = ntohs(clnt1.sin_port);
			if (USER_LIST.find(port) == USER_LIST.end())
			{
				USER_LIST[port] = str1;
				printf("%s welcome to chatroom! \n",str1);
			}
			else
				printf("recv:%s from:%s port:%d, ip: %s\n", str1, USER_LIST[port].c_str(), port, ip); 
			sendto(sd1, buffer, n1, 0, (struct sockaddr*)&clientAAddress, clientAAddressSize);
		}
		*/

		
		
		n1 = recvfrom (sd1, str1 ,100, 0, (struct sockaddr*) &clnt1, &len_clnt1 );
        
		ip = inet_ntoa(clnt1.sin_addr);
		port = ntohs(clnt1.sin_port);
		
		//n1 = recvfrom (sd1, str1 ,100, 0, (struct sockaddr*) &clnt2, &len_clnt2 );


		if (USER_LIST.find(port) == USER_LIST.end())
		{
			USER_LIST[port] = str1;
            printf("%s welcome to chatroom! \n",str1);
		}
        
		else
		{ 
			printf("recv:%s from:%s port:%d, ip: %s\n", str1, USER_LIST[port].c_str(), port, ip); 
            
			
			for (auto& pair : USER_LIST) {
				int otherPort = pair.first;
				if (otherPort != port) {
					// 向其他客戶端發送消息
					sockaddr_in otherClnt;
					otherClnt.sin_family = AF_INET;
					otherClnt.sin_port = htons(otherPort);
					otherClnt.sin_addr.s_addr = inet_addr("127.0.0.1");

					sendto(sd1, str1, n1, 0, (struct sockaddr*)&otherClnt, sizeof(otherClnt));
					printf("send:%s ",str1);
				}
			}
			
		}
		
	}
	
	closesocket(sd1);
	WSACleanup();
	system("pause");
}


