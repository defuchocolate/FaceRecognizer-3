#include <Camera.hpp>

Camera::Camera(unsigned int aDeviceNumber) :
	mIsValid(false),
	mDeviceNumber(aDeviceNumber),
	mCamera(cvCaptureFromCAM(mDeviceNumber))
{
	if(nullptr != mCamera)
	{
		mIsValid = true;
	}
}

Camera::~Camera()
{
	cvReleaseCapture();
}

cv::Mat Camera::snapshot()
{
    return cvQueryFrame(camera);
}