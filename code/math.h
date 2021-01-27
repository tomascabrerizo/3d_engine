#ifndef MATH_H
#define MATH_H

#include <cmath>

#define PI 3.1415926f

struct v2 {
    float x;
    float y;
};

v2 new_v2(float x, float y);

struct v3 {
    float x;
    float y;
    float z;
};

struct m4
{
    float m[4][4];
};

float to_rad(float a);

v3 new_v3(float x, float y, float z);
v3 operator+(const v3& v0, const v3& v1);
v3 operator-(const v3& v0, const v3& v1);
v3 operator-(const v3& v0);
v3 operator*(const v3& v0, const float& a);
v3 operator/(const v3& v0, const float& a);
v3 operator*(const v3& v0, const v3& v1);
v3 operator/(const v3& v0, const v3& v1);
void operator+=(v3& v0, const v3& v1);
void operator-=(v3& v0, const v3& v1);
v3 normalize(const v3& v0);
float dot(const v3& v0, const v3& v1);
v3 cross(const v3& v0, const v3& v1);
float length_sqrt(const v3& v0);
float length(const v3& v0);
void v3_dump(const v3& a);

m4 operator+(const m4& m0, const m4& m1);
m4 operator-(const m4& m0, const m4& m1);
m4 operator*(const m4& m0, const m4& m1);
m4 identity();
m4 scale(const v3& s);
m4 translate(v3 t);
m4 rotate_x(float a);
m4 rotate_y(float a);
m4 rotate_z(float a);
m4 rotate(float a, v3 r);
m4 perspective(float fov, float aspect, float near, float far);
m4 look_at(v3 camera_pos, v3 camera_target, v3 up);
void m4_dump(const m4& e);

#endif
