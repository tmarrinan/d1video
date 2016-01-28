#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>

#include "d1vConverter.h"

using namespace std;


string imgDir;                  // Input image sequence directory
string outVid;                  // Output video file name
unsigned short fps;             // Output video framerate
string exePath;                 // Executable path

bool parseArguments(int argc, char **argv, string *exe, string *inputDir, string *outputVideo, unsigned short *framerate);
string findFileExtension(string inputDir);
unsigned int findNumberOfFrames(string inputDir, string ext);
string getExecutablePath(string exe);
string basename(string path, bool ext = true);

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("\n");
		printf("  Usage: d1vconverter [options]\n");
		printf("\n");
		printf("  Options:\n");
		printf("\n");
		printf("    -i, --input <DIRECTORY>    directory with image set sequence\n");
		printf("    -o, --output <FILE>        file to save output D1V video [Default: output.d1v]\n");
		printf("    -r, --framerate <INTEGER>  frames per second to play back video [Default: 24]\n");
		printf("\n");
		return 0;
	}

	if (!parseArguments(argc, argv, &exePath, &imgDir, &outVid, &fps)) return 0;

	struct stat info;
	if (stat(imgDir.c_str(), &info) != 0) {
		printf("\"%s\" does not exist or cannot be accessed, please specify an image file\n", imgDir.c_str());
		return 0;
	}
	else if (!(info.st_mode & S_IFDIR)) {
		printf("\"%s\" is not a directory, please specify a directory with image files\n", imgDir.c_str());
		return 0;
	}

	string fileExt = findFileExtension(imgDir);
	if (fileExt == "") {
		printf("cannot find frame images in specified directory\n");
		return 0;
	}
	unsigned int frameCount = findNumberOfFrames(imgDir, fileExt);

	unsigned int width;
	unsigned int height;
	getImageResolution(imgDir, fileExt, &width, &height);
	unsigned char *dxt1Buf = (unsigned char*) malloc((width*height) / 2);
	unsigned char *headerBuf = (unsigned char*) malloc(18);
	headerBuf[0] = 0x2E; // .
	headerBuf[1] = 0x44; // D
	headerBuf[2] = 0x31; // 1
	headerBuf[3] = 0x56; // V
	writeUint32(headerBuf,  4, width);
	writeUint32(headerBuf,  8, height);
	writeUint32(headerBuf, 12, frameCount);
	writeUint16(headerBuf, 16, fps);

	int i;
	char frameIdx[7];
	FILE *wf = fopen(outVid.c_str(), "wb");
	fwrite(headerBuf, 1, 18, wf);
	for (i=0; i<frameCount; i++) {
		sprintf(frameIdx, "%06d", i);
		convertImageToDXT1(imgDir + "frame_" + frameIdx + "." + fileExt, width, height, &dxt1Buf);
		fwrite(dxt1Buf, 1, (width*height) / 2, wf);
	}
	fclose(wf);
	
	return 0;
}

bool parseArguments(int argc, char **argv, string *exe, string *inputDir, string *outputVideo, unsigned short *framerate) {
	*exe = getExecutablePath(argv[0]);
	*outputVideo = "output.d1v";
	*framerate = 24;
	bool hasInput = false;

	if (argc >= 3) {
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--input") == 0) {
			*inputDir = argv[2];
			hasInput = true;
		}
		else if (strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--output") == 0) {
			*outputVideo = argv[2];
		}
		else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--framerate") == 0) {
			int fr = atoi(argv[2]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	if (argc >= 5) {
		if (strcmp(argv[3], "-i") == 0 || strcmp(argv[3], "--input") == 0) {
			*inputDir = argv[4];
			hasInput = true;
		}
		else if (strcmp(argv[3], "-o") == 0 || strcmp(argv[3], "--output") == 0) {
			*outputVideo = argv[4];
		}
		else if (strcmp(argv[3], "-r") == 0 || strcmp(argv[3], "--framerate") == 0) {
			int fr = atoi(argv[4]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	if (argc >= 7) {
		if (strcmp(argv[5], "-i") == 0 || strcmp(argv[5], "--input") == 0) {
			*inputDir = argv[6];
			hasInput = true;
		}
		else if (strcmp(argv[5], "-o") == 0 || strcmp(argv[5], "--output") == 0) {
			*outputVideo = argv[6];
		}
		else if (strcmp(argv[5], "-r") == 0 || strcmp(argv[5], "--framerate") == 0) {
			int fr = atoi(argv[6]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	
	if (!hasInput) {
		printf("please specify an input image file\n");
		return false;
	}

	if ((*inputDir)[(*inputDir).length()-1] != '/')
		*inputDir += "/";

	return true;
}

string findFileExtension(string inputDir) {
	string ext = "";

	struct stat info;
	if (stat((inputDir + "frame_000000.jpg").c_str(), &info) == 0 && !(info.st_mode & S_IFDIR)) {
		ext = "jpg";
	}
	else if (stat((inputDir + "frame_000000.jpeg").c_str(), &info) == 0 && !(info.st_mode & S_IFDIR)) {
		ext = "jpeg";
	}
	else if (stat((inputDir + "frame_000000.png").c_str(), &info) == 0 && !(info.st_mode & S_IFDIR)) {
		ext = "png";
	}

	return ext;
}

unsigned int findNumberOfFrames(string inputDir, string ext) {
	unsigned int i = 0;
	char frameIdx[7];
	string nextImage;
	struct stat info;

	do {
		i++;
		sprintf(frameIdx, "%06u", i);
		nextImage = inputDir + "frame_" + frameIdx + "." + ext;
	} while(stat(nextImage.c_str(), &info) == 0 && !(info.st_mode & S_IFDIR));

	return i;
}

string getExecutablePath(string exe) {
	int sep = exe.rfind('/');
	return exe.substr(0, sep+1);
}

string basename(string path, bool ext) {
	size_t sep = path.rfind('/', path.length()-2);
	size_t dot = path.rfind('.');
	size_t length = ext ? string::npos : dot - (sep+1);
	if (path[path.length()-1] == '/')
		length = (path.length()-1) - (sep+1);
	return path.substr(sep+1, length);
}
