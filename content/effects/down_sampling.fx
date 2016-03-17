cbuffer CBufferPerRender {
	float2 ScreenResolution;
};

Texture2D ColorBuffer;

SamplerState TrilinearSampler;

float4 vertex_shader(float4 o_position : POSITION) : SV_Position {
	return o_position;
}

float4 pixel_shader(float4 p_position : SV_Position) : SV_Target {
	// convert from pixel position to texel position
	// http://www.asawicki.info/news_1516_half-pixel_offset_in_directx_11.html
	float2 uv = p_position.xy / ScreenResolution;

	return  ColorBuffer.Sample(TrilinearSampler, uv);
}

technique11 directional_light_pass {
	pass p0 {
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
	}
}