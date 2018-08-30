#include "pch.h"
#include "TsMath.h"

namespace TS
{
    const Matrix4 Matrix4::identity = DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4::Matrix4(float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44)
    {
        *this = DirectX::XMFLOAT4X4(m11, m12, m13, m14,
            m21, m22, m23, m24,
            m31, m32, m33, m34,
            m41, m42, m43, m44);
    }

    Matrix4::Matrix4(Vector3& p, Quaternion& q)
    {
        *this = CreateRotate(q) * CreateTranslate(p);
    }

    Matrix4::Matrix4(Vector3& p, Quaternion& q, Vector3& scale)
    {
        *this = CreateScale(scale) * CreateRotate(q) * CreateTranslate(p);
    }

    Matrix4&  Matrix4::operator =(const DirectX::XMMATRIX& i)
    {
        memcpy(m, &i.r[0], sizeof(float) * 16);
        return *this;
    }

    Matrix4  Matrix4::operator *(const Matrix4& matrix)const
    {
        return Matrix4(ToXMMATRIX() * matrix.ToXMMATRIX());
    }

    Matrix4& Matrix4::operator*= (const Matrix4& matrix)
    {
        return (*this = *this * matrix);
    }

    DirectX::XMMATRIX  Matrix4::ToXMMATRIX()const
    {
        return DirectX::XMLoadFloat4x4(this);
    }

    Quaternion Matrix4::ToQuaternion()const
    {
        float elem[4]; // 0:x, 1:y, 2:z, 3:w
        elem[0] = _11 - _22 - _33 + 1.0f;
        elem[1] = -_11 + _22 - _33 + 1.0f;
        elem[2] = -_11 - _22 + _33 + 1.0f;
        elem[3] = _11 + _22 + _33 + 1.0f;

        unsigned biggestIndex = 0;
        for (int i = 1; i < 4; i++) {
            if (elem[i] > elem[biggestIndex])
                biggestIndex = i;
        }

        if (elem[biggestIndex] < 0.0f)
            return Quaternion(); 
                                
        Quaternion q;
        float v = sqrtf(elem[biggestIndex]) * 0.5f;

        float mult = 0.25f / v;
        switch (biggestIndex) {
        case 0: // x
            q.x = v;
            q.y = (_12 + _21) * mult;
            q.z = (_31 + _13) * mult;
            q.w = (_32 - _23) * mult;
            break;
        case 1: // y
            q.x = (_12 + _21) * mult;
            q.y = v;
            q.z = (_23 + _32) * mult;
            q.w = (_13 - _31) * mult;
            break;
        case 2: // z
            q.x = (_31 + _13) * mult;
            q.y = (_23 + _32) * mult;
            q.z = v;
            q.w = (_21 - _12) * mult;
            break;
        case 3: // w
            q.x = (_32 - _23) * mult;
            q.y = (_13 - _31) * mult;
            q.z = (_21 - _12) * mult;
            q.w = v;
            break;
        }
        return q;
    }

    Matrix4& Matrix4::Transpose()
    {
        return *this = Transposed();
    }

    Matrix4& Matrix4::Inverse()
    {
        return *this = Inversed();
    }

    Matrix4 Matrix4::Transposed()const
    {
        return DirectX::XMMatrixTranspose(ToXMMATRIX());
    }

    Matrix4 Matrix4::Inversed()
    {
        return DirectX::XMMatrixInverse(nullptr, ToXMMATRIX());
    }

    Matrix4 Matrix4::CreateTranslate(float x, float y, float z)
    {
        return Matrix4(DirectX::XMMatrixTranslation(x, y, z));
    }

    Matrix4 Matrix4::CreateTranslate(const Vector3& offset)
    {
        return Matrix4::CreateTranslate(offset.x, offset.y, offset.z);
    }

    Matrix4 Matrix4::CreateRotate(const Quaternion& q)
    {
        return Matrix4(DirectX::XMMatrixRotationQuaternion(q.ToXMVector()));
    }

    Matrix4 Matrix4::CreateScale(float x, float y, float z)
    {
        return Matrix4(DirectX::XMMatrixScaling(x, y, z));
    }

    Matrix4 Matrix4::CreateScale(const Vector3& scale)
    {
        return Matrix4::CreateScale(scale.x, scale.y, scale.z);
    }

    Matrix4 Matrix4::CreateLookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        return Matrix4(DirectX::XMMatrixLookAtLH(eye.ToXMVector(), at.ToXMVector(), up.ToXMVector()));
    }

    Vector3 Matrix4::TransformCoord(const Vector3& v)const
    {
        return DirectX::XMVector3TransformCoord(v.ToXMVector(), ToXMMATRIX());
    }
    Vector3 Matrix4::TransformVector(const Vector3& v)const
    {
        return Vector3(XMVector3Transform(v.ToXMVector(), ToXMMATRIX()));
    }
    Vector3 Matrix4::TransformPoint(const Vector3& v)const
    {
        Vector3 result;
        result.x = v.x * m[0][0] +
            v.y * m[1][0] +
            v.z * m[2][0] +
            m[3][0];

        result.y = v.x * m[0][1] +
            v.y * m[1][1] +
            v.z * m[2][1] +
            m[3][1];

        result.z = v.x * m[0][2] +
            v.y * m[1][2] +
            v.z * m[2][2] +
            m[3][2];

        return result;
    }

    Vector3 Matrix4::TransformNormal(const Vector3& vNormal)const
    {
        Matrix4 m = *this;
        m.SetVec4Row(3, Vector4(0, 0, 0, 1));

        return m.TransformVector(vNormal);
    }

    Vector4 Matrix4::TransformVector(const Vector4& v)const
    {
        return Vector4(XMVector4Transform(v.ToXMVector(), ToXMMATRIX()));;
    }

    bool Matrix4::operator==(const Matrix4& matrix)
    {
        return  m[0][0] == matrix.m[0][0] &&
            m[0][1] == matrix.m[0][1] &&
            m[0][2] == matrix.m[0][2] &&
            m[0][3] == matrix.m[0][3] &&
            m[1][0] == matrix.m[1][0] &&
            m[1][1] == matrix.m[1][1] &&
            m[1][2] == matrix.m[1][2] &&
            m[1][3] == matrix.m[1][3] &&
            m[2][0] == matrix.m[2][0] &&
            m[2][1] == matrix.m[2][1] &&
            m[2][2] == matrix.m[2][2] &&
            m[2][3] == matrix.m[2][3] &&
            m[3][0] == matrix.m[3][0] &&
            m[3][1] == matrix.m[3][1] &&
            m[3][2] == matrix.m[3][2] &&
            m[3][3] == matrix.m[3][3];
    }

    bool Matrix4::operator!=(const Matrix4& matrix)
    {
        return !(*this == matrix);
    }

    Vector3 Matrix4::GetVec3Row(unsigned collum)const
    {
        return Vector3(m[collum][0], m[collum][1], m[collum][2]);
    }
    Vector3 Matrix4::GetVec3Collum(unsigned row)const
    {
        return Vector3(m[0][row], m[1][row], m[2][row]);
    }


    Vector3 Matrix4::GetVec4Row(unsigned collum)const
    {
        return Vector4(m[collum][0],
            m[collum][1],
            m[collum][2],
            m[collum][3]);
    }
    Vector3 Matrix4::GetVec4Collum(unsigned row)const
    {
        return Vector4(m[0][row],
            m[1][row],
            m[2][row],
            m[3][row]);
    }

    void Matrix4::SetVec3Row(unsigned collum, const Vector3& row)
    {
        for (unsigned i = 0; i < 3; ++i)
            m[collum][i] = row[i];
    }
    void Matrix4::SetVec3Collum(unsigned row, const Vector3& collum)
    {
        for (unsigned i = 0; i < 3; ++i)
            m[i][row] = collum[i];
    }

    void Matrix4::SetVec4Row(unsigned collum, const Vector4& row)
    {
        for (unsigned i = 0; i < 4; ++i)
            m[collum][i] = row[i];
    }
    void Matrix4::SetVec4Collum(unsigned row, const Vector4& collum)
    {
        for (unsigned i = 0; i < 4; ++i)
            m[i][row] = collum[i];
    }

    void Matrix4::SetVec3Row(unsigned collum, float f0, float f1, float f2)
    {
        m[collum][0] = f0;
        m[collum][1] = f1;
        m[collum][2] = f2;
    }
    void Matrix4::SetVec3Collum(unsigned row, float f0, float f1, float f2)
    {
        m[0][row] = f0;
        m[1][row] = f1;
        m[2][row] = f2;
    }

    void Matrix4::SetVec4Row(unsigned collum, float f0, float f1, float f2, float f3)
    {
        m[collum][0] = f0;
        m[collum][1] = f1;
        m[collum][2] = f2;
        m[collum][3] = f3;
    }
    void Matrix4::SetVec4Collum(unsigned row, float f0, float f1, float f2, float f3)
    {
        m[0][row] = f0;
        m[1][row] = f1;
        m[2][row] = f2;
        m[3][row] = f3;
    }
}