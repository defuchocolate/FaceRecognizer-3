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

    bool separateCommaSeparatedString(std::vector<std::string>& aElements, const std::string& aCommaSeparatedString)
    {
        std::stringstream liness(aCommaSeparatedString);
        std::string elem;

        while(getline(liness, elem, ','))
        {
            // copy element to separated element vector:
            aElements.push_back(elem);
        }

        if (aElements.size() > 0)
        {
            return true;
        }
        return false;
    }
};

int main(int argc, char** argv)
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

    if (argc < 2)
	{
		std::cout << "usage: " << argv[0] << " -c </path/to/main.ini> [-t]" << std::endl;
        std::cout << "\t -c </path/to/main.ini> -- Path to the main configuration file." << std::endl;
		std::cout << "\t -t -- Start training mode" << std::endl;
		return 1;
	}

	bool trainMode = false;
	bool faceRecognizeMetaFileSet = false;
	bool haarCascadeFileSet = false;
	bool imageDirectorySet = false;
	bool imageWidthSet = false;
	bool imageHeightSet = false;

    bool pathToConfigSet = false;
    std::string pathToConfig;

	char c;
    while ((c = getopt (argc, argv, "tc:")) != -1)
	{
		switch (c)
		{
			case 't':
				trainMode = true;
			break;

            case 'c':
				pathToConfig = optarg;
                pathToConfigSet = true;
			break;
		}
	}

    // parse configuration file
    if (pathToConfigSet)
    {
        minIni iniReader(pathToConfig);
        // [gerneral]
        int deviceId = iniReader.geti("general", "devicenumber", 0);
        std::string pathToHaarCascadeFile = iniReader.gets("general", "haardcascade", "");
        if (pathToHaarCascadeFile.size() > 0)
        {
            haarCascadeFileSet = true;
        }
        std::string pathToFaceRecognizeMetaFile = iniReader.gets("general", "eigenface", "");
        if (pathToFaceRecognizeMetaFile.size() > 0)
        {
            faceRecognizeMetaFileSet = true;
        }

        // [images]
        std::string pathToImageDirectory = iniReader.gets("images", "trainingpath", "");
        if (pathToImageDirectory.size() > 0)
        {
            imageDirectorySet = true;
        }
        int imageWidth = iniReader.geti("images", "width", 0);
        if (imageWidth > 0)
        {
            imageWidthSet = true;
        }
        int imageHeight = iniReader.geti("images", "height", 0);
        if (imageHeight > 0)
        {
            imageHeightSet = true;
        }

        // [email]
        std::string emailSenderName = iniReader.gets("email", "sendername", "");
        std::string emailSenderAddress = iniReader.gets("email", "senderaddr", "");
        std::string emailSMTPServer = iniReader.gets("email", "smtpserver", "");
        unsigned short emailSMTPPort = iniReader.geti("email", "smtpport", 0);
        std::string emailSMTPUsername = iniReader.gets("email", "smtpusername", "");
        std::string emailSMTPPassword = iniReader.gets("email", "smtppassword", "");
        std::string emailReceiverAddresses = iniReader.gets("email", "receiveraddr", "");

        std::vector<std::string> receiverAddresses;
        if (!separateCommaSeparatedString(receiverAddresses, emailReceiverAddresses))
        {
            std::cerr << "failed to separate mail addresses: " << emailReceiverAddresses << std::endl;
            return 1;
        }

        if (!haarCascadeFileSet || !faceRecognizeMetaFileSet || !imageDirectorySet || !imageWidthSet || !imageHeightSet)
        {
            std::cerr << "configuration is incomplete!" << std::endl;
            return 1;
        }

        FaceWrapper faceWrapper(pathToFaceRecognizeMetaFile, pathToHaarCascadeFile, pathToImageDirectory, imageWidth, imageHeight, deviceId, !trainMode, emailSenderName, emailSenderAddress, emailSMTPServer, emailSMTPPort, emailSMTPUsername, emailSMTPPassword, receiverAddresses);
        if (faceWrapper)
        {
            std::cout << "starting with device number: " << deviceId << std::endl;

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
    }

	std::cout << "exiting..." << std::endl;
	return 0;
}
