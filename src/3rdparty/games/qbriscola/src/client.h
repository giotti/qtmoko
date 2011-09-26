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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <QtNetwork>
#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>

#include "fine.h"
#include "avvio.h"
#include "statistiche.h"

#include "finestra.h"

extern QString tipoMazzo;

class Client : public QMainWindow, public Ui::Finestra
{
Q_OBJECT

public:
	Client(QSqlDatabase, QString, QString, int, QWidget *parent = 0);
	virtual ~Client();

	void sendMessage(QString);
	void personalizza();

	void setVerde(QLineEdit*);
	void setRosso(QLineEdit*);
	void setNormale(QLineEdit*);

	void tornaMenuIniziale();

	void settaNickServer(QString);
	void settaBriscola(QString);
	void settaCarta(QString);
	void settaNumCarte(QString);
	void settaTurno(QString);
	void settaCartaServer(QString);
	void pulisciTavolo(QString);
	void aggiornaPunti(QString);
	void ultimaMano(QString);
	void disabilitaCarte(QString);
	void stabilisciVincitore(QString);
	void attivaColorazione(QString);
	void aggiustaMazzo(QString);
	void serverAbbandonato(QString);

	void aggiornaDataBase(QString, QString, int, int);

protected:
	virtual void closeEvent(QCloseEvent *);

public slots:
	void receiveMessage();

	void cliccataCarta1();
	void cliccataCarta2();
	void cliccataCarta3();

	void mostraStatistiche();
	void esciPartita();
	
	void errore(QAbstractSocket::SocketError);

private:
	QString nick;
	QString ip;
	int porta;
	int posServer;

	QTcpSocket *socket;
	QBuffer *buffer;

	// vettore di puntatori a metodi
	void (Client::*metodo[14])(QString);

	QSqlDatabase db;
};

#endif
