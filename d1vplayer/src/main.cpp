#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <SDL.h>

#include "d1vPlayer.h"

#define PROGRAM_NAME "D1V Video Player"

using namespace std;


unsigned int IDLE = 0;
unsigned int NEXTFRAME = 1;
unsigned int HIDEGUI = 2;

SDL_Window *mainwindow;         // Window handle
SDL_GLContext maincontext;      // OpenGL context handle
d1vPlayer *renderer;           // Renderer
string d1vFile;                 // Input dxt1 video
string exePath;                 // Executable path
unsigned int framecount;        // Current frame being rendered
unsigned int framerate;         // Playback framerate (milliseconds per frame)
bool paused;                    // Whether or not video is paused
bool ctrl;                      // Whether or not user is holding 'ctrl' key ('cmd' for Mac OS X)
unsigned int startTime;         // Time used for proper animation
unsigned int startPauseTime;    // 
SDL_TimerID animationTimer;     // Animation timer
SDL_TimerID guiTimer;           // GUI timer
unsigned int guiT;              // GUI time for animation
bool fadeGui;

void parseArguments(int argc, char **argv, string *exe, string *inputFile);
void idle();
void nextFrame();
unsigned int renderNextFrame(unsigned int interval, void *param);
void onResize(unsigned int w, unsigned int h);
void onKeyPress(SDL_KeyboardEvent &key);
void onKeyRelease(SDL_KeyboardEvent &key);
string getExecutablePath(string exe);
void toggleFullScreen();
void exitFullScreen();
void resetGuiTimeout();
void finishAndQuit();
void SDL_Die(const char *msg);
void SDL_MainLoop();

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("\n");
		printf("  Usage: dxt1player [options]\n");
		printf("\n");
		printf("  Options:\n");
		printf("\n");
		printf("    -i, --input <FILE>      dxt1 compressed video to show\n");
		printf("\n");
		return 0;
	}

	parseArguments(argc, argv, &exePath, &d1vFile);
	paused = false;
	ctrl = false;
	guiTimer = 0;
	fadeGui = false;

	struct stat info;
	if (stat(d1vFile.c_str(), &info) != 0) {
		printf("\"%s\" does not exist or cannot be accessed, please specify an image file\n", d1vFile.c_str());
		return 0;
	}
	else if (info.st_mode & S_IFDIR) {
		printf("\"%s\" is not a file, please specify an image file\n", d1vFile.c_str());
		return 0;
	}


	// Initialize SDL's video subsystem (or die on error)
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		SDL_Die("Unable to initialize SDL");

	// Initialize GL attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Declare minimum OpenGL version - 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Create our window centered at initial resolution
	int width = 1280;
	int height = 720;
	mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!mainwindow)
		SDL_Die("Unable to create window");

	maincontext = SDL_GL_CreateContext(mainwindow);

	const unsigned char* glVersion = glGetString(GL_VERSION);
	const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Using OpenGL %s, GLSL %s\n", glVersion, glslVersion);

	renderer = new d1vPlayer(mainwindow, exePath);
	renderer->initGL(d1vFile, width, height);
	framecount = 0;
	framerate = (unsigned int)(1000.0 / (double)renderer->getPlaybackFps());
	startTime = SDL_GetTicks();
	renderer->render();
	nextFrame();

	SDL_MainLoop();

	return 0;
}

void parseArguments(int argc, char **argv, string *exe, string *inputFile) {
	*exe = getExecutablePath(argv[0]);
	bool hasInput = false;

	if (argc >= 3) {
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--input") == 0) {
			*inputFile = argv[2];
			hasInput = true;
		}
	}
	
	if (!hasInput) {
		printf("please specify an input image file\n");
		SDL_Quit();
		exit(0);
	}
}

void idle() {
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = &IDLE;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
}

void nextFrame() {
	framecount++;
	if (paused) return;

	unsigned int now = SDL_GetTicks();
	unsigned int t = now - startTime;

	if (t >= framerate * framecount) {
		renderNextFrame(0, NULL);
	}
	else {
		animationTimer = SDL_AddTimer((framerate * framecount) - t, renderNextFrame, NULL);
	}
}

unsigned int renderNextFrame(unsigned int interval, void *param) {
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = &NEXTFRAME;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);

	return 0;
}

unsigned int hideGui(unsigned int interval, void *param) {
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = &HIDEGUI;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);

	return 0;
}

void onResize(unsigned int w, unsigned int h) {
	renderer->resize(w, h);
}


void onKeyPress(SDL_KeyboardEvent &key) {
	switch (key.keysym.scancode) {
#ifdef __APPLE__
		case SDL_SCANCODE_LGUI:
		case SDL_SCANCODE_RGUI:
#else
		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_RCTRL:
#endif
			ctrl = true;
			break;
		case SDL_SCANCODE_ESCAPE:
			exitFullScreen();
			break;
		case SDL_SCANCODE_SPACE:
			paused = !paused;
			renderer->setPaused(paused);
			if (paused) {
				startPauseTime = SDL_GetTicks();
			}
			else {
				startTime +=  SDL_GetTicks() - startPauseTime;
				renderNextFrame(0, NULL);
			}
			break;
		case SDL_SCANCODE_LEFT:
			renderer->rewind();
			framecount = 0;
			startTime = SDL_GetTicks();
			if (paused) {
				startPauseTime = SDL_GetTicks();
				renderNextFrame(0, NULL);
			}
			break;
		case SDL_SCANCODE_F:
			if (ctrl) toggleFullScreen();
			break;
		default:
			break;
	}
}

void onKeyRelease(SDL_KeyboardEvent &key) {
	switch (key.keysym.scancode) {
#ifdef __APPLE__
		case SDL_SCANCODE_LGUI:
		case SDL_SCANCODE_RGUI:
#else
		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_RCTRL:
#endif
			ctrl = false;
			break;
		default:
			break;
	}
}

string getExecutablePath(string exe) {
	int sep = exe.rfind('/');
	return exe.substr(0, sep+1);
}

void toggleFullScreen() {
    bool IsFullscreen = SDL_GetWindowFlags(mainwindow) & SDL_WINDOW_FULLSCREEN;
    SDL_SetWindowFullscreen(mainwindow, IsFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor(IsFullscreen);
}

void exitFullScreen() {
	bool IsFullscreen = SDL_GetWindowFlags(mainwindow) & SDL_WINDOW_FULLSCREEN;
	if (IsFullscreen) {
    	SDL_SetWindowFullscreen(mainwindow, 0);
    	SDL_ShowCursor(true);
	}
}

void resetGuiTimeout() {
	if (guiTimer != 0) SDL_RemoveTimer(guiTimer);
	guiTimer = SDL_AddTimer(2500, hideGui, NULL);
	guiT = 0;
}

void finishAndQuit() {
	SDL_Quit();
	exit(0);
}

void SDL_Die(const char *msg) {
	printf("%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}

void SDL_MainLoop() {
    SDL_Event event;
    bool draw;
	while (true) {
		draw = false;
		SDL_WaitEvent(&event);
		do {
			switch (event.type) {
				case SDL_KEYDOWN:
					renderer->setGuiOpacity(1.0);
					resetGuiTimeout();
					onKeyPress(event.key);
					draw = true;
					break;
				case SDL_KEYUP:
					renderer->setGuiOpacity(1.0);
					resetGuiTimeout();
					onKeyRelease(event.key);
					draw = true;
					break;
				case SDL_MOUSEMOTION:
					renderer->setGuiOpacity(1.0);
					resetGuiTimeout();
					draw = true;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							onResize(event.window.data1, event.window.data2);
							draw = true;
							break;
						default:
							break;
					}
					break;
				case SDL_USEREVENT:
					if (*(unsigned int*)event.user.data1 == NEXTFRAME) {
						if (renderer->hasMoreFrames()) {
							renderer->updateTextures();
							nextFrame();
						}
						else {
							paused = true;
							renderer->setPaused(paused);
						}
						draw = true;
					}
					else if (*(unsigned int*)event.user.data1 == HIDEGUI) {
						guiT = SDL_GetTicks() + 501;
						fadeGui = true;
						draw = true;
					}
					else if (*(unsigned int*)event.user.data1 == IDLE) {
						if (fadeGui) {
							unsigned int t = SDL_GetTicks() + 501 - guiT;
							if (t < 500) {
								renderer->setGuiOpacity(1.0 - ((float)t/500.0));
							}
							else {
								renderer->setGuiOpacity(0.0);
								fadeGui = false;
							}

							draw = true;
						}
					}
					break;
				case SDL_QUIT:
					renderer->close();
					SDL_Quit();
					exit(0);
					break;
				default:
					break;
			}
		} while (SDL_PollEvent(&event));

		if (draw) {
			renderer->render();
			idle();
		}
    }
}
