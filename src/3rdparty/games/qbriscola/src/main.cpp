/*
* This file is part of QBriscola
* Copyright (C) 2008  Betti Sorbelli Francesco - Ciotti Roberto
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Betti Sorbelli Francesco <montoya83@gmail.com>
*                 Ciotti Roberto <ciotti.roberto@gmail.com>
*
*/

#include <QApplication>
#include <QTranslator>
#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>

#include "avvio.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#include <qtopiaapplication.h>

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,avvio)
QTOPIA_MAIN
#else

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QSplashScreen *splash = new QSplashScreen(QPixmap(":icons/avvio.png"));
	splash->show();

	// crea l'ambiente per l'utente
	QString home = QDir::homePath();

	QFile file(home + "/.qbriscola");
    
    // NOTE Nessuno ha una precedente versione di QBriscola :D
// 	if ((file.exists()) && (file.open(QIODevice::WriteOnly | QIODevice::Text))) {
// 		QString m = "Esiste una versione precedente del file di configurazione. Vuoi eliminarlo? ";
// 		m += "Se non lo elimini, non potrai salvare le impostazioni attuali.\n\n";
// 		m += "Scelta consigliata: cancella il file obsoleto.";
// 		int ret = QMessageBox::warning(splash, tr("Warning"), m, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
// 
// 		if (ret == QMessageBox::Yes)
// 			file.remove();
// 		else if (ret == QMessageBox::Cancel)
// 			return 0;
// 	}

	QDir dir;
	dir.mkdir(home + "/.qbriscola");
    
    QTranslator translator;
    QString translationFile = QLocale::languageToString(QLocale::system().language());
    translationFile = ":/translations/trans_" + translationFile + ".qm";
    translator.load(translationFile);
    app.installTranslator(&translator);

	if (!QSqlDatabase::drivers().contains("QSQLITE"))
		QMessageBox::critical(splash, QObject::tr("Error"), QObject::tr("You don't have the sqlite driver... however you can play, but the statistics will not be enabled."));

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(home + "/.qbriscola/database");
	if (db.open()) {
		QString queryString = "create table partita("
			"id numeric primary key,"
			"mod string," // 0 per vs_pc, 1 per vs_umano
			"nick1 string,"
			"nick2 string,"
			"punti1 numeric,"
			"punti2 numeric"
			")";

		QSqlQuery query(db);
		query.exec(queryString);
	}

	Avvio *avvio = new Avvio(db);
	avvio->show();

	splash->finish(avvio);
	delete splash;

	return app.exec();
}
