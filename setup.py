from distutils.core import setup, Extension

csdnocr = Extension('csdnocr', 
		include_dirs = [
			"/usr/local/include/python2.7", 
			"/usr/local/include/ImageMagick-6", 
			"/usr/local/include/leptonica", 
			"/usr/local/include/tesseract"
			],
		libraries = ["MagickWand-6.Q16", "tesseract"],
		sources = ['python.c', 'ocr.c', 'optimize.c', 'tesseract.cpp'])

setup (name = 'csdnocr', version = '1.0', description = 'This is a ocr for csdn.', ext_modules = [csdnocr])
