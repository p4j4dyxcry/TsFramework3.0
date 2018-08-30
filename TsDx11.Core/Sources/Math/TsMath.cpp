#include "pch.h"
#include "TsMath.h"

namespace TS
{
    TS::Transform TS::MatrixToTransform(const Matrix4& m)
    {
        //軸データ
        Vector3 xA(m.m[0][0], m.m[0][1], m.m[0][2]);
        Vector3 yA(m.m[1][0], m.m[1][1], m.m[1][2]);
        Vector3 zA(m.m[2][0], m.m[2][1], m.m[2][2]);

        //スケールデータ
        float xScale = xA.Length();
        float yScale = yA.Length();
        float zScale = zA.Length();

        //位置データ
        const Vector3 pos = Vector3(m.m[3][0], m.m[3][1], m.m[3][2]);

        //回転データの計算は拡縮成分・平行移動成分を除いた回転行列に落としてからクォータニオン化する。
        Matrix4 rotate;

        //軸をスケールで割ることで回転行列の各成分を求める。
        rotate.m[0][0] = xA.x / xScale;
        rotate.m[1][0] = xA.y / xScale;
        rotate.m[2][0] = xA.z / xScale;

        rotate.m[0][1] = yA.x / yScale;
        rotate.m[1][1] = yA.y / yScale;
        rotate.m[2][1] = yA.z / yScale;

        rotate.m[0][2] = zA.x / zScale;
        rotate.m[1][2] = zA.y / zScale;
        rotate.m[2][2] = zA.z / zScale;

        Transform transform;
        //値の設定（現状は全てローカル値と解釈することにする)
        transform.Scale = Vector3(xScale, yScale, zScale);
        transform.Position = pos;
        transform.Rotation = rotate.ToQuaternion();
        return transform;
    }
}


