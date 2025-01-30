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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	// code
	switch (iMsg)
	{
		case WM_CREATE:
			MessageBox(hWnd, TEXT("This is the first Message"), TEXT("WM_CREATE"), MB_OK);
			break;

		case WM_SIZE:
			MessageBox(hWnd, TEXT("Window is resized"), TEXT("WM_SIZE"), MB_OK);
			break;
		
		case WM_MOVE:
			MessageBox(hWnd, TEXT("Window is moved"), TEXT("WM_MOVE"), MB_OK);
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				MessageBox(hWnd, TEXT("Esape key is pressed"), TEXT("VK_ESCAPE"), MB_OK);
				break;
			
			default:
				break;
			}
			break;
		
		case WM_CHAR:
			switch (wParam)
			{
			case 'F':
				MessageBox(hWnd, TEXT("Capital 'F' Key is pressed"), TEXT("WM_CHAR"), MB_OK);
				break;
			
			case 'f':
				MessageBox(hWnd, TEXT("Capital 'f' Key is pressed"), TEXT("WM_CHAR"), MB_OK);
				break;
			
			default:
				break;
			}
			break;
		
		case WM_LBUTTONDOWN:
			MessageBox(hWnd, TEXT("Left mouse button is clicked"), TEXT("WM_LBUTTONDOWN"), MB_OK);
			break;
		
		case WM_CLOSE:
			MessageBox(hWnd, TEXT("Window is closing"), TEXT("WM_CLOSE"), MB_OK);
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		default:
			break;
	}

	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
