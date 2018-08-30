#pragma once

namespace TS
{
    class Vector4 : public DirectX::XMFLOAT4
    {
    public:

        using oprator = DirectX::XMFLOAT4;

        Vector4(float _x = 0, float _y = 0, float _z = 0, float _w = 0) :DirectX::XMFLOAT4(_x, _y, _z, _w) {};
        Vector4(DirectX::XMFLOAT2 vector2) :DirectX::XMFLOAT4(vector2.x, vector2.y, 0, 0) {};
        Vector4(DirectX::XMFLOAT3 vector3) :DirectX::XMFLOAT4(vector3.x, vector3.y, vector3.z, 0) {};
        Vector4(DirectX::XMFLOAT4 vector4) :DirectX::XMFLOAT4(vector4.x, vector4.y, vector4.z, vector4.w) {};
        Vector4(DirectX::XMVECTOR vector) :DirectX::XMFLOAT4(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2], vector.m128_f32[3]) {};

        Vector4& operator= (const DirectX::XMVECTOR& Float4);
        Vector4& operator= (const DirectX::XMFLOAT3& Float3);

        Vector4  operator+ (const DirectX::XMFLOAT4& Float4)const;
        Vector4  operator- (const DirectX::XMFLOAT4& Float4)const;
        Vector4  operator* (float Float)const;
        Vector4  operator/ (float Float)const;

        Vector4& operator+= (const DirectX::XMFLOAT4& Float4);
        Vector4& operator-= (const DirectX::XMFLOAT4& Float4);
        Vector4& operator*= (float Float);
        Vector4& operator/= (float Float);

        Vector4  operator * (const Matrix4& mat);
        Vector4& operator *=(const Matrix4& mat);

        friend Vector4 operator * (float f, const class Vector4& v);
        Vector4  operator - ()const;

        const float& operator[](unsigned)const;
        float& operator[](unsigned);

        bool operator==(const Vector4& value)const;
        bool operator!=(const Vector4& value)const;
        bool operator>=(const Vector4& value)const;
        bool operator<=(const Vector4& value)const;
        bool operator>(const Vector4& value)const;
        bool operator<(const Vector4& value)const;

        DirectX::XMVECTOR  ToXMVector()const;
        float Length()const;
        Vector4& Normalize();
        Vector4  Normalized()const;

        float Sum()const;


        static float Dot(Vector4 v0,
            Vector4 v1);

        static Vector4 Closs(Vector4 v0,
            Vector4 v1);

        static const Vector4 zero;
        static const Vector4 one;
        static const Vector4 up;
        static const Vector4 down;
        static const Vector4 left;
        static const Vector4 right;
        static const Vector4 front;
        static const Vector4 back;

    protected:

    };
}