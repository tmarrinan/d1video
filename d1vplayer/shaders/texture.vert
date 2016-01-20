#version 150

in vec2 aVertexPosition;
in vec2 aVertexTextureCoord;

out vec2 vTexCoord;

void main() {
	vTexCoord = aVertexTextureCoord;
	gl_Position = vec4(aVertexPosition, -1.0, 1.0);
}
