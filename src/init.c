#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>
#include "functions.c"
#include "objsType.c"
#include <stdio.h>
#include "config.c"
#include <string.h>


HWND window;
HDC hdc;
HGLRC hrc;


void tick(int fps) {
    Sleep(1000/fps);
    
}
void draw(void) {


    // Update ray positions based on key presses
    if (GetAsyncKeyState(VK_LEFT) < 0) {
        angle -= 0.01; // влево
    }
    else if (GetAsyncKeyState(VK_RIGHT) < 0) {
        angle += 0.01; // вправо
    }
    if (GetAsyncKeyState(0x57) < 0) { // W key
        startY -= 1;
    }
    if (GetAsyncKeyState(0x41) < 0) { // A key
        startX -= 1;
    }
    if (GetAsyncKeyState(0x53) < 0) { // S key
        startY += 1;
    }
    if (GetAsyncKeyState(0x44) < 0) { // D key
        startX += 1;
    }

    // Limit angle to a reasonable range
    angle = fmod(angle, 2 * 3.14159f); // Keep angle within 0 to 2*PI

    for (int i = 0; i < rayCol; i++) {
        float endX = startX + length * cos(angle + i * rayGap);
        float endY = startY + length * sin(angle + i * rayGap);

        Ray ray = {startX, startY, endX, endY, length, angle + i * 0.01};

        Point intersectionPoint;
        for(int rlen = 0; rlen < 10; rlen++) {
            short isColised = checkRayRectangleIntersection(&ray, &rectsList[rlen], &intersectionPoint);
            if (isColised > 0) {
                Color color2 = { 1, 0, 0 };
                ray.endX = intersectionPoint.x;
                ray.endY = intersectionPoint.y;
            }
        }

        Color rayColor = {1, 1, 1};
        drawRay(&ray, &rayColor);
        Point p = {ray.endX, ray.endY};
        drawPoint(&p);

    }

    for(int rd = 0; rd < 10; rd++) {
        drawRect(&rectsList[rd]);
    }
}

void R_Render(void)
{

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    tick(FPS);
    draw();
    SwapBuffers(hdc);
}

void GL_Init(void)
{
    InitMap();
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    hdc = GetDC(window);
    memset(&pfd, 0, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(hdc, &pfd);

    SetPixelFormat(hdc, iFormat, &pfd);

    hrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hrc);
    glOrtho(0, WW, WH, 0, -1, 1);
}

void GL_Shutdown(void)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
    ReleaseDC(window, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        default:
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    return 0;
}


void main()
{
    WNDCLASSA    wcl;
    memset(&wcl, 0, sizeof(wcl));

    wcl.lpfnWndProc = WndProc;
    wcl.lpszClassName = "ABC_CLASS";

    RegisterClassA(&wcl);

    window = CreateWindowExA(0, "ABC_CLASS", "name", WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
    0, 0, 1024, 512, 0, 0, 0, 0);

    GL_Init();
    ShowWindow(window, SW_SHOWNORMAL);

    unsigned char running = 1;
    MSG msg;

    while(running) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT)
                running = 0;
            else {
                DispatchMessage(&msg);
                TranslateMessage(&msg);
            }
        } else {
            R_Render();
        }
    }
    GL_Shutdown();
    DestroyWindow(window);
}