#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>
#include "objsType.c"

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
    float x = round(p->x * 100) / 100;
    float y = round(p->y * 100) / 100;
    glColor3f(1, 0, 0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

