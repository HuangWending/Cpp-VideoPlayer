#include <windows.h>
#include <vfw.h>

// 窗口类名
const char* g_WindowClassName = "VideoPlayerWindow";

// 窗口句柄
HWND g_hWnd = NULL;

// 视频句柄
HWND g_hVideoWnd = NULL;

// 媒体播放器句柄
HWND g_hMediaPlayer = NULL;

// 媒体文件名
char g_szMediaFile[MAX_PATH] = "";

// 媒体播放器回调函数
LRESULT CALLBACK MediaPlaybackWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            // 媒体播放器关闭时销毁窗口
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// 主窗口回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            // 创建媒体播放器窗口
            WNDCLASS wc = {};
            wc.lpfnWndProc = MediaPlaybackWndProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = "MediaPlaybackWindow";
            RegisterClass(&wc);
            g_hMediaPlayer = CreateWindow(wc.lpszClassName, "", WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, hWnd, NULL, GetModuleHandle(NULL), NULL);
        }
        break;

        case WM_COMMAND:
        {
            // 处理按钮点击事件
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
                case IDM_OPEN:
                {
                    // 打开文件对话框
                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = "Video Files\0*.avi;*.mp4;*.wmv;*.mkv\0All Files\0*.*\0";
                    ofn.lpstrFile = g_szMediaFile;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                    if (GetOpenFileName(&ofn))
                    {
                        // 打开视频文件
                        SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                        SendMessage(g_hVideoWnd, WM_CAP_FILE_SET_CAPTURE_FILE, 0, (LPARAM)g_szMediaFile);
                        SendMessage(g_hVideoWnd, WM_CAP_SEQUENCE, 0, 0);
                        SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                    }
                }
                break;

                case IDM_PLAY:
                {
                    // 播放视频
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_PREVIEW, 1, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                case IDM_PAUSE:
                {
                    // 暂停视频
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_PREVIEW, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                case IDM_SPEED_UP:
                {
                    // 加快播放速度
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_SCALE, 1, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                case IDM_SPEED_DOWN:
                {
                    // 减慢播放速度
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_SCALE, 2, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                case IDM_VOLUME_UP:
                {
                    // 增加音量
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_VOLUME, 100, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                case IDM_VOLUME_DOWN:
                {
                    // 减小音量
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_SET_VOLUME, 0, 0);
                    SendMessage(g_hVideoWnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
                }
                break;

                default:
                    return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        break;

        case WM_SIZE:
        {
            // 调整媒体播放器窗口大小
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            SetWindowPos(g_hMediaPlayer, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SWP_NOZORDER);
        }
        break;

        case WM_DESTROY:
            // 销毁窗口
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

// WinMain函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_WindowClassName;
    RegisterClass(&wc);

    // 创建主窗口
    g_hWnd = CreateWindow(g_WindowClassName, "Video Player", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (!g_hWnd)
    {
        return -1;
    }

    // 创建视频窗口
    g_hVideoWnd = capCreateCaptureWindow("Video", WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, g_hMediaPlayer, 0);

    // 创建功能按钮
    CreateWindow("BUTTON", "Open", WS_CHILD | WS_VISIBLE, 10, 10, 80, 30, g_hWnd, (HMENU)IDM_OPEN, hInstance, NULL);
    CreateWindow("BUTTON", "Play", WS_CHILD | WS_VISIBLE, 100, 10, 80, 30, g_hWnd, (HMENU)IDM_PLAY, hInstance, NULL);
    CreateWindow("BUTTON", "Pause", WS_CHILD | WS_VISIBLE, 190, 10, 80, 30, g_hWnd, (HMENU)IDM_PAUSE, hInstance, NULL);
    CreateWindow("BUTTON", "Speed Up", WS_CHILD | WS_VISIBLE, 280, 10, 80, 30, g_hWnd, (HMENU)IDM_SPEED_UP, hInstance, NULL);
    CreateWindow("BUTTON", "Speed Down", WS_CHILD | WS_VISIBLE, 370, 10, 80, 30, g_hWnd, (HMENU)IDM_SPEED_DOWN, hInstance, NULL);
    CreateWindow("BUTTON", "Volume Up", WS_CHILD | WS_VISIBLE, 460, 10, 80, 30, g_hWnd, (HMENU)IDM_VOLUME_UP, hInstance, NULL);
    CreateWindow("BUTTON", "Volume Down", WS_CHILD | WS_VISIBLE, 550, 10, 80, 30, g_hWnd, (HMENU)IDM_VOLUME_DOWN, hInstance, NULL);

    // 显示窗口
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
