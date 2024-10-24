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

#define Hp data[1]
#define atk data[2]
#define def data[3]
#define spA data[4]
#define spD data[5]
#define spe data[6]
#define maxValue data[7]
#define total data[8]
#define totalMax data[9]
#define counter data[10]
#define returnTask data[11]

// needs conflicting header to match (curIndex is s8 in the function, but has to be defined as u8 here)
extern s16 SeekToNextMonInBox(struct BoxPokemon * boxMons, u8 curIndex, u8 maxIndex, u8 flags);

static void BufferSelectedMonData(struct Pokemon * mon);
static void CB2_SetUpPSS(void);
static void PokeSum_TryPlayMonCry(void);
static void PokeSum_RemoveWindows(u8 curPageIndex);
static void Task_PokeSum_FlipPages(u8 taskId);
static void Task_FlipPages_FromInfo(u8 taskId);
static void Task_PokeSum_SwitchDisplayedPokemon(u8 taskId);
static void Task_PokeSum_AnimateIvsEvsStats(u8 taskId);
static void PokeSum_SeekToNextMon(u8 taskId, s8 direction);
static void Task_DestroyResourcesOnExit(u8 taskId);
static void PokeSum_HideSpritesBeforePageFlip(void);
static void PokeSum_ShowSpritesBeforePageFlip(void);
static void PokeSum_AddWindows(u8 curPageIndex);
static void PokeSum_PrintPageHeaderText(u8 curPageIndex);
static u8 PokeSum_IsPageFlipFinished(u8);
static void PokeSum_SetHelpContext(void);
static void Task_HandleInput_SelectMove(u8 taskId);
static void PokeSum_DrawBg3Tilemap(void);
static void PokeSum_PrintRightPaneText(void);
static void PokeSum_PrintBottomPaneText(void);
static void PokeSum_PrintAbilityDataOrMoveTypes(void);
static void PokeSum_PrintMonTypeIcons(void);
static void PokeSum_PrintPageName(const u8 * str);
static void PokeSum_PrintControlsString(const u8 * str);
static void PrintMonLevelNickOnWindow2(const u8 * str);
static void ShowOrHideHpBarObjs(u8 invisible);
static void ShowOrHideExpBarObjs(u8 invisible);
static void ShowOrHideMonPicSprite(u8 invisible);
static void ShowOrHideMonMarkingsSprite(u8 invisible);
static void ShowOrHideBallIconObj(u8 invisible);
static void ShowOrHideStatusIcon(u8 invisible);
static void ShowOrHidePokerusIcon(u8 invisible);
static void ShowOrHideShinyStar(u8 invisible);
static void ShowOrHideMoveSelectionCursor(u8 invisible);
static void ShowOrHideMonIconSprite(u8 invisible);
static void PokeSum_Setup_ResetCallbacks(void);
static void PokeSum_Setup_InitGpu(void);
static void PokeSum_Setup_SpritesReset(void);
static u8 PokeSum_HandleLoadBgGfx(void);
static u8 PokeSum_HandleCreateSprites(void);
static void PokeSum_CreateWindows(void);
static u8 PokeSum_Setup_BufferStrings(void);
static void CommitStaticWindowTilemaps(void);
static void PokeSum_Setup_SetVBlankCallback(void);
static void PokeSum_FinishSetup(void);
static void BufferMonInfo(void);
static void BufferMonSkills(void);
static void BufferMonMoves(void);
static u8 StatusToAilment(u32 status);
static void BufferMonMoveI(u8);
static u16 GetMonMoveBySlotId(struct Pokemon * mon, u8 moveSlot);
static u16 GetMonPpByMoveSlot(struct Pokemon * mon, u8 moveSlot);
static void CreateShinyStarObj(u16, u16);
static void CreatePokerusIconObj(u16, u16);
static void PokeSum_CreateMonMarkingsSprite(void);
static void CreateMoveSelectionCursorObjs(u16, u16);
static void CreateMonStatusIconObj(u16, u16);
static void CreateHpBarObjs(u16, u16);
static void CreateExpBarObjs(u16, u16);
static void CreateBallIconObj(void);
static void CreateMonIconSprite(void);
static void PokeSum_CreateMonPicSprite(void);
static void Task_InputHandler_SelectOrForgetMove(u8 taskId);
static void CB2_RunPokemonSummaryScreen(void);
static void PrintInfoPage(void);
static void PrintSkillsPage(void);
static void PrintIvsPage(void);
static void PrintEvsPage(void);
static void PrintMovesPage(void);
static void PokeSum_PrintMoveNamesAndPP(u8 i);
static void PokeSum_PrintTrainerMemo(void);
static void PokeSum_PrintExpPoints_NextLv(void);
static void PokeSum_PrintSelectedMoveStats(void);
static void PokeSum_PrintTrainerMemo_Mon(void);
static void PokeSum_PrintTrainerMemo_Egg(void);
static bool32 MapSecIsInKantoOrSevii(u8 metLocation);
static bool32 IsMultiBattlePartner(void);
static bool32 PokeSum_IsMonBoldOrGentle(u8 nature);
static void PokeSum_PrintTrainerMemo_Mon_NotHeldByOT(void);
static bool32 CurrentMonIsFromGBA(void);
static u8 PokeSum_BufferOtName_IsEqualToCurrentOwner(struct Pokemon * mon);
static void PokeSum_PrintAbilityNameAndDesc(void);
static void PokeSum_PrintTotalIvsStats(void);
static void PokeSum_PrintTotalEvsStats(void);
static void PokeSum_DrawMoveTypeIcons(void);
static void PokeSum_DestroySprites(void);
static void SwapMonMoveSlots(void);
static void SwapBoxMonMoveSlots(void);
static void UpdateCurrentMonBufferFromPartyOrBox(struct Pokemon * mon);
static void PokeSum_SetMonPicSpriteCallback(u16 spriteId);
static void SpriteCB_MoveSelectionCursor(struct Sprite * sprite);
static void UpdateMonStatusIconObj(void);
static void UpdateHpBarObjs(void);
static void UpdateExpBarObjs(void);
static void ShowPokerusIconObjIfHasOrHadPokerus(void);
static void ShowShinyStarObjIfMonShiny(void);
static void DestroyMonMarkingsSprite(void);
static void PokeSum_UpdateMonMarkingsAnim(void);
static s8 SeekToNextMonInSingleParty(s8);
static s8 SeekToNextMonInMultiParty(s8);
static bool32 IsStatsPagesUnlocked(void);
static void ScrollPSSBackground(void);

struct PokemonSummaryScreenData
{
    u8 windowIds[7];

    u8 ballIconSpriteId;
    u8 monPicSpriteId;
    u8 monIconSpriteId;

    u8 inputHandlerTaskId;
    u8 inhibitPageFlipInput;
    u8 isDrawingIvEvsPage;

    u8 numMonPicBounces;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 speciesNameStrBuf[POKEMON_NAME_LENGTH + 1];
        u8 nicknameStrBuf[POKEMON_NAME_LENGTH + 1];
        u8 otNameStrBuf[12];
        u8 otNameStrBufs[2][12];

        u8 dexNumStrBuf[5];
        u8 otIdStrBuf[7];
        u8 itemNameStrBuf[ITEM_NAME_LENGTH + 1];

        u8 genderSymbolStrBuf[3];
        u8 levelStrBuf[7];
        u8 curHpStrBuf[9];
        u8 statValueStrBufs[5][5];
        u8 statIvsStrBufs[7][5];
        u8 statEvsStrBufs[7][5];

        u8 moveCurPpStrBufs[5][11];
        u8 moveMaxPpStrBufs[5][11];
        u8 moveNameStrBufs[5][MOVE_NAME_LENGTH + 1];
        u8 movePowerStrBufs[5][5];
        u8 moveAccuracyStrBufs[5][5];

        u8 expPointsStrBuf[9];
        u8 expToNextLevelStrBuf[9];

        u8 abilityNameStrBuf[13];
        u8 abilityDescStrBuf[52];
    } summary;

    u8 isEgg; /* 0x3200 */
    u8 isBadEgg; /* 0x3204 */

    u8 mode; /* 0x3208 */
    u8 lastIndex; /* 0x3210 */
    u8 curPageIndex; /* 0x3214 */
    u8 isBoxMon; /* 0x321C */
    u8 typeIcons[2]; /* 0x3220 */

    u8 pageFlipDirection; /* 0x3224 */

    u8 lockMovesFlag; /* 0x3234 */

    u8 whichBgLayerToTranslate; /* 0x3238 */
    u8 skillsPageBgNum; /* 0x323C */
    u8 infoAndMovesPageBgNum; /* 0x3240 */
    u8 flippingPages; /* 0x3244 */

    u16 moveTypes[5]; /* 0x3250 */
    u16 moveIds[5]; /* 0x325A */
    u8 numMoves; /* 0x3264 */
    u8 isSwappingMoves; /* 0x3268 */

    u8 curMonStatusAilment; /* 0x326C */

    u8 state3270; /* 0x3270 */
    u8 summarySetupStep; /* 0x3274 */
    u8 loadBgGfxStep; /* 0x3278 */
    u8 spriteCreationStep; /* 0x327C */
    u8 bufferStringsStep; /* 0x3280 */
    u8 state3284; /* 0x3284 */
    u8 selectMoveInputHandlerState; /* 0x3288 */
    u8 switchMonTaskState; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite * markingSprite;

    u8 lastPageFlipDirection[2]; /* 0x3300 */
};

struct MonSkillsPrinterXpos
{
    u16 dexNumStr;
    u16 curHpStr;
    u16 atkStr;
    u16 defStr;
    u16 spAStr;
    u16 spDStr;
    u16 speStr;
    u16 expStr;
    u16 toNextLevel;
    u16 curPp[5];
    u16 maxPp[5];
};

struct MonStatsIvsPrinterXpos
{
    u16 HpStr;
    u16 atkStr;
    u16 defStr;
    u16 spAStr;
    u16 spDStr;
    u16 speStr;
    u16 totalIvsStr;
};

struct MonStatsEvsPrinterXpos
{
    u16 HpStr;
    u16 atkStr;
    u16 defStr;
    u16 spAStr;
    u16 spDStr;
    u16 speStr;
    u16 totalEvsStr;
};

struct ExpBarObjs
{
    struct Sprite * sprites[11]; /* 0x00 */
    u16 xpos[11]; /* 0x2c */
    u16 tileTag; /* 0x42 */
    u16 palTag; /* 0x44 */
};

struct HpBarObjs
{
    struct Sprite * sprites[10]; /* 0x00 */
    u16 xpos[10]; /* 0x28 */
    u16 tileTag; /* 0x3c */
    u16 palTag; /* 0x3e */
};

struct MonPicBounceState
{
    u8 animFrame; /* 0x00 */
    u8 initDelay; /* 0x04 */
    u8 vigor; /* 0x08 */
};

struct MoveSelectionCursor
{
    struct Sprite * sprite; /* 0x00 */
    u16 whichSprite; /* 0x04 */
    u16 tileTag; /* 0x06 */
    u16 palTag; /* 0x08 */
};

struct MonStatusIconObj
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct PokerusIconObj
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct ShinyStarObjData
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

extern struct PokemonSummaryScreenData * sMonSummaryScreen;
extern struct MonSkillsPrinterXpos * sMonSkillsPrinterXpos;
extern struct MonStatsIvsPrinterXpos * sMonStatsIvsPrinterXpos;
extern struct MonStatsEvsPrinterXpos * sMonStatsEvsPrinterXpos;
extern struct MoveSelectionCursor * sMoveSelectionCursorObjs[4];
extern struct MonStatusIconObj * sStatusIcon;
extern struct HpBarObjs * sHpBarObjs;
extern struct ExpBarObjs * sExpBarObjs;
extern struct PokerusIconObj * sPokerusIconObj;
extern struct ShinyStarObjData * sShinyStarObjData;
extern u8 sLastViewedMonIndex;
extern u8 sMoveSelectionCursorPos;
extern u8 sMoveSwapCursorPos;
extern struct MonPicBounceState * sMonPicBounceState;

static const u32 sUnknown_84636C0[] = INCBIN_U32("graphics/pokesummary_unk_84636C0.gbapal");
static const u16 sMonMarkingSpritePalette[] = INCBIN_U16("graphics/pokesummary_unk_84636E0.gbapal");
//static const u32 sUnknown_8463700[] = INCBIN_U32("graphics/pokesummary_unk_8463700.gbapal");

static const struct OamData sMoveSelectionCursorOamData =
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

static const union AnimCmd sMoveSelectionCursorOamAnim_Red[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sMoveSelectionCursorOamAnim_Blue[] = 
{
    ANIMCMD_FRAME(0x20, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sMoveSelectionCursorOamAnimTable[] =
{
    sMoveSelectionCursorOamAnim_Red,
    sMoveSelectionCursorOamAnim_Blue
};

static const struct OamData sStatusAilmentIconOamData = {
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

static const union AnimCmd sStatusAilmentIconAnim_PSN[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_PRZ[] = 
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_SLP[] = 
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_FRZ[] = 
{
    ANIMCMD_FRAME(12, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_BRN[] = 
{
    ANIMCMD_FRAME(16, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_PKRS[] = 
{
    ANIMCMD_FRAME(20, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sStatusAilmentIconAnim_FNT[] = 
{
    ANIMCMD_FRAME(24, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sStatusAilmentIconAnimTable[] =
{
    sStatusAilmentIconAnim_PSN,
    sStatusAilmentIconAnim_PRZ,
    sStatusAilmentIconAnim_SLP,
    sStatusAilmentIconAnim_FRZ,
    sStatusAilmentIconAnim_BRN,
    sStatusAilmentIconAnim_PKRS,
    sStatusAilmentIconAnim_FNT,
};

static const struct OamData sHpOrExpBarOamData = {
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

static const union AnimCmd sHpOrExpAnim_0[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_1[] = 
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_2[] = 
{
    ANIMCMD_FRAME(2, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_3[] = 
{
    ANIMCMD_FRAME(3, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_4[] = 
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_5[] = 
{
    ANIMCMD_FRAME(5, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_6[] = 
{
    ANIMCMD_FRAME(6, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_7[] = 
{
    ANIMCMD_FRAME(7, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_8[] = 
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_9[] = 
{
    ANIMCMD_FRAME(9, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_10[] = 
{
    ANIMCMD_FRAME(10, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sHpOrExpAnim_11[] = 
{
    ANIMCMD_FRAME(11, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sHpOrExpBarAnimTable[] =
{
    sHpOrExpAnim_0,
    sHpOrExpAnim_1,
    sHpOrExpAnim_2,
    sHpOrExpAnim_3,
    sHpOrExpAnim_4,
    sHpOrExpAnim_5,
    sHpOrExpAnim_6,
    sHpOrExpAnim_7,
    sHpOrExpAnim_8,
    sHpOrExpAnim_9,
    sHpOrExpAnim_10,
    sHpOrExpAnim_11
};

static const struct OamData sPokerusIconObjOamData = {
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

static const union AnimCmd sPokerusIconObjAnim0[] = 
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sPokerusIconObjAnimTable[] =
{
    sPokerusIconObjAnim0
};

static const u16 sPokerusIconObjPal[] = INCBIN_U16("graphics/pokesummary_unk_8463B00.gbapal");
static const u32 sPokerusIconObjTiles[] = INCBIN_U32("graphics/pokesummary_unk_8463B20.4bpp.lz");

static const struct OamData sStarObjOamData =
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

static const union AnimCmd sStarObjAnim0[] = 
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd * const sStarObjAnimTable[] =
{
    sStarObjAnim0
};

#include "nature_names.h"

static const u8 * const sEggHatchTimeTexts[] = {
    gText_8419B44,
    gText_8419B7B,
    gText_8419BAE,
    gText_8419BDB,
    gText_8419B44Spa,
    gText_8419B7BSpa,
    gText_8419BAESpa,
    gText_8419BDBSpa,
};

static const u8 * const sEggOriginTexts[] = {
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

#define POKESUM_WIN_PAGE_NAME        0
#define POKESUM_WIN_CONTROLS         1
#define POKESUM_WIN_LVL_NICK         2
#define POKESUM_WIN_RIGHT_PANE       3
#define POKESUM_WIN_TRAINER_MEMO     4

#define POKESUM_WIN_INFO_3           3
#define POKESUM_WIN_INFO_4           4
#define POKESUM_WIN_INFO_5           5
#define POKESUM_WIN_INFO_6           6

#define POKESUM_WIN_SKILLS_3         3
#define POKESUM_WIN_SKILLS_4         4
#define POKESUM_WIN_SKILLS_5         5
#define POKESUM_WIN_SKILLS_6         6

#define POKESUM_WIN_MOVES_3          3
#define POKESUM_WIN_MOVES_4          4
#define POKESUM_WIN_MOVES_5          5
#define POKESUM_WIN_MOVES_6          6

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

static const struct WindowTemplate sMonStatsIvsAndEvsWindowTemplate[] = 
{
    { // Pokemon Stats
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 18,
        .height = 14,
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
        .tilemapTop = 17,
        .width = 29,
        .height = 4,
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

static const struct WindowTemplate sWindowTemplates_Dummy[] = 
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
    [DARK]        = {0, 14, 13}, //Text Dark
    [WHITE_TITLE] = {0,  1,  2}, //Text White (title) 
    [BLUE_SHADOW] = {0, 15,  7}, //Red Shadow
    [WHITE]       = {0, 15, 14}, //Text White
    [RED_SHADOW]  = {0, 15,  1}, //Blue Shadow
    [BLUE]        = {0,  7,  6}, //Male Symbol
    [RED]         = {0,  1,  2}, //Female Symbol (red)
    [ORANGE]      = {0,  3, 14}, //Low PP
    [RED_2]       = {0,  1, 14}, //Whithout PP
};

static const u8 sMultiBattlePartyOrder[] =
{
    0, 2, 3, 1, 4, 5
};

static const s8 sMonPicBounceYDelta_Under60[] =
{
    -1, 0, 1
};

static const s8 sMonPicBounceYDelta_60to80[] =
{
    -2, -1, 0, 1, 2
};

static const s8 sMonPicBounceYDelta_80to99[] =
{
    -3, -2, -1, 0, 1, 2, 3
};

static const s8 sMonPicBounceYDelta_Full[] =
{
    -5, -3, -1, 0, 1, 3, 5
};

static const s8 sEggPicShakeXDelta_ItWillTakeSomeTime[] =
{
    1, 1, 0, -1, -1, 0, -1, -1, 0, 1, 1
};

static const s8 sEggPicShakeXDelta_OccasionallyMoves[] =
{
    2, 1, 0, -1, -2, 0, -2, -1, 0, 1, 2
};

static const s8 sEggPicShakeXDelta_AlmostReadyToHatch[] =
{
    2, 1, 1, 0, -1, -1, -2, 0, -2, -1, -1, 0, 1, 1, 2
};

static const u16 * const sHpBarPals[] =
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
    sMonSkillsPrinterXpos = AllocZeroed(sizeof(struct MonSkillsPrinterXpos));
    sMonStatsIvsPrinterXpos = AllocZeroed(sizeof(struct MonStatsIvsPrinterXpos));
    sMonStatsEvsPrinterXpos = AllocZeroed(sizeof(struct MonStatsEvsPrinterXpos));

    if (sMonSummaryScreen == NULL)
    {
        SetMainCallback2(savedCallback);
        return;
    }

    sLastViewedMonIndex = cursorPos;

    sMoveSelectionCursorPos = 0;
    sMoveSwapCursorPos = 0;
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
    sMonSummaryScreen->summarySetupStep = 0;
    sMonSummaryScreen->loadBgGfxStep = 0;
    sMonSummaryScreen->spriteCreationStep = 0;

    sMonSummaryScreen->whichBgLayerToTranslate = 0;
    sMonSummaryScreen->skillsPageBgNum = 2;
    sMonSummaryScreen->infoAndMovesPageBgNum = 1;
    sMonSummaryScreen->flippingPages = FALSE;

    BufferSelectedMonData(&sMonSummaryScreen->currentMon);
    sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG);
    sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG);

    if (sMonSummaryScreen->isBadEgg == TRUE)
        sMonSummaryScreen->isEgg = TRUE;

    sMonSummaryScreen->lastPageFlipDirection[0] = 0xff;
    SetMainCallback2(CB2_SetUpPSS);
}

void ShowSelectMovePokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, MainCallback savedCallback, u16 a4)
{
    ShowPokemonSummaryScreen(party, cursorPos, lastIdx, savedCallback, PSS_MODE_SELECT_MOVE);
    sMonSummaryScreen->moveIds[4] = a4;
}

static u8 PageFlipInputIsDisabled(u8 direction)
{
    if (sMonSummaryScreen->inhibitPageFlipInput == TRUE && sMonSummaryScreen->pageFlipDirection != direction)
        return TRUE;

    return FALSE;
}

u32 IsPageFlipInput(u8 direction)
{
    if (sMonSummaryScreen->isEgg)
        return FALSE;

    if (sMonSummaryScreen->lastPageFlipDirection[0] != 0xff && sMonSummaryScreen->lastPageFlipDirection[0] == direction)
    {
        sMonSummaryScreen->lastPageFlipDirection[0] = 0xff;
        return TRUE;
    }

    if (PageFlipInputIsDisabled(direction))
        return FALSE;

    switch (direction)
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

static void Task_InputHandler_Info(u8 taskId)
{
    switch (sMonSummaryScreen->state3270) {
    case PSS_STATE3270_0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_1;
        break;
    case PSS_STATE3270_1:
        if (!gPaletteFade.active)
        {
            PokeSum_TryPlayMonCry();
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
        else if (FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
            return;

        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (IsPageFlipInput(1) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection[0] = 1;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex < PSS_PAGE_MOVES && sMonSummaryScreen->isDrawingIvEvsPage == FALSE)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_SKILLS && !IsStatsPagesUnlocked())
						sMonSummaryScreen->curPageIndex = PSS_PAGE_MOVES;
					else
						sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_3;
                }
                return;
            }
            else if (IsPageFlipInput(0) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection[0] = 0;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex > PSS_PAGE_INFO && sMonSummaryScreen->isDrawingIvEvsPage == FALSE)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 0;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES && !IsStatsPagesUnlocked())
						sMonSummaryScreen->curPageIndex = PSS_PAGE_SKILLS;
					else
						sMonSummaryScreen->curPageIndex--;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_3;
                }
                return;
            }
        }

        if ((!FuncIsActiveTask(Task_PokeSum_FlipPages)) || FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
        {
            if (JOY_NEW(DPAD_UP) && sMonSummaryScreen->isDrawingIvEvsPage == FALSE)
            {
                PokeSum_SeekToNextMon(taskId, -1);
                return;
            }
            else if (JOY_NEW(DPAD_DOWN) && sMonSummaryScreen->isDrawingIvEvsPage == FALSE)
            {
                PokeSum_SeekToNextMon(taskId, 1);
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
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
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
            CreateTask(Task_PokeSum_FlipPages, 0);
            sMonSummaryScreen->state3270 = PSS_STATE3270_2;
        }
        else
        {
            gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_FlipPages_FromInfo;
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
            Task_DestroyResourcesOnExit(taskId);

        break;
    }
}

static void Task_PokeSum_FlipPages(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    switch (data[0])
    {
    case 0:
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_ShowSpritesBeforePageFlip();
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->flippingPages = TRUE;
        PokeSum_PrintRightPaneText();
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
            PokeSum_PrintBottomPaneText();
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintMonTypeIcons();
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 3);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
		CopyBgTilemapBufferToVram(0);
        PokeSum_DrawBg3Tilemap();
		if (sMonSummaryScreen->curPageIndex == PSS_PAGE_IVS || sMonSummaryScreen->curPageIndex == PSS_PAGE_EVS)
		{
			returnTask = 1;
			counter = 0;
			sMonSummaryScreen->isDrawingIvEvsPage = 1;			
			gTasks[taskId].func = Task_PokeSum_AnimateIvsEvsStats;
			break;
		}
		data[0]++;
        break;
    default:
        PokeSum_SetHelpContext();

        if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
            gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_HandleInput_SelectMove;

        DestroyTask(taskId);
        data[0] = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }
}

static void Task_FlipPages_FromInfo(u8 taskId)
{
    switch (sMonSummaryScreen->state3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_PrintRightPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintMonTypeIcons();
        PrintMonLevelNickOnWindow2(gText_8419C45);
		sMonSummaryScreen->flippingPages = TRUE;
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PokeSum_PrintPageName(gText_8419C39);
        else
			PokeSum_PrintPageName(gText_8419C39Spa);
        if (!(gMain.inBattle || gReceivedRemoteLinkPlayers))
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PokeSum_PrintControlsString(gText_8419C92);
            else
				PokeSum_PrintControlsString(gText_8419C92Spa);
		}
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PokeSum_PrintControlsString(gText_8419CA2);
            else
				PokeSum_PrintControlsString(gText_8419CA2Spa);
		}
        PokeSum_ShowSpritesBeforePageFlip();
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        PokeSum_DrawBg3Tilemap();
        break;
    default:
        PokeSum_SetHelpContext();
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_HandleInput_SelectMove;
        sMonSummaryScreen->state3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }

    sMonSummaryScreen->state3284++;
    return;
}

static void Task_BackOutOfSelectMove(u8 taskId)
{
    switch (sMonSummaryScreen->state3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->flippingPages = TRUE;
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_PrintMonTypeIcons();
        PrintMonLevelNickOnWindow2(gText_8419C45);
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C39);
			PokeSum_PrintControlsString(gText_8419C82);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C39Spa);
			PokeSum_PrintControlsString(gText_8419C82Spa);
		}
        PokeSum_ShowSpritesBeforePageFlip();
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        PokeSum_DrawBg3Tilemap();
        break;
    default:
        PokeSum_SetHelpContext();
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_InputHandler_Info;
        sMonSummaryScreen->state3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }

    sMonSummaryScreen->state3284++;
    return;
}

static void PokeSum_HideSpritesBeforePageFlip(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
    case PSS_PAGE_IVS:
    case PSS_PAGE_EVS:
        ShowOrHideHpBarObjs(1);
        ShowOrHideExpBarObjs(1);
        break;
    case PSS_PAGE_SKILLS:
        ShowOrHideHpBarObjs(0);
        ShowOrHideExpBarObjs(0);
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->pageFlipDirection == 1)
        {
            ShowOrHideMonPicSprite(1);
            ShowOrHideMonMarkingsSprite(1);
            ShowOrHideBallIconObj(1);
            ShowOrHideStatusIcon(1);
            ShowOrHidePokerusIcon(1);
            ShowOrHideShinyStar(1);
			ShowOrHideHpBarObjs(1);
			ShowOrHideExpBarObjs(1);
        }
        break;
    case PSS_PAGE_MOVES_INFO:
        ShowOrHideMoveSelectionCursor(1);
        ShowOrHideMonIconSprite(1);
        ShowOrHideStatusIcon(1);
        ShowOrHidePokerusIcon(1);
        ShowOrHideShinyStar(1);
        ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        break;
    }
}

static void PokeSum_ShowSpritesBeforePageFlip(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
    case PSS_PAGE_IVS:
    case PSS_PAGE_EVS:
        ShowOrHideHpBarObjs(1);
        ShowOrHideExpBarObjs(1);
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->pageFlipDirection != 0)
        {
            ShowOrHideMoveSelectionCursor(0);
            ShowOrHidePokerusIcon(0);
            ShowOrHideMonIconSprite(0);
            ShowOrHideShinyStar(0);
        }
        break;
    case PSS_PAGE_MOVES_INFO:
        ShowOrHideMonPicSprite(0);
        ShowOrHideMonMarkingsSprite(0);
        ShowOrHideStatusIcon(0);
        ShowOrHideBallIconObj(0);
        ShowOrHidePokerusIcon(0);
        ShowOrHideShinyStar(0);
        break;
    }
}

static void CB2_SetUpPSS(void)
{
    switch (sMonSummaryScreen->summarySetupStep)
    {
    case 0:
        PokeSum_Setup_ResetCallbacks();
        break;
    case 1:
        PokeSum_Setup_InitGpu();
        break;
    case 2:
        PokeSum_Setup_SpritesReset();
        break;
    case 3:
        if (!PokeSum_HandleLoadBgGfx())
            return;
        break;
    case 4:
        if (!PokeSum_HandleCreateSprites())
            return;
        break;
    case 5:
        PokeSum_CreateWindows();
        break;
    case 6:
        if (!PokeSum_Setup_BufferStrings())
            return;
        break;
    case 7:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintMonTypeIcons();
        break;
    case 8:
        BlendPalettes(0xffffffff, 16, 0);
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        CommitStaticWindowTilemaps();
        break;
    case 9:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
		PokeSum_DrawBg3Tilemap();
        break;
    case 10:
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        {
            ShowOrHideMonIconSprite(0);
            ShowOrHideMoveSelectionCursor(0);
        }
        else
        {
            ShowOrHideMonPicSprite(0);
            ShowOrHideMonMarkingsSprite(0);
            ShowOrHideBallIconObj(0);
        }

		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        ShowOrHideStatusIcon(0);
        ShowOrHidePokerusIcon(0);
        ShowOrHideShinyStar(0);
        break;
    default:
        PokeSum_Setup_SetVBlankCallback();
        PokeSum_FinishSetup();
        return;
    }

    sMonSummaryScreen->summarySetupStep++;
}

static u8 PokeSum_HandleLoadBgGfx(void)
{
    switch (sMonSummaryScreen->loadBgGfxStep)
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
  //      break;
  //  case 5:
  //      LoadPalette(sUnknown_8463700, 0x80, 0x20);
        return TRUE;
    }

    sMonSummaryScreen->loadBgGfxStep++;
    return FALSE;
}

static u8 PokeSum_Setup_BufferStrings(void)
{
    switch (sMonSummaryScreen->bufferStringsStep)
    {
    case 0:
        BufferMonInfo();
        if (sMonSummaryScreen->isEgg)
        {
            sMonSummaryScreen->bufferStringsStep = 0;
            return TRUE;
        }
        break;
    case 1:
        if (sMonSummaryScreen->isEgg == 0)
            BufferMonSkills();
        break;
    case 2:
        if (sMonSummaryScreen->isEgg == 0)
            BufferMonMoves();
        break;
    default:
        sMonSummaryScreen->bufferStringsStep = 0;
        return TRUE;
    }

    sMonSummaryScreen->bufferStringsStep++;
    return FALSE;
}

static void BufferMonInfo(void)
{
    u8 tempStr[20];
    u16 dexNum;
    u16 gender;
    u16 heldItem;
    u32 otId;

    dexNum = SpeciesToPokedexNum(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES));
    if (dexNum == 0xffff)
        StringCopy(sMonSummaryScreen->summary.dexNumStrBuf, gText_8416202);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.dexNumStrBuf, dexNum, STR_CONV_MODE_LEADING_ZEROS, 3);

    sMonSkillsPrinterXpos->dexNumStr = 0;

    if (!sMonSummaryScreen->isEgg)
    {
        dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);
        GetSpeciesName(sMonSummaryScreen->summary.speciesNameStrBuf, dexNum);
    }
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(sMonSummaryScreen->summary.speciesNameStrBuf, gText_EggNickname);
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.speciesNameStrBuf, gText_EggNicknameSpa);
		return;
    }

    sMonSummaryScreen->typeIcons[0] = gBaseStats[dexNum].type1;
    sMonSummaryScreen->typeIcons[1] = gBaseStats[dexNum].type2;

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_NICKNAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.nicknameStrBuf, tempStr, POKEMON_NAME_LENGTH);
    StringGetEnd10(sMonSummaryScreen->summary.nicknameStrBuf);

    gender = GetMonGender(&sMonSummaryScreen->currentMon);
    dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);

    if (gender == MON_FEMALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbolStrBuf, gText_FemaleSymbol);
    else if (gender == MON_MALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbolStrBuf, gText_MaleSymbol);
    else
        StringCopy(sMonSummaryScreen->summary.genderSymbolStrBuf, gText_StringDummy);

    if (dexNum == SPECIES_NIDORAN_M || dexNum == SPECIES_NIDORAN_F)
        if (StringCompare(sMonSummaryScreen->summary.nicknameStrBuf, gSpeciesNames[dexNum]) == 0)
            StringCopy(sMonSummaryScreen->summary.genderSymbolStrBuf, gText_StringDummy);

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_NAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.otNameStrBuf, tempStr, OT_NAME_LENGTH);

    ConvertInternationalString(sMonSummaryScreen->summary.otNameStrBuf, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LANGUAGE));

    otId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID) & 0xffff;
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.otIdStrBuf, otId, STR_CONV_MODE_LEADING_ZEROS, 5);

    ConvertIntToDecimalStringN(tempStr, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(sMonSummaryScreen->summary.levelStrBuf, gText_Lv);
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(sMonSummaryScreen->summary.levelStrBuf, gText_LvSpa);
    StringAppendN(sMonSummaryScreen->summary.levelStrBuf, tempStr, 4);

    heldItem = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HELD_ITEM);

    if (heldItem == ITEM_NONE)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(sMonSummaryScreen->summary.itemNameStrBuf, gText_84161EF);
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.itemNameStrBuf, gText_84161EFSpa);
	}
    else
        CopyItemName(heldItem, sMonSummaryScreen->summary.itemNameStrBuf);
}

#define GetNumberRightAlign63(x) (63 - StringLength((x)) * 6)
#define GetNumberRightAlign27(x) (27 - StringLength((x)) * 6)

static void BufferMonSkills(void)
{
    u8 tempStr[20];
    u8 level;
    u16 type;
    u16 species;
    u16 hp;
    u16 statValue;
    u32 exp;
    u32 expToNextLevelStrBuf;

    hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.curHpStrBuf, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(sMonSummaryScreen->summary.curHpStrBuf, gText_Slash);

    hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);
    ConvertIntToDecimalStringN(tempStr, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(sMonSummaryScreen->summary.curHpStrBuf, tempStr);

    sMonSkillsPrinterXpos->curHpStr = GetNumberRightAlign63(sMonSummaryScreen->summary.curHpStrBuf);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary && sMonSummaryScreen->isEnemyParty == TRUE)
    {
        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->atkStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->defStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->spAStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->spDStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED2);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->speStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE]);
    }
    else
    {
        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->atkStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->defStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->spAStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->spDStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonSkillsPrinterXpos->speStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_IV);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_HP], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->HpStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_HP]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->atkStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->defStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->spAStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->spDStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->speStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPE]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_IV, NULL) +
					GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_IV, NULL) +
					GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_IV, NULL) +
					GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_IV, NULL) +
					GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_IV, NULL) +
					GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_IV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_TOTAL], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsIvsPrinterXpos->totalIvsStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_TOTAL]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_HP], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->HpStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_HP]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_ATK], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->atkStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_ATK]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_DEF], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->defStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_DEF]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPA], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->spAStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPA]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPD], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->spDStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPD]);

        statValue = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_EV, NULL);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPE], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->speStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPE]);

        statValue = GetMonEVCount(&sMonSummaryScreen->currentMon);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_TOTAL], statValue, STR_CONV_MODE_LEFT_ALIGN, 3);
        sMonStatsEvsPrinterXpos->totalEvsStr = GetNumberRightAlign27(sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_TOTAL]);
    }

    exp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EXP);
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.expPointsStrBuf, exp, STR_CONV_MODE_LEFT_ALIGN, 7);
    sMonSkillsPrinterXpos->expStr = GetNumberRightAlign63(sMonSummaryScreen->summary.expPointsStrBuf);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL);
    expToNextLevelStrBuf = 0;
    if (level < 100)
    {
        species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);
        expToNextLevelStrBuf = gExperienceTables[gBaseStats[species].growthRate][level + 1] - exp;
    }

    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.expToNextLevelStrBuf, expToNextLevelStrBuf, STR_CONV_MODE_LEFT_ALIGN, 7);
    sMonSkillsPrinterXpos->toNextLevel = GetNumberRightAlign63(sMonSummaryScreen->summary.expToNextLevelStrBuf);

    type = GetAbilityBySpecies(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES), GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ABILITY_NUM));
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		StringCopy(sMonSummaryScreen->summary.abilityNameStrBuf, gAbilityNames[type]);
		StringCopy(sMonSummaryScreen->summary.abilityDescStrBuf, gAbilityDescriptionPointers[type]);
	}
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		StringCopy(sMonSummaryScreen->summary.abilityNameStrBuf, gAbilityNamesSpa[type]);
		StringCopy(sMonSummaryScreen->summary.abilityDescStrBuf, gAbilityDescriptionSpaPointers[type]);
	}

    sMonSummaryScreen->curMonStatusAilment = StatusToAilment(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_STATUS));
    if (sMonSummaryScreen->curMonStatusAilment == AILMENT_NONE)
        if (CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0))
            sMonSummaryScreen->curMonStatusAilment = AILMENT_PKRS;
}

static void BufferMonMoves(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        BufferMonMoveI(i);

    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
        BufferMonMoveI(4);
}

#define GetRightAlignXpos_NDigits(a, b) ((6 * (a)) - StringLength((b)) * 6)

static void BufferMonMoveI(u8 i)
{
    if (i < 4)
        sMonSummaryScreen->moveIds[i] = GetMonMoveBySlotId(&sMonSummaryScreen->currentMon, i);

    if (sMonSummaryScreen->moveIds[i] == 0)
    {
        StringCopy(sMonSummaryScreen->summary.moveNameStrBufs[i], gText_841620E);
        StringCopy(sMonSummaryScreen->summary.moveCurPpStrBufs[i], gText_8416210);
        StringCopy(sMonSummaryScreen->summary.movePowerStrBufs[i], gText_ThreeHyphens);
        StringCopy(sMonSummaryScreen->summary.moveAccuracyStrBufs[i], gText_ThreeHyphens);
        sMonSkillsPrinterXpos->curPp[i] = 0xff;
        sMonSkillsPrinterXpos->maxPp[i] = 0xff;
        return;
    }

    sMonSummaryScreen->numMoves++;
    sMonSummaryScreen->moveTypes[i] = gBattleMoves[sMonSummaryScreen->moveIds[i]].type;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(sMonSummaryScreen->summary.moveNameStrBufs[i], gMoveNames[sMonSummaryScreen->moveIds[i]]);
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(sMonSummaryScreen->summary.moveNameStrBufs[i], gMoveNamesSpa[sMonSummaryScreen->moveIds[i]]);

    if (i >= 4 && sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
    {
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.moveCurPpStrBufs[i],
                                   gBattleMoves[sMonSummaryScreen->moveIds[i]].pp, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.moveMaxPpStrBufs[i],
                                   gBattleMoves[sMonSummaryScreen->moveIds[i]].pp, STR_CONV_MODE_LEFT_ALIGN, 3);
    }
    else
    {
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.moveCurPpStrBufs[i],
                                   GetMonPpByMoveSlot(&sMonSummaryScreen->currentMon, i), STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.moveMaxPpStrBufs[i],
                                   CalculatePPWithBonus(sMonSummaryScreen->moveIds[i], GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES), i),
                                   STR_CONV_MODE_LEFT_ALIGN, 3);
    }

    sMonSkillsPrinterXpos->curPp[i] = GetRightAlignXpos_NDigits(2, sMonSummaryScreen->summary.moveCurPpStrBufs[i]);
    sMonSkillsPrinterXpos->maxPp[i] = GetRightAlignXpos_NDigits(2, sMonSummaryScreen->summary.moveMaxPpStrBufs[i]);

    if (gBattleMoves[sMonSummaryScreen->moveIds[i]].power <= 1)
        StringCopy(sMonSummaryScreen->summary.movePowerStrBufs[i], gText_ThreeHyphens);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.movePowerStrBufs[i], gBattleMoves[sMonSummaryScreen->moveIds[i]].power, STR_CONV_MODE_RIGHT_ALIGN, 3);

    if (gBattleMoves[sMonSummaryScreen->moveIds[i]].accuracy == 0)
        StringCopy(sMonSummaryScreen->summary.moveAccuracyStrBufs[i], gText_ThreeHyphens);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.moveAccuracyStrBufs[i], gBattleMoves[sMonSummaryScreen->moveIds[i]].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
}

static u8 PokeSum_HandleCreateSprites(void)
{
    switch (sMonSummaryScreen->spriteCreationStep)
    {
    case 0:
        CreateShinyStarObj(TAG_PSS_UNK_A0, TAG_PSS_UNK_A0);
        break;
    case 1:
        CreatePokerusIconObj(TAG_PSS_UNK_96, TAG_PSS_UNK_96);
        break;
    case 2:
        PokeSum_CreateMonMarkingsSprite();
        break;
    case 3:
        CreateMoveSelectionCursorObjs(TAG_PSS_UNK_64, TAG_PSS_UNK_64);
        break;
    case 4:
        CreateMonStatusIconObj(TAG_PSS_UNK_6E, TAG_PSS_UNK_6E);
        break;
    case 5:
        CreateHpBarObjs(TAG_PSS_UNK_78, TAG_PSS_UNK_78);
        break;
    case 6:
        CreateExpBarObjs(TAG_PSS_UNK_82, TAG_PSS_UNK_82);
        break;
    case 7:
        CreateBallIconObj();
        break;
    case 8:
        CreateMonIconSprite();
        break;
    default:
        PokeSum_CreateMonPicSprite();
        return TRUE;
    }

    sMonSummaryScreen->spriteCreationStep++;
    return FALSE;
}

static void PokeSum_Setup_SpritesReset(void)
{
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ScanlineEffect_Stop();
}

static void PokeSum_Setup_InitGpu(void)
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

static void PokeSum_FinishSetup(void)
{
    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        sMonSummaryScreen->inputHandlerTaskId = CreateTask(Task_InputHandler_SelectOrForgetMove, 0);
    else
        sMonSummaryScreen->inputHandlerTaskId = CreateTask(Task_InputHandler_Info, 0);

	SetMainCallback2(CB2_RunPokemonSummaryScreen);
}

static void PokeSum_PrintPageName(const u8 * str)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 0);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2, 4, 1, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME]);
}

static void PokeSum_PrintControlsString(const u8 * str)
{
    u8 v0;
    s32 width;
    u8 r1;

    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 0);
    width = GetStringWidth(0, str, 0);
    r1 = sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS];
    AddTextPrinterParameterized3(r1, 0, 0x54 - width, 0, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS]);
}

static void PrintMonLevelNickOnWindow2(const u8 * msg)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 0);

    if (!sMonSummaryScreen->isEgg)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2, 0, 14, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.levelStrBuf);
		}
		else
		{
			if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], sMonSummaryScreen->typeIcons[0] + 1, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], sMonSummaryScreen->typeIcons[1] + 1, 38, 16);
			}
			else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], sMonSummaryScreen->typeIcons[0] + 24, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], sMonSummaryScreen->typeIcons[1] + 24, 38, 16);
			}
		}
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2, 0, 2, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.nicknameStrBuf);
			
		if (GetMonGender(&sMonSummaryScreen->currentMon) == MON_FEMALE)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2, 62, 2, sPSSTextColours[RED], 0, sMonSummaryScreen->summary.genderSymbolStrBuf);
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2, 62, 2, sPSSTextColours[BLUE], 0, sMonSummaryScreen->summary.genderSymbolStrBuf);
	}

    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK]);
}

static void PokeSum_PrintRightPaneText(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[3], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PrintInfoPage();
        break;
    case PSS_PAGE_SKILLS:
        PrintSkillsPage();
        break;
    case PSS_PAGE_IVS:
        PrintIvsPage();
        break;
    case PSS_PAGE_EVS:
        PrintEvsPage();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PrintMovesPage();
        break;
    }
    PutWindowTilemap(sMonSummaryScreen->windowIds[3]);
}

static void PrintInfoPage(void)
{
    

    if (!sMonSummaryScreen->isEgg)
    {
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.speciesNameStrBuf);
		
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Name);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Type);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OT);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Item);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TypeSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OTSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ItemSpa);
		}
		
		if ((HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY))) < 8)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80 + sMonSkillsPrinterXpos->dexNumStr, 8, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNumStrBuf);
		else	
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80 + sMonSkillsPrinterXpos->dexNumStr, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNumStrBuf);
		if ((gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[3] << 24)) == GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID))
		{
			if (gSaveBlock2Ptr->playerGender == FEMALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
			if (gSaveBlock2Ptr->playerGender == MALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[BLUE], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
		}
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.otIdStrBuf);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.itemNameStrBuf);
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
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Status);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggHatchTimeTexts[hatchMsgIndex]);
        }
		else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_StatusSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggHatchTimeTexts[hatchMsgIndex + 4]);
		}
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.speciesNameStrBuf);
    }
}

static void PrintSkillsPage(void)
{
    u8 nature = GetNature(&sMonSummaryScreen->currentMon);
	
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_Attack);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_Defense);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtk);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDef);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_Speed);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPoints);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLv);
	}
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HPSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_AttackSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_DefenseSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtkSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDefSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_SpeedSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPointsSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLvSpa);
	}
	
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 68 + sMonSkillsPrinterXpos->curHpStr, 0, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.curHpStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 90 + sMonSkillsPrinterXpos->atkStr, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 90 + sMonSkillsPrinterXpos->defStr, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 90 + sMonSkillsPrinterXpos->spAStr, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 90 + sMonSkillsPrinterXpos->spDStr, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 90 + sMonSkillsPrinterXpos->speStr, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 55 + sMonSkillsPrinterXpos->expStr, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.expPointsStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 55 + sMonSkillsPrinterXpos->toNextLevel, 92, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.expToNextLevelStrBuf);
}

static void PrintIvsPage(void)
{
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_IvTittle);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 16, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Attack);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 48, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Defense);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 64, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpAtk);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 80, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpDef);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 96, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Speed);
	}
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_IvTittleSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 16, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HPSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AttackSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 48, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DefenseSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 64, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpAtkSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 80, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpDefSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 96, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpeedSpa);
	}
	
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->HpStr, 16, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_HP]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->atkStr, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->defStr, 48, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->spAStr, 64, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->spDStr, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsIvsPrinterXpos->speStr, 96, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_SPE]);
}

static void PrintEvsPage(void)
{
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_EvTittle);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 16, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Attack);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 48, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Defense);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 64, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpAtk);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 80, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpDef);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 96, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Speed);
	}
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_EvTittleSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 16, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HPSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AttackSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 48, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DefenseSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 64, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpAtkSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 80, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpDefSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 96, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_SpeedSpa);
	}
	
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->HpStr, 16, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_HP]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->atkStr, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->defStr, 48, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->spAStr, 64, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->spDStr, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 110 + sMonStatsEvsPrinterXpos->speStr, 96, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_SPE]);
}

#define GetMoveNamePrinterYpos(x) ((x) * 28 + 5)

static void PrintMovesPage(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        PokeSum_PrintMoveNamesAndPP(i);

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
            PokeSum_PrintMoveNamesAndPP(4);
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 5, GetMoveNamePrinterYpos(4) + 4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_Cancel);
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 5, GetMoveNamePrinterYpos(4) + 4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_CancelSpa);
		}
    }
}

#define GetMovePpPinterYpos(x) ((x) * 28 + 16)

static void PokeSum_PrintMoveNamesAndPP(u8 i)
{
    u8 color = WHITE;
    u8 curPP = GetMonPpByMoveSlot(&sMonSummaryScreen->currentMon, i);
    u16 move = sMonSummaryScreen->moveIds[i];
    u8 ppBonuses = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES);
    u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);

    if (i == 4)
        curPP = maxPP;
	//Add Move Names
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 3, GetMoveNamePrinterYpos(i) - 2, sPSSTextColours[WHITE], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveNameStrBufs[i]);

    if (sMonSummaryScreen->moveIds[i] == 0 || (curPP == maxPP))
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
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, GetMovePpPinterYpos(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_8416238);
	// Add PP counter
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 40 + sMonSkillsPrinterXpos->curPp[i], GetMovePpPinterYpos(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveCurPpStrBufs[i]);

    if (sMonSummaryScreen->moveIds[i] != MOVE_NONE)
    {
		// Add Slash
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 52, GetMovePpPinterYpos(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_Slash);
        // Add PP Max
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 58 + sMonSkillsPrinterXpos->maxPp[i], GetMovePpPinterYpos(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveMaxPpStrBufs[i]);
    }
}

static void PokeSum_PrintBottomPaneText(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[4], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PokeSum_PrintTrainerMemo();
        break;
    case PSS_PAGE_MOVES_INFO:
        PokeSum_PrintSelectedMoveStats();
        break;
    case PSS_PAGE_SKILLS:
    case PSS_PAGE_MOVES:
        break;
    }
   PutWindowTilemap(sMonSummaryScreen->windowIds[4]);
}

static void PokeSum_PrintTrainerMemo(void)
{
    if (!sMonSummaryScreen->isEgg)
        PokeSum_PrintTrainerMemo_Mon();
    else
        PokeSum_PrintTrainerMemo_Egg();
}

static void PokeSum_PrintTrainerMemo_Mon_HeldByOT(void)
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

    if (MapSecIsInKantoOrSevii(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || IsMultiBattlePartner() == TRUE)
				StringCopy(mapNameStr, gText_8419C13);
			else
				StringCopy(mapNameStr, gText_8419C0B);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || IsMultiBattlePartner() == TRUE)
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
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992F);
			}
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992FSpa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198D5);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198B4);
			}
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
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
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822);
			}
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841Spa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822Spa);
			}
        }
    }

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
    AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PokeSum_PrintTrainerMemo_Mon_NotHeldByOT(void)
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

    if (!MapSecIsInKantoOrSevii(metLocation) || !CurrentMonIsFromGBA())
    {
        if (IsMultiBattlePartner() == TRUE)
        {
            PokeSum_PrintTrainerMemo_Mon_HeldByOT();
            return;
        }

        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782);
			}
			else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PokeSum_IsMonBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782Spa);
			}
        }
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
		else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

        AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
        return;
    }

    if (MapSecIsInKantoOrSevii(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(mapNameStr, gText_8419C0B);
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(mapNameStr, gText_8419C0BSpa);
	}		

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL) == 0)
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
			{                
				if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4);
				else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4Spa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199AB);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199ABSpa);
			}
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                else if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

    AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PokeSum_PrintTrainerMemo_Mon(void)
{
    if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == TRUE)
        PokeSum_PrintTrainerMemo_Mon_HeldByOT();
    else
        PokeSum_PrintTrainerMemo_Mon_NotHeldByOT();
}

static void PokeSum_PrintTrainerMemo_Egg(void)
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
                if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == FALSE)
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

            if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == FALSE)
                chosenStrIndex++;
        }
    }

    if (sMonSummaryScreen->isBadEgg)
        chosenStrIndex = 0;

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggOriginTexts[chosenStrIndex]);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
    }
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggOriginTexts[chosenStrIndex + 7]);
	}
}

static void PokeSum_PrintExpPoints_NextLv(void)
{
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4D);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59);
	}
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4DSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59Spa);
	}
}

static void PokeSum_PrintSelectedMoveStats(void)
{
    if (sMoveSelectionCursorPos < 5)
    {
        if (sMonSummaryScreen->mode != PSS_MODE_SELECT_MOVE && sMoveSelectionCursorPos == 4)
            return;
		//Add Category Icon
		BlitMoveInfoIcon(sMonSummaryScreen->windowIds[4], gBattleMoves[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos]].Category+47, 85, 0);
		//Add Power
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 91, 18, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.movePowerStrBufs[sMoveSelectionCursorPos]);
		//Add Accuracy
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 91, 33, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveAccuracyStrBufs[sMoveSelectionCursorPos]);
		//Add Move Description
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Category);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Power);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Accuracy);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Effect);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos] - 1]);
        }
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_CategorySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_PowerSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AccuracySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_EffectSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos] + MOVES_COUNT - 2]);
		}
    }
}

static void PokeSum_PrintAbilityDataOrMoveTypes(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_IVS:
		PokeSum_PrintTotalIvsStats();
        break;
    case PSS_PAGE_EVS:
		PokeSum_PrintTotalEvsStats();
        break;
    case PSS_PAGE_SKILLS:
        PokeSum_PrintAbilityNameAndDesc();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PokeSum_DrawMoveTypeIcons();
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->windowIds[5]);
}

static void PokeSum_PrintAbilityNameAndDesc(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 11,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Ability);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 5,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AbilitySpa);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 60,  4, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityNameStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20, 23, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityDescStrBuf);

}

static void PokeSum_PrintTotalIvsStats(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20,  2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_Total);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20,  2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_TotalSpa);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 150 + sMonStatsIvsPrinterXpos->totalIvsStr, 2, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statIvsStrBufs[IVS_EVS_STAT_TOTAL]);
}

static void PokeSum_PrintTotalEvsStats(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20,  2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_Total);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20,  2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_TotalSpa);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 150 + sMonStatsEvsPrinterXpos->totalEvsStr, 2, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statEvsStrBufs[IVS_EVS_STAT_TOTAL]);
}

static void PokeSum_DrawMoveTypeIcons(void)
{
    u8 i;

    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->moveIds[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[i] + 1, 8, GetMoveNamePrinterYpos(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[4] + 1, 8, GetMoveNamePrinterYpos(4) - 2);
	}
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->moveIds[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[i] + 24, 8, GetMoveNamePrinterYpos(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[4] + 24, 8, GetMoveNamePrinterYpos(4) - 2);
	}
}

static void PokeSum_PrintPageHeaderText(u8 curPageIndex)
{
    switch (curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PokeSum_PrintPageName(gText_8419C1D);
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			PokeSum_PrintPageName(gText_8419C1DSpa);
        if (!sMonSummaryScreen->isEgg)
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PokeSum_PrintControlsString(gText_8419C62);
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PokeSum_PrintControlsString(gText_8419C62Spa);
		}
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				PokeSum_PrintControlsString(gText_8419C72);
            else if (gSaveBlock2Ptr->optionsLanguage == SPA)
				PokeSum_PrintControlsString(gText_8419C72Spa);
		}

        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			PrintMonLevelNickOnWindow2(gText_8419C45);
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
        break;
    case PSS_PAGE_SKILLS:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C2A);
			PokeSum_PrintControlsString(gText_8419C7B);
			PrintMonLevelNickOnWindow2(gText_8419C45);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C2ASpa);
			PokeSum_PrintControlsString(gText_8419C7BSpa);
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_IVS:
    case PSS_PAGE_EVS:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C2B);
			PokeSum_PrintControlsString(gText_8419C7B);
			PrintMonLevelNickOnWindow2(gText_8419C45);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C2BSpa);
			PokeSum_PrintControlsString(gText_8419C7BSpa);
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVES:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C39);
			PokeSum_PrintControlsString(gText_8419C82);
			PrintMonLevelNickOnWindow2(gText_8419C45);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C39Spa);
			PokeSum_PrintControlsString(gText_8419C82Spa);
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVES_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C39);
			if (!gMain.inBattle)
				PokeSum_PrintControlsString(gText_8419C92);
			else
				PokeSum_PrintControlsString(gText_8419CA2);
			PrintMonLevelNickOnWindow2(gText_8419C45);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C39Spa);
			if (!gMain.inBattle)
				PokeSum_PrintControlsString(gText_8419C92Spa);
			else
				PokeSum_PrintControlsString(gText_8419CA2Spa);
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
		}
        break;
    case PSS_PAGE_MOVE_DELETER:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			PokeSum_PrintPageName(gText_8419C39);
			PokeSum_PrintControlsString(gText_8419CA9);
			PrintMonLevelNickOnWindow2(gText_8419C45);
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			PokeSum_PrintPageName(gText_8419C39Spa);
			PokeSum_PrintControlsString(gText_8419CA9Spa);
			PrintMonLevelNickOnWindow2(gText_8419C45Spa);
		}
        break;
    default:
        break;
    }
}

static void CommitStaticWindowTilemaps(void)
{
    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME]);
    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS]);
    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK]);
}

static void Task_DestroyResourcesOnExit(u8 taskId)
{
    PokeSum_DestroySprites();
    FreeAllSpritePalettes();

    if (IsCryPlayingOrClearCrySongs() == TRUE)
        StopCryAndClearCrySongs();

    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
    FreeAllWindowBuffers();
    DestroyTask(taskId);
    SetMainCallback2(sMonSummaryScreen->savedCallback);

    sLastViewedMonIndex = GetLastViewedMonIndex();

    FREE_AND_SET_NULL_IF_SET(sMonSummaryScreen);
    FREE_AND_SET_NULL_IF_SET(sMonSkillsPrinterXpos);
    FREE_AND_SET_NULL_IF_SET(sMonStatsIvsPrinterXpos);
    FREE_AND_SET_NULL_IF_SET(sMonStatsEvsPrinterXpos);
}

static void CB2_RunPokemonSummaryScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_PokemonSummaryScreen(void)
{
	ScrollPSSBackground();
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
	
    if (sMonSummaryScreen->flippingPages == FALSE)
        return;
}

static void PokeSum_Setup_ResetCallbacks(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

static void PokeSum_Setup_SetVBlankCallback(void)
{
    SetVBlankCallback(VBlankCB_PokemonSummaryScreen);
}

static void PokeSum_CreateWindows(void)
{
    u8 i;

    InitWindows(sWindowTemplates_Dummy);

    for (i = 0; i < 3; i++)
        sMonSummaryScreen->windowIds[i] = AddWindow(&WindowTemplatePokemonInfo[i]);

    for (i = 0; i < 4; i++)
        switch (sMonSummaryScreen->curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_IVS:
        case PSS_PAGE_EVS:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsIvsAndEvsWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        default:
            break;
        }
}

static void PokeSum_AddWindows(u8 curPageIndex)
{
    u8 i;
    for (i = 0; i < 3; i++)
		sMonSummaryScreen->windowIds[i] = AddWindow(&WindowTemplatePokemonInfo[i]);
    for (i = 0; i < 4; i++)
        switch (curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
        default:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_IVS:
        case PSS_PAGE_EVS:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsIvsAndEvsWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        }
}

static void PokeSum_RemoveWindows(u8 curPageIndex)
{
    u8 i;

    for (i = 0; i < 7; i++)
        RemoveWindow(sMonSummaryScreen->windowIds[i]);
}

static void PokeSum_SetHelpContext(void)
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

static u8 PokeSum_BufferOtName_IsEqualToCurrentOwner(struct Pokemon * mon)
{
    u8 i;
    u8 multiplayerId;
    u32 trainerId = 0;

    if (sMonSummaryScreen->monList.mons == gEnemyParty)
    {
        multiplayerId = GetMultiplayerId() ^ 1;
        trainerId = gLinkPlayers[multiplayerId].trainerId & 0xffff;
        StringCopy(sMonSummaryScreen->summary.otNameStrBufs[0], gLinkPlayers[multiplayerId].name);
    }
    else
    {
        trainerId = GetPlayerTrainerId() & 0xffff;
        StringCopy(sMonSummaryScreen->summary.otNameStrBufs[0], gSaveBlock2Ptr->playerName);
    }

    if (trainerId != (GetMonData(mon, MON_DATA_OT_ID) & 0xffff))
        return FALSE;

    GetMonData(mon, MON_DATA_OT_NAME, sMonSummaryScreen->summary.otNameStrBufs[1]);

    if (!StringCompareWithoutExtCtrlCodes(sMonSummaryScreen->summary.otNameStrBufs[0], sMonSummaryScreen->summary.otNameStrBufs[1]))
        return TRUE;
    else
        return FALSE;

    return TRUE;
}

static void PokeSum_DrawBg3Tilemap(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        if (!sMonSummaryScreen->isEgg)
        {
            LZ77UnCompVram(gMapSummaryScreenPokemonInfo, (void *)(VRAM + 0xF080));
			if (IsStatsPagesUnlocked())
				LZ77UnCompVram(gMapSummaryScreenPokemonInfoTitle2, (void *)(VRAM + 0xF000));
			else
				LZ77UnCompVram(gMapSummaryScreenPokemonInfoTitle, (void *)(VRAM + 0xF000));
        }
        else
        {
            LZ77UnCompVram(gMapSummaryScreenEgg, (void *)(VRAM + 0xF080));
			if (IsStatsPagesUnlocked())
				LZ77UnCompVram(gMapSummaryScreenEggTitle2, (void *)(VRAM + 0xF000));
			else
				LZ77UnCompVram(gMapSummaryScreenEggTitle, (void *)(VRAM + 0xF000));
        }
        break;
    case PSS_PAGE_SKILLS:
		ShowOrHideHpBarObjs(0);
		ShowOrHideExpBarObjs(0);
		if (IsStatsPagesUnlocked())
			LZ77UnCompVram(gMapSummaryScreenPokemonSkillsTitle2, (void *)(VRAM + 0xF000));
		else
			LZ77UnCompVram(gMapSummaryScreenPokemonSkillsTitle, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenPokemonSkills, (void *)(VRAM + 0xF080));
		HideBg(3);
        break;
    case PSS_PAGE_IVS:
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
		LZ77UnCompVram(gMapSummaryScreenPokemonStatsIvsTitle, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenPokemonStats, (void *)(VRAM + 0xF080));
        LZ77UnCompVram(gMapSummaryScreenPokemonStatsAnim, (void *)(VRAM + 0xE000));
		ShowBg(3);
        break;
    case PSS_PAGE_EVS:
		LZ77UnCompVram(gMapSummaryScreenPokemonStatsEvsTitle, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenPokemonStats, (void *)(VRAM + 0xF080));
        LZ77UnCompVram(gMapSummaryScreenPokemonStatsAnim, (void *)(VRAM + 0xE000));
        break;
    case PSS_PAGE_MOVES:
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
		if (IsStatsPagesUnlocked())
			LZ77UnCompVram(gMapSummaryScreenKnownMovesTitle2, (void *)(VRAM + 0xF000));
		else
			LZ77UnCompVram(gMapSummaryScreenKnownMovesTitle, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenKnownMoves, (void *)(VRAM + 0xF080));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		ShowBg(3);
        break;
    case PSS_PAGE_MOVES_INFO:
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
		if (IsStatsPagesUnlocked())
			LZ77UnCompVram(gMapSummaryScreenKnownMovesTitle2, (void *)(VRAM + 0xF000));
		else
			LZ77UnCompVram(gMapSummaryScreenKnownMovesTitle, (void *)(VRAM + 0xF000));
		LZ77UnCompVram(gMapSummaryScreenMovesInfo, (void *)(VRAM + 0xF080));
        LZ77UnCompVram(gMapSummaryScreenMovesSelect, (void *)(VRAM + 0xE000));
		ShowBg(3);
        break;
    }
}

static void PokeSum_PrintMonTypeIcons(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->windowIds[3], sMonSummaryScreen->typeIcons[0] + 1, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->windowIds[3], sMonSummaryScreen->typeIcons[1] + 1, 110, 33);
			}
		}
        else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->windowIds[3], sMonSummaryScreen->typeIcons[0] + 24, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->windowIds[3], sMonSummaryScreen->typeIcons[1] + 24, 110, 33);
			}
		}
        break;
    case PSS_PAGE_SKILLS:
    case PSS_PAGE_IVS:
    case PSS_PAGE_EVS:
    case PSS_PAGE_MOVES:
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
    return sMoveSwapCursorPos;
}

void SetPokemonSummaryScreenMode(u8 mode)
{
    sMonSummaryScreen->mode = mode;
}

static bool32 IsMultiBattlePartner(void)
{
    if (!IsUpdateLinkStateCBActive()
        && IsMultiBattle() == TRUE
        && gReceivedRemoteLinkPlayers == 1
        && (sLastViewedMonIndex >= 4 || sLastViewedMonIndex == 1))
        return TRUE;

    return FALSE;
}

static void BufferSelectedMonData(struct Pokemon * mon)
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

static u16 GetMonMoveBySlotId(struct Pokemon * mon, u8 moveSlot)
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

static u16 GetMonPpByMoveSlot(struct Pokemon * mon, u8 moveSlot)
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

static u8 StatusToAilment(u32 status)
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

static void Task_HandleInput_SelectMove(u8 id)
{
    u8 i;

    switch (sMonSummaryScreen->selectMoveInputHandlerState)
    {
    case 0:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        if (JOY_NEW(DPAD_UP))
        {
            if (sMoveSelectionCursorPos > 0)
            {
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);

                for (i = sMoveSelectionCursorPos; i > 0; i--)
                    if (sMonSummaryScreen->moveIds[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i - 1;
                        return;
                    }
            }
            else
            {
                sMoveSelectionCursorPos = 4;
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                    for (i = sMoveSelectionCursorPos; i > 0; i--)
                        if (sMonSummaryScreen->moveIds[i - 1] != 0)
                        {
                            PlaySE(SE_SELECT);
                            sMoveSelectionCursorPos = i - 1;
                            return;
                        }
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sMoveSelectionCursorPos < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->selectMoveInputHandlerState = 2;

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                {
                    if (sMoveSelectionCursorPos == 5 - 2)
                    {
                        sMoveSelectionCursorPos = 0;
                        sMonSummaryScreen->selectMoveInputHandlerState = 2;
                        PlaySE(SE_SELECT);
                        return;
                    }
                    v0--;
                }

                for (i = sMoveSelectionCursorPos; i < v0; i++)
                    if (sMonSummaryScreen->moveIds[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->isSwappingMoves)
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = i;
                }
                else
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = 0;
                }

                return;
            }
            else if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMoveSwapCursorPos = 0;
                sMonSummaryScreen->isSwappingMoves = FALSE;
                ShowOrHideMoveSelectionCursor(1);
                sMonSummaryScreen->pageFlipDirection = 0;
                PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                sMonSummaryScreen->curPageIndex--;
                sMonSummaryScreen->selectMoveInputHandlerState = 1;
                return;
            }

            if (sMonSummaryScreen->isSwappingMoves != TRUE)
            {
                if (sMonSummaryScreen->isEnemyParty == FALSE
                    && gMain.inBattle == 0
                    && gReceivedRemoteLinkPlayers == 0)
                {
                    sMoveSwapCursorPos = sMoveSelectionCursorPos;
                    sMonSummaryScreen->isSwappingMoves = TRUE;
                }
                return;
            }
            else
            {
                sMonSummaryScreen->isSwappingMoves = FALSE;

                if (sMoveSelectionCursorPos == sMoveSwapCursorPos)
                    return;

                if (sMonSummaryScreen->isBoxMon == 0)
                    SwapMonMoveSlots();
                else
                    SwapBoxMonMoveSlots();

                UpdateCurrentMonBufferFromPartyOrBox(&sMonSummaryScreen->currentMon);
                BufferMonMoves();
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                return;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (sMonSummaryScreen->isSwappingMoves == TRUE)
            {
                sMoveSwapCursorPos = sMoveSelectionCursorPos;
                sMonSummaryScreen->isSwappingMoves = FALSE;
                return;
            }

            if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMoveSwapCursorPos = 0;
            }

            ShowOrHideMoveSelectionCursor(1);
            sMonSummaryScreen->pageFlipDirection = 0;
            PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
            sMonSummaryScreen->curPageIndex--;
            sMonSummaryScreen->selectMoveInputHandlerState = 1;
        }
        break;
    case 1:
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_BackOutOfSelectMove;
        sMonSummaryScreen->selectMoveInputHandlerState = 0;
        break;
    case 2:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        sMonSummaryScreen->selectMoveInputHandlerState = 3;
        break;
    case 3:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 0;
        break;
    default:
        break;
    }
}

static void SwapMonMoveSlots(void)
{
    struct Pokemon * partyMons;
    struct Pokemon * mon;

    u16 move1, move2;
    u8 pp1, pp2;
    u8 allMovesPPBonuses;
    u8 move1ppBonus, move2ppBonus;

    partyMons = sMonSummaryScreen->monList.mons;
    mon = &partyMons[GetLastViewedMonIndex()];

    move1 = GetMonData(mon, MON_DATA_MOVE1 + sMoveSelectionCursorPos);
    move2 = GetMonData(mon, MON_DATA_MOVE1 + sMoveSwapCursorPos);

    pp1 = GetMonData(mon, MON_DATA_PP1 + sMoveSelectionCursorPos);
    pp2 = GetMonData(mon, MON_DATA_PP1 + sMoveSwapCursorPos);

    allMovesPPBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);

    move1ppBonus = (allMovesPPBonuses & gPPUpGetMask[sMoveSelectionCursorPos]) >> (sMoveSelectionCursorPos * 2);
    move2ppBonus = (allMovesPPBonuses & gPPUpGetMask[sMoveSwapCursorPos]) >> (sMoveSwapCursorPos * 2);

    allMovesPPBonuses &= ~gPPUpGetMask[sMoveSelectionCursorPos];
    allMovesPPBonuses &= ~gPPUpGetMask[sMoveSwapCursorPos];
    allMovesPPBonuses |= (move1ppBonus << (sMoveSwapCursorPos * 2)) + (move2ppBonus << (sMoveSelectionCursorPos * 2));

    SetMonData(mon, MON_DATA_MOVE1 + sMoveSelectionCursorPos, (u8 *)&move2);
    SetMonData(mon, MON_DATA_MOVE1 + sMoveSwapCursorPos, (u8 *)&move1);
    SetMonData(mon, MON_DATA_PP1 + sMoveSelectionCursorPos, &pp2);
    SetMonData(mon, MON_DATA_PP1 + sMoveSwapCursorPos, &pp1);
    SetMonData(mon, MON_DATA_PP_BONUSES, &allMovesPPBonuses);
}

static void SwapBoxMonMoveSlots(void)
{
    struct BoxPokemon * boxMons;
    struct BoxPokemon * boxMon;

    u16 move1, move2;
    u8 pp1, pp2;
    u8 allMovesPPBonuses;
    u8 move1ppBonus, move2ppBonus;

    boxMons = sMonSummaryScreen->monList.boxMons;
    boxMon = &boxMons[GetLastViewedMonIndex()];

    move1 = GetBoxMonData(boxMon, MON_DATA_MOVE1 + sMoveSelectionCursorPos);
    move2 = GetBoxMonData(boxMon, MON_DATA_MOVE1 + sMoveSwapCursorPos);

    pp1 = GetBoxMonData(boxMon, MON_DATA_PP1 + sMoveSelectionCursorPos);
    pp2 = GetBoxMonData(boxMon, MON_DATA_PP1 + sMoveSwapCursorPos);

    allMovesPPBonuses = GetBoxMonData(boxMon, MON_DATA_PP_BONUSES);

    move1ppBonus = (allMovesPPBonuses & gPPUpGetMask[sMoveSelectionCursorPos]) >> (sMoveSelectionCursorPos * 2);
    move2ppBonus = (allMovesPPBonuses & gPPUpGetMask[sMoveSwapCursorPos]) >> (sMoveSwapCursorPos * 2);

    allMovesPPBonuses &= ~gPPUpGetMask[sMoveSelectionCursorPos];
    allMovesPPBonuses &= ~gPPUpGetMask[sMoveSwapCursorPos];
    allMovesPPBonuses |= (move1ppBonus << (sMoveSwapCursorPos * 2)) + (move2ppBonus << (sMoveSelectionCursorPos * 2));

    SetBoxMonData(boxMon, MON_DATA_MOVE1 + sMoveSelectionCursorPos, (u8 *)&move2);
    SetBoxMonData(boxMon, MON_DATA_MOVE1 + sMoveSwapCursorPos, (u8 *)&move1);
    SetBoxMonData(boxMon, MON_DATA_PP1 + sMoveSelectionCursorPos, &pp2);
    SetBoxMonData(boxMon, MON_DATA_PP1 + sMoveSwapCursorPos, &pp1);
    SetBoxMonData(boxMon, MON_DATA_PP_BONUSES, &allMovesPPBonuses);
}

static void UpdateCurrentMonBufferFromPartyOrBox(struct Pokemon * mon)
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

static u8 PokeSum_CanForgetSelectedMove(void)
{
    u16 move;

    move = GetMonMoveBySlotId(&sMonSummaryScreen->currentMon, sMoveSelectionCursorPos);

    if (IsMoveHm(move) == TRUE && sMonSummaryScreen->mode != PSS_MODE_FORGET_MOVE)
        return FALSE;

    return TRUE;
}

static void Task_InputHandler_SelectOrForgetMove(u8 taskId)
{
    u8 i;

    switch (sMonSummaryScreen->selectMoveInputHandlerState)
    {
    case 0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->selectMoveInputHandlerState++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            PokeSum_TryPlayMonCry();
            sMonSummaryScreen->selectMoveInputHandlerState++;
        }
        break;
    case 2:
        if (JOY_NEW(DPAD_UP))
        {
            if (sMoveSelectionCursorPos > 0)
            {
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                for (i = sMoveSelectionCursorPos; i > 0; i--)
                    if (sMonSummaryScreen->moveIds[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i - 1;
                        return;
                    }
            }
            else
            {
                sMoveSelectionCursorPos = 4;
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sMoveSelectionCursorPos < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->selectMoveInputHandlerState = 3;

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                    v0--;

                for (i = sMoveSelectionCursorPos; i < v0; i++)
                    if (sMonSummaryScreen->moveIds[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->isSwappingMoves)
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = i;
                }

                return;
            }
            else if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            if (PokeSum_CanForgetSelectedMove() == TRUE || sMoveSelectionCursorPos == 4)
            {
                PlaySE(SE_SELECT);
                sMoveSwapCursorPos = sMoveSelectionCursorPos;
                gSpecialVar_0x8005 = sMoveSwapCursorPos;
                sMonSummaryScreen->selectMoveInputHandlerState = 6;
            }
            else
            {
                PlaySE(SE_FAILURE);
                sMonSummaryScreen->selectMoveInputHandlerState = 5;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            sMoveSwapCursorPos = 4;
            gSpecialVar_0x8005 = (u16)sMoveSwapCursorPos;
            sMonSummaryScreen->selectMoveInputHandlerState = 6;
        }
        break;
    case 3:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        sMonSummaryScreen->selectMoveInputHandlerState = 4;
        break;
    case 4:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 2;
        break;
    case 5:
        FillWindowPixelBuffer(sMonSummaryScreen->windowIds[4], 0);
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419CB9);
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
			AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419CB9Spa);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 2;
        break;
    case 6:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->selectMoveInputHandlerState++;
        break;
    default:
        if (!gPaletteFade.active)
            Task_DestroyResourcesOnExit(taskId);
        break;
    }
}

static void SpriteCB_PokeSum_MonPicSprite(struct Sprite * sprite)
{
//    if (sMonSummaryScreen->numMonPicBounces >= 2)
        return;

    if (sMonPicBounceState->initDelay++ >= 2)
    {
        u8 arrayLen;

        switch (sMonPicBounceState->vigor)
        {
        case 0:
            sprite->y += sMonPicBounceYDelta_Under60[sMonPicBounceState->animFrame++];
            arrayLen = NELEMS(sMonPicBounceYDelta_Under60);
            break;
        case 1:
            sprite->y += sMonPicBounceYDelta_60to80[sMonPicBounceState->animFrame++];
            arrayLen = NELEMS(sMonPicBounceYDelta_60to80);
            break;
        case 2:
            sprite->y += sMonPicBounceYDelta_80to99[sMonPicBounceState->animFrame++];
            arrayLen = NELEMS(sMonPicBounceYDelta_80to99);
            break;
        case 3:
        default:
            sprite->y += sMonPicBounceYDelta_Full[sMonPicBounceState->animFrame++];
            arrayLen = NELEMS(sMonPicBounceYDelta_Full);
            break;
        }

        if (sMonPicBounceState->animFrame >= arrayLen)
        {
            sMonPicBounceState->animFrame = 0;
            sMonSummaryScreen->numMonPicBounces++;
        }

        sMonPicBounceState->initDelay = 0;
    }
}

static void SpriteCB_PokeSum_EggPicShake(struct Sprite * sprite)
{
    if (sMonSummaryScreen->numMonPicBounces >= 2)
        return;

    switch (sMonPicBounceState->vigor)
    {
    case 0:
    default:
        if (sMonPicBounceState->initDelay++ >= 120)
        {
            sprite->x += sEggPicShakeXDelta_ItWillTakeSomeTime[sMonPicBounceState->animFrame];
            if (++sMonPicBounceState->animFrame >= NELEMS(sEggPicShakeXDelta_ItWillTakeSomeTime))
            {
                sMonPicBounceState->animFrame = 0;
                sMonPicBounceState->initDelay = 0;
                sMonSummaryScreen->numMonPicBounces++;
            }
        }
        break;
    case 1:
        if (sMonPicBounceState->initDelay++ >= 90)
        {
            sprite->x += sEggPicShakeXDelta_OccasionallyMoves[sMonPicBounceState->animFrame];
            if (++sMonPicBounceState->animFrame >= NELEMS(sEggPicShakeXDelta_OccasionallyMoves))
            {
                sMonPicBounceState->animFrame = 0;
                sMonPicBounceState->initDelay = 0;
                sMonSummaryScreen->numMonPicBounces++;
            }
        }
        break;
    case 2:
        if (sMonPicBounceState->initDelay++ >= 60)
        {
            sprite->x += sEggPicShakeXDelta_AlmostReadyToHatch[sMonPicBounceState->animFrame];
            if (++sMonPicBounceState->animFrame >= NELEMS(sEggPicShakeXDelta_AlmostReadyToHatch))
            {
                sMonPicBounceState->animFrame = 0;
                sMonPicBounceState->initDelay = 0;
                sMonSummaryScreen->numMonPicBounces++;
            }
        }
        break;
    }
}

static void SpriteCB_MonPicDummy(struct Sprite * sprite)
{
}

static void PokeSum_CreateMonPicSprite(void)
{
    u16 spriteId;
    u16 species;
    u32 personality;
    u32 trainerId;

    sMonPicBounceState = AllocZeroed(sizeof(struct MonPicBounceState));

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
    sMonSummaryScreen->monPicSpriteId = spriteId;
    ShowOrHideMonPicSprite(1);
    PokeSum_SetMonPicSpriteCallback(spriteId);
}

static void PokeSum_SetMonPicSpriteCallback(u16 spriteId)
{
    u16 curHp;
    u16 maxHp;

    sMonSummaryScreen->numMonPicBounces = 0;

    if (sMonSummaryScreen->isEgg == TRUE)
    {
        u8 friendship = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_FRIENDSHIP);

        if (friendship <= 5)
            sMonPicBounceState->vigor = 2;
        else
        {
            if (friendship <= 10)
                sMonPicBounceState->vigor = 1;
            else if (friendship <= 40)
                sMonPicBounceState->vigor = 0;
        }

        gSprites[spriteId].callback = SpriteCB_PokeSum_EggPicShake;
        return;
    }

    if (sMonSummaryScreen->curMonStatusAilment != AILMENT_NONE && sMonSummaryScreen->curMonStatusAilment != AILMENT_PKRS)
    {
        if (sMonSummaryScreen->curMonStatusAilment == AILMENT_FNT)
            return;

        gSprites[spriteId].callback = SpriteCB_MonPicDummy;
        return;
    }

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);

    if (curHp == maxHp)
        sMonPicBounceState->vigor = 3;
    else if (maxHp * 0.8 <= curHp)
        sMonPicBounceState->vigor = 2;
    else if (maxHp * 0.6 <= curHp)
        sMonPicBounceState->vigor = 1;
    else
        sMonPicBounceState->vigor = 0;

    gSprites[spriteId].callback = SpriteCB_PokeSum_MonPicSprite;
}

static void ShowOrHideMonPicSprite(u8 invisible)
{
    gSprites[sMonSummaryScreen->monPicSpriteId].invisible = invisible;
}

static void DestroyMonPicSprite(void)
{
    FreeAndDestroyMonPicSprite(sMonSummaryScreen->monPicSpriteId);
    FREE_AND_SET_NULL(sMonPicBounceState);
}

static void CreateBallIconObj(void)
{
    u16 ballItemId;
    u8 ballId;

    if (!sMonSummaryScreen->isEgg)
        ballItemId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_POKEBALL);
    else
        ballItemId = 0;

    ballId = ItemIdToBallId(ballItemId);
    LoadBallGfx(ballId);

    sMonSummaryScreen->ballIconSpriteId = CreateSprite(&gBallSpriteTemplates[ballId], 232, 39, 0);
    gSprites[sMonSummaryScreen->ballIconSpriteId].callback = SpriteCallbackDummy;
    gSprites[sMonSummaryScreen->ballIconSpriteId].oam.priority = 0;

    ShowOrHideBallIconObj(1);
}

static void ShowOrHideBallIconObj(u8 invisible)
{
    gSprites[sMonSummaryScreen->ballIconSpriteId].invisible = invisible;
}

static void DestroyBallIconObj(void)
{
    DestroySpriteAndFreeResources2(&gSprites[sMonSummaryScreen->ballIconSpriteId]);
}

static void CreateMonIconSprite(void)
{
    u16 species;
    u32 personality;

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY);

    SafeLoadMonIconPalette(species);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }

    if (!IsPokeSpriteNotFlipped(species))
        gSprites[sMonSummaryScreen->monIconSpriteId].hFlip = FALSE;
    else
        gSprites[sMonSummaryScreen->monIconSpriteId].hFlip = TRUE;

    ShowOrHideMonIconSprite(1);
}

static void ShowOrHideMonIconSprite(u8 invisible)
{
    gSprites[sMonSummaryScreen->monIconSpriteId].invisible = invisible;
}

static void DestroyMonIconSprite(void)
{
    u16 species;
    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    SafeFreeMonIconPalette(species);
    DestroyMonIcon(&gSprites[sMonSummaryScreen->monIconSpriteId]);
}

static void CreateMoveSelectionCursorObjs(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtrs[2];
    gfxBufferPtrs[0] = AllocZeroed(0x20 * 64);
    gfxBufferPtrs[1] = AllocZeroed(0x20 * 64);

    sMoveSelectionCursorObjs[0] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[1] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[2] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[3] = AllocZeroed(sizeof(struct MoveSelectionCursor));

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
            .oam = &sMoveSelectionCursorOamData,
            .anims = sMoveSelectionCursorOamAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCB_MoveSelectionCursor,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 64 * (i % 2) + 32, sMoveSelectionCursorPos * 28 + 34, i % 2);
        sMoveSelectionCursorObjs[i]->sprite = &gSprites[spriteId];
        sMoveSelectionCursorObjs[i]->whichSprite = i;
        sMoveSelectionCursorObjs[i]->tileTag = tileTag + i;
        sMoveSelectionCursorObjs[i]->palTag = palTag;
        sMoveSelectionCursorObjs[i]->sprite->subpriority = i;

        if (i > 1)
            StartSpriteAnim(sMoveSelectionCursorObjs[i]->sprite, 1);
    }

    ShowOrHideMoveSelectionCursor(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[0]);
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[1]);
}

static void ShowOrHideMoveSelectionCursor(u8 invisible)
{
    u8 i;
    for (i = 0; i < 4; i++)
        sMoveSelectionCursorObjs[i]->sprite->invisible = invisible;
}

static void SpriteCB_MoveSelectionCursor(struct Sprite * sprite)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (sMonSummaryScreen->isSwappingMoves == TRUE && i > 1)
            continue;

        sMoveSelectionCursorObjs[i]->sprite->y = sMoveSelectionCursorPos * 28 + 34;
    }

    if (sMonSummaryScreen->isSwappingMoves != TRUE)
    {
        if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
        {
            sMoveSelectionCursorObjs[0]->sprite->invisible = FALSE;
            sMoveSelectionCursorObjs[1]->sprite->invisible = FALSE;
        }
        return;
    }

    for (i = 0; i < 2; i++)
    {
        sprite = sMoveSelectionCursorObjs[i]->sprite;
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

static void DestroyMoveSelectionCursorObjs(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (sMoveSelectionCursorObjs[i]->sprite != NULL)
            DestroySpriteAndFreeResources(sMoveSelectionCursorObjs[i]->sprite);

        FREE_AND_SET_NULL_IF_SET(sMoveSelectionCursorObjs[i]);
    }
}

static void CreateMonStatusIconObj(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sStatusIcon = AllocZeroed(sizeof(struct MonStatusIconObj));
    gfxBufferPtr = AllocZeroed(0x20 * 32);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		LZ77UnCompWram(gIconStatusSummaryScreen, gfxBufferPtr);
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		LZ77UnCompWram(gIconStatusSummaryScreenSpa, gfxBufferPtr);

    if (sStatusIcon != NULL)
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
            .oam = &sStatusAilmentIconOamData,
            .anims = sStatusAilmentIconAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 0, 0, 0);
        sStatusIcon->sprite = &gSprites[spriteId];
        sStatusIcon->tileTag = tileTag;
        sStatusIcon->palTag = palTag;
    }

    ShowOrHideStatusIcon(1);
    UpdateMonStatusIconObj();
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void DestroyMonStatusIconObj(void)
{
    if (sStatusIcon->sprite != NULL)
        DestroySpriteAndFreeResources(sStatusIcon->sprite);

    FREE_AND_SET_NULL_IF_SET(sStatusIcon);
}

static void UpdateMonStatusIconObj(void)
{
    sMonSummaryScreen->curMonStatusAilment = StatusToAilment(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_STATUS));

    if (sMonSummaryScreen->curMonStatusAilment == AILMENT_NONE)
    {
        ShowOrHideStatusIcon(1);
        return;
    }

    StartSpriteAnim(sStatusIcon->sprite, sMonSummaryScreen->curMonStatusAilment - 1);
    ShowOrHideStatusIcon(0);
}

static void ShowOrHideStatusIcon(u8 invisible)
{
    if (sMonSummaryScreen->curMonStatusAilment == AILMENT_NONE || sMonSummaryScreen->isEgg)
        sStatusIcon->sprite->invisible = TRUE;
    else
        sStatusIcon->sprite->invisible = invisible;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sStatusIcon->sprite->y != 45)
        {
            sStatusIcon->sprite->x = 16;
            sStatusIcon->sprite->y = 45;
            return;
        }
    }
    else if (sStatusIcon->sprite->y != 38)
    {
        sStatusIcon->sprite->x = 214;
        sStatusIcon->sprite->y = 38;
        return;
    }
}

static void CreateHpBarObjs(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;
    u32 curHp;
    u32 maxHp;
    u8 hpBarPalTagOffset = 0;

    sHpBarObjs = AllocZeroed(sizeof(struct HpBarObjs));
    gfxBufferPtr = AllocZeroed(0x20 * 12);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		LZ77UnCompWram(gHpBarSummaryScreen, gfxBufferPtr);
    else if (gSaveBlock2Ptr->optionsLanguage == SPA)
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

        struct SpritePalette palette1 = {.data = sHpBarPals[0], .tag = palTag};
        struct SpritePalette palette2 = {.data = sHpBarPals[1], .tag = palTag + 1};
        struct SpritePalette palette3 = {.data = sHpBarPals[2], .tag = palTag + 2};

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
            .oam = &sHpOrExpBarOamData,
            .anims = sHpOrExpBarAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sHpBarObjs->xpos[i] = i * 8 + 81;
        spriteId = CreateSprite(&template, sHpBarObjs->xpos[i], 33, 0);
        sHpBarObjs->sprites[i] = &gSprites[spriteId];
        sHpBarObjs->sprites[i]->invisible = FALSE;
        sHpBarObjs->sprites[i]->oam.priority = 2;
        sHpBarObjs->tileTag = tileTag;
        sHpBarObjs->palTag = palTag;
        StartSpriteAnim(sHpBarObjs->sprites[i], 8);
    }

    UpdateHpBarObjs();
    ShowOrHideHpBarObjs(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void UpdateHpBarObjs(void)
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
        sHpBarObjs->sprites[i]->oam.paletteNum = IndexOfSpritePaletteTag(TAG_PSS_UNK_78) + hpBarPalOffset;

    if (curHp == maxHp)
        for (i = two; i < 8; i++)
            StartSpriteAnim(sHpBarObjs->sprites[i], 8);

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
            StartSpriteAnim(sHpBarObjs->sprites[i], 8);

        animNum = (v1 * 6) / v0;
        StartSpriteAnim(sHpBarObjs->sprites[numWholeHpBarTiles], animNum);

        for (i = numWholeHpBarTiles + 1; i < 8; i++)
            StartSpriteAnim(sHpBarObjs->sprites[i], 0);
    }

    StartSpriteAnim(sHpBarObjs->sprites[0], 9);
    StartSpriteAnim(sHpBarObjs->sprites[1], 10);
    StartSpriteAnim(sHpBarObjs->sprites[8], 11);
}

static void DestroyHpBarObjs(void)
{
    u8 i;

    for (i = 0; i < 9; i++)
        if (sHpBarObjs->sprites[i] != NULL)
            DestroySpriteAndFreeResources(sHpBarObjs->sprites[i]);

    FREE_AND_SET_NULL_IF_SET(sHpBarObjs);
}

static void ShowOrHideHpBarObjs(u8 invisible)
{
    u8 i;

    for (i = 0; i < 9; i++)
        sHpBarObjs->sprites[i]->invisible = invisible;
}

static void CreateExpBarObjs(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;

    sExpBarObjs = AllocZeroed(sizeof(struct ExpBarObjs));
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
            .oam = &sHpOrExpBarOamData,
            .anims = sHpOrExpBarAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sExpBarObjs->xpos[i] = i * 8 + 156;
        spriteId = CreateSprite(&template, sExpBarObjs->xpos[i], 130, 0);
        sExpBarObjs->sprites[i] = &gSprites[spriteId];
        sExpBarObjs->sprites[i]->oam.priority = 2;
        sExpBarObjs->tileTag = tileTag;
        sExpBarObjs->palTag = palTag;
    }

    UpdateExpBarObjs();
    ShowOrHideExpBarObjs(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void UpdateExpBarObjs(void)
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
            StartSpriteAnim(sExpBarObjs->sprites[i], 8);

        if (numWholeExpBarTiles >= 10)
        {
            if (totalExpToNextLevel == curExpToNextLevel)
                return;
            else
                StartSpriteAnim(sExpBarObjs->sprites[9], 7);
        }

        animNum = (v1 * 8) / v0;
        StartSpriteAnim(sExpBarObjs->sprites[numWholeExpBarTiles], animNum);

        for (i = numWholeExpBarTiles + 1; i < 10; i++)
            StartSpriteAnim(sExpBarObjs->sprites[i], 0);
    }
    else
        for (i = two; i < 10; i++)
            StartSpriteAnim(sExpBarObjs->sprites[i], 0);

    StartSpriteAnim(sExpBarObjs->sprites[0], 9);
    StartSpriteAnim(sExpBarObjs->sprites[1], 10);
    StartSpriteAnim(sExpBarObjs->sprites[10], 11);
}

static void DestroyExpBarObjs(void)
{
    u8 i;

    for (i = 0; i < 11; i++)
        if (sExpBarObjs->sprites[i] != NULL)
            DestroySpriteAndFreeResources(sExpBarObjs->sprites[i]);

    FREE_AND_SET_NULL_IF_SET(sExpBarObjs);
}

static void ShowOrHideExpBarObjs(u8 invisible)
{
    u8 i;

    for (i = 0; i < 11; i++)
        sExpBarObjs->sprites[i]->invisible = invisible;
}

static void CreatePokerusIconObj(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sPokerusIconObj = AllocZeroed(sizeof(struct PokerusIconObj));
    gfxBufferPtr = AllocZeroed(0x20 * 1);

    LZ77UnCompWram(sPokerusIconObjTiles, gfxBufferPtr);

    if (sPokerusIconObj != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 1,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = sPokerusIconObjPal, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sPokerusIconObjOamData,
            .anims = sPokerusIconObjAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 114, 92, 0);
        sPokerusIconObj->sprite = &gSprites[spriteId];
        sPokerusIconObj->tileTag = tileTag;
        sPokerusIconObj->palTag = palTag;
    }

    ShowOrHidePokerusIcon(1);
    ShowPokerusIconObjIfHasOrHadPokerus();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void DestroyPokerusIconObj(void)
{
    if (sPokerusIconObj->sprite != NULL)
        DestroySpriteAndFreeResources(sPokerusIconObj->sprite);

    FREE_AND_SET_NULL_IF_SET(sPokerusIconObj);
}

static void ShowPokerusIconObjIfHasOrHadPokerus(void)
{
    if (!CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0)
        && CheckPartyHasHadPokerus(&sMonSummaryScreen->currentMon, 0))
        ShowOrHidePokerusIcon(0);
    else
        ShowOrHidePokerusIcon(1);
}

static void ShowOrHidePokerusIcon(u8 invisible)
{
    if (!CheckPartyPokerus(&sMonSummaryScreen->currentMon, 0)
        && CheckPartyHasHadPokerus(&sMonSummaryScreen->currentMon, 0))
    {
        sPokerusIconObj->sprite->invisible = invisible;
        return;
    }
    else
        sPokerusIconObj->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sPokerusIconObj->sprite->invisible = TRUE;
        sPokerusIconObj->sprite->x = 16;
        sPokerusIconObj->sprite->y = 44;
    }
    else
    {
        sPokerusIconObj->sprite->x = 114;
        sPokerusIconObj->sprite->y = 92;
    }
}

static void CreateShinyStarObj(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sShinyStarObjData = AllocZeroed(sizeof(struct ShinyStarObjData));
    gfxBufferPtr = AllocZeroed(0x20 * 2);

    LZ77UnCompWram(gShinyIndicatorGfx, gfxBufferPtr);

    if (sShinyStarObjData != NULL)
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
            .oam = &sStarObjOamData,
            .anims = sStarObjAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
        spriteId = CreateSprite(&template, 166, 50, 0);
        sShinyStarObjData->sprite = &gSprites[spriteId];
        sShinyStarObjData->tileTag = tileTag;
        sShinyStarObjData->palTag = palTag;
    }

    ShowOrHideShinyStar(1);
    ShowShinyStarObjIfMonShiny();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void DestroyShinyStarObj(void)
{
    if (sShinyStarObjData->sprite != NULL)
        DestroySpriteAndFreeResources(sShinyStarObjData->sprite);

    FREE_AND_SET_NULL_IF_SET(sShinyStarObjData);
}

static void ShowOrHideShinyStar(u8 invisible)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE
        && !sMonSummaryScreen->isEgg)
        sShinyStarObjData->sprite->invisible = invisible;
    else
        sShinyStarObjData->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sShinyStarObjData->sprite->x = 126;
        sShinyStarObjData->sprite->y = 20;
    }
    else
    {
        sShinyStarObjData->sprite->x = 166;
        sShinyStarObjData->sprite->y = 50;
    }
}

static void ShowShinyStarObjIfMonShiny(void)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE && !sMonSummaryScreen->isEgg)
        ShowOrHideShinyStar(0);
    else
        ShowOrHideShinyStar(1);
}

static void PokeSum_DestroySprites(void)
{
    DestroyMoveSelectionCursorObjs();
    DestroyHpBarObjs();
    DestroyExpBarObjs();
    DestroyMonPicSprite();
    DestroyMonIconSprite();
    DestroyBallIconObj();
    DestroyMonMarkingsSprite();
    DestroyMonStatusIconObj();
    DestroyPokerusIconObj();
    DestroyShinyStarObj();
    ResetSpriteData();
}

static void PokeSum_CreateSprites(void)
{
    CreateBallIconObj();
    ShowOrHideBallIconObj(0);
    CreateMonIconSprite();
    PokeSum_CreateMonPicSprite();
    ShowOrHideMonPicSprite(0);
    UpdateHpBarObjs();
    UpdateExpBarObjs();
    PokeSum_UpdateMonMarkingsAnim();
    UpdateMonStatusIconObj();
    ShowPokerusIconObjIfHasOrHadPokerus();
    ShowShinyStarObjIfMonShiny();
}

static void PokeSum_CreateMonMarkingsSprite(void)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    DestroySpriteAndFreeResources(sMonSummaryScreen->markingSprite);
    sMonSummaryScreen->markingSprite = CreateMonMarkingSprite_SelectCombo(TAG_PSS_UNK_8C, TAG_PSS_UNK_8C, sMonMarkingSpritePalette);

    if (sMonSummaryScreen->markingSprite != NULL)
    {
        StartSpriteAnim(sMonSummaryScreen->markingSprite, markings);
        sMonSummaryScreen->markingSprite->x = 208;
        sMonSummaryScreen->markingSprite->y = 114;
    }

    ShowOrHideMonMarkingsSprite(1);
}

static void DestroyMonMarkingsSprite(void)
{
    DestroySpriteAndFreeResources(sMonSummaryScreen->markingSprite);
}

static void ShowOrHideMonMarkingsSprite(u8 invisible)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    if (markings == 0)
        sMonSummaryScreen->markingSprite->invisible = TRUE;
    else
        sMonSummaryScreen->markingSprite->invisible = invisible;
}

static void PokeSum_UpdateMonMarkingsAnim(void)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    StartSpriteAnim(sMonSummaryScreen->markingSprite, markings);
    ShowOrHideMonMarkingsSprite(0);
}

static void PokeSum_SeekToNextMon(u8 taskId, s8 direction)
{
    s8 scrollResult = -1;

    if (sMonSummaryScreen->isBoxMon == TRUE)
    {
        if (sMonSummaryScreen->curPageIndex != 0)
        {
            if (direction == 1)
                direction = 0;
            else
                direction = 2;
        }
        else
        {
            // Allow Eggs
            if (direction == 1)
                direction = 1;
            else
                direction = 3;
        }

        scrollResult = SeekToNextMonInBox(sMonSummaryScreen->monList.boxMons, GetLastViewedMonIndex(), sMonSummaryScreen->lastIndex, (u8)direction);
    }
    else
    {
        if (IsUpdateLinkStateCBActive() == FALSE
            && gReceivedRemoteLinkPlayers == 1
            && IsMultiBattle() == TRUE)
            scrollResult = SeekToNextMonInMultiParty(direction);
        else
            scrollResult = SeekToNextMonInSingleParty(direction);
    }

    if (scrollResult == -1)
        return;

    sLastViewedMonIndex = scrollResult;
    CreateTask(Task_PokeSum_SwitchDisplayedPokemon, 0);
    sMonSummaryScreen->switchMonTaskState = 0;
}

static s8 SeekToNextMonInSingleParty(s8 delta)
{
    struct Pokemon *mon = sMonSummaryScreen->monList.mons;
    u8 index = sLastViewedMonIndex;
    u8 numMons = sMonSummaryScreen->lastIndex + 1;
    delta += numMons;

    index = (index + delta) % numMons;

    // skip over any Eggs unless on the Info Page
    if (sMonSummaryScreen->curPageIndex != PSS_PAGE_INFO)
        while (GetMonData(&mon[index], MON_DATA_IS_EGG))
            index = (index + delta) % numMons;

    // to avoid "scrolling" to the same Pokemon
    if (index == sLastViewedMonIndex)
        return -1;
    else
        return index;
}

static u8 PokeSum_CanSeekToMon(struct Pokemon * partyMons)
{
    if (GetMonData(partyMons, MON_DATA_SPECIES) != 0 && (sMonSummaryScreen->curPageIndex != 0 || GetMonData(partyMons, MON_DATA_IS_EGG) == 0))
        return TRUE;

    return FALSE;
}

static s8 SeekToMonInMultiParty_SeekForward(u8 a0)
{
    while (TRUE)
    {
        a0++;

        if (a0 == 6)
            return -1;
        if (PokeSum_CanSeekToMon(&gPlayerParty[sMultiBattlePartyOrder[a0]]) == TRUE)
            break;
    }

    return (s8)sMultiBattlePartyOrder[a0];
}

static s8 SeekToMonInMultiParty_SeekBack(u8 a0)
{
    while (1)
    {
        if (a0 == 0)
            return -1;

        a0--;

        if (PokeSum_CanSeekToMon(&gPlayerParty[sMultiBattlePartyOrder[a0]]) == TRUE)
            break;
    }

    return (s8)(sMultiBattlePartyOrder[a0]);
}

static s8 SeekToNextMonInMultiParty(s8 direction)
{
    u8 foundPartyIdx = 0;
    u8 i;

    for (i = 0; i < PARTY_SIZE; i++)
        if (sMultiBattlePartyOrder[i] == GetLastViewedMonIndex())
        {
            foundPartyIdx = i;
            break;
        }

    if ((direction == -1 && foundPartyIdx == 0)
        || (direction == 1 && foundPartyIdx == 5))
        return -1;

    if (direction == 1)
        return SeekToMonInMultiParty_SeekForward(foundPartyIdx);
    else
        return SeekToMonInMultiParty_SeekBack(foundPartyIdx);
}

static void Task_PokeSum_SwitchDisplayedPokemon(u8 id)
{
	s16 * data = gTasks[id].data;
	
    switch (sMonSummaryScreen->switchMonTaskState)
    {
    case 0:
        StopCryAndClearCrySongs();
        sMoveSelectionCursorPos = 0;
        sMoveSwapCursorPos = 0;
        DestroyMonPicSprite();
        DestroyMonIconSprite();
        DestroyBallIconObj();
        BufferSelectedMonData(&sMonSummaryScreen->currentMon);

        sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG);
        sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG);

        if (sMonSummaryScreen->isBadEgg == TRUE)
            sMonSummaryScreen->isEgg = TRUE;

        BufferMonInfo();
        if (!sMonSummaryScreen->isEgg)
		{
            BufferMonSkills();
			BufferMonMoves();
		}
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintMonTypeIcons();
        PokeSum_DrawBg3Tilemap();
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 1:
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_PAGE_NAME], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_CONTROLS], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[POKESUM_WIN_LVL_NICK], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
	    if (!Overworld_LinkRecvQueueLengthMoreThan2() && !sub_800B270())
        {
            PokeSum_CreateSprites();
            PokeSum_TryPlayMonCry();
		}
		if (sMonSummaryScreen->curPageIndex == PSS_PAGE_IVS || sMonSummaryScreen->curPageIndex == PSS_PAGE_EVS)
		{
			sMonSummaryScreen->isDrawingIvEvsPage = 1;
			returnTask = 0;
			counter = 0;
			gTasks[id].func = Task_PokeSum_AnimateIvsEvsStats;
			sMonSummaryScreen->switchMonTaskState++;
			break;
		}
        sMonSummaryScreen->switchMonTaskState++;
        break;
    default:
        sMonSummaryScreen->switchMonTaskState = 0;
        DestroyTask(id);
        break;
    }
}

static void Task_PokeSum_AnimateIvsEvsStats(u8 id)
{
	s16 * data = gTasks[id].data;
	s16 i = counter;
	
	if (sMonSummaryScreen->curPageIndex == PSS_PAGE_IVS)
	{
		maxValue = 31;
		Hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_IV);
		atk = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_IV);
		def = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_IV);
		spA = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_IV);
		spD = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_IV);
		spe = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_IV);
		total = Hp + atk + def + spA + spD + spe;
		totalMax = 186;
	}
	else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_EVS)
	{
		maxValue = 252;
		Hp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_EV);
		atk = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_EV);
		def = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_EV);
		spA = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_EV);
		spD = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_EV);
		spe = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_EV);
		total = Hp + atk + def + spA + spD + spe;
		totalMax = 510;
	}			
	if (i <= 40)
	{
		if ((40 * Hp / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE114 + (i / 8) * 2), 2);
		if ((40 * atk / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE194 + (i / 8) * 2), 2);
		if ((40 * def / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE214 + (i / 8) * 2), 2);
		if ((40 * spA / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE294 + (i / 8) * 2), 2);
		if ((40 * spD / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE314 + (i / 8) * 2), 2);
		if ((40 * spe / maxValue) > i)
			DmaFill16(3, 0x00BA + (i % 8), (void *)(VRAM + 0xE394 + (i / 8) * 2), 2);
	}
	if ((80 * total / totalMax) > i)
	{
		if ((80 * total / totalMax) > i)
			DmaFill16(3, 0x00C2 + (i % 8), (void *)(VRAM + 0xE494 + (i / 8) * 2), 2);
	}
    else
	{
		if (returnTask == 0)
		{
			sMonSummaryScreen->isDrawingIvEvsPage = 0;
			sMonSummaryScreen->switchMonTaskState++;
			gTasks[id].func = Task_PokeSum_SwitchDisplayedPokemon;
		}
		else
		{
			sMonSummaryScreen->isDrawingIvEvsPage = 0;
			data[0]++;
			gTasks[id].func = Task_PokeSum_FlipPages;
		}
	}
	counter++;
}

static void PokeSum_TryPlayMonCry(void)
{
    if (!GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG))
    {
        if (ShouldPlayNormalPokeCry(&sMonSummaryScreen->currentMon) == TRUE)
            PlayCry3(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2), 0, 0);
        else
            PlayCry3(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2), 0, 11);
    }
}

static bool32 PokeSum_IsMonBoldOrGentle(u8 nature)
{
    if (nature == NATURE_BOLD || nature == NATURE_GENTLE)
        return TRUE;

    return FALSE;
}

static bool32 CurrentMonIsFromGBA(void)
{
    u8 version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

    if (version == VERSION_LEAF_GREEN || version == VERSION_FIRE_RED || version == VERSION_RUBY || version == VERSION_SAPPHIRE || version == VERSION_EMERALD)
        return TRUE;

    return FALSE;
}

static bool32 MapSecIsInKantoOrSevii(u8 place)
{
    if (place >= MAPSECS_KANTO && place < MAPSEC_NONE)
        return TRUE;
    return FALSE;
}

static bool32 IsStatsPagesUnlocked(void)
{
    if (FlagGet(FLAG_STATS_PAGE_UNLOCKED))
        return TRUE;
    return TRUE; //FALSE
}

static void ScrollPSSBackground(void)
{
    ChangeBgX(2, 80, 1);
    ChangeBgY(2, 80, 1);
}
