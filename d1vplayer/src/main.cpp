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

enum guiButton {NONE, PLAY_PAUSE, TIME_KNOB, REWIND, LOOP};

SDL_Window *mainwindow;         // Window handle
SDL_GLContext maincontext;      // OpenGL context handle
unsigned int winW;              // Window Width
unsigned int winH;              // Window Height
d1vPlayer *renderer;            // Renderer
string d1vFile;                 // Input dxt1 video
string exePath;                 // Executable path
bool showGui;                   // Whether or not to show the gui
unsigned int framecount;        // Current frame being rendered
unsigned int framerate;         // Playback framerate (milliseconds per frame)
bool paused;                    // Whether or not video is paused
bool ctrl;                      // Whether or not user is holding 'ctrl' key ('cmd' for Mac OS X)
unsigned int startTime;         // Time used for proper animation
unsigned int startPauseTime;    // Time used for offset when video is paused
SDL_TimerID animationTimer;     // Animation timer
SDL_TimerID guiTimer;           // GUI timer
unsigned int guiT;              // GUI time counter
bool fadeGui;                   // Whether or not the gui is currently fading out
bool loop;                      // Whether or not to loop the video
guiButton mousePressBtn;        // Gui button that mouse pressed on

void parseArguments(int argc, char **argv, string *exe, string *inputFile, bool *gui);
void idle();
void nextFrame();
unsigned int renderNextFrame(unsigned int interval, void *param);
void onResize(unsigned int w, unsigned int h);
void onKeyPress(SDL_KeyboardEvent &key);
void onKeyRelease(SDL_KeyboardEvent &key);
void onMousePress(SDL_MouseButtonEvent &mouse);
void onMouseRelease(SDL_MouseButtonEvent &mouse);
string getExecutablePath(string exe);
guiButton findGuiButtonAtPoint(unsigned int x, unsigned int y);
void toggleFullScreen();
void exitFullScreen();
void toggleLoop();
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
		printf("    -n, --no-gui            open player without graphic ui\n");
		printf("\n");
		return 0;
	}

	parseArguments(argc, argv, &exePath, &d1vFile, &showGui);
	paused = false;
	ctrl = false;
	guiTimer = 0;
	fadeGui = false;
	loop = false;
	mousePressBtn = NONE;

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
	winW = 1280;
	winH = 720;
	mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winW, winH, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!mainwindow)
		SDL_Die("Unable to create window");

	maincontext = SDL_GL_CreateContext(mainwindow);

	const unsigned char* glVersion = glGetString(GL_VERSION);
	const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Using OpenGL %s, GLSL %s\n", glVersion, glslVersion);

	renderer = new d1vPlayer(mainwindow, exePath);
	renderer->initGL(d1vFile, winW, winH, showGui);
	framecount = 0;
	framerate = (unsigned int)(1000.0 / (double)renderer->getPlaybackFps());
	startTime = SDL_GetTicks();
	renderer->render();
	nextFrame();

	SDL_MainLoop();

	return 0;
}

void parseArguments(int argc, char **argv, string *exe, string *inputFile, bool *gui) {
	*exe = getExecutablePath(argv[0]);
	bool hasInput = false;
	*gui = true;

	if (argc >= 3) {
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--input") == 0) {
			*inputFile = argv[2];
			hasInput = true;
		}
		else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--no-gui") == 0) {
			*gui = false;
		}
	}
	if (argc >= 4) {
		if (strcmp(argv[2], "-i") == 0 || strcmp(argv[2], "--input") == 0) {
			*inputFile = argv[3];
			hasInput = true;
		}
		else if (strcmp(argv[3], "-n") == 0 || strcmp(argv[3], "--no-gui") == 0) {
			*gui = false;
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
	winW = w;
	winH = h;
	renderer->resize(winW, winH);
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
		case SDL_SCANCODE_L:
			toggleLoop();
			renderer->setLooped(loop);
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

void onMousePress(SDL_MouseButtonEvent &mouse) {
	mousePressBtn = findGuiButtonAtPoint(mouse.x, mouse.y);
}

void onMouseRelease(SDL_MouseButtonEvent &mouse) {
	guiButton mouseReleaseBtn = findGuiButtonAtPoint(mouse.x, mouse.y);

	if (mousePressBtn == mouseReleaseBtn) {
		switch (mouseReleaseBtn) {
			case PLAY_PAUSE:
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
			case TIME_KNOB:
				break;
			case REWIND:
				renderer->rewind();
				framecount = 0;
				startTime = SDL_GetTicks();
				if (paused) {
					startPauseTime = SDL_GetTicks();
					renderNextFrame(0, NULL);
				}
				break;
			case LOOP:
				toggleLoop();
				renderer->setLooped(loop);
				break;
			default:
				break;
		}
	}
}

string getExecutablePath(string exe) {
	int sep = exe.rfind('/');
	return exe.substr(0, sep+1);
}

guiButton findGuiButtonAtPoint(unsigned int x, unsigned int y) {
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
	double scaledX = 2.0 * ((double)(x - viewX) / (double)viewW) - 1.0;
	double scaledY = 2.0 * ((double)((winH - y) - viewY) / (double)viewH) - 1.0;

	if (scaledX >= -0.48 && scaledX <= -0.42 && scaledY >= -0.73 && scaledY <= -0.67) return PLAY_PAUSE;
	if (scaledX >= -0.38 && scaledX <=  0.08 && scaledY >= -0.72 && scaledY <= -0.68) return TIME_KNOB;
	if (scaledX >=  0.34 && scaledX <=  0.40 && scaledY >= -0.73 && scaledY <= -0.67) return REWIND;
	if (scaledX >=  0.42 && scaledX <=  0.48 && scaledY >= -0.73 && scaledY <= -0.67) return LOOP;

	return NONE;
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

void toggleLoop() {
	loop = !loop;
}

void resetGuiTimeout() {
	if (!showGui) return;
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
				case SDL_MOUSEBUTTONDOWN:
					onMousePress(event.button);
					break;
				case SDL_MOUSEBUTTONUP:
					onMouseRelease(event.button);
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
							if (loop) {
								renderer->rewind();
								framecount = 0;
								startTime = SDL_GetTicks();
								nextFrame();
							}
							else {
								paused = true;
								renderer->setPaused(paused);
							}
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
			if (fadeGui) idle();
		}
    }
}
