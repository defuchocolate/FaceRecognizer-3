#ifndef MAILSERVICE_HPP
#define MAILSERVICE_HPP

#include <Poco/Net/MailMessage.h>

#include <string>

class MailService
{
	bool mIsValid;
	const std::string mUsername;
	const std::string mPassword;
	const std::string mServerAddress;
	const short mPort;
	Poco::Net::MailMessage mPocoMail;

	public:
		MailService(const std::string& aSenderFullName, const std::string& aSenderAddress, const std::string& aUsername, const std::string& aPassword, const std::string& aServerAddress, const short aPort);
        ~MailService();

        void SetRecipient(const std::string& aRecipientAddress, const std::string& aRecipientFullName);
		bool SendMail(const std::string& aText);

		explicit operator bool() const;
};

#endif
