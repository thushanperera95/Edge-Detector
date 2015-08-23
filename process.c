#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pgmfile.h"
#include "malloc_image.h"

#define BLACK 0
#define WHITE 255
#define FALSE 0
#define TRUE !FALSE
#define DEFAULT_SIZE 512

void process_image(int, char *, char *, int, int);

/* get options and input/output filename from user */

int main(int argc, char *argv[])
{

  FILE *fp_in, *fp_out;
  char *temp, ch;
  int set_file_in, set_file_out, count;
  int width, height;
  char file_in[255], file_out[255];
  int pgmfile;

  width = DEFAULT_SIZE;
  height = DEFAULT_SIZE; 
  set_file_in = FALSE;
  set_file_out = FALSE;

  pgmfile = FALSE;

  if (argc > 1) {
    count = 0;
    do { 
         count++;
         temp = argv[count];
         if (*argv[count] == '-') {
           ch = *(++temp);
           switch (ch) {
           case 'i' :
            	     count++;
                     strcpy(file_in, argv[count]);
                     set_file_in = TRUE;
                     pgmfile = FALSE;
                     break;
           case 'p' :
            	     count++;
                     strcpy(file_in, argv[count]);
                     set_file_in = TRUE;
                     pgmfile = TRUE;
                     break;
           case 'o' :
            	     count++;
                     strcpy(file_out, argv[count]);
                     set_file_out = TRUE;
                     break;
           case 'w' : 
	             count++;
                     width = atoi(argv[count]);
                     break;
           case 'h' :
	             count++;
                     height = atoi(argv[count]);
                     break;
           default :
	             printf("Error on command line\n");
	   }
	 }
         else {
	   printf("Error on command line\n");
           exit(-1);
	 }
    } while (count < argc - 1);
    if (set_file_in == FALSE) {
      printf("need input file name\n");
      exit(-1);
    }
    if (set_file_out == FALSE) {
      printf("need output file name\n");
      exit(-1);
    }
    if ((fp_in = fopen(file_in,"r")) == NULL) {
        printf("Input file %s not found - aborting\n",file_in);
        exit(-1);
    }
    else 
        fclose(fp_in);
    if ((fp_out = fopen(file_out,"w")) == NULL) {
        printf("Could not open output file %s - aborting\n",file_out);
        exit(-1);
    }
    else 
        fclose(fp_out);    
    /* Main image processing here using process_image */
    printf("Reading from file %s\n",file_in);
    printf("Writing to file %s\n",file_out); 
    process_image(pgmfile, file_in, file_out, width, height);
  }
  else {
    printf(" Usage:\n");
    printf(" process_ -[ip] file_in -o file_out [options]\n");
    printf("\noptions:\n");
    printf("\n");
    printf("needs -[ip] and -o to work\n");
    printf("use -i for raw images (need height and width)\n");
    printf("use -p for pgm images\n");
    exit(-1);
  }  
  return 0;
}

/* open image file from file_in and write it out to file_out */
void process_image(int pgmfile, char *file_in, char *file_out, int width, int height)
{

  int i,j;
  unsigned char **image_in;  /* specify image array - char image */
  unsigned char **image_out;  /* specify image array - char image */
  struct pgmfile pg;

  /* if correct PGM file format  then get height and width */

  if (pgmfile == TRUE) {
    get_pgm_header(file_in,&pg);
    height = pg.pgm_height;
    width = pg.pgm_width;
  }

  printf("Image size: Height %d Width %d\n",width,height);

  /* allocate memory for image */

  image_in = malloc_char_image(width,height);

  /* if file in PGM format then read info into predefined structure */
  /* else read raw file */
  if (pgmfile == TRUE) {
    read_pgm_image(image_in,file_in,&pg);
  }
  else {
    read_image(image_in,file_in,width,height);
  }
  
  image_out = malloc_char_image(width,height);
  
  /** Do the extra processing here */



  /* This simply inverts the colors
  
  for (j=0;j<height;j++) {
    for (i=0;i<width;i++) {
      image_out[j][i] = 255 - image_in[j][i];
    }
  }
  */

  /* write output image */

  if (pgmfile == TRUE) {
    write_pgm_image(image_out, file_out, &pg);
  }
  else
  {
    write_image(image_out, file_out, width, height);
  }
}

int** convolve(char** image_in, int mask_one, int width, int height) {
  
}