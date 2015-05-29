#ifndef FACEWRAPPER_HPP
#define FACEWRAPPER_HPP

#include <FaceRecognizer.hpp>
#include <FaceDetector.hpp>
#include <Camera.hpp>

#include <thread>
#include <mutex>

class FaceWrapper
{
	bool mIsValid;
	FaceDetector mFaceDetector;
	FaceRecognizer mFaceRecognizer;
	Camera mCamera;

    bool mKeepThreadGoing;
	std::thread mGrabberThread;
	std::mutex mSnapshotBufferMutex;
	cv::Mat mBackBufferFrames[10];

    void GrabberThread();

	public:
		FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo);
        ~FaceWrapper();

		void StartProcess();

		explicit operator bool() const;
};

#endif
