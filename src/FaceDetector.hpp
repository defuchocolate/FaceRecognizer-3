#ifndef FACEDETECTOR_HPP
#define FACEDETECTOR_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>

/**
 * @brief Implementation of FaceWrapper class
 */
class FaceDetector final
{
	bool mIsValid;
	const std::string mPathToHaarCascade;
	cv::CascadeClassifier mHaarCascade;

	int mImageWidth;
	int mImageHeight;

	public:
        /**
         * @brief constructor
         *
         * @param aPathToHaarCascade    Path to the Haar Cascade parameter file
         * @param aImageWidth           Width of the face images
         * @param aImageHeight          Height of the face images
         *
         */
		FaceDetector(const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight);
		~FaceDetector();

        /**
         * @brief Detect face on a lager image
         *
         * @param aVideoFrame   OpenCV matrix containing the grabbed camera frame
         * @param aFoundFaces   Output: Vector of OpenCV matrices containing all found and cut out faces
         *
         * @return bool         true when face found
         */
		bool DetectFace(const cv::Mat& aVideoFrame, std::vector<cv::Mat>& aFoundFaces);

		explicit operator bool() const;
};

#endif
