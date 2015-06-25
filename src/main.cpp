#include <iostream>

#include <unistd.h>
#include <signal.h>

#include <FaceWrapper.hpp>
#include <GPIOTrigger.hpp>

namespace {
    bool keepGoing = true;

    void signalHandler(int signum)
    {
        (void)signum;
        keepGoing = false;
    }
};

int main(int argc, char** argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	if (argc < 6)
	{
		std::cout << "usage: " << argv[0] << " -r </path/to/face_recognize_meta.xml> -c </path/to/haar_cascade> -w <face image width> -h <face image height> -d <device id>" << std::endl;
		std::cout << "\t -t -- Start training mode" << std::endl;
		std::cout << "\t -i -- Path to directory where to store the images" << std::endl;
		std::cout << "\t -r </path/to/face_recognize_meta.xml> -- Path to the XML with the face database." << std::endl;
		std::cout << "\t -c </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << std::endl;
		std::cout << "\t -w <face image width> -- The width of the trained face images." << std::endl;
		std::cout << "\t -h <face image height> -- The height of the trained face images." << std::endl;
		std::cout << "\t -d <device id> -- The webcam device id to grab frames from." << std::endl;
		return 1;
	}

	bool trainMode = false;
	bool faceRecognizeMetaFileSet = false;
	bool haarCascadeFileSet = false;
	bool imageDirectorySet = false;
	bool imageWidthSet = false;
	bool imageHeightSet = false;
	bool deviceIdSet = false;

	std::string pathToFaceRecognizeMetaFile;
	std::string pathToHaarCascadeFile;
	std::string pathToImageDirectory;
	int imageWidth = 92; // default value
	int imageHeight = 112; // default value
	int deviceId = 0;

	char c;
	while ((c = getopt (argc, argv, "tr:c:w:h:d:i:")) != -1)
	{
		switch (c)
		{
			case 't':
				trainMode = true;
			break;

			case 'r':
				pathToFaceRecognizeMetaFile = optarg;
				faceRecognizeMetaFileSet = true;
			break;

			case 'c':
				pathToHaarCascadeFile = optarg;
				haarCascadeFileSet = true;
			break;

			case 'w':
				imageWidth = atoi(optarg);
				imageWidthSet = true;
			break;

			case 'h':
				imageHeight = atoi(optarg);
				imageHeightSet = true;
			break;

			case 'd':
				deviceId = atoi(optarg);
				deviceIdSet = true;
			break;

			case 'i':
				pathToImageDirectory = optarg;
				imageDirectorySet = true;
			break;
		}
	}

	std::cout << "starting with device: " << deviceId << std::endl;

	FaceWrapper faceWrapper(pathToFaceRecognizeMetaFile, pathToHaarCascadeFile, pathToImageDirectory, imageWidth, imageHeight, deviceId, !trainMode);
	if (faceWrapper)
	{
		if (trainMode)
		{
			faceWrapper.StartTrainSession();
			// make images and train them into database
		}
		else
		{
			//GPIOTrigger trigger(1); // TODO: choose correct pin

			while(keepGoing)
			{
				/*if (trigger.waitForMs(1000))
				{
					std::cout << "got trigger!" << std::endl;

					//faceWrapper.StartProcess();
				}*/

				//sleep(10); // to be removed when trigger works
				std::cout << "press any key to start detection" << std::endl;
				std::cin.get(c);
				faceWrapper.StartProcess();
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
