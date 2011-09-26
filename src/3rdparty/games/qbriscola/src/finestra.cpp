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

#include "finestra.h"

Finestra::Finestra(QSqlDatabase _db, QString _nick, QWidget *parent) :
	QMainWindow(parent)
{
	setupUi(this);
	db = _db;
	nick = _nick;

	/*qDebug() << tipoMazzo;*/

	th = new Thread();
	conf = new Configura();

	connect(pushButtonG1C1, SIGNAL(clicked()), this, SLOT(cliccataCarta1()));
	connect(pushButtonG1C2, SIGNAL(clicked()), this, SLOT(cliccataCarta2()));
	connect(pushButtonG1C3, SIGNAL(clicked()), this, SLOT(cliccataCarta3()));
	connect(actionEsci, SIGNAL(triggered()), this, SLOT(esciPartita()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionStatistiche, SIGNAL(triggered()), this, SLOT(mostraStatistiche()));
	connect(actionConfigura, SIGNAL(triggered()), this, SLOT(avviaConfigura()));
	connect(th, SIGNAL(segnaleRaccoltaG1()), this, SLOT(riceviSegnaleRaccoltaG1()), Qt::QueuedConnection);
	connect(th, SIGNAL(segnaleRaccoltaG2()), this, SLOT(riceviSegnaleRaccoltaG2()), Qt::QueuedConnection);
	connect(conf, SIGNAL(segnaleAggiornaThread(int)), this, SLOT(aggiornaThread(int)), Qt::QueuedConnection);

	QDesktopWidget *widget = QApplication::desktop();
	QRect dim = widget->screenGeometry();
	setGeometry((dim.width()-width())/2, (dim.height()-height())/2, width(), height());
	setWindowTitle("QBriscola - " + nick + " vs Computer");
	groupBoxG1->setTitle(nick);
	labelG1->setPixmap(QPixmap(""));
	labelG2->setPixmap(QPixmap(""));

	// leggo da file
	QString home = QDir::homePath();
	QFile file(home + "/.qbriscola/velocita");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		th->setSleepMs(500);
	} else {
		QTextStream in(&file);
		int val = in.readLine().toInt();
		th->setSleepMs(val);
	}

	setNormale(lineEditPuntiG1);
	setNormale(lineEditPuntiG2);

	randomTurno = new Random(time(0));
	randomCarta = new Random(time(0));

	iniziaPartita();
}

void Finestra::inizializzo()
{
	br = new Carta("spadi",1,10,11);
	QString tmp;
	tmp.setNum(br->getNumero());
	QString briscola = ":mazzi/" + tipoMazzo + "/" + tmp + br->getSeme() + ".png";
	labelBr->setPixmap(briscola);

	setStatusTip("Briscola: " + br->getSeme());

	mazzo->settaMax(1);

	g2 = new Giocatore(new Carta("denari",1,10,11), new Carta("denari",3,9,10), new Carta("spadi",5,3,0));
	g1 = new Giocatore(new Carta("denari",7,5,0), new Carta("bastoni",5,0,0), new Carta("bastoni",6,0,0));
	turno = 1;
}

void Finestra::iniziaPartita()
{
	// creazione del mazzo
	mazzo = new Mazzo();
	ultima = 0;
	turno = 0;
	pos1 = -1;
	pos2 = -1;

	// crea i 2 giocatori
	g1 = new Giocatore(mazzo->daiCarta(), mazzo->daiCarta(), mazzo->daiCarta());
	g2 = new Giocatore(mazzo->daiCarta(), mazzo->daiCarta(), mazzo->daiCarta());

	// setta la briscola
	settaBriscola();
	// inizializzo();
	// imposta sulla finestra le carte date ai giocatori
	settaIcone();
	settaNumCarte();

	turno = chiInizia();
	gestisciTurno(turno);
}

void Finestra::settaBriscola()
{
	br = mazzo->daiCarta();
	QString tmp;
	tmp.setNum(br->getNumero());
	QString briscola = ":mazzi/" + tipoMazzo + "/" + tmp + br->getSeme() + ".png";
	labelBr->setPixmap(briscola);

	setStatusTip("Briscola: " + br->getSeme());
}

void Finestra::settaNumCarte()
{
	QString tmp;
	tmp.setNum(mazzo->getMax()+1);
	lineEditCar->setText(tmp);

	if (tmp == "2")
		setRosso(lineEditCar);
}

int Finestra::chiInizia()
{
	int r = static_cast<int>(randomTurno->rand()*(2));

	if (r > 0.5)
		return 2;
	else
		return 1;
}

void Finestra::gestisciTurno(int giocatore)
{
	int g = giocatore;
	if (g == 1) {
		if (g1->getCarta(0) != NULL)
			pushButtonG1C1->setEnabled(true);
		if (g1->getCarta(1) != NULL)
			pushButtonG1C2->setEnabled(true);
		if (g1->getCarta(2) != NULL)
			pushButtonG1C3->setEnabled(true);
	} else {
		if (g1->getCarta(0) != NULL)
			pushButtonG1C1->setEnabled(false);
		if (g1->getCarta(1) != NULL)
			pushButtonG1C2->setEnabled(false);
		if (g1->getCarta(2) != NULL)
			pushButtonG1C3->setEnabled(false);

		if (g1->getGiocata() == NULL)
			giocaComputer();
		else
			rispostaComputer();
	}
}

void Finestra::giocaComputer()
{
	if ((g2->getCarta(0) == NULL) && (g2->getCarta(1) == NULL) && (g2->getCarta(2) == NULL)) {
		return;
	}

	int minBr = cercaMinBriscola();
	int minPr = cercaMinPriorita();
	int numBr = contaBriscole();
	int maxPr = cercaMaxPriorita();
	int cartaBuona = cercaCartaBuona();
	int cartaLibera = cercaCartaLibera();
	int cartaSemiLibera = cercaCartaSemiLibera();
	int caricoLibero = cercaCaricoLibero();

	// se e' l'ultima mano
	if ((mazzo->getMax() == 1) && (br->getPunteggio() >= 10)) {
		if (numBr < 3) {
			// Carico perche' sul tavolo c'e' una briscola grossa
			giocataComputer(maxPr);
		} else {
			giocataComputer(minBr);
		}
	} else {
		if ((maxPr != -1) && ((brUscite.size() + numBr) == 9) && (mazzo->getMax() >= 1) && (g2->getCarta(maxPr)->getPunteggio() == 11)) {
			// Gioco un Asso, tanto l'avversario ha zero briscole
			giocataComputer(maxPr);
		} else if (((brUscite.size() + numBr) == 9) && (mazzo->getMax() >= 1) && (caricoLibero != -1)) {
			// Gioco un Tre libero, tanto l'avversario ha zero briscole
			giocataComputer(caricoLibero);
		} else if ((maxPr != -1) && ((brUscite.size() + numBr) == 10) && (mazzo->getMax() == 0) && (g2->getCarta(maxPr)->getPunteggio() == 11)) {
			// UM Gioco un Asso, tanto l'avversario ha zero briscole
			giocataComputer(maxPr);
		} else if (((brUscite.size() + numBr) == 10) && (mazzo->getMax() == 0) && (caricoLibero != -1)) {
			// UM Gioco un Tre libero, tanto l'avversario ha zero briscole
			giocataComputer(caricoLibero);
		} else if (cartaBuona != -1) {
			// c'e' una carta buona (è la massima in circolazione per quel seme)
			// Trovata carta buona, e la gioco
			giocataComputer(cartaBuona);
		} else if (cartaLibera != -1) {
			// i carchi sono usciti (potrebbe essere in giro una carta <= re)
			// Trovata carta libera, Tre e Asso sono usciti, e la gioco
			giocataComputer(cartaLibera);
		} else if (cartaSemiLibera != -1) {
			// e' uscito un carico
			// Trovata carta semi libera, Tre o Asso sono usciti, e la gioco
			giocataComputer(cartaSemiLibera);
		} else {
			if (minPr == -1) {
				// se ha tutte briscole, gioca la piu piccola
				giocataComputer(minBr);
			} else if (g2->getCarta(minPr)->getPunteggio() <= 4) {
				// deve buttare o 0 o 2 o 3 o 4 punti
				giocataComputer(minPr);
			} else {
				// se deve giocare un carco, controlla il numero di briscole
				if (numBr> 1) {
					// se ha piu di una briscola, gioca la piu piccola
					giocataComputer(minBr);
				} else if ((numBr == 1) && (g2->getCarta(minBr)->getPunteggio() < 10)) {
					// se ha una sola briscola, controlla se buttare la briscola o andare pesante
					//  Ho 2 carichi e una briscoletta, gioco la briscola
					giocataComputer(minBr);
				} else {
					// se ha zero briscole, deve per forza caricare ma in modo opportuno
					if (caricoLibero == -1) {
						// Non ho Tre liberi, ora controllo...
						if (g2->getCarta(maxPr)->getPunteggio() == 10) {
							// se i carichi che ho sono solo Tre NON buoni, ne gioco purtroppo uno
							// ho solo Tre a disposizione, ne gioco uno
							giocataComputer(minPr);
						} else {
							// ho a disposizione un asso da giocare
							// ho un Asso a disposizione, e lo gioco
							giocataComputer(maxPr);
						}
					} else {
						// ho un Tre in cui l'Asso è uscito o ce l'ho io, quindi lo gioco
						// Fortunatamente ho un Tre libero, e lo gioco
						giocataComputer(caricoLibero);
					}
				}
			}
		}
	}
}

void Finestra::rispostaComputer()
{
	int minBr = cercaMinBriscola();
	int minPr = cercaMinPriorita();
	int maxSeme = cercaMaxCartaStessoSeme();
	int numBr = contaBriscole();
	int ammBr = ammazzaBriscola();
	int cons = -1;
	if (minBr != -1) {
		cons = cercaBriscolaConsecutive(minBr);
	}

	if ((mazzo->getMax() == 1) && (br->getPunteggio() >= 10)) {
		// siamo in ultima mano
		if (numBr < 3) {
			if ((maxSeme != -1) && (g2->getCarta(maxSeme)->getPunteggio() >= 10)) {
				giocataComputer(maxSeme);
			} else if (g2->getCarta(minPr)->getPunteggio() < 10) {
				giocataComputer(minPr);
			} else {
				if (minBr != -1) {
					giocataComputer(cons);
				} else {
					giocataComputer(minPr);
				}
			}
		} else
			giocataComputer(cons);
	} else {
		if (isBriscola() == true) {
			// l'utente ha giocato briscola
			// cerca di andare liscio
			if (minPr == -1) {
				// se ho tutte briscole
				if (ammBr == -1) {
					// non puo ammazzare la briscola
					// allora gli da la piu piccola
					giocataComputer(minBr);
				} else {
					// ammazza la briscola con la sua minima
					giocataComputer(ammBr);
				}
			} else {
				// se in mano il PC non ha tutte briscole
				// cerca di andare liscio
				if (g2->getCarta(minPr)->getPunteggio() >= 10) {
					// se gli deve dare un carco
					// controlla se ha una o piu briscole
					if (numBr >= 2) {
						// se ha due briscole, butta la piu piccola
						if (g2->getCarta(minBr)->getPriorita() > g1->getGiocata()->getPriorita())
							giocataComputer(cons);
						else
							giocataComputer(minBr);
					} else if ((numBr == 1) && (g2->getCarta(minBr)->getPunteggio() < 10)) {
						// se ha una sola briscola, controlla se buttare la briscola o andare pesante
						// Ho 2 carichi e una briscoletta, gioco la briscola
						giocataComputer(minBr);
					} else {
						// se ha una sola briscola o tutti i carchi, gli da un carco
						if (minBr != -1) {
							if (g2->getCarta(minBr)->getPunteggio() <= 4)
								giocataComputer(minBr);
							else
								giocataComputer(minPr);
						} else
							giocataComputer(minPr);
					}
				} else {
					// se gli deve dare 0 2 3 o 4 punti, glieli da
					giocataComputer(minPr);
				}
			}
		} else {
			// l'utente non ha giocato briscola
			// si cerca la carta piu grande dello stesso seme
			if (maxSeme != -1) {
				// il PC ha una carta maggiore con lo stesso seme
				if (g2->getCarta(maxSeme)->getPunteggio()> 0)
					giocataComputer(maxSeme);
				else
					giocataComputer(minPr);
			} else {
				// il PC non ha una carta maggiore con lo stesso seme
				// allora controlla cosa ha giocato l'utente
				if (g1->getGiocata()->getPunteggio() >= 10) {
					// l'utente ha giocato un carco
					// il computer cerca la briscola piu' piccola
					if (minBr != -1) {
						// il PC ha briscola e la gioca
						giocataComputer(cons);
					} else {
						// il PC non ha briscola
						// gli da meno punti possibili
						giocataComputer(minPr);
					}
				} else if ((g1->getGiocata()->getPunteggio() == 4) || (g1->getGiocata()->getPunteggio() == 3)) {
					// l'utente ha giocato o un re o un cavallo
					// cerca di andare liscio
					if (minPr == -1) {
						// ha tutte briscole
						giocataComputer(cons);
					} else if (g2->getCarta(minPr)->getPunteggio() >= 10) {
						// se gli deve dare un carco, ci pensa
						// cerca minBriscola
						if (minBr != -1) {
							// se ha briscola
							giocataComputer(cons);
						} else {
							// se non ha briscola, gioca il carco
							giocataComputer(minPr);
						}
					} else if (g2->getCarta(minPr)->getPunteggio() >= 3) {
						// se gli deve dare 3 o 4 punti, ci pensa
						// controlla se ha piu' di una briscola
						if (numBr >= 2) {
							// ha piu di una briscola, allora gioca la piu piccola
							giocataComputer(cons);
						} else {
							// se invece ha una sola briscola, se la tiene e gli da 3 o 4 punti
							giocataComputer(minPr);
						}
					} else if (g2->getCarta(minPr)->getPunteggio() <= 2) {
						//gli da o un liscio o 2 punti
						giocataComputer(minPr);
					}
				} else if (g1->getGiocata()->getPunteggio() <= 2) {
					// l'utente ha giocato o una fante o una scartina
					if (minPr == -1) {
						// se ha tutte briscole, gioca la piu piccola
						giocataComputer(cons);
					} else {
						// se non ha tutte briscole
						if (g2->getCarta(minPr)->getPunteggio() <= 4) {
							// se gli deve dare da un re in giu, la gioca
							giocataComputer(minPr);
						} else {
							//se invece gli deve dare un carco, ci pensa
							// cerca la minBriscola
							if (minBr == -1) {
								// non ha briscola allora gioca il carco
								giocataComputer(minPr);
							} else if (g2->getCarta(cons)->getPunteggio() >= 10) {
								// altrimenti se ha briscola, ma e' o il 3 o l asso, gli da il carco
								giocataComputer(minPr);
							} else {
								// altrimenti, se e' una briscola minore uguale al re, la gioca
								giocataComputer(cons);
							}
						}
					}
				}
			}
		}
	}
}

// metodi di risposta da parte del pc

int Finestra::cercaBriscolaConsecutive(int minBr)
{
	int cons = minBr;
	int n_br = contaBriscole();

	if (n_br == 1) {
		return minBr;
	}

	for (int i = 0; i < 3; i++) {
		if ((g2->getCarta(i) != NULL)) {
			if (g2->getCarta(i)->getSeme() == br->getSeme()) {
				if (g2->getCarta(i)->getPriorita() > g2->getCarta(cons)->getPriorita()) {
					int count = 0;
					int min = g2->getCarta(cons)->getPriorita();
					int succ = g2->getCarta(i)->getPriorita();

					// controlla tutte le carte giocate
					for (int j = 0; j < brUscite.size(); j++) {
						if ((brUscite[j]->getPriorita() > min) && (brUscite[j]->getPriorita() < succ)) {
							count++;
						}
					}

					// controlla anche le carte proprie
					for (int j = 0; j < 3; j++) {
						if ((g2->getCarta(j) != NULL)) {
							if (g2->getCarta(j)->getSeme() == br->getSeme()) {
								if ((g2->getCarta(j)->getPriorita() > min) && (g2->getCarta(j)->getPriorita() < succ)) {
									count++;
								}
							}
						}
					}

					if (count == (succ - min - 1)) {
						cons = i;
					}
				}
			}
		}
	}

	return cons;
}

int Finestra::cercaCaricoLibero()
{
	for (int i = 0; i < 3; i++) {
		if ((g2->getCarta(i) != NULL)) {
			int pr = g2->getCarta(i)->getPriorita();
			if ((g2->getCarta(i)->getSeme() != br->getSeme()) && (pr == 9)) {
				int count = 0;

				// controlla tutte le carte giocate
				for (int j = 0; j < uscite.size(); j++) {
					if ((uscite[j]->getPriorita() >= 9) && (uscite[j]->getSeme() == g2->getCarta(i)->getSeme()))
						count++;
				}

				// controlla anche le carte proprie
				for (int j = 0; j < 3; j++) {
					if ((g2->getCarta(j) != NULL)) {
						if ((g2->getCarta(j)->getPriorita() >= 9) && (g2->getCarta(j)->getSeme() == g2->getCarta(i)->getSeme()))
							count++;
					}
				}

				if (count == 2) {
					return i;
				}
			}
		}
	}

	return -1;
}

int Finestra::cercaCartaLibera()
{
	for (int i = 0; i < 3; i++) {
		if ((g2->getCarta(i) != NULL)) {
			int pr = g2->getCarta(i)->getPriorita();
			if ((g2->getCarta(i)->getSeme() != br->getSeme()) && (pr < 9)) {
				int count = 0;

				// controlla tutte le carte giocate
				for (int j = 0; j < uscite.size(); j++) {
					if ((uscite[j]->getPriorita() >= 9) && (uscite[j]->getSeme() == g2->getCarta(i)->getSeme()))
						count++;
				}

				// controlla anche le carte proprie
				for (int j = 0; j < 3; j++) {
					if ((g2->getCarta(j) != NULL)) {
						if ((g2->getCarta(j)->getPriorita() >= 9) && (g2->getCarta(j)->getSeme() == g2->getCarta(i)->getSeme()))
							count++;
					}
				}

				if ((2 - count) == 0) {
					return i;
				}
			}
		}
	}

	return -1;
}

int Finestra::cercaCartaSemiLibera()
{
	for (int i = 0; i < 3; i++) {
		if ((g2->getCarta(i) != NULL)) {
			int pr = g2->getCarta(i)->getPriorita();
			if ((g2->getCarta(i)->getSeme() != br->getSeme()) && (pr < 9)) {
				int count = 0;

				// controlla tutte le carte giocate
				for (int j = 0; j < uscite.size(); j++) {
					if ((uscite[j]->getPriorita() >= 9) && (uscite[j]->getSeme() == g2->getCarta(i)->getSeme()))
						count++;
				}

				// controlla anche le carte proprie
				for (int j = 0; j < 3; j++) {
					if ((g2->getCarta(j) != NULL)) {
						if ((g2->getCarta(j)->getPriorita() >= 9) && (g2->getCarta(j)->getSeme() == g2->getCarta(i)->getSeme()))
							count++;
					}
				}

				if ((1 - count) == 0) {
					return i;
				}
			}
		}
	}

	return -1;
}

int Finestra::cercaCartaBuona()
{
	for (int i = 0; i < 3; i++) {
		if ((g2->getCarta(i) != NULL)) {
			int pr = g2->getCarta(i)->getPriorita();
			if ((g2->getCarta(i)->getSeme() != br->getSeme()) && (pr < 9)) {
				int count = 0;

				// controlla tutte le carte giocate
				for (int j = 0; j < uscite.size(); j++) {
					if ((uscite[j]->getPriorita()> pr) && (uscite[j]->getSeme() == g2->getCarta(i)->getSeme()))
						count++;
				}

				// controlla anche le carte proprie
				for (int j = 0; j < 3; j++) {
					if ((g2->getCarta(j) != NULL)) {
						if ((g2->getCarta(j)->getPriorita()> pr) && (g2->getCarta(j)->getSeme() == g2->getCarta(i)->getSeme()))
							count++;
					}
				}

				if ((10 - count) == pr) {
					return i;
				}
			}
		}
	}

	return -1;
}

// ammazza la biscola
int Finestra::ammazzaBriscola()
{
	int carta = -1;
	int pr = 12;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if (g2->getCarta(i)->getSeme() == br->getSeme()) {
				if ((g2->getCarta(i)->getPriorita() < pr) && (g2->getCarta(i)->getPriorita()> g1->getGiocata()->getPriorita())) {
					pr = g2->getCarta(i)->getPriorita();
					carta = i;
				}
			}
		}
	}
	return carta;
}

// torna l'indice, se esiste, della briscola maggiore: -1 altrimenti
int Finestra::cercaMaxBriscola()
{
	int carta = -1;
	int pr = -1;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if (g2->getCarta(i)->getSeme() == br->getSeme()) {
				if (g2->getCarta(i)->getPriorita()> pr) {
					pr = g2->getCarta(i)->getPriorita();
					carta = i;
				}
			}
		}
	}

	return carta;
}

// conta il numero di briscole
int Finestra::contaBriscole()
{
	int n = 0;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if (g2->getCarta(i)->getSeme() == br->getSeme())
				n++;
		}
	}
	return n;
}

// torna l'indice, se esiste, della carta con minor valore (non è briscola): -1 altrimenti
int Finestra::cercaMinPriorita()
{
	int pr = 12;
	int carta = -1;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if ((g2->getCarta(i)->getPriorita() < pr) && (g2->getCarta(i)->getSeme() != br->getSeme())) {
				pr = g2->getCarta(i)->getPriorita();
				carta = i;
			}
		} else {
		}
	}

	return carta;
}

int Finestra::cercaMaxPriorita()
{
	int pr = -1;
	int carta = -1;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if ((g2->getCarta(i)->getPriorita()> pr) && (g2->getCarta(i)->getSeme() != br->getSeme())) {
				pr = g2->getCarta(i)->getPriorita();
				carta = i;
			}
		} else {
		}
	}

	return carta;
}

// torna l'indice, se esiste, della briscola minore: -1 altrimenti
int Finestra::cercaMinBriscola()
{
	int min = 12;
	int carta = -1;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if (g2->getCarta(i)->getSeme() == br->getSeme()) {
				if (g2->getCarta(i)->getPriorita() < min) {
					min = g2->getCarta(i)->getPriorita();
					carta = i;
				}
			}
		}
	}

	return carta;
}

bool Finestra::isBriscola()
{
	if (g1->getGiocata()->getSeme() == br->getSeme())
		return true;
	else
		return false;
}

// torna l'indice, se esiste, della carta maggiore dello stesso seme: -1 altrimenti
int Finestra::cercaMaxCartaStessoSeme()
{
	int max = g1->getGiocata()->getPriorita();
	int carta = -1;
	for (int i = 0; i < 3; i++) {
		if (g2->getCarta(i) != NULL) {
			if (g2->getCarta(i)->getSeme() == g1->getGiocata()->getSeme()) {
				if (g2->getCarta(i)->getPriorita()> max) {
					max = g2->getCarta(i)->getPriorita();
					carta = i;
				}
			}
		}
	}

	return carta;
}

void Finestra::giocataComputer(int i)
{
	if (i == 0) {
		g2->giocaCarta(0);
		pushButtonG2C1->setIcon(QIcon(":icons/null.png"));
		pos2 = 0;
	} else if (i == 1) {
		g2->giocaCarta(1);
		pushButtonG2C2->setIcon(QIcon(":icons/null.png"));
		pos2 = 1;
	} else {
		g2->giocaCarta(2);
		pushButtonG2C3->setIcon(QIcon(":icons/null.png"));
		pos2 = 2;
	}

	QString tmp;
	tmp.setNum(g2->getGiocata()->getNumero());
	QString carta = ":mazzi/" + tipoMazzo + "/" + tmp + g2->getGiocata()->getSeme() + ".png";
	labelG2->setPixmap(carta);

	controllaGiocata(2);
}

void Finestra::controllaGiocata(int i)
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

int Finestra::controllaGiocate()
{
	if (g1->getGiocata()->getSeme() == g2->getGiocata()->getSeme()) {
		if (g1->getGiocata()->getPriorita()> g2->getGiocata()->getPriorita()) {
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

void Finestra::attivaRaccolta(int g)
{
	if (g == 1) {
		th->setGiocatore(1);
		setVerde(lineEditPuntiG1);
		th->start();
	} else {
		th->setGiocatore(2);
		setVerde(lineEditPuntiG2);
		th->start();
	}
}

void Finestra::setVerde(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(147, 255, 101));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Finestra::setRosso(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 0, 0));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Finestra::setNormale(QLineEdit *line)
{
	QPalette palette;
	QBrush brush(QColor(255, 255, 255));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
	line->setPalette(palette);
}

void Finestra::settaIcone()
{
	QString tmp;

	tmp.setNum(mazzo->getMax()+1);
	if (mazzo->getMax() == 0)
		labelUltima->setPixmap(QPixmap(":/icons/null2.png"));
	else
		labelUltima->setPixmap(QPixmap(":mazzi/" + tipoMazzo + "/mazzo.png"));

	// icone giocatore
	if (g1->getCarta(0) != NULL) {
		tmp.setNum(g1->getCarta(0)->getNumero());
		QString icona10 = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getCarta(0)->getSeme() + ".png";
		pushButtonG1C1->setIcon(QIcon(icona10));
	} else {
		pushButtonG1C1->setIcon(QIcon(":icons/null.png"));
		pushButtonG1C1->setEnabled(false);
	}

	if (g1->getCarta(1) != NULL) {
		tmp.setNum(g1->getCarta(1)->getNumero());
		QString icona11 = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getCarta(1)->getSeme() + ".png";
		pushButtonG1C2->setIcon(QIcon(icona11));
	} else {
		pushButtonG1C2->setIcon(QIcon(":icons/null.png"));
		pushButtonG1C2->setEnabled(false);
	}

	if (g1->getCarta(2) != NULL) {
		tmp.setNum(g1->getCarta(2)->getNumero());
		QString icona12 = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getCarta(2)->getSeme() + ".png";
		pushButtonG1C3->setIcon(QIcon(icona12));
	} else {
		pushButtonG1C3->setIcon(QIcon(":icons/null.png"));
		pushButtonG1C3->setEnabled(false);
	}

	// icone computer
	if (g2->getCarta(0) != NULL) {
		pushButtonG2C1->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG2C1->setIcon(QIcon(":icons/null.png"));

	if (g2->getCarta(1) != NULL) {
		pushButtonG2C2->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG2C2->setIcon(QIcon(":icons/null.png"));

	if (g2->getCarta(2) != NULL) {
		pushButtonG2C3->setIcon(QIcon(":mazzi/" + tipoMazzo + "/dietro.png"));
	} else
		pushButtonG2C3->setIcon(QIcon(":icons/null.png"));

	// TODO ripristinare il blocco commentato di sopra
	// BEGIN (da sfonnare)
	/*	if (g2->getCarta(0) != NULL) {
	 tmp.setNum(g2->getCarta(0)->getNumero());
	 QString icona20 = ":icons/" + tmp + g2->getCarta(0)->getSeme() + ".png";
	 pushButtonG2C1->setIcon(QIcon(icona20));
	 } else {
	 pushButtonG2C1->setIcon(QIcon(":icons/null.png"));
	 pushButtonG2C1->setEnabled(false);
	 }

	 if (g2->getCarta(1) != NULL) {
	 tmp.setNum(g2->getCarta(1)->getNumero());
	 QString icona21 = ":icons/" + tmp + g2->getCarta(1)->getSeme() + ".png";
	 pushButtonG2C2->setIcon(QIcon(icona21));
	 } else {
	 pushButtonG2C2->setIcon(QIcon(":icons/null.png"));
	 pushButtonG2C2->setEnabled(false);
	 }

	 if (g2->getCarta(2) != NULL) {
	 tmp.setNum(g2->getCarta(2)->getNumero());
	 QString icona22 = ":icons/" + tmp + g2->getCarta(2)->getSeme() + ".png";
	 pushButtonG2C3->setIcon(QIcon(icona22));
	 } else {
	 pushButtonG2C3->setIcon(QIcon(":icons/null.png"));
	 pushButtonG2C3->setEnabled(false);
	 }*/
	// END (da sfonnare)
}

void Finestra::gestisciUltimaMano()
{
	if (mazzo->getMax() == 1) {
		if (turno == 2) {
			g2->aggiungiCarta(pos2, mazzo->daiIesima(0));
			g1->aggiungiCarta(pos1, br);

			labelBr->setPixmap(QPixmap(":icons/null.png"));
			labelUltima->setPixmap(QPixmap(":icons/null2.png"));
			mazzo->settaMax(0);
			lineEditCar->setText("0");
		} else {
			g1->aggiungiCarta(pos1, mazzo->daiIesima(0));
			g2->aggiungiCarta(pos2, br);

			labelBr->setPixmap(QPixmap(":icons/null.png"));
			labelUltima->setPixmap(QPixmap(":icons/null2.png"));
			mazzo->settaMax(0);
			lineEditCar->setText("0");
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
			QString risultati = "Computer:\t" + punti2 + tr(" points\n") + nick + ":\t" + punti1 + tr(" points");
			QString messaggio;

			int conclusione;

			if (g1->getPunti()> g2->getPunti())
				conclusione = Fine::Vittoria;
			else if (g1->getPunti() < g2->getPunti())
				conclusione = Fine::Sconfitta;
			else
				conclusione = Fine::Pareggio;

			Fine *fine = new Fine(conclusione, g1->getPunti(), g2->getPunti(), nick, "Computer");
			fine->exec();

			aggiornaDataBase(nick, "Computer", g1->getPunti(), g2->getPunti());

			tornaMenuIniziale();
			return;
		} else {
			ultima ++;
		}
	}
}

void Finestra::aggiornaDataBase(QString n1, QString n2, int p1, int p2)
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
		query.bindValue(":mod", "AI");
		query.bindValue(":nick1", n1);
		query.bindValue(":nick2", n2);
		query.bindValue(":punti1", p1);
		query.bindValue(":punti2", p2);

		bool ok = query.exec();
		if (ok == false)
			QMessageBox::critical(this, tr("Error"), tr("Error while storing data"));
	}
}

void Finestra::tornaMenuIniziale()
{
	setVisible(false);
	Avvio *a = new Avvio(db);
	a->show();
	a->setVisible(true);
	destroy();
}

void Finestra::ricordaCarteGiocate()
{
	uscite.append(g1->getGiocata());
	uscite.append(g2->getGiocata());
	if (g1->getGiocata()->getSeme() == br->getSeme()) {
		brUscite.append(g1->getGiocata());
	}
	if (g2->getGiocata()->getSeme() == br->getSeme()) {
		brUscite.append(g2->getGiocata());
	}
}

// eventi
void Finestra::cliccataCarta1()
{
	g1->giocaCarta(0);
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	pushButtonG1C1->setIcon(QIcon(":icons/null.png"));

	QString tmp;
	tmp.setNum(g1->getGiocata()->getNumero());
	QString carta = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getGiocata()->getSeme() + ".png";
	labelG1->setPixmap(carta);

	pos1 = 0;
	controllaGiocata(1);
}

void Finestra::cliccataCarta2()
{
	g1->giocaCarta(1);
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	pushButtonG1C2->setIcon(QIcon(":icons/null.png"));

	QString tmp;
	tmp.setNum(g1->getGiocata()->getNumero());
	QString carta = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getGiocata()->getSeme() + ".png";
	labelG1->setPixmap(carta);

	pos1 = 1;
	controllaGiocata(1);
}

void Finestra::cliccataCarta3()
{
	g1->giocaCarta(2);
	pushButtonG1C1->setEnabled(false);
	pushButtonG1C2->setEnabled(false);
	pushButtonG1C3->setEnabled(false);

	pushButtonG1C3->setIcon(QIcon(":icons/null.png"));

	QString tmp;
	tmp.setNum(g1->getGiocata()->getNumero());
	QString carta = ":mazzi/" + tipoMazzo + "/" + tmp + g1->getGiocata()->getSeme() + ".png";
	labelG1->setPixmap(carta);

	pos1 = 2;
	controllaGiocata(1);
}

void Finestra::riceviSegnaleRaccoltaG1()
{
	g1->aumentaPunti(g1->getGiocata()->getPunteggio() + g2->getGiocata()->getPunteggio());
	QString puntiG1;
	puntiG1.setNum(g1->getPunti());
	lineEditPuntiG1->setText(puntiG1);
	ricordaCarteGiocate();
	g1->settaNullGiocata();
	g2->settaNullGiocata();

	labelG1->setPixmap(QPixmap(""));
	labelG2->setPixmap(QPixmap(""));
	turno = 1;

	if (mazzo->getMax() <= 1) {
		gestisciUltimaMano();
	} else {
		g1->aggiungiCarta(pos1, mazzo->daiCarta());
		g2->aggiungiCarta(pos2, mazzo->daiCarta());
		settaNumCarte();
	}

	settaIcone();
	gestisciTurno(1);
	setNormale(lineEditPuntiG1);
}

void Finestra::riceviSegnaleRaccoltaG2()
{
	g2->aumentaPunti(g1->getGiocata()->getPunteggio() + g2->getGiocata()->getPunteggio());

	ricordaCarteGiocate();
	g1->settaNullGiocata();
	g2->settaNullGiocata();

	labelG1->setPixmap(QPixmap(""));
	labelG2->setPixmap(QPixmap(""));
	turno = 2;

	if (mazzo->getMax() <= 1) {
		gestisciUltimaMano();
	} else {
		g2->aggiungiCarta(pos2, mazzo->daiCarta());
		g1->aggiungiCarta(pos1, mazzo->daiCarta());
		settaNumCarte();
	}

	settaIcone();
	gestisciTurno(2);
	setNormale(lineEditPuntiG2);
}

void Finestra::esciPartita()
{
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		tornaMenuIniziale();
	}
}

void Finestra::closeEvent(QCloseEvent *event)
{
	event->ignore();
	int risp = QMessageBox::question(this, tr("Warning"), tr("Do you want to exit from match?"), QMessageBox::No | QMessageBox::Yes);
	if (risp == QMessageBox::Yes) {
		tornaMenuIniziale();
	}
}

void Finestra::about()
{
	QString messaggio = tr("QBriscola 1.1 - realized with C++ and Qt 4\n\nCiotti Roberto\nBetti Sorbelli Francesco");
	QMessageBox::information(this, tr("About"), messaggio);
}

void Finestra::mostraStatistiche()
{
	Statistiche *s = new Statistiche(db);
	s->show();
	s->setVisible(true);
}

void Finestra::avviaConfigura()
{
	conf->show();
	conf->setVisible(true);
}

void Finestra::aggiornaThread(int ms)
{
	th->setSleepMs(ms);
}

