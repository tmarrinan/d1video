#include "d1vPlayer.h"

using namespace std;

d1vPlayer::d1vPlayer(SDL_Window *win, string exe) {
	mainwindow = win;
	exePath = exe;
	eof = false;
}

void d1vPlayer::initGL(string inFile, unsigned int w, unsigned int h) {
	SDL_GL_SetSwapInterval(1);

	imgFile = inFile;

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	initShaders("texture");
	initBuffers();
	initTextures();
	setViewport(w, h);
}

void d1vPlayer::setViewport(unsigned int winW, unsigned int winH) {
	int viewX, viewY, viewW, viewH;
	double winAspect = (double)winW / (double)winH;
	if (imgAspect < winAspect) {
		viewW = (int)((double)winH * imgAspect);
		viewH = winH;
		viewX = (winW - viewW) / 2;
		viewY = 0;
	}
	else {
		viewW = winW;
		viewH = (int)((double)winW / imgAspect);
		viewX = 0;
		viewY = (winH - viewH) / 2;
	}
	glViewport(viewX, viewY, viewW, viewH);
}

void d1vPlayer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imgTexture);
	glUniform1i(dxt1Uniform, 0);

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	SDL_GL_SwapWindow(mainwindow);
}

void d1vPlayer::resize(unsigned int w, unsigned int h) {
	setViewport(w, h);
	render();
}

bool d1vPlayer::hasMoreFrames() {
	return !eof;
}

void d1vPlayer::initBuffers() {
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// vertices
	glGenBuffers(1, &vertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	GLfloat vertices[] = {
		-1.0, -1.0, -1.0,  // left,  bottom, back
		-1.0,  1.0, -1.0,  // left,  top,    back
		 1.0, -1.0, -1.0,  // right, bottom, back
		 1.0,  1.0, -1.0   // right, top,    back
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPositionAttribute);
	glVertexAttribPointer(vertexPositionAttribute, 3, GL_FLOAT, false, 0, 0);

	// textures
	glGenBuffers(1, &vertexTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexTextureBuffer);
	GLfloat textureCoords[] = {
		0.0, 0.0,  // left,  bottom
		0.0, 1.0,  // left,  top
		1.0, 0.0,  // right, bottom
		1.0, 1.0   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), textureCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexTextureAttribute);
	glVertexAttribPointer(vertexTextureAttribute, 2, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
	GLushort vertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), vertexIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void d1vPlayer::initTextures() {
	GLenum err;

	glGenTextures(1, &imgTexture);

	loadDXT1(imgFile);

	imgAspect = (double)frameW / (double)frameH;

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, imgTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, frameW, frameH, 0, frameSize, d1vPixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void d1vPlayer::updateTextures() {
	size_t result = fread(d1vPixels, 1, frameSize, d1vF);

	if (result != frameSize) {
		eof = true;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, imgTexture);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, frameW, frameH, 0, frameSize, d1vPixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void d1vPlayer::initShaders(std::string name) {
	string vertSource = readFile(exePath + "../dxt1player/shaders/" + name + ".vert");
	GLint vertexShader = compileShader(vertSource, GL_VERTEX_SHADER);

	string fragSource = readFile(exePath + "../dxt1player/shaders/" + name + ".frag");
	GLint fragmentShader = compileShader(fragSource, GL_FRAGMENT_SHADER);
	
	createShaderProgram(name, vertexShader, fragmentShader);
}

GLint d1vPlayer::compileShader(string source, GLint type) {
	GLint status;
	GLint shader = glCreateShader(type);

	const char *srcBytes = source.c_str();
	int srcLength = source.length();
	glShaderSource(shader, 1, &srcBytes, &srcLength);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		GLint length;
		char *info;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		info = SDL_stack_alloc(char, length+1);
		glGetShaderInfoLog(shader, length, NULL, info);
		fprintf(stderr, "Failed to compile shader:\n%s\n", info);
		SDL_stack_free(info);

		return -1;
	}
	else {
		return shader;
	}
}

void d1vPlayer::createShaderProgram(string name, GLint vertexShader, GLint fragmentShader) {
	GLint status;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(shaderProgram, 0, "aVertexPosition");
	glBindAttribLocation(shaderProgram, 1, "aVertexTextureCoord");
	glBindFragDataLocation(shaderProgram, 0, "FragColor");

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if(status == 0) {
		fprintf(stderr, "Unable to initialize the shader program\n");
	}

	// set vertex array
	vertexPositionAttribute = glGetAttribLocation(shaderProgram, "aVertexPosition");
	// set texture coord array
	vertexTextureAttribute = glGetAttribLocation(shaderProgram, "aVertexTextureCoord");
	// set image textures
	dxt1Uniform   = glGetUniformLocation(shaderProgram, "image");

	glUseProgram(shaderProgram);
}

string d1vPlayer::readFile(string filename) {
	FILE *f = fopen(filename.c_str(), "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *text = (char*)malloc(fsize);
	fread(text, fsize, 1, f);
	fclose(f);

	return string(text, fsize);
}

void d1vPlayer::loadDXT1(string filename) {
	size_t result;

	d1vF = fopen(filename.c_str(), "rb");
	fread(&frameW,    4, 1, d1vF);
	fread(&frameH,    4, 1, d1vF);
	fread(&numFrames, 4, 1, d1vF);
	fread(&d1vFps,    2, 1, d1vF);

	frameSize = frameW * frameH / 2;
	d1vPixels = (GLubyte*) malloc(frameSize);
	fread(d1vPixels, 1, frameSize, d1vF);
}

unsigned int d1vPlayer::getPlaybackFps() {
	return d1vFps;
}

void d1vPlayer::rewind() {
	fseek(d1vF, 14, SEEK_SET);
	eof = false;
}

void d1vPlayer::close() {
	fclose(d1vF);
}
