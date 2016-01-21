#version 150

uniform float opacity;

in vec4 vColor;

out vec4 FragColor;

void main() {
	FragColor = vec4(vColor.rgb, vColor.a * opacity);
}
