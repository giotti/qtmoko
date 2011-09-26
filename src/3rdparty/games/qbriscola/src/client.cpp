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

#include "client.h"

Client::Client(QSqlDatabase _db, QString _nick, QString _ip, int _porta, QWidget *parent) :
	QMainWindow(parent)
{
	setupUi(this);
	db = _db;
	nick = _nick;
	ip = _ip;
	porta = _porta;

	personalizza();

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width() - width()) / 2, (dim.height() - height()) / 2, width(), height());

	socket = new QTcpSocket(this);
	buffer = new QBuffer(this);
	buffer->open(QIODevice::ReadWrite);

	connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(actionStatistiche, SIGNAL(triggered()), this, SLOT(mostraStatistiche()));
	connect(pushButtonG1C1, SIGNAL(clicked()), this, SLOT(cliccataCarta1()));
	connect(pushButtonG1C2, SIGNAL(clicked()), this, SLOT(cliccataCarta2()));
	connect(pushButtonG1C3, SIGNAL(clicked()), this, SLOT(cliccataCarta3()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errore(QAbstractSocket::SocketError)));

	socket->connectToHost(ip, porta);

	sendMessage("00" + nick + "|");

	// inizializza il vettore di puntatori a metodi
	metodo[0] = &Client::settaNickServer;
	metodo[1] = &Client::settaBriscola;
	metodo[2] = &Client::settaCarta;
	metodo[3] = &Client::settaNumCarte;
	metodo[4] = &Client::settaTurno;
	metodo[5] = &Client::settaCartaServer;
	metodo[6] = &Client::pulisciTavolo;
	metodo[7] = &Client::aggiornaPunti;
	metodo[8] = &Client::ultimaMano;
	metodo[9] = &Client::disabilitaCarte;
	metodo[10] = &Client::stabilisciVincitore;
	metodo[11] = &Client::attivaColorazione;
	metodo[12] = &Client::aggiustaMazzo;
	metodo[13] = &Client::serverAbbandonato;
}

void Client::errore(QAbstractSocket::SocketError se)
{
	if (se == QAbstractSocket::ConnectionRefusedError) {
		delete socket;
		delete buffer;

		QMessageBox::critical(this, tr("Error"), tr("No response from server"));

		socket = new QTcpSocket(this);
		buffer = new QBuffer(this);
		buffer->open(QIODevice::ReadWrite);
	}
}

Client::~Client()
{
	buffer->close();
}

void Client::personalizza()
{
	groupBoxG1->setTitle(nick);
	lineEditPuntiG1->setText("0");
	lineEditPuntiG2->setText("/");
	pushButtonG2C1->setEnabled(false);
	pushButtonG2C2->setEnabled(false);
	pushButtonG2C3->setEnabled(false);
}

void Client::sendMessage(QString message)
{
	socket->write(message.toLatin1());
}

void Client::receiveMessage()
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

void Client::serverAbbandonato(QString)
{
	QMessageBox::critical(this, tr("Information"), tr("Server has left from this match"));
	tornaMenuIniziale();
}

void Client::aggiustaMazzo(QString car)
{
	if (car.toInt() == 1)
		labelUltima->setPixmap(QPixmap(":/icons/null2.png"));
	else
		labelUltima->setPixmap(QPixmap(":mazzi/" + tipoMazzo + "/mazzo.png"));
}

void Client::attivaColorazione(QString presa)
{
	int g = presa.toInt();

	if (g == 1) {
		setVerde(lineEditPuntiG1);
	} else if (g == 2) {
		setVerde(lineEditPuntiG2);
	}
}

void Client::stabilisciVincitore(QString p2)
{
	int punti1 = lineEditPuntiG1->text().toInt();
	int punti2 = p2.toInt();

	int conclusione;

	if (punti1 > punti2)
		conclusione = Fine::Vittoria;
	else if (punti1 < punti2)
		conclusione = Fine::Sconfitta;
	else
		conclusione = Fine::Pareggio;

	Fine *fine = new Fine(conclusione, punti1, punti2, nick, groupBoxG2->title());
	fine->exec();

	// il giocatore attuale e' posto al primo posto
	aggiornaDataBase(nick, groupBoxG2->title(), punti1, punti2);

	tornaMenuIniziale();
}

void Client::aggiornaDataBase(QString n1, QString n2, int p1, int p2)
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

void Client::tornaMenuIniziale()
{
	socket->close();
	buffer->close();
	setVisible(false);
	Avvio *a = new Avvio(db);
	a->show();
	a->setVisible(true);
	destroy();
}

void Client::disabilitaCarte(QString)
{
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);
}

void Client::ultimaMano(QString)
{
	labelBr->setPixmap(QPixmap(":icons/null.png"));
	labelUltima->setPixmap(QPixmap(":icons/null2.png"));
	lineEditCar->setText("0");
}

void Client::aggiornaPunti(QString p)
{
	lineEditPuntiG1->setText(p);
}

void Client::pulisciTavolo(QString)
{
	labelG1->setPixmap(QPixmap(":/icons/null.png"));
	labelG2->setPixmap(QPixmap(":/icons/null.png"));
	setNormale(lineEditPuntiG1);
	setNormale(lineEditPuntiG2);
}

void Client::settaCartaServer(QString c)
{
	QString pos = c.mid(0, 1);
	QString iconaG2 = ":mazzi/" + tipoMazzo + "/" + c.mid(1) + ".png";
	labelG2->setPixmap(QPixmap(iconaG2));

	if (pos == "0")
		pushButtonG2C1->setIcon(QIcon(":/icons/null.png"));
	else if (pos == "1")
		pushButtonG2C2->setIcon(QIcon(":/icons/null.png"));
	else
		pushButtonG2C3->setIcon(QIcon(":/icons/null.png"));

	posServer = pos.toInt();
}

void Client::settaTurno(QString)
{
	pushButtonG1C1->setEnabled(true);
	pushButtonG1C2->setEnabled(true);
	pushButtonG1C3->setEnabled(true);
}

void Client::settaNumCarte(QString c)
{
	lineEditCar->setText(c);
	if (c == "2")
		setRosso(lineEditCar);
}

void Client::settaCarta(QString c)
{
	QString tmp;

	if (c[0] == QChar('0')) {
		QString icona10 = ":mazzi/" + tipoMazzo + "/" + tmp + c.mid(1) + ".png";
		pushButtonG1C1->setIcon(QIcon(icona10));
	} else if (c[0] == QChar('1')) {
		QString icona11 = ":mazzi/" + tipoMazzo + "/" + tmp + c.mid(1) + ".png";
		pushButtonG1C2->setIcon(QIcon(icona11));
	} else if (c[0] == QChar('2')) {
		QString icona12 = ":mazzi/" + tipoMazzo + "/" + tmp + c.mid(1) + ".png";
		pushButtonG1C3->setIcon(QIcon(icona12));
	}

	if (posServer == 0) {
		pushButtonG2C1->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else if (posServer == 1) {
		pushButtonG2C2->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else if (posServer == 2) {
		pushButtonG2C3->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	}
}

void Client::settaBriscola(QString br)
{
	QString briscola = ":mazzi/" + tipoMazzo + "/" + br + ".png";
	labelBr->setPixmap(briscola);
	setStatusTip("Briscola: " + br);
}

void Client::settaNickServer(QString nickServer)
{
	groupBoxG2->setTitle(nickServer);
	setWindowTitle("QBriscola - " + nick + " vs " + nickServer);
}

void Client::setVerde(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(147, 255, 101));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Client::setRosso(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 0, 0));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Client::setNormale(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 255, 255));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

// eventi client
void Client::cliccataCarta1()
{
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	QIcon icon = pushButtonG1C1->icon();
	QPixmap pixmap = icon.pixmap(75, 134);
	labelG1->setPixmap(pixmap);
	pushButtonG1C1->setIcon(QIcon(":/icons/null.png"));

	sendMessage("010|");
}

void Client::cliccataCarta2()
{
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	QIcon icon = pushButtonG1C2->icon();
	QPixmap pixmap = icon.pixmap(75, 134);
	labelG1->setPixmap(pixmap);
	pushButtonG1C2->setIcon(QIcon(":/icons/null.png"));

	sendMessage("011|");
}

void Client::cliccataCarta3()
{
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	QIcon icon = pushButtonG1C3->icon();
	QPixmap pixmap = icon.pixmap(75, 134);
	labelG1->setPixmap(pixmap);
	pushButtonG1C3->setIcon(QIcon(":/icons/null.png"));

	sendMessage("012|");
}

void Client::esciPartita()
{
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from this match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		tornaMenuIniziale();
	}
}

void Client::closeEvent(QCloseEvent *event)
{
	event->ignore();
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from this match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		sendMessage("02|");
		tornaMenuIniziale();
	}
}

void Client::mostraStatistiche()
{
	Statistiche *s = new Statistiche(db);
	s->show();
	s->setVisible(true);
}
