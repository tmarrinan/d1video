#version 150

uniform vec2 translate;

in vec2 aVertexPosition;
in vec4 aVertexColor;

out vec4 vColor;

void main() {
	vColor = aVertexColor;
	gl_Position = vec4(aVertexPosition + translate, 0.0, 1.0);
}
