//command cl.exe /EHsc D3D.cpp
//DirectX COM based...  com is native.. cpp

#include<windows.h>
#include<stdio.h>//purpose for giving output(file IO)..
#include <stdio.h>
#include <math.h> // For ceil-function.

// D3D11 Related Header file
#include <d3d11.h>
#include <d3dcompiler.h>

//for D3D11 Math
#pragma warning(disable:4838)//supress the warnings
#include "XNAMath/xnamath.h"

//header files
#include "D3D.h"
#include "Sphere.h"

#pragma comment(lib, "dxgi.lib") // dxgi - DirectX Graphics Interface
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"Sphere.lib")

//MACROS //centring window
#define WIN_WIDTH 800
#define  WIN_HEIGHT 600

//global functions declarations //prototype/ signature
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations -related fullscreen
BOOL gbFullScreen = FALSE;  = global , b = boolean
HWND ghwnd = NULL;lobal handle
DWORD dwStyle;
WINDOWPLACEMENT  wpPrev; // previous window..

//global variable declarations -related FILE IO
char szLogFileName[] = "Log.txt";
FILE* gpFile = NULL; // FILE datatype is declare

//activae window related variable
BOOL gbActiveWindow = FALSE;

// exist key pressed related
BOOL gbEscapekeyPress = FALSE;

//D3D11 related variabless
IDXGISwapChain* gpIDXGISwapChain = NULL; //DXGI - DirectX graphics interface
ID3D11Device* gpID3D11Device = NULL;// device- graphics card..not a physical ...represent krnare logical graphic card
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL; //
ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;

float clearColor[4];

ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;

ID3D11Buffer* gpID3D11Buffer_PositionBuffer = NULL; //VBO
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

ID3D11Buffer* gpID3D11Buffer_IndexBuffer = NULL;
ID3D11Buffer* gpID3D11Buffer_NormalBuffer = NULL;

ID3D11InputLayout* gpID3D11InputLayout = NULL;

ID3D11RasterizerState* gpID3D11RasterizerState = NULL;

//global variale related to sphere
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumElements;
unsigned int gNumVertices;

struct CBUFFER
{
    XMMATRIX worldMatrix; // xnmath declaration 4*4 
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;

    //lights related variables
    XMVECTOR la;
    XMVECTOR ld;
    XMVECTOR ls;
    XMVECTOR lightPosition;

    XMVECTOR ka;
    XMVECTOR kd;
    XMVECTOR ks;
    float materialShininess;

    //Keypress related variable
    unsigned int lKeyPress;
};

BOOL bLight = FALSE;
BOOL bAnimation = FALSE;

float lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

float materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
float materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float materialShininess = 128.0f;

XMMATRIX perspectiveProjectionMatrix;

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
int keyPress = 0;

int widthX = 0, heightY = 0;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //Function Declarations
    HRESULT initialize(void);
    void display(void);
    void update(void);
    void uninitialize(void);

    // Variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("RTR6");
    BOOL bDone = FALSE;
    HRESULT hr = S_OK;

    // variables related window centering
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    int x = width / 2 - WIN_WIDTH / 2;
    int y = height / 2 - WIN_HEIGHT / 2;
    
    // code

    // Create LogFile
    gpFile = fopen(szLogFileName, "w"); // w - overwrites file ,writes in file
    if (gpFile == NULL)
    {
        MessageBox(NULL,//NULL means desktop is parent OR hwnd_desktop - parent desktop window  
            TEXT("LogFile Creation Failed"),
            TEXT("File I/O Error"),
            MB_OK);
        exit(0);
    }
    else//swift gives error if else{<-curly braces->} not used
    {
        fprintf(gpFile, "Program Started Successfully...\n");
    }

    // window class initialization
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

    egistraton of Window Class
    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL,
            TEXT("Window Class Registeration Failed!!!"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR);
        return (0);
    }


    // create window
    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        szAppName,
        TEXT("Ganesh Laxman Gaikwad"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        x,
        y,
        WIN_WIDTH,
        WIN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);
    ghwnd = hwnd;// initilize local handle passed ...

    // Show window
    ShowWindow(hwnd, iCmdShow);

    // Paint the background of window
    UpdateWindow(hwnd);

    // initialize
    hr = initialize();
    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+"); //append..
        fprintf(gpFile, "Inilised function failed..!!!\n");
        fclose(gpFile);
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "Initialize Function Completed Successfully ...\n");
        fclose(gpFile);
    }

    // set this as foreground and active window
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    ame loop
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
                if (gbEscapekeyPress == TRUE)
                {
                    bDone = TRUE;
                }
                ender
                display();

                // update
                update();
            }
        }
    }

    uninitialize();
    return ((int)(msg.wParam));
}

//call-back function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declarations 
    void togglefullscreen(void);
    HRESULT resize(int, int);
    void uninitialize(void);

    // variable declaration
    HRESULT hr = S_OK;

    // code 
    switch (iMsg)
    {
    case WM_CREATE:
        ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT));// Once call only when window create.. 
        wpPrev.length = sizeof(WINDOWPLACEMENT);
        break;

    case WM_SETFOCUS:
        gbActiveWindow = TRUE;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = FALSE;
        break;

    case WM_ERASEBKGND:// 
        return (0);

    case WM_SIZE:
        if (gpID3D11DeviceContext)
        {
            hr = resize(LOWORD(lParam), HIWORD(lParam));
            if (FAILED(hr))
            {
                gpFile = fopen(szLogFileName, "a+");
                fprintf(gpFile, "Resize failed.. !!!\n");
                fclose(gpFile);
                return(hr);
            }
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE: //virtual keyword
            gbEscapekeyPress = TRUE;
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
            if (gbFullScreen == FALSE)
            {
                togglefullscreen();
                gbFullScreen = TRUE;

            }
            else
            {
                togglefullscreen();
                gbFullScreen = FALSE;
            }

            break;

        case 'L':
        case 'l':
            if (bLight == FALSE)
            {
                bLight = TRUE;
            }
            else
            {
                bLight = FALSE;
            }
            break;

        case 'X':
        case 'x':
            keyPress = 1;
            angleX = 0.0f;	
            break;

        case 'Y':
        case 'y':
            keyPress = 2;
            angleY = 0.0f;	
            break;

        case 'Z':
        case 'z':
            keyPress = 3;
            angleZ = 0.0f;
            break;

        default:
            break;
        }
        break;

    case  WM_CLOSE:
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

void togglefullscreen(void)
{
    //variable declarations
    MONITORINFO mi;

    //code 

    if (gbFullScreen == FALSE)
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);//Get the style ...  handle of windonw, style 
        if (dwStyle & WS_OVERLAPPEDWINDOW)
        {
            ZeroMemory((void*)&mi, sizeof(MONITORINFO));// structure-  initilize structure member..

            mi.cbSize = sizeof(MONITORINFO);

            if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
                    SWP_NOZORDER | SWP_FRAMECHANGED);//set
            }

        }
        ShowCursor(FALSE);
    }
    else
    {
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPos(HWND_TOP, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
    }
}

HRESULT initialize(void)
{
    void  PrintDXInfo();
    HRESULT resize(int, int);

    // variable declarations
    HRESULT hr = S_OK;
    D3D_DRIVER_TYPE D3D11DriverType;
    D3D_DRIVER_TYPE D3D11DriverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, //hardware accelerated driver de..
                                           D3D_DRIVER_TYPE_WARP,//windows adanced rasterization platform
                                           D3D_DRIVER_TYPE_SOFTWARE,//Safemode driver..
                                           D3D_DRIVER_TYPE_REFERENCE //for debugging..
    };

    D3D_FEATURE_LEVEL D3DFeatureLevelRequired = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL D3DFeatureLevelAquired = D3D_FEATURE_LEVEL_10_0;//minimum 10.0 check kar miltoy ka nhi 
    UINT numDriverTypes = sizeof(D3D11DriverTypes) / sizeof(D3D11DriverTypes[0]);

    // code
    // swapchain descriptor initialization
    DXGI_SWAP_CHAIN_DESC dxgi_Swap_Chain_Desc; // desc = descriptor
    ZeroMemory((void*)&dxgi_Swap_Chain_Desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    dxgi_Swap_Chain_Desc.BufferDesc.Width = WIN_WIDTH; 
    dxgi_Swap_Chain_Desc.BufferDesc.Height = WIN_HEIGHT;
    dxgi_Swap_Chain_Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unorm - unsigned normalize
    dxgi_Swap_Chain_Desc.BufferDesc.RefreshRate.Numerator = 16;
    dxgi_Swap_Chain_Desc.BufferDesc.RefreshRate.Denominator = 1;
    dxgi_Swap_Chain_Desc.BufferCount = 1;
    dxgi_Swap_Chain_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgi_Swap_Chain_Desc.SampleDesc.Count = 1;
    dxgi_Swap_Chain_Desc.SampleDesc.Quality = 0;
    dxgi_Swap_Chain_Desc.OutputWindow = ghwnd;
    dxgi_Swap_Chain_Desc.Windowed = TRUE;

    //Get dxgiSwapChain d3d11device d3d11devicecontext supported driver and supported feature level at once
    D3D_DRIVER_TYPE D3D11DRIVERTYPE;
    D3D_DRIVER_TYPE D3D11DRIVERTYPES[] = {
                                            D3D_DRIVER_TYPE_HARDWARE,
                                            D3D_DRIVER_TYPE_WARP,
                                            D3D_DRIVER_TYPE_SOFTWARE,
                                            D3D_DRIVER_TYPE_REFERENCE
    };

    //WARP - Windows Advanced Rasterization Platform 
    D3D_FEATURE_LEVEL D3D11FEATURELEVEL_REQUIRED = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL D3D11FEATURELEVEL_ACQUIRED = D3D_FEATURE_LEVEL_10_0;
    UINT NumDriverTypes = sizeof(D3D11DRIVERTYPES) / sizeof(D3D11DRIVERTYPES[0]);


    et DXGI_swapchain, d3d11 device, D3D11 Device context, supported driver and feature level
    for (UINT i = 0; i < numDriverTypes; i++)
    {
        D3D11DriverType = D3D11DriverTypes[i];
        hr = D3D11CreateDeviceAndSwapChain(NULL,//if you want other graphic card..then give adapter //default graphic card
            D3D11DriverType, NULL, 0, &D3DFeatureLevelRequired,
            1,//count of feature level
            D3D11_SDK_VERSION, &dxgi_Swap_Chain_Desc,
            &gpIDXGISwapChain, &gpID3D11Device, &D3DFeatureLevelAquired, &gpID3D11DeviceContext// fill krun gheyache //jatana empty yetana fill krun yenar
        
        );
        if(SUCCEEDED(hr))
        {
            break;
        }

    }

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "D3D11 create device and swapchain failed... !!!\n");
        fclose(gpFile);
        return(hr);
    }

    gpFile = fopen(szLogFileName, "a+");

    // Check which driver it uses
    if (D3D11DriverType == D3D_DRIVER_TYPE_HARDWARE)
    {
        fprintf(gpFile, "Chosen driver is hardware driver..\n");
    }
    else if (D3D11DriverType == D3D_DRIVER_TYPE_WARP)
    {
        fprintf(gpFile, "Chosen driver is warp...\n");
    }
    else if (D3D11DriverType == D3D_DRIVER_TYPE_SOFTWARE)
    {
        fprintf(gpFile, "Chosen driver is software driver...\n");
    }
    else if (D3D11DriverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        fprintf(gpFile, "Chosen driver is reference driver...\n");
    }
    else
    {
        fprintf(gpFile, "Chosen driver is unknown\n");
    }

    // Check which feature level it aquired
    if (D3DFeatureLevelAquired == D3D_FEATURE_LEVEL_11_0)
    {
        fprintf(gpFile, "Feature level 11.0 is aquired\n");
    }
    else if (D3DFeatureLevelAquired == D3D10_FEATURE_LEVEL_10_1)
    {
        fprintf(gpFile, "Feature level 10.1 is aquired\n");
    }
    else if (D3DFeatureLevelAquired == D3D_FEATURE_LEVEL_10_0)
    {
        fprintf(gpFile, "Feature level 10.0 is aquired\n");
    }
    else
    {
        fprintf(gpFile, "Very old feature level aquired\n");
    }
    fclose(gpFile);

    PrintDXInfo();

    //VERTEX SHADER
    // step 1 = write the shader source code ... DIRECTX11- HLSL.     like vec4== float4 VS_POSITION== GL_POSITION
    const char* vertexShaderSourceCode =
        "cbuffer ConstantBuffer\n" \
        "{\n" \
            "float4x4 worldMatrix;\n" \
            "float4x4 viewMatrix;\n" \
            "float4x4 projectionMatrix;\n" \
            "float4 la;\n" \
            "float4 ld;\n" \
            "float4 ls;\n" \
            "float4 lightPosition;\n" \
            "float4 ka;\n" \
            "float4 kd;\n" \
            "float4 ks;\n" \
            "float materialShininess;\n" \
            "uint lKeyPress;\n" \
        "}\n" \
        "struct Vertex_Output\n" \
        "{\n" \
            "float4 position : SV_POSITION;\n" \
            "float3 transformedNormals : TNORMALS;\n" \
            "float3 lightDirection : LIGHTDIRECTION;\n" \
            "float3 viewerVector : VIEWERVECTOR;\n" \
        "};\n" \
        "Vertex_Output main(float4 pos : POSITION, float4 norm : NORMAL)\n" \
        "{\n" \
            "Vertex_Output vertex_output;\n" \
            "vertex_output.position = mul(mul(projectionMatrix,viewMatrix), mul(worldMatrix,pos));\n" \
            "if(lKeyPress == 1)\n" \
            "{\n" \
                "float4 eyeCoordinates = mul(viewMatrix, mul(worldMatrix, pos));\n" \
                "float3x3 normalMatrix = (float3x3)mul(viewMatrix, worldMatrix);\n" \
                "vertex_output.transformedNormals = (mul(normalMatrix, (float3)norm));\n" \
                "vertex_output.lightDirection = (float3)(lightPosition - eyeCoordinates);\n" \
                "vertex_output.viewerVector = -eyeCoordinates.xyz;\n" \
            "}\n" \
            "return(vertex_output);\n" \
        "}\n";

    ID3DBlob* pID3DBlob_VertexShaderCode = NULL;
    ID3DBlob* pID3DBlob_Error = NULL;

    //step 2 : compilation above shader
    hr = D3DCompile(
            vertexShaderSourceCode,
            lstrlenA(vertexShaderSourceCode) + 1, //lstrlenA - lengthofstring A- ansi//ascii last character - NULL so give 1
            "VS",//shader type
            NULL,//macro use krnar ahe ka ?
            D3D_COMPILE_STANDARD_FILE_INCLUDE ,
            "main",//entry point function
            "vs_5_0",//version is call model    
            0,//no validation,optimization,debugging,
            0,
            &pID3DBlob_VertexShaderCode,//where to store compile src code.. blob
            &pID3DBlob_Error//Error string

        );
    
    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        if (pID3DBlob_Error != NULL)
        {
            fprintf(gpFile, "D3D Compiled for Vertex shader failed with error %s...!!!\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile);
            pID3DBlob_Error->Release();//data free
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, " D3D Compiled for Vertex shader Succeeded... !!!\n");
        fclose(gpFile);
    }

    //pipeline pluging
    //step3 : create vertex shader object by using above gpu compile code  create- device set- context mostly call...
    hr = gpID3D11Device->CreateVertexShader(
            pID3DBlob_VertexShaderCode->GetBufferPointer(),//run on GPU
            pID3DBlob_VertexShaderCode->GetBufferSize(),
            NULL,
            &gpID3D11VertexShader
    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateVertexShader failed... !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateVertexShader succeded... !!!\n");
        fclose(gpFile);

    }

    //step4 : Set this vertex shader in Pipeline
    gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0); 

    //Pixel Fragment SHADER
    // step 1 = write the shader source code ... DIRECTX11- HLSL.     like vec4== float4 VS_POSITION== GL_POSITION
    const char* pixelShaderSourceCode =
        "cbuffer ConstantBuffer\n" \
        "{\n" \
            "float4x4 worldMatrix;\n" \
            "float4x4 viewMatrix;\n" \
            "float4x4 projectionMatrix;\n" \
            "float4 la;\n" \
            "float4 ld;\n" \
            "float4 ls;\n" \
            "float4 lightPosition;\n" \
            "float4 ka;\n" \
            "float4 kd;\n" \
            "float4 ks;\n" \
            "float materialShininess;\n" \
            "uint lkeyPress;\n" \
        "}\n" \
        "struct Vertex_Output\n" \
        "{\n" \
            "float4 position : SV_POSITION;\n" \
            "float3 transformedNormals : TNORMALS;\n" \
            "float3 lightDirection : LIGHTDIRECTION;\n" \
            "float3 viewerVector : VIEWERVECTOR;\n" \
        "};\n" \
        "float4 main(Vertex_Output input) : SV_TARGET\n" \
        "{\n" \
        "float4 phongADSLight;\n" \
        "if(lkeyPress == 1)\n" \
        "{\n" \
            "float3 normalizedTransformedNormals = normalize(input.transformedNormals);\n" \
            "float3 normalizedLightDirection = normalize(input.lightDirection);\n" \
            "float3 normalizedViewerVector = normalize(input.viewerVector);\n" \
            "float4 ambientLight = la * ka;\n" \
            "float4 diffuseLight = ld * kd * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0f);\n" \
            "float3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" \
            "float4 specularLight = ls * ks * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0f), materialShininess);\n" \
            "phongADSLight = ambientLight + diffuseLight + specularLight;\n" \
        "}\n" \
        "else\n" \
        "{\n" \
            "phongADSLight = float4(1.0f, 1.0f, 1.0f, 1.0f);\n" \
        "}\n" \
        "float4 color = phongADSLight;\n" \
        "return(color);\n" \
        "}\n";

    ID3DBlob* pID3DBlob_PixelShaderCode = NULL;
    pID3DBlob_Error = NULL;

    //step 2 : compilation above pixel shader
    hr = D3DCompile(
        pixelShaderSourceCode,
        lstrlenA(pixelShaderSourceCode) + 1, //lstrlenA - lengthofstring A- ansi//ascii last character - NULL so give 1
        "PS",//shader type
        NULL,//macro use krnar ahe ka ?
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",//entry point function
        "ps_5_0",//version is call model    
        0,//no validation,optimization,debugging,
        0,
        &pID3DBlob_PixelShaderCode,//where to store compile src code.. blob
        &pID3DBlob_Error//Error string

    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        if (pID3DBlob_Error != NULL)
        {
            fprintf(gpFile, "D3D Compiled for Pixel shader failed with error %s...!!!\n", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile);
            pID3DBlob_Error->Release();//data free
            pID3DBlob_Error = NULL;
            return(hr);
        }
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, " D3D Compiled for Pixel shader Succeeded... !!!\n");
        fclose(gpFile);
    }

    //pipeline pluging
    //step3 : create vertex shader object by using above gpu compile code  create- device set- context mostly call...
    hr = gpID3D11Device->CreatePixelShader(
        pID3DBlob_PixelShaderCode->GetBufferPointer(),//run on GPU
        pID3DBlob_PixelShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11PixelShader
    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreatePixelShader failed... !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreatePixelShader succeded... !!!\n");
        fclose(gpFile);

    }

    //step4 : Set this pixel shader in Pipeline
    gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);


    //Initialize Input Layout
    D3D11_INPUT_ELEMENT_DESC D3D11InputElementDesc[2];//structure
    ZeroMemory((void*)D3D11InputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
    
    //Position
    D3D11InputElementDesc[0].SemanticName = "POSITION";//starting index
    D3D11InputElementDesc[0].SemanticIndex = 0;
    D3D11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    D3D11InputElementDesc[0].InputSlot = 0;
    D3D11InputElementDesc[0].AlignedByteOffset = 0;
    D3D11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    D3D11InputElementDesc[0].InstanceDataStepRate = 0;

    //Color
     //Position
    D3D11InputElementDesc[1].SemanticName = "NORMAL";//starting index
    D3D11InputElementDesc[1].SemanticIndex = 0;
    D3D11InputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    D3D11InputElementDesc[1].InputSlot = 1;
    D3D11InputElementDesc[1].AlignedByteOffset = 0;
    D3D11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    D3D11InputElementDesc[1].InstanceDataStepRate = 0;

    //create input layout based on above structure
    hr = gpID3D11Device->CreateInputLayout(
        D3D11InputElementDesc, _ARRAYSIZE(D3D11InputElementDesc),
        pID3DBlob_VertexShaderCode->GetBufferPointer(),
        pID3DBlob_VertexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout
    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateInputLayout failed... !!!\n");
        fclose(gpFile);

        elease
        if (pID3DBlob_VertexShaderCode)
        {
            pID3DBlob_VertexShaderCode->Release();
            pID3DBlob_VertexShaderCode = NULL;
        }

        if (pID3DBlob_PixelShaderCode)
        {
            pID3DBlob_PixelShaderCode->Release();
            pID3DBlob_PixelShaderCode = NULL;
        }
      
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateInputLayout succeded... !!!\n");
        fclose(gpFile);

    }

    //set this created input layout
    gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

    //release
    if (pID3DBlob_VertexShaderCode)
    {
        pID3DBlob_VertexShaderCode->Release();
        pID3DBlob_VertexShaderCode = NULL;
    }

    if (pID3DBlob_PixelShaderCode)
    {
        pID3DBlob_PixelShaderCode->Release();
        pID3DBlob_PixelShaderCode = NULL;
    }

    //---------------Sphere-------------------------//
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    //create buffer for vertex data
    //Position
    D3D11_BUFFER_DESC D3D11vertexBufferDesc ;//structure declare
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//means GL_STATIC_DRAW
    D3D11vertexBufferDesc.ByteWidth = gNumVertices*3*sizeof(float);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    //Introduce or initialize sub resource of the data for vertex data 
    D3D11_SUBRESOURCE_DATA D3D11SubResourceData;
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = sphere_vertices;

    //Now create the actual buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        &D3D11SubResourceData,
        &gpID3D11Buffer_PositionBuffer
    );


    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded !!!\n");
        fclose(gpFile);

    }

    //Normal
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11vertexBufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    //Initialize sub resource of the buffer for Vertex data
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = sphere_normals;

    //create constant buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        &D3D11SubResourceData,
        &gpID3D11Buffer_NormalBuffer

    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded !!!\n");
        fclose(gpFile);

    }

    //Index
    //now create index buffer 
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11vertexBufferDesc.ByteWidth = gNumElements * sizeof(unsigned short);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    //Initialize sub resource of the buffer for Vertex data
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = sphere_elements;

    //create inedx buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        &D3D11SubResourceData,
        &gpID3D11Buffer_IndexBuffer

    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded !!!\n");
        fclose(gpFile);

    }

    //now create constant buffer 
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11vertexBufferDesc.ByteWidth = sizeof(CBUFFER);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    //Initialize sub resource of the buffer for Vertex data
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = sphere_elements;

    //create constant buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        NULL,
        &gpID3D11Buffer_ConstantBuffer
    
    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded !!!\n");
        fclose(gpFile);

    }

    ////set this empty constant buffer in pipeline- data will given in display
    gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

    gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

    //Set rasterizer state to disable backface culling so that back of
    D3D11_RASTERIZER_DESC d3d11RasterizerDesc;

    ZeroMemory((void*)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
    d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
    d3d11RasterizerDesc.DepthBias = 0;
    d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
    d3d11RasterizerDesc.DepthClipEnable = TRUE;
    d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
    d3d11RasterizerDesc.FrontCounterClockwise = FALSE;// Direct3D is Clockwise by default
    d3d11RasterizerDesc.MultisampleEnable = FALSE;
    d3d11RasterizerDesc.ScissorEnable = FALSE;
    d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;

    hr = gpID3D11Device->CreateRasterizerState(
        &d3d11RasterizerDesc,
        &gpID3D11RasterizerState

    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateRasterizerState failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateRasterizerState succeded !!!\n");
        fclose(gpFile);

    }



    //set the above rasterizer state in pipeline
    gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

    // set clear color
    clearColor[0] = 0.0f;
    clearColor[1] = 0.0f;
    clearColor[2] = 0.0f;
    clearColor[3] = 1.0f;

    perspectiveProjectionMatrix = XMMatrixIdentity();


    //warm up resize call
    hr = resize(WIN_WIDTH, WIN_HEIGHT);
    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "resize failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "resize succeded !!!\n");
        fclose(gpFile);
        return(hr);
    }



}

void PrintDXInfo(void)
{
	// variable declarations
	IDXGIFactory *pIDXGIFactory = NULL;
	IDXGIAdapter *pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	HRESULT hr = S_OK;                                                                                                                                        
	char str[255];

	// code
	et the DXGIFactory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
	if (FAILED(hr))
	{
		printf("CreateDXGIFactory failed with error: %d\n", hr);
		goto cleanup;
	}

	// From factory get Adapter
	if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
		pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);

		// convert WCHAR type name of graphics card in to chartype
		WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);
		
		printf("Graphics Device Name = %s\n", str);
		printf("VRAM in Bytes = %I64d \n", (__int64)dxgiAdapterDesc.DedicatedVideoMemory);
		printf("VRAM in GB = %d\n", (int)ceil(dxgiAdapterDesc.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0));
	
	}
	else
	{
		printf("IDXGIFactory::EnumAdapter failed!!!  \n");
		goto cleanup;//labelled Jump
	}

	cleanup:
	if (pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;
	}
	if (pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}
}


HRESULT resize(int width, int height)
{

    HRESULT hr = S_OK;

    //release depth stencil view 
    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

    // step 1 : Release the render target view if already present (not null)
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    // step 2 : Resize swapchain buffers according to new size
    gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);//ressize ha call color buffer na ahe depth buffer la nhi

    // step 3 A : Get the buffer from swapchain for render Target View
    ID3D11Texture2D* pID3D11Texture2D_backBuffer = NULL;
    gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pID3D11Texture2D_backBuffer);

    // step 3 B : Create render target view using above texture swapchain buffer
    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_backBuffer/*resource*/, NULL/*based upon mimap level zero tya sathy null*/, &gpID3D11RenderTargetView/**/);
    if (FAILED(hr))
    {
        pID3D11Texture2D_backBuffer->Release();
        pID3D11Texture2D_backBuffer = NULL;
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "ID3D11 device : create render target view failed !!!\n");
        fclose(gpFile);
        return(hr);
    }
    pID3D11Texture2D_backBuffer->Release();
    pID3D11Texture2D_backBuffer = NULL;


    D3D11_TEXTURE2D_DESC  d3d11Texture2DDesc;
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

    //  create 2D texture from above structure
    ID3D11Texture2D* pID3D11Texture2D_DepthStencilBuffer = NULL;
    hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_DepthStencilBuffer);
    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "resize : ID3D11Texture  CreateTexture2D  failed.\n");

        fclose(gpFile);

        return hr;
    }

    /* pID3D11Texture2D_DepthStencilBuffer->Release();
     pID3D11Texture2D_DepthStencilBuffer = NULL;*/

     // Create depth Stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
    ZeroMemory((void*)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthStencilBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);
    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "resize CreateDepthStencilView failed.\n");

        fclose(gpFile);

        return hr;
    }

    pID3D11Texture2D_DepthStencilBuffer->Release();
    pID3D11Texture2D_DepthStencilBuffer = NULL;

    // step 4 : set this render target view  and depth stencil view in pipeline
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);//DEPTH NHI so 1 ,

    // step 5 : Initialize viewport structure
    D3D11_VIEWPORT d3d11_Viewport;
    ZeroMemory((void*)&d3d11_Viewport, sizeof(D3D11_VIEWPORT));

    lviewport
    d3d11_Viewport.TopLeftX = 0.0f;
    d3d11_Viewport.TopLeftY = 0.0f;
    d3d11_Viewport.Width = (float)width;
    d3d11_Viewport.Height = (float)height;
    d3d11_Viewport.MinDepth = 0.0f;
    d3d11_Viewport.MaxDepth = 1.0f;

    // set viewport in pipeline
    gpID3D11DeviceContext->RSSetViewports(1, &d3d11_Viewport);

    //Set projection matrix
    perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    return(hr);



}


void display()
{
    // Clear OpenGL Buffer ..
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);//target, color de..clearColor ne
    gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    //Position...
    //set vertexbuffer here created in initialize
    UINT stride = sizeof(float) * 3;

    UINT offSet = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(
            0, 1,&gpID3D11Buffer_PositionBuffer , &stride, &offSet );

    stride = sizeof(float) * 3;
    offSet = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

    // set premitive topologY mean first paramter  GL_DRAW_ARRAYs
    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //set index buffer
    gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer,DXGI_FORMAT_R16_UINT,0);16 maps with 'short

     // transformation
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMMATRIX viewMatrix = XMMatrixIdentity();

    XMMATRIX translationMatrix = XMMatrixIdentity();
    translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

    worldMatrix = translationMatrix;
    
    // push this wvpmatrix into vertex shader
    CBUFFER constantBuffer;
    ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));
    constantBuffer.worldMatrix = worldMatrix;
    constantBuffer.viewMatrix = viewMatrix;
    constantBuffer.projectionMatrix = perspectiveProjectionMatrix;

    if (bLight == TRUE)
    {
        
        constantBuffer.la = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]);
        constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
        constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
       // constantBuffer.lightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);
        constantBuffer.lKeyPress = 1;
      

        if (keyPress == 1)
            constantBuffer.lightPosition = XMVectorSet(0.0f, sinf(angleX) * 3, cosf(angleX) * 3, 1.0f);
        if (keyPress == 2)
            constantBuffer.lightPosition = XMVectorSet(sinf(angleY) * 3, 0.0f, cosf(angleY) * 3, 1.0f);
        if (keyPress == 3)
            constantBuffer.lightPosition = XMVectorSet(sinf(angleZ) * 3, cosf(angleZ) * 3, 0.0f, 1.0f);


    }
    else
    {
        constantBuffer.lKeyPress = 0;
    }

    // Draw 24 Spheres
    // Variable declarations
    float materialAmbient[96];
    float materialDiffuse[96];
    float materialSpecular[96];
    float materialShininess[24];

    // Code
    // 1st Sphere of 1st Column - EMERALD 
    mbient material
    materialAmbient[0] = 0.0215f;	
    materialAmbient[1] = 0.1745f;	
    materialAmbient[2] = 0.0215f;	
    materialAmbient[3] = 1.0f;		

    // Diffuse material
    materialDiffuse[0] = 0.07568f;	
    materialDiffuse[1] = 0.61424f;	
    materialDiffuse[2] = 0.07568f;	
    materialDiffuse[3] = 1.0f;		

    // Specular material
    materialSpecular[0] = 0.633f;		
    materialSpecular[1] = 0.727811f;	
    materialSpecular[2] = 0.633f;		
    materialSpecular[3] = 1.0f;			

    // Shininess
    materialShininess[0] = 0.6f * 128.0f;
    

    //  2nd Sphere of 1st Column 
    mbient material
    materialAmbient[4] = 0.135f;	
    materialAmbient[5] = 0.2225f;	
    materialAmbient[6] = 0.1575f;	
    materialAmbient[7] = 1.0f;		

    // Diffuse material
    materialDiffuse[4] = 0.54f;	
    materialDiffuse[5] = 0.89f;	
    materialDiffuse[6] = 0.63f;	
    materialDiffuse[7] = 1.0f;	

    // Specular material
    materialSpecular[4] = 0.316228f;	
    materialSpecular[5] = 0.316228f;	
    materialSpecular[6] = 0.316228f;	
    materialSpecular[7] = 1.0f;			

    // Shininess
    materialShininess[1] = 0.1f * 128.0f;
    

    //  3rd sphere of 1st column 
    mbient material
    materialAmbient[8] = 0.05375f;	
    materialAmbient[9] = 0.05f;		
    materialAmbient[10] = 0.06625f;	
    materialAmbient[11] = 1.0f;		

    // Diffuse material
    materialDiffuse[8] = 0.18275f;	
    materialDiffuse[9] = 0.17f;		
    materialDiffuse[10] = 0.22525f;	
    materialDiffuse[11] = 1.0f;		

    // Specular material
    materialSpecular[8] = 0.332741f;	
    materialSpecular[9] = 0.328634f;	
    materialSpecular[10] = 0.346435f;	
    materialSpecular[11] = 1.0f;		

    // Shininess
    materialShininess[2] = 0.3f * 128.0f;
    

    //  4th sphere of 1st column 
    mbient material
    materialAmbient[12] = 0.25f;	
    materialAmbient[13] = 0.20725f;	
    materialAmbient[14] = 0.20725f;	
    materialAmbient[15] = 1.0f;		

    // Diffuse material
    materialDiffuse[12] = 1.0f;		
    materialDiffuse[13] = 0.829f;	
    materialDiffuse[14] = 0.829f;	
    materialDiffuse[15] = 1.0f;		

    // Specular material
    materialSpecular[12] = 0.296648f;	
    materialSpecular[13] = 0.296648f;	
    materialSpecular[14] = 0.296648f;	
    materialSpecular[15] = 1.0f;		

    // Shininess
    materialShininess[3] = 0.088f * 128.0f;
    

    //  5th sphere on 1st column
    mbient material
    materialAmbient[16] = 0.1745f;	
    materialAmbient[17] = 0.01175f;	
    materialAmbient[18] = 0.01175f;	
    materialAmbient[19] = 1.0f;		

    // Diffuse material
    materialDiffuse[16] = 0.61424f;	
    materialDiffuse[17] = 0.04136f;	
    materialDiffuse[18] = 0.04136f;	
    materialDiffuse[19] = 1.0f;		

    // Specular material
    materialSpecular[16] = 0.727811f; 
    materialSpecular[17] = 0.626959f; 
    materialSpecular[18] = 0.626959f; 
    materialSpecular[19] = 1.0f;      

    // Shininess
    materialShininess[4] = 0.6f * 128.0f;
    

    // ***** 6th sphere on 1st colum
    mbient material
    materialAmbient[20] = 0.1f;     
    materialAmbient[21] = 0.18725f; 
    materialAmbient[22] = 0.1745f;  
    materialAmbient[23] = 1.0f;     

    // Diffuse material
    materialDiffuse[20] = 0.396f;   
    materialDiffuse[21] = 0.74151f; 
    materialDiffuse[22] = 0.69102f; 
    materialDiffuse[23] = 1.0f;     

    // Specular material
    materialSpecular[20] = 0.297254f; 
    materialSpecular[21] = 0.30829f;  
    materialSpecular[22] = 0.306678f; 
    materialSpecular[23] = 1.0f;      

    // Shininess
    materialShininess[5] = 0.1f * 128.0f;
   

    //  1st sphere on 2nd column
    mbient material
    materialAmbient[24] = 0.329412f; 
    materialAmbient[25] = 0.223529f; 
    materialAmbient[26] = 0.027451f; 
    materialAmbient[27] = 1.0f;      

    // Diffuse material
    materialDiffuse[24] = 0.780392f; 
    materialDiffuse[25] = 0.568627f; 
    materialDiffuse[26] = 0.113725f; 
    materialDiffuse[27] = 1.0f;      

    // Specular material
    materialSpecular[24] = 0.992157f; 
    materialSpecular[25] = 0.941176f; 
    materialSpecular[26] = 0.807843f; 
    materialSpecular[27] = 1.0f;      

    // Shininess
    materialShininess[6] = 0.21794872f * 128.0f;
    

    //  2nd sphere on 2nd column
    mbient material
    materialAmbient[28] = 0.2125f; 
    materialAmbient[29] = 0.1275f; 
    materialAmbient[30] = 0.054f;  
    materialAmbient[31] = 1.0f;    

    // Diffuse material
    materialDiffuse[28] = 0.714f;   
    materialDiffuse[29] = 0.4284f;  
    materialDiffuse[30] = 0.18144f; 
    materialDiffuse[31] = 1.0f;     

    // Specular material
    materialSpecular[28] = 0.393548f; 
    materialSpecular[28] = 0.271906f; 
    materialSpecular[30] = 0.166721f; 
    materialSpecular[31] = 1.0f;      

    // Shininess
    materialShininess[7] = 0.2f * 128.0f;
    

    // ***** 3rd sphere on 2nd column
    mbient material
    materialAmbient[32] = 0.25f;	
    materialAmbient[33] = 0.25f;	
    materialAmbient[34] = 0.25f;	
    materialAmbient[35] = 1.0f;		

    // Diffuse material
    materialDiffuse[32] = 0.4f;	
    materialDiffuse[33] = 0.4f;	
    materialDiffuse[34] = 0.4f;	
    materialDiffuse[35] = 1.0f;	

    // Specular material
    materialSpecular[32] = 0.774597f;	
    materialSpecular[33] = 0.774597f;	
    materialSpecular[34] = 0.774597f;	
    materialSpecular[35] = 1.0f;		

    // Shininess
    materialShininess[8] = 0.6f * 128.0f;
    

    //  4th sphere on 2nd column
    mbient material
    materialAmbient[36] = 0.19125f; 
    materialAmbient[37] = 0.0735f;  
    materialAmbient[38] = 0.0225f;  
    materialAmbient[39] = 1.0f;     

    // Diffuse material
    materialDiffuse[36] = 0.7038f;  
    materialDiffuse[37] = 0.27048f; 
    materialDiffuse[38] = 0.0828f;  
    materialDiffuse[39] = 1.0f;     

    // Specular material
    materialSpecular[36] = 0.256777f; 
    materialSpecular[37] = 0.137622f; 
    materialSpecular[38] = 0.086014f; 
    materialSpecular[39] = 1.0f;      

    // Shininess
    materialShininess[9] = 0.1f * 128.0f;
    

    //  5th sphere on 2nd column
    mbient material
    materialAmbient[40] = 0.24725f; 
    materialAmbient[41] = 0.1995f;  
    materialAmbient[42] = 0.0745f;  
    materialAmbient[43] = 1.0f;     

    // Diffuse material
    materialDiffuse[40] = 0.75164f; 
    materialDiffuse[41] = 0.60648f; 
    materialDiffuse[42] = 0.22648f; 
    materialDiffuse[43] = 1.0f;     

    // Specular material
    materialSpecular[40] = 0.628281f; 
    materialSpecular[41] = 0.555802f; 
    materialSpecular[42] = 0.366065f; 
    materialSpecular[43] = 1.0f;      

    // Shininess
    materialShininess[10] = 0.4f * 128.0f;
    

    //  6th sphere on 2nd column
    mbient material
    materialAmbient[44] = 0.19225f; 
    materialAmbient[45] = 0.19225f; 
    materialAmbient[46] = 0.19225f; 
    materialAmbient[47] = 1.0f;     

    // Diffuse material
    materialDiffuse[44] = 0.50754f; 
    materialDiffuse[45] = 0.50754f; 
    materialDiffuse[46] = 0.50754f; 
    materialDiffuse[47] = 1.0f;     

    // Specular material
    materialSpecular[44] = 0.508273f; 
    materialSpecular[45] = 0.508273f; 
    materialSpecular[46] = 0.508273f; 
    materialSpecular[47] = 1.0f;      

    // Shininess
    materialShininess[11] = 0.4f * 128.0f;
   

    //  1st sphere on 3rd column
    mbient material
    materialAmbient[48] = 0.0f;  
    materialAmbient[49] = 0.0f;  
    materialAmbient[50] = 0.0f;  
    materialAmbient[51] = 1.0f;  

    // Diffuse material
    materialDiffuse[48] = 0.01f; 
    materialDiffuse[49] = 0.01f; 
    materialDiffuse[50] = 0.01f; 
    materialDiffuse[51] = 1.0f;  

    // Specular material
    materialSpecular[48] = 0.50f; 
    materialSpecular[49] = 0.50f; 
    materialSpecular[50] = 0.50f; 
    materialSpecular[51] = 1.0f;  

    // Shininess
    materialShininess[12] = 0.25f * 128.0f;
   
    //  2nd sphere on 3rd column
    mbient material
    materialAmbient[52] = 0.0f;  
    materialAmbient[53] = 0.1f;  
    materialAmbient[54] = 0.06f; 
    materialAmbient[55] = 1.0f;  

    // Diffuse material
    materialDiffuse[52] = 0.0f;        
    materialDiffuse[53] = 0.50980392f; 
    materialDiffuse[54] = 0.50980392f; 
    materialDiffuse[55] = 1.0f;        

    // Specular material
    materialSpecular[52] = 0.50196078f; 
    materialSpecular[53] = 0.50196078f; 
    materialSpecular[54] = 0.50196078f; 
    materialSpecular[55] = 1.0f;        

    // Shininess
    materialShininess[13] = 0.25f * 128.0f;
   
    //  3rd sphere on 2nd column
    mbient material
    materialAmbient[56] = 0.0f;  
    materialAmbient[57] = 0.0f;  
    materialAmbient[58] = 0.0f;  
    materialAmbient[59] = 1.0f;  

    // Diffuse material
    materialDiffuse[56] = 0.1f;   
    materialDiffuse[57] = 0.35f;  
    materialDiffuse[58] = 0.1f;   
    materialDiffuse[59] = 1.0f;   

    // Specular material
    materialSpecular[56] = 0.45f; 
    materialSpecular[57] = 0.55f; 
    materialSpecular[58] = 0.45f; 
    materialSpecular[59] = 1.0f;  

    // Shininess
    materialShininess[14] = 0.25f * 128.0f;
    
    //  4th sphere on 3rd column
    mbient material
    materialAmbient[60] = 0.0f;  
    materialAmbient[61] = 0.0f;  
    materialAmbient[62] = 0.0f;  
    materialAmbient[63] = 1.0f;  

    // Diffuse material
    materialDiffuse[60] = 0.5f;  
    materialDiffuse[61] = 0.0f;  
    materialDiffuse[62] = 0.0f;  
    materialDiffuse[63] = 1.0f;  

    // Specular material
    materialSpecular[60] = 0.7f;  
    materialSpecular[61] = 0.6f;  
    materialSpecular[62] = 0.6f;  
    materialSpecular[63] = 1.0f;  

    // Shininess
    materialShininess[15] = 0.25f * 128.0f;
  
    //  5th sphere on 3rd column
    mbient material
    materialAmbient[64] = 0.0f;  
    materialAmbient[65] = 0.0f;  
    materialAmbient[66] = 0.0f;  
    materialAmbient[67] = 1.0f;  

    // Diffuse material
    materialDiffuse[64] = 0.55f; 
    materialDiffuse[65] = 0.55f; 
    materialDiffuse[66] = 0.55f; 
    materialDiffuse[67] = 1.0f;  

    // Specular material
    materialSpecular[64] = 0.70f; 
    materialSpecular[65] = 0.70f; 
    materialSpecular[66] = 0.70f; 
    materialSpecular[67] = 1.0f;  

    // Shininess
    materialShininess[16] = 0.25f * 128.0f;
    
    //  6th sphere on 3rd column
    mbient material
    materialAmbient[68] = 0.0f;  
    materialAmbient[69] = 0.0f;  
    materialAmbient[70] = 0.0f;  
    materialAmbient[71] = 1.0f;  

    // Diffuse material
    materialDiffuse[68] = 0.5f;  
    materialDiffuse[69] = 0.5f;  
    materialDiffuse[70] = 0.0f;  
    materialDiffuse[71] = 1.0f;  

    // Specular material
    materialSpecular[68] = 0.60f; 
    materialSpecular[69] = 0.60f; 
    materialSpecular[70] = 0.50f; 
    materialSpecular[71] = 1.0f;  

    // Shininess
    materialShininess[17] = 0.25f * 128.0f;
  
    //  1st sphere on 4th column
    mbient material
    materialAmbient[72] = 0.02f; 
    materialAmbient[73] = 0.02f; 
    materialAmbient[74] = 0.02f; 
    materialAmbient[75] = 1.0f;  

    // Diffuse material
    materialDiffuse[72] = 0.01f; 
    materialDiffuse[73] = 0.01f; 
    materialDiffuse[74] = 0.01f; 
    materialDiffuse[75] = 1.0f;  

    // Specular material
    materialSpecular[72] = 0.4f;  
    materialSpecular[73] = 0.4f;  
    materialSpecular[74] = 0.4f;  
    materialSpecular[75] = 1.0f;  

    // Shininess
    materialShininess[18] = 0.078125f * 128.0f;
  
    //  2nd sphere on 4th column
    mbient material
    materialAmbient[76] = 0.0f;  
    materialAmbient[77] = 0.05f; 
    materialAmbient[78] = 0.05f; 
    materialAmbient[79] = 1.0f;  

    // Diffuse material
    materialDiffuse[76] = 0.4f;  
    materialDiffuse[77] = 0.5f;  
    materialDiffuse[78] = 0.5f;  
    materialDiffuse[79] = 1.0f;  

    // Specular material
    materialSpecular[76] = 0.04f; 
    materialSpecular[77] = 0.7f;  
    materialSpecular[78] = 0.7f;  
    materialSpecular[79] = 1.0f;  

    // Shininess
    materialShininess[19] = 0.078125f * 128.0f;
    
    //  3rd sphere on 4th column 
    mbient material
    materialAmbient[80] = 0.0f;  
    materialAmbient[81] = 0.05f; 
    materialAmbient[82] = 0.0f;  
    materialAmbient[83] = 1.0f;  

    // Diffuse material
    materialDiffuse[80] = 0.4f;  
    materialDiffuse[81] = 0.5f;  
    materialDiffuse[82] = 0.4f;  
    materialDiffuse[83] = 1.0f;  

    // Specular material
    materialSpecular[80] = 0.04f; 
    materialSpecular[81] = 0.7f;  
    materialSpecular[82] = 0.04f; 
    materialSpecular[83] = 1.0f;  

    // Shininess
    materialShininess[20] = 0.078125f * 128.0f;
  
    //  4th sphere on 4th column
    mbient material
    materialAmbient[84] = 0.05f; 
    materialAmbient[85] = 0.0f;  
    materialAmbient[86] = 0.0f;  
    materialAmbient[87] = 1.0f;  

    // Diffuse material
    materialDiffuse[84] = 0.5f;  
    materialDiffuse[85] = 0.4f;  
    materialDiffuse[86] = 0.4f;  
    materialDiffuse[87] = 1.0f;  

    // Specular material
    materialSpecular[84] = 0.7f;  
    materialSpecular[85] = 0.04f; 
    materialSpecular[86] = 0.04f; 
    materialSpecular[87] = 1.0f;  

    // Shininess
    materialShininess[21] = 0.078125f * 128.0f;
    
    //  5th sphere on 4th column 
    mbient material
    materialAmbient[88] = 0.05f; 
    materialAmbient[89] = 0.05f; 
    materialAmbient[90] = 0.05f; 
    materialAmbient[91] = 1.0f;  

    // Diffuse material
    materialDiffuse[88] = 0.5f;  
    materialDiffuse[89] = 0.5f;  
    materialDiffuse[90] = 0.5f;  
    materialDiffuse[91] = 1.0f;  

    // Specular material
    materialSpecular[88] = 0.7f;  
    materialSpecular[89] = 0.7f;  
    materialSpecular[90] = 0.7f;  
    materialSpecular[91] = 1.0f;  

    // Shininess
    materialShininess[22] = 0.078125f * 128.0f;
    
    //  6th sphere on 4th column 
    mbient material
    materialAmbient[92] = 0.05f; 
    materialAmbient[93] = 0.05f; 
    materialAmbient[94] = 0.0f;  
    materialAmbient[95] = 1.0f;  

    // Diffuse material
    materialDiffuse[92] = 0.5f;  
    materialDiffuse[93] = 0.5f;  
    materialDiffuse[94] = 0.4f;  
    materialDiffuse[95] = 1.0f;  

    // Specular material
    materialSpecular[92] = 0.7f;  
    materialSpecular[93] = 0.7f;  
    materialSpecular[94] = 0.04f; 
    materialSpecular[95] = 1.0f;  

    // Shininess
    materialShininess[23] = 0.078125f * 128.0f;


    int width = (int)(widthX / 4);
    int height = (int)(heightY / 6);

    
    D3D11_VIEWPORT d3d11Viewport;
    ZeroMemory((void*)&d3d11Viewport, sizeof(D3D11_VIEWPORT));

    int index = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            constantBuffer.ka = XMVectorSet(materialAmbient[index * 4 + 0], materialAmbient[index * 4 + 1], materialAmbient[index * 4 + 2], materialAmbient[index * 4 + 3]);

            constantBuffer.kd = XMVectorSet(materialDiffuse[index * 4 + 0], materialDiffuse[index * 4 + 1], materialDiffuse[index * 4 + 2], materialAmbient[index * 4 + 3]);

            constantBuffer.ks = XMVectorSet(materialSpecular[index * 4 + 0], materialSpecular[index * 4 + 1], materialSpecular[index * 4 + 2], materialAmbient[index * 4 + 3]);

            constantBuffer.materialShininess = materialShininess[index];

            d3d11Viewport.TopLeftX = width * i;
            d3d11Viewport.TopLeftY = height * j;
            d3d11Viewport.Width = (float)width;
            d3d11Viewport.Height = (float)height;
            d3d11Viewport.MinDepth = 0.0f;
            d3d11Viewport.MaxDepth = 1.0f;

            gpID3D11DeviceContext->RSSetViewports(1, &d3d11Viewport);
            
            perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

            gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);
            
            // Draw the Geometry
            gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

            index++;
        }
    }

    // Present the SwapChainBuffers to the SwapChain
    gpIDXGISwapChain->Present(0, 0);
  

}


void update()
{
    angleX = angleX + 0.005f;
    if (angleX >= 360.0f)
    {
        angleX = angleX - 360.0f;
    }

    angleY = angleY + 0.005f;
    if (angleY >= 360.0f)
    {
        angleY = angleY - 360.0f;
    }

    angleZ = angleZ + 0.005f;
    if (angleZ >= 360.0f)
    {
        angleZ = angleZ - 360.0f;
    }

}

void uninitialize(void)
{
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if (gpID3D11RasterizerState)
    {
        gpID3D11RasterizerState->Release();
        gpID3D11RasterizerState = NULL;
    }

    if (gpID3D11Buffer_PositionBuffer)
    {
        gpID3D11Buffer_PositionBuffer->Release();
        gpID3D11Buffer_PositionBuffer = NULL;
    }

    if (gpID3D11Buffer_ConstantBuffer)
    {
        gpID3D11Buffer_ConstantBuffer->Release();
        gpID3D11Buffer_ConstantBuffer = NULL;
    }

    if (gpID3D11Buffer_NormalBuffer)
    {
        gpID3D11Buffer_NormalBuffer->Release();
        gpID3D11Buffer_NormalBuffer = NULL;
    }

    if (gpID3D11DeviceContext)
    {
        gpID3D11DeviceContext->Release();
        gpID3D11DeviceContext = NULL;
    }

    if (gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }

    if (gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }

    if (gpFile)
    {
        fprintf(gpFile, "Program terminated successfully..\n");
        fclose(gpFile);
        gpFile = NULL;
    }

}