#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>

#include <GL/glew.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/freeglut.h>
#endif

#include "d1vPlayer.h"

#define PROGRAM_NAME "D1V Video Player"

using namespace std;


unsigned int IDLE = 0;
unsigned int NEXTFRAME = 1;
unsigned int HIDEGUI = 2;

enum guiButton {NONE, PLAY_PAUSE, TIME_KNOB, REWIND, LOOP};

unsigned int winW;              // Window Width
unsigned int winH;              // Window Height
unsigned int prevWinW;          // Previous Window Width
unsigned int prevWinH;          // Previous Window Height
unsigned int prevWinX;          // Previous Window Position X
unsigned int prevWinY;          // Previous Window Position Y
d1vPlayer *renderer;            // Renderer
string d1vFile;                 // Input dxt1 video
string exePath;                 // Executable path
bool showGui;                   // Whether or not to show the gui
unsigned int framecount;        // Current frame being rendered
unsigned int framerate;         // Playback framerate (milliseconds per frame)
bool paused;                    // Whether or not video is paused
unsigned int startTime;         // Time used for proper animation
unsigned int guiTimer;          // GUI timer
unsigned int guiT;              // GUI time counter
bool fullscreen;                // Whether or not application is fullscreen
bool fadeGui;                   // Whether or not the gui is currently fading out
bool loop;                      // Whether or not to loop the video
bool lockTimeKnob;              // Whether or not time slider knob is selected
bool wasPlaying;                // Whether or not video was playing when slider knob was selected
bool advanceVideo;              // Whether or not to grab next frame;
guiButton mousePressBtn;        // Gui button that mouse pressed on

void parseArguments(int argc, char **argv, string *exe, string *inputFile, bool *gui);
string getExecutablePath(string exe);
void display();
void reshape(int w, int h);
void idle();
void keypress(unsigned char key, int x, int y);
void specialkeypress(int key, int x, int y);
void mousepressrelease(int button, int state, int x, int y);
void mousemove(int x, int y);
void nextFrame();
void renderNextFrame(int param);
void toggleFullScreen();
void exitFullScreen();
void toggleLoop();
void resetGuiTimeout();
void hideGui(int param);
guiButton findGuiButtonAtPoint(unsigned int x, unsigned int y, double *data);


int main(int argc, char **argv) {
	// Initialize main program
	if (argc < 3) {
		printf("\n");
		printf("  Usage: d1vplayer [options]\n");
		printf("\n");
		printf("  Options:\n");
		printf("\n");
		printf("    -i, --input <FILE>      dxt1 compressed video to show\n");
		printf("    -n, --no-gui            open player without graphic ui\n");
		printf("\n");
		return 0;
	}

	parseArguments(argc, argv, &exePath, &d1vFile, &showGui);
	fullscreen = false;
	paused = false;
	//guiTimer = 0;
	fadeGui = false;
	loop = false;
	lockTimeKnob = false;
	wasPlaying = true;
	advanceVideo = true;
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

	// Initialize GLUT
	glutInit(&argc, argv);

#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	winW = 1280;
	winH = 720;
	glutCreateWindow(PROGRAM_NAME);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(specialkeypress);
	glutMouseFunc(mousepressrelease);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(mousemove);
	glutReshapeWindow(winW, winH);

	glewExperimental = GL_TRUE;
	int GlewInitResult = glewInit();
	int glewErr = glGetError();
	if (GlewInitResult != GLEW_OK) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
	}

	const unsigned char* glVersion = glGetString(GL_VERSION);
	const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Using OpenGL %s, GLSL %s\n", glVersion, glslVersion);

	renderer = new d1vPlayer(exePath);
	renderer->initGL(d1vFile, showGui);
	framecount = 0;
	framerate = (unsigned int)(1000.0 / (double)renderer->getPlaybackFps());
	startTime = glutGet(GLUT_ELAPSED_TIME);
	resetGuiTimeout();

	glutMainLoop();

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
		exit(0);
	}
}

string getExecutablePath(string exe) {
	int sep = exe.rfind('/');
	return exe.substr(0, sep+1);
}

void display() {
	renderer->render();

	if (advanceVideo)
		nextFrame();
}

void reshape(int w, int h) {
	winW = w;
	winH = h;
	renderer->resize();
}

void idle() {
	if (fadeGui) {
		unsigned int t = glutGet(GLUT_ELAPSED_TIME) + 501 - guiT;
		if (t < 500) {
			renderer->setGuiOpacity(1.0 - ((float)t/500.0));
		}
		else {
			renderer->setGuiOpacity(0.0);
			fadeGui = false;
		}

		glutPostRedisplay();
	}
}

void keypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: // esc key
			exitFullScreen();
			break;
		case ' ':
			paused = !paused;
			renderer->setPaused(paused);
			if (!paused) {
				framecount = 0;
				startTime = glutGet(GLUT_ELAPSED_TIME);
				renderNextFrame(0);
			}
			break;
		case 'F':
			toggleFullScreen();
			break;
		case 'l':
			toggleLoop();
			renderer->setLooped(loop);
			break;
		default:
			break;
	}
}

void specialkeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			renderer->rewind();
			framecount = 0;
			startTime = glutGet(GLUT_ELAPSED_TIME);
			if (paused)
				renderNextFrame(0);
			break;
		default:
			break;
	}
}

void mousepressrelease(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN){ // left click press
			double extra;
			mousePressBtn = findGuiButtonAtPoint(x, y, &extra);

			if (mousePressBtn == TIME_KNOB) {
				wasPlaying = !paused;
				if (!paused) {
					paused = true;
				}
				lockTimeKnob = true;
			}
		}
		else { // left click release
			double extra;
			guiButton mouseReleaseBtn = findGuiButtonAtPoint(x, y, &extra);

			if (lockTimeKnob) {
				renderer->setVideoTime(extra);
				if (wasPlaying) {
					paused = false;
					framecount = 0;
					startTime = glutGet(GLUT_ELAPSED_TIME);
					nextFrame();
				}
				else {
					renderNextFrame(0);
				}
				lockTimeKnob = false;
				return;
			}

			if (mousePressBtn == mouseReleaseBtn) {
				switch (mouseReleaseBtn) {
					case PLAY_PAUSE:
						paused = !paused;
						renderer->setPaused(paused);
						if (!paused) {
							framecount = 0;
							startTime = glutGet(GLUT_ELAPSED_TIME);
							renderNextFrame(0);
						}
						break;
					case TIME_KNOB:
						break;
					case REWIND:
						renderer->rewind();
						framecount = 0;
						startTime = glutGet(GLUT_ELAPSED_TIME);
						if (paused) {
							renderNextFrame(0);
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
	}

	glutPostRedisplay();
}

void mousemove(int x, int y) {
	renderer->setGuiOpacity(1.0);
	resetGuiTimeout();
	if (lockTimeKnob) {
		double extra;
		guiButton btn = findGuiButtonAtPoint(x, y, &extra);
		renderer->setVideoTime(extra);
	}

	glutPostRedisplay();
}

void nextFrame() {
	framecount++;
	if (paused) return;

	unsigned int now;
	now = glutGet(GLUT_ELAPSED_TIME);
	unsigned int t = now - startTime;

	advanceVideo = false;

	if (t >= framerate * framecount) {
		renderNextFrame(0);
	}
	else {
		glutTimerFunc((framerate * framecount) - t, renderNextFrame, 0);
	}
}

void renderNextFrame(int param) {
	if (renderer->hasMoreFrames()) {
		renderer->updateTextures();
	}
	else {
		if (loop) {
			renderer->rewind();
			framecount = 0;
			startTime = glutGet(GLUT_ELAPSED_TIME);
		}
		else {
			paused = true;
			renderer->setPaused(paused);
		}
	}

	advanceVideo = true;
	glutPostRedisplay();
}

void toggleFullScreen() {
	if (fullscreen) {
		glutReshapeWindow(prevWinW, prevWinH);
		glutPositionWindow(prevWinX, prevWinY);
	}
	else {
		prevWinW = glutGet(GLUT_WINDOW_WIDTH);
		prevWinH = glutGet(GLUT_WINDOW_HEIGHT);
		prevWinX = glutGet(GLUT_WINDOW_X);
		prevWinY = glutGet(GLUT_WINDOW_Y);
		glutFullScreen();
	}
	fullscreen = !fullscreen;
}

void exitFullScreen() {
	if (fullscreen) {
		glutReshapeWindow(prevWinW, prevWinH);
		glutPositionWindow(prevWinX, prevWinY);
	}
	fullscreen = false;
}

void toggleLoop() {
	loop = !loop;
}

void resetGuiTimeout() {
	if (!showGui) return;

	fadeGui = false;
	guiTimer = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(2500, hideGui, guiTimer);
	guiT = 0;
}

void hideGui(int param) {
	if (param != guiTimer)
		return;

	guiT = glutGet(GLUT_ELAPSED_TIME) + 501;
	fadeGui = true;

	glutPostRedisplay();
}

guiButton findGuiButtonAtPoint(unsigned int x, unsigned int y, double *data) {
	if (!showGui) return NONE;

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

	double timep = (scaledX + 0.39) / 0.46;
	if (timep < 0.0) timep = 0.0;
	if (timep > 1.0) timep = 1.0;
	*data = timep;

	if (scaledX >= -0.48 && scaledX <= -0.42 && scaledY >= -0.73 && scaledY <= -0.67) return PLAY_PAUSE;
	if (scaledX >= -0.39 && scaledX <=  0.07 && scaledY >= -0.72 && scaledY <= -0.68) return TIME_KNOB;
	if (scaledX >=  0.34 && scaledX <=  0.40 && scaledY >= -0.73 && scaledY <= -0.67) return REWIND;
	if (scaledX >=  0.42 && scaledX <=  0.48 && scaledY >= -0.73 && scaledY <= -0.67) return LOOP;

	return NONE;
}
