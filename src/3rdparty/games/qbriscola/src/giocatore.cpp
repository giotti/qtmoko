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

#include "giocatore.h"

Giocatore::Giocatore(Carta *c1, Carta *c2, Carta *c3)
{
	carte = new Carta*[3];
	carte[0] = c1;
	carte[1] = c2;
	carte[2] = c3;
	
	punti = 0;
	giocata = NULL;
}

Giocatore::~Giocatore()
{
	delete[] carte;
}

Carta* Giocatore::getCarta(int i)
{
	return carte[i];
}

void Giocatore::giocaCarta(int i)
{
	giocata = carte[i];
}

void Giocatore::settaCartaNull(int i)
{
	carte[i] = NULL;
}

void Giocatore::aggiungiCarta(int pos, Carta *c)
{
	carte[pos] = c;
}

Carta* Giocatore::getGiocata()
{
	return giocata;
}

void Giocatore::settaNullGiocata()
{
	giocata = NULL;
}

int Giocatore::getPunti()
{
	return punti;
}

void Giocatore::aumentaPunti(int p)
{
	punti += p;
}
