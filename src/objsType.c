#ifndef OBJSTYPES_H
#define OBJSTYPES_H

#define MAX_LINE_LENGTH 1024
#define MAX_CONFIG_ENTRIES 100


typedef struct {
    int startX;
    int startY;
    float endX;
    float endY;
    int length;
    float angle;
} Ray;

typedef struct {
    float r;
    float g;
    float b;
} Color;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect;

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} ConfigEntry;


#endif