#ifndef FACERECOGNIZER_HPP
#define FACERECOGNIZER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

/**
 * @brief Implementation of FaceRecognizer class
 */
class FaceRecognizer
{
	bool mIsValid;
	const std::string mEigenFaceMetaFile;
	cv::Ptr<cv::FaceRecognizer> mModel;
	unsigned int mLastId;

	public:
        /**
         * @brief constructor
         *
         * @param aEigenFaceMetaFile    Path to the already trained meta data file
         *
         */
		FaceRecognizer(const std::string& aEigenFaceMetaFile);
		~FaceRecognizer();

		explicit operator bool() const;

        /**
         * @brief Trains a number of faces into the system
         *
         * @param aImageMatrices    Vector of OpenCV matrices containing the faces
         * @param aIdentifiers      Vector of identifiers (labes) of the associated faces
         * @param aNames            Vector names of the associated faces
         *
         * @return bool             Returns true if training and saving the meta data was successful
         *
         */
		bool Train(const std::vector<cv::Mat>& aImageMatrices, const std::vector<int>& aIdentifiers, const std::vector<std::string>& aNames);

        /**
         * @brief Tries to find the identifier (label) for the given face
         *
         * @param aImage    OpenCV matrix containing the face
         *
         * @return int      Either the identifier on success or -1 on failure
         *
         */
		int FindIdentifierForFace(cv::Mat& aImage);

        /**
         * @brief Get the name of an given identifier
         *
         * @param aIdentifier   The identifier
         *
         * @return std::string  The name
         *
         */
		std::string GetNameOfId(const int aIdentifier);
};

#endif
