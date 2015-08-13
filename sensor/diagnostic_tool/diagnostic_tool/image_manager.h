#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_

class ImageManager {
public:
	ImageManager(long width, long height);
	void set_image_buffer(unsigned char* image_buffer) {
		_image_buffer = image_buffer;
	};
	int SaveImageToBmp(const char* filename);
private:
	unsigned char* _image_buffer;
	long _width, _height;
};

#endif // IMAGE_MANAGER_H_