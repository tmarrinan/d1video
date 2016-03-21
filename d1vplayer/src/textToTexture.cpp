#include "textToTexture.h"

using namespace std;

textToTexture::textToTexture() {
	if(FT_Init_FreeType(&ft)) {
		printf("Error: could not init freetype library\n");
	}
}

void textToTexture::openFont(string fontname, unsigned int size, FT_Face *face) {
	if(FT_New_Face(ft, fontname.c_str(), 0, face)) {
		printf("Error: could not open font\n");
		return;
	}

	FT_Set_Pixel_Sizes(*face, 0, size);
}

void textToTexture::renderText(FT_Face face, std::string text, unsigned char color[3], unsigned int *width, unsigned int *height, unsigned char **pixels) {
	int i, j, k;
	vector<charGlyph> glyphs;

	*width = 0;
	int bottom = 0;
	int top = 0;
	for (i=0; i<text.length(); i++) {
		if(FT_Load_Char(face, text[i], FT_LOAD_RENDER))
			continue;

		glyphs.push_back(charGlyph());
		int c = glyphs.size() - 1;
		glyphs[c].width = face->glyph->bitmap.width;
		glyphs[c].height = face->glyph->bitmap.rows;
		glyphs[c].pixels = (unsigned char *)malloc(glyphs[c].width * glyphs[c].height);
		memcpy(glyphs[c].pixels, face->glyph->bitmap.buffer, glyphs[c].width * glyphs[c].height);
		glyphs[c].left = face->glyph->bitmap_left;
		glyphs[c].top = face->glyph->bitmap_top;
		glyphs[c].advanceX = face->glyph->advance.x;

		*width += glyphs[c].advanceX / 64;
		bottom = (glyphs[c].height - glyphs[c].top) > bottom ? (glyphs[c].height - glyphs[c].top) : bottom;
		top = (glyphs[c].top) > top ? (glyphs[c].top) : top;
	}
	*height = top + bottom;

	int size = (*width) * (*height) * 4;
	*pixels = new unsigned char[size];
	for (i=0; i<size; i++) {
		(*pixels)[i] = 0;
	}

	int x = 0;
	int pt;
	for (i=0; i<glyphs.size(); i++) {
		for (j=0; j<glyphs[i].height; j++) {
			for (k=0; k<glyphs[i].width; k++) {
				pt = (*height) - (bottom + glyphs[i].top);
				(*pixels)[4 * (*width) * (pt + j) + 4 * (glyphs[i].left + x + k) + 0] = color[0];
				(*pixels)[4 * (*width) * (pt + j) + 4 * (glyphs[i].left + x + k) + 1] = color[1];
				(*pixels)[4 * (*width) * (pt + j) + 4 * (glyphs[i].left + x + k) + 2] = color[2];
				(*pixels)[4 * (*width) * (pt + j) + 4 * (glyphs[i].left + x + k) + 3] = glyphs[i].pixels[glyphs[i].width * j + k];
			}
		}
		x += glyphs[i].advanceX / 64;
		free(glyphs[i].pixels);
	}
}
