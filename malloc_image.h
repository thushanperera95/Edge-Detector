/* routines to allocate space for images of different formats */
extern unsigned char ** malloc_char_image(int, int);
extern int ** malloc_int_image(int, int);
extern long ** malloc_long_image(int, int);
extern float ** malloc_float_image(int, int);
extern double ** malloc_double_image(int, int);

extern void message_exit(char *);

/* the following for dealing with raw images - no header */
extern void read_image(unsigned char **,char *,int,int);
extern void read_float_image(float **,char *,int,int);
extern void write_image(unsigned char **,char *,int,int);
extern void write_float_image(float **,char *,int,int);

/* the following for pgm formatted images - note there are restrictions */
extern void read_pgm_image(unsigned char **,char *,struct pgmfile *);
extern void write_pgm_image(unsigned char **,char *,struct pgmfile *);
extern void read_pgm_header(FILE *,struct pgmfile *);
extern void get_pgm_header(char *,struct pgmfile *);
extern void get_size(char *,int *,int *);

/* the following for deallocating images */
extern void free_char_image(char **,int);
extern void free_int_image(int **,int);
extern void free_float_image(float **,int);
