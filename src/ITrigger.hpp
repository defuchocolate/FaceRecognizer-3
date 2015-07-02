#ifndef ITRIGGER_H
#define ITRIGGER_H

/**
 * @brief Definition of the Trigger interface
 */
class ITrigger
{
	public:
			virtual ~ITrigger() {}

			/**
			 * @brief waiting for specific event, calling this will block
			 */
			virtual void wait() = 0;

			/**
			 * @brief waiting for specific event, calling this will block for maximum aMsec milliseconds
			 *
			 * @param aMsec time in milliseconds to wait for trigger event
			 *
			 * @return bool true	-> Trigger event occured
			 * 				false	-> Timeout
			 */
			virtual bool waitForMs(unsigned int aMsec) = 0;
};

#endif
