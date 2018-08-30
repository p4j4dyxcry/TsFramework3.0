#pragma once

namespace TS
{
    class Vector2 : public DirectX::XMFLOAT2
    {
    public:

        using oprator = DirectX::XMFLOAT2;

        Vector2(float _x = 0, float _y = 0) :DirectX::XMFLOAT2(_x, _y) {};
        Vector2(DirectX::XMFLOAT2 vector2) :DirectX::XMFLOAT2(vector2) {};
        Vector2(DirectX::XMVECTOR vector) :DirectX::XMFLOAT2(vector.m128_f32[0], vector.m128_f32[1]) {};

        Vector2& operator= (const DirectX::XMVECTOR& Float2);

        Vector2  operator+ (const DirectX::XMFLOAT2& Float2)const;
        Vector2  operator- (const DirectX::XMFLOAT2& Float2)const;
        Vector2  operator* (float Float)const;
        Vector2  operator/ (float Float)const;

        Vector2& operator+= (const DirectX::XMFLOAT2& Float2);
        Vector2& operator-= (const XMFLOAT2& Float2);
        Vector2& operator*= (float Float);
        Vector2& operator/= (float Float);

        bool operator==(const Vector2& value)const;
        bool operator!=(const Vector2& value)const;
        bool operator>=(const Vector2& value)const;
        bool operator<=(const Vector2& value)const;
        bool operator>(const Vector2& value)const;
        bool operator<(const Vector2& value)const;

        const float& operator[](unsigned)const;
        float& operator[](unsigned);

        friend Vector2 operator * (float f, const class Vector2& v);
        Vector2  operator- ()const;

        DirectX::XMVECTOR  ToXMVector()const;
        float Length()const;
        float LengthSq()const;
        Vector2& Normalize();
        Vector2  Normalized()const;

        float Sum()const;

        static float Dot(Vector2 v0, Vector2 v1);

        static Vector2 Cross(Vector2 v0, Vector2 v1);

        static const Vector2 zero;
        static const Vector2 one;
        static const Vector2 up;
        static const Vector2 down;
        static const Vector2 left;
        static const Vector2 right;

    protected:

    };
}