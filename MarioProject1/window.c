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
void updateHDC(HDC* hdc, HDC* hdcSprite, HDC* memDC, FIBITMAP* map, HBITMAP* hbmOld, _Bool isFlipped) {
    if (map) {
        int newWidth = FreeImage_GetWidth(map);
        int newHeight = FreeImage_GetHeight(map);

        FIBITMAP* resizedBitmap = map;

        BYTE* bits = (BYTE*)malloc(newWidth * newHeight * 4);

        FreeImage_ConvertToRawBits(bits, resizedBitmap, newWidth * 4, 32,
            FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

        if (isFlipped) {
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

        *hdcSprite = CreateCompatibleDC(*hdc);
        if (!*hdcSprite) {
            printf("Error: hdcSprite note created.\n");
            free(bits);
            return;
        }

        HBITMAP hbmDib = CreateDIBitmap(*memDC, &bmi.bmiHeader, CBM_INIT, bits, &bmi, DIB_RGB_COLORS);
        if (!hbmDib) {
            printf("Error: hbmDib not created.\n");
            free(bits);
            DeleteDC(*hdcSprite);
            return;
        }

        *hbmOld = (HBITMAP)SelectObject(*hdcSprite, hbmDib);

        DeleteObject(hbmDib);
        free(bits);
    }
}
void updateImage(HDC* hdc, HDC* memDC, int width, _Bool isFlipped, struct Sprite* current_sprite) {
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    float amountOfFrames = width / 160;

    AlphaBlend(*memDC, 
        current_sprite->pos.x - cameraPos.x, 
        current_sprite->pos.y - cameraPos.y, 
        current_sprite->dim.x, 
        current_sprite->dim.y, 
        *hdc,
        abs(isFlipped * width * ((amountOfFrames - 1) / amountOfFrames) - current_sprite->imageCoordinate.x * MULTIPLIER),
        current_sprite->imageCoordinate.y * MULTIPLIER,
        current_sprite->imageDimension.x * MULTIPLIER, 
        current_sprite->imageDimension.y * MULTIPLIER,
        blend);
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

        HDC sprite;
        HBITMAP hbmOld;
        
        // Background texture
        updateHDC(&hdc, &sprite, &memDC, backgroundTexture, &hbmOld, false);

        for (int i = 0; i < 2; i++)
            updateImage(&sprite, &memDC, FreeImage_GetWidth(backgroundTexture), false, &backgroundSprite[i]);

        SelectObject(sprite, hbmOld);
        DeleteObject(sprite);

        // Block textures
        updateHDC(&hdc, &sprite, &memDC, blockTexture, &hbmOld, false);

        for (int i = 0; i < 121; i++) {
            updateImage(&sprite, &memDC, FreeImage_GetWidth(blockTexture), false, &staticSprites[i]);
        }

        SelectObject(sprite, hbmOld);
        DeleteObject(sprite);

        // Mario textures
        updateHDC(&hdc, &sprite, &memDC, marioTexture, &hbmOld, flipped);

        for (int i = 0; i < 1; i++) {
            updateImage(&sprite, &memDC, FreeImage_GetWidth(marioTexture), flipped, &dynamicSprites[i]);
        }

        SelectObject(sprite, hbmOld);
        DeleteObject(sprite);

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
        if (wp == VK_SPACE) {
            running = true;
        }
        if (wp == VK_LEFT) {
            if (vel.x >= -5)
                vel.x = -5;
            flipped = false;
        }
        if (wp == VK_RIGHT) {
            if (vel.x <= 5)
                vel.x = 5;
            flipped = true;
        }
        if (wp == VK_UP && grounded) {
            vel.y = -32;
            grounded = false;
        }
        if (wp == VK_DOWN) {

        }
        break;
    case WM_KEYUP:
        if (wp == VK_SPACE) {
            running = false;
        }
        if (wp == VK_LEFT && vel.x >= -5) {
            vel.x = 0;
        }
        if (wp == VK_RIGHT && vel.x <= 5) {
            vel.x = 0;
        }
        if (wp == VK_UP && vel.y < -20) {
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
    if (vel.y > 40)
        animation.type = FALLING;
    else if (!grounded)
        animation.type = JUMPING;
    else if (vel.x == 0)
        animation.type = IDLE;
    else
        animation.type = WALKING;

    return 0;
}
