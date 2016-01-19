#include "d1vPlayer.h"

using namespace std;


#ifdef _WIN32
	PFNGLATTACHSHADERPROC            glAttachShader            = (PFNGLATTACHSHADERPROC)            wglGetProcAddress("glAttachShader");
	PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation      = (PFNGLBINDATTRIBLOCATIONPROC)      wglGetProcAddress("glBindAttribLocation");
	PFNGLBINDBUFFERPROC              glBindBuffer              = (PFNGLBINDBUFFERPROC)              wglGetProcAddress("glBindBuffer");
	PFNGLBINDFRAGDATALOCATIONPROC    glBindFragDataLocation    = (PFNGLBINDFRAGDATALOCATIONPROC)    wglGetProcAddress("glBindFragDataLocation");
	PFNGLBINDVERTEXARRAYPROC         glBindVertexArray         = (PFNGLUSEPROGRAMPROC)              wglGetProcAddress("glBindVertexArray");
	PFNGLBUFFERDATAPROC              glBufferData              = (PFNGLBUFFERDATAPROC)              wglGetProcAddress("glBufferData");
	PFNGLCOMPILESHADERPROC           glCompileShader           = (PFNGLCOMPILESHADERPROC)           wglGetProcAddress("glCompileShader");
	PFNGLCREATEPROGRAMPROC           glCreateProgram           = (PFNGLCREATEPROGRAMPROC)           wglGetProcAddress("glCreateProgram");
	PFNGLCREATESHADERPROC            glCreateShader            = (PFNGLCREATESHADERPROC)            wglGetProcAddress("glCreateShader");
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
	PFNGLGENBUFFERSPROC              glGenBuffers              = (PFNGLGENBUFFERSPROC)              wglGetProcAddress("glGenBuffers");
	PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays         = (PFNGLGENVERTEXARRAYSPROC)         wglGetProcAddress("glGenVertexArrays");
	PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation       = (PFNGLGETATTRIBLOCATIONPROC)       wglGetProcAddress("glGetAttribLocation");
	PFNGLGETPROGRAMIVPROC            glGetProgramiv            = (PFNGLGETPROGRAMIVPROC)            wglGetProcAddress("glGetProgramiv");
	PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC)        wglGetProcAddress("glGetShaderInfoLog");
	PFNGLGETSHADERIVPROC             glGetShaderiv             = (PFNGLGETSHADERIVPROC)             wglGetProcAddress("glGetShaderiv");
	PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC)      wglGetProcAddress("glGetUniformLocation");
	PFNGLLINKPROGRAMPROC             glLinkProgram             = (PFNGLLINKPROGRAMPROC)             wglGetProcAddress("glLinkProgram");
	PFNGLSHADERSOURCEPROC            glShaderSource            = (PFNGLSHADERSOURCEPROC)            wglGetProcAddress("glShaderSource");
	PFNGLUNIFORM1IPROC               glUniform1i               = (PFNGLUNIFORM1IPROC)               wglGetProcAddress("glUniform1i");
	PFNGLUSEPROGRAMPROC              glUseProgram              = (PFNGLUSEPROGRAMPROC)              wglGetProcAddress("glUseProgram");
	PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer     = (PFNGLVERTEXATTRIBPOINTERPROC)     wglGetProcAddress("glVertexAttribPointer");
#elif __linux__
	PFNGLATTACHSHADERPROC            glAttachShader            = (PFNGLATTACHSHADERPROC)            glXGetProcAddress((const GLubyte*)"glAttachShader");
	PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation      = (PFNGLBINDATTRIBLOCATIONPROC)      glXGetProcAddress((const GLubyte*)"glBindAttribLocation");
	PFNGLBINDBUFFERPROC              glBindBuffer              = (PFNGLBINDBUFFERPROC)              glXGetProcAddress((const GLubyte*)"glBindBuffer");
	PFNGLBINDFRAGDATALOCATIONPROC    glBindFragDataLocation    = (PFNGLBINDFRAGDATALOCATIONPROC)    glXGetProcAddress((const GLubyte*)"glBindFragDataLocation");
	PFNGLBINDVERTEXARRAYPROC         glBindVertexArray         = (PFNGLUSEPROGRAMPROC)              glXGetProcAddress((const GLubyte*)"glBindVertexArray");
	PFNGLBUFFERDATAPROC              glBufferData              = (PFNGLBUFFERDATAPROC)              glXGetProcAddress((const GLubyte*)"glBufferData");
	PFNGLCOMPILESHADERPROC           glCompileShader           = (PFNGLCOMPILESHADERPROC)           glXGetProcAddress((const GLubyte*)"glCompileShader");
	PFNGLCREATEPROGRAMPROC           glCreateProgram           = (PFNGLCREATEPROGRAMPROC)           glXGetProcAddress((const GLubyte*)"glCreateProgram");
	PFNGLCREATESHADERPROC            glCreateShader            = (PFNGLCREATESHADERPROC)            glXGetProcAddress((const GLubyte*)"glCreateShader");
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
	PFNGLGENBUFFERSPROC              glGenBuffers              = (PFNGLGENBUFFERSPROC)              glXGetProcAddress((const GLubyte*)"glGenBuffers");
	PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays         = (PFNGLGENVERTEXARRAYSPROC)         glXGetProcAddress((const GLubyte*)"glGenVertexArrays");
	PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation       = (PFNGLGETATTRIBLOCATIONPROC)       glXGetProcAddress((const GLubyte*)"glGetAttribLocation");
	PFNGLGETPROGRAMIVPROC            glGetProgramiv            = (PFNGLGETPROGRAMIVPROC)            glXGetProcAddress((const GLubyte*)"glGetProgramiv");
	PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC)        glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
	PFNGLGETSHADERIVPROC             glGetShaderiv             = (PFNGLGETSHADERIVPROC)             glXGetProcAddress((const GLubyte*)"glGetShaderiv");
	PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC)      glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
	PFNGLLINKPROGRAMPROC             glLinkProgram             = (PFNGLLINKPROGRAMPROC)             glXGetProcAddress((const GLubyte*)"glLinkProgram");
	PFNGLSHADERSOURCEPROC            glShaderSource            = (PFNGLSHADERSOURCEPROC)            glXGetProcAddress((const GLubyte*)"glShaderSource");
	PFNGLUNIFORM1IPROC               glUniform1i               = (PFNGLUNIFORM1IPROC)               glXGetProcAddress((const GLubyte*)"glUniform1i");
	PFNGLUSEPROGRAMPROC              glUseProgram              = (PFNGLUSEPROGRAMPROC)              glXGetProcAddress((const GLubyte*)"glUseProgram");
	PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer     = (PFNGLVERTEXATTRIBPOINTERPROC)     glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
#endif



d1vPlayer::d1vPlayer(SDL_Window *win, string exe) {
	mainwindow = win;
	exePath = exe;
	eof = false;
}

void d1vPlayer::initGL(string inFile, unsigned int w, unsigned int h) {
	SDL_GL_SetSwapInterval(1);

	vidFile = inFile;

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
	if (vidAspect < winAspect) {
		viewW = (int)((double)winH * vidAspect);
		viewH = winH;
		viewX = (winW - viewW) / 2;
		viewY = 0;
	}
	else {
		viewW = winW;
		viewH = (int)((double)winW / vidAspect);
		viewX = 0;
		viewY = (winH - viewH) / 2;
	}
	glViewport(viewX, viewY, viewW, viewH);
}

void d1vPlayer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vidTexture);
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

	glGenTextures(1, &vidTexture);

	loadDXT1(vidFile);

	vidAspect = (double)frameW / (double)frameH;

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, vidTexture);

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

	glBindTexture(GL_TEXTURE_2D, vidTexture);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, frameW, frameH, 0, frameSize, d1vPixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void d1vPlayer::initShaders(std::string name) {
	string vertSource = readFile(exePath + "../d1vplayer/shaders/" + name + ".vert");
	GLint vertexShader = compileShader(vertSource, GL_VERTEX_SHADER);

	string fragSource = readFile(exePath + "../d1vplayer/shaders/" + name + ".frag");
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
