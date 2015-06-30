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
		mIsValid = false;
	}
}

Camera::~Camera()
{
	mCamera.release();
}

cv::Mat Camera::snapshot()
{
	// workaround to clear camera backbuffer to get realtime frame:
	for(int i = 0; i < 5; i++)
	{
		mCamera.grab();
		mCamera.retrieve(currentFrame);
	}

	// get the frame we really want:
	if(mCamera.grab())
	{
		mCamera.retrieve(currentFrame);
	}
    return currentFrame;
}

Camera::operator bool() const
{
	return mIsValid;
}
