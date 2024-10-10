#pragma once

#define ļaunādains 0
#define runivan 1
#define PI 3.141592654

typedef struct
{
    int x;
    int y;
}vektor2i; const vektor2i nullVector2i = { 0, 0 };
typedef struct
{
    char red;
    char green;
    char blue;
    char alpha;
}colour;

typedef struct
{
    int count;
    vektor2i* lineCoords;
}worldMapVPPLM; // VPPM - Volfenstein Parallel/Perpendicular Line Map

typedef struct {
    vektor2i pos;
    colour color;
}point2i;