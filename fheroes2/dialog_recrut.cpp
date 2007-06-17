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
#include "config.h"
#include "cursor.h"
#include "text.h"
#include "payment.h"
#include "world.h"
#include "tools.h"
#include "localevent.h"
#include "button.h"
#include "dialog.h"

u16 Dialog::RecrutMonster(const Monster::monster_t & monster, u16 available)
{
    // cursor
    const Cursor::themes_t cursor = Cursor::Get();
    Cursor::Hide();
    Cursor::Set(Cursor::POINTER);
    
    // calculate max count
    u16 max = 0;
    const PaymentConditions::BuyMonster paymentMonster(monster);
    const Resource::funds_t & kingdomResource = world.GetMyKingdom().GetFundsResource();
    while(Resource::funds_t(paymentMonster * max) <= kingdomResource && max <= available) ++max;

    u16 result = --max;

    PaymentConditions::payment_t paymentCosts(paymentMonster * result);

    const Sprite & box = AGG::GetICN("RECRBKG.ICN", 0);
    const Rect pos((display.w() - box.w()) / 2, (display.h() - box.h()) / 2, box.w(), box.h());

    Background back(pos);
    back.Save();

    display.Blit(box, pos);
    
    LocalEvent & le = LocalEvent::GetLocalEvent();

    Point dst_pt;

    std::string message("Recrut " + Monster::String(monster));

    // text recrut monster
    Text::Text(pos.x + (pos.w - Text::width(message, Font::BIG)) / 2, pos.y + 25, message, Font::BIG, true);

    // sprite monster
    message = 10 > monster ? "MONH000" : "MONH00";
    String::AddInt(message, monster);
    message += ".ICN";
    const Sprite & smon = AGG::GetICN(message, 0);
    dst_pt.x = pos.x + 70 - smon.w() / 2;
    dst_pt.y = pos.y + 120 - smon.h();
    display.Blit(smon, dst_pt);

    // info resource
    // gold
    const Sprite & sgold = AGG::GetICN("RESOURCE.ICN", 6);
    dst_pt.x = pos.x + (paymentMonster.ore ||
                        paymentMonster.wood ||
                        paymentMonster.mercury ||
                        paymentMonster.crystal ||
                        paymentMonster.sulfur ||
                        paymentMonster.gems ? 150 : 175);
    dst_pt.y = pos.y + 75;
    display.Blit(sgold, dst_pt);

    dst_pt.x = pos.x + (paymentMonster.ore ||
                        paymentMonster.wood ||
                        paymentMonster.mercury ||
                        paymentMonster.crystal ||
                        paymentMonster.sulfur ||
                        paymentMonster.gems ? 105 : 130);
    dst_pt.y = pos.y + 200;
    display.Blit(sgold, dst_pt);

    message.clear();
    String::AddInt(message, paymentMonster.gold);
    dst_pt.x = pos.x + (paymentMonster.ore ||
                        paymentMonster.wood ||
                        paymentMonster.mercury ||
                        paymentMonster.crystal ||
                        paymentMonster.sulfur ||
                        paymentMonster.gems ? 183 : 205);
    dst_pt.y = pos.y + 103;
    Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);

    // crystal
    if(paymentMonster.crystal)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 4);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 75;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 200;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.crystal);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // mercury
    if(paymentMonster.mercury)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 1);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 72;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 197;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.mercury);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // wood
    if(paymentMonster.wood)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 0);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 72;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 197;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.wood);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // ore
    if(paymentMonster.ore)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 2);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 72;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 197;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.ore);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // sulfur
    if(paymentMonster.sulfur)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 3);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 75;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 200;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.sulfur);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // gems
    if(paymentMonster.gems)
    {
        const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 5);
        dst_pt.x = pos.x + 225;
        dst_pt.y = pos.y + 75;
        display.Blit(sres, dst_pt);

        dst_pt.x = pos.x + 180;
        dst_pt.y = pos.y + 200;
        display.Blit(sres, dst_pt);

        message.clear();
        String::AddInt(message, paymentMonster.gems);
        dst_pt.x = pos.x + 240;
        dst_pt.y = pos.y + 103;
        Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }

    // text number buy
    message = "Number to buy:";
    dst_pt.x = pos.x + 30;
    dst_pt.y = pos.y + 163;
    Text::Text(dst_pt.x, dst_pt.y, message, Font::SMALL, true);

    Background static_info(Rect(pos.x + 16, pos.y + 125, pos.w - 32, 122));
    static_info.Save();

#define RedrawCurrentInfo \
    message = "Available: "; \
    String::AddInt(message, available); \
    Text::Text(pos.x + 70 - Text::width(message, Font::SMALL) / 2, pos.y + 130, message, Font::SMALL, true); \
    message.clear(); \
    String::AddInt(message, result); \
    Text::Text(pos.x + 167 - Text::width(message, Font::BIG) / 2, pos.y + 160, message, Font::BIG, true); \
    if(paymentMonster.ore || \
       paymentMonster.wood || \
       paymentMonster.mercury || \
       paymentMonster.crystal || \
       paymentMonster.sulfur || \
       paymentMonster.gems) \
    { \
	message.clear(); \
	String::AddInt(message, paymentCosts.gold); \
	Text::Text(pos.x + 133 - Text::width(message, Font::SMALL) / 2, pos.y + 228, message, Font::SMALL, true); \
	message.clear(); \
	if(paymentMonster.ore) String::AddInt(message, paymentCosts.ore); \
        else \
	if(paymentMonster.wood) String::AddInt(message, paymentCosts.wood); \
        else \
        if(paymentMonster.mercury) String::AddInt(message, paymentCosts.mercury); \
        else \
        if(paymentMonster.crystal) String::AddInt(message, paymentCosts.crystal); \
        else \
        if(paymentMonster.sulfur) String::AddInt(message, paymentCosts.sulfur); \
        else \
        if(paymentMonster.gems) String::AddInt(message, paymentCosts.gems); \
	Text::Text(pos.x + 195 - Text::width(message, Font::SMALL) / 2, pos.y + 228, message, Font::SMALL, true); \
    } \
    else \
    { \
	message.clear(); \
	String::AddInt(message, paymentCosts.gold); \
	Text::Text(pos.x + 160 - Text::width(message, Font::SMALL) / 2, pos.y + 228, message, Font::SMALL, true); \
    }

    RedrawCurrentInfo;

    // buttons
    dst_pt.x = pos.x + 34;
    dst_pt.y = pos.y + 249;
    Button buttonOk(dst_pt, "RECRUIT.ICN", 8, 9);

    dst_pt.x = pos.x + 187;
    dst_pt.y = pos.y + 249;
    Button buttonCancel(dst_pt, "RECRUIT.ICN", 6, 7);

    dst_pt.x = pos.x + 230;
    dst_pt.y = pos.y + 155;
    Button buttonMax(dst_pt, "RECRUIT.ICN", 4, 5);

    dst_pt.x = pos.x + 208;
    dst_pt.y = pos.y + 156;
    Button buttonUp(dst_pt, "RECRUIT.ICN", 0, 1);

    dst_pt.x = pos.x + 208;
    dst_pt.y = pos.y + 171;
    Button buttonDn(dst_pt, "RECRUIT.ICN", 2, 3);

    display.Flip();
    Cursor::Show();

    le.ResetKey();

    // message loop
    bool exit = false;

    while(!exit)
    {
        le.HandleEvents();

	le.MousePressLeft(buttonOk) ? buttonOk.Press() : buttonOk.Release();
	le.MousePressLeft(buttonCancel) ? buttonCancel.Press() : buttonCancel.Release();
	le.MousePressLeft(buttonMax) ? buttonMax.Press() : buttonMax.Release();
	le.MousePressLeft(buttonUp) ? buttonUp.Press() : buttonUp.Release();
	le.MousePressLeft(buttonDn) ? buttonDn.Press() : buttonDn.Release();

	if(le.MouseClickLeft(buttonUp) && result < max)
	{
	    ++result;
	    paymentCosts += paymentMonster;
	    Cursor::Hide();
	    static_info.Restore();
	    RedrawCurrentInfo;
	    Cursor::Show();
	    display.Flip();
	}

	if(le.MouseClickLeft(buttonDn) && result)
	{
	    --result;
	    paymentCosts -= paymentMonster;
	    Cursor::Hide();
	    static_info.Restore();
	    RedrawCurrentInfo;
	    Cursor::Show();
	    display.Flip();
	}

	if(le.MouseClickLeft(buttonMax) && result != max)
	{
	    result = max;
	    paymentCosts = paymentMonster * max;
	    Cursor::Hide();
	    static_info.Restore();
	    RedrawCurrentInfo;
	    Cursor::Show();
	    display.Flip();
	}
	
	if(le.MouseClickLeft(buttonOk) || le.KeyPress(SDLK_RETURN)) exit = true;
	
	if(le.MouseClickLeft(buttonCancel) || le.KeyPress(SDLK_ESCAPE)){ exit = true; result = 0; }
    }

    le.ResetKey();

    Cursor::Hide();

    back.Restore();
    Cursor::Set(cursor);

    display.Flip();
    Cursor::Show();

    return result;
}

void Dialog::DwellingInfo(const Monster::monster_t & monster, u16 available)
{
    // cursor
    const Cursor::themes_t cursor = Cursor::Get();
    Cursor::Hide();
    Cursor::Set(Cursor::POINTER);
    
    const PaymentConditions::BuyMonster paymentMonster(monster);

    const Sprite & box = AGG::GetICN("RECR2BKG.ICN", 0);
    const Rect pos((display.w() - box.w()) / 2, (display.h() - box.h()) / 2, box.w(), box.h());

    Background back(pos);
    back.Save();

    display.Blit(box, pos);
    
    LocalEvent & le = LocalEvent::GetLocalEvent();

    Point dst_pt;

    std::string message("Recrut " + Monster::String(monster));

    // text recrut monster
    Text::Text(pos.x + (pos.w - Text::width(message, Font::BIG)) / 2, pos.y + 25, message, Font::BIG, true);

    // sprite monster
    message = 10 > monster ? "MONH000" : "MONH00";
    String::AddInt(message, monster);
    message += ".ICN";
    const Sprite & smon = AGG::GetICN(message, 0);
    dst_pt.x = pos.x + 70 - smon.w() / 2;
    dst_pt.y = pos.y + 120 - smon.h();
    display.Blit(smon, dst_pt);

    // info resource
    // gold
    const Sprite & sgold = AGG::GetICN("RESOURCE.ICN", 6);
    dst_pt.x = pos.x + (paymentMonster.ore ||
                        paymentMonster.wood ||
                        paymentMonster.mercury ||
                        paymentMonster.crystal ||
                        paymentMonster.sulfur ||
                        paymentMonster.gems ? 150 : 175);
    dst_pt.y = pos.y + 75;
    display.Blit(sgold, dst_pt);

    message.clear();
    String::AddInt(message, paymentMonster.gold);
    dst_pt.x = pos.x + (paymentMonster.ore ||
                        paymentMonster.wood ||
                        paymentMonster.mercury ||
                        paymentMonster.crystal ||
                        paymentMonster.sulfur ||
                        paymentMonster.gems ? 183 : 205);
    dst_pt.y = pos.y + 103;
    Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    // crystal
    if(paymentMonster.crystal)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 4);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 75;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.crystal);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // mercury
    if(paymentMonster.mercury)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 1);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 72;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.mercury);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // wood
    if(paymentMonster.wood)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 0);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 72;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.wood);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // ore
    if(paymentMonster.ore)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 2);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 72;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.ore);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // sulfur
    if(paymentMonster.sulfur)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 3);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 75;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.sulfur);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }
    else
    // gems
    if(paymentMonster.gems)
    {
	const Sprite & sres = AGG::GetICN("RESOURCE.ICN", 5);
	dst_pt.x = pos.x + 225;
	dst_pt.y = pos.y + 75;
	display.Blit(sres, dst_pt);

	message.clear();
	String::AddInt(message, paymentMonster.gems);
	dst_pt.x = pos.x + 240;
	dst_pt.y = pos.y + 103;
	Text::Text(dst_pt.x - Text::width(message, Font::SMALL)/2, dst_pt.y, message, Font::SMALL, true);
    }

    // text available
    message = "Available: ";
    String::AddInt(message, available);
    Text::Text(pos.x + 70 - Text::width(message, Font::SMALL) / 2, pos.y + 130, message, Font::SMALL, true);
    message.clear();

    display.Flip();
    Cursor::Show();

    le.ResetKey();

    // message loop
    bool exit = false;

    while(!exit)
    {
        le.HandleEvents();

        if(!le.MouseRight()) exit = true;
    }

    le.ResetKey();

    Cursor::Hide();

    back.Restore();
    Cursor::Set(cursor);

    display.Flip();
    Cursor::Show();
}
