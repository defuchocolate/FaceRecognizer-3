#include <iostream>

#include <unistd.h>
#include <signal.h>

#include <thirdparty/minIni/minIni.h>
#include <FaceWrapper.hpp>

#if defined(RASPBERRY)
#include <GPIOTrigger.hpp>
#else
#include <KeyboardTrigger.hpp>
#endif

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

	char c;
	while ((c = getopt (argc, argv, "tr:c:w:h:d:i:")) != -1)
	{
		switch (c)
		{
			case 't':
				trainMode = true;
			break;

			/*case 'r':
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
			break;*/
		}
	}

	minIni iniReader("config/main.ini");
	int deviceId = iniReader.geti("general", "devicenumber", 0);
	std::string pathToHaarCascadeFile = iniReader.gets("general", "haardcascade", "");
	std::string pathToFaceRecognizeMetaFile = iniReader.gets("general", "eigenface", "");

	std::string pathToImageDirectory = iniReader.gets("images", "trainingpath", "");
	int imageWidth = iniReader.geti("images", "width", 0);
	int imageHeight = iniReader.geti("images", "height", 0);

	bool emailEnabled = iniReader.geti("email", "enable", 0);
	std::string emailSender = iniReader.gets("email", "sendername", "");
	std::string emailSenderAddress = iniReader.gets("email", "senderaddr", "");
	std::string emailSMTPServer = iniReader.gets("email", "smtpserver", "");
	unsigned short emailSMTPPort = iniReader.geti("email", "smtpport", 0);
	std::string emailSMTPUsername = iniReader.gets("email", "smtpusername", "");
	std::string emailSMTPPassword = iniReader.gets("email", "smtppassword", "");

	FaceWrapper faceWrapper(pathToFaceRecognizeMetaFile, pathToHaarCascadeFile, pathToImageDirectory, imageWidth, imageHeight, deviceId, !trainMode);
	if (faceWrapper)
	{
		std::cout << "starting with device: " << deviceId << std::endl;

		if (trainMode)
		{
			faceWrapper.StartTrainSession();
			// make images and train them into database
		}
		else
		{
			#if defined(RASPBERRY)
			GPIOTrigger trigger(1); // TODO: choose correct pin
			#else
			KeyboardTrigger trigger;
			#endif

			while(keepGoing)
			{
				if (trigger.waitForMs(1000))
				{
					if(keepGoing) // double check exit condition
					{
						std::cout << "got trigger event!" << std::endl;
						faceWrapper.StartProcess();
					}
				}
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
