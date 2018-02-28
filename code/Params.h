#pragma once

#include <opencv2/core.hpp>

#define WINDOW_NAME ("LK Demo")

struct GftParam
{
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	int gradientSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
};

struct LkParams
{
	cv::Size winSize = { 31, 31 };
	int maxLevel = 3;
	int flags = 0;
	double minEigThreshold = 0.001;
};
