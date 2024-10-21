#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "list_menu.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "text_window.h"
#include "task.h"
#include "graphics.h"
#include "strings.h"
#include "pokemon_icon.h"
#include "constants/songs.h"

struct MoveMenuInfoIcon
{
    u8 width;
    u8 height;
    u16 offset;
};

const struct MoveMenuInfoIcon gMoveMenuInfoIcons[] =
{
{ 12, 12, 0x000 },       // Unused
    { 32, 12, 0x020 },       // Normal icon
    { 32, 12, 0x064 },       // Fight icon
    { 32, 12, 0x060 },       // Flying icon
    { 32, 12, 0x080 },       // Poison icon
    { 32, 12, 0x048 },       // Ground icon
    { 32, 12, 0x044 },       // Rock icon
    { 32, 12, 0x06C },       // Bug icon
    { 32, 12, 0x068 },       // Ghost icon
    { 32, 12, 0x088 },       // Steel icon
    { 32, 12, 0x0A4 },       // ??? (Mystery) icon
    { 32, 12, 0x024 },       // Fire icon
    { 32, 12, 0x028 },       // Water icon
    { 32, 12, 0x02C },       // Grass icon
    { 32, 12, 0x040 },       // Electric icon
    { 32, 12, 0x084 },       // Psychic icon
    { 32, 12, 0x04C },       // Ice icon
    { 32, 12, 0x0A0 },       // Dragon icon
    { 32, 12, 0x08C },       // Dark icon
	// 3 Free Spaces
    { 40, 12, 0x0C5 },       // -Type- icon
    { 40, 12, 0x0CA },       // -Power- icon
    { 40, 12, 0x0E0 },       // -Accuracy- icon
    { 40, 12, 0x0E5 },       // -PP- icon
    { 40, 12, 0x0EC },       // -Effect- icon
	//Spanish
    { 32, 12, 0x120 },       // Normal icon
    { 32, 12, 0x164 },       // Fight icon
    { 32, 12, 0x160 },       // Flying icon
    { 32, 12, 0x180 },       // Poison icon
    { 32, 12, 0x148 },       // Ground icon
    { 32, 12, 0x144 },       // Rock icon
    { 32, 12, 0x16C },       // Bug icon
    { 32, 12, 0x168 },       // Ghost icon
    { 32, 12, 0x188 },       // Steel icon
    { 32, 12, 0x1A4 },       // ??? (Mystery) icon
    { 32, 12, 0x124 },       // Fire icon
    { 32, 12, 0x128 },       // Water icon
    { 32, 12, 0x12C },       // Grass icon
    { 32, 12, 0x140 },       // Electric icon
    { 32, 12, 0x184 },       // Psychic icon
    { 32, 12, 0x14C },       // Ice icon
    { 32, 12, 0x1A0 },       // Dragon icon
    { 32, 12, 0x18C },       // Dark icon
	// 3 Free Spaces
    { 40, 12, 0x1C5 },       // -Type- icon
    { 40, 12, 0x1CA },       // -Power- icon
    { 40, 12, 0x1E0 },       // -Accuracy- icon
    { 40, 12, 0x1E5 },       // -PP- icon
    { 40, 12, 0x1EC },       // -Effect- icon
	//Category Icons
    { 32, 16, 0x004 },       // Physical icon
    { 32, 16, 0x008 },       // Special icon
    { 32, 16, 0x00C },       // Status icon
	//Category Icons Mini
    { 16, 16, 0x100 },       // Physical icon
    { 16, 16, 0x102 },       // Special icon
    { 16, 16, 0x104 },       // Status icon
};
const struct MoveMenuInfoIcon gMoveMenuInfoIconsBattle[] =
{
    { 32, 8,  0 },       // Normal icon
    { 32, 8, 36 },       // Fight icon
    { 32, 8, 32 },       // Flying icon
    { 32, 8, 48 },       // Poison icon
    { 32, 8, 24 },       // Ground icon
    { 32, 8, 20 },       // Rock icon
    { 32, 8, 44 },       // Bug icon
    { 32, 8, 40 },       // Ghost icon
    { 32, 8, 56 },       // Steel icon
    { 32, 8, 68 },       // ??? (Mystery) icon
    { 32, 8,  4 },       // Fire icon
    { 32, 8,  8 },       // Water icon
    { 32, 8, 12 },       // Grass icon
    { 32, 8, 16 },       // Electric icon
    { 32, 8, 52 },       // Psychic icon
    { 32, 8, 28 },       // Ice icon
    { 32, 8, 64 },       // Dragon icon
    { 32, 8, 60 },       // Dark icon
	// 2 Free Spaces
	//Spanish
    { 32, 8,  80 },       // Normal icon
    { 32, 8, 116 },       // Fight icon
    { 32, 8, 112 },       // Flying icon
    { 32, 8, 128 },       // Poison icon
    { 32, 8, 104 },       // Ground icon
    { 32, 8, 100 },       // Rock icon
    { 32, 8, 124 },       // Bug icon
    { 32, 8, 120 },       // Ghost icon
    { 32, 8, 136 },       // Steel icon
    { 32, 8, 148 },       // ??? (Mystery) icon
    { 32, 8,  84 },       // Fire icon
    { 32, 8,  88 },       // Water icon
    { 32, 8,  92 },       // Grass icon
    { 32, 8,  96 },       // Electric icon
    { 32, 8, 132 },       // Psychic icon
    { 32, 8, 108 },       // Ice icon
    { 32, 8, 144 },       // Dragon icon
    { 32, 8, 140 },       // Dark icon
	// 2 Free Spaces
};
