#pragma once

namespace TS
{
    class Quaternion : public DirectX::XMFLOAT4
    {
    public:

        using oprator = DirectX::XMFLOAT4;

        //=======================================================
        //! Constructor
        //=======================================================
        Quaternion(float _x = 0, float _y = 0, float _z = 0, float _w = 1) :DirectX::XMFLOAT4(_x, _y, _z, _w) {};
        Quaternion(DirectX::XMFLOAT4 vector4) : DirectX::XMFLOAT4(vector4.x, vector4.y, vector4.z, vector4.w) {};
        Quaternion(DirectX::XMVECTOR vector) : DirectX::XMFLOAT4(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2], vector.m128_f32[3]) {};

        //=======================================================
        //! public method
        //=======================================================
        DirectX::XMVECTOR  ToXMVector()const;    // simd�ɕϊ�
        Matrix4  ToMatrix()const;      // Rotate Matrix�ɕϊ�
        Vector3 ToEuler()const;       // Euler�p�ɕϊ�

        const Quaternion& Euler(float, float, float, UnitofAngle unit_of_angle = Degree);
        const Quaternion& Euler(const Vector3&, UnitofAngle unit_of_angle = Degree);
        const Quaternion& AngleAxis(const Vector3& axis, float angle, UnitofAngle unit_of_angle = Degree);

        //=======================================================
        //! operator
        //=======================================================
        Quaternion& operator = (Matrix4& matrix);
        Quaternion  operator * (const Quaternion& quaternion)const;
        Quaternion& operator *=(const Quaternion& quaternion);

        //=======================================================
        //! static method
        //=======================================================
        static Quaternion CreateByEuler(const Vector3& Euler , UnitofAngle unit_of_angle = Degree);
        static Quaternion CreateByEuler(float x, float y, float z, UnitofAngle unit_of_angle = Degree);
        static Quaternion CreateByAngleAxis(const Vector3& axis, float angle, UnitofAngle unit_of_angle = Degree);

        //=======================================================
        //! static member
        //=======================================================
        static const Quaternion identity;
    protected:

    };
}