cbuffer CBufferPerFrame {
	float4 AmbientColor: AMBIENT;
	float4 LightColor: COLOR;
	float3 LightDirection: DIRECTION;
};

cbuffer CBufferPerObject {
	float4x4 WVP: WORLDVIEWPROJECTION;
};

Texture2D PositionBuffer;
Texture2D NormalBuffer;
Texture2D AlbedoSpecularBuffer;

RasterizerState DisableCulling {
	CullMode = NONE;
};

SamplerState TrilinearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_In {
	float4 o_position: POSITION;
	float2 texture_coords: TEXCOORD;
};

struct VS_Out {
	float4 h_position: SV_Position;
	float2 texture_coords: TEXCOORD;
};

VS_Out vertex_shader(VS_In input) {
	VS_Out output = (VS_Out)0;
	output.h_position = mul(input.o_position, WVP);
	output.texture_coords = input.texture_coords;
	return output;
}

float4 pixel_shader(VS_Out input) : SV_Target {
	float4 output = (float4)0;
	float3 w_pos = PositionBuffer.Sample(TrilinearSampler, input.texture_coords).xyz;
	float3 normal = NormalBuffer.Sample(TrilinearSampler, input.texture_coords).xyz;
	float4 texel = AlbedoSpecularBuffer.Sample(TrilinearSampler, input.texture_coords);
	float3 albedo = texel.rgb;
	float specular = texel.a;

	// Lambert
	float3 light_dir = normalize(-LightDirection);
	float n_dot_l = dot(normal, light_dir);
	float3 diffuse = (float3)0;
	if (n_dot_l > 0) {
		diffuse = n_dot_l * (LightColor.rgb * LightColor.a) * albedo;
	}
	float3 ambient = (AmbientColor.rgb * AmbientColor.a) * albedo;
	output.rgb = ambient + diffuse;
	output.a = 1.0f;
	return output;
}

technique11 main11 {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
	}
}