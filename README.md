# Edge Detector in C
Implementation of the Sobel and Robert's Cross edge detection algorithms.

Notes:
  Only supports the .pgm file format. Feel free to add support for other file formats like .png
  Deals with edges of the image by ignoring a 1 pixel border around the image.
  Lower the value of 'threshold' in the 'process' function for better edge detection.

Build:
  Run makefile
  
Usage:
  ./process -p <input file name>.pgm -o <output file name>.pgm [-s]
  -s parameter to enable Sobel. The default is Robert's Cross.
