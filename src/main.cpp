#include <iostream>

#include <signal.h>

#include <FaceWrapper.hpp>
#include <GPIOTrigger.hpp>

static bool keepGoing = true;

void signalHandler(int signum)
{
	(void)signum;
	keepGoing = false;
}

int main(int argc, char** argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	if (argc < 6)
	{
		std::cout << "usage: " << argv[0] << " </path/to/face_recognize_meta.xml> </path/to/haar_cascade> <face image width> <face image height> <device id>" << std::endl;
		std::cout << "\t </path/to/face_recognize_meta.xml> -- Path to the XML with the face database." << std::endl;
        std::cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << std::endl;
        std::cout << "\t <face image width> -- The width of the trained face images." << std::endl;
        std::cout << "\t <face image height> -- The height of the trained face images." << std::endl;
        std::cout << "\t <device id> -- The webcam device id to grab frames from." << std::endl;
		return 1;
	}

	std::string pathToFaceRecognizeMetaFile = argv[1];
	std::string pathToHaarCascadeFile = argv[2];
	int imageWidth = atoi(argv[3]);
	int imageHeight = atoi(argv[4]);
	int deviceId = atoi(argv[5]);

	FaceWrapper faceWrapper(pathToFaceRecognizeMetaFile, pathToHaarCascadeFile, imageWidth, imageHeight, deviceId);

	if (faceWrapper)
	{
		GPIOTrigger trigger(1); // TODO: choose correct pin

		while(keepGoing)
		{
			if (trigger.waitForMs(1000))
			{
				std::cout << "got trigger!" << std::endl;

				//faceWrapper.StartProcess();
			}
		}
	}
	else
	{
		std::cout << "setting up FaceWrapper failed!" << std::endl;
	}

	std::cout << "exiting..." << std::endl;
	return 0;
}
