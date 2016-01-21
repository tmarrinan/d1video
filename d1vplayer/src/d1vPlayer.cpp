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
	currFrame = 0;
	eof = false;
	isPaused = false;
	guiOpacity = 1.0;
}

void d1vPlayer::initGL(string inFile, unsigned int w, unsigned int h) {
	SDL_GL_SetSwapInterval(1);
	TTF_Init();

	vidFile = inFile;
	winW = w;
	winH = h;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	initShaders("texture", &shaderProgram);
	initShaders("color", &guiShaderProgram);
	initBuffers();
	initTextures();
	loadFonts();
	updateFontTexture(videoTime(0));
}

void d1vPlayer::setVideoViewport() {
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

void d1vPlayer::setGuiViewport() {
	int viewX, viewY, viewW, viewH;
	if (winW < winH) {
		viewW = winW;
		viewH = winW;
		viewX = 0;
		viewY = (winH - winW) / 2;
	}
	else {
		viewW = winH;
		viewH = winH;
		viewX = (winW - winH) / 2;
		viewY = 0;
	}
	glViewport(viewX, viewY, viewW, viewH);
}

void d1vPlayer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render video texture
	glUseProgram(shaderProgram);
	setVideoViewport();
	glUniform1f(texOpacityUniform, 1.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vidTexture);
	glUniform1i(imageUniform, 0);

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	// render video player gui
	if (guiOpacity > 0.0) {
		glUseProgram(guiShaderProgram);
		setGuiViewport();
		glUniform1f(opacityUniform, guiOpacity);

		glUniform2f(translateUniform, 0.0, 0.0);
		glBindVertexArray(guiVertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		if (isPaused) {
			glUniform2f(translateUniform, 0.0, 0.0);
			glBindVertexArray(playVertexArrayObject);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
		}
		else {
			glUniform2f(translateUniform, 0.0, 0.0);
			glBindVertexArray(pauseVertexArrayObject);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
		}

		glUniform2f(translateUniform, 0.0, 0.0);
		glBindVertexArray(sliderVertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		GLfloat t = ((GLfloat)currFrame / (GLfloat)numFrames) * 0.46;
		glUniform2f(translateUniform, t, 0.0);
		glBindVertexArray(knobVertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		glUniform2f(translateUniform, 0.0, 0.0);
		glBindVertexArray(rewindVertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		glUseProgram(shaderProgram);
		glUniform1f(texOpacityUniform, guiOpacity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		glUniform1i(imageUniform, 0);

		glBindVertexArray(textVertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	}

	SDL_GL_SwapWindow(mainwindow);
}

void d1vPlayer::resize(unsigned int w, unsigned int h) {
	winW = w;
	winH = h;
}

bool d1vPlayer::hasMoreFrames() {
	return !eof;
}

void d1vPlayer::initBuffers() {
	// video texture
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// vertices
	glGenBuffers(1, &vertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	GLfloat vertices[] = {
		-1.0, -1.0,  // left,  bottom
		-1.0,  1.0,  // left,  top
		 1.0, -1.0,  // right, bottom
		 1.0,  1.0   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPositionAttribute);
	glVertexAttribPointer(vertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

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


	// gui bar
	glGenVertexArrays(1, &guiVertexArrayObject);
	glBindVertexArray(guiVertexArrayObject);

	// vertices
	glGenBuffers(1, &guiVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexPositionBuffer);
	GLfloat guiVertices[] = {
		-0.47, -0.75,  // left,  bottom
		-0.47, -0.65,  // left,  top
		 0.47, -0.75,  // right, bottom
		 0.47, -0.65,  // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), guiVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &guiVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexColorBuffer);
	GLfloat guiColors[] = {
		0.54, 0.67, 0.70, 0.75,  // left,  bottom
		0.54, 0.67, 0.70, 0.75,  // left,  top
		0.54, 0.67, 0.70, 0.75,  // right, bottom
		0.54, 0.67, 0.70, 0.75,  // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), guiColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &guiVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, guiVertexIndexBuffer);
	GLushort guiVertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), guiVertexIndices, GL_STATIC_DRAW);


	// gui - play
	glGenVertexArrays(1, &playVertexArrayObject);
	glBindVertexArray(playVertexArrayObject);

	// vertices
	glGenBuffers(1, &playVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playVertexPositionBuffer);
	GLfloat playVertices[] = {
		-0.44, -0.73,  // left,  bottom
		-0.44, -0.67,  // left,  top
		-0.38, -0.70   // right, middle
	};
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), playVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &playVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playVertexColorBuffer);
	GLfloat playColors[] = {
		0.70, 0.88, 0.91, 1.00,  // left,  bottom
		0.70, 0.88, 0.91, 1.00,  // left,  top
		0.70, 0.88, 0.91, 1.00   // right, middle
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), playColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &playVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playVertexIndexBuffer);
	GLushort playVertexIndices[] = {
		 0, 2, 1
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort), playVertexIndices, GL_STATIC_DRAW);


	// gui - pause
	glGenVertexArrays(1, &pauseVertexArrayObject);
	glBindVertexArray(pauseVertexArrayObject);

	// vertices
	glGenBuffers(1, &pauseVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pauseVertexPositionBuffer);
	GLfloat pauseVertices[] = {
		-0.44, -0.73,  // left  - left,  bottom
		-0.44, -0.67,  // left  - left,  top
		-0.42, -0.73,  // left  - right, bottom
		-0.42, -0.67,  // left  - right, top
		-0.40, -0.73,  // right - left,  bottom
		-0.40, -0.67,  // right - left,  top
		-0.38, -0.73,  // right - right, bottom
		-0.38, -0.67,  // right - right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), pauseVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &pauseVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pauseVertexColorBuffer);
	GLfloat pauseColors[] = {
		0.70, 0.88, 0.91, 1.00,  // left  - left,  bottom
		0.70, 0.88, 0.91, 1.00,  // left  - left,  top
		0.70, 0.88, 0.91, 1.00,  // left  - right, bottom
		0.70, 0.88, 0.91, 1.00,  // left  - right, top
		0.70, 0.88, 0.91, 1.00,  // right - left,  bottom
		0.70, 0.88, 0.91, 1.00,  // right - left,  top
		0.70, 0.88, 0.91, 1.00,  // right - right, bottom
		0.70, 0.88, 0.91, 1.00   // right - right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(GLfloat), pauseColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &pauseVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pauseVertexIndexBuffer);
	GLushort pauseVertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2,
		 4, 7, 5,
		 7, 4, 6
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLushort), pauseVertexIndices, GL_STATIC_DRAW);


	// gui - time slider
	glGenVertexArrays(1, &sliderVertexArrayObject);
	glBindVertexArray(sliderVertexArrayObject);

	// vertices
	glGenBuffers(1, &sliderVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sliderVertexPositionBuffer);
	GLfloat sliderVertices[] = {
		-0.350, -0.705,  // left,  bottom
		-0.350, -0.695,  // left,  top
		 0.110, -0.705,  // right, bottom
		 0.110, -0.695   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), sliderVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &sliderVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sliderVertexColorBuffer);
	GLfloat sliderColors[] = {
		0.70, 0.88, 0.91, 1.00,  // left,  bottom
		0.70, 0.88, 0.91, 1.00,  // left,  top
		0.70, 0.88, 0.91, 1.00,  // right, bottom
		0.70, 0.88, 0.91, 1.00   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), sliderColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &sliderVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sliderVertexIndexBuffer);
	GLushort sliderVertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), sliderVertexIndices, GL_STATIC_DRAW);


	// gui - time knob
	glGenVertexArrays(1, &knobVertexArrayObject);
	glBindVertexArray(knobVertexArrayObject);

	// vertices
	glGenBuffers(1, &knobVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, knobVertexPositionBuffer);
	GLfloat knobVertices[] = {
		-0.36, -0.72,  // left,  bottom
		-0.36, -0.68,  // left,  top
		-0.34, -0.72,  // right, bottom
		-0.34, -0.68   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), knobVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &knobVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, knobVertexColorBuffer);
	GLfloat knobColors[] = {
		0.77, 0.96, 1.00, 1.00,  // left,  bottom
		0.77, 0.96, 1.00, 1.00,  // left,  top
		0.77, 0.96, 1.00, 1.00,  // right, bottom
		0.77, 0.96, 1.00, 1.00   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), knobColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &knobVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, knobVertexIndexBuffer);
	GLushort knobVertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), knobVertexIndices, GL_STATIC_DRAW);


	// gui - rewind
	glGenVertexArrays(1, &rewindVertexArrayObject);
	glBindVertexArray(rewindVertexArrayObject);

	// vertices
	glGenBuffers(1, &rewindVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rewindVertexPositionBuffer);
	GLfloat rewindVertices[] = {
		0.38, -0.70,  // left -  left,  middle
		0.41, -0.67,  // left -  right, bottom
		0.41, -0.73,  // left -  right, top
		0.41, -0.70,  // right - left,  middle
		0.44, -0.67,  // right - right, bottom
		0.44, -0.73   // right - right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rewindVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexPositionAttribute);
	glVertexAttribPointer(guiVertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// colors
	glGenBuffers(1, &rewindVertexColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rewindVertexColorBuffer);
	GLfloat rewindColors[] = {
		0.77, 0.96, 1.00, 1.00,  // left -  left,  middle
		0.77, 0.96, 1.00, 1.00,  // left -  right, bottom
		0.77, 0.96, 1.00, 1.00,  // left -  right, top
		0.77, 0.96, 1.00, 1.00,  // right - left,  middle
		0.77, 0.96, 1.00, 1.00,  // right - right, bottom
		0.77, 0.96, 1.00, 1.00   // right - right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), rewindColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(guiVertexColorAttribute);
	glVertexAttribPointer(guiVertexColorAttribute, 4, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &rewindVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rewindVertexIndexBuffer);
	GLushort rewindVertexIndices[] = {
		 0, 1, 2,
		 3, 4, 5
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), rewindVertexIndices, GL_STATIC_DRAW);


	// gui - time text
	glGenVertexArrays(1, &textVertexArrayObject);
	glBindVertexArray(textVertexArrayObject);

	// vertices
	glGenBuffers(1, &textVertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexPositionBuffer);
	GLfloat textVertices[] = {
		0.14, -0.72,  // left,  bottom
		0.14, -0.68,  // left,  top
		0.347, -0.72,  // right, bottom
		0.347, -0.68   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), textVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPositionAttribute);
	glVertexAttribPointer(vertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);

	// texture coords
	glGenBuffers(1, &textVertexTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexTextureBuffer);
	GLfloat textTexCoords[] = {
		0.0, 1.0,   // left,  bottom
		0.0, 0.0,   // left,  top
		1.0, 1.0,   // right, bottom
		1.0, 0.0,   // right, top
	};
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), textTexCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexTextureAttribute);
	glVertexAttribPointer(vertexTextureAttribute, 2, GL_FLOAT, false, 0, 0);

	// faces of triangles
	glGenBuffers(1, &textVertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textVertexIndexBuffer);
	GLushort textVertexIndices[] = {
		 0, 3, 1,
		 3, 0, 2
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), textVertexIndices, GL_STATIC_DRAW);


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

	currFrame++;

	if (result != frameSize) {
		eof = true;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, vidTexture);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, frameW, frameH, 0, frameSize, d1vPixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	updateFontTexture(videoTime((int)((double)currFrame / (double)d1vFps)));
}

void d1vPlayer::initShaders(string name, GLuint *program) {
	string vertSource = readFile(exePath + "../d1vplayer/shaders/" + name + ".vert");
	GLint vertexShader = compileShader(vertSource, GL_VERTEX_SHADER);

	string fragSource = readFile(exePath + "../d1vplayer/shaders/" + name + ".frag");
	GLint fragmentShader = compileShader(fragSource, GL_FRAGMENT_SHADER);
	
	createShaderProgram(name, vertexShader, fragmentShader, program);
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

void d1vPlayer::createShaderProgram(string name, GLint vertexShader, GLint fragmentShader, GLuint *program) {
	GLint status;
	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	glBindAttribLocation(*program, 0, "aVertexPosition");
	if (name == "texture")    glBindAttribLocation(*program, 1, "aVertexTextureCoord");
	else if (name == "color") glBindAttribLocation(*program, 1, "aVertexColor");
	glBindFragDataLocation(*program, 0, "FragColor");

	glLinkProgram(*program);

	glGetProgramiv(*program, GL_LINK_STATUS, &status);
	if(status == 0) {
		fprintf(stderr, "Unable to initialize the shader program\n");
	}

	if (name == "texture") {
		// set vertex array
		vertexPositionAttribute = glGetAttribLocation(*program, "aVertexPosition");
		// set texture coord array
		vertexTextureAttribute = glGetAttribLocation(*program, "aVertexTextureCoord");
		// set image textures
		imageUniform = glGetUniformLocation(*program, "image");
		// set opacity uniform
		texOpacityUniform = glGetUniformLocation(*program, "opacity");
	}
	else if (name == "color") {
		// set vertex array
		guiVertexPositionAttribute = glGetAttribLocation(*program, "aVertexPosition");
		// set color array
		guiVertexColorAttribute = glGetAttribLocation(*program, "aVertexColor");
		// set translate uniform
		translateUniform = glGetUniformLocation(*program, "translate");
		// set opacity uniform
		opacityUniform = glGetUniformLocation(*program, "opacity");
	}

	glUseProgram(*program);
}

void d1vPlayer::loadFonts() {
	font = TTF_OpenFont((exePath + "../d1vplayer/fonts/Arial.ttf").c_str(), 72);
	
	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void d1vPlayer::updateFontTexture(string text) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	SDL_Color color = {196, 245, 255};
	SDL_Surface *sFont = TTF_RenderText_Blended(font, text.c_str(), color);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(sFont);
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

	duration = (int)((double)numFrames / (double)d1vFps);
	frameSize = frameW * frameH / 2;
	d1vPixels = (GLubyte*) malloc(frameSize);
	fread(d1vPixels, 1, frameSize, d1vF);

	currFrame++;
}

string d1vPlayer::videoTime(int time) {
	char vt[20];
	int cm = time / 60;
	int cs = time % 60;
	int tm = duration / 60;
	int ts = duration % 60;
	sprintf(vt, "%02d:%02d / %02d:%02d", cm, cs, tm, ts);

	return vt;
}

unsigned int d1vPlayer::getPlaybackFps() {
	return d1vFps;
}

void d1vPlayer::setPaused(bool paused) {
	isPaused = paused;
}

void d1vPlayer::setGuiOpacity(GLfloat opacity) {
	guiOpacity = opacity;
}

void d1vPlayer::rewind() {
	fseek(d1vF, 14, SEEK_SET);
	eof = false;
	currFrame = 0;
}

void d1vPlayer::close() {
	fclose(d1vF);
}
