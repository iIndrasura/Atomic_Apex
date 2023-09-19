#include "math.hpp"

/*
#include <math.h>
#include <stdint.h>
#include <cmath> // for the sqrtf function
*/

float qsqrt(float x)
{
	union
	{
		int i;
		float f;
	} u;

	u.f = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

	u.f = u.f + x / u.f;
	u.f = 0.25f * u.f + x / u.f;

	return u.f;
}

float vec_length_sqrt(vec3 p0)
{
	return (float)qsqrt(p0.x * p0.x + p0.y * p0.y + p0.z * p0.z);
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

vec3 vec_sub(vec3 p0, vec3 p1)
{
	vec3 r;

	r.x = p0.x - p1.x;
	r.y = p0.y - p1.y;
	r.z = p0.z - p1.z;
	return r;
}

float qfloor(float x)
{
	if (x >= 0.0f)
		return (float)((int)x);
	return (float)((int)x - 1);
}

float qfmodf(float a, float b)
{
	return (a - b * qfloor(a / b));
}

void vec_clamp(vec3 *v)
{

	if (v->x > 89.0f && v->x <= 180.0f)
	{
		v->x = 89.0f;
	}
	if (v->x > 180.0f)
	{
		v->x = v->x - 360.0f;
	}
	if (v->x < -89.0f)
	{
		v->x = -89.0f;
	}
	v->y = qfmodf(v->y + 180, 360) - 180;
	v->z = 0;
}

float qatan2(float y, float x)
{
	float t0, t1, t3, t4;
	t3 = qabs(x);
	t1 = qabs(y);
	t0 = qmax(t3, t1);
	t1 = qmin(t3, t1);
	t3 = 1 / t0;
	t3 = t1 * t3;

	t4 = t3 * t3;
	t0 = -0.0134804f;
	t0 = t0 * t4 + 0.05747f;
	t0 = t0 * t4 - 0.12123f;
	t0 = t0 * t4 + 0.19563f;
	t0 = t0 * t4 - 0.33299f;
	t0 = t0 * t4 + 0.99999f;
	t3 = t0 * t3;

	t3 = (qabs(y) > qabs(x)) ? 1.57079f - t3 : t3;
	t3 = (x < 0) ? X_PI - t3 : t3;
	t3 = (y < 0) ? -t3 : t3;

	return t3;
}
float vec_distance(vec3 p0, vec3 p1)
{
	return vec_length_sqrt(vec_sub(p0, p1));
}

float qatan(float x)
{
	return qatan2(x, 1);
}

float CalcDistance(vec3 p0, vec3 p1) {
    float dx = p0.x - p1.x;
    float dy = p0.y - p1.y;
    float dz = p0.z - p1.z;
    float distance = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
    return distance;
 }

vec3 CalcAngle(vec3 src, vec3 dst)
{
	vec3 angle;

	vec3 delta = vec_sub(src, dst);

	float hyp = qsqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = qatan(delta.z / hyp) * (float)(180.0 / M_PI);
	angle.y = qatan(delta.y / delta.x) * (float)(180.0 / M_PI);
	angle.z = 0;

	if (delta.x >= 0.0)
		angle.y += 180.0f;

	return angle;
}

// Apply deadzone to the x and y components of a vec3 angle
vec3 ApplyDeadzone(vec3 target_angle, float deadzone) 
{
    float x = target_angle.x;
    float y = target_angle.y;

    // Apply deadzone logic
    if (abs(x) < deadzone) {
        x = 0.0f;
    }

    if (abs(y) < deadzone) {
        y = 0.0f;
    }

    vec3 result;
    result.x = x;
    result.y = y;
	result.z = 0.0f;
    //result.z = target_angle.z; // Keep the z component unchanged

    return result;
}


double qpow(double a, double b) {
	double c = 1;
	for (int i = 0; i < b; i++)
		c *= a;
	return c;
}

float get_fov(vec3 scrangles, vec3 aimangles)
{
	vec3 delta;

	delta.x = aimangles.x - scrangles.x;
	delta.y = aimangles.y - scrangles.y;
	delta.z = aimangles.z - scrangles.z;

	if (delta.x > 180)
		delta.x = 360 - delta.x;
	if (delta.x < 0)
		delta.x = -delta.x;

	delta.y = qfmodf(delta.y + 180, 360) - 180;
	if (delta.y < 0)
		delta.y = -delta.y;

	return qsqrt((float)(qpow(delta.x, 2.0) + qpow(delta.y, 2.0)));
}