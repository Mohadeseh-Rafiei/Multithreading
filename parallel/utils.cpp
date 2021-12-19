#include "utils.hpp"


bool ok(int i, int j){
    if(i > 0 && i < rows && j > 0 && j < cols){
        return true;
    }
    return false;
}

void mean(float *total_mean,int color){
    float sum = 0;

    for(int i = 0; i < rows; i++){
        for(int j=0; j < cols; j++){
            if(color == 0) sum += temp_reds[i][j];
            if(color == 1) sum += temp_greens[i][j];
            if(color == 2) sum += temp_blues[i][j];
        }
    }
    *total_mean = sum / (rows * cols);
}
