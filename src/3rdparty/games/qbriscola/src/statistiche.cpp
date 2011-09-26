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

#include "statistiche.h"

Statistiche::Statistiche(QSqlDatabase _db, QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width()-width())/2, (dim.height()-height())/2, width(), height());

	db = _db;

	connect(radioButtonComputer, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonUmano, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonTuttoTipo, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonVittoria, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonSconfitta, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonPareggio, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(radioButtonTuttoRis, SIGNAL(clicked()), this, SLOT(stampaQuery()));
	connect(lineEditInput, SIGNAL(textChanged(QString)), this, SLOT(stampaQuery(QString)));
	connect(pushButtonCancellaStatistiche, SIGNAL(clicked()), this, SLOT(cancellaStatistiche()));

	stampaQuery();
}

void Statistiche::cancellaStatistiche()
{
	QString m = tr("Do you want to delete your statistics?");
	int ret = QMessageBox::warning(this, tr("Warning"), m, QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::Yes) {
		QString queryString = "delete from partita";
		QSqlQuery query(db);
		query.exec(queryString);

		stampaQuery();
	}
}

void Statistiche::stampaQuery(QString)
{
	stampaQuery();
}

void Statistiche::stampaQuery()
{
	model = new QSqlTableModel(this);
	model->setTable("partita");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	QString stringFilter = "";

	QString nick = lineEditInput->text();
	if (nick != "")
		stringFilter += "(nick1 = '" + nick + "' or nick2 = '" + nick + "') ";
	else
		stringFilter += "(1=1) ";

	if (radioButtonComputer->isChecked())
		stringFilter += "and (mod = 'AI') ";
	else if (radioButtonUmano->isChecked())
		stringFilter += "and (mod = 'CS') ";

	if (radioButtonVittoria->isChecked())
		stringFilter += "and (punti1 > punti2)";
	else if (radioButtonSconfitta->isChecked())
		stringFilter += "and (punti1 < punti2)";
	else if (radioButtonPareggio->isChecked())
		stringFilter += "and (punti1 = punti2)";

	// query in stile qt
	model->select();
	model->setFilter(stringFilter);

	model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("mode"));
	model->setHeaderData(2, Qt::Horizontal, tr("player 1"));
	model->setHeaderData(3, Qt::Horizontal, tr("player 2"));
	model->setHeaderData(4, Qt::Horizontal, tr("pts 1"));
	model->setHeaderData(5, Qt::Horizontal, tr("pts 2"));
	tableView->setModel(model);

	tableView->setColumnWidth(0, 40);
	tableView->setColumnWidth(1, 40);
	tableView->setColumnWidth(2, 100);
	tableView->setColumnWidth(3, 100);
	tableView->setColumnWidth(4, 50);
	tableView->setColumnWidth(5, 50);
	tableView->setEditTriggers(QTableView::NoEditTriggers);
	tableView->verticalHeader()->hide();

	QString countString;
	countString.setNum(model->rowCount());

	labelInformazioni->setText(tr("Have been found ") + countString + tr(" matches."));
}

