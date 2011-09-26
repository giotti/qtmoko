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

#include "mazzo.h"
#include <QtGui>

Mazzo::Mazzo()
{
	max = 40;
	carte_mazzo = new Carta*[max];
	randomCarta = new Random(time(0));
	inizializza();
}

Mazzo::~Mazzo()
{
	delete[] carte_mazzo;
	delete randomCarta;
}

void Mazzo::inizializza()
{
	int count = 0;
	QString tipi[] = { "denari", "coppe", "spadi", "bastoni" };

	for (int tipo = 0; tipo < 4; tipo++) {
		for (int num = 0; num <10; num++) {
			int pr = 0;
			int pu = 0;

			switch (num) {
			case 0:
				pr = 10;
				pu = 11;
				break;
			case 1:
				pr = 1;
				pu = 0;
				break;
			case 2:
				pr = 9;
				pu = 10;
				break;
			case 3:
				pr = 2;
				pu = 0;
				break;
			case 4:
				pr = 3;
				pu = 0;
				break;
			case 5:
				pr = 4;
				pu = 0;
				break;
			case 6:
				pr = 5;
				pu = 0;
				break;
			case 7:
				pr = 6;
				pu = 2;
				break;
			case 8:
				pr = 7;
				pu = 3;
				break;
			case 9:
				pr = 8;
				pu = 4;
				break;
			}

			carte_mazzo[count] = new Carta(tipi[tipo], num+1, pr, pu);
			count++;
		}
	}
}

void Mazzo::settaMax(int v)
{
	max = v;
}

int Mazzo::getMax()
{
	return max;
}

Carta* Mazzo::daiIesima(int i)
{
	return carte_mazzo[i];
}

Carta* Mazzo::daiCarta()
{
	int i = static_cast<int>(randomCarta->rand()*(max-1));

	Carta *temp = carte_mazzo[i];
	carte_mazzo[i] = carte_mazzo[max-1];
	max --;
	return temp;
}
