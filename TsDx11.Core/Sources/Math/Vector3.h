#pragma once

namespace TS
{
    class Matrix4;

    class Vector3 : public DirectX::XMFLOAT3
    {
    public:

        using oprator = DirectX::XMFLOAT3;

        Vector3(float _x = 0, float _y = 0, float _z = 0) :DirectX::XMFLOAT3(_x, _y, _z) {};
        Vector3(DirectX::XMFLOAT2 vector2) :DirectX::XMFLOAT3(vector2.x, vector2.y, 0) {};
        Vector3(DirectX::XMFLOAT3 vector3) :DirectX::XMFLOAT3(vector3.x, vector3.y, vector3.z) {};
        Vector3(DirectX::XMFLOAT4 vector4) :DirectX::XMFLOAT3(vector4.x, vector4.y, vector4.z) {};
        Vector3(DirectX::XMVECTOR vector) :DirectX::XMFLOAT3(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]) {};

        Vector3& operator= (const DirectX::XMVECTOR& Float3);

        Vector3  operator+ (const DirectX::XMFLOAT3& Float3)const;
        Vector3  operator- (const DirectX::XMFLOAT3& Float3)const;
        Vector3  operator* (const DirectX::XMFLOAT3&)const;
        Vector3  operator/ (const DirectX::XMFLOAT3&)const;
        Vector3  operator* (float Float)const;
        Vector3  operator/ (float Float)const;

        Vector3  operator- ()const;

        Vector3& operator+= (const DirectX::XMFLOAT3& Float3);
        Vector3& operator-= (const DirectX::XMFLOAT3& Float3);
        Vector3& operator*= (float Float);
        Vector3& operator/= (float Float);

        Vector3  operator*(const Matrix4& mat)const;
        Vector3& operator*=(const Matrix4& mat);

        friend Vector3 operator * (float f, const class Vector3& v);

        bool operator==(const Vector3& value)const;
        bool operator!=(const Vector3& value)const;
        bool operator>=(const Vector3& value)const;
        bool operator<=(const Vector3& value)const;
        bool operator>(const Vector3& value)const;
        bool operator<(const Vector3& value)const;

        const float& operator[](unsigned)const;
        float& operator[](unsigned);

        Vector3& TransformCood(const Matrix4& mat);
        DirectX::XMVECTOR  ToXMVector()const;
        float Length()const;
        float LengthSq()const;
        Vector3& Normalize();
        Vector3  Normalized()const;

        float Sum()const;

        static float Dot(Vector3 v0, Vector3 v1);

        static Vector3 Cross(Vector3 v0, Vector3 v1);

        static float CrossingAngle(Vector3 v0, Vector3 v1);

        static const Vector3 zero;
        static const Vector3 one;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 left;
        static const Vector3 right;
        static const Vector3 front;
        static const Vector3 back;

    protected:

    };
}