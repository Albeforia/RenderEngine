cbuffer CBufferPerFrame {
	float4x4 VP: VIEWPROJECTION;
};

cbuffer CBufferPerObject {
	//float4x4 WVP: WORLDVIEWPROJECTION;
	float4x4 World: WORLD;
	float SpecularPower : SPECULARPOWER; // material info
};

Texture2D DiffuseTexture;
Texture2D SpecularTexture;

SamplerState TrilinearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_In {
	float4 o_position: POSITION;
	float3 o_normal: NORMAL;
	float2 texture_coords: TEXCOORD;
};

struct VS_In_Inst {
	float4 o_position: POSITION;
	float3 o_normal: NORMAL;
	float2 texture_coords: TEXCOORD;
	row_major float4x4 World: INST_WORLD;
};

struct VS_Out {
	float4 h_position: SV_Position;
	float4 w_position: TEXCOORD1;
	float3 w_normal: NORMAL;
	float2 texture_coords: TEXCOORD0;
};

VS_Out vertex_shader(VS_In input) {
	VS_Out output = (VS_Out)0;
	output.w_position = mul(input.o_position, World);
	output.h_position = mul(output.w_position, VP);
	output.w_normal = normalize(mul(float4(input.o_normal, 0), World).xyz);
	output.texture_coords = input.texture_coords;
	return output;
}

VS_Out vertex_shader_inst(VS_In_Inst input) {
	VS_Out output = (VS_Out)0;
	output.w_position = mul(input.o_position, input.World);
	output.h_position = mul(output.w_position, VP);
	output.w_normal = normalize(mul(float4(input.o_normal, 0), input.World).xyz);
	output.texture_coords = input.texture_coords;
	return output;
}

struct PS_Out {
	float4 position: SV_Target0;
	float4 normal: SV_Target1;
	float4 albedo_specular: SV_Target2;
};

PS_Out pixel_shader(VS_Out input) {
	PS_Out output = (PS_Out)0;
	output.position = input.w_position;
	output.normal.xyz = normalize(input.w_normal);
	output.normal.w = SpecularPower; // store specular power
	float4 texel = DiffuseTexture.Sample(TrilinearSampler, input.texture_coords);
	texel.a = SpecularTexture.Sample(TrilinearSampler, input.texture_coords).r;
	output.albedo_specular = texel;
	return output;
}

technique11 geometry_pass {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
	}
}

technique11 geometry_pass_inst {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader_inst()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
	}
}