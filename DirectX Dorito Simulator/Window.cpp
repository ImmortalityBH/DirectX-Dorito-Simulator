#include "Window.h"
#include "Error.h"
#include "resource.h"

using namespace DirectX;

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

    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WndClassName;
    wc.hIconSm = hIcon;

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
        title,
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

    Mouse::get();//construct static singleton instance of my mouse
    Keyboard::get();
    gamepad = std::make_unique<Gamepad>(1);
    if (!gamepad) {
        DisplayError(L"Gamepad creation failed");
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
    /*case WM_INPUT:
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
    case WM_MOUSEHOVER:*/
    case WM_KEYDOWN:
    {
        unsigned char keycode = static_cast<unsigned char>(wParam);
        Keyboard::get().OnKeyPressed(keycode);
        break;
    }
    case WM_KEYUP:
    {
        unsigned char keycode = static_cast<unsigned char>(wParam);
        Keyboard::get().OnKeyReleased(keycode);
        break;
    }
    case WM_CHAR:
    {
        unsigned char keycode = static_cast<unsigned char>(wParam);
        Keyboard::get().OnChar(keycode);
        break;
    }
    case WM_MOUSEMOVE:
    case WM_MOUSELEAVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnMouseMove(x, y);
        break;
    } 
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnLeftPressed(x, y);
        break;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnLeftReleased(x, y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnRightPressed(x, y);
        break;
    } 
    case WM_RBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnRightReleased(x, y);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnMiddlePressed(x, y);
        break;
    }  
    case WM_MBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        Mouse::get().OnMiddleReleased(x, y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        float normDelta = static_cast<float>(wheelDelta) / 120.0f;
        Mouse::get().OnWheelMove(x, y, normDelta);
        break;
    }
    /*case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        Keyboard::ProcessMessage(Msg, wParam, lParam);
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        break;*/
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
