#include "stdafx.h"
#include "ImageUtils.h"

void ImageUtils::crossImage(CImage & image)
{
	for (int i = 0; i<image.GetWidth(); i++)
	{
		for (int j = 0; j<image.GetHeight(); j++)
		{
			UCHAR *cr = (UCHAR*)image.GetPixelAddress(i, j);
			cr[0] = cr[0] * cr[3] / 255;
			cr[1] = cr[1] * cr[3] / 255;
			cr[2] = cr[2] * cr[3] / 255;
		}
	}
}

void ImageUtils::preProcessImage(CImage & image)
{
	crossImage(image);
}
