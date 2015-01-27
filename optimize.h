#include <wand/MagickWand.h>

typedef short int BOOL;
#define FALSE 0
#define TRUE 1

#ifndef POINT
#define POINT
typedef struct tagPoint
{
	int x;
	int y;
} Point, *PrtPoint;
#endif

#ifndef WORDSECTION
#define WORDSECTION
typedef struct tagWordSection
{
	Point lt;//left-top
	Point rt;//right-top
	Point lb;//left-bottom
	Point rb;//right-bottom
} WordSection, *PtrWordSection;
#endif

/*分析单点噪声时的参考模板，矩阵*/
#define ISOLATE_SPAN 3
/*彩色转灰度*/
#define GREY_BYTE(a, b, c) (a * 30 + b * 59 + c * 11 + 50) / 100

typedef unsigned char BYTE;
void SigmoidalContrast(MagickWand *image);

/*转换为灰度图像*/
void GreyImage(MagickWand *image);

/*将灰度图image转换为单色黑白图像*/
void WhiteAndBlackImage(MagickWand *image, int threshold);

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

void BaselineAlignment(MagickWand *image);

void ErodeAndExpansion(MagickWand *image, int width, int height, double level);

int FindWordSection(MagickWand *image, int width, int height, PtrWordSection ws);

void ClearFrameBorder(MagickWand *image);

void ZeroPoint(Point *p);
