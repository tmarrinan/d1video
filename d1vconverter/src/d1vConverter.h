#include <cstdlib>
#include <string>
#include <jpeglib.h>
#include <png.h>

enum rawFormat { RGB, RGBA };

void getImageResolution(std::string directory, std::string ext, unsigned int *width, unsigned int *height);
void convertImageToDXT1(std::string filename, unsigned int width, unsigned int height, unsigned char **data);
void getJPEGResolution(std::string filename, unsigned int *width, unsigned int *height);
void loadFromJPEG(std::string filename, unsigned int *width, unsigned int *height, rawFormat *format, unsigned char **data);
void getPNGResolution(std::string filename, unsigned int *width, unsigned int *height);
void loadFromPNG(std::string filename, unsigned int *width, unsigned int *height, rawFormat *format, unsigned char **data);
void extractBlock(unsigned int offset, unsigned char *inputData, unsigned int inputW, rawFormat inputFmt, unsigned char *outBlock);
void getMinMaxColors(unsigned char *colorBlock, unsigned char *colorMin, unsigned char *colorMax, unsigned char *inset);
unsigned short colorTo565(unsigned char *rgb);
unsigned int colorIndices(unsigned char *colorBlock, unsigned char *colorMin, unsigned char *colorMax, unsigned char *colors, unsigned char *indices);
unsigned int colorDistance(unsigned char *colorBlock1, int c1offset, unsigned char *colorBlock2, int c2offset);
void writeUint16(unsigned char *buffer, unsigned int offset, unsigned short value);
void writeUint32(unsigned char *buffer, unsigned int offset, unsigned int value);
