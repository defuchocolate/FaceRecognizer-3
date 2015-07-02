#include <FaceWrapper.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace
{
	const unsigned short SIZE_OF_BACKBUFFER = 10;
}

static cv::Mat norm_0_255(cv::InputArray _src) {
    cv::Mat src = _src.getMat();
    // Create and return normalized image:
    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

FaceWrapper::FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, const std::string& aPathToImageDirectory, int aImageWidth, int aImageHeight, int aCameraDeviceNo, bool aStartThread, const std::string& aEmailSenderName, const std::string& aEmailSenderAddress, const std::string& aEmailSMTPServer, const unsigned short aEmailSMTPPort, const std::string& aEmailSMTPUsername, const std::string& aEmailSMTPPassword, const std::vector<std::string>& aEmailReceiverAddresses) :
	mIsValid(false),
	mFaceDetector(aPathToHaarCascade, aImageWidth, aImageHeight),
	mFaceRecognizer(aEigenFaceMetaFile),
	mPathToImageDirectory(aPathToImageDirectory),
	mCamera(aCameraDeviceNo),
    mKeepThreadGoing(false),
	mGrabberThread(),
	mSnapshotBufferMutex(),
	mBackBufferFrames(),
	mMailService(aEmailSenderName, aEmailSenderAddress, aEmailSMTPUsername, aEmailSMTPPassword, aEmailSMTPServer, aEmailSMTPPort)
{
    for (const std::string& address : aEmailReceiverAddresses)
    {
        mMailService.SetRecipient(address);
    }

	if (mFaceDetector && mFaceRecognizer && mCamera)
	{
		mIsValid = true;
		if (aStartThread)
		{
            mKeepThreadGoing = true;
			mGrabberThread = std::thread(&FaceWrapper::GrabberThread, this);
		}
	}
}

FaceWrapper::~FaceWrapper()
{
    if (mKeepThreadGoing)
    {
		mKeepThreadGoing = false;
		mGrabberThread.join();
	}
}

FaceWrapper::operator bool() const
{
	return mIsValid;
}

void FaceWrapper::GrabberThread()
{
	std::cout << "entering " << __PRETTY_FUNCTION__ << " main loop" << std::endl;

#if defined(DEBUG)
	cv::namedWindow("Snapshot", cv::WINDOW_AUTOSIZE );// Create a window for display.
#endif

	while (mKeepThreadGoing)
	{
		// lock mutex for race condition protection:
		mSnapshotBufferMutex.lock();
		mBackBufferFrames.push(mCamera.snapshot());

		if (mBackBufferFrames.size() == SIZE_OF_BACKBUFFER)
		{
			mBackBufferFrames.pop();
		}
		mSnapshotBufferMutex.unlock();

#if defined(DEBUG)
		cv::imshow("Snapshot", mBackBufferFrames.back());
		cv::waitKey(200);
#endif

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

#if defined(DEBUG)
	cv::destroyWindow("Snapshot");
#endif

	std::cout << "leaving " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
}

/**
 * @brief this function is called by external button press
 */
void FaceWrapper::StartProcess()
{
	std::vector<cv::Mat> detectedFaces;
	std::vector<std::string> detectedNames;

	// lock snapshot backbuffer
	mSnapshotBufferMutex.lock();
	if (mBackBufferFrames.size() > 0)
	{
		// iterate over all backbuffer frames and try to detect a face:
		//for (unsigned short i = 0; i < mBackBufferFrames.size(); i++)
		while (!mBackBufferFrames.empty())
		{
			if (mFaceDetector.DetectFace(mBackBufferFrames.front(), detectedFaces))
			{
				std::cout << "detect " << detectedFaces.size() << " faces on picture" << std::endl;
				for (auto face : detectedFaces)
				{
					//cv::namedWindow("DetectedFace", cv::WINDOW_AUTOSIZE );// Create a window for display.
					//cv::imshow("DetectedFace", face);
					//cv::waitKey(200);
					//cv::destroyWindow("DetectedFace");

					int faceIdentifier = mFaceRecognizer.FindIdentifierForFace(norm_0_255(face));
					const std::string faceName = mFaceRecognizer.GetNameOfId(faceIdentifier);
					if (0 <= faceIdentifier)
					{
						detectedNames.push_back(faceName);
						std::cout << "found person: " << faceName << "(" << faceIdentifier << ")" << std::endl;
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

			mBackBufferFrames.pop();
			detectedFaces.clear();
		}
	}
	mSnapshotBufferMutex.unlock();

	if (detectedNames.size() > 0)
	{
		std::stringstream sstream;
		sstream << "Hallo\r\n\r\n";
		for (std::string& name : detectedNames)
		{
			sstream << name << "\r\n";
		}
		sstream << "\r\nhat geklingelt!\r\n";
		mMailService.SendMail(sstream.str());
	}

}

int FaceWrapper::readCSV(const std::string& aFilename, std::vector<cv::Mat>& aImages, std::vector<int>& aLabels, std::vector<std::string>& aNames)
{
    std::ifstream file(filename.c_str(), std::ifstream::in);
    int latestId = 0;

    if (!file)
    {
        return 0;
    }

    std::string line, path, classlabel, name;
    while (getline(file, line))
    {
        std::stringstream liness(line);
        getline(liness, path, ';');
        getline(liness, classlabel, ';');
        getline(liness, name);
        if(!path.empty() && !classlabel.empty() && !path.empty())
        {
			int id = atoi(classlabel.c_str());
			if (latestId < id)
			{
				latestId = id;
			}

            images.push_back(cv::imread(path, 0));
            labels.push_back(id);
            names.push_back(name);
        }
    }

    return latestId;
}

void FaceWrapper::writeCSV(const std::string& aPathToCSV, const std::string& aPathToImage, int aLabel, const std::string& aName, bool aDelete)
{
	std::ofstream file(aPathToCSV.c_str(), (aDelete ? std::ofstream::out : std::ofstream::app));

	file << aPathToImage << ";" << aLabel << ";" << aName << std::endl;
}

void FaceWrapper::StartTrainSession()
{
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	std::vector<std::string> names;

	std::string currName;
	std::string pathToCSV = mPathToImageDirectory + "/images.csv";

	// load csv file
	unsigned int lastId = readCSV(pathToCSV, images, labels, names);
	// set lastId
	unsigned int currId = lastId + 1;
	//
	std::cout << "making 10 snapshots for importing..." << std::endl;

	cv::Mat frame;

	char tmpName[300];
	std::cout << "Enter name of the person you want to train: ";
	std::cin.getline(tmpName, sizeof(tmpName));
	currName = tmpName;

	int picturesMade = 0;
	while (picturesMade < 10)
	{
		std::string pathToCurrImage = mPathToImageDirectory + "/" + std::to_string(currId) + "_" + std::to_string(picturesMade) + ".png";
		// make a shot
		std::cout << (10 - picturesMade) << " images left..." << std::endl;
		frame = mCamera.snapshot();
		cv::namedWindow("DetectedFace", cv::WINDOW_AUTOSIZE );// Create a window for display.
		cv::imshow("DetectedFace", frame);
		std::cout << "use this picture for training? press Y or N!" << std::endl;
		char c = cv::waitKey(0);
		cv::destroyWindow("DetectedFace");

		// ask user whether he wants to use this shot
		if (c == 'Y' || c == 'y')
		{
			std::vector<cv::Mat> detectedFaces;

			// try to detect faces on the shot
			if (mFaceDetector.DetectFace(frame, detectedFaces))
			{
				std::cout << "detect " << detectedFaces.size() << " faces on picture" << std::endl;

				// ask user for each face whether he wants to use it for training
				for (auto face : detectedFaces)
				{
					cv::namedWindow("DetectedFace", cv::WINDOW_AUTOSIZE );// Create a window for display.
					cv::imshow("DetectedFace", face);
					std::cout << "use this face for training? press Y or N!" << std::endl;
					char c = cv::waitKey(0);
					cv::destroyWindow("DetectedFace");

					if (c == 'Y' || c == 'y')
					{
						// write data on disk:
						imwrite(pathToCurrImage, norm_0_255(face));
						writeCSV(pathToCSV, pathToCurrImage, currId, currName);

						// store in memory for subsequent training:
						images.push_back(norm_0_255(face));
						labels.push_back(currId);
						names.push_back(currName);

						picturesMade++;
					}
				}
			}
			else
			{
				std::cout << "no faces detected on snapshot!" << std::endl;
			}
		}

		frame.release();
	}

	std::cout << "start training..." << std::endl;
	mFaceRecognizer.Train(images, labels, names);
}
