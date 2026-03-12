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

ID3D11DepthStencilView *gpID3D11DepthStencilView = NULL;

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_NormalBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;

ID3D11RasterizerState *gpID3D11RasterizerState = NULL;

BOOL bAnimation = FALSE;
BOOL bLight = FALSE;

// Lights related variable
FLOAT lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
FLOAT materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
FLOAT lightPosition[] = {0.0f, 0.0f, -2.0f, 1.0f};

struct CBUFFER
{
	XMMATRIX WorldViewMatrix;
	XMMATRIX ProjectionMatrix;

	// Light related uniforms
	XMVECTOR Ld;	// Diffuse light color
	XMVECTOR Kd;	// Diffuse material color
	XMVECTOR LightPosition;	// Light position in eye coordinates
	UINT LKeyIsPressed;	// To indicate whether 'L' key is pressed or not
};

XMMATRIX perspectiveProjectionMatrix;


float clearColor[4];

// rotation angles
float angleCube = 0.0f;

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
				if (bAnimation == TRUE)
				{
					update();
				}
				
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
		case 'A':
		case 'a':
			if(bAnimation == FALSE)
			{
				bAnimation = TRUE;
			}
			else
			{
				bAnimation = FALSE;
			}
			break;
		case 'L':
		case 'l':
			if(bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
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
			"float4x4 worldViewMatrix;\n" \
			"float4x4 projectionMatrix;\n" \
			"float4 ld;\n" \
			"float4 kd;\n" \
			"float4 lightPosition;\n" \
			"uint lKeyPressed;\n" \
		"}\n" \
		"struct vertex_output\n" \
		"{\n" \
			"float4 position : SV_POSITION;\n" \
			"float4 diffuseLight : COLOR;\n" \
		"};\n" \
		"vertex_output main(float4 pos : POSITION, float4 norm : NORMAL)\n" \
		"{\n" \
			"vertex_output output;\n" \
			"output.position = mul(projectionMatrix, mul(worldViewMatrix, pos));\n" \
			"if(lKeyPressed == 1)\n" \
			"{\n" \
				"float4 eyeCoordinates = mul(worldViewMatrix, pos);\n" \
				"float3x3 normalMatrix = (float3x3)worldViewMatrix;\n" \
				"float3 transformedNormal = normalize(mul(normalMatrix, (float3)norm));\n" \
				"float3 lightSource = (float3)normalize(lightPosition - eyeCoordinates);\n" \
				"output.diffuseLight = ld * kd * max(dot(transformedNormal, lightSource), 0.0f);\n" \
			"}\n" \
			"else\n" \
			"{\n" \
				"output.diffuseLight = float4(1.0f, 1.0f, 1.0f, 1.0f);\n" \
			"}\n" \
			"return(output);\n" \
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
		"struct vertex_output\n" \
		"{\n" \
			"float4 position : SV_POSITION;\n" \
			"float4 diffuseLight : COLOR;\n" \
		"};\n" \
		"float4 main(vertex_output input) : SV_TARGET\n" \
		"{\n" \
			"float4 color = input.diffuseLight;\n" \
			"return(color);\n" \
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
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[2];
	ZeroMemory((void*)inputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(inputElementDesc));
	inputElementDesc[0].SemanticName = "POSITION";
	inputElementDesc[0].SemanticIndex = 0;
	inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDesc[0].InputSlot = 0;
	inputElementDesc[0].AlignedByteOffset = 0;
	inputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDesc[0].InstanceDataStepRate = 0;

	// NORMAL
	inputElementDesc[1].SemanticName = "NORMAL";
	inputElementDesc[1].SemanticIndex = 0;
	inputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDesc[1].InputSlot = 1;
	inputElementDesc[1].AlignedByteOffset = 0;
	inputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDesc[1].InstanceDataStepRate = 0;
	
	hr = gpID3D11Device->CreateInputLayout(
		inputElementDesc,
		_ARRAYSIZE(inputElementDesc),
		pID3DBlobVertexShaderCode->GetBufferPointer(),
		pID3DBlobVertexShaderCode->GetBufferSize(),
		&gpID3D11InputLayout
	);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateInputLayout() Failed for %d\n", hr);
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
	// expanded to 36 vertices (6 per face) so we can draw with a single Draw call
	const float cube_position[] = {
		// front face (two triangles)
		1.0f,  1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,     1.0f, -1.0f,  1.0f,

		// right face
		1.0f,  1.0f, -1.0f,     1.0f,  1.0f,  1.0f,     1.0f, -1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,     1.0f, -1.0f,  1.0f,     1.0f, -1.0f, -1.0f,

		// back face
		1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,

		// left face
		-1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,

		// top face
		1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,

		// bottom face
		1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,
	};

	// normals for each vertex of the cube (36 vertices)
	const float cube_normal[] = {
		// front face
		0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,

		// right face
		1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,

		// back face
		0.0f,   0.0f, -1.0f,   0.0f,   0.0f, -1.0f,   0.00f,   00.f,-1.00f,
		00.f,   .00f,-1.f ,   .00f ,   .00,-1.f ,   .00 ,   .00,-1.f ,

		// left face
		-1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,

		// top face
		0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,

		// bottom face
		0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,
	};

	// Position
	// Create buffer for vertex data
	D3D11_BUFFER_DESC d3d11BufferDesc;
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to GL_STATIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(cube_position);
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	// Initialize subresource of buffer for vertex data
	D3D11_SUBRESOURCE_DATA d3d11SubResourceData;
	ZeroMemory((void*)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubResourceData.pSysMem = cube_position;
	
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

	// Normal
	// Initialize subresource of buffer for normal data
	ZeroMemory((void*)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubResourceData.pSysMem = cube_normal;

	// Create Buffer for Normal Data
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to GL_STATIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(cube_normal); // triangle has 3 vertices and each vertex has 3 coordinates (x,y,z)
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  &d3d11SubResourceData,
									  &gpID3D11Buffer_NormalBuffer);
	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Failed for Normal Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");	// if file doesn't exist creates it
		fprintf(gpFile,"ID3D11Device::CreateBuffer() Succeeded for Normal Buffer %d\n", hr);
		fclose(gpFile);
	}

	// Now create constant buffer
	ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create and Set above empty buffer in pipeline
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc,
									  NULL,
									  &gpID3D11Buffer_ConstantBuffer);
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

	// set constant buffer into vertex shader stage of pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	// Set projection matrix to identity matrix
	perspectiveProjectionMatrix = XMMatrixIdentity();

	// Set Rasterization state to diable backface culling
	D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
	ZeroMemory((void*)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
	d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;	// for 2D
	d3d11RasterizerDesc.DepthBias = 0;
	d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
	d3d11RasterizerDesc.DepthClipEnable = TRUE;
	d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3d11RasterizerDesc.FrontCounterClockwise = FALSE; // direction of triangle is clockwise
	d3d11RasterizerDesc.MultisampleEnable = FALSE;
	d3d11RasterizerDesc.ScissorEnable = FALSE;
	d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpID3D11RasterizerState);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateRasterizerState() Failed for %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"ID3D11Device::CreateRasterizerState() Succeeded.\n");
		fclose(gpFile);
	}

	// set rasterization state in pipeline
	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

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
	// Realese Depth Stencil view if already present
	if(gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}

	// Release Render Target view
	if(gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	// resize swap chain buffers according to new size
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0); // this is for color buffer

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

	// texture property of color buffer of RTV are already set by our system, our job is to just get it into texture interface.
	// That 

	D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
	ZeroMemory((void*)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3d11Texture2DDesc.Width = (UINT)width;
	d3d11Texture2DDesc.Height = (UINT)height;
	d3d11Texture2DDesc.ArraySize = 1;
	d3d11Texture2DDesc.MipLevels = 1;
	d3d11Texture2DDesc.SampleDesc.Count = 1;
	d3d11Texture2DDesc.SampleDesc.Quality = 0;
	d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2DDesc.CPUAccessFlags = 0;
	d3d11Texture2DDesc.MiscFlags = 0; 

	// Create 2D texture from above structure
	ID3D11Texture2D *pID3D11Texture2D_depthStencilBuffer = NULL;
	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_depthStencilBuffer);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"D3D11Device::CreateTexture2D Failed for Depth Stencil Buffer %d\n", hr);
		fclose(gpFile);
		return(hr);
	}

	// create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
	ZeroMemory((void*)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; // for multi sampling (MS)
	d3d11DepthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_depthStencilBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);

	if(FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");
		fprintf(gpFile,"D3D11Device::CreateDepthStencilView Failed %d\n", hr);
		fclose(gpFile);
		return(hr);
	}
    
	// Realese depth stencil buffer texture as we have already got depth stencil view from it.
	pID3D11Texture2D_depthStencilBuffer->Release();
	pID3D11Texture2D_depthStencilBuffer = NULL;

	// set this render target view in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

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
	// clear depth
	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Position
	// Set Vertex Buffer here created into initialize function
	UINT stride = sizeof(float) * 3; // 3 for x,y,z
	UINT offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	// Normal
	stride = sizeof(float) * 3; // 3 is for (x,y,z)
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// set primitive topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Transformations
	XMMATRIX translationMatrix = XMMatrixIdentity();
	translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
	XMMATRIX rotationMatrixX = XMMatrixIdentity();
	XMMATRIX rotationMatrixY = XMMatrixIdentity();
	XMMATRIX rotationMatrixZ = XMMatrixIdentity();
	XMMATRIX rotationMatrix = XMMatrixIdentity();
	rotationMatrixX = XMMatrixRotationX(XMConvertToRadians(angleCube));
	rotationMatrixY = XMMatrixRotationY(XMConvertToRadians(angleCube));	
	rotationMatrixZ = XMMatrixRotationZ(XMConvertToRadians(angleCube));
	rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = rotationMatrix * translationMatrix;
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvmatrix = worldMatrix * viewMatrix;

	// load the wvpmatrix data into constant buffer
	CBUFFER constantBuffer;
	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));
	constantBuffer.WorldViewMatrix = wvmatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if(bLight == TRUE)
	{
		constantBuffer.LKeyIsPressed = 1;
		constantBuffer.Ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
		constantBuffer.Kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);
	}
	else
	{
		constantBuffer.LKeyIsPressed = 0;
	}

	gpID3D11DeviceContext->UpdateSubresource(
		gpID3D11Buffer_ConstantBuffer,
		0,
		NULL,
		&constantBuffer,
		0,
		0
	);

	// draw cube
	gpID3D11DeviceContext->Draw(6, 0);
	gpID3D11DeviceContext->Draw(6, 6);
	gpID3D11DeviceContext->Draw(6, 12);
	gpID3D11DeviceContext->Draw(6, 18);
	gpID3D11DeviceContext->Draw(6, 24);
	gpID3D11DeviceContext->Draw(6, 30);

	// Present the swapchain buffers to the swapchain
	gpIDXGISwapChain->Present(0,0);


}

void update(void) {

	// code
	angleCube += 0.01f;
	if (angleCube >= 360.0f)
	{
		angleCube = 0.0f;
	}
}

void uninitialize(void) {

	// code
	// release COM objects
	if(gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if(gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
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
	if(gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
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


