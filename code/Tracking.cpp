#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "Tracking.h"

Tracking::Tracking() :
	mTermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03),
	mHistory(sHistorySize)
{
}


Tracking::~Tracking()
{
}


void Tracking::Process(const cv::Mat& frameBGR)
{
	CV_Assert(!frameBGR.empty() && frameBGR.channels() == 3);

	cv::cvtColor(frameBGR, mFrameGray, cv::COLOR_BGR2GRAY);

	if (mDetectPoints)
	{
		DetectPoints();

		mAddMousePoint = false;
		mDetectPoints = false;
	}
	else if (!mPreviousPoints.empty())
	{
		if (mFrameGrayPrev.empty())
			mFrameGray.copyTo(mFrameGrayPrev);

		TrackPoints();
	}

	if (mAddMousePoint && mCurrentPoints.size() < sMaxCorners)
	{
		PointVector tmp = { mMousePoint };
		cv::cornerSubPix(mFrameGray, tmp, mLkParams.winSize, { -1, -1 }, mTermCriteria);
		mCurrentPoints.push_back(tmp[0]);
		mAddMousePoint = false;
	}

	mHistory[mFrameNumber++ % sHistorySize] = mCurrentPoints;

	std::swap(mCurrentPoints, mPreviousPoints);
	cv::swap(mFrameGrayPrev, mFrameGray);
}


void Tracking::DetectPoints(const cv::Mat& mask)
{
	cv::goodFeaturesToTrack(mFrameGray, mCurrentPoints, sMaxCorners, mGftParam.qualityLevel,
		mGftParam.minDistance, mask, mGftParam.blockSize, mGftParam.gradientSize,
		mGftParam.useHarrisDetector, mGftParam.k
	);

	cv::cornerSubPix(mFrameGray, mCurrentPoints, { 10, 10 }, {-1, -1}, mTermCriteria);
}


void Tracking::TrackPoints()
{
	std::vector<uchar> status;
	std::vector<float> err;

	cv::calcOpticalFlowPyrLK(mFrameGrayPrev, mFrameGray, mPreviousPoints, mCurrentPoints, 
		status, err, mLkParams.winSize, mLkParams.maxLevel, mTermCriteria, mLkParams.flags, 
		mLkParams.minEigThreshold
	);

	size_t i, k;
	for (i = k = 0; i < mCurrentPoints.size(); i++)
	{
		if (mAddMousePoint)
		{
			if (cv::norm(mMousePoint - mCurrentPoints[i]) <= 5)
			{
				mAddMousePoint = false;
				continue;
			}
		}

		if (!status[i]) continue;

		mCurrentPoints[k++] = mCurrentPoints[i];
	}

	mCurrentPoints.resize(k);
}

void Tracking::Draw(cv::Mat& frameBGR)
{
	for (const auto& history : mHistory)
	{
		for (const auto& point : history)
		{
			cv::circle(frameBGR, point, 1, { 0, 255, 0 }, -1);
		}
	}

	for (const auto& point : mCurrentPoints)
	{
		cv::circle(frameBGR, point, 4, { 255, 255, 255 }, -1);
		cv::circle(frameBGR, point, 3, { 0, 255, 0 }, -1);
	}
}

void Tracking::ProcessKey(int key)
{
	if (key == 'r')
	{
		mDetectPoints = true;
	}
	else if (key == 'c')
	{
		mCurrentPoints.clear();
		mPreviousPoints.clear();
	}
}

void Tracking::SetMousePoint(float x, float y)
{
	mMousePoint = { x, y };
	mAddMousePoint = true;
}
