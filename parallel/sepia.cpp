#include "utils.hpp"
#include <iostream>
#include <pthread.h>
#include <cmath>

void* sepia_color(void* arg)
{
    long color = (long)arg;
    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            float r = float(reds[i][j]);
            float g = float(greens[i][j]);
            float b = float(blues[i][j]);
            if(color==0)temp_reds[i][j] = fmin(round(0.393*r + 0.769*g + 0.189*b), 255);
            if(color==1)temp_greens[i][j] = fmin(round(0.349*r + 0.686*g + 0.168*b), 255);
            if(color==2)temp_blues[i][j] = fmin(round(0.272*r + 0.534*g + 0.131*b), 255);
        }
        }

    pthread_exit(NULL);
}

void sepia()
{
    pthread_t threads[3];
    int return_code;

    for (int color = 0; color < 3; color++)
    {
        return_code = pthread_create(&threads[color], NULL, sepia_color, (void*)color);

        if (return_code)
        {
            std::cout<< "Error in create thread" << std::endl;
            exit(-1);
        }
    }
    for (int i = 0; i < 3; i++)
    {
        return_code = pthread_join(threads[i], NULL);
        if (return_code)
        {
            std::cout << "Error in join thread" << std::endl;
            exit(-1);
        }
    }
}