#version 150

uniform sampler2D image;
uniform float opacity;

in vec2 vTexCoord;

out vec4 FragColor;

void main() {
	vec4 col = texture(image, vTexCoord);
	FragColor = vec4(col.rgb, col.a * opacity);
}
