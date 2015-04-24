#include <FaceRecognizer.hpp>
#include <iostream>

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
		std::cerr << "Instantiation of FaceRecognizer failed!" << std::endl;
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

bool FaceRecognizer::TrainImage(std::vector<cv::Mat>& aImageMatrices, int aImageIdentifier)
{
	if (aImageIdentifier < 0)
	{
		return false;
	}

	std::vector<int> labels(aImageMatrices.size(), aImageIdentifier);

	mModel->train(aImageMatrices, labels);

	// save the updated model:
	try
	{
		mModel->save(mEigenFaceMetaFile);
	}
	catch(cv::Exception& e)
	{
		return false;
	}

	return true;
}

int FaceRecognizer::FindIdentifierForFace(cv::Mat& aImage)
{
	int predictLabel = -1;
	double confidence = 0.0;
	mModel->predict(aImage, predictLabel, confidence);

	if (predictLabel >= 0)
	{
		if (confidence > 0.5)
		{
			return predictLabel;
		}
	}
	return -1;
}


