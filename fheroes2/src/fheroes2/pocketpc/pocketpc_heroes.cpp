/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov                               *
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
#include "cursor.h"
#include "text.h"
#include "button.h"
#include "heroes.h"
#include "heroes_indicator.h"
#include "selectarmybar.h"
#include "selectartifactbar.h"
#include "pocketpc.h"

extern void RedrawSecondarySkill(const Point &, const std::vector<Skill::Secondary> &);

Dialog::answer_t PocketPC::HeroesOpenDialog(Heroes & hero, bool readonly)
{
    Cursor & cursor = Cursor::Get();
    Display & display = Display::Get();
    LocalEvent & le = LocalEvent::Get();

    cursor.Hide();
    cursor.SetThemes(cursor.POINTER);

    const u16 window_w = 320;
    const u16 window_h = 224;

    Dialog::FrameBorder frameborder;
    frameborder.SetPosition((display.w() - window_w) / 2 - BORDERWIDTH, (display.h() - window_h) / 2 - BORDERWIDTH, window_w, window_h);
    frameborder.Redraw();

    const Rect & dst_rt = frameborder.GetArea();
    const Sprite & background = AGG::GetICN(ICN::STONEBAK, 0);
    const Sprite & backSprite = AGG::GetICN(ICN::SWAPWIN, 0);
    display.Blit(background, Rect(0, 0, window_w, window_h), dst_rt);

    // portrait
    display.Blit(AGG::GetICN(ICN::BRCREST, 6), dst_rt.x + 8, dst_rt.y);
    display.Blit(hero.GetPortrait50x46(), dst_rt.x + 12, dst_rt.y + 4);

    // name
    std::string message = _("%{name} the %{race} ( Level %{level} )");
    String::Replace(message, "%{name}", hero.GetName());
    String::Replace(message, "%{race}", Race::String(hero.GetRace()));
    String::Replace(message, "%{level}", hero.GetLevel());
    Text text(message, Font::SMALL);
    text.Blit(dst_rt.x + 73, dst_rt.y + 1);

    // experience
    display.Blit(AGG::GetICN(ICN::HSICONS, 1), dst_rt.x + 206, dst_rt.y + 14);
    message.clear();
    String::AddInt(message, hero.GetExperience());
    text.Set(message);
    text.Blit(dst_rt.x + 223 - text.w() / 2, dst_rt.y + 37);

    // spell points
    display.Blit(AGG::GetICN(ICN::HSICONS, 8), dst_rt.x + 240, dst_rt.y + 16);
    message.clear();
    String::AddInt(message, hero.GetSpellPoints());
    message += "/";
    String::AddInt(message, hero.GetMaxSpellPoints());
    text.Set(message);
    text.Blit(dst_rt.x + 256 - text.w() / 2, dst_rt.y + 37);

    // morale
    MoraleIndicator moraleIndicator(hero);
    moraleIndicator.SetPos(Point(dst_rt.x + 280, dst_rt.y + 20));
    moraleIndicator.Redraw();

    // luck
    LuckIndicator luckIndicator(hero);
    luckIndicator.SetPos(Point(dst_rt.x + 280, dst_rt.y + 60));
    luckIndicator.Redraw();

    // prim skill
    display.Blit(backSprite, Rect(216, 51, 34, 34),  dst_rt.x + 74, dst_rt.y + 14);
    message.clear();
    String::AddInt(message, hero.GetAttack());
    text.Set(message);
    text.Blit(dst_rt.x + 74 + (34 - text.w()) / 2, dst_rt.y + 47);

    display.Blit(backSprite, Rect(216, 84, 34, 34),  dst_rt.x + 107, dst_rt.y + 14);
    message.clear();
    String::AddInt(message, hero.GetDefense());
    text.Set(message);
    text.Blit(dst_rt.x + 107 + (34 - text.w()) / 2, dst_rt.y + 47);

    display.Blit(backSprite, Rect(216, 117, 34, 34), dst_rt.x + 140, dst_rt.y + 14);
    message.clear();
    String::AddInt(message, hero.GetPower());
    text.Set(message);
    text.Blit(dst_rt.x + 140 + (34 - text.w()) / 2, dst_rt.y + 47);

    display.Blit(backSprite, Rect(216, 150, 34, 34), dst_rt.x + 173, dst_rt.y + 14);
    message.clear();
    String::AddInt(message, hero.GetKnowledge());
    text.Set(message);
    text.Blit(dst_rt.x + 173 + (34 - text.w()) / 2, dst_rt.y + 47);

    // sec skill
    display.Blit(backSprite, Rect(21, 198, 267, 36), dst_rt.x + 7, dst_rt.y + 57);
    RedrawSecondarySkill(Point(dst_rt.x + 9, dst_rt.y + 59), hero.GetSecondarySkills());

    // army bar
    const Rect rt1(36, 267, 43, 53);
    Surface sfb1(rt1.w, rt1.h);
    sfb1.Blit(backSprite, rt1, 0, 0);
    Surface sfc1(rt1.w, rt1.h - 10);
    Cursor::DrawCursor(sfc1, 0x10, true);

    SelectArmyBar selectArmy;
    selectArmy.SetArmy(hero.GetArmy());
    selectArmy.SetPos(dst_rt.x + 50, dst_rt.y + 95);
    selectArmy.SetInterval(2);
    selectArmy.SetBackgroundSprite(sfb1);
    selectArmy.SetCursorSprite(sfc1);
    selectArmy.SetUseMons32Sprite();
    selectArmy.SetSaveLastTroop();
    if(readonly) selectArmy.SetReadOnly();
    const Castle* castle = hero.inCastle();
    if(castle) selectArmy.SetCastle(*castle);
    selectArmy.Redraw();
            
    // art bar
    const Rect rt2(23, 347, 34, 34);
    Surface sfb2(rt2.w, rt2.h);
    sfb2.Blit(backSprite, rt2, 0, 0);
    Surface sfc2(rt2.w, rt2.h);
    Cursor::DrawCursor(sfc2, 0x10, true);

    SelectArtifactsBar selectArtifacts(hero);
    selectArtifacts.SetPos(dst_rt.x + 37, dst_rt.y + 150);
    selectArtifacts.SetInterval(2);
    selectArtifacts.SetBackgroundSprite(sfb2);
    selectArtifacts.SetCursorSprite(sfc2);
    selectArtifacts.SetUseArts32Sprite();
    if(readonly) selectArtifacts.SetReadOnly();
    selectArtifacts.Redraw();

    Button buttonDismiss(dst_rt.x + dst_rt.w / 2 - 160, dst_rt.y + dst_rt.h - 125, ICN::HSBTNS, 0, 1);
    Button buttonExit(dst_rt.x + dst_rt.w / 2 + 130, dst_rt.y + dst_rt.h - 125, ICN::HSBTNS, 2, 3);

    if(castle || readonly)
    {
	buttonDismiss.Press();
        buttonDismiss.SetDisable(true);
    }

    buttonDismiss.Draw();
    buttonExit.Draw();

    cursor.Show();
    display.Flip();

    while(le.HandleEvents())
    {
        le.MousePressLeft(buttonExit) ? buttonExit.PressDraw() : buttonExit.ReleaseDraw();
        if(buttonDismiss.isEnable()) le.MousePressLeft(buttonDismiss) ? buttonDismiss.PressDraw() : buttonDismiss.ReleaseDraw();

        // exit
        if(le.MouseClickLeft(buttonExit) || le.KeyPress(KEY_ESCAPE)) return Dialog::CANCEL;

        // dismiss
	if(buttonDismiss.isEnable() && le.MouseClickLeft(buttonDismiss) &&
	    Dialog::YES == Dialog::Message(hero.GetName(), _("Are you sure you want to dismiss this Hero?"), Font::BIG, Dialog::YES | Dialog::NO))
        { return Dialog::DISMISS; }

        // selector troops event
        if(le.MouseCursor(selectArmy.GetArea()))
        {
            if(selectArtifacts.isSelected()) selectArtifacts.Reset();
	    if(SelectArmyBar::QueueEventProcessing(selectArmy))
            {
                cursor.Hide();
                moraleIndicator.Redraw();
                luckIndicator.Redraw();
                cursor.Show();
        	display.Flip();
            }
        }

        // selector artifacts event
        if(le.MouseCursor(selectArtifacts.GetArea()))
        {
            if(selectArmy.isSelected()) selectArmy.Reset();
            SelectArtifactsBar::QueueEventProcessing(selectArtifacts);
        }

        if(le.MouseCursor(moraleIndicator.GetArea())) MoraleIndicator::QueueEventProcessing(moraleIndicator);
        else
        if(le.MouseCursor(luckIndicator.GetArea())) LuckIndicator::QueueEventProcessing(luckIndicator);
    }

    return Dialog::ZERO;
}