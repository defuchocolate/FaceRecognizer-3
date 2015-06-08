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
	unsigned int mLastId;

	public:
		FaceRecognizer(const std::string& aEigenFaceMetaFile);
		~FaceRecognizer();

		explicit operator bool() const;

		bool Train(const std::vector<cv::Mat>& aImageMatrices, const std::vector<int>& aIdentifiers, const std::vector<std::string>& aNames);
		int FindIdentifierForFace(cv::Mat& aImage);

		std::string GetNameOfId(const int aIdentifier);
};

#endif
