#pragma once

#include <vector>
#include <opencv2/core.hpp>

#include "Params.h"

class Tracking
{
	using PointVector = std::vector<cv::Point2f>;

public:
	Tracking();

	~Tracking();

	void Process(const cv::Mat& frameBGR);

	void ProcessKey(int key);

	void Draw(cv::Mat& frameBGR);

	void SetMousePoint(float x, float y);

private:
	static const int sMaxCorners = 500;
	static const int sHistorySize = 25;

	void DetectPoints(const cv::Mat& mask = cv::Mat());

	void TrackPoints();

	cv::Mat mFrameGray;
	cv::Mat mFrameGrayPrev;

	cv::Point2f mMousePoint;
	PointVector mCurrentPoints;
	PointVector mPreviousPoints;
	std::vector<PointVector> mHistory;
	cv::TermCriteria mTermCriteria;

	size_t mFrameNumber = 0u;
	bool mDetectPoints = false;
	bool mAddMousePoint = false;

	GftParam mGftParam;
	LkParams mLkParams;
};

