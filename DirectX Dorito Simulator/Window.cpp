#include "Window.h"
#include "Error.h"
#include "resource.h"

using namespace DirectX;

Window::Window(int width, int height, LPCWSTR title)
    : width(width), height(height), title(title), hWnd(nullptr), 
    mouse(), kbd(), gamepad(1)
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
        WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
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
        return false;
    }

    ShowWindow(hWnd, TRUE);
    UpdateWindow(hWnd);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

    if (!gfx.init(width, height, hWnd))
    {
        DisplayError(L"Graphics creation failed");
        return false;
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

Graphics& Window::getGraphics()
{
    return gfx;
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
    case WM_SIZE:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int newWidth = LOWORD(lParam);
        int newHeight = HIWORD(lParam);
        //window->Gfx().onSize(newWidth, newHeight);
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        unsigned char keycode = static_cast<unsigned char>(wParam);
        if (window->kbd.autoRepeatKeys == true)
        {
            window->kbd.OnKeyPressed(keycode);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                window->kbd.OnKeyPressed(keycode);
            }
        }
      
        break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        unsigned char keycode = static_cast<unsigned char>(wParam);
        window->kbd.OnKeyReleased(keycode);
        break;
    }
    case WM_CHAR:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        unsigned char keycode = static_cast<unsigned char>(wParam);
        if (window->kbd.autoRepeatChars == true)
        {
            window->kbd.OnChar(keycode);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                window->kbd.OnChar(keycode);
            }
        }
        break;
    }
    case WM_MOUSEMOVE:
    case WM_MOUSELEAVE:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnMouseMove(x, y);
        break;
    } 
    case WM_LBUTTONDOWN:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnLeftPressed(x, y);
        break;
    }
    case WM_LBUTTONUP:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnLeftReleased(x, y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnRightPressed(x, y);
        break;
    } 
    case WM_RBUTTONUP:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnRightReleased(x, y);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnMiddlePressed(x, y);
        break;
    }  
    case WM_MBUTTONUP:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        window->mouse.OnMiddleReleased(x, y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        float normDelta = static_cast<float>(wheelDelta) / 120.0f;
        window->mouse.OnWheelMove(x, y, normDelta);
        break;
    }
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
}
