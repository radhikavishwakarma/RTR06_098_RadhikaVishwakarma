// Win32 Headers
#include<Windows.h>

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");

	// code
	// Window Class Initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Registraion of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindow(
		szAppName,
		TEXT("Radhika Vishwakarma"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Point Background & Window
	UpdateWindow(hwnd);

	// Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);

}

// Callback Function
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	TCHAR str[] = TEXT("Hello World");
	static int iPaintFlag = -1;

	// code
	switch (iMsg)
	{
		case WM_CHAR:
			switch (wParam)
			{
				case 'R':
				case 'r':
					iPaintFlag = 1;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'G':
				case 'g':
					iPaintFlag = 2;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'B':
				case 'b':
					iPaintFlag = 3;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'Y':
				case 'y':
					iPaintFlag = 4;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'C':
				case 'c':
					iPaintFlag = 5;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'K':
				case 'k':
					iPaintFlag = 6;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 'W':
				case 'w':
					iPaintFlag = 7;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				
				default:
					break;
			}
			break;

		case WM_PAINT:
			GetClientRect(hWnd, &rc);
			hdc = BeginPaint(hWnd, &ps);
			SetBkColor(hdc, RGB(0, 0, 0));
			if (iPaintFlag == 1)
				SetTextColor(hdc, RGB(255, 0, 0));
			else if (iPaintFlag == 2)
				SetTextColor(hdc, RGB(0, 255, 0));
			else if (iPaintFlag == 3)
				SetTextColor(hdc, RGB(0, 0, 255));
			else if (iPaintFlag == 4)
				SetTextColor(hdc, RGB(255, 255, 0));
			else if (iPaintFlag == 5)
				SetTextColor(hdc, RGB(0, 255, 255));
			else if (iPaintFlag == 6)
				SetTextColor(hdc, RGB(0, 0, 0));
			else if (iPaintFlag == 7)
				SetTextColor(hdc, RGB(255, 255, 255));

			DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		default:
			break;
	}

	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
