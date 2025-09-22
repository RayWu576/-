#include <stdio.h>
#include <winsock.h>
#include <conio.h>
#include <string>
#include <windows.h>
#include <tchar.h>

#define WM_SOCKET (WM_USER + 1)

int ID_LISTBOX;
HWND textfield, button;
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
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        "My window",       /* Title Text */
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
    
	
	switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
    {
        WSADATA wsadata;
        int n = WSAStartup(0x101, (WSADATA*)&wsadata);
        if (n != 0)
        {
            // ?理初始化失?的情?
            // ...
        }

        sd = socket(AF_INET, SOCK_DGRAM, 0);
		
		
        /*
        // ?套接字和服?器信息保存在窗口?据中
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)sd);
        SetProp(hwnd, _T("SERVER_INFO"), (HANDLE)&serv1);*/
        TextBox = CreateWindow("EDIT",
            "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            10, 10, 400, 20,
            hwnd, NULL, NULL, NULL);
        button = CreateWindow("BUTTON",
            "Send",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            420, 10, 70, 20,
            hwnd, (HMENU)1, NULL, NULL);

        listBox = CreateWindow(_T("LISTBOX"), NULL,
            WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS,
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

                // ?送消息到服?器
                
                sockaddr_in serv1;
			    serv1.sin_addr.s_addr = inet_addr("127.0.0.1");
			    serv1.sin_family = AF_INET;
			    serv1.sin_port = htons(12345);
                int len_serv = sizeof(sockaddr_in);
                int n = sendto(sd, message.c_str(), message.length() + 1, 0, (struct sockaddr*)&serv1, len_serv);
                if (n == SOCKET_ERROR)
                {
                    // ?理?送失?的情?
                    // ...
                }

                // 清空文本框?容
                SetWindowText(TextBox, "");
                
				
                // 在列表框中插入新的文本
                SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)"Send:");
                SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)message.c_str());

            }
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
    {
        SOCKET sd = (SOCKET)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        closesocket(sd);
        // 清理 Winsock
        WSACleanup();
        PostQuitMessage(0);
        break;
    }
    /*
    case WM_SOCKET:
    {
    	if (WSAGETSELECTEVENT(lParam) == FD_READ)
    	{
    		
			    // 套接字準備好進行讀取操作，可以接收數據
			    char buffer[100];
			    struct sockaddr_in serverAddr;
			    int serverAddrLen = sizeof(serverAddr);
				
			    // 使用 recvfrom 函式接收數據
			    int n = recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
				printf("%s",buffer);
			    if (n > 0) {
			        // 將接收到的數據轉換為字串格式
			        buffer[n] = '\0';
					
					HWND listBox = GetDlgItem(hwnd, ID_LISTBOX);
			        // 在這裡處理接收到的數據，例如顯示在介面上或進行其他操作
			        SendMessageA(listBox, LB_INSERTSTRING, 0, (LPARAM)"Received:");
                    SendMessageA(listBox, LB_INSERTSTRING, 0, (LPARAM)buffer);
			    } else if (n == 0) {
			        // 連接已關閉
			        // 在這裡處理連接關閉的情況
			    } else {
			        // 接收失敗
			        // 在這裡處理接收失敗的情況
			    }
			

		} 
	} 
	*/
    
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
		   /* serverAddress.sin_family = AF_INET;
		    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
		    serverAddress.sin_port = htons(12345);*/
		    char buffer[1024];
		    int serverAddressSize = sizeof(serverAddress);
		    				
			
			//printf("Ready to recv on 12345...\n");
			while(1){
				int bytesRead = recvfrom(clientASocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddress, &serverAddressSize);
				//n = recvfrom (sd, str ,100, 0,(struct sockaddr*) &send, &len_send );
				//printf("recv:%s (%d)\n",str,n);
				buffer[bytesRead] = '\0';
                SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)"Received:");
                printf("R:%s",buffer);
                SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)buffer);
				
			}
            
        }
        
        break;
    }
    
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
