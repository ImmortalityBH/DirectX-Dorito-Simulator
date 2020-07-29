/********DIRECTX DORITO SIMULATOR: DELUXE EDITION*********
//TODO: name engine and keep adding to abstractions,
skybox, assimp, xaudio2

*********************************************************/
#include "Game/Game.h"

#include <random>
#include <ctime>

int WINAPI WinMain(HINSTANCE hInstance,   
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    srand(time(nullptr));
    return Game{}.run(hInstance);
}

/*#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Keyboard.h>
#include <memory>
#include <DirectXMath.h>
#include "Timer.h"
#include <string>
#include "Error.h"
#include "DDSTextureLoader.h"
#include "Audio.h"
#include <random>
#include <ctime>
#include <vector>
using namespace DirectX;

IDXGISwapChain* pSwapChain = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pTarget = nullptr;

ID3D11Buffer* pVertexBuffer = nullptr;

//Constant Buffer
ID3D11Buffer* pConstantBuffer = nullptr;

ID3D11VertexShader* pVertexShader = nullptr;
ID3D11PixelShader* pPixelShader = nullptr;
ID3DBlob* pVertexBlob = nullptr;
ID3DBlob* pPixelBlob = nullptr;
ID3D11InputLayout* vertexLayout = nullptr;

ID3D11ShaderResourceView* pTexture;
ID3D11SamplerState* pTexSamplerState;

std::unique_ptr<DirectX::Keyboard> keyboard;
std::unique_ptr<DirectX::AudioEngine> audEngine;
std::unique_ptr<DirectX::SoundEffect> soundEffect;
std::unique_ptr<DirectX::SoundEffect> soundEffect2;

std::unique_ptr<DirectX::SoundEffect> effects[13];

LPCTSTR WndClassName = L"window";
HWND hWnd = nullptr;

int Width = 800;
int Height = 600;

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
        float u, float v)
        : pos(x, y, z), uv(u, v) {}
    XMFLOAT3 pos;
    XMFLOAT2 uv;
};

struct ConstantBuffer
{
    XMMATRIX transform;
} cb;

XMMATRIX transform;
XMMATRIX scale;
XMMATRIX rotation;
XMMATRIX translation;

D3D11_INPUT_ELEMENT_DESC layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

//GAME GLOBALS
int randomKey;
bool isDead = false, isGameStart = false, didRequest = true, doOnce = false, playOnce = false;
float gameSpeed = 1.8f;
int score = 0;
const float speed = 0.1f;
Timer timer;

void SetWindowTitle(LPCWSTR text) {
    SetWindowText(hWnd, text);
}

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    srand(time(NULL));
    if (!InitWindow(hInstance, nShowCmd, Width, Height, true))
    {
        MessageBox(0, L"Window Initialization - Failed",
            L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
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

bool InitWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
    HRESULT hr;
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
        L"DirectX Dorito Simulator",
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
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    DisplayError(hr, L"something failed");

    ShowWindow(hWnd, ShowWnd);
    UpdateWindow(hWnd);

    keyboard = std::make_unique<DirectX::Keyboard>();
    if (!keyboard) {
        DisplayError(hr, L"keyboard creation failed");
    }
    audEngine = std::make_unique<DirectX::AudioEngine>();
    if (!audEngine) {
        DisplayError(hr, L"audio engine creation failed");
    }
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

    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        creationFlags, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwapChain,
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
    pConstantBuffer->Release();
}

bool InitScene()
{
    HRESULT hr;

    hr = D3DReadFileToBlob(L"res/shader/VertexShader.cso", &pVertexBlob);
    DisplayError(hr, L"Vertex shader failed to load");
    hr = D3DReadFileToBlob(L"res/shader/PixelShader.cso", &pPixelBlob);
    DisplayError(hr, L"Pixel shader failed to load");
    //Create the Shader Objects
    hr = pDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
    DisplayError(hr, L"Vertex shader failed to create");
    hr = pDevice->CreatePixelShader(pPixelBlob->GetBufferPointer(),
        pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);
    DisplayError(hr, L"Pixel shader failed to create");
    pContext->VSSetShader(pVertexShader, nullptr, 0u);
    pContext->PSSetShader(pPixelShader, nullptr, 0u);

    Vertex vertices[] =
    {
        Vertex(0.0f, 0.5f, 0.5f, 0.5f, 1.0f),
        Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f),
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


    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.CPUAccessFlags = 0;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(ConstantBuffer);

    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);


    hr = CreateDDSTextureFromFile(pDevice, L"res/img/dorito.dds", nullptr, &pTexture);
    DisplayError(hr, L"Texture failed to load!");

    D3D11_SAMPLER_DESC tsd = {};
    tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    tsd.MinLOD = 0;
    tsd.MaxLOD = D3D11_FLOAT32_MAX;

    hr = pDevice->CreateSamplerState(&tsd, &pTexSamplerState);
    DisplayError(hr, L"Create sampler state failed.");

    //Create and set the Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    pContext->RSSetViewports(1u, &viewport);

    soundEffect = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/intro.wav");
    soundEffect2 = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/outro.wav");
    
    effects[0] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/amazing.wav");
    effects[1] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/awesome.wav");
    effects[2] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/delicious.wav");
    effects[3] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/good job.wav");
    effects[4] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/incredible.wav");
    effects[5] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/mom.wav");
    effects[6] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/nice one.wav");
    effects[7] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/oh my god.wav");
    effects[8] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/poop.wav");
    effects[9] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/right.wav");
    effects[10] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/un.wav");
    effects[11] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/fire.wav");
    effects[12] = std::make_unique<SoundEffect>(audEngine.get(), L"res/audio/wow.wav");
    
    
    soundEffect->Play();
    
    return true;
}

void UpdateScene()
{
    if (!audEngine->Update())
    {

    }
    
    auto kb = keyboard->GetState();
    
    transform = XMMatrixIdentity();
    scale = XMMatrixIdentity();
    rotation = XMMatrixIdentity();
    translation = XMMatrixIdentity();

    if (timer.Peek() > 19.0f && !isGameStart)
    {
        isGameStart = true;
    }

    if (timer.Peek() > gameSpeed && isGameStart)
    {
        if (didRequest)
        {
            const int min = 65;
            const int max = 90;
            randomKey = min + (rand() % static_cast<int>(max - min + 1));
            std::wstring text = L"Press: ";
            text.push_back((wchar_t)randomKey);
            SetWindowTitle(text.c_str());
            didRequest = false;
            score++;
            if (gameSpeed > 0.5f)
            {
                gameSpeed -= 0.05f;
            }
            /*const int min1 = 0;
            const int max1 = 3;
            int randPos = min1 + (rand() % static_cast<int>(max1 - min1 + 1));
            switch (randPos)
            {
            case 0:
                translation = XMMatrixScaling(-.5f, 0.0f, 0.0f);
                break;
            case 1:
                translation = XMMatrixScaling(.5f, 0.0f, 0.0f);
                break;
            case 2:
                translation = XMMatrixScaling(0.0f, 0.5f, 0.0f);
                break;
            case 3:
                translation = XMMatrixScaling(0.0f, -0.5f, 0.0f);
                break;
            }
        }
        else
        {
            isDead = true;
            std::wstring text = L"THE DORITO DIED!!!   Your Score: "  + std::to_wstring(score);
            SetWindowTitle(text.c_str());
            if (!doOnce)
            {
                soundEffect2->Play();
                doOnce = true;
            }
        }
        timer.Mark();
    }
    if (kb.IsKeyDown(static_cast<Keyboard::Keys>(randomKey)) && !isDead)
    {
        didRequest = true;
        SetWindowTitle(L"GOOD");
        if (!playOnce)
        {
            const int min = 0;
            const int max = 12;
            int index = min + (rand() % static_cast<int>(max - min + 1));
            effects[index]->Play();
            playOnce = true;
        }   
    }
    else
    {
        playOnce = false;
    }
    /*thing += 0.01f;

    if (kb.Q)
    {
        rotation = XMMatrixRotationZ(speed * thing);
        if (randomNum == 0)
        {
            didRequest = true;
        }
    }
    else if (kb.E)
    {
        rotation = XMMatrixRotationZ(-(speed * thing));
        if (randomNum == 1)
        {
            didRequest = true;
        }
    }
    else if (kb.W)
    {
        scale = XMMatrixScaling(speed * thing, speed * thing, 0.0f);
        if (randomNum == 2)
        {
            didRequest = true;
        }
    }
    else if (kb.S)
    {
        scale = XMMatrixScaling(-speed * thing, -speed * thing, 0.0f);
        if (randomNum == 3)
        {
            didRequest = true;
        }
    }
    else if (kb.A)
    {
        translation = XMMatrixTranslation(-speed * thing, 0.0f, 0.0f);
        if (randomNum == 4)
        {
            didRequest = true;
        }
    }
    else if (kb.D)
    {
        translation = XMMatrixTranslation(speed * thing, 0.0f, 0.0f);
        if (randomNum == 5)
        {
            didRequest = true;
        }
    }

    if (timer.Peek() > 2.5f)
    {
        randomNum = rand() % 6;
        switch (randomNum)
        {
        case 0:
        {
            std::wstring title = L"Dorito Says Rotate Left, Task Number:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        case 1:
        {
            std::wstring title = L"Dorito Says Rotate Right, Score:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        case 2:
        {
            std::wstring title = L"Dorito Says Scale Up, Score:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        case 3:
        {
            std::wstring title = L"Dorito Says Scale Down, Score:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        case 4:
        {
            std::wstring title = L"Dorito Says Move Left, Score:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        case 5:
        {
            std::wstring title = L"Dorito Says Move Right, Score:" + std::to_wstring(taskNum);
            SetWindowTitle(title.c_str());
        }
        break;
        }
        if (didRequest == false)
        {
            SetWindowTitle(L"YOU FAILED");
            taskNum = 0;
            didRequest = true;
        }
        else
        {
            didRequest = false;
            taskNum++;
        }
        timer.Mark();
    }

    transform = scale * rotation * translation;

    cb.transform = XMMatrixTranspose(transform);
}

void DrawScene()
{
    const float color[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    pContext->ClearRenderTargetView(pTarget, color);

    /*D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    mappedResource.pData = &cb;
    //pContext->UpdateSubresource(pConstantBuffer, 0u, nullptr, &cb, 0u, 0u);
    pContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    pContext->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);
    pContext->Unmap(pConstantBuffer, 0);

    pContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);

    pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    pContext->PSSetShaderResources(0, 1, &pTexture);
    pContext->PSSetSamplers(0, 1, &pTexSamplerState);

    if (!isDead) 
    {
        pContext->Draw(3u, 0);
    }
    else
    {

    }
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
    /*case WM_SIZE:
    {
        RECT rect;
        if (GetWindowRect(hWnd, &rect))
        {
            Width = rect.right - rect.left;
            Height = rect.bottom - rect.top;
        }
        D3D11_VIEWPORT viewport = {};

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = Width;
        viewport.Height = Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        if (pContext != nullptr)
        {
            pContext->RSSetViewports(1u, &viewport);
        }
        
    }
        //break;
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

/*#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Keyboard.h>
#include <memory>
#include <DirectXMath.h>

using namespace DirectX;

IDXGISwapChain* pSwapChain = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pTarget = nullptr;

ID3D11Buffer* pVertexBuffer = nullptr;

//Constant Buffer
ID3D11Buffer* pConstantBuffer = nullptr;

ID3D11VertexShader* pVertexShader = nullptr;
ID3D11PixelShader* pPixelShader = nullptr;
ID3DBlob* pVertexBlob = nullptr;
ID3DBlob* pPixelBlob = nullptr;
ID3D11InputLayout* vertexLayout = nullptr;

ID3D11RasterizerState* pState = nullptr;
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

struct ConstantBuffer 
{
    XMMATRIX WVP;
} cb;

XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

XMMATRIX WVP;
XMMATRIX world;
XMMATRIX scale;
XMMATRIX rotation;
XMMATRIX translation;

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

    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        creationFlags, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwapChain,
        &pDevice, nullptr, &pContext);

    ID3D11Texture2D* BackBuffer; //Create our BackBuffer
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));

    //Create our Render Target
    hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
    BackBuffer->Release();

    //Set our Render Target
    pContext->OMSetRenderTargets(1, &pTarget, nullptr);

    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_NONE;

    pDevice->CreateRasterizerState(&rd, &pState);

    pContext->RSSetState(pState);
    
    camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
   
    camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //Set the View matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

    //Set the Projection matrix
    //camProjection = XMMatrixOrthographicFovLH
    camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)Width / Height, 1.0f, 1000.0f);
    
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
    pConstantBuffer->Release();
    pState->Release();
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


    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.StructureByteStride = 0u;

    pDevice->CreateBuffer(&cbd, nullptr, &pConstantBuffer);

    //Create and set the Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    pContext->RSSetViewports(1u, &viewport);

    camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);

    camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //Set the View matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

    //Set the Projection matrix
    camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)Width / Height, 1.0f, 1000.0f);

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

    world = XMMatrixIdentity();

    translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

    XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    rotation = XMMatrixRotationAxis(rotaxis, 45);
    translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

    world = scale * rotation * translation;

    WVP = world * camView * camProjection;

    cb.WVP = XMMatrixTranspose(WVP);

    pContext->UpdateSubresource(pConstantBuffer, 0u, nullptr, &cb, 0u, 0u);
    pContext->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);

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
}*/