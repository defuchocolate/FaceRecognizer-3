#ifndef GPIOTRIGGER_H
#define GPIOTRIGGER_H

#include <ITrigger.hpp>
#include <condition_variable>
#include <mutex>
#include <thread>

/**
 * @brief Implementation of Trigger interfacer
 */
class GPIOTrigger final : public ITrigger
{
	bool mIsValid;
	const unsigned int mPin;
	bool mCVNotified;
	bool mKeepThreadGoing;
	std::thread mListenThread;
	std::mutex mTriggerMtx;
	std::condition_variable mTriggerCond;

	GPIOTrigger(const GPIOTrigger&) = delete;
	const GPIOTrigger& operator=(const GPIOTrigger&) = delete;

	void ListenThread();

	public:
			/**
			 * @brief constructor
			 *
			 * @param aPin GPIO Pin number to listen on
			 *
			 */
			GPIOTrigger(unsigned int aPin);
			virtual ~GPIOTrigger();

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
