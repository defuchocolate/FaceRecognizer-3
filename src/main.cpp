#include <iostream>

#include <signal.h>

#include <FaceRecognizer.hpp>
#include <GPIOTrigger.hpp>

static bool keepGoing = true;

void signalHandler(int signum)
{
	(void)signum;
	keepGoing = false;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	//Mailer mail;
	FaceDetector faceDetector(); // detect and locate face in a picture
	FaceRecognizer faceRecognizer("test123.yml");
	if (faceDetector && faceRecognizer)
	{
		//Camera camera(faceDetector, faceRecognizer);
		GPIOTrigger trigger(1); // TODO: choose correct pin

		while(keepGoing)
		{
			if (trigger.waitForMs(1000))
			{
				std::cout << "got trigger!" << std::endl;

				//FaceRecognizer fr(camera.makeSnapshots());
				//if (fr)
				//{
				//	mail.send(fr.getName());
				//}
			}
			else
			{
				std::cout << "trigger timeout..." << std::endl;
			}

			std::cout << "going to next circuit..." << std::endl;
		}
	}
	else
	{
		std::cout << "setting up FaceRecognizer failed!" << std::endl;
	}

	std::cout << "exiting..." << std::endl;
	return 0;
}
