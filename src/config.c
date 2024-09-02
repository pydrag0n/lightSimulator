#include "objsType.c"

// window
#define WH 512
#define WW 1024
#define FPS 120


// ray settings
Ray rayList[100];
const int rayCol = 100;
const float rayGap = 0.01;
// ------------------------
int startY = WH/2;
int startX = WW/2;
int length = 1000;
float angle = 0.0;
float endX;
float endY;
// ------------------------



Point intersectionPoint;

// map
Rect rectsList[10]; // MAP size
int blockSize = 20;
void InitMap(void) {
    rectsList[0] = (Rect){ 0,    0,   blockSize, blockSize };
    rectsList[1] = (Rect){ 91,   690, blockSize, blockSize };
    rectsList[2] = (Rect){ 203,  203, blockSize, blockSize };
    rectsList[3] = (Rect){ 233,  330, blockSize, blockSize };
    rectsList[4] = (Rect){ 40,   52., blockSize, blockSize };
    rectsList[5] = (Rect){ 500,  50,  blockSize, blockSize };
    rectsList[6] = (Rect){ 400,  300, blockSize, blockSize };
    rectsList[7] = (Rect){ 94,   100, blockSize, blockSize };
    rectsList[8] = (Rect){ 81,   230, blockSize, blockSize };
    rectsList[9] = (Rect){ 110,  190, blockSize, blockSize };
}
