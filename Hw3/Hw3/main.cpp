//
//  main.cpp
//  Hw3
//
//  Created by Antonio Tsai on 27/03/2017.
//  Copyright © 2017 TTU. All rights reserved.
//

#include<opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

#define LEFT_TOP 0
#define RIGHT_TOP 1
#define RIGHT_BOTTOM 2
#define LEFT_BOTTOM 3

void onMouse(int event,int x,int y,int flags,void* param);

IplImage *Imagex;   //original
IplImage *Image;    //modified
Point2f Vertex[4];
int Vertex_index;
CvScalar Color; //框框顏色
int Thickness;  //框框粗細
int Shift;  //框框大小(0為正常)
int key;    //按鍵碼
// Output Quadilateral or World plane coordinates
Point2f outputQuad[4];
// Lambda Matrix
Mat lambda( 2, 4, CV_32FC1 );
//Input and Output Image;
Mat input, output;


int main( )
{
    // Init
    Color = CV_RGB(0,135,216);
    Thickness = 2;
    Shift = 0;
    key = 0;
    Vertex_index = 0;
    
    //Load the image
    input = imread("/Users/Antonio/Documents/openCV/lena.jpg", 1);
    if(! input.data ) { // Check for invalid input
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    output = input.clone();
    
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );
    
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = Point2f( 0,input.rows-1  );
    
    // Show image
    namedWindow( "Original", WINDOW_AUTOSIZE );
    imshow("Original", input);
    setMouseCallback("Original", onMouse, NULL);//設定滑鼠callback函式
    waitKey(0);
    
    return 0;
}

void onMouse(int event,int x,int y,int flag,void* param){
    if(event==CV_EVENT_LBUTTONDOWN) {
        Vertex[Vertex_index] = Point2f(x, y);
        
        // Display line between clicked point
        if(Vertex_index > 0 && Vertex_index < 4) {
            line(input, Vertex[Vertex_index-1], Vertex[Vertex_index], Color, Thickness, 8, Shift);
            if (Vertex_index == 3) {
                line(input, Vertex[Vertex_index], Vertex[0], Color, Thickness, 8, Shift);
            }
        }
        
        imshow("Original", input);
        cout << "Click times: " << Vertex_index << " " << Vertex[Vertex_index] << endl;
        Vertex_index += 1;
        
        if(Vertex_index == 4) {
            Vertex_index++;
            cout << "Check vertex_index " << Vertex_index << endl;
            // Clone image & save to new file
            // Get the Perspective Transform Matrix i.e. lambda
            lambda = getPerspectiveTransform( Vertex, outputQuad );
            // Apply the Perspective Transform just found to the src image
            warpPerspective(output, output, lambda, output.size() );
            
            //Display input and output
            imshow("Output", output);
            key = waitKey(0);
            if(key == 27) {
                destroyAllWindows();
            }
        }
    }
}
