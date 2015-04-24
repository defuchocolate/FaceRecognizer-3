#ifndef ICAPTUREDEVICE_HPP
#define ICAPTUREDEVICE_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

class ICaptureDevice
{
	public:
			virtual ~ICaptureDevice() {}

			virtual cv::Mat snapshot() = 0;
};

#endif
