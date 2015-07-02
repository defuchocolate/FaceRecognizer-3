#ifndef FACEWRAPPER_HPP
#define FACEWRAPPER_HPP

#include <FaceRecognizer.hpp>
#include <FaceDetector.hpp>
#include <Camera.hpp>
#include <MailService.hpp>

#include <thread>
#include <mutex>
#include <queue>

/**
 * @brief Implementation of FaceWrapper class
 */
class FaceWrapper final
{
	bool mIsValid;
	FaceDetector mFaceDetector;
	FaceRecognizer mFaceRecognizer;
	std::string mPathToImageDirectory;
	Camera mCamera;

    bool mKeepThreadGoing;
	std::thread mGrabberThread;
	std::mutex mSnapshotBufferMutex;
	std::queue<cv::Mat> mBackBufferFrames;

	MailService mMailService;

    /**
     * @brief Thread loop to grab frames from camera device
     */
    void GrabberThread();

    /**
     * @brief Parse comma separated values from file
     *
     * @param aFilename Path to CSV file
     * @param aImages   Output: Vector of OpenCV matrices, is filled in the function with images read from CSV
     * @param aLabels   Output: Vector of labels (identifiers)
     * @param aNames    Output: Vector of names
     *
     * @return last identifier
     *
     */
    int readCSV(const std::string& aFilename, std::vector<cv::Mat>& aImages, std::vector<int>& aLabels, std::vector<std::string>& aNames);

    /**
     * @brief Write a line into comma separated value file
     *
     * @param aPathToCSV    Path to CSV file
     * @param aPathToImage  Path to the image associated with this line
     * @param aLabel        Identifier of this image
     * @param aName         Name associated with this image
     * @param aDelete       Should file be appended or deleted?
     *
     */
    void writeCSV(const std::string& aPathToCSV, const std::string& aPathToImage, int aLabel, const std::string& aName, bool aDelete = false);

	public:
        /**
         * @brief constructor
		 *
		 * @param aEigenFaceMetaFile    Path to file containing meta data about already trained faces
         * @param aPathToHaarCascade    Path to file containing haar cascade information
         * @param aPathToImageDirectory Path to directory where image files have to be stored
         * @param aImageWidth           Width of all images
		 * @param aImageHeight          Height of all images
         * @param aCameraDeviceNo       Number of the device to be used for frame capturing
         * @param aStartThread          Start capturing thread?
         * @param aEmailSenderName      Name of E-Mail Sender
         * @param aEmailSenderAddress   Address of E-Mail Sender
         * @param aEmailSMTPServer      SMTP Server Address
         * @param aEmailSMTPPort        SMTP Server Port
         * @param aEmailSMTPUsername    SMTP Server Login username
         * @param aEmailSMTPPassword    SMTP Server Login password
         * @param aEmailReceiverAddresses Vector of string containing receiver E-Mail addresses
		 */
		FaceWrapper(const std::string& aEigenFaceMetaFile, const std::string& aPathToHaarCascade, const std::string& aPathToImageDirectory, int aImageWidth, int aImageHeight, int aCameraDeviceNo, bool aStartThread, const std::string& aEmailSenderName, const std::string& aEmailSenderAddress, const std::string& aEmailSMTPServer, const unsigned short aEmailSMTPPort, const std::string& aEmailSMTPUsername, const std::string& aEmailSMTPPassword, const std::vector<std::string>& aEmailReceiverAddresses);
        ~FaceWrapper();

        /**
         * @brief Starts the face recognition procedure
         */
		void StartProcess();

        /**
         * @brief Starts the training session
         */
		void StartTrainSession();

		explicit operator bool() const;
};

#endif
