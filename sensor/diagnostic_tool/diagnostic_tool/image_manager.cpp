#include "image_manager.h"

#include <stdio.h>
#include <stdlib.h>

ImageManager::ImageManager(long width, long height) {
	_width = width;
	_height = height;
}

int ImageManager::SaveImageToBmp(const char* filename) {
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	long file_size = (long)_width * (long)_height * 3 + 54;
	long width = _width;
	long height = _height;

	int i = 0;
	FILE *fp;

	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

	unsigned char *greyimage;
	greyimage = (unsigned char *)malloc((size_t)_width * _height * 3);
	for (i = 0; i < _width * _height; i++)
	{
		greyimage[i * 3] = _image_buffer[i];
		greyimage[i * 3 + 1] = _image_buffer[i];
		greyimage[i * 3 + 2] = _image_buffer[i];
	}
	if (!(fp = fopen(filename, "wb"))) return -1;
	fwrite(header, sizeof(unsigned char), 54, fp);
	fwrite(greyimage, sizeof(unsigned char), (size_t)(long)_width * _height * 3, fp);
	free(greyimage);
	fclose(fp);
	return 0;
}