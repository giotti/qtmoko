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

#include "fine.h"

Fine::Fine(int c, int p1, int p2, QString g1, QString g2, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);

	connect(pushButtonOk, SIGNAL(clicked()), this, SLOT(ok()));

	if (c == Vittoria) {
		labelFine->setPixmap(QString(":/icons/vittoria.png"));
		labelMessaggio->setText(tr("Congratulations, you have won!"));
	} else if (c == Sconfitta) {
		labelFine->setPixmap(QString(":/icons/sconfitta.png"));
		labelMessaggio->setText(tr("Sorry, you have lost..."));
	} else {
		labelFine->setPixmap(QString(":/icons/pareggio.png"));
		labelMessaggio->setText(tr("You have draw."));
	}

	labelG1->setText(g1);
	QString tmp1;
	tmp1.setNum(p1);
	lineEditG1->setText(tmp1);
	
	labelG2->setText(g2);
	QString tmp2;
	tmp2.setNum(p2);
	lineEditG2->setText(tmp2);
}

void Fine::ok()
{
	setVisible(false);
	destroy();
}
