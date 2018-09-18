#include "pch.h"
#include "TsMath.h"

namespace TS
{
    const Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
    const Vector4 Vector4::one = Vector4(1, 1, 1, 1);
    const Vector4 Vector4::up = Vector4(0, 1, 0, 0);
    const Vector4 Vector4::down = Vector4(0, -1, 0, 0);
    const Vector4 Vector4::left = Vector4(-1, 0, 0, 0);
    const Vector4 Vector4::right = Vector4(1, 0, 0, 0);
    const Vector4 Vector4::front = Vector4(0, 0, 1, 0);
    const Vector4 Vector4::back = Vector4(0, 0, -1, 0);

    Vector4& Vector4::operator = (const DirectX::XMVECTOR& Float4)
    {
        this->x = Float4.m128_f32[0];
        this->y = Float4.m128_f32[1];
        this->z = Float4.m128_f32[2];
        this->w = Float4.m128_f32[3];
        return *this;
    }

    Vector4& Vector4::operator= (const DirectX::XMFLOAT3& Float3)
    {
        this->x = Float3.x;
        this->y = Float3.y;
        this->z = Float3.z;
        this->w = 0;
        return *this;
    }

    Vector4  Vector4::operator+ (const DirectX::XMFLOAT4& Float4)const
    {
        return Vector4(x + Float4.x, y + Float4.y, z + Float4.z, w + Float4.w);
    }
    Vector4  Vector4::operator- (const DirectX::XMFLOAT4& Float4)const
    {
        return Vector4(x - Float4.x, y - Float4.y, z - Float4.z, w - Float4.w);
    }
    Vector4  Vector4::operator* (float Float)const
    {
        return Vector4(x * Float, y * Float, z * Float, w * Float);
    }
    Vector4  Vector4::operator/ (float Float)const
    {
        return Vector4(x / Float, y / Float, z / Float, w / Float);
    }

    Vector4& Vector4::operator+= (const DirectX::XMFLOAT4& Float4)
    {
        x += Float4.x;
        y += Float4.y;
        z += Float4.z;
        w += Float4.w;
        return *this;
    }
    Vector4& Vector4::operator-= (const DirectX::XMFLOAT4& Float4)
    {
        x -= Float4.x;
        y -= Float4.y;
        z -= Float4.z;
        w -= Float4.w;
        return *this;
    }
    Vector4& Vector4::operator*= (float Float)
    {
        x *= Float;
        y *= Float;
        z *= Float;
        w *= Float;
        return *this;
    }
    Vector4& Vector4::operator/= (float Float)
    {
        x /= Float;
        y /= Float;
        z /= Float;
        w /= Float;
        return *this;
    }

    Vector4  Vector4::operator*(const Matrix4& mat) const
    {
        return Vector4(XMVector4Transform(ToXMVector(), mat.ToXMMATRIX()));
    }

    Vector4 operator * (float f, const class Vector4& v)
    {
        return v * f;
    }

    Vector4& Vector4::operator*=(const Matrix4& mat)
    {
        return *this = *this * mat;
    }
    bool Vector4::operator==(const Vector4& value)const
    {
        return	x == value.x &&
            y == value.y &&
            z == value.z &&
            w == value.w;
    }
    bool Vector4::operator!=(const Vector4& value)const
    {
        return !(*this == value);
    }
    bool Vector4::operator>=(const Vector4& value)const
    {
        return x >= value.x && y >= value.y && z >= value.z;
    }
    bool Vector4::operator<=(const Vector4& value)const
    {
        return x <= value.x && y <= value.y && z <= value.z;
    }
    bool Vector4::operator>(const Vector4& value)const
    {
        return x > value.x && y > value.y && z > value.z;
    }
    bool Vector4::operator<(const Vector4& value)const
    {
        return x < value.x && y < value.y && z >value.z;
    }

    Vector4  Vector4::operator - ()const
    {
        return *this * -1;
    }

    const float& Vector4::operator[](unsigned n)const
    {
        return reinterpret_cast<const float*>(this)[n];
    }
    float& Vector4::operator[](unsigned n)
    {
        return reinterpret_cast<float*>(this)[n];
    }

    DirectX::XMVECTOR Vector4::ToXMVector()const
    {
        return DirectX::XMLoadFloat4(this);
    }

    float Vector4::Length()const
    {
        DirectX::XMVECTOR vLength = DirectX::XMVector2Length(ToXMVector());
        return (vLength.m128_f32[0]);
    }

    Vector4& Vector4::Normalize()
    {
        *this = Normalized();
        return *this;
    }
    Vector4 Vector4::Normalized()const
    {
        return Vector4(DirectX::XMVector4Normalize(ToXMVector()));
    }

    float Vector4::Dot(Vector4 v0,
        Vector4 v1)
    {
        return DirectX::XMVector4Dot(v0.ToXMVector(), v1.ToXMVector()).m128_f32[0];
    }

    Vector4 Vector4::Closs(Vector4 v0,
        Vector4 v1)
    {
        return Vector3(DirectX::XMVector3Cross(v0.ToXMVector(), v1.ToXMVector()));
    }

    float Vector4::Sum()const
    {
        return x + y + z + w;
    }
}