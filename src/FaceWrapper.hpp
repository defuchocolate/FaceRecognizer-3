#ifndef FACEWRAPPER_HPP
#define FACEWRAPPER_HPP

#include <FaceRecognizer.hpp>
#include <FaceDetector.hpp>
#include <Camera.hpp>

class FaceWrapper
{
	bool mIsValid;
	FaceDetector mFaceDetector;
	FaceRecognizer mFaceRecognizer;
	Camera mCamera;

	public:
		FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo);

		void StartProcess();

		explicit operator bool() const;
};

#endif
