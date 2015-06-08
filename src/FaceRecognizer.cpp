#include <FaceRecognizer.hpp>
#include <iostream>
#include <string>

static int handleError(int, const char*, const char*, const char*, int, void*)
{
    return 0;
}

FaceRecognizer::FaceRecognizer(const std::string& aEigenFaceMetaFile) :
	mIsValid(true),
	mEigenFaceMetaFile(aEigenFaceMetaFile),
	mModel(cv::createEigenFaceRecognizer()), // createLBPHFaceRecognizer(), createFisherFaceRecognizer()
	mLastId(0)
{
	cv::redirectError(handleError);
	try
	{
		mModel->load(mEigenFaceMetaFile);
	}
	catch(cv::Exception& e)
	{
		//std::cerr << "Instantiation of FaceRecognizer failed:" << e.code << " " << e.err << std::endl;
        // when loading meta data failed, this only means that the file does not exist
		//mIsValid = false;
	}
}

FaceRecognizer::~FaceRecognizer()
{
}

FaceRecognizer::operator bool() const
{
	return mIsValid;
}

bool FaceRecognizer::Train(const std::vector<cv::Mat>& aImageMatrices, const std::vector<int>& aIdentifiers, const std::vector<std::string>& aNames)
{
	std::map<int, std::string> labelsInfo;

	if (aIdentifiers.size() != aNames.size())
	{
		return false;
	}
	unsigned int size = aIdentifiers.size();

	for (unsigned int i = 0; i < size; i++)
	{
		labelsInfo.insert(std::pair<int, std::string>(aIdentifiers[i], aNames[i]));
	}

	mModel->train(aImageMatrices, aIdentifiers);
	mModel->setLabelsInfo(labelsInfo);

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
	double confidence = 0.5;
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

std::string FaceRecognizer::GetNameOfId(int aIdentifier)
{
	return mModel->getLabelInfo(aIdentifier);
}
