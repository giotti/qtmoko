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

#include "avvio.h"
#include "finestra.h"
#include "client.h"
#include "server.h"
#include "statistiche.h"

QString tipoMazzo = "null";

Avvio::Avvio(QSqlDatabase _db, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	db = _db;
	leggiImpostazioni();

	if (comboBox->currentIndex() == 1)
		groupBoxRete->setEnabled(true);

	connect(pushButtonOk, SIGNAL(clicked()), this, SLOT(avviaConnetti()));
	connect(pushButtonEsci, SIGNAL(clicked()), this, SLOT(esci()));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(abilitaRete(int)));
	connect(radioButtonServer, SIGNAL(clicked()), this, SLOT(abilitaServer()));
	connect(radioButtonClient, SIGNAL(clicked()), this, SLOT(abilitaClient()));
	connect(pushButtonStatistiche, SIGNAL(clicked()), this, SLOT(mostraStatistiche()));

	pushButtonOk->setFocus();
}

void Avvio::mostraStatistiche()
{
	Statistiche *s = new Statistiche(db);
	s->show();
	s->setVisible(true);
}

void Avvio::avviaConnetti()
{
	QString ip;
	int port;
	QString nick = lineEditNick->displayText();

	// per il mazzo
	if (comboBoxCarte->currentText() == "Piacentine")
		tipoMazzo = "pia";
	else if (comboBoxCarte->currentText() == "Bergamasche")
		tipoMazzo = "ber";
	else if (comboBoxCarte->currentText() == "Napoletane")
		tipoMazzo = "nap";
	else if (comboBoxCarte->currentText() == "Siciliane")
		tipoMazzo = "sic";

	if (nick == "") {
		QMessageBox::warning(this, tr("Warning"), tr("Set your nickname first"));
		return;
	}

	if (comboBox->currentIndex() == 0) {
		Finestra *finestra = new Finestra(db, nick);
		finestra->show();

	} else if (comboBox->currentIndex() == 1) {
		if (radioButtonClient->isChecked()) {
			// client
			port = spinBoxClientPorta->value();
			ip = lineEditClientIp->displayText();

			Client *client = new Client(db, nick, ip, port);
			client->show();

		} else if (radioButtonServer->isChecked()) {
			// server
			port = spinBoxServerPorta->value();

			Server *server = new Server(db, nick, port);
			server->show();
		} else {
			QString messaggio = tr("Select a valid option");
			QMessageBox::critical(this, tr("Error"), messaggio);
			return;
		}
	}

	salvaImpostazioni();

	this->setVisible(false);
	this->destroyed();
}

void Avvio::salvaImpostazioni()
{
	QString home = QDir::homePath();
	QFile file(home + "/.qbriscola/config");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	out << comboBox->currentIndex() << endl;
	out << lineEditNick->text() << endl;
	out << spinBoxServerPorta->value() << endl;
	out << lineEditClientIp->text() << endl;
	out << spinBoxClientPorta->value() << endl;
	out << height() << endl;
	out << width() << endl;
	out << comboBoxCarte->currentIndex() << endl;
}

void Avvio::leggiImpostazioni()
{
	QString home = QDir::homePath();
	QFile file(home + "/.qbriscola/config");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);

	comboBox->setCurrentIndex(in.readLine().toInt());
	lineEditNick->setText(in.readLine());
	spinBoxServerPorta->setValue(in.readLine().toInt());
	lineEditClientIp->setText(in.readLine());
	spinBoxClientPorta->setValue(in.readLine().toInt());
	int h = in.readLine().toInt();
	int w = in.readLine().toInt();
	comboBoxCarte->setCurrentIndex(in.readLine().toInt());

	if ((h == 0) && (w == 0))
		return;

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width() - w) / 2, (dim.height() - h) / 2, w, h);
}

void Avvio::esci()
{
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from QBriscola?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes)
		exit(1);
}

void Avvio::abilitaRete(int i)
{
	if (i == 0)
		groupBoxRete->setEnabled(false);
	else if (i == 1)
		groupBoxRete->setEnabled(true);
}

void Avvio::abilitaServer()
{
	spinBoxClientPorta->setEnabled(false);
	lineEditClientIp->setEnabled(false);
	spinBoxServerPorta->setEnabled(true);

	labelClientPorta->setEnabled(false);
	labelClientIp->setEnabled(false);
	labelServerPorta->setEnabled(true);

	pushButtonOk->setEnabled(true);

	spinBoxServerPorta->setFocus();
}

void Avvio::abilitaClient()
{
	spinBoxClientPorta->setEnabled(true);
	lineEditClientIp->setEnabled(true);
	spinBoxServerPorta->setEnabled(false);

	labelClientPorta->setEnabled(true);
	labelClientIp->setEnabled(true);
	labelServerPorta->setEnabled(false);

	pushButtonOk->setEnabled(true);

	spinBoxClientPorta->setFocus();
}

void Avvio::closeEvent(QCloseEvent *event)
{
	event->ignore();
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from QBriscola?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		exit(1);
	}
}

