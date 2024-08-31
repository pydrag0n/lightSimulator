#include <winapi/Windows.h>
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>

HWND window;
HDC hdc;
HGLRC hrc;

#define WH 512
#define WW 1024
#define defLength 800

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

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Rect;

typedef struct {
    float x;
    float y;
} Point;

Point intersectionPoint;
Color color1 = {1, 1, 1};

Ray ray1 = {WW/2, WH/2, 0, 0, defLength, 0};
Rect rect = {300, 300, 50, 50};


short checkRayRectangleIntersection(Ray *ray, Rect *rect, Point *intersectionPoint) {
    float minX = rect->x;
    float maxX = rect->x + rect->width;
    float minY = rect->y;
    float maxY = rect->y + rect->height;

    float rayDirX = cos(ray->angle);
    float rayDirY = sin(ray->angle);

    float tMin = INFINITY; // наименьшее 

    // Проверяем пересечение с левой границей
    if (rayDirX != 0) {
        float t = (minX - ray->startX) / rayDirX;
        if (t >= 0 && t <= ray->length && t < tMin) {
            float y = ray->startY + t * rayDirY;
            if (y >= minY && y <= maxY) {
                intersectionPoint->x = minX;
                intersectionPoint->y = y;
                tMin = t;
            }
        }
    }

    // Проверяем пересечение с правой границей
    if (rayDirX != 0) {
        float t = (maxX - ray->startX) / rayDirX;
        if (t >= 0 && t <= ray->length && t < tMin) {
            float y = ray->startY + t * rayDirY;
            if (y >= minY && y <= maxY) {
                intersectionPoint->x = maxX;
                intersectionPoint->y = y;
                tMin = t;
            }
        }
    }

    // Проверяем пересечение с верхней границей
    if (rayDirY != 0) {
        float t = (maxY - ray->startY) / rayDirY;
        if (t >= 0 && t <= ray->length && t < tMin) {
            float x = ray->startX + t * rayDirX;
            if (x >= minX && x <= maxX) {
                intersectionPoint->x = x;
                intersectionPoint->y = maxY;
                tMin = t;
            }
        }
    }

    // Проверяем пересечение с нижней границей
    if (rayDirY != 0) {
        float t = (minY - ray->startY) / rayDirY;
        if (t >= 0 && t <= ray->length && t < tMin) {
            float x = ray->startX + t * rayDirX;
            if (x >= minX && x <= maxX) {
                intersectionPoint->x = x;
                intersectionPoint->y = minY;
                tMin = t;
            }
        }
    }

    return (tMin != INFINITY) ? 1 : 0;
}

void drawRect(Rect *rect) {
    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    glVertex2i(rect->x, rect->y);
    glVertex2i(rect->x+rect->width, rect->y);
    glVertex2i(rect->x+rect->width, rect->y+rect->height);
    glVertex2i(rect->x, rect->y+rect->height);
    glEnd();
}

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

void drawPoint(Point *p) {
    float x = round(p->x * 100.0f) / 100.0f;
    float y = round(p->y * 100.0f) / 100.0f;
    glColor3f(1, 0, 0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void tick(int fps) {
    Sleep(1000/fps);
}

void draw(void) {

    if (GetAsyncKeyState(VK_LEFT) < 0) {
        ray1.angle -= 0.01; // влево
    };
    if (GetAsyncKeyState(VK_RIGHT) < 0) {
        ray1.angle += 0.01; // вправо
    };

    // рассчитываем конечные корды луча
    ray1.endX = ray1.startX + ray1.length * cos(ray1.angle);
    ray1.endY = ray1.startY + ray1.length * sin(ray1.angle);
    float num = ray1.angle;
    short isColised = checkRayRectangleIntersection(&ray1, &rect, &intersectionPoint);
    printf("%d\n || %.2f", isColised, num);
    if (isColised>0) {
        Color color2 = {1, 0, 0};
        Ray ray2;
        printf("Intersection point: (%f, %f)\n", intersectionPoint.x, intersectionPoint.y);
        ray1.endX = intersectionPoint.x;
        ray1.endY = intersectionPoint.y;
        // ray2.startX = intersectionPoint.x;
        // ray2.startY = intersectionPoint.y;
        // ray2.length = defLength;
        // ray2.angle = 0.0;
        // ray2.endX = ray2.startX + ray2.length * cos(ray2.angle);
        // ray2.endY = ray2.startY + ray2.length * sin(ray2.angle);
        // drawRay(&ray2, &color2);
    }

    drawRect(&rect);
    drawRay(&ray1, &color1);
    Point p = {ray1.endX, ray1.endY};
    drawPoint(&p);

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