/* #include <malloc.h> */
#include <stdio.h>
#include <stdlib.h>

#include "pgmfile.h"

/* routines to allocate space for images of different formats */
unsigned char ** malloc_char_image(int, int);
int ** malloc_int_image(int, int);
long ** malloc_long_image(int, int);
float ** malloc_float_image(int, int);
double ** malloc_double_image(int, int);

void message_exit(char *);

/* the following for dealing with raw images - no header */
void read_image(unsigned char **,char *,int,int);
void read_float_image(float **,char *,int,int);
void write_image(unsigned char **,char *,int,int);
void write_float_image(float **,char *,int,int);

/* the following for pgm formatted images - note there are restrictions */
void read_pgm_image(unsigned char **,char *,struct pgmfile *);
void write_pgm_image(unsigned char **,char *,struct pgmfile *);
void read_pgm_header(FILE *,struct pgmfile *);
void get_pgm_header(char *,struct pgmfile *);
void get_size(char *,int *,int *);

/* the following for deallocating images */
extern void free_char_image(char **,int);
extern void free_int_image(int **,int);
extern void free_float_image(float **,int);
void reverse_bytes(int *);

unsigned char ** malloc_char_image(int xres,int yres)
/*
int xres,yres;
*/
{
    unsigned char ** image;
	int j;

    image = (unsigned char **) malloc( sizeof(unsigned char *) * yres);
	if(image == NULL)
		message_exit("char");

    for(j=0;j<yres;j++){
        image[j] = (unsigned char *) malloc(sizeof(unsigned char) * xres);
		if(image[j] == NULL)
			message_exit("char");
    }
    return(image);

}

int ** malloc_int_image(int xres,int yres)
/*
int xres,yres;
*/
{
    int ** image;
	int j;

    image = (int **) malloc( sizeof(int *) * yres);
	if(image == NULL)
		message_exit("int");

    for(j=0;j<yres;j++){
        image[j] = (int *) malloc(sizeof(int) * xres);
		if(image[j] == NULL)
			message_exit("int");
    }
    return(image);
}

float ** malloc_float_image(int xres,int yres)
/*
int xres,yres;
*/
{
    float ** image;
	int j;

    image = (float **) malloc( sizeof(float *) * yres);
	if(image == NULL)
		message_exit("float");

    for(j=0;j<yres;j++){
        image[j] = (float *) malloc(sizeof(float) * xres);
		if(image[j] == NULL)
			message_exit("float");
    }
    return(image);
}

long ** malloc_long_image(int xres,int yres)
/*
int xres,yres;
*/
{
    long ** image;
	int j;

    image = (long **) malloc( sizeof(long *) * yres);
	if(image == NULL)
		message_exit("long");

    for(j=0;j<yres;j++){
        image[j] = (long *) malloc(sizeof(long) * xres);
		if(image[j] == NULL)
			message_exit("long");
    }
    return(image);
}

double ** malloc_double_image(int xres,int yres)
/*
int xres,yres;
*/
{
    double ** image;
	int j;

    image = (double **) malloc( sizeof(double *) * yres);
	if(image == NULL)
		message_exit("double");

    for(j=0;j<yres;j++){
        image[j] = (double *) malloc(sizeof(double) * xres);
		if(image[j] == NULL)
			message_exit("double");
    }
    return(image);
}

void message_exit(char *type)
/*
char *type;
*/
{
	printf("error occurred when allocating %s image memory - aborting\n",type);
	exit(-1);
}



void read_image(unsigned char **image,char *filename,int width,int height)
/*
	reads in an unsigned char image
*/
/*
unsigned char **image;
char *filename;
int width,height;
*/
{
	int i,j;
	FILE *fp_in;

	if((fp_in = fopen(filename,"rb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}
	for(i=0;i<height;i++)
	    for(j=0;j<width;j++)
	    	image[i][j] = (unsigned char) getc(fp_in);
	/* check the image file was the correct size */
	if (feof(fp_in)){
		printf("ERROR: premature end of file - image too big?\n");
	}
	else
		if (getc(fp_in) != EOF) {
			printf("ERROR: extra characters in file - image too small?\n");
		}
	fclose(fp_in);
}




void read_float_image(float **image,char *filename,int width,int height)
/*
	reads in an float image
*/
/*
float **image;
char *filename;
int width,height;
*/
{
	int i;
	FILE *fp_in;

	if((fp_in = fopen(filename,"rb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}

	/* uses fread to read a whole line of image in in one go */
	for(i=0;i<height;i++)
		fread(image[i],sizeof(float),width,fp_in);

	/* check the image file was the correct size */
	if (feof(fp_in)){
		printf("ERROR: premature end of file - image too big?\n");
	}
	else
		if (getc(fp_in) != EOF) {
			printf("ERROR: extra characters in file - image too small?\n");
		}
	fclose(fp_in);
}


void write_image(unsigned char **image,char *filename,int width,int height)
/*
	writes out an unsigned char image
*/
/*
unsigned char **image;
char *filename;
int width,height;
*/
{
	int i,j;
	FILE *fp_out;


	if((fp_out = fopen(filename,"wb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}
	
	for(i=0;i<height;i++)
	    for(j=0;j<width;j++)
			putc(image[i][j],fp_out);
	
	fclose(fp_out);


}


void write_float_image(float **image,char *filename,int width,int height)
/*
	writes out a float image
*/
/*
float **image;
char *filename;
int width,height;
*/
{
	int i;
	FILE *fp_out;

	if((fp_out = fopen(filename,"wb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}

	/* use fwrite to save a complete row at a time */
	
	for(i=0;i<height;i++){
		fwrite(image[i],sizeof(float),width,fp_out);
	}
	
	fclose(fp_out);
}



void read_pgm_image(unsigned char **image,char *filename,struct pgmfile *pg)
{
	int i,j;
	FILE *fp_in;

	if((fp_in = fopen(filename,"rb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}
	/* read pg header */
	read_pgm_header(fp_in,pg);
	printf("%d %d %d\n",(*pg).pgm_width,(*pg).pgm_height,(*pg).pgm_depth);

	for(j=0;j<(*pg).pgm_height;j++)
		for(i=0;i<(*pg).pgm_width;i++){
			image[j][i] = (unsigned char) getc(fp_in);
		}

	/* check the image file was the correct size */
	if (feof(fp_in)){
		printf("ERROR: premature end of file - image too big?\n");
	}
	else
		if (getc(fp_in) != EOF) {
			printf("ERROR: extra characters in file - image too small?\n");
		}
	fclose(fp_in);
}


void write_pgm_image(unsigned char **image,char *filename,struct pgmfile
*pg)
/*
	writes out an unsigned char image
*/
{
	int i,j;
	FILE *fp_out;

	if((fp_out = fopen(filename,"wb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}
	/* write out the pgm header */
	fprintf(fp_out,"P5\n");
	fprintf(fp_out,"#created by Geoff West after Paul Rosin\n");
	fprintf(fp_out,"%d %d\n",(*pg).pgm_width,(*pg).pgm_height);
	fprintf(fp_out,"%d\n",(*pg).pgm_depth);
	
	for(i=0;i<(*pg).pgm_height;i++)
	    for(j=0;j<(*pg).pgm_width;j++){
			putc(image[i][j],fp_out);
			if((*pg).pgm_depth == 24){
				putc(image[i][j],fp_out);
				putc(image[i][j],fp_out);
			}
		}
	fclose(fp_out);
}



void read_pgm_header(FILE *fp,struct pgmfile *pg)
{
	/* reads in a pgm header */

	char str[1000];
	int t1,t2,t3;

	/* read first line of the file and process */
	fgets(str,255,fp);
	do{
		fgets(str,255,fp);
	}while(str[0] == '#');

	/* get image params */
	sscanf(str,"%d %d",&t1,&t2);
	fscanf(fp,"%d",&t3);
	/* skip CR */
	getc(fp);

	(*pg).pgm_width = t1;
	(*pg).pgm_height = t2;
	(*pg).pgm_depth = t3;

	printf("image size: width: %d height: %d depth: %d\n",
			(*pg).pgm_width,(*pg).pgm_height,(*pg).pgm_depth);

	if((*pg).pgm_depth != 255){
		printf(" image depth wrong - aborting\n");
		exit(-1);
	}
}


void get_pgm_header(char *filename,struct pgmfile *pg)
{
	char str[1000];
	int t1,t2,t3;
	FILE *fp;

	if((fp = fopen(filename,"rb")) == NULL){
		printf("cant open %s\n",filename);
		exit(-1);
	}

	/* read first line of the file and process */
	fgets(str,255,fp);
	do{
		fgets(str,255,fp);
	}while(str[0] == '#');

	/* get image params */
	sscanf(str,"%d %d",&t1,&t2);
	fscanf(fp,"%d",&t3);
	/* skip CR */
	getc(fp);

	(*pg).pgm_width = t1;
	(*pg).pgm_height = t2;
	(*pg).pgm_depth = t3;

	printf("image size: width: %d height: %d depth: %d\n",
			(*pg).pgm_width,(*pg).pgm_height,(*pg).pgm_depth);

	if((*pg).pgm_depth != 255){
		printf(" image depth wrong - aborting\n");
		exit(-1);
	}
	fclose(fp);
}


void free_char_image(char **image,int yres)
/*
char **image;
int yres;
*/
{
	int j;

    /* use to check returned value - ANSI has void so dont now */

	for(j=0;j<yres;j++)
		free((char *)image[j]);
	free((char *)image);
}


void free_int_image(int **image,int yres)
/*
int **image;
int yres;
*/
{
	int j;

    for(j=0;j<yres;j++)
		free((char *)image[j]);
	free((char *)image);
}


void free_float_image(float **image,int yres)
/*
float **image;
int yres;
*/
{
	int j;

    for(j=0;j<yres;j++)
		free((float *)image[j]);
	free((float *)image);
}





void reverse_bytes(int *value)
{
	int i, temp, temp2;

	/* initialise */
	temp = *value;
	temp2 = temp & 255;
	
	/* swap rest bytes */
	for(i=0;i<3;i++){
		temp = temp >> 8;
		temp2 = temp2 << 8;
		temp2 = temp2 | (temp & 255);
	}
	*value = temp2;
}
