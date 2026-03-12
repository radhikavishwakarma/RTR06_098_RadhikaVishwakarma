// .cpp because COM is purely C++ Api's
// Win32 Headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>	// for ceil()
#include "D3D.h"

// D3D11 related headers
#include<d3d11.h>
#include<dxgi.h>
#include<d3dcompiler.h>

// XNAMath Header files
#pragma warning(disable: 4838)
#include"XNAMath/xnamath.h"

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")	

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
// variables related with full screen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// variables related with File I/O
const char gszLogFileName[] = "Log.txt";
FILE* gpFile = NULL;

// Active window related variable
BOOL gbActiveWindow = FALSE;

// Exit key press related
BOOL gbEscapeKeyIsPressed = FALSE;

// x and y coordinates
int xpos = 0;
int ypos = 0;

// Direct X related global variables
IDXGISwapChain *gpIDXGISwapChain = NULL;				// like SwapBuffers
ID3D11Device *gpID3D11Device = NULL;					/// device mhanje physical graphig card
														// la represent krnare logical graphic card
ID3D11DeviceContext *gpID3D11DeviceContext  = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;

struct CBUFFER
{
	XMMATRIX WorldViewProjectionMatrix;
};

XMMATRIX perspectiveProjectionMatrix;


float clearColor[4];

// Entry-point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreveInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// local functions declarations
	HRESULT initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("FIRST WINODW");
	BOOL bDone = FALSE;
	HRESULT hr = S_OK;

	// code
	
	// Create Log File
	gpFile = fopen(gszLogFileName, "w");
	
	if (gpFile == NULL) {
	
		MessageBox(NULL, TEXT("LOG FILE CREATION FAILED"), TEXT("PROGRAM EXIT"), MB_OK);
		exit(0);
	}
	else {

		fprintf(gpFile, "PROGRAM STARTED SUCCESSFULLY !!\n");
	}

	// Window Class Initialization

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registration of window class

	RegisterClassEx(&wndclass);

	// Create Window
	int screenWidth= GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	xpos = (screenWidth - WIN_WIDTH) / 2;
	ypos = (screenHeight - WIN_HEIGHT) / 2;

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Radhika Vishwakarma"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		xpos,
		ypos,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	// initialize global variable for used it in toggleFullScreen function 
	ghwnd = hwnd;

	// Show Window

	ShowWindow(hwnd, iCmdShow);

	// Paint Background Of Window

	UpdateWindow(hwnd);

	// initialize
	hr = initialize();	

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
		fprintf(gpFile,"CreateDXGIFactory() Failed for %d\n", hr);
		fclose(gpFile);
		DestroyWindow(hwnd);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
		fprintf(gpFile,"CreateDXGIFactory() SUCCEDED for %d\n", hr);
		fclose(gpFile);
	}

	// set this window as foreground and active window

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	
			if (gbActiveWindow == TRUE) 
			{	
				if (gbEscapeKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}
				// render
				display();
				// update
				update();
			}
		}
	}

	// uninitialize
	uninitialize();

	return((int)msg.wParam);

}

// callback function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	// local function declarations
	
	void toggleFullScreen(void);
	HRESULT resize(int, int);
	void uninitialize(void);

	// variable
	HRESULT hr = S_OK;

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT));
		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;
	case WM_SIZE:
		if(gpID3D11DeviceContext)
		{
			hr = resize(LOWORD(lParam), HIWORD(lParam));
			if(FAILED(hr))
			{	
				gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
				fprintf(gpFile,"gpID3D11DeviceContext Failed %d\n", hr);
				fclose(gpFile);
				return(hr);
			}
		}
		
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = TRUE;
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE) {
				toggleFullScreen();
				gbFullScreen = TRUE;
			}
			else {
				toggleFullScreen();
				gbFullScreen = FALSE;
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		uninitialize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void toggleFullScreen(void) {

	// variable declarations
	MONITORINFO mi;

	// code
	if (gbFullScreen == FALSE) {
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			ZeroMemory((void*)&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				// to expand window border
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);		//height								// width
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, (mi.rcMonitor.right - mi.rcMonitor.left), (mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}

		ShowCursor(FALSE);
	}
	else {

		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

HRESULT initialize(void) {

	// function declarations
    void PrintDXInfo(void);
	HRESULT resize(int,int);

	// variable declarations
	HRESULT hr = S_OK;

	// code
	
	// SwapChainDescriptor Initialization
	DXGI_SWAP_CHAIN_DESC DXGISWAPCHAINDESC;
	ZeroMemory((void*)&DXGISWAPCHAINDESC, sizeof(DXGI_SWAP_CHAIN_DESC));

	DXGISWAPCHAINDESC.BufferDesc.Width = WIN_WIDTH;
	DXGISWAPCHAINDESC.BufferDesc.Height = WIN_HEIGHT;
	DXGISWAPCHAINDESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	DXGISWAPCHAINDESC.BufferDesc.RefreshRate.Numerator = 60;	// frame rate
	DXGISWAPCHAINDESC.BufferDesc.RefreshRate.Denominator = 1;

	DXGISWAPCHAINDESC.BufferCount = 1;	// dedicated new buffer
	DXGISWAPCHAINDESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// FOR TEXTURE SAMPLING HOW MAKE (for multi sampling)
	DXGISWAPCHAINDESC.SampleDesc.Count = 1;
	DXGISWAPCHAINDESC.SampleDesc.Quality = 0;	// default smapling quality

	DXGISWAPCHAINDESC.OutputWindow = ghwnd;
	DXGISWAPCHAINDESC.Windowed = TRUE;

	// Get DXGISwapChain, D3D11Device, D3D11DeviceContext supported driver
	// And supported feature level at Once ----->
	D3D_DRIVER_TYPE D3D11DRIVERTYPE;
	D3D_DRIVER_TYPE D3D11DRIVERTYPES[] = {
											D3D_DRIVER_TYPE_HARDWARE, 
											D3D_DRIVER_TYPE_WARP, 
											D3D_DRIVER_TYPE_SOFTWARE,
											D3D_DRIVER_TYPE_REFERENCE	// for debugging driver
										   };
	D3D_FEATURE_LEVEL D3D11FEATURELEVEL_REQUIRED = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL D3D11FEATURELEVEL_ACQUIRED = D3D_FEATURE_LEVEL_10_0;
	
	UINT NumDriverTypes = sizeof(D3D11DRIVERTYPES) / sizeof(D3D11DRIVERTYPES[0]);

	for(UINT i = 0; i < NumDriverTypes; ++i)
	{
		D3D11DRIVERTYPE = D3D11DRIVERTYPES[i];

		hr = D3D11CreateDeviceAndSwapChain(
			NULL,	// graphic card adapter
			D3D11DRIVERTYPE,
			NULL,
			0,
			&D3D11FEATURELEVEL_REQUIRED, 
			1,
			D3D11_SDK_VERSION,
			&DXGISWAPCHAINDESC,
			&gpIDXGISwapChain,
			&gpID3D11Device,
			&D3D11FEATURELEVEL_ACQUIRED,
			&gpID3D11DeviceContext
		);

		if(SUCCEEDED(hr))
		{
			break;
		}
	}

	// when hr failed in loop
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
		fprintf(gpFile,"D3D11CreateDeviceAndSwapChain() Failed %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	
	gpFile = fopen(gszLogFileName, "a+");

	// check which driver it uses
	if(D3D11DRIVERTYPE == D3D_DRIVER_TYPE_HARDWARE)
	{
		fprintf(gpFile,"Choosen Driver is Hardware Driver\n");
	}
	else if(D3D11DRIVERTYPE == D3D_DRIVER_TYPE_WARP)
	{
		fprintf(gpFile,"Choosen Driver is WARP\n");
	}
	else if(D3D11DRIVERTYPE == D3D_DRIVER_TYPE_SOFTWARE)
	{
		fprintf(gpFile,"Choosen Driver is Software Driver\n");
	}
	else if(D3D11DRIVERTYPE == D3D_DRIVER_TYPE_REFERENCE)
	{
		fprintf(gpFile,"Choosen Driver is Reference Driver\n");
	}
	else 
	{
		fprintf(gpFile,"Choosen Driver is Unknown\n");
	}

	// check which feature level it acquired
	if(D3D11FEATURELEVEL_ACQUIRED == D3D_FEATURE_LEVEL_11_0)
	{
		fprintf(gpFile,"Feature Level 11.0 is aquired\n");
	}
	else if(D3D11FEATURELEVEL_ACQUIRED == D3D_FEATURE_LEVEL_10_0)
	{
		fprintf(gpFile,"Feature Level 10.0 is aquired\n");
	}
	else if(D3D11FEATURELEVEL_ACQUIRED == D3D_FEATURE_LEVEL_10_1)
	{
		fprintf(gpFile,"Feature Level 10.1 is aquired\n");
	}
	else
	{
		fprintf(gpFile,"Old Feature Level Aquired\n");
	}

	fclose(gpFile);

	PrintDXInfo();

	// VERTEX SHADER
	const char *vertexShaderSourceCode =
		"cbuffer ConstantBuffer\n" \
		"{\n" \
		"float4x4 worldViewProjectionMatrix;\n" \
		"}\n" \
		"float4 main(float4 pos : POSITION) : SV_POSITION\n" \
		"{\n" \
		"   float4 position = mul(worldViewProjectionMatrix, pos);\n" \
		"   return(position);\n" \
		"}\n";

		// 2. Compile the shader programatically
		ID3DBlob *pID3DBlobVertexShaderCode = NULL;		
		ID3DBlob *pID3DBlob_error = NULL;		
		hr = D3DCompile(
			vertexShaderSourceCode,
			lstrlenA(vertexShaderSourceCode) + 1,
			"VS",	// vertex shader
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",	// vertex shader model 5.0
			0,
			0,
			&pID3DBlobVertexShaderCode,
			&pID3DBlob_error
		
		);

	if(FAILED(hr))
	{
		if(pID3DBlob_error != NULL)
		{
			gpFile = fopen(gszLogFileName, "a+");
			fprintf(gpFile,"Vertex Shader Compilation Failed : %s \n", (char*)pID3DBlob_error->GetBufferPointer());
			fclose(gpFile);
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"Vertex Shader Compilation Succeeded. \n");
		fclose(gpFile);
	}

	// 3. Create the vertex shader
	hr = gpID3D11Device->CreateVertexShader(
		pID3DBlobVertexShaderCode->GetBufferPointer(),
		pID3DBlobVertexShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11VertexShader
	);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateVertexShader() Failed for %d \n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateVertexShader() Succeeded. \n");
		fclose(gpFile);
	}
	
	// 4. Set the vertex shader in the pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, 0, 0);

	// -------------------------------------------------------------
	// PIXEL SHADER
	const char *pixelShaderSourceCode =
		"float4 main(void) : SV_TARGET\n" \
		"{\n" \
		"   float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);\n" \
		"   return(color);\n" \
		"}\n";

		// 2. Compile the shader programatically
		ID3DBlob *pID3DBlobPixelShaderCode = NULL;		
		pID3DBlob_error = NULL;		
		hr = D3DCompile(
			pixelShaderSourceCode,
			lstrlenA(pixelShaderSourceCode) + 1,
			"PS",	// pixel shader
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ps_5_0",	// pixel shader model 5.0
			0,
			0,
			&pID3DBlobPixelShaderCode,
			&pID3DBlob_error
		);

	if(FAILED(hr))
	{
		if(pID3DBlob_error != NULL)
		{
			gpFile = fopen(gszLogFileName, "a+");
			fprintf(gpFile,"Pixel Shader Compilation Failed : %s \n", (char*)pID3DBlob_error->GetBufferPointer());
			fclose(gpFile);
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"Pixel Shader Compilation Succeeded. \n");
		fclose(gpFile);
	}

	// 3. Create the pixel shader
	hr = gpID3D11Device->CreatePixelShader(
		pID3DBlobPixelShaderCode->GetBufferPointer(),
		pID3DBlobPixelShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11PixelShader
	);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreatePixelShader() Failed for %d \n", hr);
		pID3DBlobPixelShaderCode->Release();
		pID3DBlobPixelShaderCode = NULL;
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreatePixelShader() Succeeded. \n");
		fclose(gpFile);
	}
	
	// 4. Set the pixel shader in the pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, 0, 0);

	// Initialize Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc;
	ZeroMemory((void*)&inputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
	inputElementDesc.SemanticName = "POSITION";
	inputElementDesc.SemanticIndex = 0;
	inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDesc.InputSlot = 0;
	inputElementDesc.AlignedByteOffset = 0;
	inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDesc.InstanceDataStepRate = 0;
	hr = gpID3D11Device->CreateInputLayout(
		&inputElementDesc,
		1,
		pID3DBlobVertexShaderCode->GetBufferPointer(),
		pID3DBlobVertexShaderCode->GetBufferSize(),
		&gpID3D11InputLayout
	);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateInputLayout() Failed for %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateInputLayout() Succeeded.\n");
		fclose(gpFile);
	}

	// 5. Set the input layout in the pipeline
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	// Release blobs
	pID3DBlobVertexShaderCode->Release();
	pID3DBlobVertexShaderCode = NULL;
	pID3DBlobPixelShaderCode->Release();
	pID3DBlobPixelShaderCode = NULL;

	// Provide vertex position, color, normals, texcoords etc
	const float rectanglePositions[] =
	{
		// 1st triangle
		-1.0f,  1.0f, 0.0f, // top left
		1.0f, 1.0f, 0.0f, // top right
		-1.0f, -1.0f, 0.0f,  // bottom left
		
		// 2nd triangle
		-1.0f, -1.0f, 0.0f,  // bottom left
		1.0f, 1.0f, 0.0f, // top right
		1.0f, -1.0f, 0.0f   // bottom right
	};
	
	// Create buffer for vetex data
	D3D11_BUFFER_DESC d3d11BufferDesc;
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to GL_STATIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(rectanglePositions);
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	// Initialize subresource of buffer for vertex data
	D3D11_SUBRESOURCE_DATA d3d11SubResourceData;
	ZeroMemory((void*)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubResourceData.pSysMem = rectanglePositions;
	
	// create vertex buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, &d3d11SubResourceData, &gpID3D11Buffer_PositionBuffer);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Position Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Position Buffer.\n");
		fclose(gpFile);
	}

	// create constant buffer
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create and set above empty buffer into pipeline
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Constant Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Constant Buffer.\n");
		fclose(gpFile);
	}
	// set constant buffer into vertex shader stage of pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	// Set projection matrix to identity matrix
	perspectiveProjectionMatrix = XMMatrixIdentity();

	// set clear color
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;
	clearColor[3] = 1.0f;

	// perspective projection matrix initialization
	perspectiveProjectionMatrix = XMMatrixIdentity();

	hr = resize(WIN_WIDTH,WIN_HEIGHT);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
		fprintf(gpFile,"hr initialized Failed %d\n", hr);
		
		return(hr);
	}
	else
	{
		fprintf(gpFile,"hr successfully initialized %d\n", hr);
		fclose(gpFile);
	}

	return(hr);
}

void PrintDXInfo(void)
{
    // variable declarations
    IDXGIFactory * pIDXGIFactory = NULL;    // struct pointer
    IDXGIAdapter * pIDXGIAdapter = NULL;    // struct pointer
    DXGI_ADAPTER_DESC dxgiAdapterDesc;      // struct normal variable
    HRESULT hr = S_OK; // S_OK means success ok(boolean value)

    char str[255];

    // code
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);

    if(FAILED(hr))
    {
        fprintf(gpFile,"CreateDXGIFactory() Failed for %d\n", hr);
        goto cleanup;
    }

    // From Factory get Adapter
    if(pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        ZeroMemory((void*)& dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));

        pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);

        // to convert WCHAR type name of graphic card into char type

        WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str,255, NULL, NULL);

        fprintf(gpFile,"Graphic Device Name = %s\n", str);
        fprintf(gpFile,"VRAM(in Byte):-  %I64d\n", (__int64)dxgiAdapterDesc.DedicatedVideoMemory);

        fprintf(gpFile,"VRAM(in GB):- %d\n", (int)ceil(dxgiAdapterDesc.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0));
    }
    else
    {
        fprintf(gpFile,"IDXGIFactory::EnumAdapters() Failed\n");
        goto cleanup;
    }
	
    cleanup:
        if(pIDXGIAdapter)
        {
            pIDXGIAdapter->Release();
            pIDXGIAdapter = NULL;
        }
        if(pIDXGIFactory)
        {
            pIDXGIFactory->Release();
            pIDXGIFactory = NULL;
        }

}

HRESULT resize(int width, int height) {

	HRESULT hr = S_OK;

	// code

	// Release Render Target view
	if(gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	// resize swap chain buffers according to new size
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// get the buffer from swap chain for render target view
	ID3D11Texture2D *pID3D11Texture2D_backBuffer = NULL;
	gpIDXGISwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D), (void**)&pID3D11Texture2D_backBuffer); 

	// create render target view using above textured swapchain buffer
	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_backBuffer, NULL, &gpID3D11RenderTargetView);

	if(FAILED(hr))
	{
		pID3D11Texture2D_backBuffer->Release();
		pID3D11Texture2D_backBuffer = NULL;
		gpFile = fopen(gszLogFileName, "a+");	// + for jr file nsel tr tyar kr file
		fprintf(gpFile,"D3D11Device::CreateRenderTargetView Failed %d\n", hr);
		fclose(gpFile);
		return(hr);
	}

	pID3D11Texture2D_backBuffer->Release();
	pID3D11Texture2D_backBuffer = NULL;

	// set this render target view in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

	// initalize viewport sturcture
	D3D11_VIEWPORT D3D11VIEWPORT;
	ZeroMemory((void*)&D3D11VIEWPORT, sizeof(D3D11_VIEWPORT));
	D3D11VIEWPORT.TopLeftX = 0.0f;
	D3D11VIEWPORT.TopLeftY = 0.0f;
	D3D11VIEWPORT.Width = float(width);
	D3D11VIEWPORT.Height = float(height);
	D3D11VIEWPORT.MinDepth = 0.0f;
	D3D11VIEWPORT.MaxDepth = 1.0f;

	gpID3D11DeviceContext->RSSetViewports(1, &D3D11VIEWPORT);

	// set perspective projection matrix
	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), float(width) / float(height), 0.1f, 100.0f);

	return(hr);
	
}

void display(void) {

	// code

	// clear color
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);

	// Set Vertex Buffer here created into initialize function
	UINT stride = sizeof(float) * 3; // 3 for x,y,z
	UINT offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	// set primitive topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Transformations
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 5.0f); // move triangle away from camera
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpmatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	// load the wvpmatrix data into constant buffer
	CBUFFER constantBuffer;
	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));
	constantBuffer.WorldViewProjectionMatrix = wvpmatrix;

	gpID3D11DeviceContext->UpdateSubresource(
		gpID3D11Buffer_ConstantBuffer,
		0,
		NULL,
		&constantBuffer,
		0,
		0
	);

	// draw triangle
	gpID3D11DeviceContext->Draw(6, 0);

	// Present the swapchain buffers to the swapchain
	gpIDXGISwapChain->Present(0,0);


}

void update(void) {

	// code
}

void uninitialize(void) {

	// code
	// release COM objects
	if(gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer)
	{
		gpID3D11Buffer_PositionBuffer->Release();
		gpID3D11Buffer_PositionBuffer = NULL;
	}
	if(gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}
	if(gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	if(gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}
	if(gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}
	
	if(gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();
		gpID3D11DeviceContext = NULL;
	}
	if(gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();
		gpIDXGISwapChain = NULL;
	}

	if(gpID3D11Device)
	{
		gpID3D11Device->Release();
		gpID3D11Device = NULL;
	}

	// close the log file
	if (gpFile) 
	{
		fprintf(gpFile, "PROGRAM TERMINATED SUCCESSFULLY !!");
		fclose(gpFile);
		gpFile = NULL;
	}

	
}


