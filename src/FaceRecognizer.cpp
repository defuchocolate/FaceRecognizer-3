#include <FaceRecognizer.hpp>

static int handleError(int, const char*, const char*, const char*, int, void*)
{
    return 0;
}

FaceRecognizer::FaceRecognizer(const std::string& aEigenFaceMetaFile) :
	mIsValid(true),
	mEigenFaceMetaFile(aEigenFaceMetaFile),
	mModel(cv::createEigenFaceRecognizer()) // createLBPHFaceRecognizer(), createFisherFaceRecognizer()
{
	cv::redirectError(handleError);
	try
	{
		mModel->load(mEigenFaceMetaFile);
	}
	catch(cv::Exception& e)
	{
		mIsValid = false;
	}
}

FaceRecognizer::~FaceRecognizer()
{
}

FaceRecognizer::operator bool() const
{
	return mIsValid;
}

bool FaceRecognizer::TrainImage(cv::Mat aImageMatrix, unsigned int aImageIdentifier)
{
	std::vector<cv::Mat> images(1);
	std::vector<int> labels(1);

	images.push_back(cv::imdecode(aImageData, CV_LOAD_IMAGE_GRAYSCALE));
	labels.push_back(aImageIdentifier);

	mModel->train(images, labels);
	return true;
}
