#pragma once

#include <math.h>
#include "komunals.h"

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

vektor2i randomDirectionVektor2I(int length)
{
    double radians = ((double)(rand() % 360) * PI) / 180.0;
    vektor2i result = { (int)((double)length * cos(radians)), (int)((double)length * sin(radians)) };
    return result;
}

vektor2i transformUnitToPixelCoordinates(int width, int heigth, vektor2i unitCoords, int fixed_precision)
{
    vektor2i result;
    if (fixed_precision >= 0)
    {
        result.x = (((unitCoords.x * (width >> 1))) >> fixed_precision) + (width >> 1);
        result.y = (((unitCoords.y * (heigth >> 1))) >> fixed_precision) + (heigth >> 1);
    }
    else
    {
        result.x = (((unitCoords.x * (width >> 1))) << abs(fixed_precision)) + (width >> 1);
        result.y = (((unitCoords.y * (heigth >> 1))) << abs(fixed_precision)) + (heigth >> 1);
    }
    return result;
}

int volfensteinDstFunction(worldMapVPPLM map, vektor2i viewpoint, vektor2i direction, int fixed_precision)
{
    int size = map.count;
    vektor2i coords[size];

    for (int i = 0; i < size; i++) // get coords
    {
        coords[i].x = (map.lineCoords + i)->x;
        coords[i].y = (map.lineCoords + i)->y;
    }

    vektor2i hitX = nullVector2i;
    vektor2i hitY = nullVector2i;
    if (direction.x == 0)
    {
        if (direction.y == 0) { return 0; } // susy
        else if (direction.y < 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (abs(coords[size - i - 1].y) < viewpoint.y) // hit found
                {
                    // find coordiantes of ray hit
                    vektor2i hitPos = { viewpoint.x, coords[size - i - 1].y };

                    // find closest perpendicular lines
                    int hitLess = 0;
                    int hitGreater = 0;
                    if (abs(coords[0].x) > hitPos.x)
                    {
                        hitGreater = coords[0].x;
                        int hitted = (((hitPos.y > 0) && (hitGreater < 0)) || ((hitPos.y < 0) && (hitGreater > 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                    else if (abs(coords[size - 1].x) < hitPos.x)
                    {
                        hitLess = coords[size - 1].x;
                        int hitted = (((hitPos.y > 0) && (hitLess > 0)) || ((hitPos.y < 0) && (hitLess < 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                    else
                    {
                        for (int n = 1; n < size; n++)
                        {
                            if (abs(coords[n].x) > hitPos.x)
                            {
                                hitGreater = coords[n].x;
                                hitLess = coords[n - 1].x;
                            }
                        }
                        int hitted = (((hitPos.y > 0) && (hitLess > 0)) || ((hitPos.y < 0) && (hitLess < 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                if (abs(coords[i].y) > viewpoint.y) // hit found
                {
                    // find coordiantes of ray hit
                    vektor2i hitPos = { viewpoint.x, coords[i].y };

                    // find closest perpendicular lines
                    int hitLess = 0;
                    int hitGreater = 0;
                    if (abs(coords[0].x) > hitPos.x)
                    {
                        hitGreater = coords[0].x;
                        int hitted = !(((hitPos.y > 0) && (hitGreater < 0)) || ((hitPos.y < 0) && (hitGreater > 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                    else if (abs(coords[size - 1].x) < hitPos.x)
                    {
                        hitLess = coords[size - 1].x;
                        int hitted = !(((hitPos.y > 0) && (hitLess > 0)) || ((hitPos.y < 0) && (hitLess < 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                    else
                    {
                        for (int n = 1; n < size; n++)
                        {
                            if (abs(coords[n].x) > hitPos.x)
                            {
                                hitGreater = coords[n].x;
                                hitLess = coords[n - 1].x;
                            }
                        }
                        int hitted = !(((hitPos.y > 0) && (hitLess > 0)) || ((hitPos.y < 0) && (hitLess < 0)));
                        if (hitted) { hitX = hitPos; break; }
                    }
                }
            }
        }
    }
    else if (direction.x < 0)
    {
        if (direction.y == 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (abs(coords[size - i - 1].x) < viewpoint.x) // hit found
                {
                    // find coordiantes of ray hit
                    vektor2i hitPos = { coords[size - i - 1].x, viewpoint.y,  };

                    // find closest perpendicular lines
                    int hitLess = 0;
                    int hitGreater = 0;
                    if (abs(coords[0].y) > hitPos.y)
                    {
                        hitGreater = coords[0].y;
                        int hitted = (((hitPos.x > 0) && (hitGreater < 0)) || ((hitPos.x < 0) && (hitGreater > 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                    else if (abs(coords[size - 1].y) < hitPos.y)
                    {
                        hitLess = coords[size - 1].y;
                        int hitted = (((hitPos.x > 0) && (hitLess > 0)) || ((hitPos.x < 0) && (hitLess < 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                    else
                    {
                        for (int n = 1; n < size; n++)
                        {
                            if (abs(coords[n].y) > hitPos.y)
                            {
                                hitGreater = coords[n].y;
                                hitLess = coords[n - 1].y;
                            }
                        }
                        int hitted = (((hitPos.x > 0) && (hitLess > 0)) || ((hitPos.x < 0) && (hitLess < 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                }
            }
        }
        else if (direction.y < 0)
        {
            
        }
        else
        {

        }
    }
    else
    {
        if (direction.y == 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (abs(coords[i].x) > viewpoint.x) // hit found
                {
                    // find coordiantes of ray hit
                    vektor2i hitPos = { coords[i].x, viewpoint.y,  };

                    // find closest perpendicular lines
                    int hitLess = 0;
                    int hitGreater = 0;
                    if (abs(coords[0].y) > hitPos.y)
                    {
                        hitGreater = coords[0].y;
                        int hitted = !(((hitPos.x > 0) && (hitGreater < 0)) || ((hitPos.x < 0) && (hitGreater > 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                    else if (abs(coords[size - 1].y) < hitPos.y)
                    {
                        hitLess = coords[size - 1].y;
                        int hitted = !(((hitPos.x > 0) && (hitLess > 0)) || ((hitPos.x < 0) && (hitLess < 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                    else
                    {
                        for (int n = 1; n < size; n++)
                        {
                            if (abs(coords[n].y) > hitPos.y)
                            {
                                hitGreater = coords[n].y;
                                hitLess = coords[n - 1].y;
                            }
                        }
                        int hitted = !(((hitPos.x > 0) && (hitLess > 0)) || ((hitPos.x < 0) && (hitLess < 0)));
                        if (hitted) { hitY = hitPos; break; }
                    }
                }
            }
        }
        else if (direction.y < 0)
        {
            
        }
        else
        {
            
        }
    }
}
