#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include "utils.hpp"

using namespace std;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#pragma pack(1)
#pragma once
#define WriteOutFile "Image.bmp"

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

int rows;
int cols;
unsigned char** reds;
unsigned char** greens;
unsigned char** blues;
unsigned char** temp_reds;
unsigned char** temp_greens;
unsigned char** temp_blues;


void blur() {
    int delta_i[8] = {1, 0, -1, 0, 1, -1, 1, -1};
    int delta_j[8] = {0, 1, 0, -1, 1, -1, -1, 1};


    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            int r_sum = int(temp_reds[i][j]);
            int g_sum =int(temp_greens[i][j]);
            int b_sum = int(temp_blues[i][j]);
            for(int k = 0; k<8; k++){
                if (ok(i + delta_i[k], j + delta_j[k], rows, cols)){
                    r_sum += int(temp_reds[i + delta_i[k]][j + delta_j[k]]);
                    g_sum += int(temp_greens[i + delta_i[k]][j + delta_j[k]]);
                    b_sum += int(temp_blues[i + delta_i[k]][j + delta_j[k]]);
                }
            }
            reds[i][j] = char(r_sum/9);
            greens[i][j] = char(g_sum/9);
            blues[i][j] = char(b_sum/9);
        }
    }

}


void sepia() {
    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            float r = float(reds[i][j]);
            float g = float(greens[i][j]);
            float b = float(blues[i][j]);
            reds[i][j] = fmin(round(0.393*r + 0.769*g + 0.189*b), 255);
            greens[i][j] = fmin(round(0.349*r + 0.686*g + 0.168*b), 255);
            blues[i][j] = fmin(round(0.272*r + 0.534*g + 0.131*b), 255);
        }
    }

}


void washedOut(){
    float r_mean = 0;
    float g_mean = 0;
    float b_mean = 0;
    mean(&r_mean, &g_mean, &b_mean, rows, cols, reds, greens, blues);
    r_mean = fmin(r_mean, 255);
    g_mean = fmin(g_mean, 255);
    b_mean = fmin(b_mean, 255);

    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            reds[i][j] = fmin(round(0.4*reds[i][j] + 0.6*r_mean), 255);
            greens[i][j] = fmin(round(0.4*greens[i][j] + 0.6*g_mean), 255);
            blues[i][j] = fmin(round(0.4*blues[i][j] + 0.6*b_mean), 255);
        }
    }
}


void multipleSign(){
    for(int i = 0; i< rows; i++){
        for(int j = 0; j < cols; j++){
            if (i == j ){
                reds[i][j] = 255;
                greens[i][j] = 255;
                blues[i][j] = 255;
                if (ok(i, j-1, rows, cols)){
                    reds[i][j-1] = 255;
                    greens[i][j-1] = 255;
                    blues[i][j-1] = 255;
                }

                if(ok(i, j+1, rows, cols)){
                    reds[i][j+1] = 255;
                    greens[i][j+1] = 255;
                    blues[i][j+1] = 255;
                }


                reds[i][cols - j] = 255;
                greens[i][cols - j] = 255;
                blues[i][cols - j] = 255;
                if(ok(i, cols - j - 1, rows, cols)){
                    reds[i][cols - j - 1] = 255;
                    greens[i][cols-j-1] = 255;
                    blues[i][cols-j-1] = 255;
                }

                if(ok(i, cols-j+1, rows, cols)){
                    reds[i][cols-j+1] = 255;
                    greens[i][cols-j+1] = 255;
                    blues[i][cols-j+1] = 255;
                }
            }
        }
    }
}


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

void getPixlesFromBMP24(int end, char* FileReadBuffer) {
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++){
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++) {
                switch (k) {
                    case 0:
                        reds[i][j] = FileReadBuffer[end - count++];
                        temp_reds[i][j] = reds[i][j];
                        break;
                    case 1:
                        greens[i][j] = FileReadBuffer[end - count++];
                        temp_greens[i][j] = greens[i][j];
                        break;
                    case 2:
                        blues[i][j] = FileReadBuffer[end - count++];
                        temp_blues[i][j] = blues[i][j];
                        break;
                }
            }
    }
}

void writeOutBmp24(char* FileBuffer, const char* NameOfFileToCreate, int BufferSize) {
    std::ofstream write(NameOfFileToCreate);
    if (!write) {
        cout << "Failed to write " << NameOfFileToCreate << endl;
        return;
    }
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++){
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++) {
                switch (k) {
                    case 0: //reds
                        FileBuffer[BufferSize - count] = reds[i][j];
                        break;
                    case 1: //green
                        FileBuffer[BufferSize - count] = greens[i][j];
                        break;
                    case 2: //blue
                        FileBuffer[BufferSize - count] = blues[i][j];
                        break;
                }
                count++;
            }
    }
    write.write(FileBuffer, BufferSize);
}




int main(int argc, char *argv[])
{
    auto start_time = std::chrono::high_resolution_clock::now();
    char *fileBuffer;
    int bufferSize;
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
    getPixlesFromBMP24(bufferSize, fileBuffer);
    blur();
    sepia();
    washedOut();
    multipleSign();

    writeOutBmp24(fileBuffer,  WriteOutFile,bufferSize);
    auto stop_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

    cout << duration.count() << std::endl;
    return 1;

}