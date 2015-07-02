#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <ICaptureDevice.hpp>

/**
 * @brief Implementation of CaptureDevice interface
 */
class Camera final : public ICaptureDevice
{
	bool mIsValid;
	const unsigned int mDeviceNumber;
	cv::VideoCapture mCamera;
	cv::Mat currentFrame;

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
			 * @brief get frame as opencv matrix from camera
			 *
			 * @return Mat opencv matrix
			 */
			virtual cv::Mat snapshot();

			explicit operator bool() const;
};

#endif
