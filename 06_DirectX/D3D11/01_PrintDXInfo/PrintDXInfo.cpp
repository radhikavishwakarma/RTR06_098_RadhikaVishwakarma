#include <stdio.h>
#include <math.h>

// D3D11 related headers
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib") // DirectX graphics interface
#pragma comment(lib, "d3d11.lib")

int main()
{
    // function declarations
    void PrintDXInfo(void);

    // variable declarations
    IDXGIFactory *pIDXGIFactory = NULL;
    IDXGIAdapter *pIDXGIAdapter = NULL;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    HRESULT hr = S_OK;
    CHAR str[255];

    // Code
    // Get DXGIFactory
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);

    if (FAILED(hr)) // checks severity bit
    {
        printf("CreateDXGIFactory() failed!! for %d \n", hr);
        goto cleanup;
    }

    // Get Adapter from Factory
    if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
        pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);

        // to convert the WCHAR type name of graphics card in CHAR type
        WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);
        printf("Graphic Device Name: %s\n", str);
        printf("VRAM in Bytes = %I64d\n", dxgiAdapterDesc.DedicatedVideoMemory);
        printf("VRAM in GB = %d\n", (int)ceil(dxgiAdapterDesc.DedicatedVideoMemory/1024.0/1024.0/1024.0));
    }
    else
    {
        printf("IDXGIFactory::EnumAdapters() failed!!\n");
        goto cleanup;
    }
    
    

    cleanup:  //labelled jump
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

    return 0;
}