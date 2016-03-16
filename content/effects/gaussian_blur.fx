#define SAMPLE_COUNT 9

cbuffer CBufferPerRender {
	float2 ScreenResolution;
	float2 SampleOffsets[SAMPLE_COUNT];
	float SampleWeights[SAMPLE_COUNT];
};

Texture2D ColorBuffer;

SamplerState TrilinearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
};

float4 vertex_shader(float4 o_position : POSITION) : SV_Position {
	return o_position;
}

float4 pixel_shader_h(float4 p_position : SV_Position) : SV_Target {
	// convert from pixel position to texel position
	// http://www.asawicki.info/news_1516_half-pixel_offset_in_directx_11.html
	float2 uv = p_position.xy / ScreenResolution;

	float4 color = (float4)0;
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		color += ColorBuffer.Sample(TrilinearSampler,
			uv + float2(SampleOffsets[i].x, 0)) * SampleWeights[i];
	}
	return color;
}

float4 pixel_shader_v(float4 p_position : SV_Position) : SV_Target {
	// convert from pixel position to texel position
	// http://www.asawicki.info/news_1516_half-pixel_offset_in_directx_11.html
	float2 uv = p_position.xy / ScreenResolution;

	float4 color = (float4)0;
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		color += ColorBuffer.Sample(TrilinearSampler,
			uv + float2(0, SampleOffsets[i].y)) * SampleWeights[i];
	}
	return color;
}

technique11 gaussian_blur_horizontal {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader_h()));
	}
}

technique11 gaussian_blur_vertical {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader_v()));
	}
}