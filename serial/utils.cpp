#include "utils.hpp"
bool ok(int i, int j, int rows, int cols){
    if(i > 0 && i < rows && j > 0 && j < cols){
        return true;
    }
    return false;
}

void mean(float *r_mean, float *g_mean, float *b_mean, int rows, int cols, unsigned char **reds, unsigned char **greens, unsigned char **blues){
    float r_sum = 0;
    float g_sum = 0;
    float b_sum = 0;
    for(int i = 0; i < rows; i++){
        for(int j=0; j < cols; j++){
            r_sum += reds[i][j];
            g_sum += greens[i][j];
            b_sum += blues[i][j];
        }
    }
    *r_mean = r_sum / (rows * cols);
    *g_mean = g_sum / (rows * cols);
    *b_mean = b_sum / (rows * cols);
}
