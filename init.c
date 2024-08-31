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

const int rayCol = 64;


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
Ray rayList[100];
Point intersectionPoint;
Color color1 = {1, 1, 1};

// Ray ray1 = {WW/2, WH/2, 0, 0, defLength, 0};
Rect rect = {300, 300, 50, 50};

short checkRayRectangleIntersection(Ray *ray, Rect *rect, Point *intersectionPoint) {
    float minX = rect->x;
    float maxX = rect->x + rect->width;
    float minY = rect->y;
    float maxY = rect->y + rect->height;

    float rayDirX = cos(ray->angle);
    float rayDirY = sin(ray->angle);

    float tMin = INFINITY;

    // Check for division by zero
    if (rayDirX == 0) {
        if (ray->startX < minX || ray->startX > maxX) return 0;
        float t = (maxY - ray->startY) / rayDirY;
        if (t >= 0 && t <= ray->length) {
            intersectionPoint->x = ray->startX;
            intersectionPoint->y = maxY;
            return 1;
        }
        t = (minY - ray->startY) / rayDirY;
        if (t >= 0 && t <= ray->length) {
            intersectionPoint->x = ray->startX;
            intersectionPoint->y = minY;
            return 1;
        }
        return 0;
    }

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

    // glLineWidth(5);
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

void drawRayList(void) {

    float defGap = -1.5;
    for(int i = 0; i<rayCol; i++) {
        float gap = 0.04;
        rayList[i].startX = WW/2;
        rayList[i].startY = WH/2;
        rayList[i].angle = defGap;
        rayList[i].length = 800;
        rayList[i].endX = rayList[i].startX + rayList[i].length * cos(rayList[i].angle);
        rayList[i].endY = rayList[i].startY + rayList[i].length * sin(rayList[i].angle);
        defGap+=gap;
    }
}

void draw(void) {
    for(int i = 0; i<rayCol; i++) {
        // Update ray positions
        if (GetAsyncKeyState(VK_LEFT) < 0) {
            rayList[i].angle -= 0.01; // влево
        };

        if (GetAsyncKeyState(VK_RIGHT) < 0) {
            rayList[i].angle += 0.01; // вправо
        };
        // W: 0x57 A:0x41 S:0x53 D:0x44
        if (GetAsyncKeyState(0x57) < 0) {
            rayList[i].startY -= 1;
        }
        if (GetAsyncKeyState(0x41) < 0) {
            rayList[i].startX -= 1;
        }
        if (GetAsyncKeyState(0x53) < 0) {
            rayList[i].startY += 1;
        }
        if (GetAsyncKeyState(0x44) < 0) {
            rayList[i].startX += 1;
        }

        // Calculate end points of the ray
        rayList[i].endX = rayList[i].startX + rayList[i].length * cos(rayList[i].angle);
        rayList[i].endY = rayList[i].startY + rayList[i].length * sin(rayList[i].angle);

        // Check intersection
        short isColised = checkRayRectangleIntersection(&rayList[i], &rect, &intersectionPoint);
        if (isColised > 0) {
            Color color2 = {1, 0, 0};
            rayList[i].endX = intersectionPoint.x;
            rayList[i].endY = intersectionPoint.y;
        }

        drawRay(&rayList[i], &color1);
        Point p = {rayList[i].endX, rayList[i].endY};
        drawPoint(&p);
    }
    drawRect(&rect);
}

void R_Render(void)
{

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    tick(120);
    draw();
    SwapBuffers(hdc);
}

void GL_Init(void)
{
    drawRayList();
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