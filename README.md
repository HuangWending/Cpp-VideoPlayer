# Cpp-VideoPlayer
C++窗口视频播放器
#include <windows.h>
#include <vfw.h>

这两行代码引入了必要的头文件，其中`windows.h`提供了Windows API的功能，`vfw.h`提供了视频功能的相关定义和函数。

const char* g_WindowClassName = "VideoPlayerWindow";
HWND g_hWnd = NULL;
HWND g_hVideoWnd = NULL;
HWND g_hMediaPlayer = NULL;
char g_szMediaFile[MAX_PATH] = "";
这几行代码定义了全局变量，用于存储窗口类名、主窗口句柄、视频窗口句柄、媒体播放器句柄和媒体文件名。

LRESULT CALLBACK MediaPlaybackWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
这是媒体播放器窗口的回调函数`MediaPlaybackWndProc`，用于处理媒体播放器窗口的消息，当接收到`WM_CLOSE`消息时销毁窗口，否则调用默认的窗口过程函数`DefWindowProc`。

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            WNDCLASS wc = {};
            wc.lpfnWndProc = MediaPlaybackWndProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = "MediaPlaybackWindow";
            RegisterClass(&wc);
            g_hMediaPlayer = CreateWindow(wc.lpszClassName, "", WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, hWnd, NULL, GetModuleHandle(NULL), NULL);
        }
        break;
        // ...
    }
    return 0;
}

这是主窗口的回调函数`WndProc`，用于处理主窗口的消息。当接收到`WM_CREATE`消息时，创建媒体播放器窗口，注册窗口类、创建窗口，并将其作为子窗口放置在主窗口内部。

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_WindowClassName;
    RegisterClass(&wc);
    // ...
}
这是程序的入口函数`WinMain`，用于创建和显示主窗口。首先注册主窗口的窗口类，并将窗口回调函数`WndProc`指定为窗口过程。

g_hWnd = CreateWindow(g_WindowClassName, "Video Player", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
使用窗口类名创建主窗口，指定窗口标题为"Video Player"，样式为`WS_OVERLAPPEDWINDOW`，初始位置和大小为默认值。

g_h

VideoWnd = capCreateCaptureWindow("Video", WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, g_hMediaPlayer, 0);
创建视频窗口，使用Video for Windows (VFW)库中的`capCreateCaptureWindow`函数创建视频捕获窗口，指定窗口标题为"Video"，样式为`WS_CHILD | WS_VISIBLE`，初始位置和大小为(0, 0, 640, 480)，父窗口为媒体播放器窗口。

CreateWindow("BUTTON", "Open", WS_CHILD | WS_VISIBLE, 10, 10, 80, 30, g_hWnd, (HMENU)IDM_OPEN, hInstance, NULL);

创建名为"Open"的按钮，样式为`WS_CHILD | WS_VISIBLE`，初始位置和大小为(10, 10, 80, 30)，父窗口为主窗口，按钮的标识符为`IDM_OPEN`，使用`hInstance`参数作为按钮的实例句柄。

其他功能按钮的创建类似，只是位置和标识符不同。

ShowWindow(g_hWnd, nCmdShow);
UpdateWindow(g_hWnd);

显示主窗口，调用`ShowWindow`函数，并调用`UpdateWindow`函数更新窗口。

MSG msg;
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
进入消息循环，使用`GetMessage`函数获取消息，然后使用`TranslateMessage`函数将虚拟键消息转换为字符消息，最后使用`DispatchMessage`函数将消息派发给相应的窗口过程进行处理。

这就是代码的基本解释，它实现了一个简单的窗口视频播放器，并包含了打开文件、播放、暂停、调节速度和音量的功能按钮。你可以根据需要进一步完善和扩展该代码。
