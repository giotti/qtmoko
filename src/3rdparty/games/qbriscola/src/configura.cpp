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

#include "configura.h"

Configura::Configura(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width()-width())/2, (dim.height()-height())/2, width(), height());

	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(aggiornaText(int)));
	connect(pushButtonOk, SIGNAL(clicked()), this, SLOT(applica()));
	connect(pushButtonAnnulla, SIGNAL(clicked()), this, SLOT(annulla()));

	// leggo da file
	QString home = QDir::homePath();
	QFile file(home + "/.qbriscola/velocita");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);

	QString val = in.readLine();
	slider->setValue(val.toInt());
	lineEdit->setText(val);
}

void Configura::aggiornaText(int val)
{
	QString v;
	v.setNum(val);
	lineEdit->setText(v);
}

void Configura::applica()
{
	int ms = slider->value();
	emit segnaleAggiornaThread(ms);

	// scrivo su un file a parte
	QString home = QDir::homePath();
	QFile file(home + "/.qbriscola/velocita");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	out << ms << endl;

	setVisible(false);
}

void Configura::annulla()
{
	setVisible(false);
}

