#include "utils.hpp"
#include <iostream>
#include <pthread.h>
void* smooth_color(void* arg)
{
    long color = (long)arg;
    pthread_t threads[rows];
    int return_code;
    int delta_i[8] = {1, 0, -1, 0, 1, -1, 1, -1};
    int delta_j[8] = {0, 1, 0, -1, 1, -1, -1, 1};

    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            int sum = 0;
            if(color == 0) sum += int(temp_reds[i][j]);
            if(color == 1) sum += int(temp_greens[i][j]);
            if(color == 2) sum += int(temp_blues[i][j]);
            for(int k = 0; k < 8; k++){
                if (ok(i + delta_i[k], j + delta_j[k])){
                    if(color == 0) sum += int(temp_reds[i + delta_i[k]][j + delta_j[k]]);
                    if(color == 1) sum += int(temp_greens[i + delta_i[k]][j + delta_j[k]]);
                    if(color == 2) sum += int(temp_blues[i + delta_i[k]][j + delta_j[k]]);
                }
            }
            if(color == 0) reds[i][j] = char(sum/9);
            if(color == 1) greens[i][j] = char(sum/9);
            if(color == 2) blues[i][j] = char(sum/9);
        }
    }
    pthread_exit(NULL);
}

void blur()
{
    pthread_t threads[3];
    int return_code;

    for (int color = 0; color < 3; color++)
    {
        return_code = pthread_create(&threads[color], NULL, smooth_color, (void*)color);
        if (return_code)
        {
            std::cout << "Error in create thread" << std::endl;
            exit(-1);
        }
    }
    for (int color = 0; color < 3; color++)
    {
        return_code = pthread_join(threads[color], NULL);
        if (return_code)
        {
            std::cout << "Error in join thread" << std::endl;
            exit(-1);
        }
    }
}