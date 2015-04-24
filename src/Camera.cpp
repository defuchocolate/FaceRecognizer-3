#include <Camera.hpp>

#include <iostream>
#include <chrono>

Camera::Camera(unsigned int aDeviceNumber) :
	mIsValid(false),
	mDeviceNumber(aDeviceNumber),
	mCamera(cvCaptureFromCAM(mDeviceNumber)),
	mKeepThreadGoing(true),
	mGrabberThread()
{
	if(nullptr != mCamera)
	{
		std::cerr << "Instantiation of Camera failed!" << std::endl;
		mIsValid = true;
	}
	else
	{
		mGrabberThread = std::thread(&Camera::GrabberThread, this);
	}
}

Camera::~Camera()
{

	cvReleaseCapture(&mCamera);
}

void Camera::GrabberThread()
{
	std::cout << "entering " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
	unsigned int currIdx = 0;

	while (mKeepThreadGoing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		mBackBufferFrames[currIdx] = cvQueryFrame(mCamera);
		currIdx = ((currIdx + 1) % 10);
	}

	std::cout << "leaving " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
}

cv::Mat Camera::snapshot()
{
    return cvQueryFrame(mCamera);
}

Camera::operator bool() const
{
	return mIsValid;
}
