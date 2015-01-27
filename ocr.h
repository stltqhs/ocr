#include <wand/MagickWand.h>

MagickWand *LoadImageFromFile(char *fn);

void WriteImageToFile(MagickWand *image, char *fn);

char* RecognizeFromFile(char *fn);

char *RecognizeFromRawImageData(unsigned char *data, long length);

