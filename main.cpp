#include <QApplication>
#include <QTime>
#include "mywindow.hpp"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	/*----Ajout d'un logo----*/
	app.setWindowIcon(QIcon("./Images/logo.png"));
	
	/*----Récupération des ports----*/
	int port1 = atoi(argv[1]);
	int port2 = atoi(argv[2]);
	
	/*----Récupération des mots secret----*/
	char *motSecret1 = argv[3];
	char *motSecret2 = argv[4];
		
	/*----Client / Serveur 1----*/
	MyWindow window(port1,port2, motSecret1);
	window.show();
	
	/*----Croiser port1 et port2----*/
	
	/*----Client / Serveur 2----*/
	MyWindow window2(port2,port1, motSecret2);	
	window2.show();
	
	return app.exec();	
}

