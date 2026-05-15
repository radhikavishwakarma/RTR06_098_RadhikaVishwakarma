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

#pragma comment(lib, "dxgi.lib") // dxgi - DirectX Graphics Interface
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

//MACROS //centring window
#define WIN_WIDTH 800
#define  WIN_HEIGHT 600

//global functions declarations //prototype/ signature
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations -related fullscreen
BOOL gbFullScreen = FALSE; // g = global , b = boolean
HWND ghwnd = NULL;// global handle
DWORD dwStyle;
WINDOWPLACEMENT  wpPrev; // previous window..

//global variable declarations -related FILE IO
char szLogFileName[] = "Log.txt";
FILE* gpFile = NULL; // FILE datatype is declare

//activae window related variable
BOOL gbActiveWindow = FALSE;

// exist key pressed related
BOOL gbEscapeKeyPressed = FALSE;

//D3D11 related variabless Interfaces
IDXGISwapChain* gpIDXGISwapChain = NULL; //DXGI - DirectX graphics interface
ID3D11Device* gpID3D11Device = NULL;// device- graphics card..not a physical ...represent krnare logical graphic card
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL; 
float clearColor[4];

ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;


ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;
ID3D11Buffer* gpID3D11Buffer_PyramidPositionBuffer = NULL; //VBO
ID3D11Buffer* gpID3D11Buffer_PyramidNormalBuffer = NULL; //VBO

ID3D11RasterizerState* gpID3D11RastertizerState = NULL; //for disable for back face culling


ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;

struct CBUFFER
{
    XMMATRIX worldMatrix; // xnmath declaration 4*4 

    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;

    // Uniforms related to Lights
    XMVECTOR la[2];
    XMVECTOR ld[2];
    XMVECTOR ls[2];
    XMVECTOR LightPosition[2];

    XMVECTOR ka;
    XMVECTOR kd;
    XMVECTOR ks;
    float materialShininess;

    // Uniform related to Keypress
    unsigned int lKeyPress;
};

struct Light
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
};

struct Light light[2];

//Global variabe related to light
float materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float materialShininess = 50.0f;

BOOL bLight = FALSE;

XMMATRIX perspectiveProjectionMatrix;

float anglePyramid = 0.0f;

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

    // Registraton of Window Class
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
        TEXT("Radhika Vishwakarma"),
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

    // game loop
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
                if (gbEscapeKeyPressed == TRUE)
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
            gbEscapeKeyPressed = TRUE;
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
    dxgi_Swap_Chain_Desc.SampleDesc.Quality = 0;//default take value
    dxgi_Swap_Chain_Desc.OutputWindow = ghwnd;
    dxgi_Swap_Chain_Desc.Windowed = TRUE;

    // get DXGI_swapchain, d3d11 device, D3D11 Device context, supported driver and feature level
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
    // //Use in parameter and out for return..
    // step 1 = write the shader source code ... DIRECTX11- HLSL.     like vec4== float4 VS_POSITION== GL_POSITION
    const char* vertexShaderSourceCode =
        //cbuffer-struct.

        "cbuffer ConstantBuffer\n" \
        "{\n" \
            "float4x4 worldMatrix;\n" \
            "float4x4 viewMatrix;\n" \
            "float4x4 projectionMatrix;\n" \
            "float4 la[2];\n" \
            "float4 ld[2];\n" \
            "float4 ls[2];\n" \
            "float4 lightPosition[2];\n" \
            "float4 ka;\n" \
            "float4 kd;\n" \
            "float4 ks;\n" \
            "float materialShininess;\n" \
            "uint lKeyPress;\n"
        "}\n" \

        "struct vertex_Output\n" \
        "{\n" \
                 "float4 position : SV_POSITION ;\n" \
                "float3 transformedNormals : TNORMALS;\n" \
                "float3 lightDirection[2] : LIGHTDIRECTION;\n" \
                "float3 viewerVector : VIEWERVECTOR;\n" \
        "};\n" \

        "vertex_Output main(float4 pos : POSITION , float4  norm : NORMAL )\n " \
        "{\n" \
               "vertex_Output output;\n" \
               "output.position = mul(mul(projectionMatrix,viewMatrix), mul(worldMatrix,pos));\n" \
			    "if(lKeyPress == 1)\n" \
			    "{\n" \
                    "float4 eyeCoordinates = mul(viewMatrix, mul(worldMatrix, pos));\n" \
                    "float3x3 normalMatrix = (float3x3)mul(viewMatrix, worldMatrix);\n" \
                    "output.transformedNormals = mul(normalMatrix, (float3)norm);\n" \
                    "output.viewerVector = -eyeCoordinates.xyz;\n" \

                    "for(int i = 0; i < 2; i++)\n" \
                    "{\n" \
                        "output.lightDirection[i] = (float3)(lightPosition[i] - eyeCoordinates);\n" \
                    "}\n" \
				   
                "}\n" \
                "return (output);" \
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

    //Pixel  SHADER
    // step 1 = write the shader source code ... DIRECTX11- HLSL.     like vec4== float4 VS_POSITION== GL_POSITION
    const char* pixelShaderSourceCode =
        "cbuffer ConstantBuffer\n" \
        "{\n" \
            "float4x4 worldMatrix;\n" \
            "float4x4 viewMatrix;\n" \
            "float4x4 projectionMatrix;\n" \
            "float4 la[2];\n" \
            "float4 ld[2];\n" \
            "float4 ls[2];\n" \
            "float4 lightPosition[2];\n" \
            "float4 ka;\n" \
            "float4 kd;\n" \
            "float4 ks;\n" \
            "float materialShininess;\n" \
            "uint lKeyPress;\n" \
        "}\n" \

        "struct vertex_Output\n" \
        "{" \
                "float4 position : SV_POSITION ;\n" \
                "float3 transformedNormals : TNORMALS;\n" \
                "float3 lightDirection[2] : LIGHTDIRECTION;\n" \
                "float3 viewerVector : VIEWERVECTOR;\n" \
        "};\n" \

        "float4 main(vertex_Output input) : SV_TARGET\n" \
        "{" \
            "float4 ambientLight[2];\n" \
            "float4 diffuseLight[2];\n" \
            "float4 specularLight[2];\n" \
            "float3 reflectionVector[2];\n" \
            "float4 phongADSLight = float4(0.0f, 0.0f, 0.0f, 1.0f);\n" \

            "if(lKeyPress == 1)\n" \
            "{\n" \
                "float3 normalizedTransformedNormals = normalize(input.transformedNormals);\n" \
                "float3 normalizedViewerVector = normalize(input.viewerVector);\n" \

                "for(int i = 0; i < 2; i++)\n" \
                "{\n" \
                    "float3 normalizedLightDirection = normalize(input.lightDirection[i]);\n" \
                    "ambientLight[i] = la[i] * ka;\n" \
                    "diffuseLight[i] = ld[i] * kd * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0f);\n" \
                    "reflectionVector[i] = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" \
                    "specularLight[i] = ls[i] * ks * pow(max(dot(reflectionVector[i], normalizedViewerVector), 0.0f), materialShininess);\n" \

                    "phongADSLight = phongADSLight + ambientLight[i] + diffuseLight[i] + specularLight[i];\n" \
                "}\n" \
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

    ZeroMemory((void*)D3D11InputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC)* _ARRAYSIZE (D3D11InputElementDesc));
    
    //Position
    D3D11InputElementDesc[0].SemanticName = "POSITION";//starting index
    D3D11InputElementDesc[0].SemanticIndex = 0;
    D3D11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    D3D11InputElementDesc[0].InputSlot = 0;
    D3D11InputElementDesc[0].AlignedByteOffset = 0;
    D3D11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    D3D11InputElementDesc[0].InstanceDataStepRate = 0;

    //Color...
    D3D11InputElementDesc[1].SemanticName = "NORMAL";//starting index
    D3D11InputElementDesc[1].SemanticIndex = 0;
    D3D11InputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    D3D11InputElementDesc[1].InputSlot = 1;
    D3D11InputElementDesc[1].AlignedByteOffset = 0;
    D3D11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    D3D11InputElementDesc[1].InstanceDataStepRate = 0;




    //create input layout based on above structure
    hr = gpID3D11Device->CreateInputLayout(
        D3D11InputElementDesc,
        _ARRAYSIZE(D3D11InputElementDesc),
        pID3DBlob_VertexShaderCode->GetBufferPointer(),
        pID3DBlob_VertexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout
    );

    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateInputLayout failed... !!!\n");
        fclose(gpFile);

        // release
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

    //declare triangle vertex/position vertices
    const float pyramid_Position[] = {
        // front
        0.0f, 1.0f, 0.0f, // front-top
        1.0f, -1.0f, -1.0f, // front-right
        -1.0f, -1.0f, -1.0f, // front-left

        // right
        0.0f, 1.0f, 0.0f, // right-top
        1.0f, -1.0f, 1.0f, // right-right
        1.0f, -1.0f, -1.0f, // right-left

        // back
        0.0f, 1.0f, 0.0f, // back-top
        -1.0f, -1.0f, 1.0f, // back-right
        1.0f, -1.0f, 1.0f, // back-left

        // left
        0.0f, 1.0f, 0.0f, // left-top
        -1.0f, -1.0f, -1.0f, // left-right
        -1.0f, -1.0f, 1.0f, // left-left
    };

    //color vbo
    
    // Normal
    const float pyramid_Normal[] =
    {
        // front
        +0.0f, +0.447214f, -0.894427f, // front-top
        +0.0f, +0.447214f, -0.894427f, // front-right
        +0.0f, +0.447214f, -0.894427f, // front-left

        // right			    
        +0.894427f, +0.447214f, +0.0f, // right-top
        +0.894427f, +0.447214f, +0.0f, // right-right
        +0.894427f, +0.447214f, +0.0f, // right-left

        // back
        +0.0f, +0.447214f, +0.894427f, // back-top
        +0.0f, +0.447214f, +0.894427f, // back-right
        +0.0f, +0.447214f, +0.894427f, // back-left

        // left
        -0.894427f, +0.447214f, +0.0f, // left-top
        -0.894427f, +0.447214f, +0.0f, // left-right
        -0.894427f, +0.447214f, +0.0f, // left-left
    };


    //Position
    //create buffer for vertex data
    D3D11_BUFFER_DESC D3D11vertexBufferDesc ;//structure declare
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//means GL_STATIC_DRAW
    D3D11vertexBufferDesc.ByteWidth = sizeof(float)*_ARRAYSIZE(pyramid_Position);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    //Introduce or initialize sub resource of the data for vertex data 
    D3D11_SUBRESOURCE_DATA D3D11SubResourceData;
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = pyramid_Position;

    //Now create the actual buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        &D3D11SubResourceData,
        &gpID3D11Buffer_PyramidPositionBuffer
    );


    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed cor for position buffer!!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded for position buffer !!!\n");
        fclose(gpFile);

    }

    //Color
    //create buffer for vertex data
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//means GL_STATIC_DRAW
    D3D11vertexBufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(pyramid_Normal);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    //Introduce or initialize sub resource of the data for vertex data 
    ZeroMemory(&D3D11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11SubResourceData.pSysMem = pyramid_Normal;

    //Now create the actual buffer
    hr = gpID3D11Device->CreateBuffer(
        &D3D11vertexBufferDesc,
        &D3D11SubResourceData,
        &gpID3D11Buffer_PyramidNormalBuffer
    );


    if (FAILED(hr))
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer failed for color buffer ..!!!\n");
        fclose(gpFile);
        return(hr);
    }
    else
    {
        gpFile = fopen(szLogFileName, "a+");
        fprintf(gpFile, "CreateBuffer succeded for color buffer !!!\n");
        fclose(gpFile);

    }

    //now create constant buffer 
    ZeroMemory((void*)&D3D11vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11vertexBufferDesc.ByteWidth = sizeof(CBUFFER);
    D3D11vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

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

    //rasteririze state disable to back face culling so that back of the triangle will also render dusring animation
    D3D11_RASTERIZER_DESC  d3d11_rasterizer_desc;

    ZeroMemory((void*)&d3d11_rasterizer_desc,sizeof(D3D11_RASTERIZER_DESC));

    d3d11_rasterizer_desc.AntialiasedLineEnable = false;//enable when to to draw linee
    d3d11_rasterizer_desc.CullMode = D3D11_CULL_NONE;// kontach surbface cul kru  nko
    d3d11_rasterizer_desc.DepthBias = 0;//0
    d3d11_rasterizer_desc.DepthBiasClamp = 0.0F; //clamp means todne
    d3d11_rasterizer_desc.DepthClipEnable = true;//
    d3d11_rasterizer_desc.FillMode = D3D11_FILL_SOLID;//like GL_POLYGON ...
    d3d11_rasterizer_desc.FrontCounterClockwise = false;//clock wise...bcz diretc3x is clockwise so..
    d3d11_rasterizer_desc.MultisampleEnable = false;//for use heavy texture..
    d3d11_rasterizer_desc.ScissorEnable = false;//scissor test enable kraychi ahe ka ?
    d3d11_rasterizer_desc.SlopeScaledDepthBias =false ;//not slope sclae depth bias//

    //create state consider create- device and set - devicecontext...
    hr = gpID3D11Device->CreateRasterizerState(&d3d11_rasterizer_desc, &gpID3D11RastertizerState);
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

    //set above rasterizer state in pipeline
    gpID3D11DeviceContext->RSSetState(gpID3D11RastertizerState);


    // set clear color
    clearColor[0] = 0.0f;
    clearColor[1] = 0.0f;
    clearColor[2] = 0.0f;
    clearColor[3] = 1.0f;

    perspectiveProjectionMatrix = XMMatrixIdentity();

    // light1 configuration
    light[0].ambient[0] = 0.0f;
    light[0].ambient[1] = 0.0f;
    light[0].ambient[2] = 0.0f;
    light[0].ambient[3] = 1.0f;

    light[0].diffuse[0] = 1.0f;
    light[0].diffuse[1] = 0.0f;
    light[0].diffuse[2] = 0.0f;
    light[0].diffuse[3] = 1.0f;

    light[0].specular[0] = 1.0f;
    light[0].specular[1] = 0.0f;
    light[0].specular[2] = 0.0f;
    light[0].specular[3] = 1.0f;

    light[0].position[0] = -2.0f;
    light[0].position[1] = 0.0f;
    light[0].position[2] = 0.0f;
    light[0].position[3] = 1.0f;

    // light2 confuguration
    light[1].ambient[0] = 0.0f;
    light[1].ambient[1] = 0.0f;
    light[1].ambient[2] = 0.0f;
    light[1].ambient[3] = 1.0f;

    light[1].diffuse[0] = 0.0f;
    light[1].diffuse[1] = 0.0f;
    light[1].diffuse[2] = 1.0f;
    light[1].diffuse[3] = 1.0f;

    light[1].specular[0] = 0.0f;
    light[1].specular[1] = 0.0f;
    light[1].specular[2] = 1.0f;
    light[1].specular[3] = 1.0f;

    light[1].position[0] = 2.0f;
    light[1].position[1] = 0.0f;
    light[1].position[2] = 0.0f;
    light[1].position[3] = 1.0f;


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
	// Get the DXGIFactory
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
    // Step 1 : render target asel tr to release kr
    // step 2 : swapchain che buffers resize kr
    // step 3 A : swapchain madhun navin buffer ghe drag and draw karnyasathi
    // step 3 B : empty texture tayar kar and te ..
    // step 3 C : ya empty texture madhe render target view tayar kr
    // step 4 : tayar zalela render target view pipeline madhe set kr
    // step 5 : viewport creatr karr
    // step 6 : tayar zalelya viewport pipeline madhe set kar

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

    //texture property of color buffer of RTV -  are already set by the SWI system our job is just to get it into texture interface  thats is what we did abobe on getBuffer and crateRendertarget view 
    //This is not athe case with depth stencil buffer it cannot be get SWI is has to created new on your own
    //amd hence its texture property has to be set by us not by WSi
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
    gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
    // step 5 : Initialize viewport structure
    D3D11_VIEWPORT d3d11_Viewport;
    ZeroMemory((void*)&d3d11_Viewport, sizeof(D3D11_VIEWPORT));

    // glviewport
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

    //set vertexbuffer here created in initialize in pipeline
    //for position
    UINT stride = sizeof(float) * 3;
    UINT offSet = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(
            0, 1,&gpID3D11Buffer_PyramidPositionBuffer , &stride, &offSet );

    //for color
    //set vertexbuffer here created in initialize in pipeline
    stride = sizeof(float) * 3;
    offSet = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(
        1, 1, &gpID3D11Buffer_PyramidNormalBuffer, &stride, &offSet);

    // set premitive topologY mean first paramter  GL_DRAW_ARRAYs
    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

     // transformation
    XMMATRIX worldMatrix = XMMatrixIdentity();
    //worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

    XMMATRIX viewMatrix = XMMatrixIdentity();

    XMMATRIX translationMatrix = XMMatrixIdentity();
    translationMatrix = XMMatrixTranslation(0.0f,0.0f,5.0f);

    XMMATRIX rotationMatrix = XMMatrixIdentity();
    rotationMatrix = XMMatrixRotationY(anglePyramid);
        
    rotationMatrix = XMMatrixRotationY(XMConvertToRadians(anglePyramid));

    worldMatrix = rotationMatrix * translationMatrix;//order is imp


    // push this wvpmatrix into vertex shader
    CBUFFER constantBuffer;
    ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));
    constantBuffer.worldMatrix = worldMatrix;
    constantBuffer.viewMatrix = viewMatrix;
    constantBuffer.projectionMatrix = perspectiveProjectionMatrix;

    if (bLight == TRUE)
    {
        constantBuffer.la[0] = XMVectorSet(light[0].ambient[0], light[0].ambient[1], light[0].ambient[2], light[0].ambient[3]);
        constantBuffer.ld[0] = XMVectorSet(light[0].diffuse[0], light[0].diffuse[1], light[0].diffuse[2], light[0].diffuse[3]);
        constantBuffer.ls[0] = XMVectorSet(light[0].specular[0], light[0].specular[1], light[0].specular[2], light[0].specular[3]);
        constantBuffer.LightPosition[0] = XMVectorSet(light[0].position[0], light[0].position[1], light[0].position[2], light[0].position[3]);

        constantBuffer.la[1] = XMVectorSet(light[1].ambient[0], light[1].ambient[1], light[1].ambient[2], light[1].ambient[3]);
        constantBuffer.ld[1] = XMVectorSet(light[1].diffuse[0], light[1].diffuse[1], light[1].diffuse[2], light[1].diffuse[3]);
        constantBuffer.ls[1] = XMVectorSet(light[1].specular[0], light[1].specular[1], light[1].specular[2], light[1].specular[3]);
        constantBuffer.LightPosition[1] = XMVectorSet(light[1].position[0], light[1].position[1], light[1].position[2], light[1].position[3]);

        constantBuffer.ka = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], materialAmbient[3]);
        constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
        constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
        constantBuffer.materialShininess = materialShininess;

        constantBuffer.lKeyPress = 1;
    }
    else
    {
        constantBuffer.lKeyPress = 0;
    }


    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

    // draw the geometry
    gpID3D11DeviceContext->Draw(12, 0); //draw for cube -(0,6) (6,6) ,(6,12) , (6,18) ,(6,24) ,(6,30)

    // Present the Swapchain buffers to WSI(Windows system infrastructure)
    gpIDXGISwapChain->Present(0, 0);//default synchronization ,by default buffer present krta

}


void update()
{
    //code

    anglePyramid = anglePyramid + 0.5f;
    if (anglePyramid >= 360.0f)
    {
        anglePyramid = anglePyramid - 360.0f;
    }

}

void uninitialize(void)
{
    if (gpID3D11Buffer_ConstantBuffer)
    {
        gpID3D11Buffer_ConstantBuffer->Release();
        gpID3D11Buffer_ConstantBuffer = NULL;
    }

    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if (gpID3D11RastertizerState)
    {
        gpID3D11RastertizerState->Release();
        gpID3D11RastertizerState = NULL;
    }

    if (gpID3D11Buffer_PyramidNormalBuffer)
    {
        gpID3D11Buffer_PyramidNormalBuffer->Release();
        gpID3D11Buffer_PyramidNormalBuffer = NULL;
    }
    if (gpID3D11Buffer_PyramidPositionBuffer)
    {
        gpID3D11Buffer_PyramidPositionBuffer->Release();
        gpID3D11Buffer_PyramidPositionBuffer = NULL;
    }

    if (gpID3D11InputLayout)
    {
        gpID3D11InputLayout->Release();
        gpID3D11InputLayout = NULL;
    }
    if (gpID3D11PixelShader)
    {
        gpID3D11PixelShader->Release();
        gpID3D11PixelShader = NULL;
    }
    if (gpID3D11VertexShader)
    {
        gpID3D11VertexShader->Release();
        gpID3D11VertexShader = NULL;
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

//OpenGL madhe enable back face culling
//DirectX11 madhe disable back face culling
//step1 - create new interface 
// initialize struct declara and initialize... 3 and create function
// set the ns avastha... 
//
