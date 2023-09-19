#ifndef MATH_HPP
#define MATH_HPP

#include <math.h>
#include <stdint.h>
#include <cmath>


#define X_PI 3.14159f
#define M_PI 3.14159265358979323846264338327950288
#define X_DIV 57.29577f
#define qabs(x) ((x) < 0 ? -(x) : (x))
#define qmax(a, b) (((a) > (b)) ? (a) : (b))
#define qmin(a, b) (((a) < (b)) ? (a) : (b))

const float GAME_UNITS_TO_METERS = 0.01905f;

typedef struct
{
	float x, y, z;
} vec3;

typedef struct
{
    float x, y;
} vec2;

typedef struct {
    int8_t GeneralGlowMode;
    int8_t BorderGlowMode;
    int8_t BorderSize;
    int8_t TransparentLevel;
} GlowMode;

typedef struct
{
	float r, b, g;
}Color;

typedef struct
{
	uint8_t pad1[0xCC];
	float x;
	uint8_t pad2[0xC];
	float y;
	uint8_t pad3[0xC];
	float z;
} matrix3x4_t;

typedef struct {
    float matrix[16];
} Matrix;

// Define a mouse_data struct and populate its fields
typedef struct {
	int x, y;
} MouseMovementData;

float lerp(float a, float b, float t);

float qsqrt(float x);

float vec_length_sqrt(vec3 p0);

vec3 vec_sub(vec3 p0, vec3 p1);

float qfloor(float x);

float qfmodf(float a, float b);

void vec_clamp(vec3 *v);

float qatan2(float y, float x);

float vec_distance(vec3 p0, vec3 p1);

float qatan(float x);

float CalcDistance(vec3 p0, vec3 p1);

vec3 CalcAngle(vec3 src, vec3 dst);

// Apply deadzone to the x and y components of a vec3 angle
vec3 ApplyDeadzone(vec3 target_angle, float deadzone);

double qpow(double a, double b);

float get_fov(vec3 scrangles, vec3 aimangles);

#endif // MATH_HPP