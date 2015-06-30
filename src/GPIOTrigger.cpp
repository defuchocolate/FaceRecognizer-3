#include <GPIOTrigger.hpp>

#include <iostream>
#include <chrono>

GPIOTrigger::GPIOTrigger(unsigned int aPin) :
	mIsValid(false),
	mPin(aPin),
	mCVNotified(false),
	mKeepThreadGoing(true),
	mListenThread(),
	mTriggerMtx(),
	mTriggerCond()
{
	mListenThread = std::thread(&GPIOTrigger::ListenThread, this);
}

GPIOTrigger::~GPIOTrigger()
{
	mKeepThreadGoing = false;
	mListenThread.join();
}

void GPIOTrigger::wait()
{
	// not implemented yet
}

bool GPIOTrigger::waitForMs(unsigned int aMsec)
{
	std::unique_lock<std::mutex> lock(mTriggerMtx);
	while (!mCVNotified)
	{
		if (std::cv_status::timeout == mTriggerCond.wait_for(lock, std::chrono::milliseconds(aMsec)))
		{
			return false;
		}
	}

	mCVNotified = false;
	return true;
}

void GPIOTrigger::ListenThread()
{
	std::cout << "entering " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
	unsigned char state = 0;
	while(mKeepThreadGoing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//gpioReadPin(mPin, state);
		if (state > 0)
		{
			mCVNotified = true;
			mTriggerCond.notify_one();
		}
	}

	std::cout << "leaving " << __PRETTY_FUNCTION__ << " main loop" << std::endl;
}

GPIOTrigger::operator bool() const
{
	return mIsValid;
}
