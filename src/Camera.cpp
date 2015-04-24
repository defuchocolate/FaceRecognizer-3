#include <Camera.hpp>

#include <iostream>

Camera::Camera(unsigned int aDeviceNumber) :
	mIsValid(false),
	mDeviceNumber(aDeviceNumber),
	mCamera(cvCaptureFromCAM(mDeviceNumber))
{
	if(nullptr != mCamera)
	{
		std::cerr << "Instantiation of Camera failed!" << std::endl;
		mIsValid = true;
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
