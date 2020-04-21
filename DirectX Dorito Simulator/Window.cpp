#include "Window.h"
#include "Error.h"

Window::Window(int width, int height, LPCWSTR title)
    : width(width), height(height), title(title), hWnd(nullptr)
{
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

bool Window::init(HINSTANCE hInstance)
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

    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    hWnd = CreateWindowEx(
        NULL,
        WndClassName,
        L"DirectX Dorito Simulator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
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

    ShowWindow(hWnd, TRUE);
    UpdateWindow(hWnd);

    kbd = std::make_unique<DirectX::Keyboard>();
    if (!kbd) {
        DisplayError(L"Keyboard object creation failed");
    }
    mouse = std::make_unique<DirectX::Mouse>();
    if (!mouse) {
        DisplayError(L"Mouse object creation failed");
    }
    pGfx = std::make_unique<Graphics>(this->getWidth(), this->getHeight(), hWnd);
    if (pGfx == nullptr) {
        DisplayError(L"Graphics object was nullptr");
    }
    aud = std::make_unique<DirectX::AudioEngine>();
    if (!aud) {
        DisplayError(L"audio engine creation failed");
    }
    return true;
}

void Window::setTitle(LPCWSTR text)
{
    SetWindowText(hWnd, text);
}

Graphics& Window::Gfx()
{
    return *pGfx;
}

LRESULT Window::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_CREATE:

        break;
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
