//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#type vertex
#pragma pack_matrix(row_major)
cbuffer Camera : register(b0) { matrix u_ViewProjection; }

struct vsIn
{
    float3 a_Position : SKYBOX_POS;
};

struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_TexCoords : TEX_COORDS;
};

vsOut main(vsIn input)
{
    vsOut output;
    float4 pos = mul(float4(input.a_Position, 1.0f), u_ViewProjection);
    output.v_Position = pos.xyww;
    output.v_TexCoords = input.a_Position;
    return output;
}

#type pixel
struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_TexCoords : TEX_COORDS;
};

TextureCube SkyboxCubemap;
SamplerState sampleType : register(s1);

float4 main(vsOut input) : SV_TARGET
{
    float4 PixelColor;
    PixelColor = SkyboxCubemap.Sample(sampleType, input.v_TexCoords);
    return PixelColor;
}