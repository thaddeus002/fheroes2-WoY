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

#include "world.h"
#include "agg.h"
#include "engine.h"
#include "cursor.h"
#include "sprite.h"
#include "gamearea.h"
#include "maps_tiles.h"
#include "castle.h"
#include "heroes.h"

bool ReflectSprite(const u16 from);
const Sprite & SpriteHero(const Heroes & hero, const u8 index, const bool reflect, const bool rotate = false);
const Sprite & SpriteFlag(const Heroes & hero, const u8 index, const bool reflect, const bool rotate = false);
const Sprite & SpriteShad(const Heroes & hero, const u8 index);
bool isNeedStayFrontObject(const Heroes & hero, const Maps::Tiles & next);

bool ReflectSprite(const u16 from)
{
    switch(from)
    {
        case Direction::BOTTOM_LEFT:
        case Direction::LEFT:
        case Direction::TOP_LEFT:		return true;

        default: break;
    }

    return false;
}

const Sprite & SpriteHero(const Heroes & hero, const u8 index, const bool reflect, const bool rotate)
{
    ICN::icn_t icn_hero = ICN::UNKNOWN;
    u16 index_sprite = 0;

    if(hero.isShipMaster()) icn_hero = ICN::BOAT32;
    else
    switch(hero.GetRace())
    {
        case Race::KNGT: icn_hero = ICN::KNGT32; break;
        case Race::BARB: icn_hero = ICN::BARB32; break;
        case Race::SORC: icn_hero = ICN::SORC32; break;
        case Race::WRLK: icn_hero = ICN::WRLK32; break;
        case Race::WZRD: icn_hero = ICN::WZRD32; break;
        case Race::NECR: icn_hero = ICN::NECR32; break;

        default: Error::Warning("Heroes::SpriteHero: unknown race"); break;
    }

    if(rotate)				index_sprite = 45;
    else
    switch(hero.GetDirection())
    {
        case Direction::TOP:            index_sprite =  0; break;
        case Direction::TOP_RIGHT:      index_sprite =  9; break;
        case Direction::RIGHT:          index_sprite = 18; break;
        case Direction::BOTTOM_RIGHT:   index_sprite = 27; break;
        case Direction::BOTTOM:         index_sprite = 36; break;
        case Direction::BOTTOM_LEFT:    index_sprite = 27; break;
        case Direction::LEFT:           index_sprite = 18; break;
        case Direction::TOP_LEFT:       index_sprite =  9; break;

        default: Error::Warning("Heroes::SpriteHero: unknown direction"); break;
    }

    return AGG::GetICN(icn_hero, index_sprite + (index % 9), reflect);
}

const Sprite & SpriteFlag(const Heroes & hero, const u8 index, const bool reflect, const bool rotate)
{
    ICN::icn_t icn_flag = ICN::UNKNOWN;
    u16 index_sprite = 0;

    switch(hero.GetColor())
    {
        case Color::BLUE:       icn_flag = hero.isShipMaster() ? ICN::B_BFLG32 : ICN::B_FLAG32; break;
        case Color::GREEN:      icn_flag = hero.isShipMaster() ? ICN::G_BFLG32 : ICN::G_FLAG32; break;
        case Color::RED:        icn_flag = hero.isShipMaster() ? ICN::R_BFLG32 : ICN::R_FLAG32; break;
        case Color::YELLOW:     icn_flag = hero.isShipMaster() ? ICN::Y_BFLG32 : ICN::Y_FLAG32; break;
        case Color::ORANGE:     icn_flag = hero.isShipMaster() ? ICN::O_BFLG32 : ICN::O_FLAG32; break;
        case Color::PURPLE:     icn_flag = hero.isShipMaster() ? ICN::P_BFLG32 : ICN::P_FLAG32; break;

        default: Error::Warning("Heroes::SpriteFlag: unknown color hero"); break;
    }

    if(rotate)				index_sprite = 45;
    else
    switch(hero.GetDirection())
    {
        case Direction::TOP:            index_sprite =  0; break;
        case Direction::TOP_RIGHT:      index_sprite =  9; break;
        case Direction::RIGHT:          index_sprite = 18; break;
        case Direction::BOTTOM_RIGHT:   index_sprite = 27; break;
        case Direction::BOTTOM:         index_sprite = 36; break;
        case Direction::BOTTOM_LEFT:    index_sprite = 27; break;
        case Direction::LEFT:           index_sprite = 18; break;
        case Direction::TOP_LEFT:       index_sprite =  9; break;

        default: Error::Warning("Heroes::SpriteFlag: unknown direction"); break;
    }

    return AGG::GetICN(icn_flag, index_sprite + (index % 9), reflect);
}

const Sprite & SpriteShad(const Heroes & hero, const u8 index)
{
    const ICN::icn_t icn_shad = hero.isShipMaster() ? ICN::BOATSHAD : ICN::SHADOW32;
    u16 index_sprite = 0;

    //if(rotate)				index_sprite = 45;
    //else
    switch(hero.GetDirection())
    {
        case Direction::TOP:            index_sprite =  0; break;
        case Direction::TOP_RIGHT:      index_sprite =  9; break;
        case Direction::RIGHT:          index_sprite = 18; break;
        case Direction::BOTTOM_RIGHT:   index_sprite = 27; break;
        case Direction::BOTTOM:         index_sprite = 36; break;
        case Direction::BOTTOM_LEFT:    index_sprite = 45; break;
        case Direction::LEFT:           index_sprite = 54; break;
        case Direction::TOP_LEFT:       index_sprite = 63; break;

        default: Error::Warning("Heroes::SpriteShadow: unknown direction"); break;
    }

    return AGG::GetICN(icn_shad, index_sprite + (index % 9));
}

bool isNeedStayFrontObject(const Heroes & hero, const Maps::Tiles & next)
{
    if(hero.isShipMaster())
        switch(next.GetObject())
        {
            case MP2::OBJ_TREASURECHEST:
            case MP2::OBJ_SHIPWRECKSURVIROR:
            case MP2::OBJ_FLOTSAM:
            case MP2::OBJ_BOTTLE:
            case MP2::OBJ_COAST:
            case MP2::OBJ_HEROES:
            return true;

            default: break;
        }
    else
        switch(next.GetObject())
        {
            case MP2::OBJ_TREASURECHEST:
            case MP2::OBJ_ANCIENTLAMP:
            case MP2::OBJ_CAMPFIRE:
            case MP2::OBJ_MONSTER:
            case MP2::OBJ_RESOURCE:
            case MP2::OBJ_ARTIFACT:
            case MP2::OBJ_HEROES:
            case MP2::OBJ_BOAT:
	    case MP2::OBJ_BARRIER:
            return true;

            case MP2::OBJ_CASTLE:
	    {
		const Castle *castle = world.GetCastle(next.GetIndex());

		if(castle && hero.GetColor() != castle->GetColor()) return true;
	    }
	    break;

            default: break;
        }

    return false;
}

void Heroes::Redraw(bool with_shadow) const
{
    Display & display = Display::Get();

    const Rect & gamearea = GameArea::Get().GetRect();

    if(!(gamearea & mp)) return;

    //const u16 center = Maps::GetIndexFromAbsPoint(GetCenter());

    bool reflect = ReflectSprite(direction);

    s16 dx = BORDERWIDTH + TILEWIDTH * (mp.x - gamearea.x);
    s16 dy = BORDERWIDTH + TILEWIDTH * (mp.y - gamearea.y);

    const Sprite & sprite1 = SpriteHero(*this, sprite_index, reflect);
    const Sprite & sprite2 = SpriteFlag(*this, sprite_index, reflect);
    const Sprite & sprite3 = SpriteShad(*this, sprite_index);

    Point dst_pt1(dx + (reflect ? TILEWIDTH - sprite1.x() - sprite1.w() : sprite1.x()), dy + sprite1.y() + TILEWIDTH);
    Point dst_pt2(dx + (reflect ? TILEWIDTH - sprite2.x() - sprite2.w() : sprite2.x()), dy + sprite2.y() + TILEWIDTH);
    Point dst_pt3(dx + sprite3.x(), dy + sprite3.y() + TILEWIDTH);

    // apply offset
    if(sprite_index < 45)
    {
	s16 ox = 0;
	s16 oy = 0;
	const u8 frame = (sprite_index % 9);

	switch(direction)
	{
    	    case Direction::TOP:            oy = -4 * frame; break;
    	    case Direction::TOP_RIGHT:      ox = 4 * frame; oy = -4 * frame; break;
    	    case Direction::TOP_LEFT:       ox = -4 * frame; oy = -4 * frame; break;
    	    case Direction::BOTTOM_RIGHT:   ox = 4 * frame; oy = 4 * frame; break;
    	    case Direction::BOTTOM:         oy = 4 * frame; break;
    	    case Direction::BOTTOM_LEFT:    ox = -4 * frame; oy = 4 * frame; break;
    	    case Direction::RIGHT:          ox = 4 * frame; break;
    	    case Direction::LEFT:           ox = -4 * frame; break;
    	    default: break;
	}

	dst_pt1.x += ox;
	dst_pt1.y += oy;
	dst_pt2.x += ox;
	dst_pt2.y += oy;
	dst_pt3.x += ox;
	dst_pt3.y += oy;
    }

    Rect src_rt1;
    Rect src_rt2;
    Rect src_rt3;

    GameArea::SrcRectFixed(src_rt1, dst_pt1, sprite1.w(), sprite1.h());
    GameArea::SrcRectFixed(src_rt2, dst_pt2, sprite2.w(), sprite2.h());
    GameArea::SrcRectFixed(src_rt3, dst_pt3, sprite3.w(), sprite3.h());

    // redraw sprites for shadow
    if(with_shadow) display.Blit(sprite3, src_rt3, dst_pt3);

    // redraw sprites hero and flag
    display.Blit(sprite1, src_rt1, dst_pt1);
    display.Blit(sprite2, src_rt2, dst_pt2);
    
    RedrawDependencesTiles();
}

void Heroes::RedrawDependencesTiles(void) const
{
    const u16 center = Maps::GetIndexFromAbsPoint(mp);

    world.GetTiles(center).RedrawTop();

    if(Maps::isValidDirection(center, Direction::BOTTOM))
    {
	Maps::Tiles & tile_bottom = world.GetTiles(Maps::GetDirectionIndex(center, Direction::BOTTOM));
	const Maps::TilesAddon * skip = NULL;
	std::vector<const Maps::TilesAddon *> v;
	v.reserve(3);

	// skip always: ICN::OBJNTWBA
	skip = tile_bottom.FindAddonICN1(ICN::OBJNTWBA);
	if(skip) v.push_back(skip);
	// skip only sprite: ICN::ROAD
	skip = tile_bottom.FindAddonICN1(ICN::ROAD);
	if(skip) v.push_back(skip);
	// skip only sprite: ICN::STREAM
	skip = tile_bottom.FindAddonICN1(ICN::STREAM);
	if(skip) v.push_back(skip);
	// add other sprite if incorrect draw hero

	if(2 > v.size())
	{
	    tile_bottom.RedrawBottom(v.size() ? v.front() : NULL);
	    tile_bottom.RedrawTop();
	}
    }

    // skip if rotate
    if(45 > GetSpriteIndex())
    {
	if(Direction::BOTTOM != direction &&
	    Direction::TOP != direction &&
	    Maps::isValidDirection(center, direction) &&
	    Maps::isValidDirection(Maps::GetDirectionIndex(center, direction), Direction::BOTTOM))
	{
	    Maps::Tiles & tile_dir_bottom = world.GetTiles(Maps::GetDirectionIndex(Maps::GetDirectionIndex(center, direction), Direction::BOTTOM));
	    const Maps::TilesAddon * skip = NULL;
	    std::vector<const Maps::TilesAddon *> v;
	    v.reserve(3);

	    // skip always: ICN::OBJNTWBA
    	    skip = tile_dir_bottom.FindAddonICN1(ICN::OBJNTWBA);
	    if(skip) v.push_back(skip);
	    // skip only sprite: ICN::ROAD
    	    skip = tile_dir_bottom.FindAddonICN1(ICN::ROAD);
	    if(skip) v.push_back(skip);
	    // skip only sprite: ICN::STREAM
    	    skip = tile_dir_bottom.FindAddonICN1(ICN::STREAM);
	    if(skip) v.push_back(skip);
	    // add other sprite if incorrect draw hero

	    if(2 > v.size())
    	    {
    		tile_dir_bottom.RedrawBottom(v.size() ? v.front() : NULL);
    		tile_dir_bottom.RedrawTop();
	    }
	}
    }
    
    if(Maps::isValidDirection(center, direction)) world.GetTiles(Maps::GetDirectionIndex(center, direction)).RedrawTop();
}

bool Heroes::MoveStep(void)
{
    const u16 index_from = Maps::GetIndexFromAbsPoint(mp);
    const u16 index_to = path.GetFrontIndex();
    const u16 index_dst = path.GetDestinationIndex();
    bool didMove = false;

    if(0 == sprite_index % 9)
    {
	if(index_to == index_dst && isNeedStayFrontObject(*this, world.GetTiles(index_to)))
	{
	    ApplyPenaltyMovement();
	    path.Reset();
	    Action(index_to);
	    SetMove(false);
	    return true;
	}
	else
	{
	    M82::m82_t wav = M82::UNKNOWN;

	    const u8 speed = 3;

	    // play sound
	    switch(world.GetTiles(mp).GetGround())
	    {
    		case Maps::Ground::WATER:       wav = (1 == speed ? M82::WSND00 : (2 == speed ? M82::WSND10 : M82::WSND20)); break;
    		case Maps::Ground::GRASS:       wav = (1 == speed ? M82::WSND01 : (2 == speed ? M82::WSND11 : M82::WSND21)); break;
    		case Maps::Ground::WASTELAND:   wav = (1 == speed ? M82::WSND02 : (2 == speed ? M82::WSND12 : M82::WSND22)); break;
    		case Maps::Ground::SWAMP:
    		case Maps::Ground::BEACH:       wav = (1 == speed ? M82::WSND03 : (2 == speed ? M82::WSND13 : M82::WSND23)); break;
    		case Maps::Ground::LAVA:        wav = (1 == speed ? M82::WSND04 : (2 == speed ? M82::WSND14 : M82::WSND24)); break;
    		case Maps::Ground::DESERT:
    		case Maps::Ground::SNOW:        wav = (1 == speed ? M82::WSND05 : (2 == speed ? M82::WSND15 : M82::WSND25)); break;
    		case Maps::Ground::DIRT:        wav = (1 == speed ? M82::WSND06 : (2 == speed ? M82::WSND16 : M82::WSND26)); break;

    		default: break;
	    }

	    if(wav != M82::UNKNOWN) AGG::PlaySound(wav);
	}
    }
    else
    if(8 == sprite_index % 9)
    {
	Maps::Tiles & tiles_from = world.GetTiles(index_from);
	Maps::Tiles & tiles_to = world.GetTiles(index_to);

	if(MP2::OBJ_HEROES != save_maps_general) tiles_from.SetObject(save_maps_general);

	SetCenter(index_to);
	Scoute();
	ApplyPenaltyMovement();

	if(index_to == index_dst)
	{
	    path.Reset();
	    Action(index_to);
	    SetMove(false);
            didMove = true;
	}

	save_maps_general = tiles_to.GetObject();
	tiles_to.SetObject(MP2::OBJ_HEROES);

	path.PopFront();

	sprite_index -= 8;

	const u16 dst_index2 = Maps::ScanAroundObject(index_to, MP2::OBJ_MONSTER);
	if(MAXU16 != dst_index2)
        {
	    GameArea::Get().Redraw();
	    Display::Get().Flip();
	    Action(dst_index2);
	    SetMove(false);
            didMove = true;
	}

	return didMove;
    }

    ++sprite_index;

    return didMove;
}

void Heroes::AngleStep(const Direction::vector_t to_direct)
{
    //bool check = false;
    bool clockwise = ShortDistanceClockWise(direction, to_direct);

    // start index
    if(45 > sprite_index && 0 == sprite_index % 9)
	switch(direction)
	{
    	    case Direction::TOP:		sprite_index = 45; break;
    	    case Direction::TOP_RIGHT:		sprite_index = clockwise ? 47 : 46; break;
    	    case Direction::TOP_LEFT:		sprite_index = clockwise ? 46 : 47; break;
    	    case Direction::RIGHT:		sprite_index = clockwise ? 49 : 48; break;
    	    case Direction::LEFT:		sprite_index = clockwise ? 48 : 49; break;
    	    case Direction::BOTTOM_RIGHT:	sprite_index = clockwise ? 51 : 50; break;
    	    case Direction::BOTTOM_LEFT:	sprite_index = clockwise ? 50 : 51; break;
    	    case Direction::BOTTOM:		sprite_index = clockwise ? 52 : 53; break;

	    default: break;
	}
    // animation process
    else
    {
	switch(direction)
	{
    	    case Direction::TOP_RIGHT:
    	    case Direction::RIGHT:
    	    case Direction::BOTTOM_RIGHT:
		clockwise ? ++sprite_index : --sprite_index;
		break;

    	    case Direction::TOP:
		++sprite_index;
		break;

    	    case Direction::TOP_LEFT:
    	    case Direction::LEFT:
    	    case Direction::BOTTOM_LEFT:
		clockwise ? --sprite_index : ++sprite_index;
		break;

    	    case Direction::BOTTOM:
		--sprite_index;
		break;

	    default: break;
	}

	bool end = false;

	Direction::vector_t next = Direction::UNKNOWN;

	switch(direction)
	{
    	    case Direction::TOP:		next = clockwise ? Direction::TOP_RIGHT : Direction::TOP_LEFT; break;
    	    case Direction::TOP_RIGHT:		next = clockwise ? Direction::RIGHT : Direction::TOP; break;
    	    case Direction::TOP_LEFT:		next = clockwise ? Direction::TOP : Direction::LEFT; break;
    	    case Direction::RIGHT:		next = clockwise ? Direction::BOTTOM_RIGHT : Direction::TOP_RIGHT; break;
    	    case Direction::LEFT:		next = clockwise ? Direction::TOP_LEFT : Direction::BOTTOM_LEFT; break;
    	    case Direction::BOTTOM_RIGHT:	next = clockwise ? Direction::BOTTOM : Direction::RIGHT; break;
    	    case Direction::BOTTOM_LEFT:	next = clockwise ? Direction::LEFT : Direction::BOTTOM; break;
    	    case Direction::BOTTOM:		next = clockwise ? Direction::BOTTOM_LEFT : Direction::BOTTOM_RIGHT; break;

	    default: break;
	}

	switch(next)
	{
    	    case Direction::TOP:		end = (sprite_index == 44); break;
    	    case Direction::TOP_RIGHT:		end = (sprite_index == (clockwise ? 47 : 46)); break;
    	    case Direction::TOP_LEFT:		end = (sprite_index == (clockwise ? 46 : 47)); break;
    	    case Direction::RIGHT:		end = (sprite_index == (clockwise ? 49 : 48)); break;
    	    case Direction::LEFT:		end = (sprite_index == (clockwise ? 48 : 49)); break;
    	    case Direction::BOTTOM_RIGHT:	end = (sprite_index == (clockwise ? 51 : 50)); break;
    	    case Direction::BOTTOM_LEFT:	end = (sprite_index == (clockwise ? 50 : 51)); break;
    	    case Direction::BOTTOM:		end = (sprite_index == 53); break;

	    default: break;
	}

	if(end)
	{
	    switch(next)
	    {
    		case Direction::TOP:            sprite_index = 0; break;
    		case Direction::BOTTOM:         sprite_index = 36; break;
    		case Direction::TOP_RIGHT:
    		case Direction::TOP_LEFT:       sprite_index = 9; break;
    		case Direction::BOTTOM_RIGHT:
    		case Direction::BOTTOM_LEFT:    sprite_index = 27; break;
    		case Direction::RIGHT:
    		case Direction::LEFT:           sprite_index = 18; break;

		default: break;
	    }

	    direction = next;
	}
    }
}

void Heroes::FadeOut(void) const
{
    const Rect & gamearea = GameArea::Get().GetRect();

    if(!(gamearea & mp)) return;

    //const u16 center = Maps::GetIndexFromAbsPoint(GetCenter());

    bool reflect = ReflectSprite(direction);

    s16 dx = BORDERWIDTH + TILEWIDTH * (mp.x - gamearea.x);
    s16 dy = BORDERWIDTH + TILEWIDTH * (mp.y - gamearea.y);

    const Sprite & sprite1 = SpriteHero(*this, sprite_index, reflect);

    Surface sf(sprite1.w(), sprite1.h());
    sf.SetColorKey();
    sf.Blit(sprite1);

    Point dst_pt1(dx + (reflect ? TILEWIDTH - sprite1.x() - sprite1.w() : sprite1.x()), dy + sprite1.y() + TILEWIDTH);
    Rect src_rt1;

    GameArea::SrcRectFixed(src_rt1, dst_pt1, sprite1.w(), sprite1.h());

    LocalEvent & le = LocalEvent::GetLocalEvent();
    u32 ticket = 0;
    u8 alpha = 250;

    while(le.HandleEvents() && alpha > 0)
    {
        if(!(ticket % ANIMATION_HIGH))
        {
            Cursor::Get().Hide();

	    for(s16 y = mp.y - 1; y <= mp.y + 1; ++y)
		for(s16 x = mp.x - 1; x <= mp.x + 1; ++x)
    		    if(Maps::isValidAbsPoint(x, y))
	    {
        	const Maps::Tiles & tile = world.GetTiles(Maps::GetIndexFromAbsPoint(x, y));

        	tile.RedrawTile();
        	tile.RedrawBottom();
    	    }

    	    sf.SetAlpha(alpha);
    	    Display::Get().Blit(sf, src_rt1, dst_pt1);

	    for(s16 y = mp.y - 1; y <= mp.y + 1; ++y)
		for(s16 x = mp.x - 1; x <= mp.x + 1; ++x)
    		    if(Maps::isValidAbsPoint(x, y))
	    {
        	const Maps::Tiles & tile = world.GetTiles(Maps::GetIndexFromAbsPoint(x, y));

        	tile.RedrawTop();
    	    }

	    Cursor::Get().Show();
	    Display::Get().Flip();
            alpha -= 10;
        }

        ++ticket;
    }
}

void Heroes::FadeIn(void) const
{
    const Rect & gamearea = GameArea::Get().GetRect();

    if(!(gamearea & mp)) return;

    //const u16 center = Maps::GetIndexFromAbsPoint(GetCenter());

    bool reflect = ReflectSprite(direction);

    s16 dx = BORDERWIDTH + TILEWIDTH * (mp.x - gamearea.x);
    s16 dy = BORDERWIDTH + TILEWIDTH * (mp.y - gamearea.y);

    const Sprite & sprite1 = SpriteHero(*this, sprite_index, reflect);

    Surface sf(sprite1.w(), sprite1.h());
    sf.SetColorKey();
    sf.Blit(sprite1);

    Point dst_pt1(dx + (reflect ? TILEWIDTH - sprite1.x() - sprite1.w() : sprite1.x()), dy + sprite1.y() + TILEWIDTH);
    Rect src_rt1;

    GameArea::SrcRectFixed(src_rt1, dst_pt1, sprite1.w(), sprite1.h());

    LocalEvent & le = LocalEvent::GetLocalEvent();
    u32 ticket = 0;
    u8 alpha = 0;

    while(le.HandleEvents() && alpha < 250)
    {
        if(!(ticket % ANIMATION_HIGH))
        {
            Cursor::Get().Hide();

	    for(s16 y = mp.y - 1; y <= mp.y + 1; ++y)
		for(s16 x = mp.x - 1; x <= mp.x + 1; ++x)
    		    if(Maps::isValidAbsPoint(x, y))
	    {
        	const Maps::Tiles & tile = world.GetTiles(Maps::GetIndexFromAbsPoint(x, y));

        	tile.RedrawTile();
        	tile.RedrawBottom();
    	    }

    	    sf.SetAlpha(alpha);
    	    Display::Get().Blit(sf, src_rt1, dst_pt1);

	    for(s16 y = mp.y - 1; y <= mp.y + 1; ++y)
		for(s16 x = mp.x - 1; x <= mp.x + 1; ++x)
    		    if(Maps::isValidAbsPoint(x, y))
	    {
        	const Maps::Tiles & tile = world.GetTiles(Maps::GetIndexFromAbsPoint(x, y));

        	tile.RedrawTop();
    	    }

	    Cursor::Get().Show();
	    Display::Get().Flip();
            alpha += 10;
        }

        ++ticket;
    }
}
