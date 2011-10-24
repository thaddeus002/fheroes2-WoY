/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
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

#include "icn.h"
#include "objsnow.h"

bool ObjSnow::isPassable(const u16 & icn, const u8 & index, u16 direct)
{
    return direct & GetPassable(icn, index);
}

u16 ObjSnow::GetPassable(const u16 & icn, const u8 & index)
{
    switch(icn)
    {
	case ICN::OBJNSNOW:
	    // cave for centaur
	    if((1 < index && index < 4))
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // camp fire
	    if(4 == index) return 0;
	    else
	    // learn to
	    if(13 == index)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // rock
	    if(22 == index || (25 < index && index < 29) || 30 == index ||
	    32 == index || (33 < index && index < 36) || (36 < index && index < 40)) return 0;
	    else
	    // stub
	    if(40 < index && index < 43)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // trees
	    if((48 < index && index < 51) || (54 < index && index < 58) || 60 == index || 
	    (63 < index && index < 66) || 68 == index || 71 == index || 
	    74 == index || 77 == index || 80 == index) return 0;
	    else
	    // lake
	    if((80 < index && index < 85)) return 0;
	    else
	    if((84 < index && index < 90) || (89 < index && index < 93) || (93 < index && index < 96))
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    //wind mill
	    if(128 == index || 132 == index)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // watch tower
	    if(138 == index) return 0;
	    else
	    // obelisk
	    if(141 == index) return 0;
	    else
	    // sign
	    if(143 == index) return 0;
	    else
	    // alchemy tower
	    if(148 < index && index < 152)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // graveyard
	    if((158 < index && index < 161) || (207 < index && index < 211))
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // water mill
	    if(177 == index || 184 == index || 191 == index)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
	    // well
	    if(194 == index) return 0;
	    else
	    // saw mill
	    if(196 < index && index < 199) return 0;
	    else
	    if(198 < index && index < 202)
		return DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW;
	    else
		return DIRECTION_ALL;

	default: break;
    }

    return 0;
}

bool ObjSnow::isShadow(const u16 & icn, const u8 & index)
{
    const u8 shadows [] = { 21, 25, 29, 31, 33, 36, 40, 48, 54, 63, 67, 70, 73, 76, 79,
	    104, 105, 106, 107, 108, 109, 110, 111, 120, 121, 122, 123, 124, 125, 126,
	    127, 137, 140 ,142, 144, 148, 193, 203, 207 };

    return ARRAY_COUNT_END(shadows) != std::find(shadows, ARRAY_COUNT_END(shadows), index);
}
