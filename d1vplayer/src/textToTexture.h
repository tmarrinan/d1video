#ifndef TEXTTOTEXTURE_H
#define TEXTTOTEXTURE_H

#include <string>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

struct charGlyph {
	unsigned int width;
	unsigned int height;
	unsigned char *pixels;
	int left;
	int top;
	int advanceX;
};

class textToTexture {
private:
	FT_Library ft;

public:
	textToTexture();
	void openFont(std::string fontname, unsigned int size, FT_Face *face);
	void renderText(FT_Face face, std::string text, unsigned char color[3], unsigned int *width, unsigned int *height, unsigned char **pixels);
};

#endif // TEXTTOTEXTURE_H
