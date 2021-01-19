#include <cstdio>
#include "math.h"

float to_rad(float a)
{
    return a*PI/180;
}


v3 new_v3(float x, float y, float z)
{
    return {x, y, z};
}

v3 operator+(const v3& v0, const v3& v1)
{
    return {v0.x + v1.x, v0.y + v1.y, v0.z + v1.z};
}

v3 operator-(const v3& v0, const v3& v1)
{
    return {v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};
}

v3 operator-(const v3& v0)
{
    return {-v0.x, -v0.y, -v0.z};
}

void operator+=(v3& v0, const v3& v1)
{
    v0 = v0 + v1;
}
void operator-=(v3& v0, const v3& v1)
{
    v0 = v0 - v1;
}

v3 operator*(const v3& v0, const float& a)
{
    return {v0.x * a, v0.y * a, v0.z * a};
}


v3 operator/(const v3& v0, const float& a)
{
    return {v0.x / a, v0.y / a, v0.z / a};
}

v3 operator*(const v3& v0, const v3& v1)
{
    return {v0.x * v1.x, v0.y * v1.y, v0.z * v1.z};
}
v3 operator/(const v3& v0, const v3& v1)
{
    return {v0.x / v1.x, v0.y / v1.y, v0.z / v1.z};
}

v3 normalize(const v3& v0)
{
    return v0 / length(v0);
}

float dot(const v3& v0, const v3& v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; 
}

v3 cross(const v3& v0, const v3& v1)
{
    return {v0.y*v1.z-v0.z*v1.y, v0.z*v1.x-v0.x*v1.z, v0.x*v1.y-v0.y*v1.x};
}

float length_sqrt(const v3& v0)
{
    return dot(v0, v0);
}

float length(const v3& v0)
{
    return sqrtf(dot(v0, v0));
}

void v3_dump(const v3& a)
{
    printf("a = x:%f y:%f z:%f\n", a.x, a.y, a.z);
}


m4 operator+(const m4& m0, const m4& m1)
{
    m4 result = {};
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] = m0.m[y][x] + m1.m[y][x];
        }
    }
    return result;
}

m4 operator-(const m4& m0, const m4& m1)
{
    m4 result = {};
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] = m0.m[y][x] - m1.m[y][x];
        }
    }
    return result;
}

m4 operator*(const m4& m0, const m4& m1)
{
    m4 result = {};
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] =
            m0.m[y][0]*m1.m[0][x] +
            m0.m[y][1]*m1.m[1][x] +
            m0.m[y][2]*m1.m[2][x] +
            m0.m[y][3]*m1.m[3][x];
        }
    }
    return result;
}

m4 identity()
{
    m4 i = {};
    i.m[0][0] = 1;
    i.m[1][1] = 1;
    i.m[2][2] = 1;
    i.m[3][3] = 1;
    return i;
}

m4 scale(const v3& s)
{
    m4 m = {};
    m.m[0][0] = s.x;
    m.m[1][1] = s.y;
    m.m[2][2] = s.z;
    m.m[3][3] = 1;
    return m;
}

m4 translate(v3 t)
{
    m4 m = identity();
    m.m[0][3] = t.x;
    m.m[1][3] = t.y;
    m.m[2][3] = t.z;
    return m;
}

m4 rotate_x(float a)
{
    float ra = to_rad(a);
    m4 m = identity();
    m.m[1][1] = cosf(ra);
    m.m[1][2] = -sinf(ra);
    m.m[2][1] = sinf(ra);
    m.m[2][2] = cosf(ra);
    return m;
}

m4 rotate_y(float a)
{
    float ra = to_rad(a);
    m4 m = identity();
    m.m[0][0] = cosf(ra);
    m.m[0][2] = sinf(ra);
    m.m[2][0] = -sinf(ra);
    m.m[2][2] = cosf(ra);
    return m;
}

m4 rotate_z(float a)
{
    float ra = to_rad(a);
    m4 m = identity();
    m.m[0][0] = cosf(ra);
    m.m[0][1] = -sinf(ra);
    m.m[1][0] = sinf(ra);
    m.m[1][1] = cosf(ra);
    return m;
}

m4 rotate(float a, v3 r)
{
    float ar = to_rad(a);
    float c = cosf(ar);
    float s = sinf(ar);
    m4 m = identity();
    m.m[0][0] = c+(1-c)*(r.x*r.x);
    m.m[0][1] = (1-c)*r.x*r.y-s*r.z;
    m.m[0][2] = (1-c)*r.x*r.z+s*r.y;

    m.m[1][0] = (1-c)*r.x*r.y+s*r.z;
    m.m[1][1] = c+(1-c)*(r.y*r.y);
    m.m[1][2] = (1-c)*r.y*r.z-s*r.x;

    m.m[2][0] = (1-c)*r.x*r.z-s*r.y;
    m.m[2][1] = (1-c)*r.y*r.z+s*r.x;
    m.m[2][2] = c+(1-c)*(r.z*r.z);
    return m;
}


m4 perspective(float fov, float aspect, float near, float far)
{
    m4 m = {};
    m.m[0][0] = 1 / (aspect*tanf(to_rad(fov)*0.5f));
    m.m[1][1] = 1 / tanf(to_rad(fov)*0.5f);
    m.m[2][2] = -(far+near)/(far-near);
    m.m[3][2] = -1;
    m.m[2][3] = -(2*far*near)/(far-near);
    return m;
}

m4 look_at(v3 camera_pos, v3 camera_target, v3 up)
{
    v3 camera_direction = normalize(camera_pos - camera_target);
    v3 camera_right = normalize(cross(up, camera_direction));
    v3 camera_up = normalize(cross(camera_direction, camera_right));
    m4 m = identity();
    m.m[0][0] = camera_right.x;
    m.m[0][1] = camera_right.y;
    m.m[0][2] = camera_right.z;
    m.m[1][0] = camera_up.x;
    m.m[1][1] = camera_up.y;
    m.m[1][2] = camera_up.z;
    m.m[2][0] = camera_direction.x;
    m.m[2][1] = camera_direction.y;
    m.m[2][2] = camera_direction.z;
    return m * translate(-camera_pos);
}

void m4_dump(const m4& e)
{
    printf("{%f, %f, %f, %f}\n", e.m[0][0], e.m[0][1], e.m[0][2], e.m[0][3]);
    printf("{%f, %f, %f, %f}\n", e.m[1][0], e.m[1][1], e.m[1][2], e.m[1][3]);
    printf("{%f, %f, %f, %f}\n", e.m[2][0], e.m[2][1], e.m[2][2], e.m[2][3]);
    printf("{%f, %f, %f, %f}\n", e.m[3][0], e.m[3][1], e.m[3][2], e.m[3][3]);
}
