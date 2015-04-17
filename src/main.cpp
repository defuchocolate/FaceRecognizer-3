#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

static bool keepGoing = true;

void signalHandler(int signum)
{
	keepGoing = false;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Matthias stinkt!" << std::endl;

	// TODO:
	Trigger trigger;
	Camera camera;
	Mailer mail;

	while(keepGoing)
	{
		trigger.wait();
		FaceRecognizer fr(camera.makeSnapshots());
		if (fr)
		{
			mail.send(fr.getName());
		}
	}

	return 1;
}
