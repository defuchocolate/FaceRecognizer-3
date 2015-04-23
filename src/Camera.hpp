#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <ICaptureDevice.hpp>

/**
 * @brief Implementation of CaptureDevice interfacer
 */
class Camera final : public ICaptureDevice
{
	bool mIsValid;
	const unsigned int mDeviceNumber;
	const CvCapture *mCamera;
	
	Camera(const Camera&) = delete;
	const Camera& operator=(const Camera&) = delete;

	public:
			/**
			 * @brief constructor
			 *
			 * @param aDeviceNumber camera device number
			 *
			 */
			Camera(unsigned int aDeviceNumber=0);
			virtual ~Camera();

			/**
			 * @brief get opencv matrix from camera snapshot
			 *
			 * @return Mat opencv matrix
			 */
			virtual cv::Mat snapshot();
};

#endif
