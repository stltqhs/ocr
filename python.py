#!/usr/local/bin/python
#test csdnocr
import sys
sys.path.append("./build/lib.linux-x86_64-2.7")
import csdnocr
import urllib2
from array import array
req = urllib2.urlopen("http://192.168.1.107:8888/ocrtest/p2.png")
leng = int(req.info()["Content-Length"])
data = req.read()
print csdnocr.recognize_from_imagedata(leng, array("c", data))
