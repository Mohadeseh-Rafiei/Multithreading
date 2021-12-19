#include "utils.hpp"
#include <iostream>
#include <pthread.h>
#include <cmath>

void* washedOutColor(void* arg)
{
    long color = (long)arg;
    long long avg = 0;
    pthread_t threads[rows];
    float total_mean = 0;
    mean(&total_mean, color);

    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            if(color == 0) reds[i][j] = fmin(round(0.4*temp_reds[i][j] + 0.6*total_mean), 255);
            if(color == 1) greens[i][j] = fmin(round(0.4*temp_greens[i][j] + 0.6*total_mean), 255);
            if(color == 2) blues[i][j] = fmin(round(0.4*temp_blues[i][j] + 0.6*total_mean), 255);
        }
    }

    pthread_exit(NULL);
}

void washedOut()
{
    pthread_t threads[3];
    int return_code;

    for (int color = 0; color < 3; color++)
    {
        return_code = pthread_create(&threads[color], NULL, washedOutColor, (void*)color);

        if (return_code)
        {
            std::cout << "Error in creat thread" << std::endl;
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