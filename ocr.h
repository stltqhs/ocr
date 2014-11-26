#include <wand/MagickWand.h>

int _main(int, char**);

MagickWand *LoadImageFromFile(char *fn);

void WriteImageToFile(MagickWand *image, char *fn);

