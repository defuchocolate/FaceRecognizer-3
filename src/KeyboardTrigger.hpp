#ifndef KEYBOARDTRIGGER_H
#define KEYBOARDTRIGGER_H

#include <ITrigger.hpp>
#include <condition_variable>
#include <mutex>
#include <thread>

/**
 * @brief Implementation of Trigger interface
 */
class KeyboardTrigger final : public ITrigger
{
	bool mIsValid;
	const char mKey;
	bool mCVNotified;
	bool mKeepThreadGoing;
	std::thread mListenThread;
	std::mutex mTriggerMtx;
	std::condition_variable mTriggerCond;

	KeyboardTrigger(const KeyboardTrigger&) = delete;
	const KeyboardTrigger& operator=(const KeyboardTrigger&) = delete;

	void ListenThread();

	public:
			/**
			 * @brief constructor
			 *
			 * @param aPin GPIO Pin number to listen on
			 *
			 */
			KeyboardTrigger(const char aKey = 0);
			virtual ~KeyboardTrigger();

			/**
			 * @brief waiting for implemented event, calling this will block
			 */
			virtual void wait();

			/**
			 * @brief waiting for implemented event, calling this will block for maximum aMsec milliseconds
			 *
			 * @param aMsec time in milliseconds to wait for trigger event
			 *
			 * @return bool true	-> Trigger event occured
			 * 				false	-> Timeout
			 */
			virtual bool waitForMs(unsigned int aMsec);

			/**
			 * @brief bool operator for RAII check
			 */
			explicit operator bool() const;
};

#endif
