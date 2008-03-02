/***************************************************************************
 *   Copyright (C) 2008 by Andrey Afletdinov                               *
 *   afletdinov@mail.dc.baikal.ru                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "error.h"
#include "castle.h"
#include "heroes.h"
#include "game_focus.h"

Game::Focus::Focus() : type(UNSEL), castle(NULL), heroes(NULL)
{
}

Game::Focus & Game::Focus::Get(void)
{
    static Game::Focus gfocus;
    
    return gfocus;
}

void Game::Focus::Set(const Heroes & hr)
{
    type = HEROES;
    heroes = &hr;
    castle = NULL;
}

void Game::Focus::Set(const Castle & cs)
{
    type = CASTLE;
    castle = & cs;
    heroes = NULL;
}

const Game::Focus::focus_t & Game::Focus::Type(void) const
{
    return type;
}

const Castle & Game::Focus::GetCastle(void) const
{
    if(NULL == castle) Error::Warning("Game::Focus::GetCastle: is NULL");

    return *castle;
}

const Heroes & Game::Focus::GetHeroes(void) const
{
    if(NULL == heroes) Error::Warning("Game::Focus::GetHeroes: is NULL");

    return *heroes;
}

const Point & Game::Focus::Center(void) const
{
    const Point *pt = NULL;

    switch(type)
    {
	case CASTLE:	if(castle) pt = &(*castle).GetCenter(); break;
	case BOAT:
	case HEROES:	if(heroes) pt = &(*heroes).GetCenter(); break;

	default:	Error::Warning("Game::Focus::GetCenter: is NULL"); break;
    }

    return *pt;
}
