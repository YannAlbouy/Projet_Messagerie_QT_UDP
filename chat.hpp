#include <QObject>
#include <QUdpSocket>

class Chat : public QObject
{
	Q_OBJECT
	private:
    	    	QUdpSocket *maSocket;
    	    	QString msgRecu;
    	    	quint16 monPort;
    	    	int flag;
	public:
		explicit Chat(int, QObject *parent = 0);
		/*----Envoie un message en guise de test----*/
		void envoieMsg(QString, int);
		void setMsgRecu(QString msg);
		QString getMsgRecu();
		
		int getFlag();
		void setFlag(int);
		
	signals:
		/*----Mon signal pour refresh----*/
		void flagRecu();
	public slots:
		void readyRead();

    
};

