#include "opencv2\opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	int i = 0, j = 0;
	int keyboard = 0;
	VideoCapture cap("C:/Users/Public/Videos/Sample Videos/animal.wmv"); // open the video file for reading

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

	cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	while (1)
	{
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		keyboard = waitKey(30);
		int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
		int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

		if (keyboard == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

		if (keyboard == 0x53)
		{
			cout << "Keypress S Captured!\n";
			char filename[80];
			sprintf(filename, "C:/Capture_Image_%d.png", i);
			imwrite(filename, frame);
			i++;
		}

		if (keyboard == 0x56)
		{
			cout << "Keypress V Captured!\n";
			char filename[80];
			sprintf(filename, "C:/Capture_video_%d.avi", j);

			VideoWriter video(filename, CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);
			for (;;){

				Mat frame;
				cap >> frame;
				video.write(frame);
				imshow("Frame", frame);
				char c = (char)waitKey(33);
				if (c == 27) break;
			}
		}
	}

	return 0;

}