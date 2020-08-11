#include "Window/Window.h"
#include "Utility/Error.h"
#include "../resource.h"
#include <fstream>

using namespace DirectX;

Window::Window()
    : width(800), height(600), title(L"DirectX Dorito Simulator : Deluxe Edition"), hWnd(nullptr),
    mouse(), kbd(), gamepad(1)
{
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

bool Window::init(HINSTANCE hInstance)
{
    try
    {
        bool isFullscreen, isVsync;
        if (!load_config("config.ini", isFullscreen, this->width, this->height, isVsync))
            THROW_NORMAL("Failed to load config.ini file. Please reinstall.");

        HRESULT hr = S_OK;
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

        if (!RegisterClassEx(&wc)) {
            THROW_NORMAL("Error registering window class");
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
            title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            wr.right - wr.left, wr.bottom - wr.top,
            nullptr,
            nullptr,
            hInstance,
            NULL
        );

        if (!hWnd) {
            THROW_NORMAL("Error creating window");
        }

        ShowWindow(hWnd, TRUE);
        UpdateWindow(hWnd);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

        if (!gfx.init(isFullscreen, isVsync, width, height, hWnd)) {
            THROW_NORMAL("graphics creation failed");
        }
        aud = std::make_unique<AudioEngine>();
        /*if (!audioEngine.init()) {
            THROW_NORMAL("audio engine creation failed");
        }*/
    }
    catch (HrException& e)
    {
        ErrorLogger::Log(e);
        return false;
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

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT Msg, 
    WPARAM wParam, LPARAM lParam);
LRESULT Window::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
        return true;
    switch (Msg)
    {
    case WM_CREATE:

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
    {
        int newWidth = LOWORD(lParam);
        int newHeight = HIWORD(lParam);
        
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        
        std::wstring str = L"Width: ";
        str.append(std::to_wstring(newWidth));
        str.append(L", Height:" + std::to_wstring(newHeight));
        OutputDebugStringW(str.c_str());
        //window->setTitle(str.c_str());
        //if (window->getGraphics().getDevice())
        //{
            //window->getGraphics().onSize(newWidth, newHeight);
       // }
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        unsigned char keycode = static_cast<unsigned char>(wParam);
        if (window->kbd.getAutoRepeatKeys())
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
        if (window->kbd.getAutoRepeatChars())
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

bool load_config(const char* filePath, bool& isFullscreen, UINT& width, UINT& height, bool& isVysnc)
{
    std::ifstream file;
    std::string line;

    try
    {
        file.open(filePath);
        if (!file.is_open())
            throw std::ios::failure("The config.ini file has been misplaced. Please reinstall the application.");

        int index = 1;
        while (std::getline(file, line))
        {
            switch (index)
            {
            case 2:
            {
                int boolInt = std::stoi(line);
                if (boolInt == 0)
                    isFullscreen = false;
                else
                    isFullscreen = true;
                break;
            }
            case 4:
                width = std::stoi(line);
                break;
            case 6:
                height = std::stoi(line);
                break;
            case 8:
            {
                int boolInt = std::stoi(line);
                if (boolInt == 0)
                    isVysnc = false;
                else
                    isVysnc = true;
                break;
            }
            }
            index++;
        }
        file.close();
    }
    catch (const std::exception& e)
    {
        ErrorLogger::Log(string_to_wstring(e.what()));
        return false;
    }
    return true;
}
