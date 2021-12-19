#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <chrono>
#include "utils.hpp"

using namespace std;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#pragma pack(1)
#pragma once
#ifndef _read_img_cpp
#define _read_img_cpp
#define WriteOutFile "Image.bmp"

char* fileBuffer;
int bufferSize;
int rows;
int cols;
unsigned char** reds;
unsigned char** greens;
unsigned char** blues;
unsigned char** temp_reds;
unsigned char** temp_greens;
unsigned char** temp_blues;

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;


#endif

void RGB_Allocate(unsigned char**& dude) {
    dude = new unsigned char*[rows];
    for (int i = 0; i < rows; i++)
        dude[i] = new unsigned char[cols];
}


bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
    std::ifstream file(fileName);

    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer = new char[length];
        file.read(&buffer[0], length);

        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;

        file_header = (PBITMAPFILEHEADER)(&buffer[0]);
        info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
        rows = info_header->biHeight;
        cols = info_header->biWidth;
        bufferSize = file_header->bfSize;
        return 1;
    }
    else
    {
        cout << "File" << fileName << " doesn't exist!" << endl;
        return 0;
    }
}

struct read_arg{
    pair<int, int> row;
    pair<int, int> col;
};

void *getPixlesFromBMP24(void *args) { // end is BufferSize (total size of file)
    read_arg *arg = (read_arg*)args;
    int count = 1  + ((arg->row.first * cols) + (cols  - arg->col.second)) * 3;
    cout<<arg->row.first<<" " << arg->col.first<<endl;
    int extra = cols % 4; // The nubmer of bytes in a row (cols) will be a multiple of 4.
    for (int i = arg->row.first; i < arg->row.second; i++) {
        count += extra;
        for (int j = arg->col.second - 1; j >= arg->col.first; j--){
            for (int k = 0; k < 3; k++) {
                switch (k) {
                    case 0:
                        reds[i][j] = fileBuffer[bufferSize - count++];
                        temp_reds[i][j] = reds[i][j];
                        break;
                    case 1:
                        greens[i][j] = fileBuffer[bufferSize - count++];
                        temp_greens[i][j] = greens[i][j];
                        break;
                    case 2:
                        blues[i][j] = fileBuffer[bufferSize - count++];
                        temp_blues[i][j] = blues[i][j];
                        break;
                }
            }
        }
        count += (cols - (arg->col.second - arg->col.first)) * 3;
    }
    pthread_exit(NULL);
}

void *writeOutBmp24(void *args) {
    read_arg *arg = (read_arg*)args;

    int count = 1;
    int extra = cols % 4; // The nubmer of bytes in a row (cols) will be a multiple of 4.
    for (int i = arg->row.first; i < arg->row.second; i++) {
        count += extra;
        for (int j = arg->col.second - 1; j >= arg->col.first; j--){
            for (int k = 0; k < 3; k++) {
                switch (k) {
                    case 0: //reds
                        fileBuffer[bufferSize - count] = reds[i][j];
                        break;
                    case 1: //green
                        fileBuffer[bufferSize - count] = greens[i][j];
                        break;
                    case 2: //blue
                        fileBuffer[bufferSize - count] = blues[i][j];
                        break;
                }
            }
        }
        count+= (cols - (arg->col.second - arg->col.first)) * 3;
    }
    pthread_exit(NULL);
}


void readPixels(){
    pthread_t threads[4];
    int return_code;
    read_arg args;
    int count = 0;
    for (int i = 0 ; i < 2; i++)
    {
        args.row.first = i*rows/2;
        args.row.second = (i+1)*rows/2;

        for(int j = 0; j < 2; j++){
            args.col.first = j*cols/2;
            args.col.second = (j+1)*cols/2;
            cout<<"read "<<args.col.first<<" "<<args.col.second<<" "<<args.row.first<<" "<<args.row.second<<endl;
            return_code = pthread_create(&threads[count], NULL, getPixlesFromBMP24, (read_arg*)&args);
            if (return_code)
            {
                std::cout << "Error in creat thread for corss_color" << std::endl;
                exit(-1);
            }
            count += 1;
        }
    }
    for (int color = 0; color < 4; color++)
    {
        return_code = pthread_join(threads[color], NULL);
        cout<<"hui"<<endl;

        if (return_code)
        {
            std::cout << "Error in join thread from cross_color" << std::endl;
            exit(-1);
        }
    }

//    for(int i =0 ; i < rows; i++){
//        for(int j = 0; j < cols; j++){
//            cout<<int(reds[i][j])<<endl;
//        }
//    }
}

void writeFile(){
    std::ofstream write(WriteOutFile);
    if (!write) {
        cout << "Failed to write " << WriteOutFile << endl;
        return;
    }

    pthread_t threads[4];
    int return_code;
    read_arg args;
    int count = 0;
    for (int i = 0 ; i < 2; i++)
    {
        args.row.first = i*rows/2;
        args.row.second = (i+1)*rows/2;

        for(int j = 0; j < 2; j++){
            args.col.first = j*cols/2;
            args.col.second = (j+1)*cols/2;
            cout<<"hui "<<args.col.first<<" "<<args.col.second<<" "<<args.row.first<<" "<<args.row.second<<endl;
            return_code = pthread_create(&threads[count], NULL, writeOutBmp24, (read_arg*)&args);
            if (return_code)
            {
                std::cout << "Error in creat thread for corss_color" << std::endl;
                exit(-1);
            }
            count += 1;
        }
    }

    for (int color = 0; color < 4; color++)
    {
        return_code = pthread_join(threads[color], NULL);
        cout<<"hui"<<endl;

        if (return_code)
        {
            std::cout << "Error in join thread from cross_color" << std::endl;
            exit(-1);
        }
    }
    write.write(fileBuffer, bufferSize);

}

int main(int argc, char *argv[])
{
    auto start_time = std::chrono::high_resolution_clock::now();

    char *fileName = argv[1];
    if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
    {
        cout << "File read error" << endl;
        return 1;
    }
    RGB_Allocate(reds);
    RGB_Allocate(greens);
    RGB_Allocate(blues);

    RGB_Allocate(temp_reds);
    RGB_Allocate(temp_greens);
    RGB_Allocate(temp_blues);
//    getPixlesFromBMP24( reds,  greens, blues,bufferSize, rows, cols, fileBuffer);
    readPixels();

    blur();
//    sepia();
//    washedOut();
//    multipleSign();
    writeFile();
//    writeOutBmp24(fileBuffer,  WriteOutFile,bufferSize);
    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);
    std::cout <<  duration.count() << std::endl;
    return 1;

}