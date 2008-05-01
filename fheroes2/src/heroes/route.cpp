/***************************************************************************
 *   Copyright (C) 2006 by Andrey Afletdinov                               *
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

#include "agg.h"
#include "world.h"
#include "heroes.h"
#include "algorithm.h"
#include "direction.h"
#include "maps.h"
#include "game.h"
#include "config.h"
#include "error.h"
#include "route.h"

/* construct */
Route::Path::Path(const Heroes & h)
    : hero(h), dst(Maps::GetIndexFromAbsPoint(h.GetCenter()))
{
}

/* return length path */
u16 Route::Path::Calculate(u16 dst_index)
{
    clear();

    Algorithm::PathFinding(Maps::GetIndexFromAbsPoint(hero.GetCenter()), dst_index, hero.GetLevelSkill(Skill::PATHFINDING), *this);

    dst = dst_index;

    return size();
}

void Route::Path::Show(void) const
{
    if(empty()) return;

    // path only human players
    if(hero.GetColor() != H2Config::MyColor()) return;

    u16 from = Maps::GetIndexFromAbsPoint(hero.GetCenter());

    std::list<Step>::const_iterator it1 = begin();
    std::list<Step>::const_iterator it2 = end();
    std::list<Step>::const_iterator it3 = it1;

    u16 move_point = hero.GetMovePoints();

    bool red_sprite = move_point ? false : true;

    for(; it1 != it2; ++it1)
    {
	Maps::Tiles & tile = world.GetTiles((*it1).to_index);

	u16 index = 0;

	if(!red_sprite)
	{
	    if(move_point >= (*it1).penalty)	move_point -= (*it1).penalty;
	    else	red_sprite = true;
	}

	if(++it3 != it2) index = GetIndexSprite(Direction::Get(from, (*it1).to_index), Direction::Get((*it1).to_index, (*it3).to_index));

	tile.AddPathSprite(& AGG::GetICN(red_sprite ? ICN::ROUTERED : ICN::ROUTE, index));
	tile.RedrawAll();

	from = (*it1).to_index;
    }

    if(H2Config::Debug()) Dump();
}

void Route::Path::Dump(void) const
{
    if(H2Config::Debug()) Error::Verbose("Route::Path: move point: ", hero.GetMovePoints());
    if(H2Config::Debug()) Error::Verbose("Route::Path: start index: ", Maps::GetIndexFromAbsPoint(hero.GetCenter()));

    std::list<Step>::const_iterator it1 = begin();
    std::list<Step>::const_iterator it2 = end();

    u16 from = Maps::GetIndexFromAbsPoint(hero.GetCenter());

    // dump route
    for(; it1 != it2; ++it1)
    {
	if(H2Config::Debug()) Error::Verbose("Route::Path: " + Direction::String(Direction::Get(from, (*it1).to_index)) + ", penalty: ", (*it1).penalty);

	from = (*it1).to_index;
    }

    if(H2Config::Debug()) Error::Verbose("Route::Path: end index: ", dst);
    if(H2Config::Debug()) Error::Verbose("Route::Path: size: ", size());
}

void Route::Path::Hide(void) const
{
    if(empty()) return;

    // path only human players
    if(hero.GetColor() != H2Config::MyColor()) return;

    // redraw tiles
    std::list<Step>::const_iterator it1 = begin();
    std::list<Step>::const_iterator it2 = end();

    for(; it1 != it2; ++it1)
    {
	Maps::Tiles & tile = world.GetTiles((*it1).to_index);

	tile.DelPathSprite();
	tile.RedrawAll();
    }
}

void Route::Path::Reset(void)
{
    if(empty()) return;

    dst = Maps::GetIndexFromAbsPoint(hero.GetCenter());

    clear();
}

u16 Route::Path::GetIndexSprite(const Direction::vector_t & from, const Direction::vector_t & to)
{
    // start index 1, 25, 49, 73, 97, 121 (size arrow path)
    u16 index = 1;

    switch(from)
    {
	case Direction::TOP:
	    switch(to)
	    {
		case Direction::TOP:		index +=  8; break;
		case Direction::TOP_RIGHT:	index += 17; break;
		case Direction::RIGHT:		index += 18; break;
		case Direction::LEFT:		index +=  6; break;
		case Direction::TOP_LEFT:	index +=  7; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::TOP_RIGHT:
	    switch(to)
	    {
		case Direction::TOP:		index +=  0; break;
		case Direction::TOP_RIGHT:	index +=  9; break;
		case Direction::RIGHT:		index += 18; break;
		case Direction::BOTTOM_RIGHT:	index += 19; break;
		case Direction::TOP_LEFT:	index +=  7; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::RIGHT:
	    switch(to)
	    {
		case Direction::TOP:		index +=  0; break;
		case Direction::BOTTOM:		index += 20; break;
		case Direction::BOTTOM_RIGHT:	index += 19; break;
		case Direction::RIGHT:		index += 10; break;
		case Direction::TOP_RIGHT:	index +=  1; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::BOTTOM_RIGHT:
	    switch(to)
	    {
		case Direction::TOP_RIGHT:	index +=  1; break;
		case Direction::RIGHT:		index +=  2; break;
		case Direction::BOTTOM_RIGHT:	index += 11; break;
		case Direction::BOTTOM:		index += 20; break;
		case Direction::BOTTOM_LEFT:	index += 21; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::BOTTOM:
	    switch(to)
	    {
		case Direction::RIGHT:		index +=  2; break;
		case Direction::BOTTOM_RIGHT:	index +=  3; break;
		case Direction::BOTTOM:		index += 12; break;
		case Direction::BOTTOM_LEFT:	index += 21; break;
		case Direction::LEFT:		index += 22; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::BOTTOM_LEFT:
	    switch(to)
	    {
		case Direction::BOTTOM_RIGHT:	index +=  3; break;
		case Direction::BOTTOM:		index +=  4; break;
		case Direction::BOTTOM_LEFT:	index += 13; break;
		case Direction::LEFT:		index += 22; break;
		case Direction::TOP_LEFT:	index += 23; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::LEFT:
	    switch(to)
	    {
		case Direction::TOP:		index += 16; break;
		case Direction::BOTTOM:		index +=  4; break;
		case Direction::BOTTOM_LEFT:	index +=  5; break;
		case Direction::LEFT:		index += 14; break;
		case Direction::TOP_LEFT:	index += 23; break;
		default: 			index  =  0; break;
	    }
	    break;

	case Direction::TOP_LEFT:
	    switch(to)
	    {
		case Direction::TOP:		index += 16; break;
		case Direction::TOP_RIGHT:	index += 17; break;
		case Direction::BOTTOM_LEFT:	index +=  5; break;
		case Direction::LEFT:		index +=  6; break;
		case Direction::TOP_LEFT:	index += 15; break;
		default: 			index  =  0; break;
	    }
	    break;

	default: 		   	        index  =  0; break;
    }

    return index;
}

/* get next to last path element */
u16 Route::Path::NextToLast(void) const
{
    if(2 > size())
    {
	Error::Warning("Route::NextToLast: path size is short, return 0");

	return 0;
    }

    std::list<Step>::const_reverse_iterator it = rbegin();

    return (*(++it)).to_index;
}
