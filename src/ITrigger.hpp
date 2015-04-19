#ifndef ITRIGGER_H
#define ITRIGGER_H

class ITrigger
{
	public:
			virtual ~ITrigger() {}

			virtual void wait() = 0;
			virtual bool waitForMs(unsigned int) = 0;
};

#endif
