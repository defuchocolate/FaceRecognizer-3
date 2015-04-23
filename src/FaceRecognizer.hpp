#ifndef FACERECOGNIZER_HPP
#define FACERECOGNIZER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

class FaceRecognizer
{
	bool mIsValid;
	const std::string mEigenFaceMetaFile;
	cv::Ptr<cv::FaceRecognizer> mModel;

	public:
		FaceRecognizer(const std::string& aEigenFaceMetaFile);
		~FaceRecognizer();

		explicit operator bool() const;

		bool TrainImage(std::vector<cv::Mat>& aImageMatrices, int aImageIdentifier);
		int FindIdentifierForFace(cv::Mat& aImage);
};

#endif
