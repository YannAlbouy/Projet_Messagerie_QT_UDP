#include <QPushButton>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QScrollArea>
#include <QTextEdit>
#include <QKeyEvent>
#include "chat.hpp"

/*----Pour le bouton help----*/
#include <QMessageBox>

/*----Pour le bouton de sauvegarde----*/
#include <QTextStream>
#include <QFileDialog>

class MyWindow : public QWidget
{
	Q_OBJECT
	private:
		/*----Objet chat----*/
		Chat *client;
		
		/*----Pour la saisie du texte----*/
		QLineEdit *saisieMessage;
		QString monTexte;

		/*----Ce que je recois et qui est utile----*/
		char *motSecret;
		int leSecondPort;
		
		/*----Affichage des messages----*/
		QScrollArea *maScrollZone;
		QTextEdit *afficheTexte;
						
	public:
		MyWindow(int port1, int port2, char *leMot){
			/*----Récupération de mon port----*/
			QString s = QString::number(port1);
			
			/*----Parametres de ma window----*/
			this->setWindowTitle("Messagerie : "+s);
			this->resize(300,500);
			/*----Fin des parametres window----*/
										
			/*---------ZONE DE DEBUG---------*/
			qDebug()<<"Je recupere mon port 1 : "<<port1;
			qDebug()<<"Je recupere mon port 2 : "<<port2;
			qDebug() << "Mot secret recu :"<< leMot;
			qDebug() << "Taille Mot secret recu :"<< strlen(leMot);
			/*-------FIN ZONE DE DEBUG-------*/
			
			/*----Récuperation port 2 et mot secret----*/
			leSecondPort = port2;
			motSecret = leMot;
			
			/*----Chat----*/
			client = new Chat(port1);		
						
			/*----Ma scroll area----*/
			maScrollZone = new QScrollArea();
			maScrollZone->setWidgetResizable(true);
						
			/*----Montrer mon texte à l'ecran----*/
			afficheTexte = new QTextEdit(this);
			afficheTexte->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			afficheTexte->setFrameShadow(QFrame::Plain);
			afficheTexte->setAlignment(Qt::AlignBottom| Qt::AlignLeft);
						
			/*----Bouton d'interaction avec l'utilisateur----*/
			QPushButton* pushEnvoi = new QPushButton("Envoyer");
			pushEnvoi->setStyleSheet("background-color: green;");		
			
			/*----Bouton help et save----*/
			QPushButton* pushSave = new QPushButton();
			QPixmap pixSave("./Images/save.png");
			QIcon iconeSave(pixSave);
			pushSave->setIcon(iconeSave);
			
			QPushButton* pushHelp = new QPushButton();
			QPixmap pixHelp("./Images/help.png");
			QIcon iconeHelp(pixHelp);
			pushHelp->setIcon(iconeHelp);
			
			/*----Ma boite ou il y aura tout----*/
			QGridLayout *maGrille = new QGridLayout();
			maScrollZone->setWidget(afficheTexte);

			
			/*----Mise ne place des boutons----*/
			QHBoxLayout* myLayout = new QHBoxLayout();
			QHBoxLayout* myLayoutSaveHelp = new QHBoxLayout();
			
			/*----Ajout du bouton envoyer a mon widget----*/
			myLayout->addWidget(pushEnvoi);
			myLayoutSaveHelp->addWidget(pushSave);
			myLayoutSaveHelp->addWidget(pushHelp);
			
			/*----Zone de saisie du texte----*/
			saisieMessage = new QLineEdit;
			saisieMessage->setPlaceholderText("Votre message :");
			saisieMessage->setFocus();
			
			/*----Connect du bouton pour envoyer le message----*/
			maGrille->addLayout(myLayoutSaveHelp,0,0);
			maGrille->addWidget(maScrollZone,1,0);
			maGrille->addWidget(saisieMessage,2,0);
			maGrille->addLayout(myLayout,3,0);

			/*----------------------Action effectuer sur le bouton----------------------*/

			/*----Envoie du message à l'aide du bouton----*/
			connect(pushEnvoi,SIGNAL(clicked()),this,SLOT(envoieMsg()));
			connect(client,SIGNAL(flagRecu()),this,SLOT(reception()));	
						
			/*----Action des boutons save et help----*/
			connect(pushHelp, SIGNAL(clicked()), this, SLOT(helpCenter()));
			connect(pushSave, SIGNAL(clicked()), this, SLOT(saveChat()));
			
			this->setLayout(maGrille);
		}
		
		/*----Activer l'envoie de message avec le bouton entrer----*/
		void keyPressEvent( QKeyEvent* event)
		{
			if((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ) && saisieMessage->text() !="" )
			{
				envoieMsg();
			}
		}
		
	public slots:
		/*----Méthode pour envoyer un message----*/
		void envoieMsg()
		{
			/*----Envoie du message----*/
			QString s =saisieMessage->text();
			client->envoieMsg(s, this->leSecondPort);
			QString affich = "You > " + saisieMessage->text();
			saisieMessage->clear();
			afficheTexte->setTextColor(QColor("red") );
			afficheTexte->append(affich);
		}
		
		/*----Methode de réception des messages----*/
		void reception()
		{
			/*----Affichage sur l'interface du message reçu----*/
			QString s = "Reçu > " + client->getMsgRecu();
			afficheTexte->setTextColor(QColor("blue") );
			afficheTexte->append(s);

			/*----On regarde si le message contient le mot clé jeu----*/
			QString msgTest = client->getMsgRecu().toLower();
			qDebug()<<"Message en lower : "<<msgTest;
			char renvoie[strlen(motSecret)];
			/*----On regarde si on veut jouer----*/
			if(msgTest[0] == "j" && msgTest[1] == "e" && msgTest[2] == "u")
			{
				qDebug()<<"On vas jouer";
				/*----Parcours du mot caché*----*/
				for(int i = 0;i<strlen(motSecret);i++)
				{
					qDebug()<<"Affichage du mot caché : "<<motSecret[i];
					qDebug()<<"Affichage du mot saisie : "<<msgTest[i+4];
					if(tolower(motSecret[i]) == msgTest[i+4])
					{
						renvoie[i] = motSecret[i];
					}
					else{
						/*----Sinon on met des traits----*/
						renvoie[i] = '_';
					}
					qDebug()<<"Mot ressortie : " << renvoie[i];
				}
				client->envoieMsg(renvoie, this->leSecondPort);
			}
			
			client->setMsgRecu("");
		}
		
		/*----Méthode help du chat----*/
		void helpCenter()
		{
			/*----Création de la message box----*/
			QMessageBox* helpDialog = new QMessageBox();
			/*----Nom de la message box----*/
			helpDialog->setWindowTitle("Help center");
			/*----Taille de la message box----*/
			helpDialog->resize(200,100);
			/*----Texte de la message box----*/
			helpDialog->setText("Pour trouver le mot caché saisissez \n le mot jeu suivi du mot voulu.");
			/*----Affichage de la message box----*/
			helpDialog->show();
		}
		
		/*----Méthode pour save le chat----*/
		void saveChat()
		{
			/*----Fenetre pour choisir l'emplacement et le nom du fichier de sauvegarde----*/
			QString nomFic = QFileDialog::getSaveFileName(this, tr("Sauvegarde du chat"), "", tr("Sauvegarde chat (*.txt);;C++ File (*.cpp *.h)"));
			/*----Si l'utilisateur à saisie un nom pour le fichier----*/
			if (nomFic != "") 
			{
        			/*----Création d'un fichier avec le nom saisie----*/
        			QFile fichier(nomFic);
        			if (fichier.open(QIODevice::ReadWrite)) 
        			{
					/*----Ouverture et écriture dans le fichier, puis fermeture----*/
					QTextStream stream(&fichier);   
            				stream << afficheTexte->toPlainText();
            				fichier.flush();
            				fichier.close();
        			}
        			else {
            				/*----Si on arrive pas a sauvegarder informe l'utilisateur----*/
            				QMessageBox::critical(this, tr("Erreur"), tr("Impossible de sauvegarder le fichier"));
            				return;
        			}
    			}
		}	
};
