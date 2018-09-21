#pragma once

namespace TS
{
    #define TS_PI (3.14159265258979f)
    class Vector2;
    class Vector3;
    class Vector4;
    class Matrix4;
    class Quaternion;

    template<typename T>
    inline T TsDegree(const T& radian)
    {
        return (radian * 180.0f / TS_PI);
    }

    template<typename T>
    inline T TsRadian(const T& degree)
    {
        return (degree * TS_PI / 180.0f);
    }

    enum UnitofAngle
    {
        Degree,
        Radian
    }; 
}

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace TS
{
    struct Transform
    {
        Vector3 Position;
        Quaternion Rotation;
        Vector3 Scale;
    };

    Transform MatrixToTransform(const Matrix4& m);
}
