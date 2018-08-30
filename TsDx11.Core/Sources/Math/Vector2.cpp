#include "pch.h"
#include "TsMath.h"

namespace TS
{
    const Vector2 Vector2::zero = Vector2(0, 0);
    const Vector2 Vector2::one = Vector2(1, 1);
    const Vector2 Vector2::up = Vector2(0, 1);
    const Vector2 Vector2::down = Vector2(0, -1);
    const Vector2 Vector2::left = Vector2(-1, 0);
    const Vector2 Vector2::right = Vector2(1, 0);

    Vector2& Vector2::operator = (const DirectX::XMVECTOR& Float2)
    {
        this->x = Float2.m128_f32[0];
        this->y = Float2.m128_f32[1];
        return *this;
    }

    Vector2  Vector2::operator+ (const DirectX::XMFLOAT2& Float2)const
    {
        return Vector2(x + Float2.x, y + Float2.y);
    }
    Vector2  Vector2::operator- (const DirectX::XMFLOAT2& Float2)const
    {
        return Vector2(x - Float2.x, y - Float2.y);
    }
    Vector2  Vector2::operator* (float Float)const
    {
        return Vector2(x*Float, y*Float);
    }
    Vector2 operator * (float f, const class Vector2& v)
    {
        return v * f;
    }
    Vector2  Vector2::operator/ (float Float)const
    {
        return Vector2(x / Float, y / Float);
    }
    Vector2& Vector2::operator+= (const DirectX::XMFLOAT2& Float2)
    {
        return *this = *this + Float2;
    }
    Vector2& Vector2::operator-= (const DirectX::XMFLOAT2& Float2)
    {
        return *this = *this - Float2;
    }
    Vector2& Vector2::operator*= (float Float)
    {
        return *this = *this * Float;
    }
    Vector2& Vector2::operator/= (float Float)
    {
        return *this = *this / Float;
    }
    bool Vector2::operator==(const Vector2& value)const
    {
        return x == value.x &&
            y == value.y;
    }
    bool Vector2::operator!=(const Vector2& value)const
    {
        return !(*this == value);
    }
    bool Vector2::operator>=(const Vector2& value)const
    {
        return x >= value.x && y >= value.y;
    }
    bool Vector2::operator<=(const Vector2& value)const
    {
        return x <= value.x && y <= value.y;
    }
    bool Vector2::operator>(const Vector2& value)const
    {
        return x > value.x && y > value.y;
    }
    bool Vector2::operator<(const Vector2& value)const
    {
        return x < value.x && y < value.y;
    }

    Vector2  Vector2::operator - ()const
    {
        return *this * -1;
    }

    const float& Vector2::operator[](unsigned n)const
    {
        return reinterpret_cast<const float*>(this)[n];
    }
    float& Vector2::operator[](unsigned n)
    {
        return reinterpret_cast<float*>(this)[n];
    }

    DirectX::XMVECTOR Vector2::ToXMVector()const
    {
        return DirectX::XMLoadFloat2(this);
    }

    float Vector2::Length()const
    {
        DirectX::XMVECTOR vLength = DirectX::XMVector2Length(ToXMVector());
        return (vLength.m128_f32[0]);
    }

    float Vector2::LengthSq()const
    {
        DirectX::XMVECTOR vLength = DirectX::XMVector2LengthSq(ToXMVector());
        return (vLength.m128_f32[0]);
    }

    Vector2& Vector2::Normalize()
    {
        *this = Normalized();
        return *this;
    }
    Vector2 Vector2::Normalized()const
    {
        return Vector2(DirectX::XMVector2Normalize(ToXMVector()));
    }


    float Vector2::Dot(Vector2 v0,
        Vector2 v1)
    {
        return DirectX::XMVector2Dot(v0.ToXMVector(), v1.ToXMVector()).m128_f32[0];
    }

    Vector2 Vector2::Cross(Vector2 v0,
        Vector2 v1)
    {
        return Vector2(DirectX::XMVector2Cross(v0.ToXMVector(), v1.ToXMVector()));
    }

    float Vector2::Sum()const
    {
        return x + y;
    }
}