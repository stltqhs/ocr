INCLUDE=-I/usr/local/include/ImageMagick-6 -I/usr/local/include/leptonica -I/usr/local/include/tesseract
LIB_DIRS=/usr/local/lib
LIB=-lMagickWand-6.Q16 -ltesseract
OBJ=ocr.o optimize.o tesseract.o
PARAMD=-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16
GNUFLAGS=-g
ocr:$(OBJ)
	$(CXX) $(INCLUDE) $(LIB) $(GNUFLAGS) -o $@ $^
tesseract.o:tesseract.cpp tesseract.h
	$(CXX) -c $(GNUFLAGS) $(INCLUDE) -o $@ $<
ocr.o:ocr.c ocr.h optimize.h tesseract.h
	$(CXX) -c $(GNUFLAGS) $(INCLUDE) $(PARAMD) -o $@ $<
optimize.o:optimize.c optimize.h
	$(CXX) -c $(GNUFLAGS) $(INCLUDE) $(PARAMD) -o $@ $<
.PHONY:clean
clean:
	rm -f *.o
	rm -f ocr
