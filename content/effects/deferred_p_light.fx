cbuffer CBufferPerFrame {
	float4 LightColor: COLOR;
	float3 LightPosition: POSITION;
	float3 LightAttenuation;
	float3 CameraPosition: CAMERAPOSITION;
};

//cbuffer CBufferPerObject {
//	float4 SpecularColor: SPECULAR;
//	float SpecularPower : SPECULARPOWER;
//};

Texture2D PositionBuffer;
Texture2D NormalBuffer;
Texture2D AlbedoSpecularBuffer;

RasterizerState DisableCulling {
	CullMode = NONE;
};

BlendState EnableAdditiveBlending {
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
};

SamplerState TrilinearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	//AddressU = WRAP;
	//AddressV = WRAP;
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
	output.h_position = input.o_position;
	output.texture_coords = input.texture_coords;
	return output;
}

float4 pixel_shader(VS_Out input) : SV_Target {
	float4 output = (float4)0;
	float3 w_pos = PositionBuffer.Sample(TrilinearSampler, input.texture_coords).xyz;
	float4 normal = NormalBuffer.Sample(TrilinearSampler, input.texture_coords);
	float SpecularPower = normal.w; // fetch specular power
	float4 texel = AlbedoSpecularBuffer.Sample(TrilinearSampler, input.texture_coords);
	float3 albedo = texel.rgb;
	float specular = texel.a;
	specular = 0.8f;	// fix specular for test

	float3 light_dir = LightPosition - w_pos;
	float dist = length(light_dir);
	float attenuation = max(1.0f, LightAttenuation.x +
						 LightAttenuation.y * dist +
						 LightAttenuation.z * dist * dist);
	float3 view_dir = normalize(CameraPosition - w_pos);
	float3 halfv = normalize(light_dir + view_dir);
	float n_dot_l = dot(normal.xyz, light_dir);
	float n_dot_h = dot(normal.xyz, halfv);

	float4 lambert_phong = lit(n_dot_l, n_dot_h, SpecularPower);
	float3 d = lambert_phong.y * (LightColor.rgb * LightColor.a) * albedo;
	// hard-coded specular color
	float4 SpecularColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 s = min(lambert_phong.z, specular) * (SpecularColor.rgb * SpecularColor.a);
	output.rgb = d + s;
	output.a = 1.0f;
	return output / attenuation;
}

technique11 main11 {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
		//SetRasterizerState(DisableCulling);
		SetBlendState(EnableAdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}