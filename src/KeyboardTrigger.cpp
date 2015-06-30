#include <KeyboardTrigger.hpp>

#include <iostream>
#include <chrono>

KeyboardTrigger::KeyboardTrigger(const char aKey) :
	mIsValid(false),
	mKey(aKey),
	mCVNotified(false),
	mKeepThreadGoing(true),
	mListenThread(),
	mTriggerMtx(),
	mTriggerCond()
{
	mListenThread = std::thread(&KeyboardTrigger::ListenThread, this);
}

KeyboardTrigger::~KeyboardTrigger()
{
	mKeepThreadGoing = false;
	mListenThread.join();
}

void KeyboardTrigger::wait()
{
	// not implemented yet
}

bool KeyboardTrigger::waitForMs(unsigned int aMsec)
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

void KeyboardTrigger::ListenThread()
{
	std::cout << "entering " << __PRETTY_FUNCTION__ << std::endl;
	char c;
	while(mKeepThreadGoing)
	{
		std::cin.get(c);
		mCVNotified = true;
		mTriggerCond.notify_one();
	}

	std::cout << "leaving " << __PRETTY_FUNCTION__ << std::endl;
}

KeyboardTrigger::operator bool() const
{
	return mIsValid;
}
