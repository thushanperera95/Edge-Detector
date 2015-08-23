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
int** convolve(unsigned char** image_in, int** mask, int width, int height);

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
  int** mask_one;
  int** mask_two;
  
  mask_one = malloc(2 * sizeof(int*));
  mask_two = malloc(2 * sizeof(int*));
  for (int i = 0; i < 2; i++) {
    mask_one[i] = malloc(2 * sizeof(int));
    mask_two[i] = malloc(2 * sizeof(int));
  }
  
  mask_one[0][0] = 1;
  mask_one[0][1] = 0;
  mask_one[1][0] = 0;
  mask_one[1][1] = -1;
  
  mask_two[0][0] = 0;
  mask_two[0][1] = 1;
  mask_two[1][0] = -1;
  mask_two[1][1] = 0;
  
  int** nesw = convolve(image_in, mask_one, width, height);
  int** nwse = convolve(image_in, mask_two, width, height);

  printf("Got here before the final image out write\n");
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      image_out[i][j] = abs(nesw[i][j]) + abs(nwse[i][j]);
    }
  }

  /* This simply inverts the colors */
  
  /*for (int j=0;j<height;j++) {
    for (int i=0;i<width;i++) {
      image_out[j][i] = 255 - image_in[j][i];
    }
  }*/

  /* write output image */

  if (pgmfile == TRUE) {
    write_pgm_image(image_out, file_out, &pg);
  }
  else
  {
    write_image(image_out, file_out, width, height);
  }
}

int** convolve(unsigned char** image_in, int** mask, int width, int height) {
  
  /* Copy of input image with extra height and width for working */
  int** work_image;
  
  /* Results of convolution will be saved to this 2D array */
  int** out_image;
  
  int new_width;
  int new_height;
  int temp;
  
  new_width = width + 1;
  new_height = height + 1;
  temp = 0; // Temp initialise value
  
  /* Allocate memory for workable image */
  work_image = malloc(new_height * sizeof(int*));
  for (int i = 0; i < height; i++) {
    work_image[i] = malloc(new_width * sizeof(int));
  }
  printf("Malloced workable image\n");
  
  /* Copy image to workable copy and extend column and row by 1 pixel */
  printf("new height = %d, new width = %d\n", new_height, new_width);
  for (int i = 0; i < new_height; i++) {
    printf("i = %d\n", i);
    for (int j = 0; j < new_width; j++) {
      printf("j = %d\n", j);
      if ( (i == new_height) && (j == new_width) ) { /* Are we at the bottom right corner? */
        work_image[i][j] = (int)image_in[i-1][j-1];
      }
      else if ( i == new_height ) { /* Are we at the bottom row? */
        work_image[i][j] = (int)image_in[i-1][j];
      }
      else if ( j == new_width ) { /* Are we at the far right column? */
        work_image[i][j] = (int)image_in[i][j-1];
      } 
      else {
        work_image[i][j] = (int)image_in[i][j];
      }
    }
  }
  printf("Copied image to workable and extend\n");
  
  /* Allocate memory for output int** */
  out_image = malloc(height * sizeof(int*));
  for (int i = 0; i < height; i++) {
    out_image[i] = malloc(width * sizeof(int));
  }
  printf("Mallocd output image\n");
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      /* Get pixel and the neighbours and then apply onto the mask matrix */
      temp = ( ((work_image[i][j] * mask[0][0]) + (work_image[i+1][j+1] * mask[1][1])) - ((work_image[i][j] * mask[0][0]) + (work_image[i+1][j+1] * mask[1][1])) );
      out_image[i][j] = temp;
    }
  }
  printf("Convolution\n");
  
  return out_image;
}