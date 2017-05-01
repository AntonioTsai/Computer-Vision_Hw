//
//  main.cpp
//  Hw6-2
//
//  Created by Antonio Tsai on 01/05/2017.
//  Copyright © 2017 TTU. All rights reserved.
//

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

/** @function main */
int main(int argc, const char * argv[])
{
    Mat src, src_gray, lapla, dst;
    int kernel_size = 3;
    int scale = 2;
    int delta = 0;
    int ddepth = CV_8U;
    double alpha = 0.5;
    double beta;
    char* window_name = (char*)"Original";
    char* laplacian_name = (char*)"Laplacian";
    char* addition = (char*)"Addition";
    
    
    /// Load an image
    src = imread( "/Users/Antonio/Documents/openCV/moon.png", 1 );
    
    if( !src.data )
    {
        cout << "Error occur when reading image." << endl;
        return -1;
    }
    
    /// Remove noise by blurring with a Gaussian filter
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
    
    /// Convert the image to grayscale
    cvtColor( src, src_gray, CV_BGR2GRAY );
    
    /// Create window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    namedWindow( laplacian_name, CV_WINDOW_AUTOSIZE );
    namedWindow( addition, CV_WINDOW_AUTOSIZE );
    
    /// Apply Laplace function
    Mat abs_dst;
    
    Laplacian( src_gray, lapla, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( dst, abs_dst );
    
    // Blending
    cvtColor(lapla, lapla, CV_GRAY2BGR);
    beta = ( 1.0 - alpha );
    addWeighted( lapla, alpha, src, beta, 0.0, dst);
    //dst = lapla + src;
    
    /// Show what you got
    imshow( window_name, src );
    imshow(laplacian_name, lapla);
    imshow(addition, dst);
    
    waitKey(0);
    
    return 0;
}
