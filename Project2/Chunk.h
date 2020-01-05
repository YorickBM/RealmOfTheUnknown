#pragma once
#include <string>

#include "CImg.h"
using namespace cimg_library;

class Chunk
{
public:
	Chunk(char heightMap) 
	{
		CImg<unsigned char> src(heightMap);
		img = src;
		int width = src.width();
		int height = src.height();
		unsigned char* ptr = src.data(10, 10); // get pointer to pixel @ 10,10
		unsigned char pixel = *ptr;
	}
	~Chunk() {}

private:
	CImg<unsigned char> img;
};

