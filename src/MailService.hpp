#ifndef MAILSERVICE_HPP
#define MAILSERVICE_HPP

#include <Poco/Net/MailMessage.h>

#include <string>

/**
 * @brief Implementation of MailService class
 */
class MailService final
{
	bool mIsValid;
	const std::string mUsername;
	const std::string mPassword;
	const std::string mServerAddress;
	const short mPort;
	Poco::Net::MailMessage mPocoMail;

	public:
        /**
         * @brief constructor
         *
         * @param aSenderFullName   Name of E-Mail Sender
         * @param aSenderAddress    Address of E-Mail Sender
         * @param aUsername         SMTP Server Login username
         * @param aPassword         SMTP Server Login password
         * @param aServerAddress    SMTP Server Address
         * @param aPort             SMTP Server Port
         *
         */
		MailService(const std::string& aSenderFullName, const std::string& aSenderAddress, const std::string& aUsername, const std::string& aPassword, const std::string& aServerAddress, const short aPort);
        ~MailService();

        /**
         * @brief Set the email address of an recipient
         *
         * @param aRecipientAddress Address of the recipient
         *
         */
        void SetRecipient(const std::string& aRecipientAddress);

        /**
         * @brief Send a mail with the given text
         *
         * @param aText The text to send
         *
         */
		bool SendMail(const std::string& aText);

		explicit operator bool() const;
};

#endif
