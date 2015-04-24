#include <FaceWrapper.hpp>

#include <iostream>

FaceWrapper::FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight, int aCameraDeviceNo) :
	mIsValid(false),
	mFaceDetector(aPathToHaarCascade, aImageWidth, aImageHeight),
	mFaceRecognizer(aEigenFaceMetaFile),
	mCamera(aCameraDeviceNo)
{
	if (mFaceDetector && mFaceRecognizer && mCamera)
	{
		mIsValid = true;
	}
}

FaceWrapper::operator bool() const
{
	return mIsValid;
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
