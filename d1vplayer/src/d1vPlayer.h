#ifndef D1VPLAYER_H
#define D1VPLAYER_H
#include <string>
#include <SDL.h>

#ifdef _WIN32
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <GL/wgl.h>
	#include <GL/wglext.h>
#elif __APPLE__
	#include <OpenGL/gl3.h>
	#include <OpenGL/gl3ext.h>
#else // __linux__
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <GL/glx.h>
	#include <GL/glxext.h>
#endif

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif

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

class d1vPlayer {
private:
	SDL_Window *mainwindow;

	GLuint vertexArrayObject;
	GLuint vertexPositionBuffer;
	GLuint vertexTextureBuffer;
	GLuint vertexIndexBuffer;

	GLuint imgTexture;

	GLuint shaderProgram;
	GLint vertexPositionAttribute;
	GLint vertexTextureAttribute;

	GLint dxt1Uniform;

	std::string exePath;
	std::string imgFile;
	double imgAspect;

	FILE *d1vF;
	bool eof;
	unsigned int frameW;
	unsigned int frameH;
	unsigned int frameSize;
	unsigned int numFrames;
	unsigned short d1vFps;
	GLubyte *d1vPixels;

public:
	d1vPlayer(SDL_Window *win, std::string exe);
	void initGL(std::string inFile, unsigned int w, unsigned int h);
	void setViewport(unsigned int w, unsigned int h);
	void render();
	void resize(unsigned int w, unsigned int h);
	bool hasMoreFrames();
	void initBuffers();
	void initTextures();
	void updateTextures();
	void initShaders(std::string name);
	GLint compileShader(std::string source, GLint type);
	void createShaderProgram(std::string name, GLint vertexShader, GLint fragmentShader);
	std::string readFile(std::string filename);
	void loadDXT1(std::string filename);
	unsigned int getPlaybackFps();
	void rewind();
	void close();
};

#endif //D1VPLAYER_H