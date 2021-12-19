#include "utils.hpp"
#include <iostream>
#include <pthread.h>

void* multipleSign_color(void* arg)
{
    long color = (long)arg;
    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            if (i == j && ok(i, j)){
                if(color == 0) reds[i][j] = 255;
                else if(color == 1) greens[i][j] = 255;
                else if(color == 2) blues[i][j] = 255;
                if (ok(i, j-1)){
                    if(color == 0) reds[i][j-1] = 255;
                    else if(color == 1) greens[i][j-1] = 255;
                    else if(color == 2) blues[i][j-1] = 255;
                }

                if(ok(i, j+1)){
                    if(color == 0)reds[i][j+1] = 255;
                    else if(color == 1)greens[i][j+1] = 255;
                    else if(color == 2)blues[i][j+1] = 255;
                }


                if(color == 0)reds[i][cols - j] = 255;
                else if(color == 1)greens[i][cols - j] = 255;
                else if(color == 2)blues[i][cols - j] = 255;
                if(ok(i, cols - j - 1)){
                    if(color == 0)reds[i][cols - j - 1] = 255;
                    else if(color == 1)greens[i][cols-j-1] = 255;
                    else if(color == 2)blues[i][cols-j-1] = 255;
                }

                if(ok(i, cols-j+1)){
                    if(color == 0)reds[i][cols-j+1] = 255;
                    else if(color == 1)greens[i][cols-j+1] = 255;
                    else if(color == 2)blues[i][cols-j+1] = 255;
                }
            }
        }
    }
    pthread_exit(NULL);
}

void multipleSign()
{
    pthread_t threads[3];
    int return_code;

    for (int color = 0 ; color < 3; color++)
    {
        return_code = pthread_create(&threads[color], NULL, multipleSign_color, (void*)color);

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