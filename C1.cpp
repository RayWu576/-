#include <stdio.h>
#include <winsock.h>
#include <conio.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <unistd.h>

#define WM_SOCKET (WM_USER + 0x10)

int ID_LISTBOX;
const int IMAGE_VIEW = 1001;  
HWND textfield, button,button1;
char TextSaved[50];
HWND TextBox, listBox;
SOCKET sd;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);


/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wincl.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(64, 224, 208));
	


    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        "ChatRoom",       	 /* Title Text */
        WS_MINIMIZEBOX | WS_SYSMENU, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        500,                 /* The programs width */
        500,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

		
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WSAAsyncSelect(sd, hwnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE);
	
	switch (message)                  /* handle the messages */
    {
		case WM_CREATE:
		{
			WSADATA wsadata;
			int n = WSAStartup(0x101, (WSADATA*)&wsadata);
			if (n != 0)
			{
				
			}

			sd = socket(AF_INET, SOCK_DGRAM, 0);
			
			
			TextBox = CreateWindowA("EDIT",
				"",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				10, 10, 400, 20,
				hwnd, NULL, NULL, NULL);
			button = CreateWindowA("BUTTON",
				"SEND",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				420, 10, 70, 20,
				hwnd, (HMENU)1, NULL, NULL);

			button1 = CreateWindowA("BUTTON",
				"UNSEND",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				420, 40, 70, 20,
				hwnd, (HMENU)2, NULL, NULL);
			
			listBox = CreateWindow(_T("LISTBOX"), NULL,
				WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | WS_VSCROLL | WS_HSCROLL,
				10, 50, 400, 400,
				hwnd, NULL, NULL, NULL);

			SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)_T("Enter Your Name:"));
			
			break;
		}
		
		
		
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 1:
				{
					
					int length = GetWindowTextLength(TextBox);
					if (length > 0)
					{
						std::string message(length + 1, '\0');
						GetWindowTextA(TextBox, &message[0], length + 1);

						sockaddr_in serv1;
						serv1.sin_addr.s_addr = inet_addr("127.0.0.1");
						serv1.sin_family = AF_INET;
						serv1.sin_port = htons(12345);
						int len_serv = sizeof(sockaddr_in);
						int n = sendto(sd, message.c_str(), message.length() + 1, 0, (struct sockaddr*)&serv1, len_serv);
						if (n == SOCKET_ERROR)
						{
							
						}

						SetWindowText(TextBox, "");
						
						// 取得日期/時間
						time_t now = time(0);
						// 把 now 轉換為字符串形式
						char* dt = ctime(&now);
						
						SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)"Send:");
						SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)message.c_str());
						SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)dt);
					}
					
					break;
				}
				case 2:
				{
					int itemCount = SendMessage(listBox, LB_GETCOUNT, 0, 0);
					char buffer1[256];
    				int index = itemCount - 3;
					SendMessage(listBox, LB_GETTEXT, index, (LPARAM)buffer1);
					if (strcmp(buffer1, "Send:") == 0){
						int index1 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 1;
						int index2 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 2;  
						int index3 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 3;
						SendMessage(listBox, LB_DELETESTRING, index1, 0);  
						SendMessage(listBox, LB_DELETESTRING, index2, 0);
						SendMessage(listBox, LB_DELETESTRING, index3, 0);
								
						sockaddr_in serv1;
						serv1.sin_addr.s_addr = inet_addr("127.0.0.1");
						serv1.sin_family = AF_INET;
						serv1.sin_port = htons(12345);
						int len_serv = sizeof(sockaddr_in);
							
						std::string deleteRequest = "DELETE_LAST_MESSAGE";
						sendto(sd, deleteRequest.c_str(), deleteRequest.length() + 1, 0, (struct sockaddr*)&serv1, len_serv);
						
						break;
					}
					
					// int index1 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 1;
					// int index2 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 2;  
					// int index3 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 3;
					// SendMessage(listBox, LB_DELETESTRING, index1, 0);  
					// SendMessage(listBox, LB_DELETESTRING, index2, 0);
					// SendMessage(listBox, LB_DELETESTRING, index3, 0);
							
					// sockaddr_in serv1;
					// serv1.sin_addr.s_addr = inet_addr("127.0.0.1");
					// serv1.sin_family = AF_INET;
					// serv1.sin_port = htons(12345);
					// int len_serv = sizeof(sockaddr_in);
						
					// std::string deleteRequest = "DELETE_LAST_MESSAGE";
    				// sendto(sd, deleteRequest.c_str(), deleteRequest.length() + 1, 0, (struct sockaddr*)&serv1, len_serv);
					
					// break;
				}

			}
			
			break;
		}
		
		case WM_DESTROY:
		{
			SOCKET sd = (SOCKET)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			closesocket(sd);
			
			WSACleanup();
			PostQuitMessage(0);
			break;
		}
		
		
		case WM_SOCKET:
		{
			if (WSAGETSELECTEVENT(lParam) == FD_READ)
			{
				SOCKET clientASocket = socket(AF_INET, SOCK_DGRAM, 0);
				if (clientASocket == INVALID_SOCKET) {
					//std::cout << "無法建立套接字" << std::endl;
					WSACleanup();
					return 1;
				}
				
				sockaddr_in serverAddress;
				serverAddress.sin_family = AF_INET;
				serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
				serverAddress.sin_port = htons(12345);
				char buffer[1024];
				int serverAddressSize = sizeof(serverAddress);

							
				
				while(1){
					memset(buffer, '\0', 1024);
					if(recvfrom(wParam, buffer, sizeof(buffer), 0, (struct sockaddr*) &serverAddress, &serverAddressSize) > 0)
					{
						
						if (strcmp(buffer, "DELETE_LAST_MESSAGE") == 0)
					    {
					        int index1 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 1;
							int index2 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 2;  
							int index3 = SendMessage(listBox, LB_GETCOUNT, 0, 0) - 3;
							SendMessage(listBox, LB_DELETESTRING, index1, 0);  
							SendMessage(listBox, LB_DELETESTRING, index2, 0);
							SendMessage(listBox, LB_DELETESTRING, index3, 0);
							break;
					    }
					    
					    else if (strcmp(buffer, "DELETE_LAST_MESSAGE") != 0){

							SendMessageA(listBox, LB_ADDSTRING, 0, (LPARAM)"Received:" );
							printf("R:%s\n",buffer);
							SendMessageA(listBox, LB_ADDSTRING, 0, (LPARAM)buffer);
							time_t now = time(0);
							char* dt = ctime(&now);
							SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)dt);
						}	
					}
					
					break;
				}
				break;
			}
			
			break;
		}
    
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
