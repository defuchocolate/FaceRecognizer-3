#ifndef FACEDETECTOR_HPP
#define FACEDETECTOR_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>

class FaceDetector final
{
	bool mIsValid;
	const std::string mPathToHaarCascade;
	cv::CascadeClassifier mHaarCascade;

	int mImageWidth;
	int mImageHeight;

	public:
		FaceDetector(const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight);
		~FaceDetector();

		bool DetectFace(const cv::Mat& aVideoFrame, std::vector<cv::Mat>& aFoundFaces);

		explicit operator bool() const;
};

#endif
