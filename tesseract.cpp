#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

char *RecognizeOptchaText(char *filename)
{
	char *outText = NULL;
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) 
	{
	
		fprintf(stderr, "Could not initialize tesseract.\n");
	        exit(1);
	}
	Pix *image = pixRead(filename);
	api->SetImage(image);
	outText = api->GetUTF8Text();
	api->End();
	pixDestroy(&image);
	return outText;
}
