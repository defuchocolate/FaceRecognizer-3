#ifndef FACEWRAPPER_HPP
#define FACEWRAPPER_HPP

#include <FaceRecognizer.hpp>
#include <FaceDetector.hpp>
#include <Camera.hpp>
#include <MailService.hpp>

#include <thread>
#include <mutex>
#include <queue>

class FaceWrapper
{
	bool mIsValid;
	FaceDetector mFaceDetector;
	FaceRecognizer mFaceRecognizer;
	std::string mPathToImageDirectory;
	Camera mCamera;
	MailService mMailService;

    bool mKeepThreadGoing;
	std::thread mGrabberThread;
	std::mutex mSnapshotBufferMutex;
	std::queue<cv::Mat> mBackBufferFrames;
	unsigned short mNumOfBackBufferFrames;

    void GrabberThread();
    int readCSV(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, std::vector<std::string>& names);
    void writeCSV(const std::string& aPathToCSV, const std::string& aPathToImage, int aLabel, const std::string& aName, bool aDelete = false);

	public:
		FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, const std::string& aPathToImageDirectory, int aImageWidth, int aImageHeight, int aCameraDeviceNo, bool aStartThread, const std::string& aEmailSenderName, const std::string& aEmailSenderAddress, const std::string& aEmailSMTPServer, const unsigned short aEmailSMTPPort, const std::string& aEmailSMTPUsername, const std::string& aEmailSMTPPassword, const std::vector<std::string>& aEmailReceiverAddresses);
        ~FaceWrapper();

		void StartProcess();
		void StartTrainSession();

		explicit operator bool() const;
};

#endif
