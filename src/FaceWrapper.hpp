#ifndef FACEWRAPPER_HPP
#define FACEWRAPPER_HPP

#include <FaceRecognizer.hpp>
#include <FaceDetector.hpp>
#include <Camera.hpp>

#include <thread>

class FaceWrapper
{
	bool mIsValid;
	FaceDetector mFaceDetector;
	FaceRecognizer mFaceRecognizer;
	Camera mCamera;

    bool mKeepThreadGoing;
	std::thread mGrabberThread;
	cv::Mat mBackBufferFrames[10];

    void GrabberThread();

	public:
		FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo);
        ~FaceWrapper();

		void StartProcess();

		explicit operator bool() const;
};

#endif
