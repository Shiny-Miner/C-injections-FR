static const struct BgTemplate sPartyMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 28,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
};

enum
{
    PARTY_BOX_LEFT_COLUMN,
    PARTY_BOX_RIGHT_COLUMN,
};

static const struct PartyMenuBoxInfoRects sPartyBoxInfoRects[] =
{
    [PARTY_BOX_LEFT_COLUMN] = 
    {
        BlitBitmapToPartyWindow_LeftColumn, 
        {
            // The below are the x, y, width, and height for each of the following info
            30,  2, 40, 12, // Nickname
            81,  2, 32,  8, // Level
            30, 12,  8,  8, // Gender
            56, 18, 24,  8, // HP
            80, 18, 24,  8, // Max HP
            64, 17, 48,  3  // HP bar
        }, 
        56, 7, 64, 16      // Description text (e.g. NO USE)
    },
    [PARTY_BOX_RIGHT_COLUMN] = 
    {
        BlitBitmapToPartyWindow_RightColumn, 
        {
             // The below are the x, y, width, and height for each of the following info
             40,  4, 40, 12, // Nickname
              6, 24, 32,  8, // Level
             98,  5,  8,  8, // Gender
             59, 24, 16,  8, // HP
             74, 24, 30,  8, // Max HP
             56, 22, 48,  3  // HP bar
        }, 
        40, 15, 64, 16        // Description text
    },
};

static const u8 sPartyMenuSpriteCoords[PARTY_LAYOUT_COUNT][PARTY_SIZE][4 * 2] =
{
    [PARTY_LAYOUT_SINGLE] = 
    {
        { 34,  12,  34,  24,  26,  33,  24,  16},
        {146,  20, 146,  32, 138,  41, 136,  24},
        { 34,  52,  34,  64,  26,  73,  24,  56},
        {146,  60, 146,  72, 138,  81, 136,  64},
        { 34,  92,  34, 104,  26, 113,  24,  96},
        {146, 100, 146, 112, 138, 121, 136, 104},
    },
    [PARTY_LAYOUT_DOUBLE] = 
    {
        { 34,  12,  34,  24,  26,  33,  24,  16},
        {146,  20, 146,  32, 138,  41, 136,  24},
        { 34,  52,  34,  64,  26,  73,  24,  56},
        {146,  60, 146,  72, 138,  81, 136,  64},
        { 34,  92,  34, 104,  26, 113,  24,  96},
        {146, 100, 146, 112, 138, 121, 136, 104},
    },
    [PARTY_LAYOUT_MULTI] = 
    {
        { 34,  12,  34,  24,  26,  33,  24,  16},
        {146,  20, 146,  32, 138,  41, 136,  24},
        { 34,  52,  34,  64,  26,  73,  24,  56},
        { 34,  92,  34, 104,  26, 113,  24,  96},
        {146,  60, 146,  72, 138,  81, 136,  64},
        {146, 100, 146, 112, 138, 121, 136, 104},
    },
    [PARTY_LAYOUT_MULTI_SHOWCASE] = 
    {
        { 34,  12,  34,  24,  26,  33,  24,  16},
        { 34,  52,  34,  64,  26,  73,  24,  56},
        { 34,  92,  34, 104,  26, 113,  24,  96},
        {146,  20, 146,  32, 138,  41, 136,  24},
        {146,  60, 146,  72, 138,  81, 136,  64},
        {146, 100, 146, 112, 138, 121, 136, 104},
    },
};

static const u32 sConfirmButton_Tilemap[] = INCBIN_U32("graphics/interface/party_menu_confirm_button.bin");
static const u32 sCancelButton_Tilemap[] = INCBIN_U32("graphics/interface/party_menu_cancel_button.bin");

static const u8 sFontColorTable[][3] =
{
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY},  // Default
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_GREEN},      // Unused
    {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_2,  TEXT_DYNAMIC_COLOR_3},  // Gender symbol
    {TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY}, // Selection actions
    {TEXT_COLOR_WHITE,       TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_BLUE}, // Field moves
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},  // Unused
};

static const struct WindowTemplate sSinglePartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 14,
        .height = 5,
        .paletteNum = 3,
        .baseBlock = 100,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 1,
        .width = 14,
        .height = 5,
        .paletteNum = 4,
        .baseBlock = 170,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 5,
        .width = 14,
        .height = 5,
        .paletteNum = 5,
        .baseBlock = 240,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 14,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 310,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 10,
        .width = 14,
        .height = 5,
        .paletteNum = 7,
        .baseBlock = 380,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 11,
        .width = 14,
        .height = 5,
        .paletteNum = 8,
        .baseBlock = 450,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 520,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sDoublePartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 14,
        .height = 5,
        .paletteNum = 3,
        .baseBlock = 100,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 1,
        .width = 14,
        .height = 5,
        .paletteNum = 4,
        .baseBlock = 170,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 5,
        .width = 14,
        .height = 5,
        .paletteNum = 5,
        .baseBlock = 240,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 14,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 310,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 10,
        .width = 14,
        .height = 5,
        .paletteNum = 7,
        .baseBlock = 380,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 11,
        .width = 14,
        .height = 5,
        .paletteNum = 8,
        .baseBlock = 450,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 520,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sMultiPartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 14,
        .height = 5,
        .paletteNum = 3,
        .baseBlock = 100,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 1,
        .width = 14,
        .height = 5,
        .paletteNum = 4,
        .baseBlock = 170,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 5,
        .width = 14,
        .height = 5,
        .paletteNum = 5,
        .baseBlock = 240,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 14,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 310,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 10,
        .width = 14,
        .height = 5,
        .paletteNum = 7,
        .baseBlock = 380,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 11,
        .width = 14,
        .height = 5,
        .paletteNum = 8,
        .baseBlock = 450,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 520,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sShowcaseMultiPartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 14,
        .height = 5,
        .paletteNum = 3,
        .baseBlock = 100,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 1,
        .width = 14,
        .height = 5,
        .paletteNum = 4,
        .baseBlock = 170,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 5,
        .width = 14,
        .height = 5,
        .paletteNum = 5,
        .baseBlock = 240,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 14,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 310,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 10,
        .width = 14,
        .height = 5,
        .paletteNum = 7,
        .baseBlock = 380,
    },
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 11,
        .width = 14,
        .height = 5,
        .paletteNum = 8,
        .baseBlock = 450,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 520,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sCancelButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 17,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 632,
};

static const struct WindowTemplate sMultiCancelButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 18,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 632,
};

static const struct WindowTemplate sConfirmButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 16,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 644,
};

static const struct WindowTemplate sDefaultPartyMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 22,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 656,
};

static const struct WindowTemplate sDoWhatWithMonMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 17,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 656,
};

static const struct WindowTemplate sDoWhatWithItemMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 20,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 656,
};

static const struct WindowTemplate sDoWhatWithMailMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 17,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 656,
};

static const struct WindowTemplate sWhichMoveMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 16,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 656,
};

static const struct WindowTemplate sNicknameChangeWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 23,
    .tilemapTop = 15,
    .width = 6,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 774,
};

static const struct WindowTemplate sItemGiveTakeMoveWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 23,
    .tilemapTop = 11,
    .width = 6,
    .height = 8,
    .paletteNum = 14,
    .baseBlock = 774,
};

static const struct WindowTemplate sMailReadTakeWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 20,
    .tilemapTop = 13,
    .width = 9,
    .height = 6,
    .paletteNum = 14,
    .baseBlock = 774,
};

static const struct WindowTemplate sMoveSelectWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 19,
    .tilemapTop = 11,
    .width = 10,
    .height = 8,
    .paletteNum = 14,
    .baseBlock = 774,
};

static const struct WindowTemplate sPartyMenuYesNoWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 856,
};

static const struct WindowTemplate sLevelUpStatsWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 19,
    .tilemapTop = 1,
    .width = 10,
    .height = 11,
    .paletteNum = 14,
    .baseBlock = 880,
};

static const struct WindowTemplate sTutorialOakWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 2,
    .tilemapTop = 15,
    .width = 26,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 520,
};

static const struct WindowTemplate sHMDescriptionWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 0,
    .tilemapTop = 13,
    .width = 18,
    .height = 3,
    .paletteNum = 12,
    .baseBlock = 520,
};

static const u8 sMainSlotTileNums[] =
{
	 9, 10, 11, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 
	15, 16, 17, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	21, 17, 17, 17, 22, 56, 57, 49, 49, 49, 49, 49, 49, 50, 
	25, 17, 17, 17, 26, 58, 59, 60, 60, 60, 60, 60, 53, 54, 
	30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33,
};

static const u8 sMainSlotTileNumsSpa[] =
{
	 9, 10, 11, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 
	15, 16, 17, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	21, 17, 17, 17, 22, 47, 48, 49, 49, 49, 49, 49, 49, 50, 
	25, 17, 17, 17, 26, 51, 52, 60, 60, 60, 60, 60, 53, 54, 
	30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33,
};

static const u8 sMainSlotTileNums_Egg[] =
{
	 9, 10, 11, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 
	15, 16, 17, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	21, 17, 17, 17, 22, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	25, 17, 17, 17, 26, 27, 27, 27, 27, 27, 27, 27, 28, 29, 
	30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33,
};

static const u8 sOtherSlotsTileNums[] =
{
	34, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 37, 
	38, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 
	41, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 
	41, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 42, 
	43, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 46,
};

static const u8 sOtherSlotsTileNums_Egg[] =
{
	 9, 10, 11, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 
	15, 16, 17, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	21, 17, 17, 17, 22, 19, 19, 19, 19, 19, 19, 19, 19, 20, 
	25, 17, 17, 17, 26, 27, 27, 27, 27, 27, 27, 27, 28, 29, 
	30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33,
};

static const u8 sEmptySlotTileNums[] = 
{
	34, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 37, 
	38, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 
	41, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 
	41, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 42, 
	43, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 46,
};

static const u8 sGenderPalOffsets[] = {11, 12};

static const u8 sHPBarPalOffsets[] = {9, 10};

static const u8 sPartyBoxPalOffsets1[] = {4, 5, 6};

static const u8 sPartyBoxPalOffsets2[] = {1, 7, 8};

static const u8 sGenderMalePalIds[] = {59, 60};

static const u8 sGenderFemalePalIds[] = {75, 76};

static const u8 sHPBarGreenPalIds[] = {57, 58};

static const u8 sHPBarYellowPalIds[] = {73, 74};

static const u8 sHPBarRedPalIds[] = {89, 90};

static const u8 sPartyBoxEmptySlotPalIds1[] = {52, 53, 54};

static const u8 sPartyBoxMultiPalIds1[] = {68, 69, 70};

static const u8 sPartyBoxFaintedPalIds1[] = {84, 85, 86};

static const u8 sPartyBoxCurrSelectionPalIds1[] = {116, 117, 118};

static const u8 sPartyBoxCurrSelectionMultiPalIds[] = {132, 133, 134};

static const u8 sPartyBoxCurrSelectionFaintedPalIds[] = {148, 149, 150};

static const u8 sPartyBoxSelectedForActionPalIds1[] = {100, 101, 102};

static const u8 sPartyBoxEmptySlotPalIds2[] = {49, 55, 56};

static const u8 sPartyBoxMultiPalIds2[] = {65, 71, 72};

static const u8 sPartyBoxFaintedPalIds2[] = {81, 87, 88};

static const u8 sPartyBoxCurrSelectionPalIds2[] = {97, 103, 104};

static const u8 sPartyBoxSelectedForActionPalIds2[] = {161, 167, 168};

static const u8 *const sActionStringTable[] =
{
    [PARTY_MSG_CHOOSE_MON]             = gText_ChoosePokemon,
    [PARTY_MSG_CHOOSE_MON_OR_CANCEL]   = gText_ChoosePokemonCancel,
    [PARTY_MSG_CHOOSE_MON_AND_CONFIRM] = gText_ChoosePokemonConfirm,
    [PARTY_MSG_MOVE_TO_WHERE]          = gText_MoveToWhere,
    [PARTY_MSG_TEACH_WHICH_MON]        = gText_TeachWhichPokemon,
    [PARTY_MSG_USE_ON_WHICH_MON]       = gText_UseOnWhichPokemon,
    [PARTY_MSG_GIVE_TO_WHICH_MON]      = gText_GiveToWhichPokemon,
    [PARTY_MSG_NOTHING_TO_CUT]         = gText_NothingToCut,
    [PARTY_MSG_CANT_SURF_HERE]         = gText_CantSurfHere,
    [PARTY_MSG_ALREADY_SURFING]        = gText_AlreadySurfing,
    [PARTY_MSG_CURRENT_TOO_FAST]       = gText_CurrentIsTooFast,
    [PARTY_MSG_ENJOY_CYCLING]          = gText_EnjoyCycling,
    [PARTY_MSG_ALREADY_IN_USE]         = gText_InUseAlready_PM,
    [PARTY_MSG_CANT_USE_HERE]          = gText_CantUseHere,
    [PARTY_MSG_NO_MON_FOR_BATTLE]      = gText_NoPokemonForBattle,
    [PARTY_MSG_CHOOSE_MON_2]           = gText_ChoosePokemon2,
    [PARTY_MSG_NOT_ENOUGH_HP]          = gText_NotEnoughHp,
    [PARTY_MSG_THREE_MONS_ARE_NEEDED]  = gText_ThreePkmnAreNeeded,
    [PARTY_MSG_TWO_MONS_ARE_NEEDED]    = gText_TwoPokemonAreNeeded,
    [PARTY_MSG_MONS_CANT_BE_SAME]      = gText_PokemonCantBeSame,
    [PARTY_MSG_NO_SAME_HOLD_ITEMS]     = gText_NoIdenticalHoldItems,
    [PARTY_MSG_UNUSED]                 = gText_StringDummy,
    [PARTY_MSG_DO_WHAT_WITH_MON]       = gText_DoWhatWithPokemon,
    [PARTY_MSG_RESTORE_WHICH_MOVE]     = gText_RestoreWhichMove,
    [PARTY_MSG_BOOST_PP_WHICH_MOVE]    = gText_BoostPp,
    [PARTY_MSG_DO_WHAT_WITH_NICKNAME]  = gText_DoWhatWithNickname,
    [PARTY_MSG_DO_WHAT_WITH_ITEM]      = gText_DoWhatWithItem,
    [PARTY_MSG_MOVE_ITEM_TO_WHERE]     = gText_MoveToWhere,
    [PARTY_MSG_DO_WHAT_WITH_MAIL]      = gText_DoWhatWithMail,
    [PARTY_MSG_CHOOSE_MON + 29]             = gText_ChoosePokemonSpa,
    [PARTY_MSG_CHOOSE_MON_OR_CANCEL + 29]   = gText_ChoosePokemonCancelSpa,
    [PARTY_MSG_CHOOSE_MON_AND_CONFIRM + 29] = gText_ChoosePokemonConfirmSpa,
    [PARTY_MSG_MOVE_TO_WHERE + 29]          = gText_MoveToWhereSpa,
    [PARTY_MSG_TEACH_WHICH_MON + 29]        = gText_TeachWhichPokemonSpa,
    [PARTY_MSG_USE_ON_WHICH_MON + 29]       = gText_UseOnWhichPokemonSpa,
    [PARTY_MSG_GIVE_TO_WHICH_MON + 29]      = gText_GiveToWhichPokemonSpa,
    [PARTY_MSG_NOTHING_TO_CUT + 29]         = gText_NothingToCutSpa,
    [PARTY_MSG_CANT_SURF_HERE + 29]         = gText_CantSurfHereSpa,
    [PARTY_MSG_ALREADY_SURFING + 29]        = gText_AlreadySurfingSpa,
    [PARTY_MSG_CURRENT_TOO_FAST + 29]       = gText_CurrentIsTooFastSpa,
    [PARTY_MSG_ENJOY_CYCLING + 29]          = gText_EnjoyCyclingSpa,
    [PARTY_MSG_ALREADY_IN_USE + 29]         = gText_InUseAlready_PMSpa,
    [PARTY_MSG_CANT_USE_HERE + 29]          = gText_CantUseHereSpa,
    [PARTY_MSG_NO_MON_FOR_BATTLE + 29]      = gText_NoPokemonForBattleSpa,
    [PARTY_MSG_CHOOSE_MON_2 + 29]           = gText_ChoosePokemon2Spa,
    [PARTY_MSG_NOT_ENOUGH_HP + 29]          = gText_NotEnoughHpSpa,
    [PARTY_MSG_THREE_MONS_ARE_NEEDED + 29]  = gText_ThreePkmnAreNeededSpa,
    [PARTY_MSG_TWO_MONS_ARE_NEEDED + 29]    = gText_TwoPokemonAreNeededSpa,
    [PARTY_MSG_MONS_CANT_BE_SAME + 29]      = gText_PokemonCantBeSameSpa,
    [PARTY_MSG_NO_SAME_HOLD_ITEMS + 29]     = gText_NoIdenticalHoldItemsSpa,
    [PARTY_MSG_UNUSED + 29]                 = gText_StringDummySpa,
    [PARTY_MSG_DO_WHAT_WITH_MON + 29]       = gText_DoWhatWithPokemonSpa,
    [PARTY_MSG_RESTORE_WHICH_MOVE + 29]     = gText_RestoreWhichMoveSpa,
    [PARTY_MSG_BOOST_PP_WHICH_MOVE + 29]    = gText_BoostPpSpa,
    [PARTY_MSG_DO_WHAT_WITH_NICKNAME + 29]  = gText_DoWhatWithNicknameSpa,
    [PARTY_MSG_DO_WHAT_WITH_ITEM + 29]      = gText_DoWhatWithItemSpa,
    [PARTY_MSG_MOVE_ITEM_TO_WHERE + 29]     = gText_MoveToWhereSpa,
    [PARTY_MSG_DO_WHAT_WITH_MAIL + 29]      = gText_DoWhatWithMailSpa,
};

static const u8 *const sDescriptionStringTable[] =
{
    [PARTYBOX_DESC_NO_USE]     = gText_NoUse,
    [PARTYBOX_DESC_ABLE_3]     = gText_Able,
    [PARTYBOX_DESC_FIRST]      = gText_First_PM,
    [PARTYBOX_DESC_SECOND]     = gText_Second_PM,
    [PARTYBOX_DESC_THIRD]      = gText_Third_PM,
    [PARTYBOX_DESC_ABLE]       = gText_Able2,
    [PARTYBOX_DESC_NOT_ABLE]   = gText_NotAble,
    [PARTYBOX_DESC_ABLE_2]     = gText_Able3,
    [PARTYBOX_DESC_NOT_ABLE_2] = gText_NotAble2,
    [PARTYBOX_DESC_LEARNED]    = gText_Learned,
    [PARTYBOX_DESC_NO_USE + 10]     = gText_NoUseSpa,
    [PARTYBOX_DESC_ABLE_3 + 10]     = gText_AbleSpa,
    [PARTYBOX_DESC_FIRST + 10]      = gText_First_PMSpa,
    [PARTYBOX_DESC_SECOND + 10]     = gText_Second_PMSpa,
    [PARTYBOX_DESC_THIRD + 10]      = gText_Third_PMSpa,
    [PARTYBOX_DESC_ABLE + 10]       = gText_Able2Spa,
    [PARTYBOX_DESC_NOT_ABLE + 10]   = gText_NotAbleSpa,
    [PARTYBOX_DESC_ABLE_2 + 10]     = gText_Able3Spa,
    [PARTYBOX_DESC_NOT_ABLE_2 + 10] = gText_NotAble2Spa,
    [PARTYBOX_DESC_LEARNED + 10]    = gText_LearnedSpa,
};

static const u8 *const sHMDescriptionTable[] =
{
    [FIELD_MOVE_FLASH]          = gText_LightUpDarkness,
    [FIELD_MOVE_CUT]            = gText_CutATreeOrGrass,
    [FIELD_MOVE_FLY]            = gText_FlyToAKnownTown,
    [FIELD_MOVE_STRENGTH]       = gText_MoveHeavyBoulders,
    [FIELD_MOVE_SURF]           = gText_TravelOnWater,
    [FIELD_MOVE_ROCK_SMASH]     = gText_ShatterACrackedRock,
    [FIELD_MOVE_WATERFALL]      = gText_ClimbAWaterfall,
    [FIELD_MOVE_TELEPORT]       = gText_ReturnToAHealingSpot,
    [FIELD_MOVE_DIG]            = gText_EscapeFromHere,
    [FIELD_MOVE_MILK_DRINK]     = gText_ShareHp,
    [FIELD_MOVE_SOFT_BOILED]    = gText_ShareHp,
    [FIELD_MOVE_SWEET_SCENT]    = gText_LureWildPokemon,
    [FIELD_MOVE_DIVE]           = gText_DiveUnderwater,
    [FIELD_MOVE_FLASH + 13]          = gText_LightUpDarknessSpa,
    [FIELD_MOVE_CUT + 13]            = gText_CutATreeOrGrassSpa,
    [FIELD_MOVE_FLY + 13]            = gText_FlyToAKnownTownSpa,
    [FIELD_MOVE_STRENGTH + 13]       = gText_MoveHeavyBouldersSpa,
    [FIELD_MOVE_SURF + 13]           = gText_TravelOnWaterSpa,
    [FIELD_MOVE_ROCK_SMASH + 13]     = gText_ShatterACrackedRockSpa,
    [FIELD_MOVE_WATERFALL + 13]      = gText_ClimbAWaterfallSpa,
    [FIELD_MOVE_TELEPORT + 13]       = gText_ReturnToAHealingSpotSpa,
    [FIELD_MOVE_DIG + 13]            = gText_EscapeFromHereSpa,
    [FIELD_MOVE_MILK_DRINK + 13]     = gText_ShareHpSpa,
    [FIELD_MOVE_SOFT_BOILED + 13]    = gText_ShareHpSpa,
    [FIELD_MOVE_SWEET_SCENT + 13]    = gText_LureWildPokemonSpa,
    [FIELD_MOVE_DIVE + 13]           = gText_DiveUnderwaterSpa,
};

static const u32 sHeldItemGfx[] = INCBIN_U32("graphics/party_menu/hold_icons.4bpp");
static const u16 sHeldItemPalette[] = INCBIN_U16("graphics/party_menu/hold_icons.gbapal");

static const struct OamData sOamData_HeldItem =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_HeldItem[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HeldMail[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HeldBall[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HeldBerry[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_HeldItem[] =
{
    sSpriteAnim_HeldItem,
    sSpriteAnim_HeldMail,
	sSpriteAnim_HeldBall,
	sSpriteAnim_HeldBerry,
};

static const struct SpriteSheet sSpriteSheet_HeldItem =
{
    sHeldItemGfx, sizeof(sHeldItemGfx), 0xD750
};

static const struct SpritePalette sSpritePalette_HeldItem =
{
    sHeldItemPalette, 0xD750
};

static const struct SpriteTemplate sSpriteTemplate_HeldItem =
{
    0xD750,
    0xD750,
    &sOamData_HeldItem,
    sSpriteAnimTable_HeldItem,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy,
};

static const struct OamData sOamData_StatusCondition =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_StatusPoison[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusParalyzed[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusSleep[] =
{
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFrozen[] =
{
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusBurn[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusPokerus[] =
{
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFaint[] =
{
    ANIMCMD_FRAME(24, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusPoisonSpa[] =
{
    ANIMCMD_FRAME(28, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusParalyzedSpa[] =
{
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusSleepSpa[] =
{
    ANIMCMD_FRAME(36, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFrozenSpa[] =
{
    ANIMCMD_FRAME(40, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusBurnSpa[] =
{
    ANIMCMD_FRAME(44, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusPokerusSpa[] =
{
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFaintSpa[] =
{
    ANIMCMD_FRAME(52, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Blank[] =
{
    ANIMCMD_FRAME(56, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteTemplate_StatusCondition[] =
{
    sSpriteAnim_StatusPoison,
    sSpriteAnim_StatusParalyzed,
    sSpriteAnim_StatusSleep,
    sSpriteAnim_StatusFrozen,
    sSpriteAnim_StatusBurn,
    sSpriteAnim_StatusPokerus,
    sSpriteAnim_StatusFaint,
    sSpriteAnim_Blank,
    sSpriteAnim_StatusPoisonSpa,
    sSpriteAnim_StatusParalyzedSpa,
    sSpriteAnim_StatusSleepSpa,
    sSpriteAnim_StatusFrozenSpa,
    sSpriteAnim_StatusBurnSpa,
    sSpriteAnim_StatusPokerusSpa,
    sSpriteAnim_StatusFaintSpa,
    sSpriteAnim_Blank,
};

static const struct CompressedSpriteSheet sSpriteSheet_StatusIcons =
{
    gPartyMenuStatusGfx_Icons, 0x800, 1202
};

static const struct CompressedSpritePalette sSpritePalette_StatusIcons =
{
    gPartyMenuStatusPal_Icons, 1202
};

static const struct SpriteTemplate sSpriteTemplate_StatusIcons =
{
    .tileTag = 1202,
    .paletteTag = 1202,
    .oam = &sOamData_StatusCondition,
    .anims = sSpriteTemplate_StatusCondition,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const bool8 sMultiBattlePartnersPartyMask[PARTY_SIZE + 2] = 
{
    FALSE, 
    TRUE, 
    FALSE, 
    FALSE, 
    TRUE, 
    TRUE, 
    FALSE,
};

static const u16 sTMHMMoves_Duplicate[] =
{
    MOVE_FOCUS_PUNCH,
    MOVE_DRAGON_CLAW,
    MOVE_WATER_PULSE,
    MOVE_CALM_MIND,
    MOVE_ROAR,
    MOVE_TOXIC,
    MOVE_HAIL,
    MOVE_BULK_UP,
    MOVE_BULLET_SEED,
    MOVE_HIDDEN_POWER,
    MOVE_SUNNY_DAY,
    MOVE_TAUNT,
    MOVE_ICE_BEAM,
    MOVE_BLIZZARD,
    MOVE_HYPER_BEAM,
    MOVE_LIGHT_SCREEN,
    MOVE_PROTECT,
    MOVE_RAIN_DANCE,
    MOVE_GIGA_DRAIN,
    MOVE_SAFEGUARD,
    MOVE_FRUSTRATION,
    MOVE_SOLAR_BEAM,
    MOVE_IRON_TAIL,
    MOVE_THUNDERBOLT,
    MOVE_THUNDER,
    MOVE_EARTHQUAKE,
    MOVE_RETURN,
    MOVE_DIG,
    MOVE_PSYCHIC,
    MOVE_SHADOW_BALL,
    MOVE_BRICK_BREAK,
    MOVE_DOUBLE_TEAM,
    MOVE_REFLECT,
    MOVE_SHOCK_WAVE,
    MOVE_FLAMETHROWER,
    MOVE_SLUDGE_BOMB,
    MOVE_SANDSTORM,
    MOVE_FIRE_BLAST,
    MOVE_ROCK_TOMB,
    MOVE_AERIAL_ACE,
    MOVE_TORMENT,
    MOVE_FACADE,
    MOVE_SECRET_POWER,
    MOVE_REST,
    MOVE_ATTRACT,
    MOVE_THIEF,
    MOVE_STEEL_WING,
    MOVE_SKILL_SWAP,
    MOVE_SNATCH,
    MOVE_OVERHEAT,
    MOVE_CUT,
    MOVE_FLY,
    MOVE_SURF,
    MOVE_STRENGTH,
    MOVE_FLASH,
    MOVE_ROCK_SMASH,
    MOVE_WATERFALL,
    MOVE_DIVE,
};

enum
{
    MENU_SUMMARY,
    MENU_SWITCH,
    MENU_NICKNAME,
    MENU_CHANGE,
    MENU_CANCEL1,
    MENU_ITEM,
    MENU_GIVE,
    MENU_TAKE_ITEM,
    MENU_MOVE_ITEM,
    MENU_MAIL,
    MENU_TAKE_MAIL,
    MENU_READ,
    MENU_CANCEL2,
    MENU_SHIFT,
    MENU_SEND_OUT,
    MENU_ENTER,
    MENU_NO_ENTRY,
    MENU_STORE,
    MENU_REGISTER,
    MENU_TRADE1,
    MENU_TRADE2,
    MENU_FIELD_MOVES,
};

static struct
{
    const u8 *text;
    TaskFunc func;
} const sCursorOptions[] =
{
    [MENU_SUMMARY] = {gText_Summary5, CursorCB_Summary},
    [MENU_NICKNAME] = {gText_Nickname, CursorCB_Nickname},
    [MENU_CHANGE] = {gText_Shift, CursorCB_ChangeNickname},
    [MENU_SWITCH] = {gText_Switch2, CursorCB_Switch},
    [MENU_CANCEL1] = {gText_FameChecker_Cancel, CursorCB_Cancel1},
    [MENU_ITEM] = {gText_Item, CursorCB_Item},
    [MENU_GIVE] = {gText_Give, CursorCB_Give},
    [MENU_TAKE_ITEM] = {gText_Take, CursorCB_TakeItem},
    [MENU_MOVE_ITEM] = {gText_Move, CursorCB_MoveItem},
    [MENU_MAIL] = {gText_Mail, CursorCB_Mail},
    [MENU_TAKE_MAIL] = {gText_Take2, CursorCB_TakeMail},
    [MENU_READ] = {gText_Read2, CursorCB_Read},
    [MENU_CANCEL2] = {gText_FameChecker_Cancel, CursorCB_Cancel2},
    [MENU_SHIFT] = {gText_Shift, CursorCB_SendMon},
    [MENU_SEND_OUT] = {gText_SendOut, CursorCB_SendMon},
    [MENU_ENTER] = {gText_Enter, CursorCB_Enter},
    [MENU_NO_ENTRY] = {gText_NoEntry, CursorCB_NoEntry},
    [MENU_STORE] = {gText_Store, CursorCB_Store},
    [MENU_REGISTER] = {gText_Register, CursorCB_Register},
    [MENU_TRADE1] = {gText_Trade4, CursorCB_Trade1},
    [MENU_TRADE2] = {gText_Trade4, CursorCB_Trade2},
    [MENU_FIELD_MOVES + FIELD_MOVE_FLASH] = {gMoveNames[MOVE_FLASH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_CUT] = {gMoveNames[MOVE_CUT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_FLY] = {gMoveNames[MOVE_FLY], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_STRENGTH] = {gMoveNames[MOVE_STRENGTH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SURF] = {gMoveNames[MOVE_SURF], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_ROCK_SMASH] = {gMoveNames[MOVE_ROCK_SMASH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_WATERFALL] = {gMoveNames[MOVE_WATERFALL], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_TELEPORT] = {gMoveNames[MOVE_TELEPORT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_DIG] = {gMoveNames[MOVE_DIG], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_MILK_DRINK] = {gMoveNames[MOVE_MILK_DRINK], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SOFT_BOILED] = {gMoveNames[MOVE_SOFT_BOILED], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SWEET_SCENT] = {gMoveNames[MOVE_SWEET_SCENT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_DIVE] = {gMoveNames[MOVE_DIVE], CursorCB_FieldMove},
	[MENU_SUMMARY + 34] = {gText_Summary5Spa, CursorCB_Summary},
    [MENU_NICKNAME + 34] = {gText_NicknameSpa, CursorCB_Nickname},
    [MENU_CHANGE + 34] = {gText_ShiftSpa, CursorCB_ChangeNickname},
    [MENU_SWITCH + 34] = {gText_Switch2Spa, CursorCB_Switch},
    [MENU_CANCEL1 + 34] = {gText_SSCancelSpa, CursorCB_Cancel1},
    [MENU_ITEM + 34] = {gText_ItemSpa, CursorCB_Item},
    [MENU_GIVE + 34] = {gText_GiveSpa, CursorCB_Give},
    [MENU_TAKE_ITEM + 34] = {gText_TakeSpa, CursorCB_TakeItem},
    [MENU_MOVE_ITEM + 34] = {gText_MoveSpa, CursorCB_MoveItem},
    [MENU_MAIL + 34] = {gText_MailSpa, CursorCB_Mail},
    [MENU_TAKE_MAIL + 34] = {gText_Take2Spa, CursorCB_TakeMail},
    [MENU_READ + 34] = {gText_Read2Spa, CursorCB_Read},
    [MENU_CANCEL2 + 34] = {gText_SSCancelSpa, CursorCB_Cancel2},
    [MENU_SHIFT + 34] = {gText_ShiftSpa, CursorCB_SendMon},
    [MENU_SEND_OUT + 34] = {gText_SendOutSpa, CursorCB_SendMon},
    [MENU_ENTER + 34] = {gText_EnterSpa, CursorCB_Enter},
    [MENU_NO_ENTRY + 34] = {gText_NoEntrySpa, CursorCB_NoEntry},
    [MENU_STORE + 34] = {gText_StoreSpa, CursorCB_Store},
    [MENU_REGISTER + 34] = {gText_RegisterSpa, CursorCB_Register},
    [MENU_TRADE1 + 34] = {gText_Trade4Spa, CursorCB_Trade1},
    [MENU_TRADE2 + 34] = {gText_Trade4Spa, CursorCB_Trade2},
    [MENU_FIELD_MOVES + FIELD_MOVE_FLASH + 34] = {gMoveNamesSpa[MOVE_FLASH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_CUT + 34] = {gMoveNamesSpa[MOVE_CUT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_FLY + 34] = {gMoveNamesSpa[MOVE_FLY], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_STRENGTH + 34] = {gMoveNamesSpa[MOVE_STRENGTH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SURF + 34] = {gMoveNamesSpa[MOVE_SURF], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_ROCK_SMASH + 34] = {gMoveNamesSpa[MOVE_ROCK_SMASH], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_WATERFALL + 34] = {gMoveNamesSpa[MOVE_WATERFALL], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_TELEPORT + 34] = {gMoveNamesSpa[MOVE_TELEPORT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_DIG + 34] = {gMoveNamesSpa[MOVE_DIG], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_MILK_DRINK + 34] = {gMoveNamesSpa[MOVE_MILK_DRINK], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SOFT_BOILED + 34] = {gMoveNamesSpa[MOVE_SOFT_BOILED], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_SWEET_SCENT + 34] = {gMoveNamesSpa[MOVE_SWEET_SCENT], CursorCB_FieldMove},
    [MENU_FIELD_MOVES + FIELD_MOVE_DIVE + 34] = {gMoveNames[MOVE_DIVE], CursorCB_FieldMove},
};

static const u8 sPartyMenuAction_SummarySwitchCancel[] = {MENU_SUMMARY, MENU_SWITCH, MENU_CANCEL1};
static const u8 sPartyMenuAction_ShiftSummaryCancel[] = {MENU_SHIFT, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_SendOutSummaryCancel[] = {MENU_SEND_OUT, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_SummaryCancel[] = {MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_EnterSummaryCancel[] = {MENU_ENTER, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_NoEntrySummaryCancel[] = {MENU_NO_ENTRY, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_StoreSummaryCancel[] = {MENU_STORE, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_GiveTakeMoveItemCancel[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};
static const u8 sPartyMenuAction_ChangeCancel[] = {MENU_CHANGE, MENU_CANCEL2};
static const u8 sPartyMenuAction_ReadTakeMailCancel[] = {MENU_READ, MENU_TAKE_MAIL, MENU_CANCEL2};
static const u8 sPartyMenuAction_RegisterSummaryCancel[] = {MENU_REGISTER, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_TradeSummaryCancel1[] = {MENU_TRADE1, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_TradeSummaryCancel2[] = {MENU_TRADE2, MENU_SUMMARY, MENU_CANCEL1};

// IDs for the action lists that appear when a party mon is selected
enum
{
    ACTIONS_NONE,
    ACTIONS_SWITCH,
    ACTIONS_SHIFT,
    ACTIONS_SEND_OUT,
    ACTIONS_ENTER,
    ACTIONS_NO_ENTRY,
    ACTIONS_STORE,
    ACTIONS_SUMMARY_ONLY,
    ACTIONS_ITEM,
    ACTIONS_NICKNAME,
    ACTIONS_MAIL,
    ACTIONS_REGISTER,
    ACTIONS_TRADE,
    ACTIONS_SPIN_TRADE,
};

static const u8 *const sPartyMenuActions[] =
{
    [ACTIONS_NONE]          = NULL,
    [ACTIONS_SWITCH]        = sPartyMenuAction_SummarySwitchCancel,
    [ACTIONS_SHIFT]         = sPartyMenuAction_ShiftSummaryCancel,
    [ACTIONS_SEND_OUT]      = sPartyMenuAction_SendOutSummaryCancel,
    [ACTIONS_ENTER]         = sPartyMenuAction_EnterSummaryCancel,
    [ACTIONS_NO_ENTRY]      = sPartyMenuAction_NoEntrySummaryCancel,
    [ACTIONS_STORE]         = sPartyMenuAction_StoreSummaryCancel,
    [ACTIONS_SUMMARY_ONLY]  = sPartyMenuAction_SummaryCancel,
    [ACTIONS_ITEM]          = sPartyMenuAction_GiveTakeMoveItemCancel,
    [ACTIONS_NICKNAME]      = sPartyMenuAction_ChangeCancel,
    [ACTIONS_MAIL]          = sPartyMenuAction_ReadTakeMailCancel,
    [ACTIONS_REGISTER]      = sPartyMenuAction_RegisterSummaryCancel,
    [ACTIONS_TRADE]         = sPartyMenuAction_TradeSummaryCancel1,
    [ACTIONS_SPIN_TRADE]    = sPartyMenuAction_TradeSummaryCancel2,
};

static const u8 sPartyMenuActionCounts[] =
{
    [ACTIONS_NONE]          = 0,
    [ACTIONS_SWITCH]        = NELEMS(sPartyMenuAction_SummarySwitchCancel),
    [ACTIONS_SHIFT]         = NELEMS(sPartyMenuAction_ShiftSummaryCancel),
    [ACTIONS_SEND_OUT]      = NELEMS(sPartyMenuAction_SendOutSummaryCancel),
    [ACTIONS_ENTER]         = NELEMS(sPartyMenuAction_EnterSummaryCancel),
    [ACTIONS_NO_ENTRY]      = NELEMS(sPartyMenuAction_NoEntrySummaryCancel),
    [ACTIONS_STORE]         = NELEMS(sPartyMenuAction_StoreSummaryCancel),
    [ACTIONS_SUMMARY_ONLY]  = NELEMS(sPartyMenuAction_SummaryCancel),
    [ACTIONS_ITEM]          = NELEMS(sPartyMenuAction_GiveTakeMoveItemCancel),
    [ACTIONS_NICKNAME]      = NELEMS(sPartyMenuAction_ChangeCancel),
    [ACTIONS_MAIL]          = NELEMS(sPartyMenuAction_ReadTakeMailCancel),
    [ACTIONS_REGISTER]      = NELEMS(sPartyMenuAction_RegisterSummaryCancel),
    [ACTIONS_TRADE]         = NELEMS(sPartyMenuAction_TradeSummaryCancel1),
    [ACTIONS_SPIN_TRADE]    = NELEMS(sPartyMenuAction_TradeSummaryCancel2),
};

static const u16 sFieldMoves[] =
{
    MOVE_FLASH,
    MOVE_CUT, 
    MOVE_FLY, 
    MOVE_STRENGTH, 
    MOVE_SURF, 
    MOVE_ROCK_SMASH, 
    MOVE_WATERFALL, 
    MOVE_TELEPORT,
    MOVE_DIG, 
    MOVE_MILK_DRINK, 
    MOVE_SOFT_BOILED, 
    MOVE_SWEET_SCENT, 
    MOVE_DIVE, 
    FIELD_MOVE_END // this may be misuse of enum. same in emerald
};

static struct
{
    bool8 (*fieldMoveFunc)(void);
    u8 msgId;
} const sFieldMoveCursorCallbacks[] =
{
    [FIELD_MOVE_FLASH]        = {SetUpFieldMove_Flash,       PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_CUT]          = {SetUpFieldMove_Cut,         PARTY_MSG_NOTHING_TO_CUT},
    [FIELD_MOVE_FLY]          = {SetUpFieldMove_Fly,         PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_STRENGTH]     = {SetUpFieldMove_Strength,    PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_SURF]         = {SetUpFieldMove_Surf,        PARTY_MSG_CANT_SURF_HERE},
    [FIELD_MOVE_ROCK_SMASH]   = {SetUpFieldMove_RockSmash,   PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_WATERFALL]    = {SetUpFieldMove_Waterfall,   PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_TELEPORT]     = {SetUpFieldMove_Teleport,    PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_DIG]          = {SetUpFieldMove_Dig,         PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_MILK_DRINK]   = {SetUpFieldMove_SoftBoiled,  PARTY_MSG_NOT_ENOUGH_HP},
    [FIELD_MOVE_SOFT_BOILED]  = {SetUpFieldMove_SoftBoiled,  PARTY_MSG_NOT_ENOUGH_HP},
    [FIELD_MOVE_SWEET_SCENT]  = {SetUpFieldMove_SweetScent,  PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_DIVE]         = {SetUpFieldMove_Dive,        PARTY_MSG_CANT_USE_HERE},
};

static const u8 *const sUnionRoomTradeMessages[] =
{
    [UR_TRADE_MSG_NOT_MON_PARTNER_WANTS - 1]       = gText_NotPkmnOtherTrainerWants,
    [UR_TRADE_MSG_NOT_EGG - 1]                     = gText_ThatIsntAnEgg,
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_1 - 1]        = gText_PkmnCantBeTradedNow,
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_2 - 1]        = gText_PkmnCantBeTradedNow,
    [UR_TRADE_MSG_PARTNERS_MON_CANT_BE_TRADED - 1] = gText_OtherTrainersPkmnCantBeTraded,
    [UR_TRADE_MSG_EGG_CANT_BE_TRADED -1]           = gText_EggCantBeTradedNow,
    [UR_TRADE_MSG_PARTNER_CANT_ACCEPT_MON - 1]     = gText_OtherTrainerCantAcceptPkmn,
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_1 - 1]   = gText_CantTradeWithTrainer,
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_2 - 1]   = gText_CantTradeWithTrainer,
    [UR_TRADE_MSG_NOT_MON_PARTNER_WANTS - 1 + 9]       = gText_NotPkmnOtherTrainerWantsSpa,
    [UR_TRADE_MSG_NOT_EGG - 1 + 9]                     = gText_ThatIsntAnEggSpa,
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_1 - 1 + 9]        = gText_PkmnCantBeTradedNowSpa,
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_2 - 1 + 9]        = gText_PkmnCantBeTradedNowSpa,
    [UR_TRADE_MSG_PARTNERS_MON_CANT_BE_TRADED - 1 + 9] = gText_OtherTrainersPkmnCantBeTradedSpa,
    [UR_TRADE_MSG_EGG_CANT_BE_TRADED -1 + 9]           = gText_EggCantBeTradedNowSpa,
    [UR_TRADE_MSG_PARTNER_CANT_ACCEPT_MON - 1 + 9]     = gText_OtherTrainerCantAcceptPkmnSpa,
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_1 - 1 + 9]   = gText_CantTradeWithTrainerSpa,
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_2 - 1 + 9]   = gText_CantTradeWithTrainerSpa,
};

static const u16 sTMHMMoves[] =
{
    MOVE_FOCUS_PUNCH,
    MOVE_DRAGON_CLAW,
    MOVE_WATER_PULSE,
    MOVE_CALM_MIND,
    MOVE_ROAR,
    MOVE_TOXIC,
    MOVE_HAIL,
    MOVE_BULK_UP,
    MOVE_BULLET_SEED,
    MOVE_HIDDEN_POWER,
    MOVE_SUNNY_DAY,
    MOVE_TAUNT,
    MOVE_ICE_BEAM,
    MOVE_BLIZZARD,
    MOVE_HYPER_BEAM,
    MOVE_LIGHT_SCREEN,
    MOVE_PROTECT,
    MOVE_RAIN_DANCE,
    MOVE_GIGA_DRAIN,
    MOVE_SAFEGUARD,
    MOVE_FRUSTRATION,
    MOVE_SOLAR_BEAM,
    MOVE_IRON_TAIL,
    MOVE_THUNDERBOLT,
    MOVE_THUNDER,
    MOVE_EARTHQUAKE,
    MOVE_RETURN,
    MOVE_DIG,
    MOVE_PSYCHIC,
    MOVE_SHADOW_BALL,
    MOVE_BRICK_BREAK,
    MOVE_DOUBLE_TEAM,
    MOVE_REFLECT,
    MOVE_SHOCK_WAVE,
    MOVE_FLAMETHROWER,
    MOVE_SLUDGE_BOMB,
    MOVE_SANDSTORM,
    MOVE_FIRE_BLAST,
    MOVE_ROCK_TOMB,
    MOVE_AERIAL_ACE,
    MOVE_TORMENT,
    MOVE_FACADE,
    MOVE_SECRET_POWER,
    MOVE_REST,
    MOVE_ATTRACT,
    MOVE_THIEF,
    MOVE_STEEL_WING,
    MOVE_SKILL_SWAP,
    MOVE_SNATCH,
    MOVE_OVERHEAT,
    MOVE_CUT,
    MOVE_FLY,
    MOVE_SURF,
    MOVE_STRENGTH,
    MOVE_FLASH,
    MOVE_ROCK_SMASH,
    MOVE_WATERFALL,
    MOVE_DIVE,
};
