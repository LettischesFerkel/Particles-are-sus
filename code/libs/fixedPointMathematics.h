#pragma once

typedef struct {
    int value;
    char precision;
}fixed; // fixed point scalar
fixed addFixed(fixed a, fixed b)
{
    char relativePrecision = b.precision - a.precision;
    int rawA;
    int rawB;
}


typedef struct {
    int x;
    int y;
    char precision;
}vektor2F; // Fixed point 2dimensional vektor