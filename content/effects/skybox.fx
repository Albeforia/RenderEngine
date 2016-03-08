cbuffer CBufferPerObject {
	float4x4 WVP: WORLDVIEWPROJECTION;
};

TextureCube SkyboxTexture;

SamplerState TrilinearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState DisableCulling {
	CullMode = NONE;
};

struct VS_In {
	float4 objPos: POSITION;
};

struct VS_Out {
	float4 position: SV_Position;
	float3 texCoord: TEXCOORD0;
};

VS_Out vertex_shader(VS_In input) {
	VS_Out output = (VS_Out)0;
	output.position = mul(input.objPos, WVP);
	output.texCoord = input.objPos.xyz;
	return output;
}

float4 pixel_shader(VS_Out input) : SV_Target {
	return SkyboxTexture.Sample(TrilinearSampler, input.texCoord);
}

technique11 main11 {
    pass p0 {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
        SetRasterizerState(DisableCulling);
    }
}