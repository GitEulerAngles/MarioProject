#ifndef WINDOW
#define WINDOW

#include <windows.h>
#include <FreeImage.h>
#include <stdio.h>
#include "sprites.h"

#define true 1
#define false 0

HWND hwnd;

_Bool createConsole();
FIBITMAP* loadPNGImage(char path[]);
void updateImage(HDC* hdc, HDC* memDC, int i, struct sprite* sprites);
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

#endif // WINDOW