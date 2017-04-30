//
//  main.cpp
//  Hw4-2
//
//  Created by Antonio Tsai on 27/03/2017.
//  Copyright © 2017 TTU. All rights reserved.
//
/**
 * @function EqualizeHist_Demo.cpp
 * @brief Demo code for equalizeHist function
 * @author OpenCV team
 */

// #include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void draw(Mat src, string str) {
    /// Draw -------------------------------------------------------------------------
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
    }
    
    /// Display
    namedWindow(str, CV_WINDOW_AUTOSIZE );
    imshow(str, histImage );
}

/**
 * @function main
 */
int main( int, char** argv )
{
    Mat src, dst;
    
    const char* source_window = "Source image";
    const char* equalized_window = "Equalized Image";
    
    /// Load image
    src = imread( argv[1], IMREAD_COLOR );
    
    if( src.empty() )
    { cout<<"Usage: ./EqualizeHist_Demo <path_to_image>"<<endl;
        return -1;
    }
    
    /// Convert to grayscale
    cvtColor( src, src, COLOR_BGR2GRAY );
    draw(src, "Origin");
    
    /// Apply Histogram Equalization
    equalizeHist( src, dst );
    
    /// Display results
    namedWindow( source_window, WINDOW_AUTOSIZE );
    namedWindow( equalized_window, WINDOW_AUTOSIZE );
    
    imshow( source_window, src );
    imshow( equalized_window, dst );
    
    draw(dst, "Histogram");
    /// Wait until user exits the program
    waitKey(0);
    
    return 0;
    
}
