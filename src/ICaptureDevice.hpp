#ifndef ICAPTUREDEVICE_HPP
#define ICAPTUREDEVICE_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * @brief Definition of the CaptureDevice interface
 */
class ICaptureDevice
{
	public:
			virtual ~ICaptureDevice() {}

            /**
			 * @brief get frame as opencv matrix from camera
			 *
			 * @return Mat opencv matrix
			 */
			virtual cv::Mat snapshot() = 0;
};

#endif
