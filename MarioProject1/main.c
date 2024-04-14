#include "window.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = { 0 };

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"Mario";
    wc.lpfnWndProc = windowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    hwnd = CreateWindowW(L"Mario", L"Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, 1000, 1000, NULL, NULL, NULL, NULL);

    setup();

    MSG msg = { 0 };
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return msg.wParam;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DWORD startTime = GetTickCount();

        input();
        update();
        render();

        DWORD frameTime = GetTickCount() - startTime;

        const DWORD FRAME_TIME_MS = 1000 / 60;

        printf("%d\n", frameTime);

        if (frameTime < FRAME_TIME_MS) {
            Sleep(FRAME_TIME_MS - frameTime);
        }

    }

    //clean();

    return 0;
}
