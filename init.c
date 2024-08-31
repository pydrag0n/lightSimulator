#include <winapi/Windows.h>
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>

HWND window;
HDC hdc;
HGLRC hrc;

#define WH 512
#define WW 1024

typedef struct {
    float startX;
    float startY;
    float endX;
    float endY;
    float length;
    float angle;
} Ray;

typedef struct {
    float r;
    float g;
    float b;
} Color;

Color color1 = {1, 1, 1};

Ray ray1 = {WW/2, WH/2, 0, 0, 100, 0};

void drawRay(Ray *ray, Color *color) {

    glColor3f(color->r, color->g, color->b);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(ray->startX, ray->startY);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(ray->startX, ray->startY);
    glVertex2f(ray->endX, ray->endY);
    glEnd();

}
void tick(int fps) {
    Sleep(1000/fps);
}

void draw(void) {

    if (GetAsyncKeyState(VK_LEFT) < 0) {
        ray1.angle -= 0.1; // поворот влево
    };
    if (GetAsyncKeyState(VK_RIGHT) < 0) {
        ray1.angle += 0.1; // поворот вправо
    };

    // рассчитываем конечные координаты луча
    ray1.endX = ray1.startX + ray1.length * cos(ray1.angle);
    ray1.endY = ray1.startY + ray1.length * sin(ray1.angle);
    printf("%f", ray1.angle);
    drawRay(&ray1, &color1);
}

void R_Render(void)
{

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    tick(60);
    draw();
    SwapBuffers(hdc);
}

void GL_Init(void)
{
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