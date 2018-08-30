#include "pch.h"
#include "TsMath.h"

namespace TS
{
    const Vector3 Vector3::zero = Vector3(0, 0, 0);
    const Vector3 Vector3::one = Vector3(1, 1, 1);
    const Vector3 Vector3::up = Vector3(0, 1, 0);
    const Vector3 Vector3::down = Vector3(0, -1, 0);
    const Vector3 Vector3::left = Vector3(-1, 0, 0);
    const Vector3 Vector3::right = Vector3(1, 0, 0);
    const Vector3 Vector3::front = Vector3(0, 0, 1);
    const Vector3 Vector3::back = Vector3(0, 0, -1);

    Vector3& Vector3::operator = (const DirectX::XMVECTOR& Float3)
    {
        this->x = Float3.m128_f32[0];
        this->y = Float3.m128_f32[1];
        this->z = Float3.m128_f32[2];
        return *this;
    }

    Vector3  Vector3::operator+ (const DirectX::XMFLOAT3& Float3)const
    {
        return Vector3(x + Float3.x, y + Float3.y, z + Float3.z);
    }
    Vector3  Vector3::operator- (const DirectX::XMFLOAT3& Float3)const
    {
        return Vector3(x - Float3.x, y - Float3.y, z - Float3.z);
    }
    Vector3  Vector3::operator* (float Float)const
    {
        return Vector3(x * Float, y * Float, z * Float);
    }
    Vector3  Vector3::operator/ (float Float)const
    {
        return Vector3(x / Float, y / Float, z / Float);
    }

    Vector3 Vector3::operator/ (const DirectX::XMFLOAT3& v)const
    {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }

    Vector3 Vector3::operator* (const DirectX::XMFLOAT3& v)const
    {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3& Vector3::operator+= (const DirectX::XMFLOAT3& Float3)
    {
        return *this = *this + Float3;
    }
    Vector3& Vector3::operator-= (const DirectX::XMFLOAT3& Float3)
    {
        return *this = *this - Float3;
    }
    Vector3& Vector3::operator*= (float Float)
    {
        return *this = *this * Float;
    }
    Vector3& Vector3::operator/= (float Float)
    {
        return *this = *this / Float;
    }

    Vector3  Vector3::operator*(const Matrix4& mat)const
    {
        return Vector3(DirectX::XMVector3Transform(ToXMVector(), mat.ToXMMATRIX()));
    }

    Vector3 operator * (float f, const class Vector3& v)
    {
        return v * f;
    }

    Vector3& Vector3::operator*=(const Matrix4& mat)
    {
        return *this = *this * mat;
    }

    bool Vector3::operator==(const Vector3& value)const
    {
        return	abs(x - value.x) < 0.00001f  &&
            abs(y - value.y) < 0.00001f &&
            abs(z - value.z) < 0.00001f;
    }
    bool Vector3::operator!=(const Vector3& value)const
    {
        return !(*this == value);
    }
    bool Vector3::operator>=(const Vector3& value)const
    {
        return x >= value.x && y >= value.y && z >= value.z;
    }
    bool Vector3::operator<=(const Vector3& value)const
    {
        return x <= value.x && y <= value.y && z <= value.z;
    }
    bool Vector3::operator>(const Vector3& value)const
    {
        return x > value.x && y > value.y && z > value.z;
    }
    bool Vector3::operator<(const Vector3& value)const
    {
        return x < value.x && y < value.y && z >value.z;
    }

    Vector3  Vector3::operator - ()const
    {
        return *this * -1;
    }

    const float& Vector3::operator[](unsigned n)const
    {
        return reinterpret_cast<const float*>(this)[n];
    }
    float& Vector3::operator[](unsigned n)
    {
        return reinterpret_cast<float*>(this)[n];
    }
    Vector3& Vector3::TransformCood(const Matrix4& mat)
    {
        return 	*this = DirectX::XMVector3TransformCoord(ToXMVector(), mat.ToXMMATRIX());
    }
    DirectX::XMVECTOR Vector3::ToXMVector()const
    {
        return DirectX::XMLoadFloat3(this);
    }

    float Vector3::Length()const
    {
        DirectX::XMVECTOR vLength = DirectX::XMVector3Length(ToXMVector());
        return (vLength.m128_f32[0]);
    }

    float Vector3::LengthSq()const
    {
        DirectX::XMVECTOR vLength = DirectX::XMVector3LengthSq(ToXMVector());
        return (vLength.m128_f32[0]);
    }

    Vector3& Vector3::Normalize()
    {
        *this = Normalized();
        return *this;
    }
    Vector3 Vector3::Normalized()const
    {
        Vector3 v = Vector3(DirectX::XMVector3Normalize(ToXMVector()));
        v.x = abs(v.x) < 0.000001f ? 0 : v.x;
        v.y = abs(v.y) < 0.000001f ? 0 : v.y;
        v.z = abs(v.z) < 0.000001f ? 0 : v.z;
        return v;
    }

    float Vector3::Dot(Vector3 v0,
        Vector3 v1)
    {
        return DirectX::XMVector3Dot(v0.ToXMVector(), v1.ToXMVector()).m128_f32[0];
    }

    Vector3 Vector3::Cross(Vector3 v0,
        Vector3 v1)
    {
        return Vector3(DirectX::XMVector3Cross(v0.ToXMVector(), v1.ToXMVector()));
    }

    float Vector3::CrossingAngle(Vector3 v0, Vector3 v1)
    {
        float d = v0.Length() * v1.Length();

        if (d == 0)return 0;

        float c = Dot(v0, v1) / d;

        if (c >= 1.0) return 0;
        if (c <= -1.0) return TS_PI;

        return acos(c);
    }

    float Vector3::Sum()const
    {
        return x + y + z;
    }
}