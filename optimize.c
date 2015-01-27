#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <wand/MagickWand.h>
#include "optimize.h"

#ifndef QuantumScale
#define QuantumScale  ((MagickRealType) 1.0/(MagickRealType) QuantumRange)
#endif
#define _SigmoidalContrast(x) \
	  (QuantumRange*(1.0/(1+exp(10.0*(0.5-QuantumScale*x)))-0.0066928509)*1.0092503)

#define SCALE_FACTOR 1.8
void SigmoidalContrast(MagickWand *image)
{
	MagickPixelPacket pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			PixelGetMagickColor(image_pixel[x], &pixel);
			pixel.red = _SigmoidalContrast(pixel.red);
			pixel.green = _SigmoidalContrast(pixel.green);
			pixel.blue = _SigmoidalContrast(pixel.blue);
			pixel.index = _SigmoidalContrast(pixel.index);

			PixelSetMagickColor(image_pixel[x], &pixel);
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
}

void GreyImage(MagickWand *image)
{
	MagickPixelPacket pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	double r, g, b, grey;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			PixelGetMagickColor(image_pixel[x], &pixel);

			r = pixel.red * QuantumScale * 255;
			g = pixel.green * QuantumScale * 255;
			b = pixel.blue * QuantumScale * 255;
			grey = (r * 30 + g * 59 + b * 11 + 50) / 100;
			pixel.red = pixel.green = pixel.blue = grey * 1.0 / 255 * QuantumRange;
			PixelSetMagickColor(image_pixel[x], &pixel);
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
}

void WhiteAndBlackImage(MagickWand *image, int threshold)
{
	MagickPixelPacket pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	double r, g, b, grey;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			PixelGetMagickColor(image_pixel[x], &pixel);

			r = pixel.red * QuantumScale * 255;
			g = pixel.green * QuantumScale * 255;
			b = pixel.blue * QuantumScale * 255;
			grey = (r * 30 + g * 59 + b * 11 + 50) / 100;
			if (grey > threshold)
				grey = 255;
			else
				grey = 0;
			pixel.red = pixel.green = pixel.blue = grey * 1.0 / 255 * QuantumRange;
			PixelSetMagickColor(image_pixel[x], &pixel);
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
}

void ReverseImage(MagickWand *image)
{
	MagickPixelPacket pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			PixelGetMagickColor(image_pixel[x], &pixel);
			pixel.red = QuantumRange - (int)pixel.red;
			pixel.green = QuantumRange - (int)pixel.green;
			pixel.blue = QuantumRange - (int)pixel.blue;
			PixelSetMagickColor(image_pixel[x], &pixel);
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
}

void ReduceNoise(MagickWand *image)
{
	MagickPixelPacket pixel, tmp_pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	BYTE *data = NULL, *pdata;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	data = (BYTE*)calloc(width * height, 4);
	MagickExportImagePixels(image, 0, 0, width, height, "RGBA", CharPixel, data);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			if (IsIsolatedNoise(data, y * width + x, width, height, &tmp_pixel))
			{
				PixelGetMagickColor(image_pixel[x], &pixel);
				pixel.red = tmp_pixel.red;
				pixel.green = tmp_pixel.green;
				pixel.blue = tmp_pixel.blue;
				PixelSetMagickColor(image_pixel[x], &pixel);
			}	
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
	free(data);
}

int IsIsolatedNoise(BYTE *data, int offset, int width, int height, MagickPixelPacket *pixel)
{
	int pixel_unit[ISOLATE_SPAN * ISOLATE_SPAN];
	int x, y, i, j, grey, t = ISOLATE_SPAN / 2, m, n, k, tgrey, color, rf;
	BYTE *pdata = data, r, g, b;
	int r2, g2, b2;
	int *npdata = (int*)data; 

	y = offset % width;//column
	x = offset / width;//row
	k = 0;
	tgrey = 0;
	for (i = x - t; i <= x + t; i++)
	{
		for(j = y - t; j <= y + t; j++)
		{
			if ( i < 0)
				m = 0;
			else if (i >= height)
				m = height - 1;
			else
				m = i;
			if ( j < 0)
				n = 0;
			else if (j >= width)
				n = width - 1;
			else
				n = j;	
			r = npdata[m * width + n] & 0x000000FF;
			g = (npdata[m * width + n] >> 8) & 0x000000FF;
			b = (npdata[m * width + n] >> 16) & 0x000000FF;
			grey = GREY_BYTE(r, g, b);
			
			color = r;
			color = color | (g << 8);
			color = color | (b << 16);
			color = color | (grey << 24);
			pixel_unit[k++] = color;
			if (k != ISOLATE_SPAN * ISOLATE_SPAN / 2)
				tgrey += grey;
		}
	}
	tgrey /= ISOLATE_SPAN * ISOLATE_SPAN - 1;
	j = 0;
	k = ISOLATE_SPAN * ISOLATE_SPAN / 2;
	r2 = 0;
	g2 = 0;
	b2 = 0;
	for ( i = 0; i < ISOLATE_SPAN * ISOLATE_SPAN; i++)
	{
			if (i == k)
				continue;
			r = (pixel_unit[k] >> 24) & 0x000000FF;
			g = (pixel_unit[i] >> 24) & 0x000000FF;
			if (abs(r - g) > 5)
				j++;

			r = pixel_unit[i] & 0x000000FF;
			g = (pixel_unit[i] >> 8) & 0x000000FF;
			b = (pixel_unit[i] >> 16) & 0x000000FF;
			r2 += r;
			g2 += g;
			b2 += b;
	}
	rf = 0;
	if (j == ISOLATE_SPAN * ISOLATE_SPAN - 1)
	{
		rf = 1;
	}
	if (pixel != NULL)
	{
		i =  ISOLATE_SPAN * ISOLATE_SPAN - 1;
		r2 /= i;
		g2 /= i;
		b2 /= i;
		pixel->red = r2 * 1.0 / 255 * QuantumRange;
		pixel->green = g2 * 1.0 / 255 * QuantumRange;
		pixel->blue = b2 * 1.0 / 255 * QuantumRange;
	}
	return rf;
}

void ReverseImageIfNeeded(MagickWand *image)
{
	
	MagickPixelPacket pixel;
	PixelIterator *pixel_iterator;
	PixelWand **image_pixel;
	unsigned long x, y, width, height;
	int t;
	
	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	pixel_iterator = NewPixelIterator(image);	
	if (pixel_iterator == (PixelIterator*)NULL)
	{
		printf("can not create pixel iterator");
		return;
	}
	t = 0;
	for (y=0; y < height; y++)
	{
		image_pixel = PixelGetNextIteratorRow(pixel_iterator, &width);
		if (image_pixel == (PixelWand**) NULL)
		{
			printf("can not get pixels of row");
			return;
		}
		for (x = 0; x < width; x++)
		{
			PixelGetMagickColor(image_pixel[x], &pixel);
			if (pixel.red == 0)
				t++;
		}
		(void)PixelSyncIterator(pixel_iterator);
	}
	DestroyPixelIterator(pixel_iterator);
	if (t * 1.0 / (width * height) * 100 > 50)
		ReverseImage(image);
}

void BaselineAlignment(MagickWand *image)
{
	long width, height;
        int i , j, k;
	long w, h;
	int wordCount = 0;
	DrawingWand *drawing;
	MagickWand **alphabetic;
	PixelWand *fillColor;
	BYTE *wordsections;

	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);
	wordsections = (BYTE*)calloc(10, sizeof(WordSection));
	wordCount = FindWordSection(image, width, height, (PtrWordSection)wordsections);
	if (wordCount <= 1)
		return;
	alphabetic = (MagickWand**)calloc(wordCount, sizeof(MagickWand*));
	drawing = NewDrawingWand();
	fillColor = NewPixelWand();
	for (i = 0; i < wordCount; i++)
	{
		PtrWordSection pws = &((PtrWordSection)wordsections)[i];
		MagickWand *im = MagickGetImageRegion(image, pws->rb.x - pws->lb.x + 1, pws->rb.y - pws->lt.y + 1, pws->lt.x, pws->lt.y);
		if (i == 0)
		{
			w = MagickGetImageWidth(im);
			h = MagickGetImageHeight(im);
			k = (height - h) / 2;
		}
		*(alphabetic + i) = im;
	}
	PixelSetColor(fillColor, "#FFFFFF");
	DrawSetFillColor(drawing, fillColor);
	DrawRectangle(drawing, 0, 0, width, height);//background color
	
	j = 4;
	for (i = 0; i < wordCount; i++)
	{
		DrawComposite(drawing, CopyCompositeOp, j, k, w, h, *(alphabetic + i));
		j += 8 + w;
		free(alphabetic[i]);
	}
	MagickDrawImage(image, drawing);

	width *= SCALE_FACTOR;
	height *= SCALE_FACTOR;
	MagickScaleImage(image, width, height);
	free(alphabetic);
}

void ErodeAndExpansion(MagickWand *image, int width, int height, double level)
{
	int w, h;
	w = width / level;
	h = height / level;
	MagickScaleImage(image, w, h);
	MagickScaleImage(image, width, height);
}

int FindWordSection(MagickWand *image, int width, int height, PtrWordSection ws)
{
	MagickWand *tmp_image = CloneMagickWand(image);
	int wordCount = 0;
	int xoffset = 0;
	int maxNoColor = 2;
	int noColorCount = 0;
	int x, y;
	BOOL calcPosition = FALSE;
	BYTE *data;
	int *pdata;
	Point a, b, c, d;

	ErodeAndExpansion(tmp_image, width, height, 1.5);
	WhiteAndBlackImage(tmp_image, 250);

	data = (BYTE*)calloc(width * height, 4);
	MagickExportImagePixels(tmp_image, 0, 0, width, height, "RGBA", CharPixel, data);
	/*
	 * a  b
	 * c  d
	 */
	void PreparePoint(Point *a, Point *b, Point *c, Point *d, int width, int height);
	void CopyPoint(Point *dest, Point *src);
	PreparePoint(&a, &b, &c, &d, width, height);
	for (x = 0; x < width; x++)
	{
		BOOL hasColor = FALSE;
		for (y = 0; y < height; y++)
		{
			int color = *((int*)(data + (y * width + x) * 4));
			color = color & 0x00FFFFFF;
			if (color == 0 )
			{
				a.y = nmini(a.y, y);
				a.x = nmini(a.x, x);
				b.y = nmini(b.y, y);
				b.x = nmaxi(b.x, x);
				c.y = nmaxi(c.y, y);
				c.x = nmini(c.x, x);
				d.y = nmaxi(d.y, y);
				d.x = nmaxi(d.x, x);

				hasColor = TRUE;
				calcPosition = TRUE;
				noColorCount = 0;
			}
		}
		if (!hasColor)
			noColorCount++;
		if (noColorCount >= maxNoColor && calcPosition)
		{
			CopyPoint(&(ws[wordCount].lt), &a);
			CopyPoint(&(ws[wordCount].rt), &b);
			CopyPoint(&(ws[wordCount].lb), &c);
			CopyPoint(&(ws[wordCount].rb), &d);
			PreparePoint(&a, &b, &c, &d, width, height);
			wordCount++;
			calcPosition = FALSE;
			noColorCount = 0;
		}
	}

	if (calcPosition)
	{
		CopyPoint(&(ws[wordCount].lt), &a);
		CopyPoint(&(ws[wordCount].rt), &b);
		CopyPoint(&(ws[wordCount].lb), &c);
		CopyPoint(&(ws[wordCount].rb), &d);
		PreparePoint(&a, &b, &c, &d, width, height);
		wordCount++;
		calcPosition = FALSE;
		noColorCount = 0;
	}

	DestroyMagickWand(tmp_image);

	return wordCount;
}

void CopyPoint(Point *dest, Point *src)
{
	dest->x = src->x;
	dest->y = src->y;
}

void PreparePoint(Point *a, Point *b, Point *c, Point *d, int width, int height)
{
	a->x = width;
	a->y = height;
	b->x = 0;
	b->y = height;
	c->x = width;
	c->y = 0;
	d->x = 0;
	d->y = 0;
}

void ClearFrameBorder(MagickWand *image)
{
	long width, height;
        int i, j;
	PixelWand **pixel_wand;
	PixelIterator *iterator;

	width = MagickGetImageWidth(image);
	height = MagickGetImageHeight(image);

	iterator = NewPixelIterator(image);
	for (i = 0; i < height; i++)
	{
		pixel_wand = PixelGetNextIteratorRow(iterator, (size_t*)&width);
		for (j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1)
			{
				PixelSetColor(pixel_wand[j], "#ffffff");
			}
			else
			{
				if (j == 0 || j == width - 1)
					PixelSetColor(pixel_wand[j], "#ffffff");
			}			
		}
		(void)PixelSyncIterator(iterator);
	}
	DestroyPixelIterator(iterator);
}

int nmaxi(int a, int b)
{
	return a >= b ? a:b;
}

int nmaxl(long a, long b)
{
	return a >= b ? a:b;
}

int nmini(int a, int b)
{
	return a < b ? a:b;
}

int nminl(long a, long b)
{
	return a < b?a:b;
}
void ZeroPoint(Point *p)
{
	p->x = 0;
	p->y = 0;
}
