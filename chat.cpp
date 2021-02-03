#include "chat.hpp"

Chat::Chat(int monPort, QObject *parent) : QObject(parent)
{
	maSocket = new QUdpSocket(this);
	maSocket->bind(QHostAddress::LocalHost,monPort);
	connect(maSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

/*----Envoyer un message----*/
void Chat::envoieMsg(QString str, int monPortEnvoie)
{
	QByteArray donne;
	//donne = str.toUtf8().toBase64();
	donne.append(str);
	/*----La ou je vais envoyer----*/
	maSocket->writeDatagram(donne,QHostAddress::LocalHost,monPortEnvoie);
	qDebug() << "--------------------- ";
	qDebug()<<"Envoie msg au port :"<<monPortEnvoie;
}

void Chat::setMsgRecu(QString msg)
{
	this->msgRecu = msg;
}
QString Chat::getMsgRecu()
{
	return this->msgRecu;
}

int Chat::getFlag()
{
	return this->flag;
}
void Chat::setFlag(int i)
{
	this->flag = i;
}
/*----Recevoir un message----*/
void Chat::readyRead()
{
	/*----Nous dit quand on a reçu une donnée pour la lire----*/
	QByteArray donneRecu;
	/*----Modification de la taille pour s'assurer qu'on recoive bien tout----*/
	donneRecu.resize(maSocket->pendingDatagramSize());
	
	/*----Coordonnées de a qui on envoie----*/
	QHostAddress envoyeur;
	quint16 portEnvoyeur;
	//monPort = portEnvoyeur;
	/*----Permet de lire le paquet que l'on reçoit----*/
	
	maSocket->readDatagram(donneRecu.data(),donneRecu.size(),&envoyeur,&portEnvoyeur);
	
	/*----Debug pour voir de qui vient lemessage----*/
	
	qDebug() << "Adresse de l'envoyeur " <<envoyeur.toString();
	qDebug() << "Port de l'envoyeur : " <<portEnvoyeur;
	qDebug() << "Message " <<donneRecu;
	/*----Recuperation du message reçu----*/
	
	this->setMsgRecu(donneRecu);
	/*----Envoie du signal pour dire que mon message à bien ete recu et refresh interface----*/
	emit flagRecu();
}




