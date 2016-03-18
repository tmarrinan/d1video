#ifndef D1VPLAYER_H
#define D1VPLAYER_H
#include <cstdlib>
#include <cmath>
#include <string>
#include <sys/stat.h>

#include <GL/glew.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/freeglut.h>
#endif


class d1vPlayer {
private:
	GLuint vertexArrayObject;
	GLuint vertexPositionBuffer;
	GLuint vertexTextureBuffer;
	GLuint vertexIndexBuffer;

	GLuint vidTexture;

	GLuint shaderProgram;
	GLint vertexPositionAttribute;
	GLint vertexTextureAttribute;

	GLint imageUniform;
	GLint translateUniform;
	GLint opacityUniform;
	GLint texOpacityUniform;

	GLuint guiVertexArrayObject;
	GLuint guiVertexPositionBuffer;
	GLuint guiVertexColorBuffer;
	GLuint guiVertexIndexBuffer;

	GLuint playVertexArrayObject;
	GLuint playVertexPositionBuffer;
	GLuint playVertexColorBuffer;
	GLuint playVertexIndexBuffer;

	GLuint pauseVertexArrayObject;
	GLuint pauseVertexPositionBuffer;
	GLuint pauseVertexColorBuffer;
	GLuint pauseVertexIndexBuffer;

	GLuint sliderVertexArrayObject;
	GLuint sliderVertexPositionBuffer;
	GLuint sliderVertexColorBuffer;
	GLuint sliderVertexIndexBuffer;

	GLuint knobVertexArrayObject;
	GLuint knobVertexPositionBuffer;
	GLuint knobVertexColorBuffer;
	GLuint knobVertexIndexBuffer;

	GLuint rewindVertexArrayObject;
	GLuint rewindVertexPositionBuffer;
	GLuint rewindVertexColorBuffer;
	GLuint rewindVertexIndexBuffer;

	GLuint loopVertexArrayObject;
	GLuint loopVertexPositionBuffer;
	GLuint loopVertexColorBuffer;
	GLuint loopVertexIndexBuffer;

	GLuint textVertexArrayObject;
	GLuint textVertexPositionBuffer;
	GLuint textVertexTextureBuffer;
	GLuint textVertexIndexBuffer;

	GLuint guiShaderProgram;
	GLint guiVertexPositionAttribute;
	GLint guiVertexColorAttribute;

	//TTF_Font *font;
	GLuint fontTexture;

	int duration;

	bool isPaused;
	GLfloat guiOpacity;

	std::string exePath;
	std::string vidFile;
	double vidAspect;

	int winW;
	int winH;
	bool showGui;

	FILE *d1vF;
	bool eof;
	unsigned int currFrame;
	unsigned int frameW;
	unsigned int frameH;
	unsigned int frameSize;
	unsigned int numFrames;
	unsigned short d1vFps;
	GLubyte *d1vPixels;

public:
	d1vPlayer(std::string exe);
	void initGL(std::string inFile, bool gui);
	void setVideoViewport();
	void setGuiViewport();
	void render();
	void resize();
	bool hasMoreFrames();
	void initBuffers();
	void initTextures();
	void updateTextures();
	void initShaders(std::string name, GLuint *program);
	GLint compileShader(std::string source, GLint type);
	void createShaderProgram(std::string name, GLint vertexShader, GLint fragmentShader, GLuint *program);
	void loadFonts();
	void updateFontTexture(std::string text);
	std::string readFile(std::string filename);
	void loadDXT1(std::string filename);
	std::string videoTime(int time);
	unsigned int getPlaybackFps();
	void setPaused(bool paused);
	void setGuiOpacity(GLfloat opacity);
	GLfloat getGuiOpacity();
	void setLooped(bool loop);
	void setVideoTime(double timep);
	void rewind();
	void close();
	unsigned int toBigEndian(unsigned int val);
	unsigned int toLittleEndian(unsigned int val);
};

#endif //D1VPLAYER_H