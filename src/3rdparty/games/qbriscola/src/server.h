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

#ifndef H_SERVER
#define H_SERVER

#include <QtNetwork>
#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>

#include "mazzo.h"
#include "thread.h"
#include "carta.h"
#include "giocatore.h"
#include "fine.h"
#include "avvio.h"
#include "statistiche.h"

#include "finestra.h"

extern QString tipoMazzo;

class Server : public QMainWindow, public Ui::Finestra
{
Q_OBJECT

public:
	Server(QSqlDatabase, QString, int, QWidget *parent = 0);

	// metodi per partita
	void iniziaPartita();
	int chiInizia();
	void controllaGiocata(int);
	int controllaGiocate();
	void attivaRaccolta(int);
	void gestisciUltimaMano();

	void sendMessage(QString);
	void personalizza();
	void inizializza();

	void settaNickClient(QString);
	void clientCarta(QString);
	void clientAbbandonato(QString);

	void settaBriscola();
	void settaNumCarte();
	void gestisciTurno(int);

	void setVerde(QLineEdit*);
	void setRosso(QLineEdit*);
	void setNormale(QLineEdit*);

	void tornaMenuIniziale();
	void settaIcone();

	void aggiornaDataBase(QString, QString, int, int);

private slots:
	void addConnection();
	void receiveMessage();

	void cliccataCarta1();
	void cliccataCarta2();
	void cliccataCarta3();

	void riceviSegnaleRaccoltaG1();
	void riceviSegnaleRaccoltaG2();

	void mostraStatistiche();
	void esciPartita();

protected:
	void closeEvent(QCloseEvent *);

private:
	QString nick;
	int porta;
	QTcpServer* server;
	QTcpSocket* socket;
	QBuffer* buffer;

	Giocatore *g1;
	Giocatore *g2;
	Carta *br;
	Mazzo *mazzo;
	Random *randomTurno;
	Random *randomCarta;
	int turno;
	int pos1;
	int pos2;
	int ultima;
	Thread *th;

	// vettore di puntatori a metodi
	void (Server::*metodo[3])(QString);

	QSqlDatabase db;
};

#endif
