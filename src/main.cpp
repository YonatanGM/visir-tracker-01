#include "types.h"


CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
void detectAndDisplay(Mat frame);

int main() {
	VideoCapture camera;

	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};

	const std::string face_cascade_name = "C:/Users/Yonathan/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	//load cascade
	if (!face_cascade.load(face_cascade_name))
	{
		std::cout << "--(!)Error loading face cascade\n";
		return -1;
	};

	// Main loop
	Mat img;
	double elapsed = 0;
	int frame = 0;

	for(;;) {
		double t = cv::getTickCount();
		camera >> img;
		//imshow("Camera", img);
		detectAndDisplay(img);
		int key = waitKey(5);	
		if (key == 27 || key == 'q') break;

		elapsed += (cv::getTickCount() - t) / cv::getTickFrequency();
		frame++;

		if (elapsed >= 2.0) {
			//average
			std::cout << "FPS: " <<  frame / elapsed << std::endl;
			elapsed = 0;
			frame = 0;	
			//instantaneous 
			//std::cout << "FPS: " << 1 / (double)((cv::getTickCount() - t) / cv::getTickFrequency()) << std::endl;
		}
	}
	camera.release();
	return 0;
}



void detectAndDisplay(Mat frame)
{
	//https://docs.opencv.org/4.2.0/db/d28/tutorial_cascade_classifier.html
	Mat frame_gray;
	frame_gray.convertTo(frame_gray, CV_64F);
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	std::vector<Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		Mat faceROI = frame_gray(faces[i]);
	}
	imshow("Capture - Face detection", frame);
}