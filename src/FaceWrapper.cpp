#include <FaceWrapper.hpp>

#include <iostream>

namespace
{
	const unsigned short SIZE_OF_BACKBUFFER = 10;
}

FaceWrapper::FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo, bool aStartThread) :
	mIsValid(false),
	mFaceDetector(aPathToHaarCascade, aImageWidth, aImageHeight),
	mFaceRecognizer(aEigenFaceMetaFile),
	mCamera(aCameraDeviceNo),
    mKeepThreadGoing(aStartThread),
	mGrabberThread(),
	mSnapshotBufferMutex(),
	mBackBufferFrames(),
	mNumOfBackBufferFrames(0)
{
	if (mFaceDetector && mFaceRecognizer && mCamera)
	{
		mIsValid = true;
		if (aStartThread)
		{
			mGrabberThread = std::thread(&FaceWrapper::GrabberThread, this);
		}
	}
}

FaceWrapper::~FaceWrapper()
{
    if (mKeepThreadGoing)
    {
		mKeepThreadGoing = false;
		mGrabberThread.join();
	}
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
		// lock mutex for race condition protection:
		mSnapshotBufferMutex.lock();
		mBackBufferFrames[currIdx] = mCamera.snapshot();

		if (mNumOfBackBufferFrames < SIZE_OF_BACKBUFFER)
		{
			mNumOfBackBufferFrames++;
		}
		mSnapshotBufferMutex.unlock();

		//std::cout << "size: " << mBackBufferFrames[currIdx].size().width << "x" << mBackBufferFrames[currIdx].size().height << std::endl;
        //std::cout << "currIdx: " << currIdx << std::endl;
		currIdx = ((currIdx + 1) % SIZE_OF_BACKBUFFER);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "leaving " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
}

/**
 * @brief this function is called by external button press
 */
void FaceWrapper::StartProcess()
{
	std::vector<cv::Mat> detectedFaces;

	// lock snapshot backbuffer
	mSnapshotBufferMutex.lock();
	if (mNumOfBackBufferFrames > 0)
	{
		// iterate over all backbuffer frames and try to detect a face:
		for (unsigned short i = 0; i < mNumOfBackBufferFrames; i++)
		{
			if (mFaceDetector.DetectFace(mBackBufferFrames[i], detectedFaces))
			{
				std::cout << "detect " << detectedFaces.size() << " faces on picture " << i << std::endl;
				for (auto face : detectedFaces)
				{
					std::cout << "FACE FOUND on picture index: " << i << "!!" << std::endl;
					cv::namedWindow("DetectedFace", cv::WINDOW_AUTOSIZE );// Create a window for display.
					cv::imshow("DetectedFace", face);
					cv::waitKey(200);
					cv::destroyWindow("DetectedFace");

					/*int faceIdentifier = mFaceRecognizer.FindIdentifierForFace(face);
					if (0 <= faceIdentifier)
					{
						std::cout << "found person: " << faceIdentifier << std::endl;
					}
					else
					{
						std::cout << "unknown person!" << std::endl;
					}*/
				}
			}
			else
			{
				std::cout << "no faces detected on snapshot!" << std::endl;
			}

			detectedFaces.clear();
		}
	}
	mSnapshotBufferMutex.unlock();
}

void FaceWrapper::StartTrainSession()
{
	std::cout << "making 10 snapshots for importing..." << std::endl;

	cv::Mat frame;
	for (int i = 0; i < 10; i++)
	{
		frame = mCamera.snapshot();
		cv::namedWindow("DetectedFace", cv::WINDOW_AUTOSIZE );// Create a window for display.
		cv::imshow("DetectedFace", frame);
		std::cout << "use this picture for trainign? press Y or N!" << std::endl;
		char c = cv::waitKey(0);
		cv::destroyWindow("DetectedFace");

		if (c == 'Y' || c == 'y')
		{
			std::cout << "start training..." << std::endl;
		}

		frame.release();
	}
}
