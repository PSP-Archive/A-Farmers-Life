#include "inc/main.h"

typedef struct
{
	unsigned short u, v;
	short x, y, z;
} FontVertex;

static int getPow2(int width)
{
	int b = width;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * width) b >>= 1;
	return b;
}

Font* Font_Load(const char* filename) {

        FT_Error initError;
        initError = FT_Init_FreeType( &ft_library );
  	if(initError)
          return 0;

	Font* font = (Font*) malloc(sizeof(Font));

	// Cache font
	FILE* fontFile = fopen(filename, "rb");

	if (!fontFile)
        error_callback("Can't open font file.\n");

	fseek(fontFile, 0, SEEK_END);
	int filesize = ftell(fontFile);
	u8* fontData = (u8*) malloc(filesize);
	if (!fontData) {
		fclose(fontFile);
		error_callback("Not enough memory to cache font file.\n");
	}
	rewind(fontFile);
	fread(fontData, filesize, 1, fontFile);
	fclose(fontFile);
	int error = FT_New_Memory_Face(ft_library, fontData, filesize, 0, &font->face);
	if (error) {
		free(font);
		free(fontData);
		error_callback("Error loading font.\n");
	}
	font->data = fontData;
	font->name = malloc(strlen(filename));
	font->name = (char *)filename;
	return font;
}

int Font_Unload(Font* font) {
	FT_Done_Face(font->face);
	free(font->name);
	if (font->data)	free(font->data);
	free(font);
	return 0;
}

int Font_SetSize(Font* font, int height) {
	FT_Set_Pixel_Sizes(font->face, 0, height);
	return 1;
}

void Font_SetAngle(Font *font, double angle)
{
	if(angle >= 360.0 || angle <= -360.0)
		font->FontAngle = 0.0;
	else if(angle < 0.0)
		font->FontAngle = 360.0 - angle;
	else
		font->FontAngle = angle;
}


static void fontPrintTextImpl(FT_Bitmap* bitmap, int xofs, int yofs, u32 color, u32* framebuffer, int width, int height, int lineSize)
{
	u8 rf = color & 0xff;
	u8 gf = (color >> 8) & 0xff;
	u8 bf = (color >> 16) & 0xff;
	u8 af = (color >> 24) & 0xff;
	int x, y;
	
	u8* line = bitmap->buffer;
	u32* fbLine = framebuffer + xofs + yofs * lineSize;
	for (y = 0; y < bitmap->rows; y++) {
		u8* column = line;
		u32* fbColumn = fbLine;
		for (x = 0; x < bitmap->width; x++) {
			if (x + xofs < width && x + xofs >= 0 && y + yofs < height && y + yofs >= 0) {
				u8 val = *column;
				color = *fbColumn;
				u8 r = color & 0xff;
				u8 g = (color >> 8) & 0xff;
				u8 b = (color >> 16) & 0xff;
				u8 a = (color >> 24) & 0xff;
				r = rf * val / 255 + (255 - val) * r / 255;
				g = gf * val / 255 + (255 - val) * g / 255;
				b = bf * val / 255 + (255 - val) * b / 255;
				a = af * val / 255 + (255 - val) * a / 255;
				*fbColumn = r | (g << 8) | (b << 16) | (a << 24);
			}
			column++;
			fbColumn++;
		}
		line += bitmap->pitch;
		fbLine += lineSize;
	}
}

static void fontPrintTextScreen(FT_Bitmap* bitmap, int x, int y, u32 color, u32* framebuffer)
{
	fontPrintTextImpl(bitmap, x, y, color, framebuffer, 480, 272, 512);
}
//GRAPHICS LIB FRAMEBUFFER IS getVramDrawBuffer()

static void fontPrintTextTexture(FT_Bitmap* bitmap, int x, int y, u32 color, FontTexture* fonttexture)
{
	fontPrintTextImpl(bitmap, x, y, color, fonttexture->data, fonttexture->width, fonttexture->height, fonttexture->textureWidth);
}

int Font_PrintScreen(Font* font, int x, int y, const char* text, u32 color, u32* framebuffer) {

	int num_chars = strlen(text);
	FT_GlyphSlot slot = font->face->glyph;
	int n;
	
	FT_Matrix matrix;
	double angle;
	
	angle = ( font->FontAngle / 360 ) * 3.14159 * 2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
	
	int x2 = x * 64;
	int y2 = (272 - y) * 64;
	
	for (n = 0; n < num_chars; n++) {
		FT_Set_Transform(font->face, &matrix, 0);
		
		FT_UInt glyph_index = FT_Get_Char_Index(font->face, text[n]);
		int error = FT_Load_Glyph(font->face, glyph_index, FT_LOAD_DEFAULT);
		if (error) continue;
		error = FT_Render_Glyph(font->face->glyph, ft_render_mode_normal);
		if (error) continue;

			fontPrintTextScreen(&slot->bitmap, x2 + slot->bitmap_left, y2 - slot->bitmap_top, color, framebuffer);

		x2 += slot->advance.x;
		y2 += slot->advance.y;
	}

	return 0;
}

int Font_RenderTexture(Font* font, int x, int y, const char* text, u32 color, FontTexture* fonttexture) {

	int num_chars = strlen(text);
	FT_GlyphSlot slot = font->face->glyph;
	int n;
	
	FT_Matrix matrix;
	double angle;
	
	angle = ( font->FontAngle / 360 ) * 3.14159 * 2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
	
	for (n = 0; n < num_chars; n++) {
		FT_Set_Transform(font->face, &matrix, 0);
		
		FT_UInt glyph_index = FT_Get_Char_Index(font->face, text[n]);
		int error = FT_Load_Glyph(font->face, glyph_index, FT_LOAD_DEFAULT);
		if (error) continue;
		error = FT_Render_Glyph(font->face->glyph, ft_render_mode_normal);
		if (error) continue;

			fontPrintTextTexture(&slot->bitmap, x + slot->bitmap_left, y - slot->bitmap_top, color, fonttexture);

		x += slot->advance.x >> 6;
		y += slot->advance.y >> 6;
	}

	return 0;
}

FontTexture* Font_CreateTexture(int width, int height)
{
	FontTexture* fonttexture = (FontTexture*) malloc(sizeof(FontTexture));
	if (!fonttexture) return NULL;
	fonttexture->width = width;
	fonttexture->height = height;
	fonttexture->textureWidth = getPow2(width);
	fonttexture->textureHeight = getPow2(height);
	fonttexture->data = (u32*) memalign(16, fonttexture->textureWidth * fonttexture->textureHeight * sizeof(u32));
	if (!fonttexture->data) return NULL;
	memset(fonttexture->data, 0, fonttexture->textureWidth * fonttexture->textureHeight * sizeof(u32));
	return fonttexture;
}

void Font_ClearTexture(FontTexture* fonttexture)
{
	int i;
	int size = fonttexture->textureWidth * fonttexture->textureHeight;
	u32* data = fonttexture->data;
	for (i = 0; i < size; i++, data++) *data = 0x00FFFFFF;
}

void Font_DisplayTexture(FontTexture* source, int dx, int dy)
{

	sceKernelDcacheWritebackInvalidateAll();

	sceGuTexImage(0, source->textureWidth, source->textureHeight, source->textureWidth, (void*) source->data);
	float u = 1.0f / ((float)source->textureWidth);
	float v = 1.0f / ((float)source->textureHeight);
	sceGuTexScale(u, v);

	int sx = 0;
	int sy = 0;
	int width = source->width;
	int height = source->height;

	int j = 0;
	while (j < width) {
		FontVertex* vertices = (FontVertex*) sceGuGetMemory(2 * sizeof(FontVertex));
		int sliceWidth = 64;
		if (j + sliceWidth > width) sliceWidth = width - j;
		vertices[0].u = sx + j;
		vertices[0].v = sy;
		vertices[0].x = dx + j;
		vertices[0].y = dy;
		vertices[0].z = 0;
		vertices[1].u = sx + j + sliceWidth;
		vertices[1].v = sy + height;
		vertices[1].x = dx + j + sliceWidth;
		vertices[1].y = dy + height;
		vertices[1].z = 0;
		sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
		j += sliceWidth;
	}
	
	sceGuFinish();
	sceGuSync(0, 0);
}

void Font_UnloadTexture(FontTexture* fonttexture)
{
     free(fonttexture->data);
     free(fonttexture);
}
