#pragma once

#include <math.h>

#define FIXED_PRECISION 16


typedef struct // intager vektor
{
    int x;
    int y;
}vektor2i;
const vektor2i nullVector2f = { 0, 0 };

vektor2i addVektors2I(vektor2i vektorA, vektor2i vektorB) // vektorA + vektorB
{
    return (vektor2i){ vektorA.x + vektorB.x, vektorA.y + vektorB.y };
}
vektor2i subtractVektors2I(vektor2i vektorA, vektor2i vektorB) // vektorA - vektorB
{
    return (vektor2i){ vektorA.x - vektorB.x, vektorA.y - vektorB.y };
}
vektor2i negateVektor2I(vektor2i vektor) // -vektor
{
    return (vektor2i){ -vektor.x, -vektor.y };
}
vektor2i scaleVektor2I(int scalar, vektor2i vektor)
{
    return (vektor2i){ scalar * vektor.x, scalar * vektor.y };
}
int magnitudeSquaredOfVektor2I(vektor2i vektor)
{
    return (int)((vektor.x * vektor.x) + (vektor.y * vektor.y));
}
int magnitudeOfVektor2I(vektor2i vektor)
{
    return (int)sqrt((vektor.x * vektor.x) + (vektor.y * vektor.y));
}
vektor2i normaliseVektor2I(int scalar, vektor2i vektor)
{
    int length = (int)sqrt((vektor.x * vektor.x) + (vektor.y * vektor.y));
    return (vektor2i){ (vektor.x * scalar) / length, (vektor.y * scalar) / length };
}