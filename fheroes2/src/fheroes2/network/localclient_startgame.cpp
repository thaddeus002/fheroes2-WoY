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

#ifdef WITH_NET

#include <vector>
#include <algorithm>

#include "agg.h"
#include "cursor.h"
#include "settings.h"
#include "dialog.h"
#include "world.h"
#include "castle.h"
#include "heroes.h"
#include "kingdom.h"
#include "army.h"
#include "game_interface.h"
#include "game_focus.h"
#include "localclient.h"

namespace Game
{
    menu_t HumanTurn(void);
};

bool FH2LocalClient::StartGame(void)
{
    Game::SetFixVideoMode();

    // cursor
    Cursor & cursor = Cursor::Get();
    Settings & conf = Settings::Get();
    Display & display = Display::Get();

    GameOver::Result::Get().Reset();

    cursor.Hide();

    AGG::FreeObject(ICN::HEROES);
    AGG::FreeObject(ICN::BTNSHNGL);
    AGG::FreeObject(ICN::SHNGANIM);
    AGG::FreeObject(ICN::BTNNEWGM);
    AGG::FreeObject(ICN::REDBACK);
    AGG::FreeObject(ICN::NGEXTRA);
    AGG::FreeObject(ICN::NGHSBKG);
    AGG::FreeObject(ICN::REQSBKG);
    AGG::FreeObject(ICN::REQUEST);
    AGG::FreeObject(ICN::REQUESTS);
    AGG::FreeObject(ICN::ESCROLL);
    AGG::FreeObject(ICN::HSBKG);
    AGG::FreeObject(ICN::HISCORE);

    if(Settings::Get().LowMemory())
    {
        AGG::ICNRegistryEnable(false);
        AGG::ICNRegistryFreeObjects();
    }

    // preload sounds
    Mixer::Reset();

    // draw interface
    Interface::Basic & I = Interface::Basic::Get();

    Interface::GameArea & areaMaps = I.gameArea;
    areaMaps.Build();

    Game::Focus & global_focus = Game::Focus::Get();
    global_focus.Reset();

    Interface::Radar & radar = I.radar;
    Interface::HeroesIcons & heroesBar = I.iconsPanel.GetHeroesBar();
    Interface::CastleIcons & castleBar = I.iconsPanel.GetCastleBar();
    Interface::StatusWindow& statusWin = I.statusWindow;
    heroesBar.Reset();
    castleBar.Reset();

    radar.Build();

    I.Redraw(REDRAW_ICONS | REDRAW_BUTTONS | REDRAW_BORDER);
    castleBar.Hide();
    heroesBar.Hide();

    cursor.Show();
    display.Flip();
    
    //Kingdom & kingdom = world.GetMyKingdom();

    cursor.Hide();
    cursor.SetThemes(Cursor::WAIT);
    cursor.Show();
    display.Flip();

    QueueMessage packet;

    while(LocalEvent::Get().HandleEvents())
    {
	if(Ready())
        {
	    if(!Recv(packet))
	    {
		Dialog::Message("Error", "FH2LocalClient::StartGame: recv: error", Font::BIG, Dialog::OK);
		return false;
            }
	    DEBUG(DBG_NETWORK , DBG_INFO, "FH2LocalClient::StartGame: recv: " << Network::GetMsgString(packet.GetID()));

            switch(packet.GetID())
            {
		case MSG_MAPS_LOAD:
		{
		    if(Game::IO::LoadBIN(packet))
		    {
			conf.SetMyColor(Color::Get(player_color));
			heroesBar.Reset();
			castleBar.Reset();
		    }
            	    else
            		DEBUG(DBG_NETWORK , DBG_WARN, "FH2LocalClient::StartGame: MSG_MAPS_LOAD error");
		}
		break;

		case MSG_YOUR_TURN:
		{
		    u8 color, percent;
		    packet.Pop(color);
		    packet.Pop(percent);

		    radar.HideArea();
		    conf.SetCurrentColor(Color::Get(color));
		    //Interface::Basic::SetTurnProgress(percent);

		    DEBUG(DBG_NETWORK , DBG_INFO, "FH2LocalClient::StartGame: player: " << Color::String(color));
		    world.ClearFog(color);

		    if(conf.MyColor() == color)
            	    {
			if(Game::ENDTURN == Game::HumanTurn())
			{
			    /*
			    Network::PackKingdom(packet, kingdom);
			    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::StartGame: send MSG_KINGDOM");
			    if(!Send(packet)) return false;
			    
			    // send all heroes
			    const std::vector<Heroes *> & heroes = kingdom.GetHeroes();
			    std::vector<Heroes *>::const_iterator ith1 = heroes.begin();
			    std::vector<Heroes *>::const_iterator ith2 = heroes.end();
			    for(; ith1 != ith2; ++ith1) if(*ith1)
			    {
				Network::PackHero(packet, **ith1);
				DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::StartGame: send MSG_HEROES");
				if(!Send(packet)) return false;
			    }

			    // send all castles
			    const std::vector<Castle *> & castles = kingdom.GetCastles();
			    std::vector<Castle *>::const_iterator itc1 = castles.begin();
			    std::vector<Castle *>::const_iterator itc2 = castles.end();
			    for(; itc1 != itc2; ++itc1) if(*itc1)
			    {
				Network::PackCastle(packet, **itc1);
				DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::StartGame: send MSG_CASTLE");
				if(!Send(packet)) return false;
			    }
			    */

			    packet.Reset();
			    packet.SetID(MSG_END_TURN);
			    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::StartGame: send MSG_END_TURN");
			    if(!Send(packet)) return false;
			}
			else
			    return true;
		    }
		    else
		    {
			if(STATUS_AITURN != statusWin.GetState())
			{
                    	    // for pocketpc: show status window
                    	    if(conf.PocketPC() && !conf.ShowStatus())
                    	    {
                        	conf.SetModes(Settings::SHOWSTATUS);
                    	    }

                    	    cursor.Hide();

			    statusWin.Reset();
                    	    statusWin.SetState(STATUS_AITURN);

                    	    I.SetRedraw(REDRAW_GAMEAREA | REDRAW_STATUS);

                    	    cursor.SetThemes(Cursor::WAIT);
                    	    I.Redraw();
                    	    cursor.Show();
                    	    display.Flip();
			}

		    }
		}
		break;

		//case MSG_KINGDOM:
		//{
		//    Network::UnpackKingdom(packet);
		//}
		//break;
		//
		//case MSG_TILES:
		//{
		//    Network::UnpackTile(packet);
		//}
		//break;
		//
		//case MSG_HEROES:
		//{
		//    Network::unpackHero(packet);
		//}
		//break;

		default: break;
	    }
	}
    }

    return true;
}

void FH2LocalClient::SendCastleBuyBuilding(const Castle & castle, u32 build)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_CASTLE_BUILD);
    packet.Push(castle.GetIndex());
    packet.Push(build);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendCastleBuyBuilding: " << castle.GetName() << ", build: " << Castle::GetStringBuilding(build, castle.GetRace()));
    client.Send(packet);
}

void FH2LocalClient::SendCastleRecruitHero(const Castle & castle, const Heroes & hero)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_CASTLE_RECRUIT_HERO);
    packet.Push(castle.GetIndex());
    packet.Push(static_cast<u8>(hero.GetID()));

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendCastleRecruitHero: " << castle.GetName() << ", recruit: " << hero.GetName());
    client.Send(packet);
}

void FH2LocalClient::SendCastleBuyBoat(const Castle & castle, u16 index)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_CASTLE_BUY_BOAT);
    packet.Push(castle.GetIndex());
    packet.Push(index);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendCastleBuyBoat: " << castle.GetName() << ", index: " << index);
    client.Send(packet);
}

void FH2LocalClient::SendCastleRecruitMonster(const Castle & castle, u32 dwelling, u16 count)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_CASTLE_RECRUIT_MONSTER);
    packet.Push(castle.GetIndex());
    packet.Push(dwelling);
    packet.Push(count);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendCastleRecruitMonster: " << castle.GetName());
    client.Send(packet);
}

void FH2LocalClient::SendMarketSellResource(const Kingdom & kingdom, u8 resource, u32 count, bool tradingPost)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_MARKET_SELL_RESOURCE);
    packet.Push(static_cast<u8>(tradingPost));
    packet.Push(static_cast<u8>(kingdom.GetColor()));
    packet.Push(resource);
    packet.Push(count);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendMarketSellResource: " << Resource::String(resource) << "(" << count << ")");
    client.Send(packet);
}

void FH2LocalClient::SendMarketBuyResource(const Kingdom & kingdom, u8 resource, u32 count, bool tradingPost)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_MARKET_BUY_RESOURCE);
    packet.Push(static_cast<u8>(tradingPost));
    packet.Push(static_cast<u8>(kingdom.GetColor()));
    packet.Push(resource);
    packet.Push(count);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendMarketBuyResource: " << Resource::String(resource) << "(" << count << ")");
    client.Send(packet);
}

void FH2LocalClient::SendHeroesBuyMagicBook(const Heroes & hero)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_HEROES_BUY_MAGICBOOK);
    packet.Push(static_cast<u8>(hero.GetID()));

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendHeroesBuyMagicBook: " << hero.GetName());
    client.Send(packet);
}

void FH2LocalClient::SendHeroesSwapArtifacts(const Heroes & hero1, u8 index1, const Heroes & hero2, u8 index2)
{
    if(!Network::isLocalClient()) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_HEROES_SWAP_ARTIFACTS);
    packet.Push(static_cast<u8>(hero1.GetID()));
    packet.Push(index1);
    packet.Push(static_cast<u8>(hero2.GetID()));
    packet.Push(index2);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendHeroesSwapArtifacts: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmyUpgradeTroop(const Army::army_t & army, u8 index)
{
    const HeroBase* commander = army.GetCommander();
    if(!Network::isLocalClient() || !commander) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_UPGRADE_TROOP);
    packet.Push(static_cast<u8>(commander->GetType()));
    packet.Push(commander->GetIndex());
    packet.Push(index);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmyUpgradeTroop: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmyDismissTroop(const Army::army_t & army, u8 index)
{
    const HeroBase* commander = army.GetCommander();
    if(!Network::isLocalClient() || !commander) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_DISMISS_TROOP);
    packet.Push(static_cast<u8>(commander->GetType()));
    packet.Push(commander->GetIndex());
    packet.Push(index);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmyDismissTroop: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmySwapTroops(const Army::army_t & army1, u8 index1, const Army::army_t & army2, u8 index2)
{
    const HeroBase* commander1 = army1.GetCommander();
    const HeroBase* commander2 = army2.GetCommander();
    if(!Network::isLocalClient() || !commander1 || !commander2) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_SWAP_TROOPS);
    packet.Push(static_cast<u8>(commander1->GetType()));
    packet.Push(commander1->GetIndex());
    packet.Push(index1);
    packet.Push(static_cast<u8>(commander2->GetType()));
    packet.Push(commander2->GetIndex());
    packet.Push(index2);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmySwapTroop: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmySplitTroop(const Army::army_t & army1, u8 index1, const Army::army_t & army2, u8 index2, u16 count)
{
    const HeroBase* commander1 = army1.GetCommander();
    const HeroBase* commander2 = army2.GetCommander();
    if(!Network::isLocalClient() || !commander1 || !commander2) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_SPLIT_TROOP);
    packet.Push(static_cast<u8>(commander1->GetType()));
    packet.Push(commander1->GetIndex());
    packet.Push(index1);
    packet.Push(static_cast<u8>(commander2->GetType()));
    packet.Push(commander2->GetIndex());
    packet.Push(index2);
    packet.Push(count);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmySplitTroop: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmyJoinTroops(const Army::army_t & army1, u8 index1, const Army::army_t & army2, u8 index2)
{
    const HeroBase* commander1 = army1.GetCommander();
    const HeroBase* commander2 = army2.GetCommander();
    if(!Network::isLocalClient() || !commander1 || !commander2) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_JOIN_TROOP);
    packet.Push(static_cast<u8>(commander1->GetType()));
    packet.Push(commander1->GetIndex());
    packet.Push(index1);
    packet.Push(static_cast<u8>(commander2->GetType()));
    packet.Push(commander2->GetIndex());
    packet.Push(index2);

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmyJoinTroop: ");
    client.Send(packet);
}

void FH2LocalClient::SendArmyCombatFormation(const Army::army_t & army)
{
    const HeroBase* commander = army.GetCommander();
    if(!Network::isLocalClient() || !commander) return;

    FH2LocalClient & client = FH2LocalClient::Get();
    QueueMessage packet;

    packet.SetID(MSG_ARMY_COMBAT_FORMATION);
    packet.Push(static_cast<u8>(commander->GetType()));
    packet.Push(commander->GetIndex());
    packet.Push(army.GetCombatFormat());

    DEBUG(DBG_NETWORK, DBG_INFO, "FH2LocalClient::SendArmyCombatFormation: ");
    client.Send(packet);
}

#endif
