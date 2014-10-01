#ifndef AUTHENTICATIONMANAGER_HPP
#define AUTHENTICATIONMANAGER_HPP

#include "ASyncCommand.hpp"
#include <QObject>

namespace iris
{
	class AuthenticationManager : public ASyncCommand
	{
	private:
		Q_OBJECT
	public:
		AuthenticationManager();
		virtual ~AuthenticationManager();

	public slots:
		virtual void exec()=0;
		virtual bool checkUserHasPermissionsForCommand(QString user, QString commmand)=0;
		virtual bool loginUser(QString nick, QStringList params)=0;
	};
}

#endif // AUTHENTICATIONMANAGER_HPP
