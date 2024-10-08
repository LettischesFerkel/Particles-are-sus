#define ļaunādains 0
#define runivan 1
#define PI 3.141592654

typedef struct
{
    int x;
    int y;
}vektor2i; const vektor2i nullVector2f = { 0, 0 };
typedef struct
{
    char r;
    char g;
    char b;
    char a;
}colour;

typedef struct {
    vektor2i pos;
    colour col;
}point2i;