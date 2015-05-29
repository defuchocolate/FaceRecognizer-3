#include <Camera.hpp>

#include <iostream>
#include <chrono>

Camera::Camera(unsigned int aDeviceNumber) :
	mIsValid(true),
	mDeviceNumber(aDeviceNumber),
	mCamera(mDeviceNumber),
	currentFrame()
{
	cv::waitKey(100);
	if(!mCamera.isOpened())
	{
		std::cerr << "Instantiation of Camera failed!" << std::endl;
		//mIsValid = false;
	}
}

Camera::~Camera()
{
	mCamera.release();
}

cv::Mat Camera::snapshot()
{
	if(mCamera.grab())
	{
		std::cout << "grabbed frame!" << std::endl;
		mCamera.retrieve(currentFrame);
	}
    return currentFrame;
}

Camera::operator bool() const
{
	return mIsValid;
}
