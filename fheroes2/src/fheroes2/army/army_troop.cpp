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

#include <algorithm>
#include <functional>
#include "army.h"
#include "army_troop.h"

Army::Troop::Troop(monster_t m, u16 c) : Monster(m), count(c), army(NULL)
{
}

bool Army::Troop::HasMonster(monster_t m) const
{
    return id == m;
}

void Army::Troop::Set(const Monster & m, u16 c)
{
    id = m();
    count = c;
}

void Army::Troop::Set(monster_t m, u16 c)
{
    id = m;
    count = c;
}

void Army::Troop::SetMonster(const Monster & m)
{
    id = m();
}

void Army::Troop::SetMonster(monster_t m)
{
    id = m;
}

void Army::Troop::SetCount(u16 c)
{
    count = c;
}

void Army::Troop::BattleNewTurn(void)
{
    ResetModes(MOVED);
}

void Army::Troop::Reset(void)
{
    id = Monster::UNKNOWN;
    count = 0;
}

const Skill::Primary* Army::Troop::MasterSkill(void) const
{
    return army ? army->commander : NULL;
}

const Army::army_t* Army::Troop::GetArmy(void) const
{
    return army;
}

const std::string & Army::Troop::GetName(void) const
{
    return 1 < count ? Monster::GetMultiName() : Monster::GetName();
}

u16 Army::Troop::Count(void) const
{
    return count;
}

u8 Army::Troop::GetAttack(void) const
{
    return army && army->commander ? army->commander->GetAttack() + Monster::GetAttack() : Monster::GetAttack();
}

u8 Army::Troop::GetDefense(void) const
{
    return army && army->commander ? army->commander->GetDefense() + Monster::GetDefense() : Monster::GetDefense();
}

Color::color_t Army::Troop::GetColor(void) const
{
    return army && army->commander ? army->commander->GetColor() : Color::GRAY;
}

u32 Army::Troop::GetHitPoints(void) const
{
    return Monster::GetHitPoints() * count;
}

u16 Army::Troop::GetDamageMin(void) const
{
    return Monster::GetDamageMin() * count;
}

u16 Army::Troop::GetDamageMax(void) const
{
    return Monster::GetDamageMax() * count;
}

u8 Army::Troop::GetSpeed(void) const
{
    return Modes(MOVED) ? 0 : Monster::GetSpeed();
}

bool Army::Troop::isValid(void) const
{
    return Monster::UNKNOWN < id && count;
}

bool Army::isValidTroop(const Troop & troop)
{
    return troop.isValid();
}

bool Army::StrongestTroop(const Troop & t1, const Troop & t2)
{
    return t1.isValid() && t2.isValid() && (t1.GetDamageMin() > t2.GetDamageMin());
}

bool Army::WeakestTroop(const Troop & t1, const Troop & t2)
{
    return t1.isValid() && t2.isValid() && (t1.GetDamageMax() < t2.GetDamageMax());
}

bool Army::SlowestTroop(const Troop & t1, const Troop & t2)
  {
    return t1.isValid() && t2.isValid() && (t1.GetSpeed() < t2.GetSpeed());
}

bool Army::FastestTroop(const Troop & t1, const Troop & t2)
{
    return t1.isValid() && t2.isValid() && (t1.GetSpeed() > t2.GetSpeed());
}

void Army::SwapTroops(Troop & t1, Troop & t2)
{
    std::swap(t1, t2);
}
