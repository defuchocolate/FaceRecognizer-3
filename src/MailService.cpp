#include <iostream>
#include <MailService.hpp>

#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>

MailService::MailService(const std::string& aSenderFullName, const std::string& aSenderAddress, const std::string& aUsername, const std::string& aPassword, const std::string& aServerAddress, const short aPort) :
	mIsValid(false),
	mUsername(aUsername),
	mPassword(aPassword),
	mServerAddress(aServerAddress),
	mPort(aPort),
	mPocoMail()
{
	std::string sender = aSenderFullName + "<" + aSenderAddress + ">";
	mPocoMail.setSender(sender);
	mPocoMail.setSubject("TESTMAIL FACERECOGNIZER");
	mIsValid = true;
}

MailService::~MailService()
{

}

void MailService::SetRecipient(const std::string& aRecipientAddress)
{
	mPocoMail.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, aRecipientAddress));
}

bool MailService::SendMail(const std::string& aText)
{
	mPocoMail.setContent(aText);

	try
	{
        Poco::Net::SMTPClientSession smtp(mServerAddress, mPort);
        smtp.open();
        try
        {
            smtp.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, mUsername, mPassword);
            smtp.sendMessage(mPocoMail);
            std::cout << "Message successfully sent" << std::endl;
            smtp.close();
        }
        catch(Poco::Net::SMTPException &e)
        {
            std::cerr << e.displayText() << std::endl;
            smtp.close();
            return false;
        }
    }
    catch(Poco::Net::NetException &e)
    {
        std::cerr << e.displayText() << std::endl;
        return false;
    }

	return true;
}

