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
void updateImage(HDC* hdc, HDC* memDC, int i, struct sprite *sprites) {
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

        float amountOfFrames = newWidth / 160;

        AlphaBlend(*memDC, 
            current_sprite->pos.x, 
            current_sprite->pos.y, 
            current_sprite->dim.x, 
            current_sprite->dim.y, 
            hdcSprite,
            abs(flipped * newWidth * ((amountOfFrames - 1) / amountOfFrames) - current_sprite->imageCoordinate.x * MULTIPLIER),
            current_sprite->imageCoordinate.y * MULTIPLIER,
            current_sprite->imageDimension.x * MULTIPLIER, 
            current_sprite->imageDimension.y * MULTIPLIER,
            blend);

        SelectObject(hdcSprite, hbmOld);
        DeleteObject(hdcSprite);
        DeleteObject(hbmDib);
        free(bits);
    }
}
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    float speed = 1;

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
        _Bool stored = flipped;
        flipped = false;
        for (int i = 1; i < 100; i++) {
            updateImage(&hdc, &memDC, i, staticSprites);
        }
        flipped = stored;
        for (int i = 0; i < 1; i++) {
            updateImage(&hdc, &memDC, i, dynamicSprites);
        }

        BitBlt(hdc, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBmp);
        DeleteObject(bmp);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_TIMER:
    {
        input();
        update();
        render();

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
            vel.x = -5;
            flipped = false;
        }
        if (wp == VK_RIGHT) {
            vel.x = 5;
            flipped = true;
        }
        if (wp == VK_UP && grounded) {
            vel.y = -32;
            grounded = false;
        }
        if (wp == VK_DOWN) {
            vel.y = speed;
        }
        break;
    case WM_KEYUP:
        if (wp == VK_LEFT) {
            vel.x = 0;
        }
        if (wp == VK_RIGHT) {
            vel.x = 0;
        }
        if (wp == VK_UP && vel.y < -10) {
            vel.y += 10;
        }
        if (wp == VK_DOWN) {
            //vel.y = 0;
        }
        // Handle key up event
        break;

    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }

    if (vel.y < 0)
        animation.type = JUMPING;
    else if (vel.y > 1)
        animation.type = FALLING;
    else if (vel.x == 0)
        animation.type = IDLE;
    else
        animation.type = WALKING;

    return 0;
}
