  /* 
      Author: Thushan Perera
      Email: thushan.perera95@gmail.com
  */
  
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
  
  void process_image(int, char *, char *, int, int, int);
  int** convolve(unsigned char**, int**, int, int, int);
  
  /* get options and input/output filename from user */
  
  int main(int argc, char *argv[])
  {
  
    FILE *fp_in, *fp_out;
    char *temp, ch;
    int set_file_in, set_file_out, set_sobel, count;
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
            case 's' :
                count++;
                      set_sobel = TRUE;
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
      process_image(pgmfile, file_in, file_out, width, height, set_sobel);
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
  void process_image(int pgmfile, char *file_in, char *file_out, int width, int height, int set_sobel)
  {
  
    unsigned char **image_in;  /* specify image array - char image */
    unsigned char **image_out;  /* specify image array - char image */
    struct pgmfile pg;
    
    /* Robert's cross masks */
    int** mask_one;
    int** mask_two;
    
    /* Sobel masks */
    int** smask_one;
    int** smask_two;
    
    int** first;
    int** second;
    
    int temp; // Holds current convoluted value
    int threshold = 127; // Darker images will require lower thresholds
  
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
    
    /* Allocate memory for the 2 robert's cross masks */ 
    mask_one = malloc(2 * sizeof(int*));
    mask_two = malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++) {
      mask_one[i] = malloc(2 * sizeof(int));
      mask_two[i] = malloc(2 * sizeof(int));
    }
    
    /* Allocate memory for the 2 sobel masks */
    smask_one = malloc(3 * sizeof(int*));
    smask_two = malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
      smask_one[i] = malloc(3 * sizeof(int));
      smask_two[i] = malloc(3 * sizeof(int));
    }
    
    /* Assign values of Robert's cross mask one */
    mask_one[0][0] = 1;
    mask_one[0][1] = 0;
    mask_one[1][0] = 0;
    mask_one[1][1] = -1;
    
    /* Assign values of Robert's cross mask two */
    mask_two[0][0] = 0;
    mask_two[0][1] = 1;
    mask_two[1][0] = -1;
    mask_two[1][1] = 0;
    
    /* Assign values of Sobel vertical mask */
    smask_one[0][0] = -1;
    smask_one[0][1] = 0;
    smask_one[0][2] = 1;
    smask_one[1][0] = -2;
    smask_one[1][1] = 0;
    smask_one[1][2] = 2;
    smask_one[2][0] = -1;
    smask_one[2][1] = 0;
    smask_one[2][2] = 1;
    
    /* Assign values of Sobel horizontal mask */
    smask_two[0][0] = 1;
    smask_two[0][1] = 2;
    smask_two[0][2] = 1;
    smask_two[1][0] = 0;
    smask_two[1][1] = 0;
    smask_two[1][2] = 0;
    smask_two[2][0] = -1;
    smask_two[2][1] = -2;
    smask_two[2][2] = -1;
    
    if (set_sobel == TRUE) { /* Did user select sobel or not? */
      printf("\nWill be using Sobel edge detection...\n");
      
      first = convolve(image_in, smask_one, 3, width, height);
      second = convolve(image_in, smask_two, 3, width, height);
    } else {
      printf("\nWill be using Robert's Cross edge detection...\n");
      
      first = convolve(image_in, mask_one, 2, width, height);
      second = convolve(image_in, mask_two, 2, width, height);
    }
  
    /* Get convolution result and add them to get the final pixel */
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        temp = abs(first[i][j]) + abs(second[i][j]);
        if (temp > threshold) {
          temp = threshold;
        }
        image_out[i][j] = (unsigned char)temp;
      }
    }
    
    /* Memory cleanup */
    free(mask_one);
    free(mask_two);
    free(smask_one);
    free(smask_two);
    free(first);
    free(second);
  
    /* write output image */
  
    if (pgmfile == TRUE) {
      write_pgm_image(image_out, file_out, &pg);
    }
    else
    {
      write_image(image_out, file_out, width, height);
    }
  }
  
  /* Given an image and a mask, it will return the convolution result.
     Note that each mask needs to be sent separately.
     Also note that this method deals with special edge cases by ignoring a 1 pixel border around the image */
  int** convolve(unsigned char** image_in, int** mask, int mask_size, int width, int height) {
    
    /* Copy of input image with extra height and width for working */
    int** work_image;
    
    /* Results of convolution will be saved to this 2D array */
    int** out_image;
    int temp;
    
    /* Allocate memory for workable image */
    work_image = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
      work_image[i] = malloc(width * sizeof(int));
    }
    
    /* Copy contents of input image to the workable copy */
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {          
          work_image[i][j] = (int)image_in[i][j];
      }
    }
    
    /* Allocate memory for output int** */
    out_image = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
      out_image[i] = malloc(width * sizeof(int));
    }
    
    /* We will be ignoring the 1 pixel border around image for edge cases */
    for (int i = 1; i < height-1; i++) {
      for (int j = 1; j < width-1; j++) {
        
        /* Get pixel and the neighbours and then apply onto the mask matrix */
        
        if (mask_size == 3) { // Multiplies the 3x3 matrix with a pixel and its 8 neighbours
          temp = (work_image[i-1][j-1] * mask[0][0]) + (work_image[i-1][j] * mask[0][1]) + 
                  (work_image[i-1][j+1] * mask[0][2]) + (work_image[i][j-1] * mask[1][0]) + 
                  (work_image[i][j] * mask[1][1]) + (work_image[i][j+1] * mask[1][2]) + 
                  (work_image[i+1][j-1] * mask[2][0]) + (work_image[i+1][j] * mask[2][1]) + 
                  (work_image[i+1][j+1] * mask[2][2]);
                  
        } else if (mask_size == 2) { // Multiplies the 2x2 matrix with pixel and its 3 neighbours
          temp = (work_image[i][j] * mask[0][0]) + (work_image[i+1][j+1] * mask[1][1]) + 
                  (work_image[i][j+1] * mask[0][1]) + (work_image[i+1][j] * mask[1][0]);
        }
        
        out_image[i][j] = temp;
      }
    }
  
    free(work_image);
    
    return out_image;
  }
