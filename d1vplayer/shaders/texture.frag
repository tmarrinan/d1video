#version 150

uniform sampler2D image;

in vec2 vTexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(image, vTexCoord);
}
