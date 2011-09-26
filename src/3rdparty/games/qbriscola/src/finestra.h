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

#ifndef FINESTRA_H
#define FINESTRA_H

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "finestra.h"

#include "mazzo.h"
#include "giocatore.h"
#include "random.h"
#include "avvio.h"
#include "thread.h"
#include "fine.h"
#include "statistiche.h"
#include "configura.h"

extern QString tipoMazzo;

class Finestra : public QMainWindow, public Ui::Finestra
{
Q_OBJECT

public:
	Finestra(QSqlDatabase, QString, QWidget *parent = 0);

	// metodi che sono comuni
	void iniziaPartita();
	void settaBriscola();
	void settaNumCarte();
	int chiInizia();
	void gestisciTurno(int);
	void controllaGiocata(int);
	int controllaGiocate();
	void gestisciUltimaMano();

	// metodi che NON sono comuni
	void giocaComputer();
	void rispostaComputer();
	void attivaRaccolta(int);
	void settaIcone();
	void giocataComputer(int);
	bool isBriscola();
	int cercaMaxCartaStessoSeme();
	int cercaMinBriscola();
	int cercaMinPriorita();
	int cercaMaxPriorita();
	int cercaMaxBriscola();
	int cercaCartaBuona();
	int cercaCartaLibera();
	int cercaCartaSemiLibera();
	int cercaCaricoLibero();
	int contaBriscole();
	int ammazzaBriscola();
	int cercaBriscolaConsecutive(int);
	
	void inizializzo();
	
	void setVerde(QLineEdit*);
	void setRosso(QLineEdit*);
	void setNormale(QLineEdit*);

	void tornaMenuIniziale();
	void aggiornaDataBase(QString, QString, int, int);

public slots:
	void cliccataCarta1();
	void cliccataCarta2();
	void cliccataCarta3();
	void esciPartita();
	void about();
	void riceviSegnaleRaccoltaG1();
	void riceviSegnaleRaccoltaG2();
	void mostraStatistiche();
	void ricordaCarteGiocate();
	void avviaConfigura();
	void aggiornaThread(int);

protected:
	void closeEvent(QCloseEvent *);

private:
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
	QString nick;
	Thread *th;

	QSqlDatabase db;
	QVector<Carta*> uscite;
	QVector<Carta*> brUscite;
	
	Configura *conf;
};

#endif
