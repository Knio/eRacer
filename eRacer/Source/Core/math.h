#ifndef INC_MATH
#define INC_MATH

#include <limits>
#include <cfloat>

#define PI      (3.14159265358979323846)
#define CI      (3.14159265358979323846*2)
#define EI      (3.14159265358979323846/3)
#define RI      (3.14159265358979323846/2)
#define FI      (3.14159265358979323846/4)

#define EE      (2.71828182845904523536)

#define DEG2RAD(x) (PI*(x)/180.0)
#define RAD2DEG(x) (180.0*(x)/PI)

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

#define ZERO (FLT_EPSILON)

#define SQ(x) ((x)*(x))

float INF   = std::numeric_limits<float>::infinity();
float NaN   = std::numeric_limits<float>::quiet_NaN();


template <typename T>
bool isnan(T x) { return x != x; }

double normal(double alpha, double x)
{
    return pow(EE, -(SQ(x)/(2*SQ(alpha)))) / sqrt(2*PI*alpha);
}



#endif
