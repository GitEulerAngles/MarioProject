#include "window.h"
#include "game.h"

_Bool flipped = true;

_Bool createConsole() {
    if (AllocConsole())
        AttachConsole(GetCurrentProcessId());
    else
        return false;

    FILE* consoleStream;
    freopen_s(&consoleStream, "CONOUT$", "w", stdout);

    return true;
}
FIBITMAP* loadPNGImage(char path[]) {
    return FreeImage_Load(FIF_PNG, path, 0);
}
void updateImage(HDC* hdc, HDC* memDC, int i) {
    struct sprite* current_sprite = &sprites[i];

    if (current_sprite->resizedBitmap) {
        int newWidth = current_sprite->newDim.x;
        int newHeight = current_sprite->newDim.y;

        FIBITMAP* resizedBitmap = current_sprite->resizedBitmap;

        BYTE* bits = (BYTE*)malloc(newWidth * newHeight * 4);

        FreeImage_ConvertToRawBits(bits, resizedBitmap, newWidth * 4, 32,
            FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

        if (flipped) {
            BYTE* flippedBits = (BYTE*)malloc(newWidth * newHeight * 4);

            for (int y = 0; y < newHeight; y++) {
                for (int x = 0; x < newWidth; x++) {
                    int srcIndex = (y * newWidth + x) * 4;
                    int destIndex = (y * newWidth + (newWidth - 1 - x)) * 4;

                    flippedBits[destIndex] = bits[srcIndex];
                    flippedBits[destIndex + 1] = bits[srcIndex + 1];
                    flippedBits[destIndex + 2] = bits[srcIndex + 2];
                    flippedBits[destIndex + 3] = bits[srcIndex + 3];
                }
            }

            free(bits);
            bits = flippedBits;
        }

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = newWidth;
        bmi.bmiHeader.biHeight = -newHeight;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        HDC hdcSprite = CreateCompatibleDC(*hdc);
        if (!hdcSprite) {
            printf("Error: hdcSprite note created.\n");
            free(bits);
            return;
        }

        HBITMAP hbmDib = CreateDIBitmap(*memDC, &bmi.bmiHeader, CBM_INIT, bits, &bmi, DIB_RGB_COLORS);
        if (!hbmDib) {
            printf("Error: hbmDib not created.\n");
            free(bits);
            DeleteDC(hdcSprite);
            return;
        }

        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcSprite, hbmDib);

        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.SourceConstantAlpha = 255;
        blend.AlphaFormat = AC_SRC_ALPHA;

        AlphaBlend(*memDC, current_sprite->pos.x, current_sprite->pos.y, newWidth / 2, newHeight, hdcSprite, 
            abs(flipped * (newWidth / 2) - current_sprite->imageCoordinate.x * 10), 0, newWidth / 2, newHeight, blend);

        SelectObject(hdcSprite, hbmOld);
        DeleteObject(hbmDib);
        free(bits);
    }
}
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC memDC = CreateCompatibleDC(hdc);
        if (!memDC) {
            EndPaint(hWnd, &ps);
            return 0;
        }

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        HBITMAP bmp = CreateCompatibleBitmap(hdc,
            clientRect.right - clientRect.left,
            clientRect.bottom - clientRect.top);

        HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

        FillRect(memDC, &clientRect, (HBRUSH)(COLOR_WINDOW));
        for (int i = 0; i < 1; i++) {
            updateImage(&hdc, &memDC, i);
        }

        BitBlt(hdc, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBmp); // select back original bitmap
        DeleteObject(bmp); // delete bitmap since it is no longer required
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_TIMER:
    {
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_CREATE:
        if (createConsole())
            printf("Console Created...\n");
        SetTimer(hWnd, 1, 16, NULL);
        break;
    case WM_DESTROY:
        clean();
        break;
    case WM_KEYDOWN:
        if (wp == VK_LEFT) {
            sprites[0].vel.x = -1;
            flipped = false;
            animation.type = WALKING;
        }
        if (wp == VK_RIGHT) {
            sprites[0].vel.x = 1;
            flipped = true;
            animation.type = WALKING;
        }
        break;
    case WM_KEYUP:
        sprites[0].vel.x = 0;
        animation.type = IDLE;
        // Handle key up event
        break;

    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}
