#ifndef _Utils_hpp_
#define _Utils_hpp_

extern int rows;
extern int cols;
extern unsigned char** reds;
extern unsigned char** greens;
extern unsigned char** blues;
extern unsigned char** temp_reds;
extern unsigned char** temp_greens;
extern unsigned char** temp_blues;
//extern char *fileBuffer;
//extern int bufferSize;

bool ok(int i, int j);
void mean(float *total_mean, int color);
extern void blur();
extern void sepia();
extern void washedOut();
extern void multipleSign();
//extern void readPixels();

#endif