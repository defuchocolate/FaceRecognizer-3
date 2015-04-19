#include <iostream>

#include <signal.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

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
	std::cout << "Matthias stinkt!" << std::endl;

	// TODO:
	GPIOTrigger trigger(1); // TODO: choose correct pin
	//Camera camera;
	//Mailer mail;

	while(keepGoing)
	{
		if (trigger.waitForMs(1000))
		{
			std::cout << "got trigger!" << std::endl;
		}
		else
		{
			std::cout << "trigger timeout..." << std::endl;
		}
		//FaceRecognizer fr(camera.makeSnapshots());
		//if (fr)
		//{
		//	mail.send(fr.getName());
		//}

		std::cout << "going to next circuit..." << std::endl;
	}

	std::cout << "exiting..." << std::endl;
	return 0;
}
