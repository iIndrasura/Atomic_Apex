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
const float INCHES_TO_METER_RATE = 39.3701;

typedef struct
{
	float x; // 0x8-0xC
    float y; // 0xC-0x10
    float z; // 0x10-0x14
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

// Distance calculation
double distanceToMeters(float distance);
float CalcDistance(vec3 p0, vec3 p1);
double calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2);
double calculateDistance2D(float x1, float y1, float x2, float y2);
void convertPointByYaw(double yaw, double x, double y, double& xOut, double& yOut);
double calculateDistanceInMeters(float x1, float y1, float z1, float x2, float y2, float z2);

vec3 CalcAngle(vec3 src, vec3 dst);

// Apply deadzone to the x and y components of a vec3 angle
vec3 ApplyDeadzone(vec3 target_angle, float deadzone);

double qpow(double a, double b);

float get_fov(vec3 scrangles, vec3 aimangles);

#endif // MATH_HPP