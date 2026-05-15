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

// For D3D11 Math
#pragma warning(disable:4838)
#include "XNAMath/xnamath.h"

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
ID3D11Device *gpID3D11Device = NULL;					
														
ID3D11DeviceContext *gpID3D11DeviceContext  = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;														

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11HullShader *gpID3D11HullShader = NULL;
ID3D11DomainShader *gpID3D11DomainShader = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;

struct CBUFFER_HULL_SHADER
{
	XMVECTOR Hull_Constant_Function_Params;
};

struct CBUFFER_DOMAIN_SHADER
{
	XMMATRIX worldViewProjectionMatrix;
};

struct CBUFFER_PIXEL_SHADER
{
	XMVECTOR lineColor;
};

unsigned int uiNumberofLineSegments = 1;
XMVECTOR gLineColor = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);

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
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"CreateDXGIFactory() Failed for %d\n", hr);
		fclose(gpFile);
		DestroyWindow(hwnd);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
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
				gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
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
		case VK_UP:
			uiNumberofLineSegments++;
			if (uiNumberofLineSegments > 30)
			{
				uiNumberofLineSegments = 30;
			}
			break;
		case VK_DOWN:
			uiNumberofLineSegments--;
			if (uiNumberofLineSegments < 1)
			{
				uiNumberofLineSegments = 1;
			}
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
		case 'R':
		case 'r':
			gLineColor = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 'G':
		case 'g':
			gLineColor = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 'B':
		case 'b':
			gLineColor = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
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
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
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
	// 1. Write shader source code
	const char* vertexShaderSourceCode =
		"struct VS_OUTPUT\n" \
		"{\n" \
		"    float4 position : POSITION;\n" \
		"};\n" \
		"VS_OUTPUT main(float2 pos : POSITION)\n" \
		"{\n" \
		"    VS_OUTPUT output;\n" \
		"    output.position = float4(pos, 0.0, 1.0);\n" \
		"    return output;\n" \
		"}\n";

	// 2. Compile the shader programmatically
	ID3DBlob *pID3DBlobVertexShaderCode = NULL;
	ID3DBlob *pID3DBlob_error = NULL;
	hr = D3DCompile(
				vertexShaderSourceCode,
				lstrlenA(vertexShaderSourceCode) + 1,
				"VS", // Vertex Shader
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"vs_5_0",
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
		fprintf(gpFile,"Vertex Shader Compilation Succeeded \n");
		fclose(gpFile);
	}

	// 3. Create the Vertex Shader
	hr = gpID3D11Device->CreateVertexShader(
			pID3DBlobVertexShaderCode->GetBufferPointer(),
			pID3DBlobVertexShaderCode->GetBufferSize(),
			NULL,
			&gpID3D11VertexShader
		);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateVertexShader() Failed for %d\n", hr);
		fclose(gpFile);
		pID3DBlobVertexShaderCode->Release();
		pID3DBlobVertexShaderCode = NULL;
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateVertexShader() Succeeded for %d\n", hr);
		fclose(gpFile);
	}
	
	// 4.  set vertex shader into pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	// ----------------------------------
	// HULL SHADER
	const char* hullShaderSourceCode =
		"cbuffer ConstantBuffer\n" \
		"{\n" \
			"float4 hull_constant_function_params;\n" \
		"};\n" \
		"struct vertex_output\n" \
		"{\n" \
			"float4 position : POSITION;\n" \
		"};\n" \
		"struct hull_constant_output\n" \
		"{\n" \
			"float edges[2] : SV_TESSFACTOR;\n" \
		"};\n" \
		"hull_constant_output hull_constant_function(void)\n" \
		"{\n" \
			"hull_constant_output output;\n" \
			"float numberOfLineStrips = hull_constant_function_params[0];\n" \
			"float numberOfSegments = hull_constant_function_params[1];\n" \
			"output.edges[0] = numberOfLineStrips;\n" \
			"output.edges[1] = numberOfSegments;\n" \
			"return(output);\n" \
		"}\n" \
		"struct hull_output\n" \
		"{\n" \
			"float4 position : POSITION;\n" \
		"};\n" \
		"[domain(\"isoline\")]\n" \
		"[partitioning(\"integer\")]\n" \
		"[outputtopology(\"line\")]\n" \
		"[outputcontrolpoints(4)]\n" \
		"[patchconstantfunc(\"hull_constant_function\")]\n" \
		"hull_output main(InputPatch<vertex_output, 4> input_patch, uint i : SV_OUTPUTCONTROLPOINTID)\n" \
		"{\n" \
			"hull_output output;\n" \
			"output.position = input_patch[i].position;\n" \
			"return(output);\n" \
		"}\n";

	// 2. Compile the shader programmatically
	ID3DBlob *pID3DBlobHullShaderCode = NULL;
	pID3DBlob_error = NULL;
	hr = D3DCompile(
				hullShaderSourceCode,
				lstrlenA(hullShaderSourceCode) + 1,
				"HS", // Hull Shader
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"hs_5_0",
				0,
				0,
				&pID3DBlobHullShaderCode,
				&pID3DBlob_error
			);

	if(FAILED(hr))
	{
		if(pID3DBlob_error != NULL)
		{
			gpFile = fopen(gszLogFileName, "a+");
			fprintf(gpFile,"Hull Shader Compilation Failed : %s \n", (char*)pID3DBlob_error->GetBufferPointer());
			fclose(gpFile);
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"Hull Shader Compilation Succeeded \n");
		fclose(gpFile);
	}

	// 3. Create the Hull Shader
	hr = gpID3D11Device->CreateHullShader(
			pID3DBlobHullShaderCode->GetBufferPointer(),
			pID3DBlobHullShaderCode->GetBufferSize(),
			NULL,
			&gpID3D11HullShader
		);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateHullShader() Failed for %d\n", hr);
		fclose(gpFile);
		pID3DBlobHullShaderCode->Release();
		pID3DBlobHullShaderCode = NULL;
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateHullShader() Succeeded for %d\n", hr);
		fclose(gpFile);
	}

	// 4.  set hull shader into pipeline
	gpID3D11DeviceContext->HSSetShader(gpID3D11HullShader, NULL, 0);

	// DOMAIN SHADER
	const char* domainShaderSourceCode =
		"cbuffer ConstantBuffer\n" \
		"{\n" \
			"float4x4 worldViewProjectionMatrix;\n" \
		"};\n" \
		"struct hull_constant_output\n" \
		"{\n" \
			"float edges[2] : SV_TESSFACTOR;\n" \
		"};\n" \
		"struct hull_output\n" \
		"{\n" \
			"float4 position : POSITION;\n" \
		"};\n" \
		"struct domain_output\n" \
		"{\n" \
			"float4 position : SV_POSITION;\n" \
		"};\n" \
		"[domain(\"isoline\")]\n" \
		"domain_output main(hull_constant_output input, OutputPatch<hull_output, 4> output_patch, float2 tessCoord : SV_DOMAINLOCATION)\n" \
		"{\n" \
			"domain_output output;\n" \
			"float4 p0 = output_patch[0].position;\n" \
			"float4 p1 = output_patch[1].position;\n" \
			"float4 p2 = output_patch[2].position;\n" \
			"float4 p3 = output_patch[3].position;\n" \
			"float4 p = p0 * (1.0 - tessCoord.x) * (1.0 - tessCoord.x) * (1.0 - tessCoord.x) + p1 * 3.0 * tessCoord.x * (1.0 - tessCoord.x) * (1.0 - tessCoord.x) + p2 * 3.0 * tessCoord.x * tessCoord.x * (1.0 - tessCoord.x) + p3 * tessCoord.x * tessCoord.x * tessCoord.x;\n" \
			"output.position = mul(worldViewProjectionMatrix, p);\n" \
			"return(output);\n" \
		"}\n";

	// 2. Compile the shader programmatically
	ID3DBlob *pID3DBlobDomainShaderCode = NULL;
	pID3DBlob_error = NULL;
	hr = D3DCompile(
				domainShaderSourceCode,
				lstrlenA(domainShaderSourceCode) + 1,
				"DS", // Domain Shader
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"ds_5_0",
				0,
				0,
				&pID3DBlobDomainShaderCode,
				&pID3DBlob_error
			);

	if(FAILED(hr))
	{
		if(pID3DBlob_error != NULL)
		{
			gpFile = fopen(gszLogFileName, "a+");
			fprintf(gpFile,"Domain Shader Compilation Failed : %s \n", (char*)pID3DBlob_error->GetBufferPointer());
			fclose(gpFile);
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"Domain Shader Compilation Succeeded \n");
		fclose(gpFile);
	}

	// 3. Create the Domain Shader
	hr = gpID3D11Device->CreateDomainShader(
			pID3DBlobDomainShaderCode->GetBufferPointer(),
			pID3DBlobDomainShaderCode->GetBufferSize(),
			NULL,
			&gpID3D11DomainShader
		);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateDomainShader() Failed for %d\n", hr);
		fclose(gpFile);
		pID3DBlobDomainShaderCode->Release();
		pID3DBlobDomainShaderCode = NULL;
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateDomainShader() Succeeded for %d\n", hr);
		fclose(gpFile);
	}

	// 4.  set domain shader into pipeline
	gpID3D11DeviceContext->DSSetShader(gpID3D11DomainShader, NULL, 0);

	// ----------------------------------
	// PIXEL SHADER
	// 1. Write shader source code
	const char* pixelShaderSourceCode =
		"cbuffer ConstantBuffer\n" \
		"{\n" \
			"float4 lineColor;\n" \
		"};\n" \
		"float4 main(void) : SV_TARGET\n" \
		"{\n" \
			"return lineColor;\n" \
		"}\n";

	// 2. Compile the shader programmatically
	ID3DBlob *pID3DBlobPixelShaderCode = NULL;
	pID3DBlob_error = NULL;
	hr = D3DCompile(
				pixelShaderSourceCode,
				lstrlenA(pixelShaderSourceCode) + 1,
				"PS", // Pixel Shader
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"ps_5_0",
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
		fprintf(gpFile,"Pixel Shader Compilation Succeeded \n");
		fclose(gpFile);
	}

	// 3. Create the Pixel Shader
	hr = gpID3D11Device->CreatePixelShader(
			pID3DBlobPixelShaderCode->GetBufferPointer(),
			pID3DBlobPixelShaderCode->GetBufferSize(),
			NULL,
			&gpID3D11PixelShader
		);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreatePixelShader() Failed for %d\n", hr);
		pID3DBlobPixelShaderCode->Release();
		pID3DBlobPixelShaderCode = NULL;
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreatePixelShader() Succeeded for %d\n", hr);
		fclose(gpFile);
	}
	
	// 4.  set pixel shader into pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// Initialize Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[1];
	ZeroMemory((void*)inputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(inputElementDesc));
	inputElementDesc[0].SemanticName = "POSITION";
	inputElementDesc[0].SemanticIndex = 0;
	inputElementDesc[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDesc[0].InputSlot = 0;
	inputElementDesc[0].AlignedByteOffset = 0;
	inputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDesc[0].InstanceDataStepRate = 0;

	hr = gpID3D11Device->CreateInputLayout(
			inputElementDesc,
			_ARRAYSIZE(inputElementDesc),
			pID3DBlobVertexShaderCode->GetBufferPointer(),
			pID3DBlobVertexShaderCode->GetBufferSize(),
			&gpID3D11InputLayout
		);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateInputLayout() Failed %d\n", hr);
		fclose(gpFile);
		// Release blobs
		pID3DBlobVertexShaderCode->Release();
		pID3DBlobVertexShaderCode = NULL;
		pID3DBlobPixelShaderCode->Release();
		pID3DBlobPixelShaderCode = NULL;

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateInputLayout() Succeeded %d\n", hr);
		fclose(gpFile);
	}

	// 5. Set the input layout in pipeline
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	// Release blobs
	pID3DBlobVertexShaderCode->Release();
	pID3DBlobVertexShaderCode = NULL;
	pID3DBlobPixelShaderCode->Release();
	pID3DBlobPixelShaderCode = NULL;

	// Provide vertex position, color, normal, texCoord etc.
	const float line_position[] = {
		-1.0f, -1.0f,
		-0.5f, 1.0f,
		0.5f, -1.0f,
		1.0f, 1.0f
	};

	// Position
	// Create Buffer for Vertex Data
	D3D11_BUFFER_DESC d3d11BufferDesc;
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to GL_STATIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(line_position); // line has 4 vertices and each vertex has 2 coordinates (x,y)
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	// Initialize subresource of buffer for vertex data
	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = line_position;

	// Now create the vertex buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  &d3d11SubresourceData,
									  &gpID3D11Buffer_PositionBuffer);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Position Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Position Buffer %d\n", hr);
		fclose(gpFile);
	}

	// Now create constant hull buffer
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_HULL_SHADER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create and Set above empty buffer in pipeline
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  NULL,
									  &gpID3D11Buffer_ConstantBuffer_HullShader);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Constant Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Constant Buffer %d\n", hr);
		fclose(gpFile);
	}
	
	// Set constant buffer into hull shader pipeline
	gpID3D11DeviceContext->HSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_HullShader);

	// Now create constant domain buffer
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_DOMAIN_SHADER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create and Set above empty buffer in pipeline
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  NULL,
									  &gpID3D11Buffer_ConstantBuffer_DomainShader);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Constant Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Constant Buffer %d\n", hr);
		fclose(gpFile);
	}

	// Set constant buffer into domain shader pipeline
	gpID3D11DeviceContext->DSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_DomainShader);

	// Now create constant pixel buffer
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_PIXEL_SHADER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create and Set above empty buffer in pipeline
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  NULL,
									  &gpID3D11Buffer_ConstantBuffer_PixelShader);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Constant Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Constant Buffer %d\n", hr);
		fclose(gpFile);
	}

	// Set constant buffer into pixel shader pipeline
	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_PixelShader);

	// set projection matrix to identity matrix
	perspectiveProjectionMatrix = XMMatrixIdentity();
	
	// Set clear color
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;
	clearColor[3] = 1.0f;

	hr = resize(WIN_WIDTH,WIN_HEIGHT);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
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
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
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
	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f),
														   float(width) / float(height),
														   0.1f,
														   100.0f);

	return(hr);
	
}

void display(void) {

	// code

	// clear color
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);

	// Position
	// Set Vertex Buffer created in initialize() in pipeline
	UINT stride = sizeof(float) * 2; // 2 is for (x,y)
	UINT offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);
	// Set Primitive Topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	// Transformations
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpmatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	// Push this wvpmatrix data to constant domain shader buffer
	CBUFFER_DOMAIN_SHADER constantBuffer_domainShader;
	ZeroMemory((void*)&constantBuffer_domainShader, sizeof(CBUFFER_DOMAIN_SHADER));
	constantBuffer_domainShader.worldViewProjectionMatrix = XMMatrixTranspose(wvpmatrix);

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_DomainShader,
											0,
											NULL,
											&constantBuffer_domainShader,
											0,
											0);

	// Push constant data to hull shader
	CBUFFER_HULL_SHADER constantBuffer_hullShader;
	ZeroMemory((void*)&constantBuffer_hullShader, sizeof(CBUFFER_HULL_SHADER));
	constantBuffer_hullShader.Hull_Constant_Function_Params = XMVectorSet(1.0f, (float)uiNumberofLineSegments, 0.0f, 0.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_HullShader,
												0,
												NULL,
												&constantBuffer_hullShader,
												0,
												0);

	TCHAR str[255];
	wsprintf(str, TEXT("Radhika Vishwakarma : NumberOfLineSegments : %d"), uiNumberofLineSegments);
	SetWindowText(ghwnd, str);
	
	// Push constant data to pixel shader
	CBUFFER_PIXEL_SHADER constantBuffer_pixelShader;
	ZeroMemory((void*)&constantBuffer_pixelShader, sizeof(CBUFFER_PIXEL_SHADER));
	constantBuffer_pixelShader.lineColor = gLineColor;
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PixelShader,
						 0,
						 NULL,
						 &constantBuffer_pixelShader,
						 0,
						 0);

	// Draw patch
	gpID3D11DeviceContext->Draw(4, 0);

	// Present the swapchain buffers to the swapchain
	gpIDXGISwapChain->Present(0,0);
}

void update(void)
{
	// No per-frame update required for this tessellation demo.
}

void uninitialize(void) {

	// code
	if (gpID3D11Buffer_ConstantBuffer_PixelShader)
	{
		gpID3D11Buffer_ConstantBuffer_PixelShader->Release();
		gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
	}
	if(gpID3D11Buffer_ConstantBuffer_DomainShader)
	{
		gpID3D11Buffer_ConstantBuffer_DomainShader->Release();
		gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
	}
	if(gpID3D11Buffer_ConstantBuffer_HullShader)
	{
		gpID3D11Buffer_ConstantBuffer_HullShader->Release();
		gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
	}
	if(gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	if(gpID3D11DomainShader)
	{
		gpID3D11DomainShader->Release();
		gpID3D11DomainShader = NULL;
	}
	if(gpID3D11HullShader)
	{
		gpID3D11HullShader->Release();
		gpID3D11HullShader = NULL;
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


