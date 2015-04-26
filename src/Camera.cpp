#include <Camera.hpp>

#include <iostream>
#include <chrono>

Camera::Camera(unsigned int aDeviceNumber) :
	mIsValid(true),
	mDeviceNumber(aDeviceNumber),
	mCamera(cvCaptureFromCAM(mDeviceNumber))
{
	if(nullptr == mCamera)
	{
		std::cerr << "Instantiation of Camera failed!" << std::endl;
		//mIsValid = false;
	}
}

Camera::~Camera()
{
	cvReleaseCapture(&mCamera);
}

cv::Mat Camera::snapshot()
{
    return cvQueryFrame(mCamera);
}

Camera::operator bool() const
{
	return mIsValid;
}
