#ifndef ICAPTUREDEVICE_H
#define ICAPTUREDEVICE_H

class ICaptureDevice
{
	public:
			virtual ~ICaptureDevice() {}

			virtual Mat snapshot() = 0;
};

#endif
