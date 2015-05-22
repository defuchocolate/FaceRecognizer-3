#include <FaceWrapper.hpp>

#include <iostream>

FaceWrapper::FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo) :
	mIsValid(false),
	mFaceDetector(aPathToHaarCascade, aImageWidth, aImageHeight),
	mFaceRecognizer(aEigenFaceMetaFile),
	mCamera(aCameraDeviceNo),
    mKeepThreadGoing(true),
	mGrabberThread()
{
	if (mFaceDetector && mFaceRecognizer && mCamera)
	{
		mIsValid = true;
        mGrabberThread = std::thread(&FaceWrapper::GrabberThread, this);
	}
}

FaceWrapper::~FaceWrapper()
{
    mKeepThreadGoing = false;
    mGrabberThread.join();
}

FaceWrapper::operator bool() const
{
	return mIsValid;
}

void FaceWrapper::GrabberThread()
{
	std::cout << "entering " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
	unsigned int currIdx = 0;

	while (mKeepThreadGoing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		//mBackBufferFrames[currIdx] = mCamera.snapshot();
		cv::Mat test = mCamera.snapshot();

		std::cout << "size: " << test.size().height << std::endl;
		//namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
		imshow( "Display window", test);                   // Show our image inside it.

        std::cout << "currIdx: " << currIdx << std::endl;
		currIdx = ((currIdx + 1) % 10);
	}

	std::cout << "leaving " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
}

void FaceWrapper::StartProcess()
{
	cv::Mat snapshot = mCamera.snapshot();
	std::vector<cv::Mat> detectedFaces;

	if (mFaceDetector.DetectFace(snapshot, detectedFaces))
	{
		for (auto face : detectedFaces)
		{
			int faceIdentifier = mFaceRecognizer.FindIdentifierForFace(face);
			if (0 <= faceIdentifier)
			{
				std::cout << "found person: " << faceIdentifier << std::endl;
			}
			else
			{
				std::cout << "unknown person!" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "no faces detected on snapshot!" << std::endl;
	}
}
