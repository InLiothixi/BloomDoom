#include "GameButton.h"
#include "../../LawnApp.h"
#include "../../GameConstants.h"
#include "../System/Music.h"
#include "ChallengeScreen.h"
#include "../../Resources.h"
#include "../ToolTipWidget.h"
#include "../System/PlayerInfo.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Debug.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/WidgetManager.h"

ChallengeDefinition gChallengeDefs[NUM_CHALLENGE_MODES] = {
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_1,              0,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  0,  _S("[SURVIVAL_DAY_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_2,              1,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  1,  _S("[SURVIVAL_NIGHT_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_3,              2,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  2,  _S("[SURVIVAL_POOL_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_4,              3,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  3,  _S("[SURVIVAL_FOG_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_5,              4,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  4,  _S("[SURVIVAL_ROOF_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_6,              4,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    0,  5,  _S("[SURVIVAL_GLOOM_NORMAL]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_1,                5,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  0,  _S("[SURVIVAL_DAY_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_2,                6,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  1,  _S("[SURVIVAL_NIGHT_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_3,                7,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  2,  _S("[SURVIVAL_POOL_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_4,                8,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  3,  _S("[SURVIVAL_FOG_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_5,                9,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  4,  _S("[SURVIVAL_ROOF_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_6,                9,   ChallengePage::CHALLENGE_PAGE_SURVIVAL,    1,  5,  _S("[SURVIVAL_GLOOM_HARD]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_1,             11,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,	   2,  0,  _S("[SURVIVAL_DAY_ENDLESS]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_2,             12,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,    2,  1,  _S("[SURVIVAL_NIGHT_ENDLESS]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_3,             10,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,    2,  2,  _S("[SURVIVAL_POOL_ENDLESS]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_4,             13,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,    2,  3,  _S("[SURVIVAL_FOG_ENDLESS]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_5,             14,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,    2,  4,  _S("[SURVIVAL_ROOF_ENDLESS]") },
	{ GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_6,             14,  ChallengePage::CHALLENGE_PAGE_SURVIVAL,    2,  5,  _S("[SURVIVAL_GLOOM_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_1,         0,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  0,  _S("[COOP_DAY_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_2,         1,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  1,  _S("[COOP_NIGHT_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_3,         2,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  2,  _S("[COOP_POOL_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_4,		   3,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  3,  _S("[COOP_FOG_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_5,		   4,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  4,  _S("[COOP_ROOF_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_NORMAL_STAGE_6,		   4,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        0,  5,  _S("[COOP_GLOOM_NORMAL]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_1,		   5,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  0,  _S("[COOP_DAY_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_2,		   6,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  1,  _S("[COOP_NIGHT_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_3,		   7,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  2,  _S("[COOP_POOL_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_4,           8,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  3,  _S("[COOP_FOG_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_5,           9,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  4,  _S("[COOP_ROOF_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_HARD_STAGE_6,           9,   ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        1,  5,  _S("[COOP_GLOOM_HARD]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_1,        11,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  0,  _S("[COOP_DAY_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_2,        12,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  1,  _S("[COOP_NIGHT_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_3,        10,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  2,  _S("[COOP_POOL_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_4,        13,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  3,  _S("[COOP_FOG_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_5,        14,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  4,  _S("[COOP_ROOF_ENDLESS]") },
	{ GameMode::GAMEMODE_COOP_SURVIVAL_ENDLESS_STAGE_6,        14,  ChallengePage::CHALLENGE_PAGE_COOP_SURVIVAL,        2,  5,  _S("[COOP_GLOOM_ENDLESS]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY,				   0,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   0,  0,  _S("[WAR_AND_PEAS]") },
	{ GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING,            6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   0,  1,  _S("[WALL_NUT_BOWLING]") },
	{ GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE,               2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   0,  2,  _S("[SLOT_MACHINE]") },
	{ GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS,              3,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   0,  3,  _S("[ITS_RAINING_SEEDS]") },
	{ GameMode::GAMEMODE_CHALLENGE_BEGHOULED,                  1,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   0,  4,  _S("[BEGHOULED]") },
	{ GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL,                8,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   1,  0,  _S("[INVISIGHOUL]") },
	{ GameMode::GAMEMODE_CHALLENGE_SEEING_STARS,               5,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   1,  1,  _S("[SEEING_STARS]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM,               7,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   1,  2,  _S("[ZOMBIQUARIUM]") },
	{ GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST,            20,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   1,  3,  _S("[BEGHOULED_TWIST]") },
	{ GameMode::GAMEMODE_CHALLENGE_LITTLE_TROUBLE,             12,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   1,  4,  _S("[LITTLE_TROUBLE]") },
	{ GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT,              15,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   2,  0,  _S("[PORTAL_COMBAT]") },
	{ GameMode::GAMEMODE_CHALLENGE_COLUMN,                     4,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   2,  1,  _S("[COLUMN_AS_YOU_SEE_EM]") },
	{ GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA,            17,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   2,  2,  _S("[BOBSLED_BONANZA]") },
	{ GameMode::GAMEMODE_CHALLENGE_SPEED,                      18,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   2,  3,  _S("[ZOMBIES_ON_SPEED]") },
	{ GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE,             16,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   2,  4,  _S("[WHACK_A_ZOMBIE]") },
	{ GameMode::GAMEMODE_CHALLENGE_SURPRISE_ATTACK,			   18,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   3,  0,  _S("[SURPRISE_ATTACK]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2,				   19,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   3,  1,  _S("[WAR_AND_PEAS_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2,          20,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   3,  2,  _S("[WALL_NUT_BOWLING_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_POGO_PARTY,                 14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   3,  3,  _S("[POGO_PARTY]") },
	{ GameMode::GAMEMODE_CHALLENGE_DR_ZOMBOSS_REVENGE,         19,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   3,  4,  _S("[FINAL_BOSS]") },
	{ GameMode::GAMEMODE_CHALLENGE_SQUIRREL,                   0,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   4,  0,  _S("[SQUIRREL]") },
	{ GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS_2,            1,	ChallengePage::CHALLENGE_PAGE_CHALLENGE,   4,  1,  _S("[ITS_RAINING_SEEDS_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_BIG_TROUBLE,                2,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   4,  2,  _S("[GIANT_TROUBLE]") },
	{ GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY,               3,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   4,  3,  _S("[HIGH_GRAVITY]") },
	{ GameMode::GAMEMODE_CHALLENGE_BIG_TIME,                   4,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   4,  4,  _S("[BIG_TIME]") },
	{ GameMode::GAMEMODE_CHALLENGE_BUNGEE_BLITZ,               9,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   5,  0,  _S("[BUNGEE_BLITZ]") },
	{ GameMode::GAMEMODE_CHALLENGE_SEEING_FLOWERS,			   5,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   5,  1,  _S("[ART_CHALLENGE_SUNFLOWERS]") },
	{ GameMode::GAMEMODE_CHALLENGE_UNSODDED,                   6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   5,  2,  _S("[UNSODDED]") },
	{ GameMode::GAMEMODE_CHALLENGE_DARK_STORMY_NIGHT,          13,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   5,  3,  _S("[DARK_STORMY_NIGHT]") },
	{ GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2,            7,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   5,  4,  _S("[PORTAL_COMBAT_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_HEAT_WAVE,				   8,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   6,  0,  _S("[HEAT_WAVE]") },
	{ GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY,                  9,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   6,  1,  _S("[SUNNY_DAY]") },
	{ GameMode::GAMEMODE_CHALLENGE_CAN_YOU_DIG_IT,             10,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   6,  2,  _S("[CAN_YOU_DIG_IT]") },
	{ GameMode::GAMEMODE_CHALLENGE_DANCE_PARTY,                11,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   6,  3,  _S("[DANCE_PARTY]") },
	{ GameMode::GAMEMODE_CHALLENGE_AIR_RAID,                   12,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   6,  4,  _S("[AIR_RAID]") },
	{ GameMode::GAMEMODE_CHALLENGE_BLIND_FAITH,                13,	ChallengePage::CHALLENGE_PAGE_CHALLENGE,   7,  0,  _S("[BLIND_FAITH]") },
	{ GameMode::GAMEMODE_CHALLENGE_GRAVE_DANGER,               14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   7,  1,  _S("[GRAVE_DANGER]") },
	{ GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL,				   15,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   7,  2,  _S("[ICE_LEVEL]") },
	{ GameMode::GAMEMODE_CHALLENGE_ART_SEEING_NUTS,			   16,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   7,  3,  _S("[ART_CHALLENGE_WALLNUT]") },
	{ GameMode::GAMEMODE_CHALLENGE_METAL_FESTIVAL,             17,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   7,  4,  _S("[METAL_FESTIVAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN,                 7,   ChallengePage::CHALLENGE_PAGE_LIMBO,       1,  2,  _S("[ZEN_GARDEN]") },
	{ GameMode::GAMEMODE_TREE_OF_WISDOM,                       10,  ChallengePage::CHALLENGE_PAGE_LIMBO,       2,  4,  _S("[TREE_OF_WISDOM]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_1,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      0,  0,  _S("[SCARY_POTTER_1]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_2,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      0,  1,  _S("[SCARY_POTTER_2]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_3,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      0,  2,  _S("[SCARY_POTTER_3]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_4,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      0,  3,  _S("[SCARY_POTTER_4]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_5,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      0,  4,  _S("[SCARY_POTTER_5]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_6,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      1,  0,  _S("[SCARY_POTTER_6]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_7,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      1,  1,  _S("[SCARY_POTTER_7]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_8,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      1,  2,  _S("[SCARY_POTTER_8]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_9,                       10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      1,  3,  _S("[SCARY_POTTER_9]") },
	{ GameMode::GAMEMODE_SCARY_POTTER_ENDLESS,                 10,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      1,  4,  _S("[SCARY_POTTER_ENDLESS]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      2,  0,  _S("[I_ZOMBIE_1]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      2,  1,  _S("[I_ZOMBIE_2]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      2,  2,  _S("[I_ZOMBIE_3]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      2,  3,  _S("[I_ZOMBIE_4]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      2,  4,  _S("[I_ZOMBIE_5]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      3,  0,  _S("[I_ZOMBIE_6]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      3,  1,  _S("[I_ZOMBIE_7]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      3,  2,  _S("[I_ZOMBIE_8]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9,                    11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      3,  3,  _S("[I_ZOMBIE_9]") },
	{ GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS,              11,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      3,  4,  _S("[I_ZOMBIE_ENDLESS]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_DAY,             21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,	   4,  0,  _S("[LAST_STAND_DAY_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_NIGHT,           21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,	   4,  1,  _S("[LAST_STAND_NIGHT_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_POOL,            21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,	   4,  2,  _S("[LAST_STAND_POOL_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_FOG,             21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      4,  3,  _S("[LAST_STAND_FOG_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_ROOF,            21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      4,  4,  _S("[LAST_STAND_ROOF_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_LAST_STAND_GLOOM,           21,  ChallengePage::CHALLENGE_PAGE_PUZZLE,      4,  5,  _S("[LAST_STAND_GLOOM_NORMAL]") },
	{ GameMode::GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES,          11,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   8,  1,  _S("[ECLIPSE]") },
	{ GameMode::GAMEMODE_CHALLENGE_HARD_HEADED,				   14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   8,  2,  _S("[HARD_HEADED]") },
	{ GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_3,          6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   8,  3,  _S("[WALL_NUT_BOWLING_3]") },
	{ GameMode::GAMEMODE_CHALLENGE_BLOOD_MOON,				   18,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   8,  4,  _S("[BLOOD_MOON]") },
	{ GameMode::GAMEMODE_CHALLENGE_WE_SMASH,				   18,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   9,  0,  _S("[WE_SMASH]") },
	{ GameMode::GAMEMODE_CHALLENGE_POOL_PARTY,               3,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   9,  1,  _S("[POOL_PARTY]") },
	{ GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE_2,             2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   9,  2,  _S("[SLOT_MACHINE_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN,           4,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   9,  3,  _S("[BUTTERED_POPCORN]") },
	{ GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE,                  7,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   9,  4,  _S("[OPEN_FIRE]") },
	{ GameMode::GAMEMODE_CHALLENGE_UNSODDED_2,				   14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   10, 0,  _S("[UNSODDED_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_BORDER_TREATY,              2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   10, 1,  _S("[BORDER_TREATY]") },
	{ GameMode::GAMEMODE_CHALLENGE_FLOOR_IS_LAVA,              6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   10, 2,  _S("[THE_FLOOR_IS_LAVA]") },
	{ GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON,			   9,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   10, 3,  _S("[WEIRDMAGEDDON]") },
	{ GameMode::GAMEMODE_CHALLENGE_STORMY_NIGHT_2,			   13,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   10, 4,  _S("[DARK_STORMY_NIGHT_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_RENEWABLE_ENERGY,		   1,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   11, 0,  _S("[RENEWABLE_ENERGY]") },
	{ GameMode::GAMEMODE_CHALLENGE_NONE_SHALL_PASS,			   12,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   11, 1,  _S("[NONE_SHALL_PASS]") },
	{ GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2,		   16,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   11, 2,  _S("[WHACK_A_ZOMBIE_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_LIGHTS_OUT,				   13,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   11, 3,  _S("[LIGHTS_OUT]") },
	{ GameMode::GAMEMODE_CHALLENGE_WAR_AND_PEAS_3,		       15,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   11, 4,  _S("[WAR_AND_PEAS_3]") },
	{ GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER,		   19,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   12, 0,  _S("[GRAVEYARD_ENCOUNTER]") },
	{ GameMode::GAMEMODE_CHALLENGE_SPEEDY_LANES,			   18,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   12, 1,  _S("[SPEEDY_LANES]") },
	{ GameMode::GAMEMODE_CHALLENGE_BEJEWELED_2,				   3,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   12, 2,  _S("[BEJEWELED_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_PLANT_MINIMALIST,		   8,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   12, 3,  _S("[PLANT_MINIMALIST]") },
	{ GameMode::GAMEMODE_CHALLENGE_EVERYONE_IS_DEAD,		   14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   12, 4,  _S("[EVERYONE_IS_DEAD]") },
	{ GameMode::GAMEMODE_CHALLENGE_BRUTAL_EVENING,			   4,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,  8,  0,  _S("[BRUTAL_EVENING]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZOMBIE_TRAP,				   17,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   13, 0,  _S("[ZOMBIE_TRAP]") },
	{ GameMode::GAMEMODE_CHALLENGE_HAIL,			           6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   13, 1,  _S("[HAILSTORM]") },
	{ GameMode::GAMEMODE_CHALLENGE_WINDNESDAY,			       9,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   13, 2,  _S("[WINDNESDAY]") },
	{ GameMode::GAMEMODE_CHALLENGE_HIGHGROUND,				   10,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   13, 3,  _S("[HIGHGROUND]") },
	{ GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON,			   8,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   13, 4,  _S("[HEAVY_WEAPON]") },
	{ GameMode::GAMEMODE_CHALLENGE_HOMERUN_DERBY,			   4,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   14, 0,  _S("[HOMERUN_DERBY]") },
	{ GameMode::GAMEMODE_CHALLENGE_HEAVY_RAIN,			       14,  ChallengePage::CHALLENGE_PAGE_CHALLENGE,   14, 1,  _S("[HEAVY_RAINSTORM]") },
	{ GameMode::GAMEMODE_CHALLENGE_BUNGEE_BLITZ_2,			   3,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   14, 2,  _S("[BUNGEE_BLITZ_2]") },
	{ GameMode::GAMEMODE_CHALLENGE_BOMB_ALL,			       2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   14, 3,  _S("[BOMB_ALL_TOGETHER]") },
	{ GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL_REVERSE,	       6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   14, 4,  _S("[INVISIGHOUL_REVERSE]") },
	{ GameMode::GAMEMODE_CHALLENGE_ZOMBIEZOMBIE,			   6,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   15, 0,  _S("[ZOMBIES_VS_ZOMBIES]") },
	{ GameMode::GAMEMODE_CHALLENGE_INFLATION,		           2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   15, 1,  _S("[INFLATION]") },
	{ GameMode::GAMEMODE_CHALLENGE_WORLDS_COLLIDING,		   2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   15, 2,  _S("[WORLDS_COLLIDING]") },
	{ GameMode::GAMEMODE_CHALLENGE_PLANT_FUSION,			   2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   15, 3,  _S("[PLANT_FUSION]") },
	{ GameMode::GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT,		   2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   15, 4,  _S("[DARK_FOGGY_NIGHT]") },
	{ GameMode::GAMEMODE_CHALLENGE_SLOWMO,					   2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   16, 0,  _S("[ZOMBIES_ON_SLOWMO]")},
	{ GameMode::GAMEMODE_CHALLENGE_FOREST,					   2,   ChallengePage::CHALLENGE_PAGE_CHALLENGE,   16, 1,  _S("[FOREST]") },
	{ GameMode::GAMEMODE_VERSUS_PLANTS_VS_PLANTS,		       1,   ChallengePage::CHALLENGE_PAGE_VERSUS,      0,  0,  _S("[PLANTS_VERSUS]") },
	{ GameMode::GAMEMODE_UPSELL,                               10,  ChallengePage::CHALLENGE_PAGE_LIMBO,       3,  4,  _S("[UPSELL]") },
	{ GameMode::GAMEMODE_INTRO,                                10,  ChallengePage::CHALLENGE_PAGE_LIMBO,       3,  0,  _S("[INTRO]") }
};

Rect challengeClipRect = Rect(250, 152, 778, 475);
//0x42DAE0
ChallengeScreen::ChallengeScreen(LawnApp* theApp, ChallengePage thePage)
{
	mLockShakeX = 0;
	mLockShakeY = 0;

	mScrollAmount = 0;
	mScrollPosition = 0;
	mMaxScrollPosition = 0;

	mPageIndex = thePage;
	mApp = theApp;
	mClip = false;
	mCheatEnableChallenges = false;
	mUnlockState = UNLOCK_OFF;
	mUnlockChallengeIndex = -1;
	mUnlockStateCounter = 0;
	TodLoadResources("DelayLoad_ChallengeScreen");

	mBackButton = MakeNewButton(ChallengeScreen::ChallengeScreen_Back, this, _S("[BACK_TO_MENU]"), nullptr, Sexy::IMAGE_SEEDCHOOSER_BUTTON2, 
		Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW, Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW);
	mBackButton->mTextDownOffsetX = 1;
	mBackButton->mTextDownOffsetY = 1;
	mBackButton->mColors[ButtonWidget::COLOR_LABEL] = Color(42, 42, 90);
	mBackButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(42, 42, 90);
	mBackButton->Resize(18 + BOARD_ADDITIONAL_WIDTH, 568 + BOARD_OFFSET_Y, 111, 26);

	if (thePage == CHALLENGE_PAGE_CHALLENGE)
		mMaxScrollPosition = 119 * 4 * 4;
	else if (thePage == CHALLENGE_PAGE_PUZZLE)
		mMaxScrollPosition = 119 * 4 * 2;
	else
		mMaxScrollPosition = 0;

	for (int aPageIdx = CHALLENGE_PAGE_SURVIVAL; aPageIdx < MAX_CHALLANGE_PAGES; aPageIdx++)
	{
		ButtonWidget* aPageButton = new ButtonWidget(ChallengeScreen::ChallengeScreen_Page + aPageIdx, this);
		aPageButton->mDoFinger = true;
		mPageButton[aPageIdx] = aPageButton;
		aPageButton->mButtonImage = Sexy::IMAGE_BLANK;
		aPageButton->mOverImage = Sexy::IMAGE_BLANK;
		aPageButton->mDownImage = Sexy::IMAGE_BLANK;
		aPageButton->SetFont(Sexy::FONT_BRIANNETOD12);
		aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
		aPageButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(220, 220, 0);
		bool hideButton = aPageIdx == CHALLENGE_PAGE_LIMBO || aPageIdx == CHALLENGE_PAGE_SURVIVAL || aPageIdx == CHALLENGE_PAGE_PUZZLE;

		if (aPageIdx == CHALLENGE_PAGE_VERSUS)
		{
			aPageButton->mLabel = TodStringTranslate(_S("[VERSUS]"));
			aPageButton->Resize(mBackButton->mX + mBackButton->mButtonImage->GetWidth() + 15 + 75 + 75, 540 + BOARD_OFFSET_Y, 100, 75);
		}
		else if (aPageIdx == CHALLENGE_PAGE_COOP_SURVIVAL)
		{
			aPageButton->mLabel = TodStringTranslate(_S("[COOP]"));
			aPageButton->Resize(mBackButton->mX + mBackButton->mButtonImage->GetWidth() + 10 + 75, 540 + BOARD_OFFSET_Y, 100, 75);
		}
		else if (aPageIdx == CHALLENGE_PAGE_CHALLENGE)
		{
			aPageButton->mLabel = TodStringTranslate(_S("[CLASSIC]"));
			aPageButton->Resize(mBackButton->mX + mBackButton->mButtonImage->GetWidth() + 5, 540 + BOARD_OFFSET_Y, 100, 75);
		}


		if (hideButton || mPageIndex != CHALLENGE_PAGE_CHALLENGE)
			aPageButton->mVisible = false;
	}
	
	for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++)
	{
		ChallengeDefinition& aChlDef = GetChallengeDefinition(aChallengeMode);
		ButtonWidget* aChallengeButton = new ButtonWidget(ChallengeScreen::ChallengeScreen_Mode + aChallengeMode, this);
		mChallengeButtons[aChallengeMode] = aChallengeButton;
		aChallengeButton->mDoFinger = true;
		aChallengeButton->mFrameNoDraw = true;
		if (aChlDef.mPage == CHALLENGE_PAGE_CHALLENGE || aChlDef.mPage == CHALLENGE_PAGE_LIMBO || aChlDef.mPage == CHALLENGE_PAGE_PUZZLE)
			aChallengeButton->Resize(38 + aChlDef.mCol * 155 + BOARD_ADDITIONAL_WIDTH, 93 + aChlDef.mRow * 119 + BOARD_OFFSET_Y, 104, 115);
		else
			aChallengeButton->Resize(38 + aChlDef.mCol * 124 + BOARD_ADDITIONAL_WIDTH, 125 + aChlDef.mRow * 145 + BOARD_OFFSET_Y, 104, 115);

		if (MoreTrophiesNeeded(aChallengeMode) && aChlDef.mPage != CHALLENGE_PAGE_VERSUS && aChlDef.mPage != CHALLENGE_PAGE_COOP_SURVIVAL)
		{
			aChallengeButton->mDoFinger = false;
			aChallengeButton->mDisabled = true;
		}
	}

	mToolTip = new ToolTipWidget();
	mToolTip->mCenter = true;
	mToolTip->mVisible = false;
	UpdateButtons();
	
	if (mApp->HasFinishedAdventure() && mApp->TrophiesNeedForGoldSunflower() <= 0 && mApp->mGameMode != GAMEMODE_UPSELL && mApp->mGameScene != SCENE_LEVEL_INTRO)
	{
		mApp->mMusic->MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FLORAL_HEAVEN);
	}
	else if (mApp->mGameMode != GAMEMODE_UPSELL || mApp->mGameScene != SCENE_LEVEL_INTRO)
	{
		mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_CHOOSE_YOUR_SEEDS);
	}

	bool aIsIZombie = false;
	if (mPageIndex == CHALLENGE_PAGE_SURVIVAL && mApp->mPlayerInfo->mHasNewSurvival)
	{
		SetUnlockChallengeIndex(mPageIndex, false);
		mApp->mPlayerInfo->mHasNewSurvival = false;
	}
	else if (mPageIndex == CHALLENGE_PAGE_CHALLENGE && mApp->mPlayerInfo->mHasNewMiniGame)
	{
		SetUnlockChallengeIndex(mPageIndex, false);
		mApp->mPlayerInfo->mHasNewMiniGame = false;
	}
	else if (mPageIndex == CHALLENGE_PAGE_PUZZLE)
	{
		if (mApp->mPlayerInfo->mHasNewScaryPotter)
		{
			SetUnlockChallengeIndex(mPageIndex, false);
			mApp->mPlayerInfo->mHasNewScaryPotter = false;
		}
		else if (mApp->mPlayerInfo->mHasNewIZombie)
		{
			SetUnlockChallengeIndex(mPageIndex, true);
			mApp->mPlayerInfo->mHasNewIZombie = false;
		}
	}

	Color aBtnColor = Color(42, 42, 90);
	Image* aBtnImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON2;
	Image* aOverImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW;
	int aImageWidth = aBtnImage->GetWidth();
	int aImageHeight = aOverImage->GetHeight();

	mQUICKPLAY = new GameButton(0);
	mQUICKPLAY->SetLabel(_S("Quick Play"));
	mQUICKPLAY->mButtonImage = aBtnImage;
	mQUICKPLAY->mOverImage = aOverImage;
	mQUICKPLAY->mDownImage = nullptr;
	mQUICKPLAY->SetFont(Sexy::FONT_BRIANNETOD12);
	mQUICKPLAY->mColors[0] = aBtnColor;
	mQUICKPLAY->mColors[1] = aBtnColor;
	mQUICKPLAY->mParentWidget = this;
	mQUICKPLAY->mTextOffsetY = 1;
	mQUICKPLAY->mDisabled = mQUICKPLAY->mBtnNoDraw = true;

	mpageDOWN = new GameButton(0);
	mpageDOWN->SetLabel(_S("Page Down"));
	mpageDOWN->mButtonImage = aBtnImage;
	mpageDOWN->mOverImage = aOverImage;
	mpageDOWN->mDownImage = nullptr;
	mpageDOWN->SetFont(Sexy::FONT_BRIANNETOD12);
	mpageDOWN->mColors[0] = aBtnColor;
	mpageDOWN->mColors[1] = aBtnColor;
	mpageDOWN->Resize(1280 - 10 - BOARD_ADDITIONAL_WIDTH - aImageWidth, 568 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mpageDOWN->mParentWidget = this;
	mpageDOWN->mTextOffsetY = 1;

	mpageUP = new GameButton(0);
	mpageUP->SetLabel(_S("Page Up"));
	mpageUP->mButtonImage = aBtnImage;
	mpageUP->mOverImage = aOverImage;
	mpageUP->mDownImage = nullptr;
	mpageUP->SetFont(Sexy::FONT_BRIANNETOD12);
	mpageUP->mColors[0] = aBtnColor;
	mpageUP->mColors[1] = aBtnColor;
	mpageUP->Resize(mpageDOWN->mX - aImageWidth - 10, 568 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mpageUP->mParentWidget = this;
	mpageUP->mTextOffsetY = 1;

	mQUICKPLAY->Resize((mBackButton->mX + aImageWidth * 2 + 10 + mpageUP->mX - 100) / 2 - aImageWidth / 2, 568 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);

	mSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT4, IMAGE_OPTIONS_SLIDERKNOB3, 0 ,this);
	mSlider->SetValue(max(0.0, min(mMaxScrollPosition, mScrollPosition)));
	mSlider->mHorizontal = false;
	mSlider->Resize(778 + BOARD_ADDITIONAL_WIDTH + 10, 152, 40, 475);
	mSlider->mThumbOffsetX -= 14;
	mSlider->mVisible = mMaxScrollPosition > 0;

	int maxPage = floor(mMaxScrollPosition / (119 * 4)) + 1;
	mpageDOWN->mBtnNoDraw = mpageUP->mBtnNoDraw = maxPage == 1;
	mSlider->mDisabled = maxPage == 1;
	mSlider->mVisible = maxPage > 1;
	if (maxPage == 1)
	{
		mQUICKPLAY->mX = BOARD_WIDTH / 2 - aImageWidth / 2;
	}

	if (mPageIndex == CHALLENGE_PAGE_CHALLENGE)
		mApp->UpdateDiscordRPC("Mini-Games", "Selecting Minigame");
	else if (mPageIndex == CHALLENGE_PAGE_PUZZLE)
		mApp->UpdateDiscordRPC("Puzzles", "Selecting Puzzle");
	else if (mPageIndex == CHALLENGE_PAGE_SURVIVAL)
		mApp->UpdateDiscordRPC("Survival", "Selecting Survival");
	else if (mPageIndex == CHALLENGE_PAGE_COOP_SURVIVAL)
		mApp->UpdateDiscordRPC("Co-op", "Selecting Co-op");
	else if (mPageIndex == CHALLENGE_PAGE_VERSUS)
		mApp->UpdateDiscordRPC("Versus", "Selecting Versus");
	else if (mPageIndex == CHALLENGE_PAGE_LIMBO)
		mApp->UpdateDiscordRPC("Limbo", "How did you get here?");
}

void ChallengeScreen::SliderVal(int theId, double theVal)
{
	switch (theId)
	{
		case 0:
			mScrollPosition = theVal * mMaxScrollPosition;
			break;
	}
}


//0x42E280 & 0x42E2A0
ChallengeScreen::~ChallengeScreen()
{
	delete mBackButton;
	for (ButtonWidget* aPageButton : mPageButton) delete aPageButton;
	for (ButtonWidget* aChallengeButton : mChallengeButtons) delete aChallengeButton;
	if (mQUICKPLAY) delete mQUICKPLAY;
	if (mpageDOWN) delete mpageDOWN;
	if (mpageUP) delete mpageUP;
	delete mSlider;
	delete mToolTip;
}

ChallengeDefinition& GetChallengeDefinition(int theChallengeMode)
{
	TOD_ASSERT(theChallengeMode >= 0 && theChallengeMode < NUM_CHALLENGE_MODES);

	ChallengeDefinition& aDef = gChallengeDefs[theChallengeMode];
	TOD_ASSERT(aDef.mChallengeMode == theChallengeMode + GAMEMODE_SURVIVAL_NORMAL_STAGE_1);

	return gChallengeDefs[theChallengeMode];
}

bool ChallengeScreen::IsScaryPotterLevel(GameMode theGameMode)
{
	return theGameMode >= GAMEMODE_SCARY_POTTER_1 && theGameMode <= GAMEMODE_SCARY_POTTER_ENDLESS;
}

bool ChallengeScreen::IsIZombieLevel(GameMode theGameMode)
{
	return theGameMode >= GAMEMODE_PUZZLE_I_ZOMBIE_1 && theGameMode <= GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS;
}

//0x42E3C0
void ChallengeScreen::SetUnlockChallengeIndex(ChallengePage thePage, bool theIsIZombie)
{
	mUnlockState = UNLOCK_SHAKING;
	mUnlockStateCounter = 100;
	mUnlockChallengeIndex = 0;
	for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++)
	{
		ChallengeDefinition& aDef = GetChallengeDefinition(aChallengeMode);
		if (aDef.mPage == thePage)
		{
			if (thePage != CHALLENGE_PAGE_PUZZLE || (!theIsIZombie && IsScaryPotterLevel(aDef.mChallengeMode)) || (theIsIZombie && IsIZombieLevel(aDef.mChallengeMode)))
			{
				if (AccomplishmentsNeeded(aChallengeMode) <= 0)
				{
					mUnlockChallengeIndex = aChallengeMode;
				}
			}
		}
	}
}

//0x42E440
int ChallengeScreen::MoreTrophiesNeeded(int theChallengeIndex)
{
	ChallengeDefinition& aDef = GetChallengeDefinition(theChallengeIndex);
	if (mApp->mGameMode == GAMEMODE_UPSELL && mApp->mGameScene == SCENE_LEVEL_INTRO)
	{
		return aDef.mChallengeMode == GAMEMODE_CHALLENGE_DR_ZOMBOSS_REVENGE ? 1 : 0;
	}
	
	if (mApp->IsTrialStageLocked())
	{
		if (mPageIndex == CHALLENGE_PAGE_PUZZLE && aDef.mChallengeMode >= GAMEMODE_SCARY_POTTER_4 && aDef.mChallengeMode <= GAMEMODE_SCARY_POTTER_ENDLESS)
		{
			return aDef.mChallengeMode == GAMEMODE_SCARY_POTTER_4 ? 1 : 2;
		}
		else if (mPageIndex == CHALLENGE_PAGE_PUZZLE && aDef.mChallengeMode >= GAMEMODE_CHALLENGE_LAST_STAND_NIGHT && aDef.mChallengeMode <= GAMEMODE_CHALLENGE_LAST_STAND_GLOOM)
		{
			return aDef.mChallengeMode == GAMEMODE_CHALLENGE_LAST_STAND_NIGHT ? 1 : 2;
		}
		else if (mPageIndex == CHALLENGE_PAGE_CHALLENGE && aDef.mChallengeMode >= GAMEMODE_CHALLENGE_RAINING_SEEDS)
		{
			return aDef.mChallengeMode == GAMEMODE_CHALLENGE_RAINING_SEEDS ? 1 : 2;
		}
		else if (mPageIndex == CHALLENGE_PAGE_SURVIVAL && aDef.mChallengeMode >= GAMEMODE_SURVIVAL_NORMAL_STAGE_6)
		{
			return aDef.mChallengeMode == GAMEMODE_SURVIVAL_NORMAL_STAGE_6 ? 1 : 2;
		}
		
	}

	if (aDef.mPage == CHALLENGE_PAGE_PUZZLE)
	{
		if (IsScaryPotterLevel(aDef.mChallengeMode))
		{
			int aLevelsCompleted = 0;
			for (ChallengeDefinition& aSPDef : gChallengeDefs)
			{
				if (IsScaryPotterLevel(aSPDef.mChallengeMode) && mApp->HasBeatenChallenge(aSPDef.mChallengeMode))
				{
					aLevelsCompleted++;
				}
			}

			if (aDef.mChallengeMode < GAMEMODE_SCARY_POTTER_4 || mApp->HasFinishedAdventure() || aLevelsCompleted < 3)
			{
				return ClampInt(aDef.mChallengeMode - GAMEMODE_SCARY_POTTER_1 - aLevelsCompleted, 0, 9);
			}
			else
			{
				return aDef.mChallengeMode == GAMEMODE_SCARY_POTTER_4 ? 1 : 2;
			}
		}
		else if (IsIZombieLevel(aDef.mChallengeMode))
		{
			int aLevelsCompleted = 0;
			for (ChallengeDefinition& aIZDef : gChallengeDefs)
			{
				if (IsIZombieLevel(aIZDef.mChallengeMode) && mApp->HasBeatenChallenge(aIZDef.mChallengeMode))
				{
					aLevelsCompleted++;
				}
			}

			if (aDef.mChallengeMode < GAMEMODE_PUZZLE_I_ZOMBIE_4 || mApp->HasFinishedAdventure() || aLevelsCompleted < 3)
			{
				return ClampInt(aDef.mChallengeMode - GAMEMODE_PUZZLE_I_ZOMBIE_1 - aLevelsCompleted, 0, 9);
			}
			else
			{
				return aDef.mChallengeMode == GAMEMODE_PUZZLE_I_ZOMBIE_4 ? 1 : 2;
			}
		}
	}
	else
	{
		int aIdxInPage = aDef.mRow * 5 + aDef.mCol;
		if ((aDef.mPage == CHALLENGE_PAGE_CHALLENGE || aDef.mPage == CHALLENGE_PAGE_SURVIVAL) && !mApp->HasFinishedAdventure())
		{
			return aIdxInPage < 3 ? 0 : aIdxInPage == 3 ? 1 : 2;
		}
		else
		{
			int aNumTrophies = mApp->GetNumTrophies(mPageIndex);
			if (aDef.mPage == CHALLENGE_PAGE_LIMBO)
			{
				return 0;
			}
			if (mApp->IsSurvivalEndless(aDef.mChallengeMode))
			{
				return 12 - aNumTrophies;
			}
			if (aDef.mPage == CHALLENGE_PAGE_SURVIVAL || aDef.mPage == CHALLENGE_PAGE_CHALLENGE)
			{
				aNumTrophies += 3;
			}
			else if (aDef.mPage == CHALLENGE_PAGE_VERSUS || aDef.mPage == CHALLENGE_PAGE_COOP_SURVIVAL) {
				return 0;
			}
			else
			{
				//TOD_ASSERT();
			}

			return aIdxInPage >= aNumTrophies ? aIdxInPage - aNumTrophies + 1 : 0;
		}
	}
}

//0x42E6E0
bool ChallengeScreen::ShowPageButtons()
{
	return mApp->mTodCheatKeys && mPageIndex != CHALLENGE_PAGE_SURVIVAL;
}

//0x42E710
void ChallengeScreen::UpdateButtons()
{
	for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++)
		mChallengeButtons[aChallengeMode]->mVisible = GetChallengeDefinition(aChallengeMode).mPage == mPageIndex;

	for (int aPage = 0; aPage < MAX_CHALLANGE_PAGES; aPage++)
	{
		ButtonWidget* aPageButton = mPageButton[aPage];

		
		if (aPage == mPageIndex)
		{
			aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(64, 64, 64);
			aPageButton->mDisabled = true;
		}
		else
		{
			aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
			aPageButton->mDisabled = false;
		}
	}
}

//0x42E8A0
int ChallengeScreen::AccomplishmentsNeeded(int theChallengeIndex)
{
	int aTrophiesNeeded = MoreTrophiesNeeded(theChallengeIndex);
	GameMode aGameMode = GetChallengeDefinition(theChallengeIndex).mChallengeMode;
	if (mApp->IsSurvivalEndless(aGameMode) && aTrophiesNeeded <= 3 && (mApp->mVeteranMode ? mApp->GetNumTrophiesVeteran(CHALLENGE_PAGE_SURVIVAL) : mApp->GetNumTrophies(CHALLENGE_PAGE_SURVIVAL)) < 13 &&
		mApp->HasFinishedAdventure() && !mApp->IsTrialStageLocked()) aTrophiesNeeded = 1;

	return mCheatEnableChallenges ? 0 : aTrophiesNeeded;
}

//0x42E920
void ChallengeScreen::DrawButton(Graphics* g, int theChallengeIndex)
{
	ButtonWidget* aChallengeButton = mChallengeButtons[theChallengeIndex];
	if (aChallengeButton->mVisible)
	{
		ChallengeDefinition& aDef = GetChallengeDefinition(theChallengeIndex);

		bool has5Columns = aDef.mPage == CHALLENGE_PAGE_SURVIVAL || aDef.mPage == CHALLENGE_PAGE_COOP_SURVIVAL || mApp->IsLastStandLevel(aDef.mChallengeMode);
		int offsetY = aDef.mPage == CHALLENGE_PAGE_SURVIVAL || aDef.mPage == CHALLENGE_PAGE_COOP_SURVIVAL ? 125: 93;
		aChallengeButton->mX = 38 + aDef.mCol * (has5Columns ? 124 : 155) + BOARD_ADDITIONAL_WIDTH;
		aChallengeButton->mY = offsetY + aDef.mRow * (aDef.mPage == CHALLENGE_PAGE_SURVIVAL || aDef.mPage == CHALLENGE_PAGE_COOP_SURVIVAL ? 145 : 119) + BOARD_OFFSET_Y - mScrollPosition;
		
		int aPosX = aChallengeButton->mX;
		int aPosY = aChallengeButton->mY;

		if (aChallengeButton->mY + 1 >= challengeClipRect.mY + challengeClipRect.mHeight || aChallengeButton->mY + 121 <= challengeClipRect.mY || mApp->mWidgetManager->mLastMouseY >= challengeClipRect.mY + challengeClipRect.mHeight || mApp->mWidgetManager->mLastMouseY <= challengeClipRect.mY)
		{
			aChallengeButton->mY = BOARD_HEIGHT;
		}

		if (!aChallengeButton->mIsDown)
			aChallengeButton->mIsOver = aChallengeButton->Contains(mApp->mWidgetManager->mLastMouseX, mApp->mWidgetManager->mLastMouseY);
		
		if (aChallengeButton->mIsDown)
		{
			aPosX++;
			aPosY++;
		}

		if (AccomplishmentsNeeded(theChallengeIndex) <= 1)
		{
			// ============================================================================================
			// ▲ 绘制按钮上的小游戏图标
			// ============================================================================================
			if (aChallengeButton->mDisabled)
			{
				g->SetColor(Color(92, 92, 92));
				g->SetColorizeImages(true);
			}
			if (theChallengeIndex == mUnlockChallengeIndex)
			{
				if (mUnlockState == UNLOCK_SHAKING)
				{
					g->SetColor(Color(92, 92, 92));
				}
				else if (mUnlockState == UNLOCK_FADING)
				{
					int aColor = TodAnimateCurve(50, 25, mUnlockStateCounter, 92, 255, CURVE_LINEAR);
					g->SetColor(Color(aColor, aColor, aColor));
				}
				g->SetColorizeImages(true);
			}

			g->SetClipRect(challengeClipRect);
			if (mPageIndex == CHALLENGE_PAGE_SURVIVAL || mPageIndex == CHALLENGE_PAGE_COOP_SURVIVAL)
			{
				g->DrawImageCel(Sexy::IMAGE_SURVIVAL_THUMBNAILS, aPosX + 13, aPosY + 4 , aDef.mChallengeIconIndex);
			}
			else
			{
				if (aDef.mChallengeMode >= GAMEMODE_CHALLENGE_ZOMBOTANY && aDef.mChallengeMode <= GAMEMODE_CHALLENGE_DR_ZOMBOSS_REVENGE && aDef.mChallengeMode != GAMEMODE_CHALLENGE_SURPRISE_ATTACK && aDef.mChallengeMode != GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2 && aDef.mChallengeMode != GAMEMODE_CHALLENGE_ZOMBOTANY_2 || aDef.mChallengeMode == GAMEMODE_CHALLENGE_BUNGEE_BLITZ || aDef.mChallengeMode == GAMEMODE_CHALLENGE_DARK_STORMY_NIGHT)
				{
					g->DrawImageCel(Sexy::IMAGE_CHALLENGE_THUMBNAILS, aPosX + 13, aPosY + 4, aDef.mChallengeIconIndex);
				}
				else if (aDef.mChallengeMode >= GAMEMODE_CHALLENGE_SQUIRREL && aDef.mChallengeMode <= GAMEMODE_CHALLENGE_METAL_FESTIVAL || aDef.mChallengeMode == GAMEMODE_CHALLENGE_SURPRISE_ATTACK || aDef.mChallengeMode == GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2 || aDef.mChallengeMode == GAMEMODE_CHALLENGE_ZOMBOTANY_2)
				{
					g->DrawImageCel(Sexy::IMAGE_CHALLENGE_THUMBNAILS2, aPosX + 13, aPosY + 4, aDef.mChallengeIconIndex);
				}
			}

			// ============================================================================================
			// ▲ 绘制小游戏按钮边框
			// ============================================================================================
			bool aHighLight = aChallengeButton->mIsOver && theChallengeIndex != mUnlockChallengeIndex;
			g->SetColorizeImages(false);

			g->DrawImage(aHighLight ? Sexy::IMAGE_CHALLENGE_WINDOW_HIGHLIGHT : Sexy::IMAGE_CHALLENGE_WINDOW, aPosX - 6, aPosY - 2 );

			// ============================================================================================
			// ▲ 绘制小游戏的名称
			// ============================================================================================
			Color aTextColor = aHighLight ? Color(250, 40, 40) : Color(42, 42, 90);
			SexyString aName = TodStringTranslate(aDef.mChallengeName);
			
			/*if (aName == "Metal Festival")
				aName = " Metal Festival";
			else if (aName == "Heavy Rainstorm")
				aName = " Heavy  Rainstorm";
			else if (aName == "Worlds Colliding")
				aName = " Worlds  Colliding";
			else if (aName == "Plant Minimalist")
				aName = "  Plant   Minimalist";
			else if (aName == "Weirdmageddon")
				aName = "  Weird   mageddon";*/

			if (aChallengeButton->mDisabled || (theChallengeIndex == mUnlockChallengeIndex && mUnlockState == UNLOCK_SHAKING))
			{
				aName = _S("?");
			}

			g->SetClipRect(challengeClipRect);
			int aNameLen = aName.size();

			if (TodDrawStringWrappedHelper(g, aName, Rect(aPosX + 6, aPosY + 76, 94, 28), Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER, false) < 28)
			{
				TodDrawString(g, aName, aPosX + 52, aPosY + 96, Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER);
			}
			else
			{
				TodDrawStringWrapped(g, aName, Rect(aPosX + 6, aPosY + 76, 94, 28), Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER_VERTICAL_MIDDLE);
			}
			/*
			if (aNameLen < 13)
			{
				TodDrawString(g, aName, aPosX + 52, aPosY + 96 , Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER);
			}
			else
			{
				// 先尝试在名称字符串的后半段取空格以将字符串分隔为两行，若后半段中无空格则在整个字符串中寻找空格
				int aHalfPos = ((mPageIndex == CHALLENGE_PAGE_SURVIVAL || aDef.mPage == CHALLENGE_PAGE_COOP_SURVIVAL) && !aChallengeButton->mDisabled) ? 7 : (aNameLen / 2 - 1);
				const SexyChar* aSpacedChar = _S(aName.c_str() + aHalfPos, _S(' '));
				while(aSpacedChar[0]!=' ')
				{
					aHalfPos++;
					aSpacedChar = _S(aName.c_str() + aHalfPos, _S(' '));
					if(aSpacedChar[0]=='\0')
					{
						aHalfPos--;
						aSpacedChar = _S(aName.c_str() + aHalfPos, _S(' '));
						break;
					}
				}
				aHalfPos--;
				aSpacedChar = _S(aName.c_str() + aHalfPos, _S(' '));

				
				if (aSpacedChar == nullptr)
				{
					aSpacedChar = _S(aName.c_str(), _S(' '));
				}

				// 分别计算取得两行文本的长度
				int aLine1Len = aNameLen;
				int aLine2Len = 0;
				if (aSpacedChar != nullptr)
				{
					aLine1Len = aSpacedChar - aName.c_str();
					aLine2Len = aNameLen - aLine1Len - 1;
				}
				
				// 分别绘制两行文本字符串
				auto topStr=aName.substr(0, aLine1Len+1);
				auto botStr=aName.substr(aLine1Len + 1, aLine2Len);
				if (botStr.empty())
				{
					TodDrawString(g, aName, aPosX + 52, aPosY + 96 , Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER);
				}
				else
				{
					TodDrawString(g, topStr, aPosX + 52, aPosY + 88 , Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER);
					if (aLine2Len > 0)
					{
						TodDrawString(g, botStr, aPosX + 52, aPosY + 102 , Sexy::FONT_BRIANNETOD12, aTextColor, DS_ALIGN_CENTER);
					}
				}
			}*/

			// ============================================================================================
			// ▲ 绘制关卡锁定或关卡完成的贴图以及关卡最高记录的文本等
			// ============================================================================================
			int aRecord = mApp->mPlayerInfo->mChallengeRecords[theChallengeIndex];
			if (theChallengeIndex == mUnlockChallengeIndex)
			{
				Image* aLockImage = Sexy::IMAGE_LOCK;
				if (mUnlockState == UNLOCK_FADING)
				{
					aLockImage = Sexy::IMAGE_LOCK_OPEN;
					g->SetColor(Color(255, 255, 255, TodAnimateCurve(25, 0, mUnlockStateCounter, 255, 0, CURVE_LINEAR)));
					g->SetColorizeImages(true);
				}
				TodDrawImageScaledF(g, aLockImage, aPosX + 24 + mLockShakeX, aPosY + 9 + mLockShakeY , 0.7f, 0.7f);
				g->SetColorizeImages(false);
			}
			else if (aRecord > 0 && mPageIndex != CHALLENGE_PAGE_VERSUS && mPageIndex != CHALLENGE_PAGE_COOP_SURVIVAL)
			{
				if (mApp->HasBeatenChallengeVeteran(aDef.mChallengeMode))
				{
					g->DrawImage(Sexy::IMAGE_MINIGAME_DIAMOND_TROPHY, aPosX - 6, aPosY - 2);
				}
				else if (mApp->HasBeatenChallenge(aDef.mChallengeMode))
				{
					g->DrawImage(Sexy::IMAGE_MINIGAME_TROPHY, aPosX - 6, aPosY - 2 );
				}
				else if (mApp->IsEndlessScaryPotter(aDef.mChallengeMode) || mApp->IsEndlessIZombie(aDef.mChallengeMode))
				{
					SexyString aAchievement = mApp->Pluralize(aRecord, _S("[ONE_FLAG]"), _S("[COUNT_FLAGS]"));
					TodDrawString(g, aAchievement, aPosX + 48, aPosY + 48 , Sexy::FONT_CONTINUUMBOLD14OUTLINE, Color::White, DS_ALIGN_CENTER);
					TodDrawString(g, aAchievement, aPosX + 48, aPosY + 48 , Sexy::FONT_CONTINUUMBOLD14, Color(255, 0, 0), DS_ALIGN_CENTER);
				}
				else if (mApp->IsSurvivalEndless(aDef.mChallengeMode))
				{
					SexyString aAchievement = TodReplaceNumberString(_S("[LONGEST_STREAK]"), _S("{STREAK}"), aRecord);
					Rect aRect(aPosX, aPosY + 15 , 96, 200);
					TodDrawStringWrapped(g, aAchievement, aRect, Sexy::FONT_CONTINUUMBOLD14OUTLINE, Color::White, DS_ALIGN_CENTER);
					TodDrawStringWrapped(g, aAchievement, aRect, Sexy::FONT_CONTINUUMBOLD14, Color(255, 0, 0), DS_ALIGN_CENTER);
				}
			}
			else if (aChallengeButton->mDisabled)
			{
				TodDrawImageScaledF(g, Sexy::IMAGE_LOCK, aPosX + 24, aPosY + 9 , 0.7f, 0.7f);
			}

			g->ClearClipRect();
		}
		else
		{
			g->SetClipRect(challengeClipRect);
			g->DrawImage(Sexy::IMAGE_CHALLENGE_BLANK, aPosX, aPosY );
			g->ClearClipRect();
		}
	}

	g->ClearClipRect();
}

void ChallengeScreen::MouseWheel(int theDelta)
{
	mScrollAmount -= mBaseScrollSpeed * theDelta;
	mScrollAmount -= mScrollAmount * mScrollAccel;
}

//0x42F160
void ChallengeScreen::Draw(Graphics* g)
{
	g->SetLinearBlend(true);
	g->DrawImage(Sexy::IMAGE_CHALLENGE_BACKGROUND, 0, 0);

	SexyString aTitleString = 
		mPageIndex == CHALLENGE_PAGE_SURVIVAL ? _S("[PICK_AREA]") : 
		mPageIndex == CHALLENGE_PAGE_PUZZLE ? _S("[SCARY_POTTER]") : _S("[PICK_CHALLENGE]");

	if (mPageIndex == CHALLENGE_PAGE_VERSUS)
		aTitleString = "Versus";
	else if (mPageIndex == CHALLENGE_PAGE_COOP_SURVIVAL)
		aTitleString = _S("Cooperative");
	else if (mPageIndex == CHALLENGE_PAGE_CHALLENGE)
		aTitleString = _S("[PICK_CHALLENGE]");

	TodDrawString(g, aTitleString, 400 + BOARD_ADDITIONAL_WIDTH, 58 + BOARD_OFFSET_Y, Sexy::FONT_HOUSEOFTERROR28, Color(220, 220, 220), DS_ALIGN_CENTER);

	int curPage = round(mScrollPosition / (119 * 4)) + 1, maxPage = round(mMaxScrollPosition / (119 * 4)) + 1;
	string pageLabel = maxPage != 1 ? "pp. " + to_string(curPage) + " / pp. " + to_string(maxPage) : "pp. " + to_string(curPage);
	int xPos = maxPage != 1 ? mpageUP->mX - 60 : 1280 - 60 - BOARD_ADDITIONAL_WIDTH;
	TodDrawString(g, pageLabel, xPos, 568 + BOARD_OFFSET_Y + 17, Sexy::FONT_BRIANNETOD16, Color(42, 42, 90), DS_ALIGN_CENTER);

	int aTrophiesGot = (mApp->mVeteranMode ? mApp->GetNumTrophiesVeteran(mPageIndex) : mApp->GetNumTrophies(mPageIndex));
	int aTrophiesTotal = mPageIndex == CHALLENGE_PAGE_SURVIVAL ? 12 : mPageIndex == CHALLENGE_PAGE_CHALLENGE ? 82 : mPageIndex == CHALLENGE_PAGE_PUZZLE ? 24 : 0;
	if (aTrophiesTotal > 0)
	{
		SexyString aTrophyString = StrFormat(_S("%d/%d"), aTrophiesGot, aTrophiesTotal);
		TodDrawString(g, aTrophyString, 739 + BOARD_ADDITIONAL_WIDTH, 73 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, Color(255, 240, 0), DS_ALIGN_CENTER);
		TodDrawImageScaledF(g, (mApp->mVeteranMode ? Sexy::IMAGE_DIAMOND_TROPHY : Sexy::IMAGE_TROPHY), 718 + BOARD_ADDITIONAL_WIDTH, 26 + BOARD_OFFSET_Y, 0.5f, 0.5f);
	}

	for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++)
	{
		DrawButton(g, aChallengeMode);
	}

	mToolTip->Draw(g);
	mQUICKPLAY->Draw(g);
	mpageDOWN->Draw(g);
	mpageUP->Draw(g);
}

void ChallengeScreen::MouseDown(int x, int y, int theClickCount)
{
	if (mpageDOWN->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
		int curScroll = round(mScrollPosition / (119 * 4)) + 1;
		mScrollPosition = curScroll * (119 * 4);
		mScrollPosition = ClampFloat(mScrollPosition, 0, mMaxScrollPosition);
	}
	else if (mpageUP->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
		int curScroll = round(mScrollPosition / (119 * 4)) - 1;
		mScrollPosition = curScroll * (119 * 4);
		mScrollPosition = ClampFloat(mScrollPosition, 0, mMaxScrollPosition);
	}
	else if (mQUICKPLAY->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
		mApp->KillChallengeScreen();
		int rngChallenge = 0;
		do {
			rngChallenge = rand() % NUM_CHALLENGE_MODES;
		} while (gChallengeDefs[rngChallenge].mPage != mPageIndex && !mApp->HasBeatenChallenge(GetChallengeDefinition(rngChallenge).mChallengeMode));
		
		mApp->PreNewGame((GameMode)rngChallenge, true);
	}
}

//0x42F510
void ChallengeScreen::Update()
{
	Widget::Update();
	UpdateToolTip();

	float aScrollSpeed = mBaseScrollSpeed + abs(mScrollAmount) * mScrollAccel;
	mScrollPosition = ClampFloat(mScrollPosition += mScrollAmount * aScrollSpeed, 0, mMaxScrollPosition);
	mScrollAmount *= (1.0f - mScrollAccel);
	mSlider->SetValue(max(0.0, min(mMaxScrollPosition, mScrollPosition)) / mMaxScrollPosition);
	mQUICKPLAY->Update();
	mpageDOWN->Update();
	mpageUP->Update();

	if (mUnlockStateCounter > 0) mUnlockStateCounter--;
	if (mUnlockState == UNLOCK_SHAKING)
	{
		if (mUnlockStateCounter == 0)
		{
			mApp->PlayFoley(FOLEY_PAPER);
			mUnlockState = UNLOCK_FADING;
			mUnlockStateCounter = 50;
			mLockShakeX = 0;
			mLockShakeY = 0;
		}
		else
		{
			mLockShakeX = RandRangeFloat(-2, 2);
			mLockShakeY = RandRangeFloat(-2, 2);
		}
	}
	else if (mUnlockState == UNLOCK_FADING && mUnlockStateCounter == 0)
	{
		mUnlockState = UNLOCK_OFF;
		mUnlockStateCounter = 0;
		mUnlockChallengeIndex = -1;
	}

	MarkDirty();
}

//0x42F640
void ChallengeScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	AddWidget(mBackButton);
	AddWidget(mSlider);
	for (ButtonWidget* aButton : mPageButton) AddWidget(aButton);
	for (ButtonWidget* aButton : mChallengeButtons) AddWidget(aButton);
}

//0x42F6B0
void ChallengeScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	RemoveWidget(mBackButton);
	RemoveWidget(mSlider);
	for (ButtonWidget* aButton : mPageButton) RemoveWidget(aButton);
	for (ButtonWidget* aButton : mChallengeButtons) RemoveWidget(aButton);
}

//0x42F720
void ChallengeScreen::ButtonPress(int theId)
{
	mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
}

//0x42F740
void ChallengeScreen::ButtonDepress(int theId)
{
	if (theId == ChallengeScreen::ChallengeScreen_Back)
	{
		mApp->KillChallengeScreen();
		mApp->DoBackToMainNotSilent();
	}

	int aChallengeMode = theId - ChallengeScreen::ChallengeScreen_Mode;
	if (aChallengeMode >= 0 && aChallengeMode < NUM_CHALLENGE_MODES)
	{
		mApp->KillChallengeScreen();
		mApp->PreNewGame((GameMode)(aChallengeMode + 1), true);
	}

	int aPageIndex = theId - ChallengeScreen::ChallengeScreen_Page;
	if (aPageIndex >= 0 && aPageIndex < MAX_CHALLANGE_PAGES)
	{
		mPageIndex = (ChallengePage)aPageIndex;
		if (mPageIndex == CHALLENGE_PAGE_CHALLENGE)
			mMaxScrollPosition = 119 * 4 * 3;
		else
			mMaxScrollPosition = 0;
		int maxPage = floor(mMaxScrollPosition / (119 * 4)) + 1;
		mpageDOWN->mBtnNoDraw = mpageUP->mBtnNoDraw = maxPage == 1;
		mSlider->mDisabled = maxPage == 1;
		mSlider->mVisible = maxPage > 1;
		UpdateButtons();
	}
}

//0x42F7E0
void ChallengeScreen::UpdateToolTip()
{
	if (!mApp->mWidgetManager->mMouseIn || !mApp->mActive)
	{
		mToolTip->mVisible = false;
		return;
	}

	for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++)
	{
		ChallengeDefinition& aDef = GetChallengeDefinition(aChallengeMode);
		ButtonWidget* aChallengeButton = mChallengeButtons[aChallengeMode];
		if (aChallengeButton->mVisible && aChallengeButton->mDisabled &&
			aChallengeButton->Contains(mApp->mWidgetManager->mLastMouseX, mApp->mWidgetManager->mLastMouseY) &&
			(AccomplishmentsNeeded(aChallengeMode) <= 1))
		{
			mToolTip->mX = aChallengeButton->mWidth / 2 + aChallengeButton->mX;
			mToolTip->mY = aChallengeButton->mY;
			if (MoreTrophiesNeeded(aChallengeMode) > 0)
			{
				SexyString aLabel;
				if (mPageIndex == CHALLENGE_PAGE_PUZZLE)
				{
					if (IsScaryPotterLevel(aDef.mChallengeMode))
					{
						if (!mApp->HasFinishedAdventure() && aDef.mChallengeMode == GAMEMODE_SCARY_POTTER_4)
						{
							aLabel = _S("[FINISH_ADVENTURE_TOOLTIP]");
						}
						else
						{
							aLabel = _S("[ONE_MORE_SCARY_POTTER_TOOLTIP]");
						}
					}
					else if (IsIZombieLevel(aDef.mChallengeMode))
					{
						if (!mApp->HasFinishedAdventure() && aDef.mChallengeMode == GAMEMODE_PUZZLE_I_ZOMBIE_4)
						{
							aLabel = _S("[FINISH_ADVENTURE_TOOLTIP]");
						}
						else
						{
							aLabel = _S("[ONE_MORE_IZOMBIE_TOOLTIP]");
						}
					}
				}
				else if (!mApp->HasFinishedAdventure() || mApp->IsTrialStageLocked())
				{
					aLabel = _S("[FINISH_ADVENTURE_TOOLTIP]");
				}
				else if (mApp->IsSurvivalEndless(aDef.mChallengeMode))
				{
					aLabel = _S("[10_SURVIVAL_TOOLTIP]");
				}
				else if (mPageIndex == CHALLENGE_PAGE_SURVIVAL)
				{
					aLabel = _S("[ONE_MORE_SURVIVAL_TOOLTIP]");
				}
				else if (mPageIndex == CHALLENGE_PAGE_CHALLENGE)
				{
					aLabel = _S("[ONE_MORE_CHALLENGE_TOOLTIP]");
				}
				else continue;

				mToolTip->SetLabel(aLabel);
				mToolTip->mVisible = true;
				return;
			} // end if (MoreTrophiesNeeded(aChallengeMode) > 0)
		} // end 需要显示标签的条件判断
	}

	mToolTip->mVisible = false;
}
