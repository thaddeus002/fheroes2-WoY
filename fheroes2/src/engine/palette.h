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

#ifndef H2PALETTE_H
#define H2PALETTE_H

#include <vector>
#include "types.h"

struct SDL_Palette;

class Palette
{
public:
    Palette(); 
    Palette(const std::vector<char> & v);
    Palette(const Palette & p);

    ~Palette();

    Palette & operator= (const Palette & p);

    void Load(const std::vector<char> & v);

    u32 Size(void) const;
    u32 Color(const u16 index) const;

    const SDL_Color * SDLColors(void) const;


private:
    std::vector<u32> pal;
    SDL_Palette *sdlpal;
};

#endif
