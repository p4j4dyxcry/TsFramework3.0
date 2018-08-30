#include "pch.h"
#include "TsMath.h"

namespace TS
{
    const Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);

    DirectX::XMVECTOR Quaternion::ToXMVector()const
    {
        return DirectX::XMLoadFloat4(this);
    }

    Matrix4 Quaternion::ToMatrix()const
    {
        return Matrix4::CreateRotate(*this);
    }

    Vector3 Quaternion::ToEuler()const
    {
        Vector3 result;

        float& yAxis = result.y;    //  y axis
        float& zAxis = result.z;    //  z axis
        float& xAxis = result.x;    //  x axis

        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;

        float unit = sqx + sqy + sqz + sqw;
        float test = x * y + z * w;
        if (test > 0.499f * unit)
        {
            // singularity at north pole
            yAxis = 2.0f * static_cast<float>(atan2f(x, w));
            zAxis = static_cast<float>(TS_PI) / 2.0f;
            xAxis = 0.0f;
        }
        else if (test < -0.499*unit)
        {
            // singularity at south pole
            yAxis = -2.0f * static_cast<float>(atan2f(x, w));
            zAxis = -static_cast<float>(TS_PI) / 2.0f;
            xAxis = 0.0f;
        }
        else
        {
            yAxis = static_cast<float>(atan2f((2.0f * y * w - 2.0f * x * z),
                                              (sqx - sqy - sqz + sqw)));
            zAxis = static_cast<float>(asinf(2.0f * test / unit));
            xAxis = static_cast<float>(atan2f((2.0f * x * w - 2.0f * y * z),
                                              (-sqx + sqy - sqz + sqw)));
        }

        return TsDegree(result);
    }

    const Quaternion& Quaternion::Euler(float x, float y, float z, UnitofAngle unit_of_angle )
    {
        return *this = CreateByEuler(x, y, z, unit_of_angle);
    }
    const Quaternion& Quaternion::Euler(const Vector3& v, UnitofAngle unit_of_angle )
    {
        return *this = Euler(v.x, v.y, v.z, unit_of_angle);
    }

    const Quaternion& Quaternion::AngleAxis(const Vector3& axis, float angle, UnitofAngle unit_of_angle)
    {
        return *this = CreateByAngleAxis(axis, angle, unit_of_angle);
    }

    Quaternion& Quaternion::operator=(Matrix4& matrix)
    {
        return *this = matrix.ToQuaternion();
    }

    Quaternion  Quaternion::operator * (const Quaternion& quaternion)const
    {
        return Quaternion(DirectX::XMQuaternionMultiply(ToXMVector(), quaternion.ToXMVector()));
    }

    Quaternion& Quaternion::operator *=(const Quaternion& quaternion)
    {
        return *this = *this * quaternion;
    }

    Quaternion Quaternion::CreateByEuler(float x, float y, float z, UnitofAngle unit_of_angle)
    {
        if(unit_of_angle == Radian)
        {
            x = TsRadian(x);
            y = TsRadian(y);
            z = TsRadian(z);
        }

        return
            CreateByAngleAxis(Vector3::right, x) *
            CreateByAngleAxis(Vector3::up,    y) *
            CreateByAngleAxis(Vector3::front, z);
    }

    Quaternion Quaternion::CreateByEuler(const Vector3& Euler, UnitofAngle unit_of_angle)
    {
        float x = Euler.x;
        float y = Euler.y;
        float z = Euler.z;
        return CreateByEuler(x, y, z);
    }

    Quaternion Quaternion::CreateByAngleAxis(const Vector3& axis, float angle, UnitofAngle unit_of_angle)
    {
        if (unit_of_angle == Radian)
            angle = TsRadian(angle);

        return Quaternion(DirectX::XMQuaternionRotationMatrix(DirectX::XMMatrixRotationAxis(axis.ToXMVector(), angle)));
    }
}