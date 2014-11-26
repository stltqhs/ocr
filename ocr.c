#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
#include "ocr.h"
#include "optimize.h"
#include "tesseract.h"

int main(int argc, char **argv)
{
	int retc = 0;
	MagickWandGenesis();
	retc = _main(argc, argv);
	MagickWandTerminus();
	return retc;
}

int _main(int argc, char **argv)
{
	MagickBooleanType status;
	MagickWand *image;

	image = LoadImageFromFile(argv[1]);
	SigmoidalContrast(image);
	ReduceNoise(image);
	GreyImage(image);
	WhiteAndBlackImage(image);
	ReverseImageIfNeeded(image);
	WriteImageToFile(image, argv[2]);
	DestroyMagickWand(image);
	printf("%s", RecognizeOptchaText(argv[2]));
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
