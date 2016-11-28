#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "d1vConverter.h"

using namespace std;


string imgDir;                  // Input image sequence directory
string imgTemplate;             // Input image sequence template
int start;                      // Input image sequence start number
int increment;                  // Input image sequence increment number
string outVid;                  // Output video file name
unsigned short fps;             // Output video framerate
string exePath;                 // Executable path

bool parseArguments(int argc, char **argv, string *exe, string *inputDir, string *seqTemp, int *start, int *increment, string *outputVideo, unsigned short *framerate);
void getFrameList(string inputDir, string seqTemp, int start, int increment, vector<string> *frameList);
string getExecutablePath(string exe);
string basename(string path, bool ext = true);

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("\n");
		printf("  Usage: d1vconverter [options]\n");
		printf("\n");
		printf("  Options:\n");
		printf("\n");
		printf("    -i, --input <FILE SEQUENCE TEMPLATE>  image sequence template (%%d or %%0Nd for)\n");
		printf("    -s, --start <INTEGER>                 number to start image sequence set [Default: 0]\n");
		printf("    -n, --increment <INTEGER>             increment for image sequence set [Default: 1]\n");
		printf("    -o, --output <FILE>                   file to save output D1V video [Default: output.d1v]\n");
		printf("    -r, --framerate <INTEGER>             frames per second to play back video [Default: 24]\n");
		printf("\n");
		return 0;
	}

	if (!parseArguments(argc, argv, &exePath, &imgDir, &imgTemplate, &start, &increment, &outVid, &fps)) return 0;

	struct stat info;
	if (stat(imgDir.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
		fprintf(stderr, "Error: \"%s\" does not exist or cannot be accessed\n", imgDir.c_str());
		return 0;
	}

	vector<string> frameList;
	getFrameList(imgDir, imgTemplate, start, increment, &frameList);

	unsigned int width;
	unsigned int height;
	getImageResolution(frameList[0], &width, &height);
	unsigned char *dxt1Buf = (unsigned char*) malloc((width*height) / 2);
	unsigned char *headerBuf = (unsigned char*) malloc(18);
	headerBuf[0] = 0x2E; // .
	headerBuf[1] = 0x44; // D
	headerBuf[2] = 0x31; // 1
	headerBuf[3] = 0x56; // V
	writeUint32(headerBuf,  4, width);
	writeUint32(headerBuf,  8, height);
	writeUint32(headerBuf, 12, frameList.size());
	writeUint16(headerBuf, 16, fps);

	int i;
	char frameIdx[7];
	FILE *wf = fopen(outVid.c_str(), "wb");
	fwrite(headerBuf, 1, 18, wf);
	for (i=0; i<frameList.size(); i++) {
		convertImageToDXT1(frameList[i], width, height, &dxt1Buf);
		fwrite(dxt1Buf, 1, (width*height) / 2, wf);
	}
	fclose(wf);
	
	return 0;
}

bool parseArguments(int argc, char **argv, string *exe, string *inputDir, string *seqTemp, int *start, int *increment, string *outputVideo, unsigned short *framerate) {
	*exe = getExecutablePath(argv[0]);
	*start = 0;
	*increment = 1;
	*outputVideo = "output.d1v";
	*framerate = 24;
	bool hasInput = false;

	if (argc >= 3) {
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--input") == 0) {
			*inputDir = argv[2];
			hasInput = true;
		}
		else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--start") == 0) {
			*start = atoi(argv[2]);
		}
		else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--increment") == 0) {
			*increment = atoi(argv[2]);
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
		else if (strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--start") == 0) {
			*start = atoi(argv[4]);
		}
		else if (strcmp(argv[3], "-n") == 0 || strcmp(argv[3], "--increment") == 0) {
			*increment = atoi(argv[4]);
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
		else if (strcmp(argv[5], "-s") == 0 || strcmp(argv[5], "--start") == 0) {
			*start = atoi(argv[6]);
		}
		else if (strcmp(argv[5], "-n") == 0 || strcmp(argv[5], "--increment") == 0) {
			*increment = atoi(argv[6]);
		}
		else if (strcmp(argv[5], "-o") == 0 || strcmp(argv[5], "--output") == 0) {
			*outputVideo = argv[6];
		}
		else if (strcmp(argv[5], "-r") == 0 || strcmp(argv[5], "--framerate") == 0) {
			int fr = atoi(argv[6]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	if (argc >= 9) {
		if (strcmp(argv[7], "-i") == 0 || strcmp(argv[7], "--input") == 0) {
			*inputDir = argv[8];
			hasInput = true;
		}
		else if (strcmp(argv[7], "-s") == 0 || strcmp(argv[7], "--start") == 0) {
			*start = atoi(argv[8]);
		}
		else if (strcmp(argv[7], "-n") == 0 || strcmp(argv[7], "--increment") == 0) {
			*increment = atoi(argv[8]);
		}
		else if (strcmp(argv[7], "-o") == 0 || strcmp(argv[7], "--output") == 0) {
			*outputVideo = argv[8];
		}
		else if (strcmp(argv[7], "-r") == 0 || strcmp(argv[7], "--framerate") == 0) {
			int fr = atoi(argv[8]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	if (argc >= 11) {
		if (strcmp(argv[9], "-i") == 0 || strcmp(argv[9], "--input") == 0) {
			*inputDir = argv[10];
			hasInput = true;
		}
		else if (strcmp(argv[9], "-s") == 0 || strcmp(argv[9], "--start") == 0) {
			*start = atoi(argv[10]);
		}
		else if (strcmp(argv[9], "-n") == 0 || strcmp(argv[9], "--increment") == 0) {
			*increment = atoi(argv[10]);
		}
		else if (strcmp(argv[9], "-o") == 0 || strcmp(argv[9], "--output") == 0) {
			*outputVideo = argv[10];
		}
		else if (strcmp(argv[9], "-r") == 0 || strcmp(argv[9], "--framerate") == 0) {
			int fr = atoi(argv[10]);
			if (fr >= 1 && fr <= 65535) *framerate = (unsigned short)fr;
		}
	}
	
	if (!hasInput) {
		printf("please specify an input image file\n");
		return false;
	}


	int sep = (*inputDir).rfind('/');
	*seqTemp = (*inputDir).substr(sep+1);
	*inputDir = (*inputDir).substr(0, sep+1);

	/*
	if ((*inputDir)[(*inputDir).length()-1] != '/')
		*inputDir += "/";
	*/
	return true;
}

void getFrameList(string inputDir, string seqTemp, int start, int increment, vector<string> *frameList) {
	int i = 0;
	int pos;
	int padding;
	int frameIdx;
	char frameCStr[12];
	string frameStr;
	string imgBase;
	string imgEnd;
	string tmp;
	string nextImage;
	struct stat info;

	pos = seqTemp.find("%d");
	if (pos != string::npos) {
		padding = 0;
	}
	else {
		pos = seqTemp.find("%0");
		if (pos != string::npos) {
			padding = atoi(seqTemp.substr(pos+1).c_str());
		}
		else {
			fprintf(stderr, "Error: must specify image sequence template (%%d or %%0Nd)");
		}
	}

	imgBase = seqTemp.substr(0, pos);
	tmp = seqTemp.substr(pos);
	imgEnd = tmp.substr(tmp.find('d') + 1);

	do {
		frameIdx = start + (i * increment);
		sprintf(frameCStr, "%d", frameIdx);
		if (padding > 0) {
			tmp = string(padding, '0').append(frameCStr);
			frameStr = tmp.substr(tmp.length() - padding);
		}
		else {
			frameStr = frameCStr;
		}
		nextImage = inputDir + imgBase + frameStr + imgEnd;
		frameList->push_back(nextImage);
		printf("img: %s\n", nextImage.c_str());
		i++;
	} while (stat(nextImage.c_str(), &info) == 0 && !(info.st_mode & S_IFDIR));

	frameList->pop_back();
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
