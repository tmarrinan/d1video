#include "d1vConverter.h"

using namespace std;


void getImageResolution(std::string directory, std::string ext, unsigned int *width, unsigned int *height) {
	unsigned int origW;
	unsigned int origH;

	if (ext == "jpeg" || ext == "jpg") {
		getJPEGResolution(directory + "frame_000000." + ext, &origW, &origH);
	}
	else if (ext == "png") {
		getPNGResolution(directory + "frame_000000." + ext, &origW, &origH);
	}

	*width  = origW - (origW%4);
	*height = origH - (origH%4);
}

void convertImageToDXT1(string filename, unsigned int width, unsigned int height, unsigned char **data) {
	size_t pos = filename.rfind('.');
	if (pos == string::npos) return;

	string ext = filename.substr(pos+1, string::npos);
	
	unsigned int inputW;
	unsigned int inputH;
	rawFormat inputFmt;
	unsigned char *inputData;

	// read file into raw buffer (rgb or rgba)
	if (ext == "jpeg" || ext == "jpg") {
		loadFromJPEG(filename, &inputW, &inputH, &inputFmt, &inputData);
	}
	else if (ext == "png") {
		loadFromPNG(filename, &inputW, &inputH, &inputFmt, &inputData);
	}
	else {
		printf("Please specify a JPEG or PNG image\n");
		return;
	}

	// convert raw buffer to dxt1 compressed buffer
	if (width  != (inputW - (inputW%4)) || height != (inputH - (inputH%4))) {
		printf("Error: images do not all have same resolution\n");
		return;
	}

	int i, j;
	unsigned int offset;
	unsigned int currentPos = 0;
	unsigned char *block    = (unsigned char*) malloc(64);
	unsigned char *colorMin = (unsigned char*) malloc( 3);
	unsigned char *colorMax = (unsigned char*) malloc( 3);
	unsigned char *inset    = (unsigned char*) malloc( 3);
	unsigned char *colors   = (unsigned char*) malloc(16);
	unsigned char *indices  = (unsigned char*) malloc(16);
	for (j=height-4; j>=0; j-=4) {
		for (i=0; i<width; i+=4) {
			if (inputFmt == RGB) offset = inputW*3*j + i*3;
			else                 offset = inputW*4*j + i*4;
			
			extractBlock(offset, inputData, inputW, inputFmt, block);
			getMinMaxColors(block, colorMin, colorMax, inset);
			
			writeUint16(*data, currentPos,   colorTo565(colorMax));
			writeUint16(*data, currentPos+2, colorTo565(colorMin));
			writeUint32(*data, currentPos+4, colorIndices(block, colorMin, colorMax, colors, indices));

			currentPos += 8;
		}
	}
}

void getJPEGResolution(string filename, unsigned int *width, unsigned int *height) {
	FILE *f;
	int i, depth;

	f = fopen(filename.c_str(), "rb");

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, 0);
	cinfo.scale_num = 1;
	cinfo.scale_denom = 1;
	jpeg_start_decompress(&cinfo);
	
	*width = cinfo.output_width;
	*height = cinfo.output_height;

	fclose(f);
	//jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

void loadFromJPEG(string filename, unsigned int *width, unsigned int *height, rawFormat *format, unsigned char **data) {
	FILE *f;
	int i, depth;

	f = fopen(filename.c_str(), "rb");

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	unsigned long location = 0;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, 0);
	cinfo.scale_num = 1;
	cinfo.scale_denom = 1;
	jpeg_start_decompress(&cinfo);
	
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	depth = cinfo.num_components; //should always be 3
	if (depth == 3) *format = RGB;
	else *format = RGBA;

	*data = (unsigned char *) malloc((*width) * (*height) * depth);
	row_pointer[0] = (unsigned char *) malloc((*width) * depth);
	
	// read one scan line at a time
	while (cinfo.output_scanline < cinfo.output_height)	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i=0; i<((*width) * depth); i++)
			(*data)[location++] = row_pointer[0][i];
	}
	fclose(f);
	free(row_pointer[0]);
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

void getPNGResolution(string filename, unsigned int *width, unsigned int *height) {
	FILE *f;

	f = fopen(filename.c_str(), "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return;

	png_infop info = png_create_info_struct(png);
	if (!info) return;

	if (setjmp(png_jmpbuf(png))) return;

	png_init_io(png, f);
	png_read_info(png, info);

	*width = png_get_image_width(png, info);
	*height = png_get_image_height(png, info);

	png_destroy_read_struct(&png, &info, NULL);
	fclose(f);
}

void loadFromPNG(string filename, unsigned int *width, unsigned int *height, rawFormat *format, unsigned char **data) {
	FILE *f;
	int i;

	f = fopen(filename.c_str(), "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return;

	png_infop info = png_create_info_struct(png);
	if (!info) return;

	if (setjmp(png_jmpbuf(png))) return;

	png_init_io(png, f);
	png_read_info(png, info);

	*width = png_get_image_width(png, info);
	*height = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	*format = RGBA;

	*data = (unsigned char*) malloc((*width) * (*height) * 4);
	png_bytep *row_pointers = (png_bytep*) malloc((*height) * sizeof(png_bytep));
	for (i=0; i<(*height); i++) {
		row_pointers[i] = (png_byte*) (*data)+(i*(*width)*4);
	}

	png_read_image(png, row_pointers);

	png_destroy_read_struct(&png, &info, NULL);
	free(row_pointers);
	fclose(f);
}

void extractBlock(unsigned int offset, unsigned char *inputData, unsigned int inputW, rawFormat inputFmt, unsigned char *outBlock) {
	int i, j, k;
	unsigned int start = offset;
	if (inputFmt == RGB) {
		for (i=3; i>=0; i--) {
			k = 0;
			for (j=0; j<16; j++){
				if (j%4 == 3) { outBlock[i*16+j] = 255; }
				else { outBlock[i*16+j] = inputData[start+k]; k++; }
			}
			start += inputW*3;
		}
	}
	else {
		for (i=3; i>=0; i--) {
			for (j=0; j<16; j++){
				outBlock[i*16+j] = inputData[start+j];
			}
			start += inputW*4;
		}
	}
}

void getMinMaxColors(unsigned char *colorBlock, unsigned char *colorMin, unsigned char *colorMax, unsigned char *inset) {
	int i;
	
	colorMin[0] = 255;
	colorMin[1] = 255;
	colorMin[2] = 255;
	colorMax[0] =   0;
	colorMax[1] =   0;
	colorMax[2] =   0;
	
	for (i=0; i<16; i++) {
		if(colorBlock[i*4]   < colorMin[0]) colorMin[0] = colorBlock[i*4];
		if(colorBlock[i*4+1] < colorMin[1]) colorMin[1] = colorBlock[i*4+1];
		if(colorBlock[i*4+2] < colorMin[2]) colorMin[2] = colorBlock[i*4+2];
		if(colorBlock[i*4]   > colorMax[0]) colorMax[0] = colorBlock[i*4];
		if(colorBlock[i*4+1] > colorMax[1]) colorMax[1] = colorBlock[i*4+1];
		if(colorBlock[i*4+2] > colorMax[2]) colorMax[2] = colorBlock[i*4+2];
	}
	
	inset[0] = (colorMax[0] - colorMin[0]) >> 4;
	inset[1] = (colorMax[1] - colorMin[1]) >> 4;
	inset[2] = (colorMax[2] - colorMin[2]) >> 4;
	
	colorMin[0] = (colorMin[0] + inset[0] <= 255) ? colorMin[0] + inset[0] : 255;
	colorMin[1] = (colorMin[1] + inset[1] <= 255) ? colorMin[1] + inset[1] : 255;
	colorMin[2] = (colorMin[2] + inset[2] <= 255) ? colorMin[2] + inset[2] : 255;
	colorMax[0] = (colorMax[0] - inset[0] >=   0) ? colorMax[0] - inset[0] :   0;
	colorMax[1] = (colorMax[1] - inset[1] >=   0) ? colorMax[1] - inset[1] :   0;
	colorMax[2] = (colorMax[2] - inset[2] >=   0) ? colorMax[2] - inset[2] :   0;
}

unsigned short colorTo565(unsigned char *rgb) {
	return ((rgb[0]>>3) << 11) | ((rgb[1]>>2) << 5) | (rgb[2]>>3);
}

unsigned int colorIndices(unsigned char *colorBlock, unsigned char *colorMin, unsigned char *colorMax, unsigned char *colors, unsigned char *indices) {
	int i, j;

	unsigned char C565_5_MASK = 0xF8;   // 0xFF minus last three bits
	unsigned char C565_6_MASK = 0xFC;   // 0xFF minus last two bits
	
	colors[0]  = (colorMax[0] & C565_5_MASK) | (colorMax[0] >> 5);
	colors[1]  = (colorMax[1] & C565_6_MASK) | (colorMax[1] >> 6);
	colors[2]  = (colorMax[2] & C565_5_MASK) | (colorMax[2] >> 5);
	colors[4]  = (colorMin[0] & C565_5_MASK) | (colorMin[0] >> 5);
	colors[5]  = (colorMin[1] & C565_6_MASK) | (colorMin[1] >> 6);
	colors[6]  = (colorMin[2] & C565_5_MASK) | (colorMin[2] >> 5);
	colors[8]  = (2*colors[0] +   colors[4]) / 3;
	colors[9]  = (2*colors[1] +   colors[5]) / 3;
	colors[10] = (2*colors[2] +   colors[6]) / 3;
	colors[12] = (  colors[0] + 2*colors[4]) / 3;
	colors[13] = (  colors[1] + 2*colors[5]) / 3;
	colors[14] = (  colors[2] + 2*colors[6]) / 3;
	
	for (i=0; i<16; i++) {
		unsigned int minDistance = 195076; // (255 * 255 * 255) + 1
		for (j=0; j<4; j++) {
			unsigned int dist = colorDistance(colorBlock, i*4, colors, j*4);
			if (dist < minDistance) {
				minDistance = dist;
				indices[i] = j;
			}
		}
	}
	
	unsigned int result = 0;
	for(i=0; i<16; i++) {
		result |= (indices[i] << (i<<1));
	}
	
	return result;
}

unsigned int colorDistance(unsigned char *colorBlock1, int c1offset, unsigned char *colorBlock2, int c2offset) {
	int dx = colorBlock1[c1offset]  -colorBlock2[c2offset];
	int dy = colorBlock1[c1offset+1]-colorBlock2[c2offset+1];
	int dz = colorBlock1[c1offset+2]-colorBlock2[c2offset+2];
	
	return (dx*dx) + (dy*dy) + (dz*dz);
}

void writeUint16(unsigned char *buffer, unsigned int offset, unsigned short value) {
	buffer[offset]   = value         & 0xFF;
	buffer[offset+1] = (value >>  8) & 0xFF;
}

void writeUint32(unsigned char *buffer, unsigned int offset, unsigned int value) {
	buffer[offset]   = value         & 0xFF;
	buffer[offset+1] = (value >>  8) & 0xFF;
	buffer[offset+2] = (value >> 16) & 0xFF;
	buffer[offset+3] = (value >> 24) & 0xFF;
}
