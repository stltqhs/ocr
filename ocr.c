#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
#include <string.h>
#include "ocr.h"
#include "optimize.h"
#include "tesseract.h"

char *RecognizeImage(MagickWand *image);
int main(int argc, char **argv)
{
	int retc = 0;
	char *text;
	MagickWandGenesis();
	text = RecognizeFromFile(argv[1]);
	if (text != NULL)
	{
		printf("%s\n", text);
		free(text);
	}
	MagickWandTerminus();
	return retc;
}

char* RecognizeFromFile(char *fn)
{
	MagickWand *image;

	image = LoadImageFromFile(fn);
	if (image != NULL)
		return RecognizeImage(image);
	return NULL;
}
char *RecognizeFromRawImageData(unsigned char *data, long length)
{
	MagickWand *image;
	MagickBooleanType status;
	image = NewMagickWand();
	status = MagickReadImageBlob(image, data, length);
	if (status == MagickFalse)
		return NULL;
	return RecognizeImage(image);
}
char *RecognizeImage(MagickWand *image)
{
	char *text;
	char *buffer;
	BYTE *data;
	long width, height;
	SigmoidalContrast(image);
	ReduceNoise(image);
	GreyImage(image);
	WhiteAndBlackImage(image, 127);
	ReverseImageIfNeeded(image);
	ClearFrameBorder(image);
	BaselineAlignment(image);

	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	data = (BYTE*)calloc(4, width * height);
	
	MagickExportImagePixels(image, 0, 0, width, height, "RGBA", CharPixel, data);
	text = RecognizeOptchaTextFromData((unsigned char*)data, width, height, 4, width * 4);
	DestroyMagickWand(image);
	free(data);
	if (text != NULL)
	{
		int i, k;
		int j = strlen(text);
		buffer = (char*)malloc(strlen(text) * sizeof(char));
		k = 0;
		for (i = 0; i < j; i++)
		{
			if ((text[i] >= 48 && text[i] <= 57) || (text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
			{
				buffer[k++] = text[i];
			}
		}
		buffer[k] = 0;
		free(text);
		text = buffer;
	}
	return text;
}

MagickWand *LoadImageFromFile(char *fn)
{
	MagickWand *image;
	MagickBooleanType status;
	image = NewMagickWand();
	status = MagickReadImage(image, fn);
	if (status == MagickFalse)
		return NULL;
	return image;
}

void WriteImageToFile(MagickWand *image, char *fn)
{
	MagickWriteImages(image, fn, MagickTrue);
}
