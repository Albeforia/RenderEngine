cbuffer CBufferPerLight {
	float4x4 WVP: WORLDVIEWPROJECTION;
};

RasterizerState DisableCulling {
	CullMode = NONE;
};

DepthStencilState LightVolumeCulling {
	// RO depth buffer
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;

	StencilEnable = TRUE;
	// always pass stencil test
	FrontFaceStencilFunc = ALWAYS;
	BackFaceStencilFunc = ALWAYS;
	// -1/+1 when failed depth test
	FrontFaceStencilDepthFail = DECR;
	BackFaceStencilDepthFail = INCR;
	// keep when passed depth test
	FrontFaceStencilPass = KEEP;
	BackFaceStencilPass = KEEP;
};

float4 vertex_shader(float4 o_position : POSITION) : SV_Position {
	return mul(o_position, WVP);
}

technique11 point_light_pass {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
		SetRasterizerState(DisableCulling);
		SetDepthStencilState(LightVolumeCulling, 0xFFFFFFFF);
	}
}