//#include "utils.hpp"
//#include <iostream>
//#include <pthread.h>
//#include <cmath>
//
//void* getPixlesFromBMP24(void* arg) { // end is BufferSize (total size of file)
//    long color = (long)arg;
//    int count = 1;
//    int extra = cols % 4; // The nubmer of bytes in a row (cols) will be a multiple of 4.
//    for (int i = 0; i < rows; i++){
//        count += extra;
//        for (int j = cols - 1; j >= 0; j--) {
//            if (color == 0) {
//                reds[i][j] = fileBuffer[bufferSize - count++];
//                temp_reds[i][j] = reds[i][j];
//            }
//            else if (color == 1) {
//                greens[i][j] = fileBuffer[bufferSize - count++];
//                temp_greens[i][j] = greens[i][j];
//            }
//            else if (color == 2) {
//                blues[i][j] = fileBuffer[bufferSize - count++];
//                temp_blues[i][j] = blues[i][j];
//            }
//        }
//    }
//}
//
//void readPixels(){
//    pthread_t threads[3];
//    int return_code;
//
//    for (int color = 0 ; color < 3; color++)
//    {
//        return_code = pthread_create(&threads[color], NULL, getPixlesFromBMP24, (void*)color);
//
//        if (return_code)
//        {
//            std::cout << "Error in creat thread for corss_color" << std::endl;
//            exit(-1);
//        }
//    }
//    for (int color = 0; color < 3; color++)
//    {
//        return_code = pthread_join(threads[color], NULL);
//
//        if (return_code)
//        {
//            std::cout << "Error in join thread from cross_color" << std::endl;
//            exit(-1);
//        }
//    }
//}