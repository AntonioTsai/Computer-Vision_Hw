//
//  main.cpp
//  Hw2
//
//  Created by Antonio Tsai on 13/03/2017.
//  Copyright © 2017 TTU. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

static void help()
{
    cout
    << "\n--------------------------------------------------------------------------" << endl
    << "This program shows how to scan image objects in OpenCV (cv::Mat). As use case"
    << " we take an input image and divide the native color palette (255) with the "  << endl
    << "input. Shows C operator[] method, iterators and at function for on-the-fly item address calculation."<< endl
    << "Usage:"                                                                       << endl
    << "./howToScanImages imageNameToUse divideWith [G]"                              << endl
    << "if you add a G parameter the image is processed in gray scale"                << endl
    << "--------------------------------------------------------------------------"   << endl
    << endl;
}

Mat& ScanImageAndReduceC(Mat& I, int divideWith);
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* table);
Mat& ScanImageAndReduceRandomAccess(Mat& I, int divideWith);

int main( int argc, char* argv[])
{
    help();
    if (argc < 3)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }
    
    Mat I, J;
    if( argc == 4 && !strcmp(argv[3],"G") )
        I = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    else
        I = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    
    if (!I.data)
    {
        cout << "The image" << argv[1] << " could not be loaded." << endl;
        return -1;
    }
    
    int divideWith = 0; // convert our input string to number - C++ style
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if (!s || !divideWith)
    {
        cout << "Invalid number entered for dividing. " << endl;
        return -1;
    }
    
    cv::Mat clone_i;
    
    // The efficient way
    clone_i = I.clone();
    // Cal time
    double t_efficient = (double)getTickCount();
    J = ScanImageAndReduceC(clone_i, divideWith);
    // Cal time
    t_efficient = ((double)getTickCount() - t_efficient)/getTickFrequency();
    cout << "The efficient way process  " << t_efficient << " seconds" << endl;
    imwrite("efficient_way.png", J);
    
    // On-the-fly
    clone_i = I.clone();
    // Cal time
    double t_on_the_fly = (double)getTickCount();
    J = ScanImageAndReduceRandomAccess(clone_i, divideWith);
    // Cal time
    t_on_the_fly = ((double)getTickCount() - t_on_the_fly)/getTickFrequency();
    cout << "The On-the-fly way Process " << t_on_the_fly << " seconds" << endl;
    imwrite("on_the_fly.png", J);
    
    cout << "The efficient way is faster than On-the-fly way " << t_on_the_fly / t_efficient
        << "times." << endl;
    return 0;
}

Mat& ScanImageAndReduceC(Mat& I, int divideWith)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    
    int channels = I.channels();
    
    int nRows = I.rows;
    int nCols = I.cols * channels;
    
    int cols = I.cols * channels;
    
    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }
    
    uchar* p = I.ptr<uchar>(0);
    for(int row = 0; row < nCols; row += cols * divideWith) {
        for (int col = 0; col < cols; col += divideWith * channels) {
            int avg[3] = {0};
            int offset;
            int counts = 0;
            // Get average
            for (int _row = 0; _row < divideWith; _row++) {
                for (int _col = 0; _col < divideWith; _col++) {
                    offset = row + col + _row * cols + _col * channels;
                    if(offset < nCols) {
                        avg[0] += p[offset];
                        avg[1] += p[offset + 1];
                        avg[2] += p[offset + 2];
                        counts++;
                    }
                }
            }
            // Cal average
            for (int i = 0; i < 3; i++)
                avg[i] /= counts;
            // Set average
            for (int _row = 0; _row < divideWith; _row++) {
                for (int _col = 0; _col < divideWith; _col++) {
                    offset = row + col + _row * cols + _col * channels;
                    if(offset < nCols) {
                        p[offset] = avg[0];
                        p[offset + 1] = avg[1];
                        p[offset + 2] = avg[2];
                    }
                }
            }
        }
    }
    return I;
}

Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    
    const int channels = I.channels();
    switch(channels)
    {
        case 1:
        {
            MatIterator_<uchar> it, end;
            for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                *it = table[*it];
            break;
        }
        case 3:
        {
            MatIterator_<Vec3b> it, end;
            for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
            {
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];
            }
            
            //
            
        }
    }
    
    return I;
}

Mat& ScanImageAndReduceRandomAccess(Mat& I, int divideWith)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    
    const int channels = I.channels();
    switch(channels)
    {
        case 1:
        {
            for(int i = 0; i < I.rows; i += divideWith)
            {
                for( int j = 0; j < I.cols; j += divideWith)
                {
                    int avg = 0, counts = 0;
                    // Get average
                    for (int _x = 0; _x < divideWith && (i + _x) < I.rows; _x++)
                    {
                        for (int _y = 0; _y < divideWith && (j + _y) < I.cols; _y++)
                        {
                            avg += I.at<uchar>(i + _x, j + _y);
                            counts++;
                        }
                    }
                    // Cal average
                    avg /= counts;
                    // Set average
                    for (int _x = 0; _x < divideWith && (i + _x) < I.rows; _x++)
                        for (int _y = 0; _y < divideWith && (j + _y) < I.cols; _y++)
                            I.at<uchar>(i + _x, j + _y) = (uchar)avg;
                }
            }
            break;
        }
        case 3:
        {
            Mat_<Vec3b> _I = I;
            
            for(int i = 0; i < I.rows; i += divideWith)
            {
                for( int j = 0; j < I.cols; j += divideWith)
                {
                    int avg[3] = {0};
                    int counts = 0;
                    // Get average
                    for (int _x = 0; _x < divideWith && (i + _x) < I.rows; _x++)
                    {
                        for (int _y = 0; _y < divideWith && (j + _y) < I.cols; _y++)
                        {
                            avg[0] += _I(i + _x, j + _y)[0];
                            avg[1] += _I(i + _x, j + _y)[1];
                            avg[2] += _I(i + _x, j + _y)[2];
                            counts++;
                        }
                    }
                    // Cal average
                    for (int i = 0; i < 3; i++)
                        avg[i] /= counts;
                    // Set average
                    for (int _x = 0; _x < divideWith && (i + _x) < I.rows; _x++) {
                        for (int _y = 0; _y < divideWith && (j + _y) < I.cols; _y++){
                            _I(i + _x, j + _y)[0] = (uchar)avg[0];
                            _I(i + _x, j + _y)[1] = (uchar)avg[1];
                            _I(i + _x, j + _y)[2] = (uchar)avg[2];
                        }
                    }
                }
            }
            I = _I;
            break;
        }
    }
    
    return I;
}
