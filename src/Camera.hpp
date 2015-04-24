#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <ICaptureDevice.hpp>

/**
 * @brief Implementation of CaptureDevice interfacer
 */
class Camera final : public ICaptureDevice
{
	bool mIsValid;
	const unsigned int mDeviceNumber;
	CvCapture *mCamera;

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

			explicit operator bool() const;
};

#endif
