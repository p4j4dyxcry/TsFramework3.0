#pragma once
namespace TS
{
    class Matrix4 : public DirectX::XMFLOAT4X4
    {
    public:
        using oprator = DirectX::XMFLOAT4X4;
        Matrix4(DirectX::XMFLOAT4X4 float4x4) :DirectX::XMFLOAT4X4(float4x4) {};
        Matrix4(float, float, float, float,
                float, float, float, float,
                float, float, float, float,
                float, float, float, float);
        Matrix4() { *this = Matrix4::identity; };
        Matrix4(DirectX::XMMATRIX matrix) { *this = matrix; }
        Matrix4(Vector3& pos, Quaternion& rotate);
        Matrix4(Vector3& pos, Quaternion& rotate, Vector3& scale);

        Matrix4& operator = (const DirectX::XMMATRIX& matrix);
        Matrix4  operator *  (const Matrix4& matrix)const;
        Matrix4& operator *= (const Matrix4& matrix);
        DirectX::XMMATRIX  ToXMMATRIX()const;
        Quaternion ToQuaternion()const;
        Matrix4& Transpose();
        Matrix4& Inverse();

        Matrix4 Transposed()const;
        Matrix4 Inversed();

        Vector3 TransformCoord(const Vector3&)const;
        Vector3 TransformVector(const Vector3&)const;

        Vector3 TransformPoint(const Vector3&)const;

        Vector3 TransformNormal(const Vector3&)const;

        Vector4 TransformVector(const Vector4&)const;

        bool operator == (const Matrix4&);
        bool operator != (const Matrix4&);

        Vector3 GetVec3Row(unsigned collum)const;

        Vector3 GetVec3Collum(unsigned row)const;

        Vector3 GetVec4Row(unsigned collum)const;
        Vector3 GetVec4Collum(unsigned row)const;

        void SetVec3Row(unsigned collum, const Vector3&);
        void SetVec3Collum(unsigned row, const Vector3&);

        void SetVec4Row(unsigned collum, const Vector4&);
        void SetVec4Collum(unsigned row, const Vector4&);

        void SetVec3Row(unsigned collum, float, float, float);
        void SetVec3Collum(unsigned row, float, float, float);

        void SetVec4Row(unsigned collum, float, float, float, float);
        void SetVec4Collum(unsigned row, float, float, float, float);

        static Matrix4 CreateTranslate(float x, float y, float z);
        static Matrix4 CreateTranslate(const Vector3& translate);
        static Matrix4 CreateRotate(const Quaternion& q);
        static Matrix4 CreateScale(float x, float y, float z);
        static Matrix4 CreateScale(const Vector3& scale);
        static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& at, const Vector3& up);
        static Matrix4 CreateOrtho(float width, float height, float _near, float _far)
        {
            return Matrix4(DirectX::XMMatrixOrthographicLH(width, height, _near, _far));
        }

        static const Matrix4 identity;

    protected:
    };
}
