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

#include <cstdlib> 
#include <fstream> 
#include "error.h" 
#include "agg.h" 
#include "artifact.h"
#include "resource.h"
#include "config.h" 
#include "kingdom.h" 
#include "heroes.h" 
#include "castle.h" 
#include "gameevent.h" 
#include "mp2.h" 
#include "text.h" 
#include "rand.h" 
#include "world.h"

u32 World::uniq0 = 0;

World & World::GetWorld(void)
{
    static World insideWorld;

    return insideWorld;
}

/* new maps */
void World::NewMaps(const u16 sw, const u16 sh)
{
    FreeOldMaps();

    width = sw;
    height = sh;

    ultimate_artifact = 0xFFFF;

    day = 0;
    week = 0;
    month = 0;

    begin_week = true;
    begin_month = true;

    free_recruit_hero1 = Heroes::UNKNOWN;
    free_recruit_hero2 = Heroes::UNKNOWN;

    // playing kingdom
    Settings::Get().FileInfo().SetKingdomColors(Color::BLUE | Color::GREEN | Color::RED | Color::YELLOW | Color::ORANGE | Color::PURPLE);
    vec_kingdoms.resize(KINGDOMMAX);
    vec_kingdoms[0] = new Kingdom(Color::BLUE);
    vec_kingdoms[1] = new Kingdom(Color::GREEN);
    vec_kingdoms[2] = new Kingdom(Color::RED);
    vec_kingdoms[3] = new Kingdom(Color::YELLOW);
    vec_kingdoms[4] = new Kingdom(Color::ORANGE);
    vec_kingdoms[5] = new Kingdom(Color::PURPLE);

    // initialize all heroes
    vec_heroes.resize(HEROESMAXCOUNT + 2);
    vec_heroes[0] = new Heroes(Heroes::LORDKILBURN, Race::KNGT, "Lord Kilburn");
    vec_heroes[1] = new Heroes(Heroes::SIRGALLANTH, Race::KNGT, "Sir Gallanth");
    vec_heroes[2] = new Heroes(Heroes::ECTOR, Race::KNGT, "Ector");
    vec_heroes[3] = new Heroes(Heroes::GVENNETH, Race::KNGT, "Gvenneth");
    vec_heroes[4] = new Heroes(Heroes::TYRO, Race::KNGT, "Tyro");
    vec_heroes[5] = new Heroes(Heroes::AMBROSE, Race::KNGT, "Ambrose");
    vec_heroes[6] = new Heroes(Heroes::RUBY, Race::KNGT, "Ruby");
    vec_heroes[7] = new Heroes(Heroes::MAXIMUS, Race::KNGT, "Maximus");
    vec_heroes[8] = new Heroes(Heroes::DIMITRY, Race::KNGT, "Dimitry");

    vec_heroes[9] = new Heroes(Heroes::THUNDAX, Race::BARB, "Thundax");
    vec_heroes[10] = new Heroes(Heroes::FINEOUS, Race::BARB, "Fineous");
    vec_heroes[11] = new Heroes(Heroes::JOJOSH, Race::BARB, "Jojosh");
    vec_heroes[12] = new Heroes(Heroes::CRAGHACK, Race::BARB, "Crag Hack");
    vec_heroes[13] = new Heroes(Heroes::JEZEBEL, Race::BARB, "Jezebel");
    vec_heroes[14] = new Heroes(Heroes::JACLYN, Race::BARB, "Jaclyn");
    vec_heroes[15] = new Heroes(Heroes::ERGON, Race::BARB, "Ergon");
    vec_heroes[16] = new Heroes(Heroes::TSABU, Race::BARB, "Tsabu");
    vec_heroes[17] = new Heroes(Heroes::ATLAS, Race::BARB, "Atlas");

    vec_heroes[18] = new Heroes(Heroes::ASTRA, Race::SORC, "Astra");
    vec_heroes[19] = new Heroes(Heroes::NATASHA, Race::SORC, "Natasha");
    vec_heroes[20] = new Heroes(Heroes::TROYAN, Race::SORC, "Troyan");
    vec_heroes[21] = new Heroes(Heroes::VATAWNA, Race::SORC, "Vatawna");
    vec_heroes[22] = new Heroes(Heroes::REBECCA, Race::SORC, "Rebecca");
    vec_heroes[23] = new Heroes(Heroes::GEM, Race::SORC, "Gem");
    vec_heroes[24] = new Heroes(Heroes::ARIEL, Race::SORC, "Ariel");
    vec_heroes[25] = new Heroes(Heroes::CARLAWN, Race::SORC, "Carlawn");
    vec_heroes[26] = new Heroes(Heroes::LUNA, Race::SORC, "Luna");

    vec_heroes[27] = new Heroes(Heroes::ARIE, Race::WRLK, "Arie");
    vec_heroes[28] = new Heroes(Heroes::ALAMAR, Race::WRLK, "Alamar");
    vec_heroes[29] = new Heroes(Heroes::VESPER, Race::WRLK, "Vesper");
    vec_heroes[30] = new Heroes(Heroes::CRODO, Race::WRLK, "Crodo");
    vec_heroes[31] = new Heroes(Heroes::BAROK, Race::WRLK, "Barok");
    vec_heroes[32] = new Heroes(Heroes::KASTORE, Race::WRLK, "Kastore");
    vec_heroes[33] = new Heroes(Heroes::AGAR, Race::WRLK, "Agar");
    vec_heroes[34] = new Heroes(Heroes::FALAGAR, Race::WRLK, "Falagar");
    vec_heroes[35] = new Heroes(Heroes::WRATHMONT, Race::WRLK, "Wrathmont");

    vec_heroes[36] = new Heroes(Heroes::MYRA, Race::WZRD, "Myra");
    vec_heroes[37] = new Heroes(Heroes::FLINT, Race::WZRD, "Flint");
    vec_heroes[38] = new Heroes(Heroes::DAWN, Race::WZRD, "Dawn");
    vec_heroes[39] = new Heroes(Heroes::HALON, Race::WZRD, "Halon");
    vec_heroes[40] = new Heroes(Heroes::MYRINI, Race::WZRD, "Myrini");
    vec_heroes[41] = new Heroes(Heroes::WILFREY, Race::WZRD, "Wilfrey");
    vec_heroes[42] = new Heroes(Heroes::SARAKIN, Race::WZRD, "Sarakin");
    vec_heroes[43] = new Heroes(Heroes::KALINDRA, Race::WZRD, "Kalindra");
    vec_heroes[44] = new Heroes(Heroes::MANDIGAL, Race::WZRD, "Mandigal");

    vec_heroes[45] = new Heroes(Heroes::ZOM, Race::NECR, "Zom");
    vec_heroes[46] = new Heroes(Heroes::DARLANA, Race::NECR, "Darlana");
    vec_heroes[47] = new Heroes(Heroes::ZAM, Race::NECR, "Zam");
    vec_heroes[48] = new Heroes(Heroes::RANLOO, Race::NECR, "Ranloo");
    vec_heroes[49] = new Heroes(Heroes::CHARITY, Race::NECR, "Charity");
    vec_heroes[50] = new Heroes(Heroes::RIALDO, Race::NECR, "Rialdo");
    vec_heroes[51] = new Heroes(Heroes::ROXANA, Race::NECR, "Roxana");
    vec_heroes[52] = new Heroes(Heroes::SANDRO, Race::NECR, "Sandro");
    vec_heroes[53] = new Heroes(Heroes::CELIA, Race::NECR, "Celia");

    vec_heroes[54] = new Heroes(Heroes::ROLAND, Race::WZRD, "Roland");
    vec_heroes[55] = new Heroes(Heroes::CORLAGON, Race::KNGT, "Lord Corlagon");
    vec_heroes[56] = new Heroes(Heroes::ELIZA, Race::SORC, "Sister Eliza");
    vec_heroes[57] = new Heroes(Heroes::ARCHIBALD, Race::WRLK, "Archibald");
    vec_heroes[58] = new Heroes(Heroes::HALTON, Race::KNGT, "Lord Halton");
    vec_heroes[59] = new Heroes(Heroes::BAX, Race::NECR, "Brother Bax");

    if(Settings::Get().Modes(Settings::PRICELOYALTY))
    {
	vec_heroes[60] = new Heroes(Heroes::SOLMYR, Race::WZRD, "Solmyr");
	vec_heroes[61] = new Heroes(Heroes::DAINWIN, Race::WRLK, "Dainwin");
	vec_heroes[62] = new Heroes(Heroes::MOG, Race::NECR, "Mog");
	vec_heroes[63] = new Heroes(Heroes::UNCLEIVAN, Race::BARB, "Uncle Ivan");
	vec_heroes[64] = new Heroes(Heroes::JOSEPH, Race::KNGT, "Joseph");
	vec_heroes[65] = new Heroes(Heroes::GALLAVANT, Race::KNGT, "Gallavant");
	vec_heroes[66] = new Heroes(Heroes::ELDERIAN, Race::WRLK, "Elderian");
	vec_heroes[67] = new Heroes(Heroes::CEALLACH, Race::KNGT, "Ceallach");
	vec_heroes[68] = new Heroes(Heroes::DRAKONIA, Race::WZRD, "Drakonia");
	vec_heroes[69] = new Heroes(Heroes::MARTINE, Race::SORC, "Martine");
	vec_heroes[70] = new Heroes(Heroes::JARKONAS, Race::BARB, "Jarkonas");
	vec_heroes[71] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[72] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
    }
    else
    {
	vec_heroes[60] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[61] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[62] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[63] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[64] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[65] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[66] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[67] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[68] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[69] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[70] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[71] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[72] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
    }

    Display & display = Display::Get();

    // loading info
    display.Fill(0, 0, 0);
    TextBox("Maps Loading...", Font::BIG, Rect(0, display.h()/2, display.w(), display.h()/2));
    display.Flip();

    AGG::PreloadObject(TIL::GROUND32);

    vec_tiles.resize(width * height);

    // init all tiles
    for(u16 ii = 0; ii < width * height; ++ii)
    {
	MP2::mp2tile_t mp2tile;
	
	mp2tile.tileIndex	= Rand::Get(16, 19);	// index sprite ground, see ground32.til
        mp2tile.objectName1	= 0;			// object sprite level 1
        mp2tile.indexName1	= 0xff;			// index sprite level 1
        mp2tile.quantity1	= 0;
        mp2tile.quantity2	= 0;
        mp2tile.objectName2	= 0;			// object sprite level 2
        mp2tile.indexName2	= 0xff;			// index sprite level 2
        mp2tile.shape		= Rand::Get(0, 3);	// shape reflect % 4, 0 none, 1 vertical, 2 horizontal, 3 any
        mp2tile.generalObject	= MP2::OBJ_ZERO;
        mp2tile.indexAddon	= 0;
        mp2tile.uniqNumber1	= 0;
        mp2tile.uniqNumber2	= 0;

	vec_tiles[ii] = new Maps::Tiles(ii, mp2tile);
    }
}

/* load maps */
void World::LoadMaps(const std::string &filename)
{
    FreeOldMaps();

    ultimate_artifact = 0xFFFF;

    day = 0;
    week = 0;
    month = 0;

    begin_week = true;
    begin_month = true;

    free_recruit_hero1 = Heroes::UNKNOWN;
    free_recruit_hero2 = Heroes::UNKNOWN;

    std::fstream fd(filename.c_str(), std::ios::in | std::ios::binary);

    if(! fd || fd.fail()) Error::Except("LoadMP2: " + filename + ", file not found.");

    // playing kingdom
    vec_kingdoms.resize(KINGDOMMAX);
    vec_kingdoms[0] = new Kingdom(Color::BLUE);
    vec_kingdoms[1] = new Kingdom(Color::GREEN);
    vec_kingdoms[2] = new Kingdom(Color::RED);
    vec_kingdoms[3] = new Kingdom(Color::YELLOW);
    vec_kingdoms[4] = new Kingdom(Color::ORANGE);
    vec_kingdoms[5] = new Kingdom(Color::PURPLE);

    // initialize all heroes
    vec_heroes.resize(HEROESMAXCOUNT + 2);
    vec_heroes[0] = new Heroes(Heroes::LORDKILBURN, Race::KNGT, "Lord Kilburn");
    vec_heroes[1] = new Heroes(Heroes::SIRGALLANTH, Race::KNGT, "Sir Gallanth");
    vec_heroes[2] = new Heroes(Heroes::ECTOR, Race::KNGT, "Ector");
    vec_heroes[3] = new Heroes(Heroes::GVENNETH, Race::KNGT, "Gvenneth");
    vec_heroes[4] = new Heroes(Heroes::TYRO, Race::KNGT, "Tyro");
    vec_heroes[5] = new Heroes(Heroes::AMBROSE, Race::KNGT, "Ambrose");
    vec_heroes[6] = new Heroes(Heroes::RUBY, Race::KNGT, "Ruby");
    vec_heroes[7] = new Heroes(Heroes::MAXIMUS, Race::KNGT, "Maximus");
    vec_heroes[8] = new Heroes(Heroes::DIMITRY, Race::KNGT, "Dimitry");

    vec_heroes[9] = new Heroes(Heroes::THUNDAX, Race::BARB, "Thundax");
    vec_heroes[10] = new Heroes(Heroes::FINEOUS, Race::BARB, "Fineous");
    vec_heroes[11] = new Heroes(Heroes::JOJOSH, Race::BARB, "Jojosh");
    vec_heroes[12] = new Heroes(Heroes::CRAGHACK, Race::BARB, "Crag Hack");
    vec_heroes[13] = new Heroes(Heroes::JEZEBEL, Race::BARB, "Jezebel");
    vec_heroes[14] = new Heroes(Heroes::JACLYN, Race::BARB, "Jaclyn");
    vec_heroes[15] = new Heroes(Heroes::ERGON, Race::BARB, "Ergon");
    vec_heroes[16] = new Heroes(Heroes::TSABU, Race::BARB, "Tsabu");
    vec_heroes[17] = new Heroes(Heroes::ATLAS, Race::BARB, "Atlas");

    vec_heroes[18] = new Heroes(Heroes::ASTRA, Race::SORC, "Astra");
    vec_heroes[19] = new Heroes(Heroes::NATASHA, Race::SORC, "Natasha");
    vec_heroes[20] = new Heroes(Heroes::TROYAN, Race::SORC, "Troyan");
    vec_heroes[21] = new Heroes(Heroes::VATAWNA, Race::SORC, "Vatawna");
    vec_heroes[22] = new Heroes(Heroes::REBECCA, Race::SORC, "Rebecca");
    vec_heroes[23] = new Heroes(Heroes::GEM, Race::SORC, "Gem");
    vec_heroes[24] = new Heroes(Heroes::ARIEL, Race::SORC, "Ariel");
    vec_heroes[25] = new Heroes(Heroes::CARLAWN, Race::SORC, "Carlawn");
    vec_heroes[26] = new Heroes(Heroes::LUNA, Race::SORC, "Luna");

    vec_heroes[27] = new Heroes(Heroes::ARIE, Race::WRLK, "Arie");
    vec_heroes[28] = new Heroes(Heroes::ALAMAR, Race::WRLK, "Alamar");
    vec_heroes[29] = new Heroes(Heroes::VESPER, Race::WRLK, "Vesper");
    vec_heroes[30] = new Heroes(Heroes::CRODO, Race::WRLK, "Crodo");
    vec_heroes[31] = new Heroes(Heroes::BAROK, Race::WRLK, "Barok");
    vec_heroes[32] = new Heroes(Heroes::KASTORE, Race::WRLK, "Kastore");
    vec_heroes[33] = new Heroes(Heroes::AGAR, Race::WRLK, "Agar");
    vec_heroes[34] = new Heroes(Heroes::FALAGAR, Race::WRLK, "Falagar");
    vec_heroes[35] = new Heroes(Heroes::WRATHMONT, Race::WRLK, "Wrathmont");

    vec_heroes[36] = new Heroes(Heroes::MYRA, Race::WZRD, "Myra");
    vec_heroes[37] = new Heroes(Heroes::FLINT, Race::WZRD, "Flint");
    vec_heroes[38] = new Heroes(Heroes::DAWN, Race::WZRD, "Dawn");
    vec_heroes[39] = new Heroes(Heroes::HALON, Race::WZRD, "Halon");
    vec_heroes[40] = new Heroes(Heroes::MYRINI, Race::WZRD, "Myrini");
    vec_heroes[41] = new Heroes(Heroes::WILFREY, Race::WZRD, "Wilfrey");
    vec_heroes[42] = new Heroes(Heroes::SARAKIN, Race::WZRD, "Sarakin");
    vec_heroes[43] = new Heroes(Heroes::KALINDRA, Race::WZRD, "Kalindra");
    vec_heroes[44] = new Heroes(Heroes::MANDIGAL, Race::WZRD, "Mandigal");

    vec_heroes[45] = new Heroes(Heroes::ZOM, Race::NECR, "Zom");
    vec_heroes[46] = new Heroes(Heroes::DARLANA, Race::NECR, "Darlana");
    vec_heroes[47] = new Heroes(Heroes::ZAM, Race::NECR, "Zam");
    vec_heroes[48] = new Heroes(Heroes::RANLOO, Race::NECR, "Ranloo");
    vec_heroes[49] = new Heroes(Heroes::CHARITY, Race::NECR, "Charity");
    vec_heroes[50] = new Heroes(Heroes::RIALDO, Race::NECR, "Rialdo");
    vec_heroes[51] = new Heroes(Heroes::ROXANA, Race::NECR, "Roxana");
    vec_heroes[52] = new Heroes(Heroes::SANDRO, Race::NECR, "Sandro");
    vec_heroes[53] = new Heroes(Heroes::CELIA, Race::NECR, "Celia");

    vec_heroes[54] = new Heroes(Heroes::ROLAND, Race::WZRD, "Roland");
    vec_heroes[55] = new Heroes(Heroes::CORLAGON, Race::KNGT, "Lord Corlagon");
    vec_heroes[56] = new Heroes(Heroes::ELIZA, Race::SORC, "Sister Eliza");
    vec_heroes[57] = new Heroes(Heroes::ARCHIBALD, Race::WRLK, "Archibald");
    vec_heroes[58] = new Heroes(Heroes::HALTON, Race::KNGT, "Lord Halton");
    vec_heroes[59] = new Heroes(Heroes::BAX, Race::NECR, "Brother Bax");

    if(Settings::Get().Modes(Settings::PRICELOYALTY))
    {
	vec_heroes[60] = new Heroes(Heroes::SOLMYR, Race::WZRD, "Solmyr");
	vec_heroes[61] = new Heroes(Heroes::DAINWIN, Race::WRLK, "Dainwin");
	vec_heroes[62] = new Heroes(Heroes::MOG, Race::NECR, "Mog");
	vec_heroes[63] = new Heroes(Heroes::UNCLEIVAN, Race::BARB, "Uncle Ivan");
	vec_heroes[64] = new Heroes(Heroes::JOSEPH, Race::KNGT, "Joseph");
	vec_heroes[65] = new Heroes(Heroes::GALLAVANT, Race::KNGT, "Gallavant");
	vec_heroes[66] = new Heroes(Heroes::ELDERIAN, Race::WRLK, "Elderian");
	vec_heroes[67] = new Heroes(Heroes::CEALLACH, Race::KNGT, "Ceallach");
	vec_heroes[68] = new Heroes(Heroes::DRAKONIA, Race::WZRD, "Drakonia");
	vec_heroes[69] = new Heroes(Heroes::MARTINE, Race::SORC, "Martine");
	vec_heroes[70] = new Heroes(Heroes::JARKONAS, Race::BARB, "Jarkonas");
	vec_heroes[71] = H2Config::Debug() ? new Heroes(Heroes::SANDYSANDY, Race::WRLK, "SandySandy") : new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[72] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
    }
    else
    {
	vec_heroes[60] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[61] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[62] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[63] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[64] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[65] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[66] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[67] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[68] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[69] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[70] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[71] = H2Config::Debug() ? new Heroes(Heroes::SANDYSANDY, Race::WRLK, "SandySandy") : new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
	vec_heroes[72] = new Heroes(Heroes::UNKNOWN, Race::KNGT, "Unknown");
    }

    Display & display = Display::Get();

    // loading info
    display.Fill(0, 0, 0);
    TextBox("Maps Loading...", Font::BIG, Rect(0, display.h()/2, display.w(), display.h()/2));
    display.Flip();

    AGG::PreloadObject(TIL::GROUND32);

    char byte8;
    u16  byte16;
    u32  byte32;
    std::vector<u16> vec_object; // index maps for OBJ_CASTLE, OBJ_HEROES, OBJ_SIGN, OBJ_BOTTLE, OBJ_EVENT


    // endof
    fd.seekg(0, std::ios_base::end);
    const u32 endof_mp2 = fd.tellg();

    // read uniq
    fd.seekg(endof_mp2 - sizeof(u32), std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&uniq0), sizeof(u32));
    SWAP32(uniq0);

    // offset data
    fd.seekg(MP2OFFSETDATA - 2 * sizeof(u32), std::ios_base::beg);

    // width
    fd.read(reinterpret_cast<char *>(&byte32), sizeof(u32));
    SWAP32(byte32);

    switch(byte32)
    {
        case Maps::SMALL:  width = Maps::SMALL;  break;
        case Maps::MEDIUM: width = Maps::MEDIUM; break;
        case Maps::LARGE:  width = Maps::LARGE;  break;
        case Maps::XLARGE: width = Maps::XLARGE; break;
	default: width = Maps::ZERO; break;
    }
    height = width;

    // height
    fd.read(reinterpret_cast<char *>(&byte32), sizeof(u32));
    SWAP32(byte32);

    if(byte32 != static_cast<u32>(height)) Error::Warning("World::World: maps size mismatch!");

    // seek to ADDONS block
    fd.ignore(width * height * SIZEOFMP2TILE);

    // count mp2addon_t
    fd.read(reinterpret_cast<char *>(&byte32), sizeof(u32));
    SWAP32(byte32);

    // read all addons
    std::vector<MP2::mp2addon_t> vec_mp2addons;

    for(unsigned int ii = 0; ii < byte32; ++ii)
    {
	MP2::mp2addon_t mp2addon;

	fd.read(reinterpret_cast<char *>(&mp2addon.indexAddon), sizeof(u16));
	SWAP16(mp2addon.indexAddon);

	fd.read(&byte8, 1);
	mp2addon.objectNameN1 = byte8 * 2;

	fd.read(&byte8, 1);
	mp2addon.indexNameN1 = byte8;

	fd.read(&byte8, 1);
	mp2addon.quantityN = byte8;

	fd.read(&byte8, 1);
	mp2addon.objectNameN2 = byte8;

	fd.read(&byte8, 1);
	mp2addon.indexNameN2 = byte8;

	fd.read(reinterpret_cast<char *>(&mp2addon.uniqNumberN1), sizeof(u32));
	SWAP32(mp2addon.uniqNumberN1);

	fd.read(reinterpret_cast<char *>(&mp2addon.uniqNumberN2), sizeof(u32));
	SWAP32(mp2addon.uniqNumberN2);

	vec_mp2addons.push_back(mp2addon);
    }

    const u32 endof_addons = fd.tellg();

    if(H2Config::Debug()) Error::Verbose("World::World: read all tiles addons, tellg: ", endof_addons);

    // offset data
    fd.seekg(MP2OFFSETDATA, std::ios_base::beg);

    vec_tiles.resize(width * height);

    // read all tiles
    for(u16 ii = 0; ii < width * height; ++ii)
    {
	MP2::mp2tile_t mp2tile;

	// byte16
	fd.read(reinterpret_cast<char *>(&mp2tile.tileIndex), sizeof(u16));
	SWAP16(mp2tile.tileIndex);

	fd.read(&byte8, 1);
	mp2tile.objectName1 = byte8;

	fd.read(&byte8, 1);
	mp2tile.indexName1 = byte8;

	fd.read(&byte8, 1);
	mp2tile.quantity1 = byte8;

	fd.read(&byte8, 1);
	mp2tile.quantity2 = byte8;

	fd.read(&byte8, 1);
	mp2tile.objectName2 = byte8;

	fd.read(&byte8, 1);
	mp2tile.indexName2 = byte8;

	fd.read(&byte8, 1);
	mp2tile.shape = byte8;

	fd.read(&byte8, 1);
	mp2tile.generalObject = byte8;

	switch(mp2tile.generalObject)
	{
	    case MP2::OBJ_RNDTOWN:
	    case MP2::OBJ_RNDCASTLE:
	    case MP2::OBJ_CASTLE:
	    case MP2::OBJ_HEROES:
	    case MP2::OBJ_SIGN:
	    case MP2::OBJ_BOTTLE:
	    case MP2::OBJ_EVENT:
	    case MP2::OBJ_SPHINX:
		vec_object.push_back(ii);
		break;
	    default:
		break;
	}

	// offset first addon
	fd.read(reinterpret_cast<char *>(&byte16), sizeof(u16));
	SWAP16(byte16);

	// byte32
	fd.read(reinterpret_cast<char *>(&mp2tile.uniqNumber1), sizeof(u32));
	SWAP32(mp2tile.uniqNumber1);

	// byte32
	fd.read(reinterpret_cast<char *>(&mp2tile.uniqNumber2), sizeof(u32));
	SWAP32(mp2tile.uniqNumber2);

	Maps::Tiles * tile = new Maps::Tiles(ii, mp2tile);

	// load all addon for current tils
	while(byte16){

	    if(vec_mp2addons.size() <= byte16){ Error::Warning("World::World: index addons out of range!"); break; }

	    (*tile).AddonsPushLevel1(vec_mp2addons.at(byte16));
	    (*tile).AddonsPushLevel2(vec_mp2addons.at(byte16));

	    byte16 = vec_mp2addons.at(byte16).indexAddon;
	}

	(*tile).AddonsSort();

	vec_tiles[ii] = tile;
    }

    if(H2Config::Debug()) Error::Verbose("World::World: read all tiles, tellg: ", fd.tellg());

    // after addons
    fd.seekg(endof_addons, std::ios_base::beg);

    // cood castles
    // 72 x 3 byte (cx, cy, id)
    for(u8 ii = 0; ii < 72; ++ii)
    {
	char cx, cy, id;

	fd.read(&cx, 1);
	fd.read(&cy, 1);
	fd.read(&id, 1);
	
	// empty block
	if(-1 == cx && -1 == cy) continue;

	switch(id){
	    case 0x00: break; // tower: knight
	    case 0x01: break; // tower: barbarian
	    case 0x02: break; // tower: sorceress
	    case 0x03: break; // tower: warlock
	    case 0x04: break; // tower: wizard
	    case 0x05: break; // tower: necromancer
	    case 0x06: break; // tower: random
	    case 0x80: break; // castle: knight
	    case 0x81: break; // castle: barbarian
	    case 0x82: break; // castle: sorceress
	    case 0x83: break; // castle: warlock
	    case 0x84: break; // castle: wizard
	    case 0x85: break; // castle: necromancer
	    case 0x86: break; // castle: random
	    default:
		Error::Warning("World::World: castle block, unknown id: ", id);
		if(H2Config::Debug()) Error::Verbose("maps index: ", cx + cy * w());
		break;
	}
    }

    if(H2Config::Debug()) Error::Verbose("World::World: read coord castles, tellg: ", fd.tellg());

    // cood resource kingdoms
    // 144 x 3 byte (cx, cy, id)
    for(u8 ii = 0; ii < 144; ++ii)
    {
	char cx, cy, id;

	fd.read(&cx, 1);
	fd.read(&cy, 1);
	fd.read(&id, 1);
	
	// empty block
	if(-1 == cx && -1 == cy) continue;

	switch(id){
	    case 0x00: break; // mines: wood
	    case 0x01: break; // mines: mercury
 	    case 0x02: break; // mines: ore
	    case 0x03: break; // mines: sulfur
	    case 0x04: break; // mines: crystal
	    case 0x05: break; // mines: gems
	    case 0x06: break; // mines: gold
	    case 0x64: break; // lighthouse
	    case 0x65: break; // dragon city
	    case 0x67: break; // abandoned mines
	    default:
		Error::Warning("World::World: kingdom block, unknown id: ", id);
		if(H2Config::Debug()) Error::Verbose("maps index: ", cx + cy * w());
		break;	
	}
    }

    if(H2Config::Debug()) Error::Verbose("World::World: read coord other resource, tellg: ", fd.tellg());

    // unknown byte
    char unk_byte;
    fd.read(&unk_byte, 1);
    if(4 < H2Config::Debug() && unk_byte)
	printf("World::World: dump unknown byte: %hhX\n", unk_byte);

    if(4 < H2Config::Debug())
	    printf("World::World: dump final block: ");

    // count final mp2 blocks
    u16 countblock = 0;
    byte16 = 0xffff;
    while(byte16)
    {
	// debug endof mp2
	if(endof_mp2 < fd.tellg()) Error::Except("World::World: read maps: out of range.");

	fd.read(reinterpret_cast<char *>(&byte16), sizeof(u16));
	SWAP16(byte16);

	if(4 < H2Config::Debug())
	    printf("%hX", byte16);

	if(byte16) countblock = byte16 - 1;
    }

    if(4 < H2Config::Debug())
	printf("\n");

    if(H2Config::Debug()) Error::Verbose("World::World: read find final mp2 blocks, tellg: ", fd.tellg());

    // castle or heroes or (events, rumors, etc)
    for(u16 ii = 0; ii < countblock; ++ii)
    {
	// debug
	if(endof_mp2 < fd.tellg()) Error::Except("World::World: read maps: out of range.");

	// size block
	u16 sizeblock;
	fd.read(reinterpret_cast<char *>(&sizeblock), sizeof(u16));
	SWAP16(sizeblock);

	char *pblock = new char[sizeblock];

	// read block
	fd.read(pblock, sizeblock);

	std::vector<u16>::const_iterator it_index = vec_object.begin();
	bool findobject = false;

	while(it_index != vec_object.end())
	{
	    const Maps::Tiles & tile = *vec_tiles.at(*it_index);

	    // orders(quantity2, quantity1)
	    u16 orders = (tile.GetQuantity2() ? tile.GetQuantity2() : 0);
	    orders <<= 8;
	    orders |= static_cast<u16>(tile.GetQuantity1());
	    
	    if(orders && !(orders % 0x08) && (ii + 1 == orders / 0x08)){ findobject = true; break; }

	    ++it_index;
	}

	if(findobject)
	{
	    Maps::Tiles & tile = *vec_tiles.at(*it_index);
	    const Maps::TilesAddon *addon = NULL;

	    switch(tile.GetObject())
	    {
		case MP2::OBJ_CASTLE:
		    // add castle
		    if(SIZEOFMP2CASTLE != sizeblock) Error::Warning("World::World: read castle: incorrect size block.");
		    else
		    vec_castles.push_back(new Castle(tile.GetUniq1(), *it_index, pblock));
		    break;
		case MP2::OBJ_RNDTOWN:
		case MP2::OBJ_RNDCASTLE:
		    // add rnd castle
		    if(SIZEOFMP2CASTLE != sizeblock) Error::Warning("World::World: read castle: incorrect size block.");
		    else
		    vec_castles.push_back(new Castle(tile.GetUniq1(), *it_index, pblock, true));
		    break;
		case MP2::OBJ_HEROES:
		    // add heroes
		    if(SIZEOFMP2HEROES != sizeblock) Error::Warning("World::World: read heroes: incorrect size block.");
		    else
		    if(NULL != (addon = tile.FindMiniHero()))
		    {
			// calculate color
			const u8 index_name = addon->index;
			Color::color_t color = Color::GRAY;

			if( 7 > index_name)
    			    color = Color::BLUE;
			else
			if(14 > index_name)
    			    color = Color::GREEN;
			else
	    		if(21 > index_name)
		    	    color = Color::RED;
			else
			if(28 > index_name)
			    color = Color::YELLOW;
			else
			if(35 > index_name)
			    color = Color::ORANGE;
			else
			    color = Color::PURPLE;

			Kingdom & kingdom = GetKingdom(color);

			// caclulate race
			Race::race_t race = Race::BOMG;
			switch(index_name % 7)
			{
			    case 0: race = Race::KNGT; break;
			    case 1: race = Race::BARB; break;
			    case 2: race = Race::SORC; break;
			    case 3: race = Race::WRLK; break;
			    case 4: race = Race::WZRD; break;
			    case 5: race = Race::NECR; break;
			    case 6: race = (Color::GRAY != color ? Settings::Get().FileInfo().KingdomRace(color) : Race::Rand()); break;
			}

			// check heroes max count
			if(kingdom.GetHeroes().size() < KINGDOMMAXHEROES)
			{
			    const Heroes * hero = NULL;

			    hero = (pblock[17] &&
				    pblock[18] < Heroes::BAX &&
				    static_cast<u8>(pblock[18]) < vec_heroes.size() &&
				    vec_heroes.at(pblock[18])->isFreeman() ?
				    vec_heroes[pblock[18]] : GetFreemanHeroes(race));

			    if(hero)
			    {
				Heroes * herow = const_cast<Heroes *>(hero);
			    	(*herow).LoadFromMP2(*it_index, pblock, color);
			    	kingdom.AddHeroes(herow);
			    }
			}
			else
			    Error::Warning("World::LoadMaps: load heroes maximum");
		    }
		    break;
		case MP2::OBJ_SIGN:
		case MP2::OBJ_BOTTLE:
		    // add sign or buttle
		    if(SIZEOFMP2SIGN - 1 < sizeblock && 0x01 == pblock[0])
			map_sign[*it_index] = std::string(&pblock[9]);
		    break;
		case MP2::OBJ_EVENT:
		    // add event maps
		    if(SIZEOFMP2EVENT - 1 < sizeblock && 0x01 == pblock[0])
				    vec_eventsmap.push_back(new GameEvent::Coord(*it_index, pblock));
		    break;
		case MP2::OBJ_SPHINX:
		    // add riddle sphinx
		    if(SIZEOFMP2RIDDLE - 1 < sizeblock && 0x00 == pblock[0])
				    vec_riddles.push_back(new GameEvent::Riddle(*it_index, pblock));
		    break;
		default:
		    break;
	    }
	}
	// other events
	else 
	if(0x00 == pblock[0])
	{
	    // add event day
	    if(SIZEOFMP2EVENT - 1 < sizeblock && 1 == pblock[42])
		vec_eventsday.push_back(new GameEvent::Day(pblock));

	    // add rumors
	    else if(SIZEOFMP2RUMOR - 1 < sizeblock)
	    {
		if(pblock[8])
		{
		    vec_rumors.push_back(&pblock[8]);
		    if(H2Config::Debug()) Error::Verbose("add Rumors: " + vec_rumors.back());
		}
	    }
	}
	// debug
	else Error::Warning("World::World: read maps: unknown block addons, size: ", sizeblock);

	delete [] pblock;
    }

    // last rumors
    vec_rumors.push_back("This game is now in pre alpha development version. ;)");

    // close mp2
    fd.close();

    // modify other objects
    const u16 vec_size = vec_tiles.size();

    for(u16 ii = 0; ii < vec_size; ++ii)
    {
	Maps::Tiles & tile = *vec_tiles[ii];
	const Maps::TilesAddon *addon = NULL;

	switch(tile.GetObject())
	{
	    case MP2::OBJ_WITCHSHUT:
		map_witchshut[ii] = Skill::Secondary::RandForWitchsHut();
		break;

	    case MP2::OBJ_SHRINE1:
		map_shrine[ii] = Rand::Get(10) % 2 ? Spell::RandCombat(1) : Spell::RandAdventure(1);
		break;
	    case MP2::OBJ_SHRINE2:
		map_shrine[ii] = Rand::Get(10) % 2 ? Spell::RandCombat(2) : Spell::RandAdventure(2);
		break;
	    case MP2::OBJ_SHRINE3:
		map_shrine[ii] = Rand::Get(10) % 2 ? Spell::RandCombat(3) : Spell::RandAdventure(3);
		break;

	    case MP2::OBJ_STONELIGHTS:
		vec_teleports.push_back(ii);
		break;

	    case MP2::OBJ_EVENT:
		// remove event sprite
		if(NULL != (addon = tile.FindEvent()))
		{
		    tile.Remove(addon->uniq);
		}
		break;
	
    	    case MP2::OBJ_RNDARTIFACT:
    	    case MP2::OBJ_RNDARTIFACT1:
    	    case MP2::OBJ_RNDARTIFACT2:
    	    case MP2::OBJ_RNDARTIFACT3:
		// modify rnd artifact sprite
		Artifact::ChangeTileWithRNDArtifact(tile);
		break;

    	    case MP2::OBJ_RNDULTIMATEARTIFACT:
		// remove ultimate artifact sprite
		if(NULL != (addon = tile.FindUltimateArtifact()))
		{
		    tile.Remove(addon->uniq);
		    tile.SetObject(MP2::OBJ_ZERO);
		    ultimate_artifact = ii;
		}
		break;

    	    case MP2::OBJ_BOAT:
		// remove small sprite boat
		if(NULL != (addon = tile.FindBoat()))
		{
		    tile.Remove(addon->uniq);
		}
		break;

	    case MP2::OBJ_TREASURECHEST:
		if(Maps::Ground::WATER == tile.GetGround())
		{
		    switch(Rand::Get(1, 10))
		    {
			// 70% - 15 * 100 gold
			default:
			    tile.SetQuantity2(15);
			    break;

			// 20% - empty
			case 7:
			case 8:
			    break;

			// 10% - 10 * 100 gold + art
			case 10:
			    tile.SetQuantity1(Artifact::Rand1());
			    tile.SetQuantity2(10);
			    break;
		    }
		}
		else
		{
		    switch(Rand::Get(1, 20))
		    {
			// 32% - 20 * 100 gold or 1500 exp
			default:
			    tile.SetQuantity2(20);
			    break;

			// 31% - 15 * 100 gold or 1000 exp
			case 2:
			case 5:
			case 8:
			case 11:
			case 14:
			case 17:
			    tile.SetQuantity2(15);
			    break;

			// 31% - 10 * 100 gold or 500 exp
			case 3:
			case 6:
			case 9:
			case 12:
			case 15:
			case 18:
			    tile.SetQuantity2(10);
			    break;

			// 10% - art
			case 20:
			    tile.SetQuantity1(Artifact::Rand1());
			    break;
		    }
		}
		break;

	    case MP2::OBJ_RESOURCE:
		tile.SetQuantity2(Rand::Get(RNDRESOURCEMIN, RNDRESOURCEMAX));
		break;

	    case MP2::OBJ_RNDRESOURCE:
		// modify rnd resource sprite
		Resource::ChangeTileWithRNDResource(tile);
		tile.SetQuantity2(Rand::Get(RNDRESOURCEMIN, RNDRESOURCEMAX));
		break;

	    case MP2::OBJ_RNDMONSTER:
	    case MP2::OBJ_RNDMONSTER1:
	    case MP2::OBJ_RNDMONSTER2:
	    case MP2::OBJ_RNDMONSTER3:
	    case MP2::OBJ_RNDMONSTER4:
		// modify rnd monster sprite
		Monster::ChangeTileWithRNDMonster(vec_tiles, ii);
		break;

	    default:
		break;
	}
    }

    AGG::FreeObject(TIL::GROUND32);

    // sort castles to kingdoms
    for(u8 ii = 0; ii < vec_kingdoms.size(); ++ii)
	if((*vec_kingdoms[ii]).isPlay()) for(u16 cc = 0; cc < vec_castles.size(); ++cc)
	    if((*vec_kingdoms[ii]).GetColor() == (*vec_castles[cc]).GetColor())
		(*vec_kingdoms[ii]).AddCastle(vec_castles[cc]);

    // play with debug hero
    if(H2Config::Debug())
    {
	// get first castle position
	Kingdom & kingdom = GetMyKingdom();

	if(kingdom.GetCastles().size())
	{
	    const Castle & castle = *(kingdom.GetCastles().at(0));
	    const Heroes *hero = vec_heroes[Heroes::SANDYSANDY];

	    // place hero
	    if(hero && (*hero).isFreeman())
	    {
		const_cast<Heroes &>(*hero).Recruit(castle);

		kingdom.AddHeroes(const_cast<Heroes *>(hero));
	    }
	}
    }
    else
    // play with hero
    if(Settings::Get().FileInfo().PlayWithHeroes())
	for(u8 ii = 0; ii < vec_kingdoms.size(); ++ii)
	    if((*vec_kingdoms[ii]).isPlay() && (*vec_kingdoms[ii]).GetCastles().size())
	    {
		// get first castle position
		Kingdom & kingdom = *(vec_kingdoms[ii]);
		const Castle & castle = *(kingdom.GetCastles().at(0));

		// place hero
		if(const Heroes *hero = GetFreemanHeroes(castle.GetRace()))
		{
		    const_cast<Heroes &>(*hero).Recruit(castle);

		    kingdom.AddHeroes(const_cast<Heroes *>(hero));
		}
	    }

    if(H2Config::Debug()) Error::Verbose("World::LoadMaps: end load.");
}

/* get human kindom */
Kingdom & World::GetMyKingdom(void)
{ return GetKingdom(Settings::Get().MyColor()); }

const Kingdom & World::GetMyKingdom(void) const
{ return GetKingdom(Settings::Get().MyColor()); }

/* get kingdom */
Kingdom & World::GetKingdom(Color::color_t color)
{
    switch(color)
    {
        case Color::BLUE:       return *vec_kingdoms[0]; break;
        case Color::GREEN:      return *vec_kingdoms[1]; break;
        case Color::RED:        return *vec_kingdoms[2]; break;
        case Color::YELLOW:     return *vec_kingdoms[3]; break;
        case Color::ORANGE:     return *vec_kingdoms[4]; break;
        case Color::PURPLE:     return *vec_kingdoms[5]; break;
        case Color::GRAY:       break;
    }

    Error::Warning("World::GetKingdom: return Color::GRAY.");

    return *vec_kingdoms[0];
}

const Kingdom & World::GetKingdom(Color::color_t color) const
{
    switch(color)
    {
        case Color::BLUE:       return *vec_kingdoms[0]; break;
        case Color::GREEN:      return *vec_kingdoms[1]; break;
        case Color::RED:        return *vec_kingdoms[2]; break;
        case Color::YELLOW:     return *vec_kingdoms[3]; break;
        case Color::ORANGE:     return *vec_kingdoms[4]; break;
        case Color::PURPLE:     return *vec_kingdoms[5]; break;
        case Color::GRAY:       break;
    }

    Error::Warning("World::GetKingdom: return Color::GRAY.");

    return *vec_kingdoms[0];
}

/* get castle from index maps */
const Castle * World::GetCastle(u16 maps_index)
{
    return GetCastle(maps_index % width, maps_index / height);
}

/* get castle from coord maps */
const Castle * World::GetCastle(u8 ax, u8 ay)
{
    std::vector<Castle *>::const_iterator it1 = vec_castles.begin();
    std::vector<Castle *>::const_iterator it2 = vec_castles.end();

    for(; it1 != it2; ++it1)
        if(*it1 && (*it1)->ContainCoord(ax, ay)) return *it1;

    Error::Warning("World::GetCastle: return NULL pointer");

    return NULL;
}

/* get heroes from index maps */
const Heroes * World::GetHeroes(u16 maps_index) const
{
    return GetHeroes(maps_index % width, maps_index / height);
}

/* get heroes from coord maps */
const Heroes * World::GetHeroes(u8 ax, u8 ay) const
{
    std::vector<Heroes *>::const_iterator it1 = vec_heroes.begin();
    std::vector<Heroes *>::const_iterator it2 = vec_heroes.end();

    for(; it1 != it2; ++it1)
        if(*it1 && (*it1)->GetCenter().x == ax && (*it1)->GetCenter().y == ay) return *it1;

    return NULL;
}

/* end day */
void World::NextDay(void)
{
    ++day;

    if(!(day % DAYOFWEEK))
    {
        ++week;
    
        if(!(week % WEEKOFMONTH)) ++month;
    }
    
    begin_week = (day % DAYOFWEEK ? false : true);
            
    begin_month = (begin_week && !(week % WEEKOFMONTH) ? true : false);

    // action new day
    for(u8 ii = 0; ii < vec_kingdoms.size(); ++ii) if((*vec_kingdoms[ii]).isPlay()) (*vec_kingdoms[ii]).ActionNewDay();

    // action new week
    if(begin_week)
    {
        for(u8 ii = 0; ii < vec_kingdoms.size(); ++ii) if((*vec_kingdoms[ii]).isPlay()) (*vec_kingdoms[ii]).ActionNewWeek();
        
        NewWeek();
    }
    
    // action new month
    if(begin_month)
    {
        for(u8 ii = 0; ii < vec_kingdoms.size(); ++ii) if((*vec_kingdoms[ii]).isPlay()) (*vec_kingdoms[ii]).ActionNewMonth();
        
        NewMonth();
    }
}

void World::NewWeek(void)
{
    // change free recruit hero
    const Race::race_t & rc = Settings::Get().FileInfo().KingdomRace(Settings::Get().MyColor());
    free_recruit_hero1 = GetFreemanHeroes(rc)->GetHeroes();
    free_recruit_hero2 = GetFreemanHeroes()->GetHeroes();
}

void World::NewMonth(void)
{
}

void World::FreeOldMaps(void)
{
    // maps tiles
    if(vec_tiles.size())
    {
	std::vector<Maps::Tiles *>::const_iterator it = vec_tiles.begin();
	
	for(; it != vec_tiles.end(); ++it) delete *it;
    }
    vec_tiles.clear();

    // kingdoms
    if(vec_kingdoms.size())
    {
	std::vector<Kingdom *>::const_iterator it = vec_kingdoms.begin();
	
	for(; it != vec_kingdoms.end(); ++it) delete *it;
    }
    vec_kingdoms.clear();

    // event day
    if(vec_eventsday.size())
    {
	std::vector<GameEvent::Day *>::const_iterator it = vec_eventsday.begin();
	
	for(; it != vec_eventsday.end(); ++it) delete *it;
    }
    vec_eventsday.clear();

    // event maps
    if(vec_eventsmap.size())
    {
	std::vector<GameEvent::Coord *>::const_iterator it = vec_eventsmap.begin();
	
	for(; it != vec_eventsmap.end(); ++it) delete *it;
    }
    vec_eventsmap.clear();

    // riddle
    if(vec_riddles.size())
    {
	std::vector<GameEvent::Riddle *>::const_iterator it = vec_riddles.begin();
	
	for(; it != vec_riddles.end(); ++it) delete *it;
    }
    vec_riddles.clear();

    // rumors
    vec_rumors.clear();

    // castles
    if(vec_castles.size())
    {
	std::vector<Castle *>::const_iterator it = vec_castles.begin();
	
	for(; it != vec_castles.end(); ++it) delete *it;
    }
    vec_castles.clear();
    
    // heroes
    if(vec_heroes.size())
    {
	std::vector<Heroes *>::const_iterator it = vec_heroes.begin();
	
	for(; it != vec_heroes.end(); ++it) delete *it;
    }
    vec_heroes.clear();
}

const Heroes::heroes_t & World::GetFreeRecruit1(void)
{
    const Race::race_t & rc = Settings::Get().FileInfo().KingdomRace(Settings::Get().MyColor());

    if(Heroes::UNKNOWN == free_recruit_hero1 || !(*vec_heroes[free_recruit_hero1]).isFreeman()) free_recruit_hero1 = GetFreemanHeroes(rc)->GetHeroes();

    return free_recruit_hero1;
}

const Heroes::heroes_t & World::GetFreeRecruit2(void)
{
    if(Heroes::UNKNOWN == free_recruit_hero2 || !(*vec_heroes[free_recruit_hero2]).isFreeman()) free_recruit_hero2 = GetFreemanHeroes()->GetHeroes();

    while(free_recruit_hero1 == free_recruit_hero2)
    {
	Error::Verbose("World::GetFreeRecruit2: hero1 equal hero2");

	free_recruit_hero2 = GetFreemanHeroes()->GetHeroes();
    }

    return free_recruit_hero2;
}

const Heroes * World::GetFreemanHeroes(Race::race_t rc)
{
    u8 min = 0;
    u8 max = 0;

    switch(rc)
    {
	case Race::KNGT:
	    min = 0;
	    max = 8;
	    break;
	
	case Race::BARB:
	    min = 9;
	    max = 17;
	    break;
	
	case Race::SORC:
	    min = 18;
	    max = 26;
	    break;
	    
	case Race::WRLK:
	    min = 27;
	    max = 35;
	    break;
	
	case Race::WZRD:
	    min = 36;
	    max = 44;
	    break;
	
	case Race::NECR:
	    min = 45;
	    max = 53;
	    break;
	
	default:
	    min = 0;
	    max = 53;
	    break;
    }

    std::vector<Heroes::heroes_t> freeman_heroes;
    
    // find freeman in race
    if(Race::BOMG != rc)
	for(u8 ii = min; ii <= max; ++ii)
	    if((*vec_heroes[ii]).isFreeman()) freeman_heroes.push_back((*vec_heroes[ii]).GetHeroes());

    // not found, find other race
    if(Race::BOMG == rc || freeman_heroes.empty())
	for(u8 ii = 0; ii <= 53; ++ii)
	    if((*vec_heroes[ii]).isFreeman()) freeman_heroes.push_back((*vec_heroes[ii]).GetHeroes());

    // not found, all heroes busy
    if(freeman_heroes.empty())
    {
	Error::Warning("World::GetFreemanHeroes: freeman not found, all heroes busy.");

	return NULL;
    }

    return vec_heroes[ freeman_heroes[Rand::Get(freeman_heroes.size() - 1)] ];
}

const std::string & World::GetRumors(void)
{
    return vec_rumors[Rand::Get(vec_rumors.size() - 1)];
}

/* return spell from shrine circle */
Spell::spell_t World::SpellFromShrine(const u16 index)
{
    return map_shrine[index];
}

/* return random teleport destination */
u16 World::NextTeleport(const u16 index) const
{
    if(vec_teleports.empty() || 1 == vec_teleports.size())
    {
	Error::Warning("World::NextTeleport: is empty.");

	return index;
    }

    u16 result;

    while((result = Rand::Get(vec_teleports.size() - 1)) == index) result = Rand::Get(vec_teleports.size() - 1);

    // check if busy
    return GetHeroes(vec_teleports[result]) ? index : vec_teleports[result];
}

/* return skill from witchs hut */
Skill::Secondary::skill_t World::SkillFromWitchsHut(const u16 index)
{
    return map_witchshut[index];
}

/* return message from sign */
const std::string & World::MessageSign(const u16 index)
{
    return map_sign[index];
}

/* return count captured object */
u16 World::CountCapturedObject(const MP2::object_t obj, const Color::color_t col) const
{
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it1 = map_captureobj.begin();
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it2 = map_captureobj.end();

    u16 result = 0;

    for(; it1 != it2; ++it1) if((*it1).second.first == obj && (*it1).second.second == col) ++result;

    return result;
}

/* return count captured mines */
u16 World::CountCapturedMines(const Resource::resource_t res, const Color::color_t col) const
{
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it1 = map_captureobj.begin();
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it2 = map_captureobj.end();

    u16 result = 0;

    for(; it1 != it2; ++it1)
	if(MP2::OBJ_MINES == (*it1).second.first || MP2::OBJ_HEROES == (*it1).second.first)
    {
	    // scan for find mines
	    const Maps::TilesAddon * addon = GetTiles((*it1).first).FindMines();

	    if(addon)
	    {
		// index sprite EXTRAOVR
		if(0 == addon->index && Resource::ORE == res && (*it1).second.second == col) ++result;
		else
		if(1 == addon->index && Resource::SULFUR == res && (*it1).second.second == col) ++result;
		else
		if(2 == addon->index && Resource::CRYSTAL == res && (*it1).second.second == col) ++result;
		else
		if(3 == addon->index && Resource::GEMS == res && (*it1).second.second == col) ++result;
		else
		if(4 == addon->index && Resource::GOLD == res && (*it1).second.second == col) ++result;
	    }
    }
    
    return result;
}

/* capture object */
void World::CaptureObject(const u16 index, const Color::color_t col)
{
    MP2::object_t obj = GetTiles(index).GetObject();
    
    if(MP2::OBJ_HEROES == obj)
    {
	const Heroes * hero = GetHeroes(index);
	if(NULL == hero) return;

	obj = hero->GetUnderObject();
    }

    map_captureobj[index].first = obj;
    map_captureobj[index].second = col;
}

/* return color captured object */
Color::color_t World::ColorCapturedObject(const u16 index) const
{
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it1 = map_captureobj.begin();
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it2 = map_captureobj.end();

    for(; it1 != it2; ++it1) if((*it1).first == index) return (*it1).second.second;

    return Color::GRAY;
}

void World::ClearFog(const u8 color)
{
    // clear abroad castles
    if(vec_castles.size())
    {
        std::vector<Castle *>::const_iterator it1 = vec_castles.begin();
        std::vector<Castle *>::const_iterator it2 = vec_castles.end();

        for(; it1 != it2; ++it1) if(*it1 && color & (**it1).GetColor()) Maps::ClearFog((**it1).GetCenter(), 5, color);
    }

    // clear adboar heroes
    if(vec_heroes.size())
    {
        std::vector<Heroes *>::const_iterator it1 = vec_heroes.begin();
        std::vector<Heroes *>::const_iterator it2 = vec_heroes.end();

        for(; it1 != it2; ++it1) if(*it1 && color & (**it1).GetColor()) (**it1).Scoute();
    }

    // clear abroad objects
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it1 = map_captureobj.begin();
    std::map<u16, std::pair<MP2::object_t, Color::color_t> >::const_iterator it2 = map_captureobj.end();

    for(; it1 != it2; ++it1)
	if(color & (*it1).second.second)
    {
	u8 scoute = 0;

	switch((*it1).second.first)
	{
	    case MP2::OBJ_MINES:
	    case MP2::OBJ_ALCHEMYTOWER:
	    case MP2::OBJ_SAWMILL:	scoute = 2; break;

	    case MP2::OBJ_LIGHTHOUSE:	scoute = 4; break; // FIXME: scoute and lighthouse

	    default: break;
	}

        if(scoute) Maps::ClearFog((*it1).first, scoute, color);
    }
}
