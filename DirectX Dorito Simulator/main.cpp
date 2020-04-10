#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Keyboard.h>
#include <memory>

IDXGISwapChain* pSwapChain = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pTarget = nullptr;

ID3D11Buffer* pVertexBuffer = nullptr;
ID3D11VertexShader* pVertexShader = nullptr;
ID3D11PixelShader* pPixelShader = nullptr;
ID3DBlob* pVertexBlob = nullptr;
ID3DBlob* pPixelBlob = nullptr;
ID3D11InputLayout* vertexLayout = nullptr;


std::unique_ptr<DirectX::Keyboard> keyboard;


LPCTSTR WndClassName = L"window";
HWND hWnd = nullptr;

const int Width = 800;
const int Height = 600;

bool InitDirectX(HINSTANCE hInstance);
void Destroy();
bool InitScene();
void UpdateScene();
void DrawScene();

bool InitWindow(HINSTANCE hInstance, int ShowWnd, 
    int width, int height, bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
    WPARAM wParam, LPARAM lParam);

struct Vertex  
{
    Vertex(float x, float y, float z, 
        float r, float g, float b)
        : x(x), y(y), z(z), 
          r(r), g(g), b(b) { }
    float x, y, z;
    float r, g, b;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

void SetWindowTitle(LPCWSTR text) {
    SetWindowText(hWnd, text);
}

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    if (!InitWindow(hInstance, nShowCmd, Width, Height, true))
    {
        MessageBox(0, L"Window Initialization - Failed",
            L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    keyboard = std::make_unique<DirectX::Keyboard>();
    if (!InitDirectX(hInstance))    //Initialize Direct3D
    {
        MessageBox(0, L"Direct3D Initialization - Failed",
            L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    if (!InitScene())    //Initialize our scene
    {
        MessageBox(0, L"Scene Initialization - Failed",
            L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    messageloop();
    Destroy();
    return 0;
}

bool InitWindow(HINSTANCE hInstance,int ShowWnd,int width, int height, bool windowed)
{

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WndClassName;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Error registering class",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    hWnd = CreateWindowEx(
        NULL,
        WndClassName,
        L"Lesson 4 - Begin Drawing",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL, L"Error creating window",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hWnd, ShowWnd);
    UpdateWindow(hWnd);

    return true;
}

bool InitDirectX(HINSTANCE hInstance)
{
    HRESULT hr;
    //Describe our Buffer
    DXGI_MODE_DESC bd = {};

    bd.Width = Width;
    bd.Height = Height;
    bd.RefreshRate.Numerator = 60;
    bd.RefreshRate.Denominator = 1;
    bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC sd = {};

    sd.BufferDesc = bd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
        NULL, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwapChain, 
        &pDevice, nullptr, &pContext);

    ID3D11Texture2D* BackBuffer; //Create our BackBuffer
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));

    //Create our Render Target
    hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
    BackBuffer->Release();

    //Set our Render Target
    pContext->OMSetRenderTargets(1, &pTarget, nullptr);

    return true;
}

void Destroy()
{
    pSwapChain->Release();
    pDevice->Release();
    pContext->Release();
    pTarget->Release();
    pVertexBuffer->Release();
    pVertexShader->Release();
    pPixelShader->Release();
    pVertexBlob->Release();
    pPixelBlob->Release();
    vertexLayout->Release();
}

bool InitScene()
{
    HRESULT hr;

    hr = D3DReadFileToBlob(L"VertexShader.cso", &pVertexBlob);
    hr = D3DReadFileToBlob(L"PixelShader.cso", &pPixelBlob);
    //Create the Shader Objects
    hr = pDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(), 
        pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
    hr = pDevice->CreatePixelShader(pPixelBlob->GetBufferPointer(),
        pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);
  
    pContext->VSSetShader(pVertexShader, nullptr, 0u);
    pContext->PSSetShader(pPixelShader, nullptr, 0u);

    Vertex vertices[] =
    {
        Vertex(0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f),
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

    //Create the Input Layout
    hr = pDevice->CreateInputLayout(layout, numElements, pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), &vertexLayout);

    //Set the Input Layout
    pContext->IASetInputLayout(vertexLayout);

    //Set Primitive Topology
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Create and set the Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;

    pContext->RSSetViewports(1u, &viewport);

    return true;
}

void UpdateScene()
{
    auto kb = keyboard->GetState();
    if (kb.A)
    {
        MessageBox(nullptr, L"Lol you pressed A button", L"LOL", MB_OK | MB_ICONWARNING);
    }
}

void DrawScene()
{
    const float color[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    pContext->ClearRenderTargetView(pTarget, color);

    pContext->Draw(3u, 0);

    //Present the backbuffer to the screen
    pSwapChain->Present(0, 0);
}

int messageloop() 
{
    MSG msg = {};

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else 
        {          
            UpdateScene();
            DrawScene();
        }
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ACTIVATEAPP:

        DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);     
        break;

    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        //Mouse::ProcessMessage(message, wParam, lParam);
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}