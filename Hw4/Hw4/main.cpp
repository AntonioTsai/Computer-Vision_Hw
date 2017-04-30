//
//  main.cpp
//  Hw4
//
//  Created by Antonio Tsai on 27/03/2017.
//  Copyright © 2017 TTU. All rights reserved.
//

#include <opencv2/core/core.hpp>
// #include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */
double gammas;

int main( int argc, char** argv )
{
    /// Read image given by user
    Mat image = imread( argv[1] );
    Mat new_image = Mat::zeros( image.size(), image.type() );
    
    /// Initialize values
    std::cout<<" Basic Linear Transforms "<<std::endl;
    std::cout<<"-------------------------"<<std::endl;
    std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
    std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;
    // Add garma
    std::cout<<"* Enter the gamma value [0-1.0]: "; std::cin>>gammas;
    
    /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
    for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
    { for( int c = 0; c < 3; c++ )
    {
        new_image.at<Vec3b>(y,x)[c] =
        saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
        new_image.at<Vec3b>(y,x)[c] = pow((double)new_image.at<Vec3b>(y,x)[c] / 255, gammas) * 255;
    }
    }
    }
    
    /// Create Windows
    namedWindow("Original Image", 1);
    namedWindow("New Image", 1);
    
    /// Show stuff
    imshow("Original Image", image);
    imshow("New Image", new_image);
    
    /// Wait until user press some key
    waitKey();
    return 0;
}
