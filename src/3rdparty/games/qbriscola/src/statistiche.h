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

#ifndef STATISTICHE_H_
#define STATISTICHE_H_

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "ui_statistiche.h"

class Statistiche : public QWidget, public Ui::Statistiche
{
Q_OBJECT

public:
	Statistiche(QSqlDatabase, QWidget *parent = 0);

public slots:
	void stampaQuery();
	void stampaQuery(QString);
	void cancellaStatistiche();

private:
	QSqlDatabase db;
	QSqlTableModel *model;
};

#endif
