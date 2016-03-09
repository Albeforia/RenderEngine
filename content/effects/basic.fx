cbuffer CBufferPerObject {
	float4x4 WVP : WORLDVIEWPROJECTION;
};

RasterizerState DisableCulling {
	CullMode = NONE;
};

struct VS_In {
	float4 objPos: POSITION;
	float4 color: COLOR;
};

struct VS_Out {
	float4 position: SV_Position;
	float4 color: COLOR;
};

VS_Out vertex_shader(VS_In input) {
	VS_Out output = (VS_Out)0;
	output.position = mul(input.objPos, WVP);
	output.color = input.color;
	return output;
}

float4 pixel_shader(VS_Out input) : SV_Target {
	return input.color;
}

technique11 main11 {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
		SetRasterizerState(DisableCulling);
	}
}