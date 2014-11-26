#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
#include "optimize.h"

#ifndef QuantumScale
#define QuantumScale  ((MagickRealType) 1.0/(MagickRealType) QuantumRange)
#endif
#define _SigmoidalContrast(x) \
	  (QuantumRange*(1.0/(1+exp(10.0*(0.5-QuantumScale*x)))-0.0066928509)*1.0092503)

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

void WhiteAndBlackImage(MagickWand *image)
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
			if (grey > 127)
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
	int (*npdata)[width] = (int(*)[width])data; 

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
			r = npdata[m][n] & 0x000000FF;
			g = (npdata[m][n] >> 8) & 0x000000FF;
			b = (npdata[m][n] >> 16) & 0x000000FF;
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
			if (abs(r - g) > 1)
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
