#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

extern "C" {
char *RecognizeOptchaTextFromData(const unsigned char *imagedata, int width, int height, int bytes_per_pixel, int bytes_per_line)
{
	char *outText = NULL;
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) 
	{
	
		fprintf(stderr, "Could not initialize tesseract.\n");
	        exit(1);
	}
	api->SetImage(imagedata, width, height, bytes_per_pixel, bytes_per_line);
	outText = api->GetUTF8Text();
	api->End();
	return outText;
}
}
