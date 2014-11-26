#include <wand/MagickWand.h>

/*分析单点噪声时的参考模板，矩阵*/
#define ISOLATE_SPAN 3
/*彩色转灰度*/
#define GREY_BYTE(a, b, c) (a * 30 + b * 59 + c * 11 + 50) / 100

typedef unsigned char BYTE;
void SigmoidalContrast(MagickWand *image);

/*转换为灰度图像*/
void GreyImage(MagickWand *image);

/*将灰度图image转换为单色黑白图像*/
void WhiteAndBlackImage(MagickWand *image);

/*反色*/
void ReverseImage(MagickWand *image);

/*降低噪声*/
void ReduceNoise(MagickWand *image);

int nmaxi(int a, int b);

int nmaxl(long a, long b);

int nmini(int a, int b);

int nminl(long a, long b);

void ReverseImageIfNeeded(MagickWand*);

int IsIsolatedNoise(BYTE *data, int offset, int width, int height, MagickPixelPacket *pixel);
