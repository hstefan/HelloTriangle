#version 330

out vec4 outputColor;

void main()
{
	outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f),
		vec4(0.2f, 0.2f, 0.2f, 1.0f), gl_FragCoord.y/500.0f);
}
