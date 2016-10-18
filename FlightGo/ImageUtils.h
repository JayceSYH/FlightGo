#pragma once

#include<atlimage.h>


class ImageUtils {
private:
	static void crossImage(CImage& image);
public:
	static void preProcessImage(CImage& image);
};