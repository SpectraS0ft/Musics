#ifndef MATH3D_H
#define MATH3D_H

#include <cmath>
#include <cstring>

namespace Math3D {

struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
    
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }
    
    Vector3& operator+=(const Vector3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vector3 normalize() const {
        float len = length();
        if (len > 0) return *this * (1.0f / len);
        return Vector3();
    }
    
    float dot(const Vector3& v) const {
        return x*v.x + y*v.y + z*v.z;
    }
    
    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x
        );
    }
};

struct Vector4 {
    float x, y, z, w;
    
    Vector4() : x(0), y(0), z(0), w(1) {}
    Vector4(float vx, float vy, float vz, float vw) : x(vx), y(vy), z(vz), w(vw) {}
};

struct Matrix4 {
    float m[16];
    
    Matrix4() {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    
    static Matrix4 identity() {
        return Matrix4();
    }
    
    static Matrix4 perspective(float fov, float aspect, float near, float far) {
        Matrix4 result;
        memset(result.m, 0, sizeof(result.m));
        
        float tanHalfFov = std::tan(fov / 2.0f);
        result.m[0] = 1.0f / (aspect * tanHalfFov);
        result.m[5] = 1.0f / tanHalfFov;
        result.m[10] = -(far + near) / (far - near);
        result.m[11] = -1.0f;
        result.m[14] = -(2.0f * far * near) / (far - near);
        
        return result;
    }
    
    static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        Matrix4 result;
        
        Vector3 f = (center - eye).normalize();
        Vector3 s = f.cross(up).normalize();
        Vector3 u = s.cross(f);
        
        result.m[0] = s.x;
        result.m[1] = u.x;
        result.m[2] = -f.x;
        result.m[4] = s.y;
        result.m[5] = u.y;
        result.m[6] = -f.y;
        result.m[8] = s.z;
        result.m[9] = u.z;
        result.m[10] = -f.z;
        result.m[12] = -s.dot(eye);
        result.m[13] = -u.dot(eye);
        result.m[14] = f.dot(eye);
        
        return result;
    }
    
    static Matrix4 translate(const Vector3& t) {
        Matrix4 result;
        result.m[12] = t.x;
        result.m[13] = t.y;
        result.m[14] = t.z;
        return result;
    }
    
    static Matrix4 rotate(float angle, const Vector3& axis) {
        Matrix4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        Vector3 a = axis.normalize();
        
        result.m[0] = a.x*a.x*(1-c)+c;
        result.m[1] = a.y*a.x*(1-c)+a.z*s;
        result.m[2] = a.x*a.z*(1-c)-a.y*s;
        result.m[4] = a.x*a.y*(1-c)-a.z*s;
        result.m[5] = a.y*a.y*(1-c)+c;
        result.m[6] = a.y*a.z*(1-c)+a.x*s;
        result.m[8] = a.x*a.z*(1-c)+a.y*s;
        result.m[9] = a.y*a.z*(1-c)-a.x*s;
        result.m[10] = a.z*a.z*(1-c)+c;
        
        return result;
    }
    
    static Matrix4 scale(float s) {
        Matrix4 result;
        result.m[0] = s;
        result.m[5] = s;
        result.m[10] = s;
        return result;
    }
    
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i*4+j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i*4+j] += m[k*4+j] * other.m[i*4+k];
                }
            }
        }
        return result;
    }
};

struct Quaternion {
    float w, x, y, z;
    
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float qw, float qx, float qy, float qz) : w(qw), x(qx), y(qy), z(qz) {}
    
    static Quaternion fromAxisAngle(float angle, const Vector3& axis) {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        return Quaternion(
            std::cos(halfAngle),
            axis.x * s,
            axis.y * s,
            axis.z * s
        );
    }
    
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        );
    }
    
    Vector3 rotate(const Vector3& v) const {
        Vector3 qv(x, y, z);
        Vector3 uv = qv.cross(v);
        Vector3 uuv = qv.cross(uv);
        return v + ((uv * w) + uuv) * 2.0f;
    }
};

} // namespace Math3D

#endif // MATH3D_H
