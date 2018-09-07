#include "stdafx.h"
#include "texture.h"

#include "util.h"
#include "picoPNG.h"

Texture::Texture()
{
	texSrc = NULL;

	premultiplied = false;

	glGenTextures(1, &id);
	bind();
}

Texture::~Texture()
{
	// TODO: MemoryLeak
	//destroy();
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::activate(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
}

void Texture::filter(int minMode, int maxMode)
{
	bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, maxMode);
}

void Texture::wrap(int s, int t)
{
	bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::destroy()
{
	glDeleteTextures(1, &id);
	if (texSrc != NULL)
	{
		SDL_FreeSurface(texSrc);
	}
}

void Texture::pixels(int w, int h, std::vector<byte> data)
{
	bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &(data[0]));
}

void Texture::pixels(int w, int h, std::vector<int> pixels)
{
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixels[0]));
}

Texture* Texture::create(int width, int height, std::vector<byte> data)
{
	Texture* tex = new Texture();
	tex->pixels(width, height, data);
	return tex;
}

void Texture::bitmap(const std::string& bitmap)
{
	//_getBytesPNG(bitmap, out, w, h);
	SDL_Surface* img = IMG_Load(bitmap.c_str());
	Texture::bitmap(img);
}

void Texture::bitmap(SDL_Surface* img)
{
	//width = w, height = h;
	width = img->w;
	height = img->h;

	bind();

	if (img->format->BytesPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	}
	else if (img->format->BytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	}
	else
	{
		*(int*)0 = 1;
	}

	premultiplied = true;

	texSrc = img;
}

void Texture::_getBytesPNG(const std::string& bitmap, std::vector<byte>& data, unsigned long &width, unsigned long &height)
{
	////This is the input data to decodePNG, which we load from a file
	//std::vector<unsigned char> in;
	//
	////Read in the image file contents into a buffer
	//if (IOManager::readFileToBuffer(bitmap, in) == false)
	//{
	//	fatalError("Failed to load PNG file to buffer!");
	//}
	//
	////Decode the .png format into an array of pixels
	//int errorCode = decodePNG(data, width, height, &(in[0]), in.size());
	//if (errorCode != 0)
	//{
	//	fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	//}
}

int Texture::getPixel(int x, int y)
{
	int bpp = texSrc->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)texSrc->pixels + y * texSrc->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Texture::setPixel(int x, int y, int color)
{
	int bpp = texSrc->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)texSrc->pixels + y * texSrc->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = color & 0xFF;
		break;

	case 2:
		*(Uint16 *)p = color & 0xFFFF;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			//return p[0] << 16 | p[1] << 8 | p[2];
		{
			p[0] = (color << 16) & 0xFF;
			p[1] = (color << 8) & 0xFF;
			p[2] = color & 0xFF;
		}
		else
			//return p[0] | p[1] << 8 | p[2] << 16;
		{
			p[0] = color & 0xFF; 
			p[1] = (color << 8) & 0xFF;
			p[2] = (color << 16) & 0xFF;
		}
		break;

	case 4:
		*(Uint32 *)p = color;
		break;

	default:
		//return 0;       /* shouldn't happen, but avoids warnings */
		break;
	}
}
