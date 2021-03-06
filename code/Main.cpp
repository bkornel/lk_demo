#include <cctype>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "Tracking.h"


static void help()
{
	// print a welcome message, and the OpenCV version
	std::cout << "Demo of Lukas-Kanade optical flow." << std::endl << std::endl;
	std::cout << "It uses camera by default, but you can provide a path to video as an argument." << std::endl << std::endl;
	std::cout << "Hot keys:" << std::endl;
	std::cout << "\tESC - quit the program" << std::endl;
	std::cout << "\tr - auto-initialize tracking" << std::endl;
	std::cout << "\tc - delete all the points" << std::endl;
}


static void onMouse(int event, int x, int y, int flags, void* param)
{
	if (param)
	{
		Tracking* tracking = reinterpret_cast<Tracking*>(param);
		if (tracking && event == cv::EVENT_LBUTTONDOWN)
		{
			tracking->SetMousePoint(static_cast<float>(x), static_cast<float>(y));
		}
	}
}


int main(int argc, char** argv)
{
	cv::CommandLineParser parser(argc, argv, "{@input|0|}");
	std::string input = parser.get<std::string>("@input");
	cv::VideoCapture videoCapture;

	help();

	if (input.size() == 1 && isdigit(input[0]))
		videoCapture.open(input[0] - '0');
	else
		videoCapture.open(input);

	if (!videoCapture.isOpened())
	{
		std::cerr << "Could not initialize video capture" << std::endl;
		return -1;
	}

	cv::Mat frameBGR;
	Tracking tracking;

	cv::namedWindow(WINDOW_NAME, 1);
	cv::setMouseCallback(WINDOW_NAME, onMouse, &tracking);

	while (true)
	{
		videoCapture >> frameBGR;
		if (frameBGR.empty()) break;

		tracking.Process(frameBGR);
		tracking.Draw(frameBGR);

		cv::imshow(WINDOW_NAME, frameBGR);
		int key = cv::waitKey(1);
		if (key == 27) break;

		tracking.ProcessKey(key);
	}

	return 0;
}
