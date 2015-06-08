#include <FaceDetector.hpp>

#include <iostream>

FaceDetector::FaceDetector(const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight) :
	mIsValid(true),
	mPathToHaarCascade(aPathToHaarCascade),
	mHaarCascade(),
	mImageWidth(aImageWidth),
	mImageHeight(aImageHeight)
{
	try
	{
		mHaarCascade.load(aPathToHaarCascade);
	}
	catch(cv::Exception& e)
	{
		std::cerr << "Instantiation of FaceDetector failed!" << std::endl;
		mIsValid = false;
	}
}

FaceDetector::~FaceDetector()
{

}

FaceDetector::operator bool() const
{
	return mIsValid;
}

bool FaceDetector::DetectFace(const cv::Mat& aVideoFrame, std::vector<cv::Mat>& aFoundFaces)
{
	cv::Mat original = aVideoFrame.clone();

	// Convert the current frame to grayscale:
	cv::Mat gray;
	cvtColor(original, gray, CV_BGR2GRAY);

	// Find the faces in the frame:
	std::vector<cv::Rect_<int>> faces;
	mHaarCascade.detectMultiScale(gray, faces);

	for (auto currFace : faces)
	{
		// Crop the face from the image. So simple with OpenCV C++:
		cv::Mat face = gray(currFace);
		cv::Mat face_resized;
		cv::resize(face, face_resized, cv::Size(mImageWidth, mImageHeight), 1.0, 1.0, cv::INTER_CUBIC);

		aFoundFaces.push_back(face_resized);
	}

	if (0 < aFoundFaces.size())
	{
		return true;
	}
	return false;
}
