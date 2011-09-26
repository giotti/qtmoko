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

#include "server.h"

Server::Server(QSqlDatabase _db, QString _nick, int _porta, QWidget *parent) :
	QMainWindow(parent)
{
	setupUi(this);
	db = _db;
	nick = _nick;
	porta = _porta;

	personalizza();

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width() - width()) / 2, (dim.height() - height()) / 2, width(), height());

	th = new Thread();
	th->setSleepMs(1700);

	metodo[0] = &Server::settaNickClient;
	metodo[1] = &Server::clientCarta;
	metodo[2] = &Server::clientAbbandonato;

	server = new QTcpServer();
	server->listen(QHostAddress::Any, porta);

	socket = new QTcpSocket(this);
	buffer = new QBuffer(this);
	buffer->open(QIODevice::ReadWrite);

	connect(server, SIGNAL(newConnection()), this, SLOT(addConnection()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(actionStatistiche, SIGNAL(triggered()), this, SLOT(mostraStatistiche()));
	connect(th, SIGNAL(segnaleRaccoltaG1()), this, SLOT(riceviSegnaleRaccoltaG1()), Qt::QueuedConnection);
	connect(th, SIGNAL(segnaleRaccoltaG2()), this, SLOT(riceviSegnaleRaccoltaG2()), Qt::QueuedConnection);

	connect(pushButtonG2C1, SIGNAL(clicked()), this, SLOT(cliccataCarta1()));
	connect(pushButtonG2C2, SIGNAL(clicked()), this, SLOT(cliccataCarta2()));
	connect(pushButtonG2C3, SIGNAL(clicked()), this, SLOT(cliccataCarta3()));
}

void Server::settaBriscola()
{
	br = mazzo->daiCarta();

	// sistema la briscola in locale
	QString tmp;
	tmp.setNum(br->getNumero());
	QString briscola = ":mazzi/" + tipoMazzo + "/" + tmp + br->getSeme() + ".png";
	labelBr->setPixmap(briscola);

	setStatusTip("Briscola: " + br->getSeme());

	// invia la briscola al client
	QString numero;
	numero.setNum(br->getNumero());
	sendMessage("01" + numero + br->getSeme() + "|");
}

void Server::personalizza()
{
	groupBoxG2->setTitle(nick);
	lineEditPuntiG1->setText("/");
	lineEditPuntiG2->setText("0");
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);
}

void Server::addConnection()
{
	socket = server->nextPendingConnection();
	buffer = new QBuffer(this);
	buffer->open(QIODevice::ReadWrite);

	sendMessage("00" + nick + "|");

	connect(socket, SIGNAL(readyRead()), SLOT(receiveMessage()));

	iniziaPartita();
}

void Server::iniziaPartita()
{
	// creazione del mazzo
	mazzo = new Mazzo();

	randomTurno = new Random(time(0));

	ultima = 0;
	turno = 0;
	pos1 = -1;
	pos2 = -1;

	// crea i 2 giocatori
	g1 = new Giocatore(mazzo->daiCarta(), mazzo->daiCarta(), mazzo->daiCarta());
	g2 = new Giocatore(mazzo->daiCarta(), mazzo->daiCarta(), mazzo->daiCarta());

	// setta la briscola
	settaBriscola();
	QString tmp, num;

	tmp.setNum(g2->getCarta(0)->getNumero());
	QString icona20 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(0)->getSeme() + ".png";
	pushButtonG2C1->setIcon(QIcon(icona20));
	num.setNum(g1->getCarta(0)->getNumero());
	sendMessage("020" + num + g1->getCarta(0)->getSeme() + "|");

	tmp.setNum(g2->getCarta(1)->getNumero());
	QString icona21 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(1)->getSeme() + ".png";
	pushButtonG2C2->setIcon(QIcon(icona21));
	num.setNum(g1->getCarta(1)->getNumero());
	sendMessage("021" + num + g1->getCarta(1)->getSeme() + "|");

	tmp.setNum(g2->getCarta(2)->getNumero());
	QString icona22 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(2)->getSeme() + ".png";
	pushButtonG2C3->setIcon(QIcon(icona22));
	num.setNum(g1->getCarta(2)->getNumero());
	sendMessage("022" + num + g1->getCarta(2)->getSeme() + "|");

	settaNumCarte();
	turno = chiInizia();
	gestisciTurno(turno);
}

int Server::chiInizia()
{
	int r = static_cast<int> (randomTurno->rand() * (2));

	if (r > 0.5)
		return 2;
	else
		return 1;
}

void Server::gestisciTurno(int g)
{
	if (g == 2) {
		pushButtonG2C1->setEnabled(true);
		pushButtonG2C2->setEnabled(true);
		pushButtonG2C3->setEnabled(true);
	} else {
		sendMessage("04Turno|");
	}
}

void Server::sendMessage(QString message)
{
	socket->write(message.toLatin1());
}

void Server::receiveMessage()
{
	qint64 bytes = buffer->write(socket->readAll());
	buffer->seek(buffer->pos() - bytes);
	QString message = buffer->readAll();
	QStringList list;
	list = message.split("|");

	for (QList<QString>::iterator i = list.begin(); i != list.end(); i++) {
		QString split = *i;
		if (split != "") {
			int codice = split.mid(0, 2).toInt();
			QString contenuto = split.mid(2);
			(*this.*metodo[codice])(contenuto);
		}
	}
}

void Server::clientAbbandonato(QString)
{
	QMessageBox::critical(this, tr("Information"), tr("Client has been exited from match"));
	tornaMenuIniziale();
}

void Server::settaNickClient(QString nickClient)
{
	groupBoxG1->setTitle(nickClient);
	setWindowTitle("QBriscola - " + nick + " vs " + nickClient);
}

void Server::clientCarta(QString c)
{
	int carta = c.toInt();

	g1->giocaCarta(carta);
	pos1 = carta;

	if (carta == 0) {
		pushButtonG1C1->setIcon(QIcon(":/icons/null.png"));
	} else if (carta == 1) {
		pushButtonG1C2->setIcon(QIcon(":/icons/null.png"));
	} else if (carta == 2) {
		pushButtonG1C3->setIcon(QIcon(":/icons/null.png"));
	}

	QString tmp1;
	tmp1.setNum(g1->getGiocata()->getNumero());
	QString carta1 = ":mazzi/" + tipoMazzo + "/" + tmp1 + g1->getGiocata()->getSeme() + ".png";
	labelG1->setPixmap(carta1);

	controllaGiocata(1);
}

void Server::settaNumCarte()
{
	QString num;
	num.setNum(mazzo->getMax() + 1);
	lineEditCar->setText(num);

	if (num == "2")
		setRosso(lineEditCar);

	sendMessage("03" + num + "|");
}

void Server::attivaRaccolta(int g)
{
	if (g == 1) {
		th->setGiocatore(1);
		setVerde(lineEditPuntiG1);
		sendMessage("111|");
		th->start();
	} else if (g == 2) {
		th->setGiocatore(2);
		setVerde(lineEditPuntiG2);
		sendMessage("112|");
		th->start();
	}
}

void Server::controllaGiocata(int i)
{
	if (i == 2) {
		if (g1->getGiocata() == NULL) {
			gestisciTurno(1);
		} else {
			int p = controllaGiocate();
			attivaRaccolta(p);
		}
	} else {
		if (g2->getGiocata() == NULL) {
			gestisciTurno(2);
		} else {
			int p = controllaGiocate();
			attivaRaccolta(p);
		}
	}
}

int Server::controllaGiocate()
{
	if (g1->getGiocata()->getSeme() == g2->getGiocata()->getSeme()) {
		if (g1->getGiocata()->getPriorita() > g2->getGiocata()->getPriorita()) {
			return 1;
		} else {
			return 2;
		}
	} else {
		if (g1->getGiocata()->getSeme() == br->getSeme()) {
			return 1;
		} else if (g2->getGiocata()->getSeme() == br->getSeme()) {
			return 2;
		} else {
			return turno;
		}
	}
}

void Server::setVerde(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(147, 255, 101));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Server::setRosso(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 0, 0));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Server::setNormale(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 255, 255));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Server::aggiornaDataBase(QString n1, QString n2, int p1, int p2)
{
	QString home = QDir::homePath();
	if (db.open()) {
		QString queryString = "select count(*) from partita";
		QSqlQuery querySql(queryString);
		int count = 0;
		while (querySql.next()) {
			count = querySql.value(0).toInt();
		}

		QSqlQuery query;
		query.prepare("INSERT INTO partita (id, mod, nick1, nick2, punti1, punti2) "
			"VALUES (:id, :mod, :nick1, :nick2, :punti1, :punti2)");
		query.bindValue(":id", ++count);
		query.bindValue(":mod", "CS");
		query.bindValue(":nick1", n1);
		query.bindValue(":nick2", n2);
		query.bindValue(":punti1", p1);
		query.bindValue(":punti2", p2);

		bool ok = query.exec();
		if (ok == false)
			QMessageBox::critical(this, tr("Error"), tr("Error while storing data"));
	}
}

void Server::tornaMenuIniziale()
{
	server->close();
	socket->close();
	buffer->close();
	setVisible(false);
	Avvio *a = new Avvio(db);
	a->show();
	a->setVisible(true);
}

void Server::settaIcone()
{
	QString tmp;

	tmp.setNum(mazzo->getMax() + 1);
	if (mazzo->getMax() == 0)
		labelUltima->setPixmap(QPixmap(":/icons/null2.png"));
	else
		labelUltima->setPixmap(QPixmap(":mazzi/" + tipoMazzo + "/mazzo.png"));

	sendMessage("12" + tmp + "|");

	if (g1->getCarta(0) != NULL) {
		pushButtonG1C1->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG1C1->setIcon(QIcon(":icons/null.png"));

	if (g1->getCarta(1) != NULL) {
		pushButtonG1C2->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG1C2->setIcon(QIcon(":icons/null.png"));

	if (g1->getCarta(2) != NULL) {
		pushButtonG1C3->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG1C3->setIcon(QIcon(":icons/null.png"));

	if (g2->getCarta(0) != NULL) {
		tmp.setNum(g2->getCarta(0)->getNumero());
		QString icona20 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(0)->getSeme() + ".png";
		pushButtonG2C1->setIcon(QIcon(icona20));
	} else
		pushButtonG2C1->setIcon(QIcon(":icons/null.png"));

	if (g2->getCarta(1) != NULL) {
		tmp.setNum(g2->getCarta(1)->getNumero());
		QString icona21 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(1)->getSeme() + ".png";
		pushButtonG2C2->setIcon(QIcon(icona21));
	} else
		pushButtonG2C2->setIcon(QIcon(":icons/null.png"));

	if (g2->getCarta(2) != NULL) {
		tmp.setNum(g2->getCarta(2)->getNumero());
		QString icona22 = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getCarta(2)->getSeme() + ".png";
		pushButtonG2C3->setIcon(QIcon(icona22));
	} else
		pushButtonG2C3->setIcon(QIcon(":icons/null.png"));
}

void Server::gestisciUltimaMano()
{
	if (mazzo->getMax() == 1) {
		if (turno == 2) {
			g2->aggiungiCarta(pos2, mazzo->daiIesima(0));
			g1->aggiungiCarta(pos1, br);

			labelBr->setPixmap(QPixmap(":icons/null.png"));
			labelUltima->setPixmap(QPixmap(":icons/null2.png"));
			mazzo->settaMax(0);
			lineEditCar->setText("0");

			QString num, pos;
			num.setNum(g1->getCarta(pos1)->getNumero());
			pos.setNum(pos1);
			sendMessage("02" + pos + num + g1->getCarta(pos1)->getSeme() + "|");
			sendMessage("08|");

		} else {
			g1->aggiungiCarta(pos1, mazzo->daiIesima(0));
			g2->aggiungiCarta(pos2, br);

			labelBr->setPixmap(QPixmap(":icons/null.png"));
			labelUltima->setPixmap(QPixmap(":icons/null2.png"));
			mazzo->settaMax(0);
			lineEditCar->setText("0");

			QString num, pos;
			num.setNum(g1->getCarta(pos1)->getNumero());
			pos.setNum(pos1);
			sendMessage("02" + pos + num + g1->getCarta(pos1)->getSeme() + "|");
			sendMessage("08|");
		}
	} else {
		g1->settaCartaNull(pos1);
		g2->settaCartaNull(pos2);

		if (pos1 == 0) {
			pushButtonG1C1->setIcon(QIcon(":icons/null.png"));
			pushButtonG1C1->setEnabled(false);
		} else if (pos1 == 1) {
			pushButtonG1C2->setIcon(QIcon(":icons/null.png"));
			pushButtonG1C2->setEnabled(false);
		} else if (pos1 == 2) {
			pushButtonG1C3->setIcon(QIcon(":icons/null.png"));
			pushButtonG1C3->setEnabled(false);
		}

		if (pos2 == 0) {
			if (g2->getCarta(0) != NULL)
				pushButtonG2C1->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
		} else if (pos2 == 1) {
			if (g2->getCarta(1) != NULL)
				pushButtonG2C2->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
		} else if (pos2 == 2) {
			if (g2->getCarta(2) != NULL)
				pushButtonG2C3->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
		}

		if (ultima == 2) {
			labelG1->setPixmap(QPixmap(":icons/null.png"));
			labelG2->setPixmap(QPixmap(":icons/null.png"));

			QString puntiG2;
			puntiG2.setNum(g2->getPunti());
			lineEditPuntiG2->setText(puntiG2);
			lineEditCar->setFocus();

			QString punti1, punti2;
			punti1.setNum(g1->getPunti());
			punti2.setNum(g2->getPunti());

			int conclusione;

			if (g1->getPunti() > g2->getPunti())
				conclusione = Fine::Sconfitta;
			else if (g1->getPunti() < g2->getPunti())
				conclusione = Fine::Vittoria;
			else
				conclusione = Fine::Pareggio;

			sendMessage("10" + punti2 + "|");
			Fine *fine = new Fine(conclusione, g1->getPunti(), g2->getPunti(), groupBoxG1->title(), nick);
			fine->exec();

			// il giocatore attuale e' posto al primo posto
			aggiornaDataBase(nick, groupBoxG1->title(), g2->getPunti(), g1->getPunti());

			tornaMenuIniziale();
			return;
		} else {
			ultima++;
		}
	}
}

// eventi server
void Server::cliccataCarta1()
{
	g2->giocaCarta(0);
	pushButtonG2C1->setIcon(QIcon(":/icons/null.png"));
	pushButtonG2C1->setEnabled(false);
	pushButtonG2C2->setEnabled(false);
	pushButtonG2C3->setEnabled(false);

	QString tmp1;
	tmp1.setNum(g2->getGiocata()->getNumero());
	QString carta1 = ":mazzi/" + tipoMazzo + "/" + tmp1 + g2->getGiocata()->getSeme() + ".png";
	labelG2->setPixmap(carta1);

	sendMessage("050" + tmp1 + g2->getGiocata()->getSeme() + "|");

	pos2 = 0;
	controllaGiocata(2);
}

void Server::cliccataCarta2()
{
	g2->giocaCarta(1);
	pushButtonG2C2->setIcon(QIcon(":/icons/null.png"));
	pushButtonG2C1->setEnabled(false);
	pushButtonG2C2->setEnabled(false);
	pushButtonG2C3->setEnabled(false);

	QString tmp2;
	tmp2.setNum(g2->getGiocata()->getNumero());
	QString carta2 = ":mazzi/" + tipoMazzo + "/" + tmp2 + g2->getGiocata()->getSeme() + ".png";
	labelG2->setPixmap(carta2);

	sendMessage("051" + tmp2 + g2->getGiocata()->getSeme() + "|");

	pos2 = 1;
	controllaGiocata(2);
}

void Server::cliccataCarta3()
{
	g2->giocaCarta(2);
	pushButtonG2C3->setIcon(QIcon(":/icons/null.png"));
	pushButtonG2C1->setEnabled(false);
	pushButtonG2C2->setEnabled(false);
	pushButtonG2C3->setEnabled(false);

	QString tmp3;
	tmp3.setNum(g2->getGiocata()->getNumero());
	QString carta3 = ":mazzi/" + tipoMazzo + "/" + tmp3 + g2->getGiocata()->getSeme() + ".png";
	labelG2->setPixmap(carta3);

	sendMessage("052" + tmp3 + g2->getGiocata()->getSeme() + "|");

	pos2 = 2;
	controllaGiocata(2);
}

void Server::riceviSegnaleRaccoltaG1()
{
	g1->aumentaPunti(g1->getGiocata()->getPunteggio() + g2->getGiocata()->getPunteggio());
	QString punti;
	punti.setNum(g1->getPunti());
	sendMessage("07" + punti + "|");

	g1->settaNullGiocata();
	g2->settaNullGiocata();

	labelG1->setPixmap(QPixmap(":icons/null.png"));
	labelG2->setPixmap(QPixmap(":icons/null.png"));
	turno = 1;

	if (mazzo->getMax() <= 1) {
		gestisciUltimaMano();
	} else {
		g1->aggiungiCarta(pos1, mazzo->daiCarta());
		g2->aggiungiCarta(pos2, mazzo->daiCarta());
		settaNumCarte();
		QString num, pos;
		num.setNum(g1->getCarta(pos1)->getNumero());
		pos.setNum(pos1);
		sendMessage("02" + pos + num + g1->getCarta(pos1)->getSeme() + "|");
	}

	sendMessage("06|");
	sendMessage("09|");
	gestisciTurno(1);
	settaIcone();

	setNormale(lineEditPuntiG1);
}

void Server::riceviSegnaleRaccoltaG2()
{
	g2->aumentaPunti(g1->getGiocata()->getPunteggio() + g2->getGiocata()->getPunteggio());
	QString punti;
	punti.setNum(g2->getPunti());
	lineEditPuntiG2->setText(punti);

	g1->settaNullGiocata();
	g2->settaNullGiocata();

	labelG1->setPixmap(QPixmap(":icons/null.png"));
	labelG2->setPixmap(QPixmap(":icons/null.png"));
	turno = 2;

	if (mazzo->getMax() <= 1) {
		gestisciUltimaMano();
	} else {
		g2->aggiungiCarta(pos2, mazzo->daiCarta());
		g1->aggiungiCarta(pos1, mazzo->daiCarta());
		settaNumCarte();
		QString num, pos;
		num.setNum(g1->getCarta(pos1)->getNumero());
		pos.setNum(pos1);
		sendMessage("02" + pos + num + g1->getCarta(pos1)->getSeme() + "|");
	}

	sendMessage("06|");

	gestisciTurno(2);
	settaIcone();
	setNormale(lineEditPuntiG2);
}

void Server::esciPartita()
{
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		tornaMenuIniziale();
	}
}

void Server::closeEvent(QCloseEvent *event)
{
	event->ignore();
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		sendMessage("13|");
		tornaMenuIniziale();
	}
}

void Server::mostraStatistiche()
{
	Statistiche *s = new Statistiche(db);
	s->show();
	s->setVisible(true);
}
