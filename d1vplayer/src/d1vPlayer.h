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

#ifndef __APPLE__
extern PFNGLATTACHSHADERPROC            glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation;
extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBINDFRAGDATALOCATIONPROC    glBindFragDataLocation;
extern PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLCOMPILESHADERPROC           glCompileShader;
extern PFNGLCREATEPROGRAMPROC           glCreateProgram;
extern PFNGLCREATESHADERPROC            glCreateShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC             glLinkProgram;
extern PFNGLSHADERSOURCEPROC            glShaderSource;
extern PFNGLUNIFORM1IPROC               glUniform1i;
extern PFNGLUSEPROGRAMPROC              glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
#endif

class d1vPlayer {
private:
	SDL_Window *mainwindow;

	GLuint vertexArrayObject;
	GLuint vertexPositionBuffer;
	GLuint vertexTextureBuffer;
	GLuint vertexIndexBuffer;

	GLuint vidTexture;

	GLuint shaderProgram;
	GLint vertexPositionAttribute;
	GLint vertexTextureAttribute;

	GLint dxt1Uniform;

	std::string exePath;
	std::string vidFile;
	double vidAspect;

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