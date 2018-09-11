//! hlsl utility


#define PI                    (3.14159265358979323846f)
#define PackUnsigned(value)   (value * 0.5f + 0.5f)
#define UnPackUnsigned(value) ((value - 0.5f) * 2.0f)
#define EaseIn(t)             (t*t)
#define EaseOut(t)            (t*(2 - t))
#define Qubic(t)              (t * t * (3.0f - 2.0f + t))
#define HexLerp(t)            (t * t * t * (t * (6 * t - 15) + 10))
                              
#define ToLinear( gamma )     (pow(gamma, 1.0f / 2.2f))
#define ToGamma( lin )        (pow(lin, 2.2f))

#define Tone(color, exposure) (1.0 - exp(color * exposure))

//! 輝度抽出
float Luminance(float3 color)
{
    return dot(color, float3(0.298912f, 0.586611f, 0.114478f));
}

//! 4点サンプリング
float4 Sample4(Texture2D tex,SamplerState samp,float2 texcoord)
{
    float2 size = 1.0f / TexSize(tex);
    float x = size.x;
    float y = size.y;

    float4 result = 0;

    result += tex.Sample(samp, texcoord + float2(x, 0));
    result += tex.Sample(samp, texcoord + float2(-x, 0));
    result += tex.Sample(samp, texcoord + float2(0, y));
    result += tex.Sample(samp, texcoord + float2(0, -y));

    const float invTotal = 1.0f / 5.0f;

    return result * invTotal;
}

//! 8点サンプリング
float4 Sample8(Texture2D tex,SamplerState samp,float2 texcoord)
{
    float2 size = 1.0f / TexSize(tex);
    float x = size.x;
    float y = size.y;
    float4 result = 0;
    result += tex.Sample(samp, texcoord);

    result += tex.Sample(samp, texcoord + float2(x, 0));
    result += tex.Sample(samp, texcoord + float2(-x, 0));
    result += tex.Sample(samp, texcoord + float2(0, y));
    result += tex.Sample(samp, texcoord + float2(0, -y));

    result += tex.Sample(samp, texcoord + float2(x, y));
    result += tex.Sample(samp, texcoord + float2(-x, y));
    result += tex.Sample(samp, texcoord + float2(x, y));
    result += tex.Sample(samp, texcoord + float2(-x, -y));

    const float invTotal = 1.0f / 9.0f;

    return result * invTotal;
}

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}