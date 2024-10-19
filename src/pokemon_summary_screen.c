#include "global.h"
#include "gflib.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "help_system.h"
#include "task.h"
#include "menu_helpers.h"
#include "link.h"
#include "overworld.h"
#include "graphics.h"
#include "constants/songs.h"
#include "strings.h"
#include "new_menu_helpers.h"
#include "menu.h"
#include "constants/species.h"
#include "constants/items.h"
#include "data.h"
#include "item.h"
#include "constants/party_menu.h"
#include "trade.h"
#include "battle_main.h"
#include "scanline_effect.h"
#include "constants/moves.h"
#include "dynamic_placeholder_text_util.h"
#include "constants/region_map_sections.h"
#include "region_map.h"
#include "field_specials.h"
#include "party_menu.h"
#include "constants/battle.h"
#include "event_data.h"
#include "trainer_pokemon_sprites.h"
#include "battle_anim.h"
#include "pokeball.h"
#include "pokemon_icon.h"
#include "battle_interface.h"
#include "mon_markings.h"
#include "pokemon_storage_system.h"

// Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

// Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

// needs conflicting header to match (curIndex is s8 in the function, but has to be defined as u8 here)
extern s16 SeekToNextMonInBox(struct BoxPokemon * boxMons, u8 curIndex, u8 maxIndex, u8 flags);

static void sub_8138B8C(struct Pokemon * mon);
static void sub_8135C34(void);
static void PSS_PlayMonCry(void);
static void PSS_RemoveAllWindows(u8 curPageIndex);
static void sub_8134BAC(u8 taskId);
static void sub_8134E84(u8 taskId);
static void sub_813B3F0(u8 taskId);
static void sub_813B120(u8, s8);
static void sub_8137E64(u8 taskId);
static void sub_8135638(void);
static void sub_81356EC(void);
static void sub_8138280(u8 curPageIndex);
static void sub_8137D28(u8 curPageIndex);
static void sub_8135514(void);
static u8 sub_81357A0(u8);
static void PSS_SetHelpContext(void);
static void sub_8138CD8(u8 taskId);
static void sub_8135AA4(void);
static void sub_8135B90(void);
static void sub_8138538(void);
static void sub_8137D28(u8 curPageIndex);
static void PSS_AddTextToWin3(void);
static void PSS_AddTextToWin4(void);
static void PSS_AddTextToWin5(void);
static void PSS_DrawMonMoveIcon(void);
static void PSS_AddTextToWin0(const u8 * str);
static void PSS_AddTextToWin1(const u8 * str);
static void PSS_AddTextToWin2(const u8 * str);
static void PSS_SetInvisibleHpBar(u8 invisible);
static void PSS_SetInvisibleExpBar(u8 invisible);
static void PSS_SetInvisibleMonSprite(u8 invisible);
static void PSS_SetInvisibleMarkings(u8 invisible);
static void PSS_SetInvisiblePokeball(u8 invisible);
static void PSS_SetInvisibleIconStatus(u8 invisible);
static void sub_813ACF8(u8 invisible);
static void sub_813AEB0(u8 invisible);
static void sub_813A0E8(u8 invisible);
static void sub_8139EE4(u8 invisible);
static void PSS_TurnOffScreen(void);
static void PSS_InitDisplay(void);
static void PSS_ResetAll(void);
static u8 PSS_LoadTilesAndMap(void);
static u8 PSS_LoadSpritesData(void);
static void PSS_InitWindows(void);
static u8 sub_8136040(void);
static void sub_8137E28(void);
static void sub_81381C0(void);
static void sub_8136D54(void);
static void PSS_GetDataPokemon(void);
static void PSS_GetStatsPokemon(void);
static void sub_81367B0(void);
static u8 PSS_CheckMonStatus(u32 status);
static void PSS_GetMoveNames(u8);
static u16 PSS_GetMonMove(struct Pokemon * mon, u8 moveSlot);
static u16 PSS_GetMovePP(struct Pokemon * mon, u8 moveSlot);
static void PSS_LoadShinyIndicator(u16, u16);
static void PSS_LoadUnkPoint(u16, u16);
static void PSS_LoadMarkings(void);
static void PSS_LoadMovesSelectCursor(u16, u16);
static void PSS_LoadIconStatus(u16, u16);
static void PSS_LoadHpBar(u16, u16);
static void PSS_LoadExpBar(u16, u16);
static void PSS_LoadPokeball(void);
static void PSS_LoadMonIcon(void);
static void PSS_LoadMonSprite(void);
static void sub_81393D4(u8 taskId);
static void sub_8137EE8(void);
static void PSS_ShowInfoPokemon(void);
static void PSS_ShowMonStats(void);
static void PSS_PrintMoveNamesOrCancel(void);
static void PSS_PrintMoveNamesAndPP(u8 i);
static void PSS_CheckIfMonIsEgg(void);
static void PSS_PrintExpPointAndNextLvTexts(void);
static void PSS_ShowAttackInfo(void);
static void PSS_ShowEggInfo(void);
static void PSS_ShowMonInfo(void);
static bool32 sub_813B838(u8 metLocation);
static bool32 sub_8138B4C(void);
static bool32 PSS_IsNatureBoldOrGentle(u8 nature);
static void PSS_ShowTrainerMemo(void);
static bool32 PSS_IsMonFromGenIII(void);
static u8 sub_813847C(struct Pokemon * mon);
static void PSS_PrintAbilityNameAndDescription(void);
static void PSS_DrawMoveIcon(void);
static void sub_813AF90(void);
static void sub_81380F0(void);
static void sub_81390B0(void);
static void sub_81391EC(void);
static void sub_8139328(struct Pokemon * mon);
static void sub_8139AAC(u16 spriteId);
static void sub_813A124(struct Sprite * sprite);
static void PSS_ShowIconStatus(void);
static void PSS_SetupHpBar(void);
static void PSS_SetupExpBar(void);
static void sub_813ACB4(void);
static void sub_813AF50(void);
static void PSS_UnloadMarkings(void);
static void PSS_GetMarkings(void);
static s8 sub_813B20C(s8);
static s8 sub_813B38C(s8);
static void PSS_ScrollPSSBackground(void);

struct PokemonSummaryScreenData
{
    u8 window[7];

    u8 spriteId_0;
    u8 spriteId_1;
    u8 spriteId_2;

    u8 task;
    u8 unk301C;

    u8 unk3020;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 nickname[POKEMON_NAME_LENGTH + 1];
        u8 specieName[POKEMON_NAME_LENGTH + 1];
        u8 ot_name[12];
        u8 unk304C[2][12];

        u8 dexNum[5];
        u8 ot_id[7];
        u8 heldItem[ITEM_NAME_LENGTH + 1];

        u8 genderSymbol[3];
        u8 level[7];
        u8 unk3090[9];
        u8 unk309C[5][5];

        u8 unk30B8[5][11];
        u8 unk30F0[5][11];
        u8 moveName[5][MOVE_NAME_LENGTH + 1];
        u8 power[5][5];
        u8 unk3188[5][5];

        u8 unk31A4[9];
        u8 unk31B0[9];

        u8 abilityName[13];
        u8 abilityDescription[52];
    } summary;

    u8 isEgg; /* 0x3200 */
    u8 isBadEgg; /* 0x3204 */

    u8 mode; /* 0x3208 */
    u8 unk320C; /* 0x320C */
    u8 lastIndex; /* 0x3210 */
    u8 curPageIndex; /* 0x3214 */
    u8 unk3218; /* 0x3218 */
    u8 isBoxMon; /* 0x321C */
    u8 typeIcons[2]; /* 0x3220 */

    u8 unk3224; /* 0x3224 */
    u8 unk3228; /* 0x3228 */
    u8 unk322C; /* 0x322C */
    u8 unk3230; /* 0x3230 */

    u8 lockMovesFlag; /* 0x3234 */

    u8 unk3238; /* 0x3238 */
    u8 unk323C; /* 0x323C */
    u8 unk3240; /* 0x3240 */
    u8 unk3244; /* 0x3244 */
    u8 unk3248; /* 0x3248 */
    s16 unk324C; /* 0x324C */

    u16 move[5]; /* 0x3250 */
    u16 currentMove[5]; /* 0x325A */
    u8 unk3264; /* 0x3264 */
    u8 unk3268; /* 0x3268 */

    u8 monStatus; /* 0x326C */

    u8 state3270; /* 0x3270 */
    u8 state3274; /* 0x3274 */
    u8 unk3278; /* 0x3278 */
    u8 state; /* 0x327C */
    u8 unk3280; /* 0x3280 */
    u8 unk3284; /* 0x3284 */
    u8 unk3288; /* 0x3288 */
    u8 unk328C; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite * markingSprite;

    u8 unk3300[2]; /* 0x3300 */
    u8 unk3304[3]; /* 0x3304 */
    u8 unused[200]; /* 0x3304 */
};

struct Struct203B144
{
    u16 unk00;
    u16 unk02;
    u16 tileTag;
    u16 palTag;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;

    u16 unk12[5];
    u16 unk1C[5];

    u16 unk26;
};

struct Struct203B160
{
    struct Sprite * sprites[11]; /* 0x00 */
    u16 cordX[11]; /* 0x2c */
    u16 tileTag; /* 0x42 */
    u16 palTag; /* 0x44 */
};

struct Struct203B15C
{
    struct Sprite * sprites[10]; /* 0x00 */
    u16 cordX[10]; /* 0x28 */
    u16 unk3C; /* 0x3c */
    u16 unk3E; /* 0x3e */
};

struct Struct203B170
{
    u8 unk00; /* 0x00 */
    u8 tileTag; /* 0x04 */
    u8 unk08; /* 0x08 */
};

struct Struct203B148
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
    u16 unk08; /* 0x08 */
};

struct Struct203B158
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B164
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B168
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

static EWRAM_DATA struct PokemonSummaryScreenData * sMonSummaryScreen = NULL;
static EWRAM_DATA struct Struct203B144 * sUnknown_203B144 = NULL;
static EWRAM_DATA struct Struct203B148 * sUnknown_203B148[4] = {};
static EWRAM_DATA struct Struct203B158 * sStatusIconSummaryScreen = NULL;
static EWRAM_DATA struct Struct203B15C * sHpBarSummaryScreen = NULL;
static EWRAM_DATA struct Struct203B160 * sExpBarSummaryScreen = NULL;
static EWRAM_DATA struct Struct203B164 * sUnknown_203B164 = NULL;
static EWRAM_DATA struct Struct203B168 * sUnknown_203B168 = NULL;
static EWRAM_DATA u8 sLastViewedMonIndex = 0;
static EWRAM_DATA u8 sUnknown_203B16D = 0;
static EWRAM_DATA u8 sUnknown_203B16E = 0;
static EWRAM_DATA struct Struct203B170 * sUnknown_203B170 = NULL;

extern const u32 gUnknown_8E9B750[];
extern const u32 gUnknown_8E9B950[];
extern const u32 gUnknown_8E9B598[];
extern const u32 gUnknown_8E9BA9C[];
extern const u32 gUnknown_8E9BBCC[];
extern const u16 gUnknown_8E9B310[];
extern const u32 gUnknown_8E9A460[];

static const u32 sUnknown_84636C0[] = INCBIN_U32("graphics/interface/pokesummary_unk_84636C0.gbapal");
static const u16 sUnknown_84636E0[] = INCBIN_U16("graphics/interface/pokesummary_unk_84636E0.gbapal");
static const u32 sUnknown_8463700[] = INCBIN_U32("graphics/interface/pokesummary_unk_8463700.gbapal");

static const struct OamData sUnknown_846398C =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const union AnimCmd sUnknown_8463994[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_846399C[] = 
{
    ANIMCMD_FRAME(0x20, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sUnknown_84639A4[] =
{
    sUnknown_8463994,
    sUnknown_846399C
};

static const struct OamData sUnknown_84639AC = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const union AnimCmd sUnknown_84639B4[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639BC[] = 
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639C4[] = 
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639CC[] = 
{
    ANIMCMD_FRAME(12, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639D4[] = 
{
    ANIMCMD_FRAME(16, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639DC[] = 
{
    ANIMCMD_FRAME(20, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639E4[] = 
{
    ANIMCMD_FRAME(24, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_84639EC[] = 
{
    ANIMCMD_FRAME(28, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sUnknown_84639F4[] =
{
    sUnknown_84639B4,
    sUnknown_84639BC,
    sUnknown_84639C4,
    sUnknown_84639CC,
    sUnknown_84639D4,
    sUnknown_84639DC,
    sUnknown_84639E4,
    sUnknown_84639EC
};

static const struct OamData sUnknown_8463A14 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const union AnimCmd sUnknown_8463A1C[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A24[] = 
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A2C[] = 
{
    ANIMCMD_FRAME(2, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A34[] = 
{
    ANIMCMD_FRAME(3, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A3C[] = 
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A44[] = 
{
    ANIMCMD_FRAME(5, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A4C[] = 
{
    ANIMCMD_FRAME(6, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A54[] = 
{
    ANIMCMD_FRAME(7, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A5C[] = 
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A64[] = 
{
    ANIMCMD_FRAME(9, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A6C[] = 
{
    ANIMCMD_FRAME(10, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sUnknown_8463A74[] = 
{
    ANIMCMD_FRAME(11, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sUnknown_8463A7C[] =
{
    sUnknown_8463A1C,
    sUnknown_8463A24,
    sUnknown_8463A2C,
    sUnknown_8463A34,
    sUnknown_8463A3C,
    sUnknown_8463A44,
    sUnknown_8463A4C,
    sUnknown_8463A54,
    sUnknown_8463A5C,
    sUnknown_8463A64,
    sUnknown_8463A6C,
    sUnknown_8463A74
};

static const struct OamData sUnknown_8463AEC = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const union AnimCmd sUnknown_8463AF4[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sUnknown_8463AFC[] =
{
    sUnknown_8463AF4
};

static const u16 sUnknown_8463B00[] = INCBIN_U16("graphics/interface/pokesummary_unk_8463B00.gbapal");
static const u32 sUnknown_8463B20[] = INCBIN_U32("graphics/interface/pokesummary_unk_8463B20.4bpp.lz");

static const struct OamData sUnknown_8463B30 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const union AnimCmd sUnknown_8463B38[] = 
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sUnknown_8463B40[] =
{
    sUnknown_8463B38
};

static const u32 sUnknown_8463B88[] = INCBIN_U32( "graphics/interface/pokesummary_unk_8463B88.bin.lz");
static const u32 sUnknown_8463C80[] = INCBIN_U32( "graphics/interface/pokesummary_unk_8463C80.bin.lz");

#include "data/text/nature_names.h"

static const u8 * const sUnknown_8463EC4[] = {
    gText_8419B44,
    gText_8419B7B,
    gText_8419BAE,
    gText_8419BDB,
    gText_8419B44Spa,
    gText_8419B7BSpa,
    gText_8419BAESpa,
    gText_8419BDBSpa,
};

static const u8 * const sUnknown_8463ED4[] = {
    gText_8419A3D,
    gText_8419B18,
    gText_8419A6E,
    gText_8419B18,
    gText_8419AA2,
    gText_8419ADE,
    gText_8419B18,
    gText_8419A3DSpa,
    gText_8419B18Spa,
    gText_8419A6ESpa,
    gText_8419B18Spa,
    gText_8419AA2Spa,
    gText_8419ADESpa,
    gText_8419B18Spa,
};

static const struct BgTemplate SummayScreenBgTemplate[] = 
{
	{ //Text
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 29,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0	   
	},
	{ //Page Showed
	 	.bg = 1,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 30,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 2,
	 	.baseTile = 0
	 },
	 { // BG
	 	.bg = 2,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 31,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 3,
	 	.baseTile = 0
	 },
	 { //Moves
	 	.bg = 3,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 28,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 1,
	 	.baseTile = 0
	 }
};

static const struct WindowTemplate WindowTemplatePokemonInfo[] =
{
    { //PAGE TITLE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 13,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0258
    },
    { //INSTRUCTION
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 0,
        .width = 11,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0272
    },
    { // NICKNAME & LV OR TYPES
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 2,
        .width = 10,
        .height = 4,
        .paletteNum = 6,
        .baseBlock = 0x0288
    }
};

static const struct WindowTemplate sDataMonAndNatureWindowTemplate[] = 
{
    { //Data Pokemon
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 21,
        .height = 10,
        .paletteNum = 6,
        .baseBlock = 0
    },
    { // Nature and Found place
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 12,
        .width = 28,
        .height = 8,
        .paletteNum = 6,
        .baseBlock = 220
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMonStatsAndAbilityWindowTemplate[] = 
{
    { // Pokemon Stats
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 17,
        .height = 13,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Exp Point & Next Lv
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { // Ability & Description
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 29,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 292
    },
    { // DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMovesInfoWindowTemplate[] = 
{
    { //Move Names
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 2,
        .width = 10,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Move Data & Description
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 15,
        .height = 14,
        .paletteNum = 6,
        .baseBlock = 181
    },
    { //Move Icons
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 5,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 452
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sUnknown_8463F9C[] = 
{
    {
        .bg = 255,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0x0000
    },
};

static const s8 sPSSNatureStatTable[][5] =
{
    // Atk Def Spd Sp.Atk Sp.Def
    {    0,  0,  0,     0,     0}, // Hardy
    {   +1, -1,  0,     0,     0}, // Lonely
    {   +1,  0, -1,     0,     0}, // Brave
    {   +1,  0,  0,    -1,     0}, // Adamant
    {   +1,  0,  0,     0,    -1}, // Naughty
    {   -1, +1,  0,     0,     0}, // Bold
    {    0,  0,  0,     0,     0}, // Docile
    {    0, +1, -1,     0,     0}, // Relaxed
    {    0, +1,  0,    -1,     0}, // Impish
    {    0, +1,  0,     0,    -1}, // Lax
    {   -1,  0, +1,     0,     0}, // Timid
    {    0, -1, +1,     0,     0}, // Hasty
    {    0,  0,  0,     0,     0}, // Serious
    {    0,  0, +1,    -1,     0}, // Jolly
    {    0,  0, +1,     0,    -1}, // Naive
    {   -1,  0,  0,    +1,     0}, // Modest
    {    0, -1,  0,    +1,     0}, // Mild
    {    0,  0, -1,    +1,     0}, // Quiet
    {    0,  0,  0,     0,     0}, // Bashful
    {    0,  0,  0,    +1,    -1}, // Rash
    {   -1,  0,  0,     0,    +1}, // Calm
    {    0, -1,  0,     0,    +1}, // Gentle
    {    0,  0, -1,     0,    +1}, // Sassy
    {    0,  0,  0,    -1,    +1}, // Careful
    {    0,  0,  0,     0,     0}, // Quirky
};

enum
{
	DARK,
	WHITE_TITLE,
	BLUE_SHADOW,
	WHITE,
	RED_SHADOW,
	BLUE,
	RED,
	ORANGE,
	RED_2,
};

enum
{
	tTimer,
	Offset,
};

static const u8 sPSSTextColours[][3] =
{
    [DARK] = {0, 14, 13},		//Text Dark
    [WHITE_TITLE] = {0, 1, 2}, 	//Text White (title) 
    [BLUE_SHADOW] = {0, 15, 7}, //Red Shadow
    [WHITE] = {0, 15, 14},		//Text White
    [RED_SHADOW] = {0, 15, 1}, 	//Blue Shadow
    [BLUE] = {0, 7, 6}, 		//Male Symbol
    [RED] = {0, 1, 2}, 			//Female Symbol (red)
    [ORANGE] = {0, 3, 14}, 		//Low PP
    [RED_2] = {0, 1, 14}, 		//Whithout PP
};

static const u8 sUnknown_8463FB8[] =
{
    0, 2, 3, 1, 4, 5
};

static const s8 sUnknown_8463FBE[] =
{
    -1, 0, 1
};

static const s8 sUnknown_8463FC1[] =
{
    -2, -1, 0, 1, 2
};

static const s8 sUnknown_8463FC6[] =
{
    -3, -2, -1, 0, 1, 2, 3
};

static const s8 sUnknown_8463FCD[] =
{
    -5, -3, -1, 0, 1, 3, 5
};

static const s8 sUnknown_8463FD4[] =
{
    1, 1, 0, -1, -1, 0, -1, -1, 0, 1, 1
};

static const s8 sUnknown_8463FDF[] =
{
    2, 1, 0, -1, -2, 0, -2, -1, 0, 1, 2
};

static const s8 sUnknown_8463FEA[] =
{
    2, 1, 1, 0, -1, -1, -2, 0, -2, -1, -1, 0, 1, 1, 2
};

static const u16 * const sHpBarPalettes[] =
{
    gPal0SummaryScreen,
    gPal1SummaryScreen,
    gPal2SummaryScreen,
};

#define FREE_AND_SET_NULL_IF_SET(ptr) \
{                                     \
    if (ptr != NULL)                  \
    {                                 \
        free(ptr);                    \
        (ptr) = NULL;                 \
    }                                 \
}

void ShowPokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, MainCallback savedCallback, u8 mode)
{
    sMonSummaryScreen = AllocZeroed(sizeof(struct PokemonSummaryScreenData));
    sUnknown_203B144 = AllocZeroed(sizeof(struct Struct203B144));

    if (sMonSummaryScreen == NULL)
    {
        SetMainCallback2(savedCallback);
        return;
    }

    sLastViewedMonIndex = cursorPos;

    sUnknown_203B16D = 0;
    sUnknown_203B16E = 0;
    sMonSummaryScreen->savedCallback = savedCallback;
    sMonSummaryScreen->monList.mons = party;

    if (party == gEnemyParty)
        sMonSummaryScreen->isEnemyParty = TRUE;
    else
        sMonSummaryScreen->isEnemyParty = FALSE;

    sMonSummaryScreen->lastIndex = lastIdx;
    sMonSummaryScreen->mode = mode;

    switch (sMonSummaryScreen->mode)
    {
    case PSS_MODE_NORMAL:
    default:
        SetHelpContext(HELPCONTEXT_POKEMON_INFO);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_INFO;
        sMonSummaryScreen->isBoxMon = FALSE;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        break;
    case PSS_MODE_BOX:
        SetHelpContext(HELPCONTEXT_POKEMON_INFO);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_INFO;
        sMonSummaryScreen->isBoxMon = TRUE;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        break;
    case PSS_MODE_SELECT_MOVE:
    case PSS_MODE_FORGET_MOVE:
        SetHelpContext(HELPCONTEXT_POKEMON_MOVES);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_MOVES_INFO;
        sMonSummaryScreen->isBoxMon = FALSE;
        sMonSummaryScreen->lockMovesFlag = TRUE;
        break;
    }

    sMonSummaryScreen->state3270 = 0;
    sMonSummaryScreen->state3274 = 0;
    sMonSummaryScreen->unk3278 = 0;
    sMonSummaryScreen->state = 0;

    sMonSummaryScreen->unk3238 = 0;
    sMonSummaryScreen->unk323C = 2;
    sMonSummaryScreen->unk3240 = 1;
    sMonSummaryScreen->unk3244 = FALSE;

    sMonSummaryScreen->unk3228 = 0;
    sMonSummaryScreen->unk322C = 1;

    sub_8138B8C(&sMonSummaryScreen->currentMon);
    sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG);
    sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG);

    if (sMonSummaryScreen->isBadEgg == TRUE)
        sMonSummaryScreen->isEgg = TRUE;

    sMonSummaryScreen->unk3300[0] = 0xff;
    SetMainCallback2(sub_8135C34);
}

void ShowSelectMovePokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, MainCallback savedCallback, u16 a4)
{
    ShowPokemonSummaryScreen(party, cursorPos, lastIdx, savedCallback, PSS_MODE_SELECT_MOVE);
    sMonSummaryScreen->currentMove[4] = a4;
}

static u8 sub_813476C(u8 a0)
{
    if (sMonSummaryScreen->unk301C == TRUE && sMonSummaryScreen->unk3224 != a0)
        return TRUE;

    return FALSE;
}

u32 sub_81347A4(u8 a0)
{
    if (sMonSummaryScreen->isEgg)
        return FALSE;

    if (sMonSummaryScreen->unk3300[0] != 0xff && sMonSummaryScreen->unk3300[0] == a0)
    {
        sMonSummaryScreen->unk3300[0] = 0xff;
        return TRUE;
    }

    if (sub_813476C(a0))
        return FALSE;

    switch (a0)
    {
    case 1:
        if (JOY_NEW(DPAD_RIGHT))
            return TRUE;

        if (gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR && JOY_NEW(R_BUTTON))
            return TRUE;

        break;
    case 0:
        if (JOY_NEW(DPAD_LEFT))
            return TRUE;

        if (gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR && JOY_NEW(L_BUTTON))
            return TRUE;

        break;
    }

    return FALSE;
}

static void sub_8134840(u8 taskId)
{
    switch (sMonSummaryScreen->state3270) {
    case PSS_STATE3270_0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_1;
        break;
    case PSS_STATE3270_1:
        if (!gPaletteFade.active)
        {
            PSS_PlayMonCry();
            sMonSummaryScreen->state3270 = PSS_STATE3270_2;
            return;
        }

        sMonSummaryScreen->state3270 = PSS_STATE3270_1;
        break;
    case PSS_STATE3270_2:
        if (MenuHelpers_CallLinkSomething() == TRUE)
            return;
        else if (sub_800B270() == TRUE)
            return;
        else if (FuncIsActiveTask(sub_813B3F0))
            return;

        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (sub_81347A4(1) == TRUE)
            {
                if (FuncIsActiveTask(sub_8134BAC))
                {
                    sMonSummaryScreen->unk3300[0] = 1;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex < PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->unk3224 = 1;
                    PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_3;
                }
                return;
            }
            else if (sub_81347A4(0) == TRUE)
            {
                if (FuncIsActiveTask(sub_8134BAC))
                {
                    sMonSummaryScreen->unk3300[0] = 0;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex > PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->unk3224 = 0;
                    PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex--;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_3;
                }
                return;
            }
        }

        if ((!FuncIsActiveTask(sub_8134BAC)) || FuncIsActiveTask(sub_813B3F0))
        {
            if (JOY_NEW(DPAD_UP))
            {
                sub_813B120(taskId, -1);
                return;
            }
            else if (JOY_NEW(DPAD_DOWN))
            {
                sub_813B120(taskId, 1);
                return;
            }
            else if (JOY_NEW(A_BUTTON))
            {
                if (sMonSummaryScreen->curPageIndex == PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->state3270 = PSS_STATE3270_4;
                }
                else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->unk3224 = 1;
                    PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_3;
                }
                return;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                sMonSummaryScreen->state3270 = PSS_STATE3270_4;
            }
        }
        break;
    case PSS_STATE3270_3:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            CreateTask(sub_8134BAC, 0);
            sMonSummaryScreen->state3270 = PSS_STATE3270_2;
        }
        else
        {
            gTasks[sMonSummaryScreen->task].func = sub_8134E84;
            sMonSummaryScreen->state3270 = PSS_STATE3270_2;
        }
        break;
    case PSS_STATE3270_4:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_5;
        break;
    case PSS_STATE3270_5:
		SetGpuReg(REG_OFFSET_BLDCNT, 0);
		SetGpuReg(REG_OFFSET_BLDALPHA, 0);

        if (Overworld_LinkRecvQueueLengthMoreThan2() == TRUE)
            return;
        else if (sub_800B270() == TRUE)
            return;

        sMonSummaryScreen->state3270 = PSS_STATE3270_6;
        break;
    default:
        if (!gPaletteFade.active)
            sub_8137E64(taskId);

        break;
    }
}

static void sub_8134BAC(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    switch (data[0])
    {
    case 0:
        sub_8135638();
        sub_81356EC();
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->unk301C = TRUE;
        sub_8138280(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (!(sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES && sMonSummaryScreen->unk3224 == 0))
            {
				
            }
        }
        break;
    case 2:
        sub_8135AA4();
        sub_8135B90();
        sub_8138538();
        sub_8137D28(sMonSummaryScreen->curPageIndex);
        break;
    case 3:
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        break;
    case 4:
        break;
    case 5:
        sub_8135514();
        sMonSummaryScreen->unk3244 = TRUE;
        break;
    case 6:
        if (!sub_81357A0(sMonSummaryScreen->unk3224))
            return;

        break;
    case 7:
        PSS_AddTextToWin3();
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
            PSS_AddTextToWin4();

        PSS_AddTextToWin5();
        PSS_DrawMonMoveIcon();
        break;
    case 8:
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        break;
    case 9:
        HideBg(0);
		if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
            ShowBg(0);
        }
        else
            return;

        break;
    default:
        PSS_SetHelpContext();

        if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
            gTasks[sMonSummaryScreen->task].func = sub_8138CD8;

        DestroyTask(taskId);
        data[0] = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->unk301C = FALSE;
        return;
    }

    data[0]++;
}

static void sub_8134E84(u8 taskId)
{
    switch (sMonSummaryScreen->unk3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->unk301C = TRUE;
        sub_8138280(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (!(sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES && sMonSummaryScreen->unk3224 == 0))
            {
				
            }
        }
        break;
    case 2:
        sub_8135638();
        sub_8135B90();
        sub_8138538();
        sub_8135AA4();
        break;
    case 3:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PSS_AddTextToWin0(gText_8419C39);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			PSS_AddTextToWin0(gText_8419C39Spa);
        if (!(gMain.inBattle || gReceivedRemoteLinkPlayers))
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PSS_AddTextToWin1(gText_8419C92);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PSS_AddTextToWin1(gText_8419C92Spa);
		}
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PSS_AddTextToWin1(gText_8419CA2);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PSS_AddTextToWin1(gText_8419CA2Spa);
		}
        break;
    case 4:
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        break;
    case 5:
        break;
    case 6:
        PSS_AddTextToWin3();
        PSS_AddTextToWin5();
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        break;
    case 7:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
            sub_8135514();
            sMonSummaryScreen->unk3244 = TRUE;
        }
        else
            return;
        break;
    case 8:
        if (!sub_81357A0(sMonSummaryScreen->unk3224))
            return;

        PSS_AddTextToWin4();
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        break;
    case 9:
        PSS_DrawMonMoveIcon();
        PSS_AddTextToWin2(gText_8419C45);
        break;
    case 10:
        sub_81356EC();
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        break;
    case 11:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
        }
        else
            return;
        break;
    default:
        PSS_SetHelpContext();
        gTasks[sMonSummaryScreen->task].func = sub_8138CD8;
        sMonSummaryScreen->unk3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->unk301C = FALSE;
        return;
    }

    sMonSummaryScreen->unk3284++;
    return;
}

static void sub_81351A0(u8 taskId)
{
    switch (sMonSummaryScreen->unk3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->unk301C = TRUE;
        sub_8138280(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        break;
    case 2:
        sub_8135AA4();
        break;
    case 3:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyBgTilemapBufferToVram(0);
        break;
    case 4:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PSS_AddTextToWin0(gText_8419C39);
			PSS_AddTextToWin1(gText_8419C82);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PSS_AddTextToWin0(gText_8419C39Spa);
			PSS_AddTextToWin1(gText_8419C82Spa);
		}
        break;
    case 5:
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        break;
    case 6:
        sub_8135514();
        sMonSummaryScreen->unk3244 = TRUE;
        sub_8135638();
        PSS_DrawMonMoveIcon();
        break;
    case 7:
        break;
    case 8:
        if (sub_81357A0(sMonSummaryScreen->unk3224) == 0)
            return;

        PSS_AddTextToWin2(gText_8419C45);
        break;
    case 9:
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        CopyBgTilemapBufferToVram(0);
        break;
    case 10:
        sub_8135B90();
        sub_8138538();
        sub_81356EC();
        break;
    default:
        PSS_SetHelpContext();
        gTasks[sMonSummaryScreen->task].func = sub_8134840;
        sMonSummaryScreen->unk3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->unk301C = FALSE;
        return;
    }

    sMonSummaryScreen->unk3284++;
    return;
}

static void sub_8135514(void)
{
    s8 pageDelta = 1;

    if (sMonSummaryScreen->unk3224 == 1)
        pageDelta = -1;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sMonSummaryScreen->unk324C = 240;
        return;
    }

    if ((sMonSummaryScreen->curPageIndex + pageDelta) == PSS_PAGE_MOVES_INFO)
    {
        sMonSummaryScreen->unk324C = 0;
        return;
    }
    if (sMonSummaryScreen->unk3224 == 1)
        sMonSummaryScreen->unk324C = 0;
    if (sMonSummaryScreen->curPageIndex != PSS_PAGE_SKILLS)
	{
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
	}
    else
	{
		PSS_SetInvisibleHpBar(0);
		PSS_SetInvisibleExpBar(0);
	}
}

static void sub_8135638(void)
{
    u8 newPage;

    if (sMonSummaryScreen->unk3224 == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        PSS_SetInvisibleHpBar(1);
        PSS_SetInvisibleExpBar(1);
        break;
    case PSS_PAGE_SKILLS:
        PSS_SetInvisibleHpBar(0);
        PSS_SetInvisibleExpBar(0);
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->unk3224 == 1)
        {
            PSS_SetInvisibleMonSprite(1);
            PSS_SetInvisibleMarkings(1);
            PSS_SetInvisiblePokeball(1);
            PSS_SetInvisibleIconStatus(1);
            sub_813ACF8(1);
            sub_813AEB0(1);
			PSS_SetInvisibleHpBar(1);
			PSS_SetInvisibleExpBar(1);
        }

        break;
    case PSS_PAGE_MOVES_INFO:
        sub_813A0E8(1);
        sub_8139EE4(1);
        PSS_SetInvisibleIconStatus(1);
        sub_813ACF8(1);
        sub_813AEB0(1);
        PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
        break;
    }
}

static void sub_81356EC(void)
{
    u8 newPage;

    if (sMonSummaryScreen->unk3224 == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        PSS_SetInvisibleHpBar(1);
        PSS_SetInvisibleExpBar(1);
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->unk3224 != 0)
        {
            sub_813A0E8(0);
            sub_813ACF8(0);
            sub_8139EE4(0);
            sub_813AEB0(0);
        }
        break;
    case PSS_PAGE_MOVES_INFO:
        PSS_SetInvisibleMonSprite(0);
        PSS_SetInvisibleMarkings(0);
        PSS_SetInvisibleIconStatus(0);
        PSS_SetInvisiblePokeball(0);
        sub_813ACF8(0);
        sub_813AEB0(0);
        break;
    }
}

static u8 sub_81357A0(u8 a0)
{
    s8 pageDelta = 1;

    if (sMonSummaryScreen->unk3224 == 1)
        pageDelta = -1;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
        if (sMonSummaryScreen->unk324C <= 0)
        {
            sMonSummaryScreen->unk324C = 0;
            sMonSummaryScreen->unk3238 ^= 1;
            sMonSummaryScreen->unk3244 = FALSE;
            return TRUE;
        }

    if ((sMonSummaryScreen->curPageIndex + pageDelta) == PSS_PAGE_MOVES_INFO)
        if (sMonSummaryScreen->unk324C >= 240)
        {
            sMonSummaryScreen->unk324C = 240;
            sMonSummaryScreen->unk3238 ^= 1;
            sMonSummaryScreen->unk3244 = FALSE;
            return TRUE;
        }

    if (sMonSummaryScreen->unk3224 == 1)
    {
        if (sMonSummaryScreen->unk324C >= 240)
        {
            sMonSummaryScreen->unk324C = 240;
            sMonSummaryScreen->unk3238 ^= 1;
            sMonSummaryScreen->unk3244 = FALSE;
            return TRUE;
        }
    }
    else if (sMonSummaryScreen->unk324C <= 0)
    {
        sMonSummaryScreen->unk3238 ^= 1;
        sMonSummaryScreen->unk324C = 0;
        sMonSummaryScreen->unk3244 = FALSE;
        return TRUE;
    }

    return FALSE;
}

static void sub_8135AA4(void)
{
    u8 newPage;

    if (sMonSummaryScreen->unk3224 == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void sub_8135B90(void)
{
    u8 newPage;

    if (sMonSummaryScreen->unk3224 == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void sub_8135C34(void)
{
    switch (sMonSummaryScreen->state3274)
    {
    case 0:
        PSS_TurnOffScreen();
        break;
    case 1:
        PSS_InitDisplay();
        break;
    case 2:
        PSS_ResetAll();
        break;
    case 3:
        if (!PSS_LoadTilesAndMap())
            return;
        break;
    case 4:
        if (!PSS_LoadSpritesData())
            return;
        break;
    case 5:
        PSS_InitWindows();
        break;
    case 6:
        if (!sub_8136040())
            return;
        break;
    case 7:
        PSS_AddTextToWin3();
        break;
    case 8:
        PSS_AddTextToWin4();
        break;
    case 9:
        PSS_AddTextToWin5();
        PSS_DrawMonMoveIcon();
        break;
    case 10:
        sub_8138538();
        break;
    case 11:
        break;
    case 12:
        BlendPalettes(0xffffffff, 16, 0);
        sub_8137D28(sMonSummaryScreen->curPageIndex);
        sub_8137E28();
        break;
    case 13:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        break;
    case 14:
        CopyBgTilemapBufferToVram(0);
        break;
    case 15:
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        {
            sub_8139EE4(0);
            sub_813A0E8(0);
        }
        else
        {
            PSS_SetInvisibleMonSprite(0);
            PSS_SetInvisibleMarkings(0);
            PSS_SetInvisiblePokeball(0);
        }

		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
        PSS_SetInvisibleIconStatus(0);
        sub_813ACF8(0);
        sub_813AEB0(0);
        break;
    default:
        sub_81381C0();
        sub_8136D54();
        return;
    }

    sMonSummaryScreen->state3274++;
}

static u8 PSS_LoadTilesAndMap(void)
{
    switch (sMonSummaryScreen->unk3278)
    {
    case 0:
        LoadPalette(gPalSummaryScreen, 0, 0x20);
        break;
    case 1:
        ListMenuLoadStdPalAt(0x60, 1);
        LoadPalette(sUnknown_84636C0, 0x70, 0x20);
        break;
    case 2:
        LZ77UnCompVram(gTilesSummaryScreen, (void *)(VRAM));
        break;
    case 3:
        LZ77UnCompVram(gMapSummaryScreenBg, (void *)(VRAM + 0xF800));
        break;
    case 4:
        LZ77UnCompVram(gMapSummaryScreenBg, (void *)(VRAM + 0xF800));
        break;
    case 5:
        LoadPalette(sUnknown_8463700, 0x80, 0x20);
        return TRUE;
    }

    sMonSummaryScreen->unk3278++;
    return FALSE;
}

static u8 sub_8136040(void)
{
    switch (sMonSummaryScreen->unk3280)
    {
    case 0:
        PSS_GetDataPokemon();
        if (sMonSummaryScreen->isEgg)
        {
            sMonSummaryScreen->unk3280 = 0;
            return TRUE;
        }

        break;
    case 1:
        if (sMonSummaryScreen->isEgg == 0)
            PSS_GetStatsPokemon();
        break;
    case 2:
        if (sMonSummaryScreen->isEgg == 0)
            sub_81367B0();
        break;
    default:
        sMonSummaryScreen->unk3280 = 0;
        return TRUE;
    }

    sMonSummaryScreen->unk3280++;
    return FALSE;
}

static void PSS_GetDataPokemon(void)
{
    u8 tempStr[20];
    u16 dexNum;
    u16 gender;
    u16 heldItem;
    u32 otId;

    dexNum = SpeciesToPokedexNum(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES));
    if (dexNum == 0xffff)
        StringCopy(sMonSummaryScreen->summary.dexNum, gText_8416202);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.dexNum, dexNum, STR_CONV_MODE_LEADING_ZEROS, 3);

    sUnknown_203B144->unk00 = 0;

    if (!sMonSummaryScreen->isEgg)
    {
        dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);
        GetSpeciesName(sMonSummaryScreen->summary.specieName, dexNum);
    }
    else
    {
        StringCopy(sMonSummaryScreen->summary.specieName, gText_EggNickname);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.specieName, gText_EggNicknameSpa);
		return;
    }

    sMonSummaryScreen->typeIcons[0] = gBaseStats[dexNum].type1;
    sMonSummaryScreen->typeIcons[1] = gBaseStats[dexNum].type2;

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_NICKNAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.nickname, tempStr, POKEMON_NAME_LENGTH);
    StringGetEnd10(sMonSummaryScreen->summary.nickname);

    gender = GetMonGender(&sMonSummaryScreen->currentMon);
    dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);

    if (gender == MON_FEMALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_FemaleSymbol);
    else if (gender == MON_MALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_MaleSymbol);
    else
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_StringDummy);

    if (dexNum == SPECIES_NIDORAN_M || dexNum == SPECIES_NIDORAN_F)
        if (StringCompare(sMonSummaryScreen->summary.nickname, gSpeciesNames[dexNum]) == 0)
            StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_StringDummy);

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_NAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.ot_name, tempStr, OT_NAME_LENGTH);

    ConvertInternationalString(sMonSummaryScreen->summary.ot_name, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LANGUAGE));

    otId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID) & 0xffff;
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.ot_id, otId, STR_CONV_MODE_LEADING_ZEROS, 5);

    ConvertIntToDecimalStringN(tempStr, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(sMonSummaryScreen->summary.level, gText_Lv);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(sMonSummaryScreen->summary.level, gText_LvSpa);
    StringAppendN(sMonSummaryScreen->summary.level, tempStr, 4);

    heldItem = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HELD_ITEM);

    if (heldItem == ITEM_NONE)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(sMonSummaryScreen->summary.heldItem, gText_84161EF);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.heldItem, gText_84161EFSpa);
	}
    else
        CopyItemName(heldItem, sMonSummaryScreen->summary.heldItem);
}

#define MACRO_8136350_0(x) (63 - StringLength((x)) * 6)
#define MACRO_8136350_1(x) (27 - StringLength((x)) * 6)

static void PSS_GetStatsPokemon(void)
{
    u8 tempStr[20];
    u8 level;
    u16 type;
    u16 species;
    u16 hp;
    u16 statValue;
    u32 exp;
    u32 expToNextLevel;

    hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk3090, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(sMonSummaryScreen->summary.unk3090, gText_Slash);

    hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);
    ConvertIntToDecimalStringN(tempStr, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(sMonSummaryScreen->summary.unk3090, tempStr);

    sUnknown_203B144->unk02 = MACRO_8136350_0(sMonSummaryScreen->summary.unk3090);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary && sMonSummaryScreen->isEnemyParty == TRUE)
    {
        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->tileTag = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->palTag = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk08 = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk0A = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk0C = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE]);
    }
    else
    {
        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->tileTag = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->palTag = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk08 = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk0A = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sUnknown_203B144->unk0C = MACRO_8136350_1(sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE]);
    }

    exp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EXP);
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk31A4, exp, STR_CONV_MODE_LEFT_ALIGN, 7);
    sUnknown_203B144->unk0E = MACRO_8136350_0(sMonSummaryScreen->summary.unk31A4);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL);
    expToNextLevel = 0;
    if (level < 100)
    {
        species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);
        expToNextLevel = gExperienceTables[gBaseStats[species].growthRate][level + 1] - exp;
    }

    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk31B0, expToNextLevel, STR_CONV_MODE_LEFT_ALIGN, 7);
    sUnknown_203B144->unk10 = MACRO_8136350_0(sMonSummaryScreen->summary.unk31B0);

    type = GetAbilityBySpecies(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES), GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ABILITY_NUM));
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		StringCopy(sMonSummaryScreen->summary.abilityName, gAbilityNames[type]);
		StringCopy(sMonSummaryScreen->summary.abilityDescription, gAbilityDescriptionPointers[type]);
	}
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		StringCopy(sMonSummaryScreen->summary.abilityName, gAbilityNamesSpa[type]);
		StringCopy(sMonSummaryScreen->summary.abilityDescription, gAbilityDescriptionSpaPointers[type]);
	}

    sMonSummaryScreen->monStatus = PSS_CheckMonStatus(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_STATUS));
    if (sMonSummaryScreen->monStatus == AILMENT_NONE)
        if (CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0))
            sMonSummaryScreen->monStatus = AILMENT_PKRS;
}

static void sub_81367B0(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        PSS_GetMoveNames(i);

    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
        PSS_GetMoveNames(4);
}

#define MACRO_81367E8_0(a, b) ((6 * (a)) - StringLength((b)) * 6)

static void PSS_GetMoveNames(u8 i)
{
    if (i < 4)
        sMonSummaryScreen->currentMove[i] = PSS_GetMonMove(&sMonSummaryScreen->currentMon, i);

    if (sMonSummaryScreen->currentMove[i] == 0)
    {
        StringCopy(sMonSummaryScreen->summary.moveName[i], gText_841620E);
        StringCopy(sMonSummaryScreen->summary.unk30B8[i], gText_8416210);
        StringCopy(sMonSummaryScreen->summary.power[i], gText_ThreeHyphens);
        StringCopy(sMonSummaryScreen->summary.unk3188[i], gText_ThreeHyphens);
        sUnknown_203B144->unk12[i] = 0xff;
        sUnknown_203B144->unk1C[i] = 0xff;
        return;
    }

    sMonSummaryScreen->unk3264++;
    sMonSummaryScreen->move[i] = gBattleMoves[sMonSummaryScreen->currentMove[i]].type;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(sMonSummaryScreen->summary.moveName[i], gMoveNames[sMonSummaryScreen->currentMove[i]]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(sMonSummaryScreen->summary.moveName[i], gMoveNamesSpa[sMonSummaryScreen->currentMove[i]]);

    if (i >= 4 && sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
    {
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk30B8[i],
                                   gBattleMoves[sMonSummaryScreen->currentMove[i]].pp, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk30F0[i],
                                   gBattleMoves[sMonSummaryScreen->currentMove[i]].pp, STR_CONV_MODE_LEFT_ALIGN, 3);
    }
    else
    {
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk30B8[i],
                                   PSS_GetMovePP(&sMonSummaryScreen->currentMon, i), STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk30F0[i],
                                   CalculatePPWithBonus(sMonSummaryScreen->currentMove[i], GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES), i),
                                   STR_CONV_MODE_LEFT_ALIGN, 3);
    }

    sUnknown_203B144->unk12[i] = MACRO_81367E8_0(2, sMonSummaryScreen->summary.unk30B8[i]);
    sUnknown_203B144->unk1C[i] = MACRO_81367E8_0(2, sMonSummaryScreen->summary.unk30F0[i]);

    if (gBattleMoves[sMonSummaryScreen->currentMove[i]].power <= 1)
        StringCopy(sMonSummaryScreen->summary.power[i], gText_ThreeHyphens);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.power[i], gBattleMoves[sMonSummaryScreen->currentMove[i]].power, STR_CONV_MODE_RIGHT_ALIGN, 3);

    if (gBattleMoves[sMonSummaryScreen->currentMove[i]].accuracy == 0)
        StringCopy(sMonSummaryScreen->summary.unk3188[i], gText_ThreeHyphens);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.unk3188[i], gBattleMoves[sMonSummaryScreen->currentMove[i]].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
}

static u8 PSS_LoadSpritesData(void)
{
    switch (sMonSummaryScreen->state)
    {
    case 0:
        PSS_LoadShinyIndicator(TAG_PSS_UNK_A0, TAG_PSS_UNK_A0);
        break;
    case 1:
        PSS_LoadUnkPoint(TAG_PSS_UNK_96, TAG_PSS_UNK_96);
        break;
    case 2:
        PSS_LoadMarkings();
        break;
    case 3:
        PSS_LoadMovesSelectCursor(TAG_PSS_UNK_64, TAG_PSS_UNK_64);
        break;
    case 4:
        PSS_LoadIconStatus(TAG_PSS_UNK_6E, TAG_PSS_UNK_6E);
        break;
    case 5:
        PSS_LoadHpBar(TAG_PSS_UNK_78, TAG_PSS_UNK_78);
        break;
    case 6:
        PSS_LoadExpBar(TAG_PSS_UNK_82, TAG_PSS_UNK_82);
        break;
    case 7:
        PSS_LoadPokeball();
        break;
    case 8:
        PSS_LoadMonIcon();
        break;
    default:
        PSS_LoadMonSprite();
        return TRUE;
    }

    sMonSummaryScreen->state++;
    return FALSE;
}

static void PSS_ResetAll(void)
{
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ScanlineEffect_Stop();
}

static void PSS_InitDisplay(void)
{
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, SummayScreenBgTemplate, NELEMS(SummayScreenBgTemplate));

    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);

    DeactivateAllTextPrinters();
//Transparencia
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1 | BLDCNT_TGT2_BG2);
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(13, 16));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

static void sub_8136D54(void)
{
    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        sMonSummaryScreen->task = CreateTask(sub_81393D4, 0);
    else
        sMonSummaryScreen->task = CreateTask(sub_8134840, 0);

	SetMainCallback2(sub_8137EE8);
}

static void PSS_AddTextToWin0(const u8 * str)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[0], 0);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[0], 2, 4, 1, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->window[0]);
}

static void PSS_AddTextToWin1(const u8 * str)
{
    u8 v0;
    s32 width;
    u8 r1;

    FillWindowPixelBuffer(sMonSummaryScreen->window[1], 0);
    width = GetStringWidth(0, str, 0);
    r1 = sMonSummaryScreen->window[1];
    AddTextPrinterParameterized3(r1, 0, 0x54 - width, 0, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->window[1]);
}

static void PSS_AddTextToWin2(const u8 * msg)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[2], 0);

    if (!sMonSummaryScreen->isEgg)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 0, 14, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.level);
		}
		else
		{
			if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[0] + 1, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[1] + 1, 38, 16);
			}
			if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[0] + 24, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[1] + 24, 38, 16);
			}
		}
        AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 0, 2, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.nickname);
			
		if (GetMonGender(&sMonSummaryScreen->currentMon) == MON_FEMALE)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 62, 2, sPSSTextColours[RED], 0, sMonSummaryScreen->summary.genderSymbol);
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 62, 2, sPSSTextColours[BLUE], 0, sMonSummaryScreen->summary.genderSymbol);
	}

    PutWindowTilemap(sMonSummaryScreen->window[2]);
}

static void PSS_AddTextToWin3(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[3], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PSS_ShowInfoPokemon();
        break;
    case PSS_PAGE_SKILLS:
        PSS_ShowMonStats();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PSS_PrintMoveNamesOrCancel();
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->window[3]);
}

static void PSS_ShowInfoPokemon(void)
{
    

    if (!sMonSummaryScreen->isEgg)
    {
        AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.specieName);
		
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Name);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Type);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OT);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Item);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TypeSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OTSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ItemSpa);
		}
		
		if ((HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY))) < 8)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80 + sUnknown_203B144->unk00, 8, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNum);
		else	
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80 + sUnknown_203B144->unk00, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNum);
		if ((gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[3] << 24)) == GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID))
		{
			if (gSaveBlock2Ptr->playerGender == FEMALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
			if (gSaveBlock2Ptr->playerGender == MALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[BLUE], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
		}
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_id);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.heldItem);
    }
    else
    {
        u8 eggCycles;
        u8 hatchMsgIndex;

        eggCycles = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_FRIENDSHIP);

        if (eggCycles <= 5)
            hatchMsgIndex = 3;
        else if (eggCycles <= 10)
            hatchMsgIndex = 2;
        else if (eggCycles <= 40)
            hatchMsgIndex = 1;
        else
            hatchMsgIndex = 0;

        if (sMonSummaryScreen->isBadEgg)
            hatchMsgIndex = 0;

        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Status);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463EC4[hatchMsgIndex]);
        }
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_StatusSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463EC4[hatchMsgIndex + 4]);
		}
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.specieName);
    }
}

static void PSS_ShowMonStats(void)
{
    u8 nature = GetNature(&sMonSummaryScreen->currentMon);
	
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPoints);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLv);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_Attack);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_Defense);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtk);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDef);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_Speed);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HPSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPointsSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLvSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_AttackSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_DefenseSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtkSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDefSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_SpeedSpa);
	}
	
	AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 68 + sUnknown_203B144->unk02, 0, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk3090);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->tileTag, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->palTag, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk08, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk0A, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk0C, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 55 + sUnknown_203B144->unk0E, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk31A4);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 55 + sUnknown_203B144->unk10, 92, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk31B0);
}

#define MACRO_8137270(x) ((x) * 28 + 5)

static void PSS_PrintMoveNamesOrCancel(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        PSS_PrintMoveNamesAndPP(i);

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
            PSS_PrintMoveNamesAndPP(4);
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(4), sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_Cancel);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(4), sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_CancelSpa);
		}
    }
}

#define MACRO_81372E4(x) ((x) * 28 + 16)

static void PSS_PrintMoveNamesAndPP(u8 i)
{
    u8 color = WHITE;
    u8 curPP = PSS_GetMovePP(&sMonSummaryScreen->currentMon, i);
    u16 move = sMonSummaryScreen->currentMove[i];
    u8 ppBonuses = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES);
    u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);

    if (i == 4)
        curPP = maxPP;
	//Add Move Names
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(i) - 2, sPSSTextColours[WHITE], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveName[i]);

    if (sMonSummaryScreen->currentMove[i] == 0 || (curPP == maxPP))
        color = WHITE;
    else if (curPP == 0)
        color = RED_2;
    else if (maxPP == 3)
    {
        if (curPP == 2)
            color = ORANGE;
        else if (curPP == 1)
            color = ORANGE;
    }
    else if (maxPP == 2)
    {
        if (curPP == 1)
            color = ORANGE;
    }
    else
    {
        if (curPP <= (maxPP / 4))
            color = ORANGE;
        else if (curPP <= (maxPP / 2))
            color = ORANGE;
    }
	//Add PP text
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 20, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_8416238);
	// Add PP counter
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 40 + sUnknown_203B144->unk12[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk30B8[i]);

    if (sMonSummaryScreen->currentMove[i] != MOVE_NONE)
    {
		// Add Slash
        AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 52, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_Slash);
        // Add PP Max
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 58 + sUnknown_203B144->unk1C[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk30F0[i]);
    }
}

static void PSS_AddTextToWin4(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[4], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PSS_CheckIfMonIsEgg();
        break;
    case PSS_PAGE_SKILLS:
        PSS_PrintExpPointAndNextLvTexts();
        break;
    case PSS_PAGE_MOVES_INFO:
        PSS_ShowAttackInfo();
        break;
    case PSS_PAGE_MOVES:
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->window[4]);
}

static void PSS_CheckIfMonIsEgg(void)
{
    if (!sMonSummaryScreen->isEgg)
        PSS_ShowEggInfo();
    else
        PSS_ShowMonInfo();
}

static void sub_8137578(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNameSpaPointers[nature]);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL);

    if (level == 0)
        level = 1;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (sub_813B838(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || sub_8138B4C() == TRUE)
				StringCopy(mapNameStr, gText_8419C13);
			else
				StringCopy(mapNameStr, gText_8419C0B);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || sub_8138B4C() == TRUE)
				StringCopy(mapNameStr, gText_8419C13Spa);
			else
				StringCopy(mapNameStr, gText_8419C0BSpa);
		}
    }

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL) == 0)
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992F);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992FSpa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198D5);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198B4);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198D5Spa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198B4Spa);
			}
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841Spa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822Spa);
			}
        }
    }

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
    AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PSS_ShowTrainerMemo(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNameSpaPointers[nature]);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL);

    if (level == 0)
        level = 1;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (!sub_813B838(metLocation) || !PSS_IsMonFromGenIII())
    {
        if (sub_8138B4C() == TRUE)
        {
            sub_8137578();
            return;
        }

        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782Spa);
			}
        }
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

        AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
        return;
    }

    if (sub_813B838(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(mapNameStr, gText_8419C0B);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(mapNameStr, gText_8419C0BSpa);
	}		

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL) == 0)
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{                
				if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4);
				if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4Spa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199AB);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199ABSpa);
			}
        }
        else
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

    AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PSS_ShowEggInfo(void)
{
    if (sub_813847C(&sMonSummaryScreen->currentMon) == TRUE)
        sub_8137578();
    else
        PSS_ShowTrainerMemo();
}

static void PSS_ShowMonInfo(void)
{
    u8 metLocation;
    u8 version;
    u8 chosenStrIndex = 0;

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (sMonSummaryScreen->monList.mons != gEnemyParty)
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
                chosenStrIndex = 1;
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (chosenStrIndex == 0 || chosenStrIndex == 2)
                if (sub_813847C(&sMonSummaryScreen->currentMon) == FALSE)
                    chosenStrIndex++;
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
            {
                if (metLocation == METLOC_SPECIAL_EGG)
                    chosenStrIndex = 5;
            }
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (sub_813847C(&sMonSummaryScreen->currentMon) == FALSE)
                chosenStrIndex++;
        }
    }

    if (sMonSummaryScreen->isBadEgg)
        chosenStrIndex = 0;

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463ED4[chosenStrIndex]);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
    }
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
		AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463ED4[chosenStrIndex + 7]);
	}
}

static void PSS_PrintExpPointAndNextLvTexts(void)
{
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4D);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4DSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59Spa);
	}
}

static void PSS_ShowAttackInfo(void)
{
    if (sUnknown_203B16D < 5)
    {
        if (sMonSummaryScreen->mode != PSS_MODE_SELECT_MOVE && sUnknown_203B16D == 4)
            return;
		//Add Category Icon
	//	BlitMoveInfoIcon(sMonSummaryScreen->window[4], gBattleMoves[sMonSummaryScreen->currentMove[sUnknown_203B16D]].category + 47, 85, 0);
		//Add Power
        AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 91, 18, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.power[sUnknown_203B16D]);
		//Add Accuracy
        AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 91, 33, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk3188[sUnknown_203B16D]);
		//Add Move Description
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Category);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Power);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Accuracy);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Effect);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->currentMove[sUnknown_203B16D] - 1]);
        }
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_CategorySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_PowerSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AccuracySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_EffectSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->currentMove[sUnknown_203B16D] + MOVES_COUNT - 2]);
		}
        PutWindowTilemap(sMonSummaryScreen->window[4]);
    }
}

static void PSS_AddTextToWin5(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        PSS_PrintAbilityNameAndDescription();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PSS_DrawMoveIcon();
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->window[5]);
}

static void PSS_PrintAbilityNameAndDescription(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[5], 0);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 11,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Ability);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 5,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AbilitySpa);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 60,  4, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityName);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 20, 23, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityDescription);

}

static void PSS_DrawMoveIcon(void)
{
    u8 i;

    FillWindowPixelBuffer(sMonSummaryScreen->window[5], 0);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->currentMove[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[i] + 1, 8, MACRO_8137270(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[4] + 1, 8, MACRO_8137270(4) - 2);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->currentMove[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[i] + 24, 8, MACRO_8137270(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[4] + 24, 8, MACRO_8137270(4) - 2);
	}
}

static void sub_8137D28(u8 curPageIndex)
{
    switch (curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PSS_AddTextToWin0(gText_8419C1D);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			PSS_AddTextToWin0(gText_8419C1DSpa);
        if (!sMonSummaryScreen->isEgg)
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PSS_AddTextToWin1(gText_8419C62);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PSS_AddTextToWin1(gText_8419C62Spa);
		}
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PSS_AddTextToWin1(gText_8419C72);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PSS_AddTextToWin1(gText_8419C72Spa);
		}

        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PSS_AddTextToWin2(gText_8419C45);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			PSS_AddTextToWin2(gText_8419C45Spa);
        break;
    case PSS_PAGE_SKILLS:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PSS_AddTextToWin0(gText_8419C2A);
			PSS_AddTextToWin1(gText_8419C7B);
			PSS_AddTextToWin2(gText_8419C45);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PSS_AddTextToWin0(gText_8419C2ASpa);
			PSS_AddTextToWin1(gText_8419C7BSpa);
			PSS_AddTextToWin2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVES:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PSS_AddTextToWin0(gText_8419C39);
			PSS_AddTextToWin1(gText_8419C82);
			PSS_AddTextToWin2(gText_8419C45);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PSS_AddTextToWin0(gText_8419C39Spa);
			PSS_AddTextToWin1(gText_8419C82Spa);
			PSS_AddTextToWin2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVES_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PSS_AddTextToWin0(gText_8419C39);
			if (!gMain.inBattle)
				PSS_AddTextToWin1(gText_8419C92);
			else
				PSS_AddTextToWin1(gText_8419CA2);
			PSS_AddTextToWin2(gText_8419C45);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PSS_AddTextToWin0(gText_8419C39Spa);
			if (!gMain.inBattle)
				PSS_AddTextToWin1(gText_8419C92Spa);
			else
				PSS_AddTextToWin1(gText_8419CA2Spa);
			PSS_AddTextToWin2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVE_DELETER:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PSS_AddTextToWin0(gText_8419C39);
			PSS_AddTextToWin1(gText_8419CA9);
			PSS_AddTextToWin2(gText_8419C45);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PSS_AddTextToWin0(gText_8419C39Spa);
			PSS_AddTextToWin1(gText_8419CA9Spa);
			PSS_AddTextToWin2(gText_8419C45Spa);
		}
        break;
    default:
        break;
    }
}

static void sub_8137E28(void)
{
    PutWindowTilemap(sMonSummaryScreen->window[0]);
    PutWindowTilemap(sMonSummaryScreen->window[1]);
    PutWindowTilemap(sMonSummaryScreen->window[2]);
}

static void sub_8137E64(u8 taskId)
{
    sub_813AF90();
    FreeAllSpritePalettes();

    if (IsCryPlayingOrClearCrySongs() == TRUE)
        StopCryAndClearCrySongs();

    PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
    FreeAllWindowBuffers();
    DestroyTask(taskId);
    SetMainCallback2(sMonSummaryScreen->savedCallback);

    sLastViewedMonIndex = GetLastViewedMonIndex();

    FREE_AND_SET_NULL_IF_SET(sMonSummaryScreen);
    FREE_AND_SET_NULL_IF_SET(sUnknown_203B144);
}

static void sub_8137EE8(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
	PSS_ScrollPSSBackground();
}

static void sub_8137FF4(void)
{
    if (sMonSummaryScreen->unk324C < 240)
    {
        sMonSummaryScreen->unk324C += 240;
        if (sMonSummaryScreen->unk324C > 240)
            sMonSummaryScreen->unk324C = 240;
    }
}

static void sub_813805C(void)
{
    if (sMonSummaryScreen->unk324C >= 240)
    {
        sMonSummaryScreen->unk324C -= 240;
        if (sMonSummaryScreen->unk324C < 0)
            sMonSummaryScreen->unk324C = 0;
    }
}

static void sub_81380F0(void)
{
    if (sMonSummaryScreen->unk3224 == 1)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
            sub_8137FF4();
        else
            sub_813805C();
    }
    else
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES)
            sub_813805C();
        else
            sub_8137FF4();
    }
}

static void sub_813817C(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    if (sMonSummaryScreen->unk3244 == FALSE)
        return;

    sub_81380F0();
}

static void PSS_TurnOffScreen(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

static void sub_81381C0(void)
{
    SetVBlankCallback(sub_813817C);
}

static void PSS_InitWindows(void)
{
    u8 i;

    InitWindows(sUnknown_8463F9C);

    for (i = 0; i < 3; i++)
        sMonSummaryScreen->window[i] = AddWindow(&WindowTemplatePokemonInfo[i]);

    for (i = 0; i < 4; i++)
        switch (sMonSummaryScreen->curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        default:
            break;
        }
}

static void sub_8138280(u8 curPageIndex)
{
    u8 i;
    for (i = 0; i < 3; i++)
		sMonSummaryScreen->window[i] = AddWindow(&WindowTemplatePokemonInfo[i]);
    for (i = 0; i < 4; i++)
        switch (curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
        default:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        }
}

static void PSS_RemoveAllWindows(u8 curPageIndex)
{
    u8 i;

    for (i = 0; i < 7; i++)
        RemoveWindow(sMonSummaryScreen->window[i]);
}

static void PSS_SetHelpContext(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        SetHelpContext(HELPCONTEXT_POKEMON_INFO);
        break;
    case PSS_PAGE_SKILLS:
        SetHelpContext(HELPCONTEXT_POKEMON_SKILLS);
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        SetHelpContext(HELPCONTEXT_POKEMON_MOVES);
        break;
    }
}

static u8 sub_813847C(struct Pokemon * mon)
{
    u8 i;
    u8 multiplayerId;
    u32 trainerId = 0;

    if (sMonSummaryScreen->monList.mons == gEnemyParty)
    {
        multiplayerId = GetMultiplayerId() ^ 1;
        trainerId = gLinkPlayers[multiplayerId].trainerId & 0xffff;
        StringCopy(sMonSummaryScreen->summary.unk304C[0], gLinkPlayers[multiplayerId].name);
    }
    else
    {
        trainerId = GetPlayerTrainerId() & 0xffff;
        StringCopy(sMonSummaryScreen->summary.unk304C[0], gSaveBlock2Ptr->playerName);
    }

    if (trainerId != (GetMonData(mon, MON_DATA_OT_ID) & 0xffff))
        return FALSE;

    GetMonData(mon, MON_DATA_OT_NAME, sMonSummaryScreen->summary.unk304C[1]);

    if (!StringCompareWithoutExtCtrlCodes(sMonSummaryScreen->summary.unk304C[0], sMonSummaryScreen->summary.unk304C[1]))
        return TRUE;
    else
        return FALSE;

    return TRUE;
}

static void sub_8138538(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (!sMonSummaryScreen->isEgg)
        {
            LZ77UnCompVram(gMapSummaryScreenPokemonInfo, (void *)(VRAM + 0xF000));
        }
        else
        {
            LZ77UnCompVram(gMapSummaryScreenEgg, (void *)(VRAM + 0xF000));
        }
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
        break;
    case PSS_PAGE_SKILLS:
        LZ77UnCompVram(gMapSummaryScreenPokemonSkills, (void *)(VRAM + 0xF000));
		PSS_SetInvisibleHpBar(0);
		PSS_SetInvisibleExpBar(0);
		HideBg(3);
        break;
    case PSS_PAGE_MOVES:
        LZ77UnCompVram(gMapSummaryScreenKnownMoves, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
		ShowBg(3);
        break;
    case PSS_PAGE_MOVES_INFO:
		LZ77UnCompVram(gMapSummaryScreenMovesInfo, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
		ShowBg(3);
        break;
    }
}

static void PSS_DrawMonMoveIcon(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[0] + 1, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[1] + 1, 110, 33);
			}
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[0] + 24, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[1] + 24, 110, 33);
			}
		}
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

u8 GetLastViewedMonIndex(void)
{
    return sLastViewedMonIndex;
}

u8 GetMoveSlotToReplace(void)
{
    return sUnknown_203B16E;
}

void sub_8138B38(u8 mode)
{
    sMonSummaryScreen->mode = mode;
}

static bool32 sub_8138B4C(void)
{
    if (!IsUpdateLinkStateCBActive()
        && IsMultiBattle() == TRUE
        && gReceivedRemoteLinkPlayers == 1
        && (sLastViewedMonIndex >= 4 || sLastViewedMonIndex == 1))
        return TRUE;

    return FALSE;
}

static void sub_8138B8C(struct Pokemon * mon)
{
    if (!sMonSummaryScreen->isBoxMon)
    {
        struct Pokemon * partyMons = sMonSummaryScreen->monList.mons;
        *mon = partyMons[GetLastViewedMonIndex()];
    }
    else
    {
        struct BoxPokemon * boxMons = sMonSummaryScreen->monList.boxMons;
        BoxMonToMon(&boxMons[GetLastViewedMonIndex()], mon);
    }
}

static u16 PSS_GetMonMove(struct Pokemon * mon, u8 moveSlot)
{
    u16 move;

    switch (moveSlot)
    {
    case 0:
        move = GetMonData(mon, MON_DATA_MOVE1);
        break;
    case 1:
        move = GetMonData(mon, MON_DATA_MOVE2);
        break;
    case 2:
        move = GetMonData(mon, MON_DATA_MOVE3);
        break;
    default:
        move = GetMonData(mon, MON_DATA_MOVE4);
    }

    return move;
}

static u16 PSS_GetMovePP(struct Pokemon * mon, u8 moveSlot)
{
    u16 pp;

    switch (moveSlot)
    {
    case 0:
        pp = GetMonData(mon, MON_DATA_PP1);
        break;
    case 1:
        pp = GetMonData(mon, MON_DATA_PP2);
        break;
    case 2:
        pp = GetMonData(mon, MON_DATA_PP3);
        break;
    default:
        pp = GetMonData(mon, MON_DATA_PP4);
    }
    return pp;
}

static u8 PSS_CheckMonStatus(u32 status)
{
    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP) == 0)
        return AILMENT_FNT;

    if ((status & STATUS1_PSN_ANY) != 0)
        return AILMENT_PSN;

    if ((status & STATUS1_PARALYSIS) != 0)
        return AILMENT_PRZ;

    if ((status & STATUS1_SLEEP) != 0)
        return AILMENT_SLP;

    if ((status & STATUS1_FREEZE) != 0)
        return AILMENT_FRZ;

    if ((status & STATUS1_BURN) != 0)
        return AILMENT_BRN;

    if (CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0))
        return AILMENT_PKRS;

    return AILMENT_NONE;
}

static void sub_8138CD8(u8 id)
{
    u8 i;

    switch (sMonSummaryScreen->unk3288)
    {
    case 0:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        if (JOY_NEW(DPAD_UP))
        {
            if (sUnknown_203B16D > 0)
            {
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);

                for (i = sUnknown_203B16D; i > 0; i--)
                    if (sMonSummaryScreen->currentMove[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i - 1;
                        return;
                    }
            }
            else
            {
                sUnknown_203B16D = 4;
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);

                if (sMonSummaryScreen->unk3268 == TRUE)
                    for (i = sUnknown_203B16D; i > 0; i--)
                        if (sMonSummaryScreen->currentMove[i - 1] != 0)
                        {
                            PlaySE(SE_SELECT);
                            sUnknown_203B16D = i - 1;
                            return;
                        }
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sUnknown_203B16D < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->unk3288 = 2;

                if (sMonSummaryScreen->unk3268 == TRUE)
                {
                    if (sUnknown_203B16D == 5 - 2)
                    {
                        sUnknown_203B16D = 0;
                        sMonSummaryScreen->unk3288 = 2;
                        PlaySE(SE_SELECT);
                        return;
                    }
                    v0--;
                }

                for (i = sUnknown_203B16D; i < v0; i++)
                    if (sMonSummaryScreen->currentMove[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->unk3268)
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = i;
                }
                else
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = 0;
                }

                return;
            }
            else if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sUnknown_203B16E = 0;
                sMonSummaryScreen->unk3268 = FALSE;
                sub_813A0E8(1);
                sMonSummaryScreen->unk3224 = 0;
                PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
                sMonSummaryScreen->curPageIndex--;
                sMonSummaryScreen->unk3288 = 1;
                return;
            }

            if (sMonSummaryScreen->unk3268 != TRUE)
            {
                if (sMonSummaryScreen->isEnemyParty == FALSE
                    && gMain.inBattle == 0
                    && gReceivedRemoteLinkPlayers == 0)
                {
                    sUnknown_203B16E = sUnknown_203B16D;
                    sMonSummaryScreen->unk3268 = TRUE;
                }
                return;
            }
            else
            {
                sMonSummaryScreen->unk3268 = FALSE;

                if (sUnknown_203B16D == sUnknown_203B16E)
                    return;

                if (sMonSummaryScreen->isBoxMon == 0)
                    sub_81390B0();
                else
                    sub_81391EC();

                sub_8139328(&sMonSummaryScreen->currentMon);
                sub_81367B0();
                sMonSummaryScreen->unk3288 = 2;
                return;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (sMonSummaryScreen->unk3268 == TRUE)
            {
                sUnknown_203B16E = sUnknown_203B16D;
                sMonSummaryScreen->unk3268 = FALSE;
                return;
            }

            if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sUnknown_203B16E = 0;
            }

            sub_813A0E8(1);
            sMonSummaryScreen->unk3224 = 0;
            PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
            sMonSummaryScreen->curPageIndex--;
            sMonSummaryScreen->unk3288 = 1;
        }
        break;
    case 1:
        gTasks[sMonSummaryScreen->task].func = sub_81351A0;
        sMonSummaryScreen->unk3288 = 0;
        break;
    case 2:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk3288 = 3;
        break;
    case 3:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 0;
        break;
    default:
        break;
    }
}

static void sub_81390B0(void)
{
    struct Pokemon * partyMons;
    struct Pokemon * mon;

    u16 move1, move2;
    u8 pp1, pp2;
    u8 allMovesPPBonuses;
    u8 move1ppBonus, move2ppBonus;

    partyMons = sMonSummaryScreen->monList.mons;
    mon = &partyMons[GetLastViewedMonIndex()];

    move1 = GetMonData(mon, MON_DATA_MOVE1 + sUnknown_203B16D);
    move2 = GetMonData(mon, MON_DATA_MOVE1 + sUnknown_203B16E);

    pp1 = GetMonData(mon, MON_DATA_PP1 + sUnknown_203B16D);
    pp2 = GetMonData(mon, MON_DATA_PP1 + sUnknown_203B16E);

    allMovesPPBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);

    move1ppBonus = (allMovesPPBonuses & gPPUpGetMask[sUnknown_203B16D]) >> (sUnknown_203B16D * 2);
    move2ppBonus = (allMovesPPBonuses & gPPUpGetMask[sUnknown_203B16E]) >> (sUnknown_203B16E * 2);

    allMovesPPBonuses &= ~gPPUpGetMask[sUnknown_203B16D];
    allMovesPPBonuses &= ~gPPUpGetMask[sUnknown_203B16E];
    allMovesPPBonuses |= (move1ppBonus << (sUnknown_203B16E * 2)) + (move2ppBonus << (sUnknown_203B16D * 2));

    SetMonData(mon, MON_DATA_MOVE1 + sUnknown_203B16D, (u8 *)&move2);
    SetMonData(mon, MON_DATA_MOVE1 + sUnknown_203B16E, (u8 *)&move1);
    SetMonData(mon, MON_DATA_PP1 + sUnknown_203B16D, &pp2);
    SetMonData(mon, MON_DATA_PP1 + sUnknown_203B16E, &pp1);
    SetMonData(mon, MON_DATA_PP_BONUSES, &allMovesPPBonuses);
}

static void sub_81391EC(void)
{
    struct BoxPokemon * boxMons;
    struct BoxPokemon * boxMon;

    u16 move1, move2;
    u8 pp1, pp2;
    u8 allMovesPPBonuses;
    u8 move1ppBonus, move2ppBonus;

    boxMons = sMonSummaryScreen->monList.boxMons;
    boxMon = &boxMons[GetLastViewedMonIndex()];

    move1 = GetBoxMonData(boxMon, MON_DATA_MOVE1 + sUnknown_203B16D);
    move2 = GetBoxMonData(boxMon, MON_DATA_MOVE1 + sUnknown_203B16E);

    pp1 = GetBoxMonData(boxMon, MON_DATA_PP1 + sUnknown_203B16D);
    pp2 = GetBoxMonData(boxMon, MON_DATA_PP1 + sUnknown_203B16E);

    allMovesPPBonuses = GetBoxMonData(boxMon, MON_DATA_PP_BONUSES);

    move1ppBonus = (allMovesPPBonuses & gPPUpGetMask[sUnknown_203B16D]) >> (sUnknown_203B16D * 2);
    move2ppBonus = (allMovesPPBonuses & gPPUpGetMask[sUnknown_203B16E]) >> (sUnknown_203B16E * 2);

    allMovesPPBonuses &= ~gPPUpGetMask[sUnknown_203B16D];
    allMovesPPBonuses &= ~gPPUpGetMask[sUnknown_203B16E];
    allMovesPPBonuses |= (move1ppBonus << (sUnknown_203B16E * 2)) + (move2ppBonus << (sUnknown_203B16D * 2));

    SetBoxMonData(boxMon, MON_DATA_MOVE1 + sUnknown_203B16D, (u8 *)&move2);
    SetBoxMonData(boxMon, MON_DATA_MOVE1 + sUnknown_203B16E, (u8 *)&move1);
    SetBoxMonData(boxMon, MON_DATA_PP1 + sUnknown_203B16D, &pp2);
    SetBoxMonData(boxMon, MON_DATA_PP1 + sUnknown_203B16E, &pp1);
    SetBoxMonData(boxMon, MON_DATA_PP_BONUSES, &allMovesPPBonuses);
}

static void sub_8139328(struct Pokemon * mon)
{
    if (!sMonSummaryScreen->isBoxMon)
    {
        struct Pokemon * partyMons;
        partyMons = sMonSummaryScreen->monList.mons;
        *mon = partyMons[GetLastViewedMonIndex()];
    }
    else
    {
        struct BoxPokemon * boxMons;
        boxMons = sMonSummaryScreen->monList.boxMons;
        BoxMonToMon(&boxMons[GetLastViewedMonIndex()], mon);
    }
}

static u8 sub_8139388(void)
{
    u16 move;

    move = PSS_GetMonMove(&sMonSummaryScreen->currentMon, sUnknown_203B16D);

    if (IsMoveHm(move) == TRUE && sMonSummaryScreen->mode != PSS_MODE_FORGET_MOVE)
        return FALSE;

    return TRUE;
}

static void sub_81393D4(u8 taskId)
{
    u8 i;

    switch (sMonSummaryScreen->unk3288)
    {
    case 0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->unk3288++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            PSS_PlayMonCry();
            sMonSummaryScreen->unk3288++;
        }
        break;
    case 2:
        if (JOY_NEW(DPAD_UP))
        {
            if (sUnknown_203B16D > 0)
            {
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                for (i = sUnknown_203B16D; i > 0; i--)
                    if (sMonSummaryScreen->currentMove[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i - 1;
                        return;
                    }
            }
            else
            {
                sUnknown_203B16D = 4;
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sUnknown_203B16D < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->unk3288 = 3;

                if (sMonSummaryScreen->unk3268 == TRUE)
                    v0--;

                for (i = sUnknown_203B16D; i < v0; i++)
                    if (sMonSummaryScreen->currentMove[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->unk3268)
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = i;
                }

                return;
            }
            else if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            if (sub_8139388() == TRUE || sUnknown_203B16D == 4)
            {
                PlaySE(SE_SELECT);
                sUnknown_203B16E = sUnknown_203B16D;
                gSpecialVar_0x8005 = sUnknown_203B16E;
                sMonSummaryScreen->unk3288 = 6;
            }
            else
            {
                PlaySE(SE_FAILURE);
                sMonSummaryScreen->unk3288 = 5;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            sUnknown_203B16E = 4;
            gSpecialVar_0x8005 = (u16)sUnknown_203B16E;
            sMonSummaryScreen->unk3288 = 6;
        }
        break;
    case 3:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk3288 = 4;
        break;
    case 4:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 2;
        break;
    case 5:
        FillWindowPixelBuffer(sMonSummaryScreen->window[4], 0);
        AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,
                                     7, 42,
                                     0, 0,
                                     sPSSTextColours[DARK], TEXT_SPEED_FF,
                                     gText_8419CB9);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 2;
        break;
    case 6:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->unk3288++;
        break;
    default:
        if (!gPaletteFade.active)
            sub_8137E64(taskId);
        break;
    }
}

static void sub_8139768(struct Sprite * sprite)
{
    if (sMonSummaryScreen->unk3020 >= 2)
        return;

    if (sUnknown_203B170->tileTag++ >= 2)
    {
        u8 v0;

        switch (sUnknown_203B170->unk08)
        {
        case 0:
            sprite->pos1.y += sUnknown_8463FBE[sUnknown_203B170->unk00++];
            v0 = NELEMS(sUnknown_8463FBE);
            break;
        case 1:
            sprite->pos1.y += sUnknown_8463FC1[sUnknown_203B170->unk00++];
            v0 = NELEMS(sUnknown_8463FC1);
            break;
        case 2:
            sprite->pos1.y += sUnknown_8463FC6[sUnknown_203B170->unk00++];
            v0 = NELEMS(sUnknown_8463FC6);
            break;
        case 3:
        default:
            sprite->pos1.y += sUnknown_8463FCD[sUnknown_203B170->unk00++];
            v0 = NELEMS(sUnknown_8463FCD);
            break;
        }

        if (sUnknown_203B170->unk00 >= v0)
        {
            sUnknown_203B170->unk00 = 0;
            sMonSummaryScreen->unk3020++;
        }

        sUnknown_203B170->tileTag = 0;
    }
}

static void sub_8139868(struct Sprite * sprite)
{
    if (sMonSummaryScreen->unk3020 >= 2)
        return;

    switch (sUnknown_203B170->unk08)
    {
    case 0:
    default:
        if (sUnknown_203B170->tileTag++ >= 120)
        {
            sprite->pos1.x += sUnknown_8463FD4[sUnknown_203B170->unk00];
            if (++sUnknown_203B170->unk00 >= NELEMS(sUnknown_8463FD4))
            {
                sUnknown_203B170->unk00 = 0;
                sUnknown_203B170->tileTag = 0;
                sMonSummaryScreen->unk3020++;
            }
        }
        break;
    case 1:
        if (sUnknown_203B170->tileTag++ >= 90)
        {
            sprite->pos1.x += sUnknown_8463FDF[sUnknown_203B170->unk00];
            if (++sUnknown_203B170->unk00 >= NELEMS(sUnknown_8463FDF))
            {
                sUnknown_203B170->unk00 = 0;
                sUnknown_203B170->tileTag = 0;
                sMonSummaryScreen->unk3020++;
            }
        }
        break;
    case 2:
        if (sUnknown_203B170->tileTag++ >= 60)
        {
            sprite->pos1.x += sUnknown_8463FEA[sUnknown_203B170->unk00];
            if (++sUnknown_203B170->unk00 >= NELEMS(sUnknown_8463FEA))
            {
                sUnknown_203B170->unk00 = 0;
                sUnknown_203B170->tileTag = 0;
                sMonSummaryScreen->unk3020++;
            }
        }
        break;
    }
}

static void nullsub_96(struct Sprite * sprite)
{
}

static void PSS_LoadMonSprite(void)
{
    u16 spriteId;
    u16 species;
    u32 personality;
    u32 trainerId;

    sUnknown_203B170 = AllocZeroed(sizeof(struct Struct203B170));

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY);
    trainerId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            spriteId = CreateMonPicSprite(species, trainerId, personality, 1, 204, 78, 12, 0xffff, 1);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, 1, 204, 78, 12, 0xffff);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            spriteId = CreateMonPicSprite(species, trainerId, personality, 1, 204, 78, 12, 0xffff, 1);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, 1, 204, 78, 12, 0xffff);
    }

    FreeSpriteOamMatrix(&gSprites[spriteId]);
    sMonSummaryScreen->spriteId_1 = spriteId;
    PSS_SetInvisibleMonSprite(1);
    sub_8139AAC(spriteId);
}

static void sub_8139AAC(u16 spriteId)
{
    u16 curHp;
    u16 maxHp;

    sMonSummaryScreen->unk3020 = 0;

    if (sMonSummaryScreen->isEgg == TRUE)
    {
        u8 friendship = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_FRIENDSHIP);

        if (friendship <= 5)
            sUnknown_203B170->unk08 = 2;
        else
        {
            if (friendship <= 10)
                sUnknown_203B170->unk08 = 1;
            else if (friendship <= 40)
                sUnknown_203B170->unk08 = 0;
        }

        gSprites[spriteId].callback = sub_8139868;
        return;
    }

    if (sMonSummaryScreen->monStatus != AILMENT_NONE && sMonSummaryScreen->monStatus != AILMENT_PKRS)
    {
        if (sMonSummaryScreen->monStatus == AILMENT_FNT)
            return;

        gSprites[spriteId].callback = nullsub_96;
        return;
    }

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);

    if (curHp == maxHp)
        sUnknown_203B170->unk08 = 3;
    else if (maxHp * 0.8 <= curHp)
        sUnknown_203B170->unk08 = 2;
    else if (maxHp * 0.6 <= curHp)
        sUnknown_203B170->unk08 = 1;
    else
        sUnknown_203B170->unk08 = 0;

    gSprites[spriteId].callback = sub_8139768;
}

static void PSS_SetInvisibleMonSprite(u8 invisible)
{
    gSprites[sMonSummaryScreen->spriteId_1].invisible = invisible;
}

static void PSS_UnloadMonSprite(void)
{
    FreeAndDestroyMonPicSprite(sMonSummaryScreen->spriteId_1);
    FREE_AND_SET_NULL(sUnknown_203B170);
}

static void PSS_LoadPokeball(void)
{
    u16 ballItemId;
    u8 ballId;

    if (!sMonSummaryScreen->isEgg)
        ballItemId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_POKEBALL);
    else
        ballItemId = 0;

    ballId = ItemIdToBallId(ballItemId);
    LoadBallGfx(ballId);

    sMonSummaryScreen->spriteId_0 = CreateSprite(&gBallSpriteTemplates[ballId], 232, 39, 0);
    gSprites[sMonSummaryScreen->spriteId_0].callback = SpriteCallbackDummy;
    gSprites[sMonSummaryScreen->spriteId_0].oam.priority = 0;

    PSS_SetInvisiblePokeball(1);
}

static void PSS_SetInvisiblePokeball(u8 invisible)
{
    gSprites[sMonSummaryScreen->spriteId_0].invisible = invisible;
}

static void sub_8139D90(void)
{
    DestroySpriteAndFreeResources2(&gSprites[sMonSummaryScreen->spriteId_0]);
}

static void PSS_LoadMonIcon(void)
{
    u16 species;
    u32 personality;

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY);

    SafeLoadMonIconPalette(species);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }

    if (!IsPokeSpriteNotFlipped(species))
        gSprites[sMonSummaryScreen->spriteId_2].hFlip = FALSE;
    else
        gSprites[sMonSummaryScreen->spriteId_2].hFlip = TRUE;

    sub_8139EE4(1);
}

static void sub_8139EE4(u8 invisible)
{
    gSprites[sMonSummaryScreen->spriteId_2].invisible = invisible;
}

static void sub_8139F20(void)
{
    u16 species;
    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    SafeFreeMonIconPalette(species);
    DestroyMonIcon(&gSprites[sMonSummaryScreen->spriteId_2]);
}

static void PSS_LoadMovesSelectCursor(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtrs[2];
    gfxBufferPtrs[0] = AllocZeroed(0x20 * 64);
    gfxBufferPtrs[1] = AllocZeroed(0x20 * 64);

    sUnknown_203B148[0] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[1] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[2] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[3] = AllocZeroed(sizeof(struct Struct203B148));

    LZ77UnCompWram(gSelectCursorGfxLeft, gfxBufferPtrs[0]);
    LZ77UnCompWram(gSelectCursorGfxRight, gfxBufferPtrs[1]);

    for (i = 0; i < 4; i++)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtrs[i % 2],
            .size = 0x20 * 64,
            .tag = tileTag + i
        };

        struct SpritePalette palette = {.data = gSelectCursorPalette, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag + i,
            .paletteTag = palTag,
            .oam = &sUnknown_846398C,
            .anims = sUnknown_84639A4,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = sub_813A124,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 64 * (i % 2) + 32, sUnknown_203B16D * 28 + 34, i % 2);
        sUnknown_203B148[i]->sprite = &gSprites[spriteId];
        sUnknown_203B148[i]->tileTag = i;
        sUnknown_203B148[i]->palTag = tileTag + i;
        sUnknown_203B148[i]->unk08 = palTag;
        sUnknown_203B148[i]->sprite->subpriority = i;

        if (i > 1)
            StartSpriteAnim(sUnknown_203B148[i]->sprite, 1);
    }

    sub_813A0E8(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[0]);
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[1]);
}

static void sub_813A0E8(u8 invisible)
{
    u8 i;
    for (i = 0; i < 4; i++)
        sUnknown_203B148[i]->sprite->invisible = invisible;
}

static void sub_813A124(struct Sprite * sprite)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (sMonSummaryScreen->unk3268 == TRUE && i > 1)
            continue;

        sUnknown_203B148[i]->sprite->pos1.y = sUnknown_203B16D * 28 + 34;
    }

    if (sMonSummaryScreen->unk3268 != TRUE)
    {
        if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
        {
            sUnknown_203B148[0]->sprite->invisible = FALSE;
            sUnknown_203B148[1]->sprite->invisible = FALSE;
        }
        return;
    }

    for (i = 0; i < 2; i++)
    {
        sprite = sUnknown_203B148[i]->sprite;
        sprite->data[0]++;

        if (sprite->invisible)
        {
            if (sprite->data[0] > 60)
            {
                sprite->invisible = FALSE;
                sprite->data[0] = 0;
            }
        }
        else if (sprite->data[0] > 60)
        {
            sprite->invisible = TRUE;
            sprite->data[0] = 0;
        }
    }
}

static void sub_813A21C(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (sUnknown_203B148[i]->sprite != NULL)
            DestroySpriteAndFreeResources(sUnknown_203B148[i]->sprite);

        FREE_AND_SET_NULL_IF_SET(sUnknown_203B148[i]);
    }
}

static void PSS_LoadIconStatus(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sStatusIconSummaryScreen = AllocZeroed(sizeof(struct Struct203B158));
    gfxBufferPtr = AllocZeroed(0x20 * 32);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		LZ77UnCompWram(gIconStatusSummaryScreen, gfxBufferPtr);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		LZ77UnCompWram(gIconStatusSummaryScreenSpa, gfxBufferPtr);

    if (sStatusIconSummaryScreen != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 32,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = gIconStatusPalSummaryScreen, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_84639AC,
            .anims = sUnknown_84639F4,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 0, 0, 0);
        sStatusIconSummaryScreen->sprite = &gSprites[spriteId];
        sStatusIconSummaryScreen->tileTag = tileTag;
        sStatusIconSummaryScreen->palTag = palTag;
    }

    PSS_SetInvisibleIconStatus(1);
    PSS_ShowIconStatus();
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void PSS_UnloadIconStatus(void)
{
    if (sStatusIconSummaryScreen->sprite != NULL)
        DestroySpriteAndFreeResources(sStatusIconSummaryScreen->sprite);

    FREE_AND_SET_NULL_IF_SET(sStatusIconSummaryScreen);
}

static void PSS_ShowIconStatus(void)
{
    sMonSummaryScreen->monStatus = PSS_CheckMonStatus(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_STATUS));

    if (sMonSummaryScreen->monStatus == AILMENT_NONE)
    {
        PSS_SetInvisibleIconStatus(1);
        return;
    }

    StartSpriteAnim(sStatusIconSummaryScreen->sprite, sMonSummaryScreen->monStatus - 1);
    PSS_SetInvisibleIconStatus(0);
}

static void PSS_SetInvisibleIconStatus(u8 invisible)
{
    if (sMonSummaryScreen->monStatus == AILMENT_NONE || sMonSummaryScreen->isEgg)
        sStatusIconSummaryScreen->sprite->invisible = TRUE;
    else
        sStatusIconSummaryScreen->sprite->invisible = invisible;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sStatusIconSummaryScreen->sprite->pos1.y != 45)
        {
            sStatusIconSummaryScreen->sprite->pos1.x = 16;
            sStatusIconSummaryScreen->sprite->pos1.y = 45;
            return;
        }
    }
    else if (sStatusIconSummaryScreen->sprite->pos1.y != 38)
    {
        sStatusIconSummaryScreen->sprite->pos1.x = 214;
        sStatusIconSummaryScreen->sprite->pos1.y = 38;
        return;
    }
}

static void PSS_LoadHpBar(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;
    u32 curHp;
    u32 maxHp;
    u8 hpBarPalTagOffset = 0;

    sHpBarSummaryScreen = AllocZeroed(sizeof(struct Struct203B15C));
    gfxBufferPtr = AllocZeroed(0x20 * 12);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		LZ77UnCompWram(gHpBarSummaryScreen, gfxBufferPtr);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		LZ77UnCompWram(gHpBarSummaryScreenSpa, gfxBufferPtr);

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);

    if (maxHp / 4 > curHp)
        hpBarPalTagOffset = 2;
    else if (maxHp / 2 > curHp)
        hpBarPalTagOffset = 1;

    if (gfxBufferPtr != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 12,
            .tag = tileTag
        };

        struct SpritePalette palette1 = {.data = sHpBarPalettes[0], .tag = palTag};
        struct SpritePalette palette2 = {.data = sHpBarPalettes[1], .tag = palTag + 1};
        struct SpritePalette palette3 = {.data = sHpBarPalettes[2], .tag = palTag + 2};

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette1);
        LoadSpritePalette(&palette2);
        LoadSpritePalette(&palette3);
    }

    for (i = 0; i < 9; i++)
    {
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag + hpBarPalTagOffset,
            .oam = &sUnknown_8463A14,
            .anims = sUnknown_8463A7C,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sHpBarSummaryScreen->cordX[i] = i * 8 + 81;
        spriteId = CreateSprite(&template, sHpBarSummaryScreen->cordX[i], 33, 0);
        sHpBarSummaryScreen->sprites[i] = &gSprites[spriteId];
        sHpBarSummaryScreen->sprites[i]->invisible = FALSE;
        sHpBarSummaryScreen->sprites[i]->oam.priority = 2;
        sHpBarSummaryScreen->unk3C = tileTag;
        sHpBarSummaryScreen->unk3E = palTag;
        StartSpriteAnim(sHpBarSummaryScreen->sprites[i], 8);
    }

    PSS_SetupHpBar();
    PSS_SetInvisibleHpBar(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void PSS_SetupHpBar(void)
{
    u8 numWholeHpBarTiles = 0;
    u8 i;
    u8 animNum;
    u8 two = 2;
    u8 hpBarPalOffset = 0;
    u32 curHp;
    u32 maxHp;
    s64 v0;
    s64 v1;

    if (sMonSummaryScreen->isEgg)
        return;

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);

    if (maxHp / 5 >= curHp)
        hpBarPalOffset = 2;
    else if (maxHp / 2 >= curHp)
        hpBarPalOffset = 1;

    switch (GetHPBarLevel(curHp, maxHp))
    {
    case 3:
    default:
        hpBarPalOffset = 0;
        break;
    case 2:
        hpBarPalOffset = 1;
        break;
    case 1:
        hpBarPalOffset = 2;
        break;
    }

    for (i = 0; i < 9; i++)
        sHpBarSummaryScreen->sprites[i]->oam.paletteNum = IndexOfSpritePaletteTag(TAG_PSS_UNK_78) + hpBarPalOffset;

    if (curHp == maxHp)
        for (i = two; i < 8; i++)
            StartSpriteAnim(sHpBarSummaryScreen->sprites[i], 8);

    else
    {
        v0 = (maxHp << 2) / 6;
        v1 = (curHp << 2);

        while (TRUE)
        {
            if (v1 <= v0)
                break;
            v1 -= v0;
            numWholeHpBarTiles++;
        }

        numWholeHpBarTiles += two;

        for (i = two; i < numWholeHpBarTiles; i++)
            StartSpriteAnim(sHpBarSummaryScreen->sprites[i], 8);

        animNum = (v1 * 6) / v0;
        StartSpriteAnim(sHpBarSummaryScreen->sprites[numWholeHpBarTiles], animNum);

        for (i = numWholeHpBarTiles + 1; i < 8; i++)
            StartSpriteAnim(sHpBarSummaryScreen->sprites[i], 0);
    }

    StartSpriteAnim(sHpBarSummaryScreen->sprites[0], 9);
    StartSpriteAnim(sHpBarSummaryScreen->sprites[1], 10);
    StartSpriteAnim(sHpBarSummaryScreen->sprites[8], 11);
}

static void PSS_UnloadHpBar(void)
{
    u8 i;

    for (i = 0; i < 9; i++)
        if (sHpBarSummaryScreen->sprites[i] != NULL)
            DestroySpriteAndFreeResources(sHpBarSummaryScreen->sprites[i]);

    FREE_AND_SET_NULL_IF_SET(sHpBarSummaryScreen);
}

static void PSS_SetInvisibleHpBar(u8 invisible)
{
    u8 i;

    for (i = 0; i < 9; i++)
        sHpBarSummaryScreen->sprites[i]->invisible = invisible;
}

static void PSS_LoadExpBar(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;

    sExpBarSummaryScreen = AllocZeroed(sizeof(struct Struct203B160));
    gfxBufferPtr = AllocZeroed(0x20 * 12);

    LZ77UnCompWram(gExpBarSummaryScreen, gfxBufferPtr);
    if (gfxBufferPtr != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 12,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = gPal0SummaryScreen, .tag = palTag};
        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
    }

    for (i = 0; i < 11; i++)
    {
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_8463A14,
            .anims = sUnknown_8463A7C,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sExpBarSummaryScreen->cordX[i] = i * 8 + 156;
        spriteId = CreateSprite(&template, sExpBarSummaryScreen->cordX[i], 130, 0);
        sExpBarSummaryScreen->sprites[i] = &gSprites[spriteId];
        sExpBarSummaryScreen->sprites[i]->oam.priority = 2;
        sExpBarSummaryScreen->tileTag = tileTag;
        sExpBarSummaryScreen->palTag = palTag;
    }

    PSS_SetupExpBar();
    PSS_SetInvisibleExpBar(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void PSS_SetupExpBar(void)
{
    u8 numWholeExpBarTiles = 0;
    u8 i;
    u8 level;
    u32 exp;
    u32 totalExpToNextLevel;
    u32 curExpToNextLevel;
    u16 species;
    s64 v0;
    s64 v1;
    u8 animNum;
    u8 two = 2;

    if (sMonSummaryScreen->isEgg)
        return;

    exp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EXP);
    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL);
    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);

    if (level < 100)
    {
        totalExpToNextLevel = gExperienceTables[gBaseStats[species].growthRate][level + 1] - gExperienceTables[gBaseStats[species].growthRate][level];
        curExpToNextLevel = exp - gExperienceTables[gBaseStats[species].growthRate][level];
        v0 = ((totalExpToNextLevel << 2) / 8);
        v1 = (curExpToNextLevel << 2);

        while (TRUE)
        {
            if (v1 <= v0)
                break;
            v1 -= v0;
            numWholeExpBarTiles++;
        }

        numWholeExpBarTiles += two;

        for (i = two; i < numWholeExpBarTiles; i++)
            StartSpriteAnim(sExpBarSummaryScreen->sprites[i], 8);

        if (numWholeExpBarTiles >= 10)
        {
            if (totalExpToNextLevel == curExpToNextLevel)
                return;
            else
                StartSpriteAnim(sExpBarSummaryScreen->sprites[9], 7);
        }

        animNum = (v1 * 8) / v0;
        StartSpriteAnim(sExpBarSummaryScreen->sprites[numWholeExpBarTiles], animNum);

        for (i = numWholeExpBarTiles + 1; i < 10; i++)
            StartSpriteAnim(sExpBarSummaryScreen->sprites[i], 0);
    }
    else
        for (i = two; i < 10; i++)
            StartSpriteAnim(sExpBarSummaryScreen->sprites[i], 0);

    StartSpriteAnim(sExpBarSummaryScreen->sprites[0], 9);
    StartSpriteAnim(sExpBarSummaryScreen->sprites[1], 10);
    StartSpriteAnim(sExpBarSummaryScreen->sprites[10], 11);
}

static void sub_813AB38(void)
{
    u8 i;

    for (i = 0; i < 11; i++)
        if (sExpBarSummaryScreen->sprites[i] != NULL)
            DestroySpriteAndFreeResources(sExpBarSummaryScreen->sprites[i]);

    FREE_AND_SET_NULL_IF_SET(sExpBarSummaryScreen);
}

static void PSS_SetInvisibleExpBar(u8 invisible)
{
    u8 i;

    for (i = 0; i < 11; i++)
        sExpBarSummaryScreen->sprites[i]->invisible = invisible;
}

static void PSS_LoadUnkPoint(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sUnknown_203B164 = AllocZeroed(sizeof(struct Struct203B164));
    gfxBufferPtr = AllocZeroed(0x20 * 1);

    LZ77UnCompWram(sUnknown_8463B20, gfxBufferPtr);

    if (sUnknown_203B164 != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 1,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = sUnknown_8463B00, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_8463AEC,
            .anims = sUnknown_8463AFC,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 114, 92, 0);
        sUnknown_203B164->sprite = &gSprites[spriteId];
        sUnknown_203B164->tileTag = tileTag;
        sUnknown_203B164->palTag = palTag;
    }

    sub_813ACF8(1);
    sub_813ACB4();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void sub_813AC8C(void)
{
    if (sUnknown_203B164->sprite != NULL)
        DestroySpriteAndFreeResources(sUnknown_203B164->sprite);

    FREE_AND_SET_NULL_IF_SET(sUnknown_203B164);
}

static void sub_813ACB4(void)
{
    if (!CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0)
        && CheckPartyHasHadPokerus(&sMonSummaryScreen->currentMon, 0))
        sub_813ACF8(0);
    else
        sub_813ACF8(1);
}

static void sub_813ACF8(u8 invisible)
{
    if (!CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0)
        && CheckPartyHasHadPokerus(&sMonSummaryScreen->currentMon, 0))
    {
        sUnknown_203B164->sprite->invisible = invisible;
        return;
    }
    else
        sUnknown_203B164->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sUnknown_203B164->sprite->invisible = TRUE;
        sUnknown_203B164->sprite->pos1.x = 16;
        sUnknown_203B164->sprite->pos1.y = 44;
    }
    else
    {
        sUnknown_203B164->sprite->pos1.x = 114;
        sUnknown_203B164->sprite->pos1.y = 92;
    }
}

static void PSS_LoadShinyIndicator(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sUnknown_203B168 = AllocZeroed(sizeof(struct Struct203B168));
    gfxBufferPtr = AllocZeroed(0x20 * 2);

    LZ77UnCompWram(gShinyIndicatorGfx, gfxBufferPtr);

    if (sUnknown_203B168 != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 2,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = gShinyIndicatorPalette, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_8463B30,
            .anims = sUnknown_8463B40,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
        spriteId = CreateSprite(&template, 166, 50, 0);
        sUnknown_203B168->sprite = &gSprites[spriteId];
        sUnknown_203B168->tileTag = tileTag;
        sUnknown_203B168->palTag = palTag;
    }

    sub_813AEB0(1);
    sub_813AF50();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void sub_813AE88(void)
{
    if (sUnknown_203B168->sprite != NULL)
        DestroySpriteAndFreeResources(sUnknown_203B168->sprite);

    FREE_AND_SET_NULL_IF_SET(sUnknown_203B168);
}

static void sub_813AEB0(u8 invisible)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE
        && !sMonSummaryScreen->isEgg)
        sUnknown_203B168->sprite->invisible = invisible;
    else
        sUnknown_203B168->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sUnknown_203B168->sprite->pos1.x = 126;
        sUnknown_203B168->sprite->pos1.y = 20;
    }
    else
    {
        sUnknown_203B168->sprite->pos1.x = 166;
        sUnknown_203B168->sprite->pos1.y = 50;
    }
}

static void sub_813AF50(void)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE && !sMonSummaryScreen->isEgg)
        sub_813AEB0(0);
    else
        sub_813AEB0(1);
}

static void sub_813AF90(void)
{
    sub_813A21C();
    PSS_UnloadHpBar();
    sub_813AB38();
    PSS_UnloadMonSprite();
    sub_8139F20();
    sub_8139D90();
    PSS_UnloadMarkings();
    PSS_UnloadIconStatus();
    sub_813AC8C();
    sub_813AE88();
    ResetSpriteData();
}

static void sub_813AFC4(void)
{
    PSS_LoadPokeball();
    PSS_SetInvisiblePokeball(0);
    PSS_LoadMonIcon();
    PSS_LoadMonSprite();
    PSS_SetInvisibleMonSprite(0);
    PSS_SetupHpBar();
    PSS_SetupExpBar();
    PSS_GetMarkings();
    PSS_ShowIconStatus();
    sub_813ACB4();
    sub_813AF50();
}

static void PSS_LoadMarkings(void)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    DestroySpriteAndFreeResources(sMonSummaryScreen->markingSprite);
    sMonSummaryScreen->markingSprite = CreateMonMarkingSprite_SelectCombo(TAG_PSS_UNK_8C, TAG_PSS_UNK_8C, sUnknown_84636E0);

    if (sMonSummaryScreen->markingSprite != NULL)
    {
        StartSpriteAnim(sMonSummaryScreen->markingSprite, markings);
        sMonSummaryScreen->markingSprite->pos1.x = 208;
        sMonSummaryScreen->markingSprite->pos1.y = 114;
    }

    PSS_SetInvisibleMarkings(1);
}

static void PSS_UnloadMarkings(void)
{
    DestroySpriteAndFreeResources(sMonSummaryScreen->markingSprite);
}

static void PSS_SetInvisibleMarkings(u8 invisible)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    if (markings == 0)
        sMonSummaryScreen->markingSprite->invisible = TRUE;
    else
        sMonSummaryScreen->markingSprite->invisible = invisible;
}

static void PSS_GetMarkings(void)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    StartSpriteAnim(sMonSummaryScreen->markingSprite, markings);
    PSS_SetInvisibleMarkings(0);
}

static void sub_813B120(u8 taskId, s8 a1)
{
    s8 v0 = -1;

    if (sMonSummaryScreen->isBoxMon == TRUE)
    {
        if (sMonSummaryScreen->curPageIndex != 0)
        {
            if (a1 == 1)
                a1 = 0;
            else
                a1 = 2;
        }
        else
        {
            if (a1 == 1)
                a1 = 1;
            else
                a1 = 3;
        }

        v0 = SeekToNextMonInBox(sMonSummaryScreen->monList.boxMons, GetLastViewedMonIndex(), sMonSummaryScreen->lastIndex, (u8)a1);
    }
    else
    {
        if (IsUpdateLinkStateCBActive() == FALSE
            && gReceivedRemoteLinkPlayers == 1
            && IsMultiBattle() == TRUE)
            v0 = sub_813B38C(a1);
        else
            v0 = sub_813B20C(a1);
    }

    if (v0 == -1)
        return;

    sLastViewedMonIndex = v0;
    CreateTask(sub_813B3F0, 0);
    sMonSummaryScreen->unk328C = 0;
}

static s8 sub_813B20C(s8 a0)
{
    struct Pokemon * partyMons = sMonSummaryScreen->monList.mons;
    s8 v1 = 0;

    if (sMonSummaryScreen->curPageIndex == 0)
    {
        if (a0 == -1 && sLastViewedMonIndex == 0)
            return -1;
        else if (a0 == 1 && sLastViewedMonIndex >= sMonSummaryScreen->lastIndex)
            return -1;
        else
            return sLastViewedMonIndex + a0;
    }

    while (TRUE)
    {
        v1 += a0;
        if (0 > sLastViewedMonIndex + v1 || sLastViewedMonIndex + v1 > sMonSummaryScreen->lastIndex)
            return -1;

        if (GetMonData(&partyMons[sLastViewedMonIndex + v1], MON_DATA_IS_EGG) == 0)
            return sLastViewedMonIndex + v1;
    }

    return -1;
}

static u8 sub_813B2C8(struct Pokemon * partyMons)
{
    if (GetMonData(partyMons, MON_DATA_SPECIES) != 0 && (sMonSummaryScreen->curPageIndex != 0 || GetMonData(partyMons, MON_DATA_IS_EGG) == 0))
        return TRUE;

    return FALSE;
}

static s8 sub_813B304(u8 a0)
{
    while (TRUE)
    {
        a0++;

        if (a0 == 6)
            return -1;
        if (sub_813B2C8(&gPlayerParty[sUnknown_8463FB8[a0]]) == TRUE)
            break;
    }

    return (s8)sUnknown_8463FB8[a0];
}

static s8 sub_813B348(u8 a0)
{
    while (1)
    {
        if (a0 == 0)
            return -1;

        a0--;

        if (sub_813B2C8(&gPlayerParty[sUnknown_8463FB8[a0]]) == TRUE)
            break;
    }

    return (s8)(sUnknown_8463FB8[a0]);
}

static s8 sub_813B38C(s8 a0)
{
    u8 v0 = 0;
    u8 i;

    for (i = 0; i < PARTY_SIZE; i++)
        if (sUnknown_8463FB8[i] == GetLastViewedMonIndex())
        {
            v0 = i;
            break;
        }

    if ((a0 == -1 && v0 == 0)
        || (a0 == 1 && v0 == 5))
        return -1;

    if (a0 == 1)
        return sub_813B304(v0);
    else
        return sub_813B348(v0);
}

static void sub_813B3F0(u8 id)
{
    switch (sMonSummaryScreen->unk328C)
    {
    case 0:
        StopCryAndClearCrySongs();
        sUnknown_203B16D = 0;
        sUnknown_203B16E = 0;
        sMonSummaryScreen->unk328C++;
        break;
    case 1:
        PSS_UnloadMonSprite();
        sub_8139F20();
        sub_8139D90();
        sMonSummaryScreen->unk328C++;
        break;
    case 2:
        sub_8138B8C(&sMonSummaryScreen->currentMon);

        sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG);
        sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG);

        if (sMonSummaryScreen->isBadEgg == TRUE)
            sMonSummaryScreen->isEgg = TRUE;

        sMonSummaryScreen->unk328C++;
        break;
    case 3:
        sMonSummaryScreen->unk328C++;
        break;
    case 4:
        sMonSummaryScreen->unk328C++;
        break;
    case 5:
        PSS_GetDataPokemon();
        sMonSummaryScreen->unk328C++;
        break;
    case 6:
        if (!sMonSummaryScreen->isEgg)
            PSS_GetStatsPokemon();

        sMonSummaryScreen->unk328C++;
        break;
    case 7:
        if (!sMonSummaryScreen->isEgg)
            sub_81367B0();

        sMonSummaryScreen->unk328C++;
        break;
    case 8:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk328C++;
        break;
    case 9:
        PSS_DrawMonMoveIcon();
        sub_8138538();
        sub_8137D28(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->unk328C++;
        break;
    case 10:
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk328C++;
        break;
    case 11:
        if (!Overworld_LinkRecvQueueLengthMoreThan2() && !sub_800B270())
        {
            sub_813AFC4();
            PSS_PlayMonCry();
            sMonSummaryScreen->unk328C++;
        }
        break;
    default:
        sMonSummaryScreen->unk328C = 0;
        DestroyTask(id);
        break;
    }
}

static void PSS_PlayMonCry(void)
{
    if (!GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG))
    {
        if (ShouldPlayNormalPokeCry(&sMonSummaryScreen->currentMon) == TRUE)
            PlayCry3(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2), 0, 0);
        else
            PlayCry3(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2), 0, 11);
    }
}

static bool32 PSS_IsNatureBoldOrGentle(u8 nature)
{
    if (nature == NATURE_BOLD || nature == NATURE_GENTLE)
        return TRUE;

    return FALSE;
}

static bool32 PSS_IsMonFromGenIII(void)
{
    u8 version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

    if (version == VERSION_LEAF_GREEN || version == VERSION_FIRE_RED || version == VERSION_RUBY || version == VERSION_SAPPHIRE || version == VERSION_EMERALD)
        return TRUE;

    return FALSE;
}

static bool32 sub_813B838(u8 place)
{
    if (place >= MAPSECS_KANTO && place < MAPSEC_NONE)
        return TRUE;
    return FALSE;
}

static void sub_813B854(void)
{
    ShowPokemonSummaryScreen(0, 0, 0, CB2_ReturnToField, PSS_MODE_NORMAL);
}

static void PSS_ScrollPSSBackground(void)
{
    ChangeBgX(2, 80, 1);
    ChangeBgY(2, 80, 1);
}

