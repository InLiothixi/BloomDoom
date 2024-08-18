#include "Coin.h"
#include "Plant.h"
#include "Board.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "GridItem.h"
#include "ZenGarden.h"
#include "Challenge.h"
#include "Projectile.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../GameConstants.h"
#include "System/PlayerInfo.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../Sexy.TodLib/TodStringFile.h"

PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES] = {  //0x69F2B0
    { SeedType::SEED_PEASHOOTER,        nullptr, ReanimationType::REANIM_PEASHOOTER,    0,  100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_SUNFLOWER,         nullptr, ReanimationType::REANIM_SUNFLOWER,     1,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER") },
    { SeedType::SEED_CHERRYBOMB,        nullptr, ReanimationType::REANIM_CHERRYBOMB,    3,  150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHERRY_BOMB") },
    { SeedType::SEED_WALLNUT,           nullptr, ReanimationType::REANIM_WALLNUT,       2,  50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("WALL_NUT") },
    { SeedType::SEED_POTATOMINE,        nullptr, ReanimationType::REANIM_POTATOMINE,    37, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("POTATO_MINE") },
    { SeedType::SEED_SNOWPEA,           nullptr, ReanimationType::REANIM_SNOWPEA,       4,  175,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SNOW_PEA") },
    { SeedType::SEED_CHOMPER,           nullptr, ReanimationType::REANIM_CHOMPER,       31, 150,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHOMPER") },
    { SeedType::SEED_REPEATER,          nullptr, ReanimationType::REANIM_REPEATER,      5,  200,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("REPEATER") },
    { SeedType::SEED_PUFFSHROOM,        nullptr, ReanimationType::REANIM_PUFFSHROOM,    6,  0,      750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PUFF_SHROOM") },
    { SeedType::SEED_SUNSHROOM,         nullptr, ReanimationType::REANIM_SUNSHROOM,     7,  25,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUN_SHROOM") },
    { SeedType::SEED_FUMESHROOM,        nullptr, ReanimationType::REANIM_FUMESHROOM,    9,  75,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("FUME_SHROOM") },
    { SeedType::SEED_GRAVEBUSTER,       nullptr, ReanimationType::REANIM_GRAVE_BUSTER,  40, 75,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GRAVE_BUSTER") },
    { SeedType::SEED_HYPNOSHROOM,       nullptr, ReanimationType::REANIM_HYPNOSHROOM,   10, 75,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("HYPNO_SHROOM") },
    { SeedType::SEED_SCAREDYSHROOM,     nullptr, ReanimationType::REANIM_SCRAREYSHROOM, 33, 25,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SCAREDY_SHROOM") },
    { SeedType::SEED_ICESHROOM,         nullptr, ReanimationType::REANIM_ICESHROOM,     36, 75,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("ICE_SHROOM") },
    { SeedType::SEED_DOOMSHROOM,        nullptr, ReanimationType::REANIM_DOOMSHROOM,    20, 250,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("DOOM_SHROOM") },
    { SeedType::SEED_LILYPAD,           nullptr, ReanimationType::REANIM_LILYPAD,       19, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("LILY_PAD") },
    { SeedType::SEED_SQUASH,            nullptr, ReanimationType::REANIM_SQUASH,        21, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SQUASH") },
    { SeedType::SEED_THREEPEATER,       nullptr, ReanimationType::REANIM_THREEPEATER,   12, 300,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("THREEPEATER") },
    { SeedType::SEED_TANGLEKELP,        nullptr, ReanimationType::REANIM_TANGLEKELP,    17, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TANGLE_KELP") },
    { SeedType::SEED_JALAPENO,          nullptr, ReanimationType::REANIM_JALAPENO,      11, 125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("JALAPENO") },
    { SeedType::SEED_SPIKEWEED,         nullptr, ReanimationType::REANIM_SPIKEWEED,     22, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEWEED") },
    { SeedType::SEED_TORCHWOOD,         nullptr, ReanimationType::REANIM_TORCHWOOD,     29, 175,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TORCHWOOD") },
    { SeedType::SEED_TALLNUT,           nullptr, ReanimationType::REANIM_TALLNUT,       28, 125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TALL_NUT") },
    { SeedType::SEED_SEASHROOM,         nullptr, ReanimationType::REANIM_SEASHROOM,     39, 0,      3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SEA_SHROOM") },
    { SeedType::SEED_PLANTERN,          nullptr, ReanimationType::REANIM_PLANTERN,      38, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("PLANTERN") },
    { SeedType::SEED_CACTUS,            nullptr, ReanimationType::REANIM_CACTUS,        15, 175,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CACTUS") },
    { SeedType::SEED_BLOVER,            nullptr, ReanimationType::REANIM_BLOVER,        18, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("BLOVER") },
    { SeedType::SEED_SPLITPEA,          nullptr, ReanimationType::REANIM_SPLITPEA,      32, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SPLIT_PEA") },
    { SeedType::SEED_STARFRUIT,         nullptr, ReanimationType::REANIM_STARFRUIT,     30, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("STARFRUIT") },
    { SeedType::SEED_PUMPKINSHELL,      nullptr, ReanimationType::REANIM_PUMPKIN,       25, 125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("PUMPKIN") },
    { SeedType::SEED_MAGNETSHROOM,      nullptr, ReanimationType::REANIM_MAGNETSHROOM,  35, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MAGNET_SHROOM") },
    { SeedType::SEED_CABBAGEPULT,       nullptr, ReanimationType::REANIM_CABBAGEPULT,   13, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("CABBAGE_PULT") },
    { SeedType::SEED_FLOWERPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,    33, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("FLOWER_POT") },
    { SeedType::SEED_KERNELPULT,        nullptr, ReanimationType::REANIM_KERNELPULT,    13, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("KERNEL_PULT") },
    { SeedType::SEED_INSTANTCOFFEE,     nullptr, ReanimationType::REANIM_COFFEEBEAN,    33, 75,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("COFFEE_BEAN") },
    { SeedType::SEED_GARLIC,            nullptr, ReanimationType::REANIM_GARLIC,        8,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GARLIC") },
    { SeedType::SEED_UMBRELLA,          nullptr, ReanimationType::REANIM_UMBRELLALEAF,  23, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("UMBRELLA_LEAF") },
    { SeedType::SEED_MARIGOLD,          nullptr, ReanimationType::REANIM_MARIGOLD,      24, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("MARIGOLD") },
    { SeedType::SEED_MELONPULT,         nullptr, ReanimationType::REANIM_MELONPULT,     14, 300,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("MELON_PULT") },
    { SeedType::SEED_SPORE_SHROOM,      nullptr, ReanimationType::REANIM_SPORESHROOM,   10,  25,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("SPORE_SHROOM") },
    { SeedType::SEED_AMPLIFLOWER,       nullptr, ReanimationType::REANIM_AMPLIFLOWER,   10, 125,    750,    PlantSubClass::SUBCLASS_NORMAL,     3250,   _S("AMPLIFLOWER") },
    { SeedType::SEED_GATLINGPEA,        nullptr, ReanimationType::REANIM_GATLINGPEA,    5,  250,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("GATLING_PEA") },
    { SeedType::SEED_TWINSUNFLOWER,     nullptr, ReanimationType::REANIM_TWIN_SUNFLOWER,1,  150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("TWIN_SUNFLOWER") },
    { SeedType::SEED_GLOOMSHROOM,       nullptr, ReanimationType::REANIM_GLOOMSHROOM,   27, 150,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    200,    _S("GLOOM_SHROOM") },
    { SeedType::SEED_CATTAIL,           nullptr, ReanimationType::REANIM_CATTAIL,       27, 275,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CATTAIL") },
    { SeedType::SEED_WINTERMELON,       nullptr, ReanimationType::REANIM_WINTER_MELON,  27, 300,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("WINTER_MELON") },
    { SeedType::SEED_GOLD_MAGNET,       nullptr, ReanimationType::REANIM_GOLD_MAGNET,   27, 50,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GOLD_MAGNET") },
    { SeedType::SEED_SPIKEROCK,         nullptr, ReanimationType::REANIM_SPIKEROCK,     27, 125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEROCK") },
    { SeedType::SEED_COBCANNON,         nullptr, ReanimationType::REANIM_COBCANNON,     16, 700,    5000,   PlantSubClass::SUBCLASS_NORMAL,     600,    _S("COB_CANNON") },
    { SeedType::SEED_IMITATER,          nullptr, ReanimationType::REANIM_IMITATER,      33, 0,      750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("IMITATER") },
    { SeedType::SEED_FROST_CABBAGE,   nullptr, ReanimationType::REANIM_FROSTCABBAGE,    13, 175,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("FROST_CABBAGE")},
    { SeedType::SEED_SLINGPEA,          nullptr, ReanimationType::REANIM_SLINGPEA,      53, 325,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("SLING_PEA")},
    { SeedType::SEED_WATERPOT,          nullptr, ReanimationType::REANIM_FLOWER_POT,    33, 50,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("WATER_POT")},
    { SeedType::SEED_CHILIBEAN,         nullptr, ReanimationType::REANIM_GARLIC,        55,  75,     3000,  PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHILI_BEAN") },
    { SeedType::SEED_MOONFLOWER,        nullptr, ReanimationType::REANIM_MOONFLOWER,    56,  100,     750,  PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("MOONFLOWER") },
    { SeedType::SEED_SUNBLOOM,          nullptr, ReanimationType::REANIM_SUNFLOWER,     57, 0,      5000,    PlantSubClass::SUBCLASS_NORMAL,     0,     _S("GOLD_BLOOM")},
    { SeedType::SEED_EINSTEINAPPLE,    nullptr, ReanimationType::REANIM_ICESHROOM,     58,  175,     5000,    PlantSubClass::SUBCLASS_NORMAL,     0,   _S("EINSTEIN_APPLE") },
    { SeedType::SEED_PLASMAWOOD,       nullptr, ReanimationType::REANIM_FLAMEWOOD,     59,  225,     5000,    PlantSubClass::SUBCLASS_NORMAL,     0,   _S("PLASMA_WOOD") },
    { SeedType::SEED_ICYFUMESHROOM,    nullptr, ReanimationType::REANIM_ICY_FUMESHROOM,60,  225,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("ICY_FUME_SHROOM") },
    { SeedType::SEED_BONKCHOY,          nullptr, ReanimationType::REANIM_BONKCHOY,      61,  150,     750,    PlantSubClass::SUBCLASS_NORMAL,    0,    _S("BONKCHOY") },
    { SeedType::SEED_BOMBBERRY,         nullptr, ReanimationType::REANIM_CABBAGEPULT,   62,  200,     750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("BOMBBERRY") },
    { SeedType::SEED_CHERRYHOVERBOMB,  nullptr, ReanimationType::REANIM_CHERRYBOMB,    2,  75,     3000,    PlantSubClass::SUBCLASS_NORMAL,    0,    _S("CHERRY_HOVERBOMB") },
    { SeedType::SEED_SEEDROOT,          nullptr, ReanimationType::REANIM_SEEDROOT,        63,  250,     5000,    PlantSubClass::SUBCLASS_NORMAL,    0,    _S("SEEDROOT") },
    { SeedType::SEED_HURRIKALE,         nullptr, ReanimationType::REANIM_HURRIKALE,     64,  175,     750,    PlantSubClass::SUBCLASS_NORMAL,    0,    _S("HURRIKALE") },
    { SeedType::SEED_PRIMAL_SUNFLOWER,  nullptr, ReanimationType::REANIM_SUNFLOWER,     65,  100,     750,    PlantSubClass::SUBCLASS_NORMAL, 2500,    _S("PRIMAL_SUNFLOWER") },
    { SeedType::SEED_ARTIC_NUT,         nullptr, ReanimationType::REANIM_ARTICNUT,       66,  125,     3000,    PlantSubClass::SUBCLASS_NORMAL,     0,   _S("ARTIC_NUT")},
    { SeedType::SEED_STARCICLE,         nullptr, ReanimationType::REANIM_STARFRUIT,     30, 200,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("STARCICLE") },
    { SeedType::SEED_MAGNIFYINGGRASS,  nullptr, ReanimationType::REANIM_MAGNIFYINGGRASS,   68, 75,  750,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MAGNIFYING_GRASS") },
    { SeedType::SEED_DUMMY,             nullptr, ReanimationType::REANIM_DUMMY,         69,  1000,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,    _S("DUMMY_NUT") },
    { SeedType::SEED_SUNBEAN,           nullptr, ReanimationType::REANIM_SUNBEAN,       70,  50,     750,   PlantSubClass::SUBCLASS_NORMAL,     0,    _S("SUNBEAN") },
    { SeedType::SEED_FIRESHROOM,        nullptr, ReanimationType::REANIM_FIRESHROOM,    71,  150,     3000,   PlantSubClass::SUBCLASS_NORMAL,     150,      _S("FIRE_SHROOM") },
    { SeedType::SEED_SUPERCHOMPER,      nullptr, ReanimationType::REANIM_SUPERCHOMPER,  72, 200,    5000,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUPERCHOMPER") },
    { SeedType::SEED_SHOOTINGSTAR,      nullptr, ReanimationType::REANIM_SHOOTINGSTAR,  73, 325,    5000,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SHOOTING_STAR") },
    { SeedType::SEED_STINGER,           nullptr, ReanimationType::REANIM_STINGER,        74, 175,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("STINGER") },
    { SeedType::SEED_BRAVEYSHROOM,     nullptr, ReanimationType::REANIM_BRAVEYSHROOM,   75, 75,     5000,    PlantSubClass::SUBCLASS_SHOOTER,    75,    _S("BRAVEY_SHROOM") },
    { SeedType::SEED_ICEBERG_LETTUCE,   nullptr, ReanimationType::REANIM_ICEBERG_LETTUCE,  76, 0,  5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("ICEBERG_LETTUCE") },
    { SeedType::SEED_FLYINGPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,    77, 100,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,         _S("FLYING_POT") },
    { SeedType::SEED_EXPLODE_O_NUT,     nullptr, ReanimationType::REANIM_EXPLODE_O_NUT,       3,  175,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,    _S("EXPLODE_O_NUT") },
    { SeedType::SEED_GIANT_WALLNUT,     nullptr, ReanimationType::REANIM_WALLNUT,       2,  0,      3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GIANT_WALLNUT") },
    { SeedType::SEED_SPROUT,            nullptr, ReanimationType::REANIM_ZENGARDEN_SPROUT,  33, 0,  3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPROUT") },
    { SeedType::SEED_LEFTPEATER,        nullptr, ReanimationType::REANIM_REPEATER,      5,  200,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("LEFTPEATER") }
};

//0x401B20
Plant::Plant()
{
}

//0x45DB60
void Plant::PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
    mPlantCol = theGridX;
    mRow = theGridY;
    if (mBoard && !mIsPreview && !mIsPlayer)
    {
        mX = mBoard->GridToPixelX(theGridX, theGridY);
        mY = mBoard->GridToPixelY(theGridX, theGridY);
    }
    mAnimCounter = 0;
    mAnimPing = true;
    mFrame = 0;
    mShootingCounter = 0;
    mShakeOffsetX = 0.0f;
    mShakeOffsetY = 0.0f;
    mFrameLength = RandRangeInt(12, 18);
    mCoffeebeaned = false;
    mTargetX = -1;
    mTargetY = -1;
    mLastPortalX = -1;
    mHighlighted = false;
    mStartRow = mRow;
    mAmplifiedCounter = 0;
    mPlantAmplifierID = PLANTID_NULL;
    mNumFrames = 5;
    mState = PlantState::STATE_NOTREADY;
    mDead = false;
    mSquished = false;
    mSeedType = theSeedType;
    mImitaterType = theImitaterType;
    mPlantAge = 0;
    mPlantHealth = 300;
    mDoSpecialCountdown = 0;
    mDisappearCountdown = 200;
    mStateCountdown = 0;
    mFacingLeft = false;
    mParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mBodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mBackbodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID2 = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID3 = ReanimationID::REANIMATIONID_NULL;
    mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;
    mLightReanimID = ReanimationID::REANIMATIONID_NULL;
    mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    mBlinkCountdown = 0;
    mRecentlyEatenCountdown = 0;
    mEatenFlashCountdown = 0;
    mBeghouledFlashCountdown = 0;
    mWidth = 80;
    mHeight = 80;
    memset(mMagnetItems, 0, sizeof(mMagnetItems));
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    mIsAsleep = false;
    mWakeUpCounter = 0;
    mOnBungeeState = PlantOnBungeeState::NOT_ON_BUNGEE;
    mPottedPlantIndex = -1;
    mLaunchRate = aPlantDef.mLaunchRate;
    mSubclass = aPlantDef.mSubClass;
    mRenderOrder = CalcRenderOrder();
    mHeatWaveTimer = 0;
    mBowlingSlope = 0;
    mBerserkCounter = 0;
    mExplodePulse = 0;
    mDarken = false;
    mRootSeedType = SEED_NONE;
    mRootMaxTime = 0;
    mRootTime = 0;
    mMindControlled = false;
    mChilledCounter = 0;
    mIceTrapCounter = 0;
    mButteredCounter = 0;
    mBurnCounter = 0;
    mIgnoreBlink = false;
    mIsDummy = false;
    mSingingCounter = 0;

    Reanimation* aBodyReanim = nullptr;
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
        aBodyReanim = mApp->AddReanimation(0.0f, aOffsetY, mRenderOrder + 1, aPlantDef.mReanimationType);
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);

        if (mSeedType == SEED_MOONFLOWER)
            aBodyReanim->mAnimRate *= 2;

        if (aBodyReanim->TrackExists("anim_idle"))
            aBodyReanim->SetFramesForLayer("anim_idle");
        
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL && mSeedType == SEED_UMBRELLA)
        {
            aBodyReanim->SetFramesForLayer("anim_cover");
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
        }

        if ((mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (mSeedType == SEED_WALLNUT || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_GIANT_WALLNUT || mSeedType == SEED_EXPLODE_O_NUT)) && aBodyReanim->TrackExists("_ground") && !mIsPreview)
        {
            aBodyReanim->SetFramesForLayer("_ground");
            if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_ARTIC_NUT)
                aBodyReanim->mAnimRate = RandRangeFloat(12.0f, 18.0f);
            else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 10.0f);
        }

        aBodyReanim->mIsAttachment = true;
        mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
        mBlinkCountdown = 400 + Sexy::Rand(400);
    }

    if (IsNocturnal(mSeedType) && mBoard && !mBoard->StageIsNight() && !mIsPreview)
        SetSleeping(true);

    if (mLaunchRate > 0)
    {
        if (MakesSun())
            mLaunchCounter = RandRangeInt(300, mLaunchRate / 2);
        else
            mLaunchCounter = RandRangeInt(0, mLaunchRate);
    }
    else
        mLaunchCounter = 0;

    switch (theSeedType)
    {
    case SeedType::SEED_MAGNIFYINGGRASS:
    case SeedType::SEED_SEEDROOT:
    {
        mState = STATE_READY;
        break;
    }
    case SeedType::SEED_SUNBLOOM:
    {
        mDoSpecialCountdown = 250;
        mState = STATE_DOINGSPECIAL;
        break;
    }
    case SeedType::SEED_HURRIKALE:
    case SeedType::SEED_BLOVER:
    {
        mDoSpecialCountdown = 50;

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_blow");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aBodyReanim->mAnimRate = 20.0f;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimRate = 10.0f;
        }

        break;
    }
    case SeedType::SEED_STINGER:
    {
        if (aBodyReanim)
        {
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = 12;
            aHeadReanim->SetFramesForLayer("anim_wings");
            mBackbodyReanimID = mApp->ReanimationGetID(aHeadReanim);
            aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_face");
        }
        break;
    }
    case SeedType::SEED_SHOOTINGSTAR: 
    {
        if (aBodyReanim)
        {
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = 12;
            aHeadReanim->SetFramesForLayer("anim_spiral");
            mBackbodyReanimID = mApp->ReanimationGetID(aHeadReanim);
            aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    }
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_GATLINGPEA:
    {
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            if (aBodyReanim->TrackExists("anim_stem"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            else if (aBodyReanim->TrackExists("anim_idle"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");

            if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mSeedType == SEED_PEASHOOTER)
                mHeatWaveTimer = RandRangeInt(2000, 3000);

        }

        /*if (mSeedType == SEED_GATLINGPEA)
        {
            Reanimation* aBodyReanimOriginal = mApp->ReanimationGet(mBodyReanimID);
            aBodyReanimOriginal->ReanimationDie();

            Reanimation* aHeadReanimOriginal = mApp->ReanimationGet(mHeadReanimID);
            aHeadReanimOriginal->ReanimationDie();

            aBodyReanim = mApp->AddReanimation(0, 0, 0, REANIM_GATLINGPEA);
            mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
            aBodyReanim->PlayReanim("anim_idle", REANIM_LOOP, 0, 0);
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);

            aBodyReanim->AssignRenderGroupToTrack("stalk_top", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("stalk_bottom", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("backleaf_right_tip", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("backleaf_left_tip", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("backleaf", RENDER_GROUP_HIDDEN);

            Reanimation* aHeadReanim = mApp->AddReanimation(0, 0, 0, REANIM_GATLINGPEA);
            aHeadReanim->PlayReanim("anim_head_idle", REANIM_LOOP, 0, 10);
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head_idle");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 8, -4.5f + 22.5f, 0, 1, 1);

            if (mBoard->GetFlyingPotAt(mPlantCol, mRow))
            {
                mEnemy = true;
                mBoard->GetFlyingPotAt(mPlantCol, mRow)->mEnemy = true;
            }

            //aHeadReanim->mIsAttachment = true;
        }*/

        break;
    }
    case SeedType::SEED_SPLITPEA:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        break;
    }
    case SeedType::SEED_THREEPEATER:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle1");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_head1");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_head_idle2");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_head2");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        Reanimation* aHeadReanim3 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->SetFramesForLayer("anim_head_idle3");
        aHeadReanim3->AttachToAnotherReanimation(aBodyReanim, "anim_head3");
        mHeadReanimID3 = mApp->ReanimationGetID(aHeadReanim3);

        break;
    }
    case SeedType::SEED_DUMMY:
        mPlantHealth = 200;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_FIRESHROOM:
    {
        mPlantHealth = 2000;

        if (aBodyReanim && mBackbodyReanimID == REANIMATIONID_NULL && (mBoard && mBoard->StageIsNight() || !IsInPlay()))
        {
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = 12;
            aHeadReanim->SetFramesForLayer("anim_fire");
            mBackbodyReanimID = mApp->ReanimationGetID(aHeadReanim);
            //aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "Fireshroom_fire");

            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("FireShroom_cap");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 0, 0, 0, 1, 1);
        }
    
        break;
    }
    case SeedType::SEED_WALLNUT:
        mPlantHealth = 4000; // 6000
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_EXPLODE_O_NUT:
        mPlantHealth = 3000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        //aBodyReanim->mColorOverride = Color(255, 64, 64);
        break;
    case SeedType::SEED_ARTIC_NUT:
        mPlantHealth = 4000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GIANT_WALLNUT:
        mPlantHealth = 12000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_TALLNUT:
        mPlantHealth = 8000;
        mHeight = 80;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GARLIC:
        TOD_ASSERT(aBodyReanim);
        mPlantHealth = 400;
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_GOLD_MAGNET:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_IMITATER:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(25.0f, 30.0f);
        mStateCountdown = 200;
        break;
    case SeedType::SEED_CHERRYHOVERBOMB: 
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            mDoSpecialCountdown = 100;

            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        }

        aBodyReanim->AssignRenderGroupToPrefix("Cherrybomb_right1", RENDER_GROUP_HIDDEN);
        aBodyReanim->AssignRenderGroupToPrefix("CherryBomb_rightstem", RENDER_GROUP_HIDDEN);
        aBodyReanim->AssignRenderGroupToPrefix("CherryBomb_rightmouth1", RENDER_GROUP_HIDDEN);
        aBodyReanim->AssignRenderGroupToPrefix("CherryBomb_righteye21", RENDER_GROUP_HIDDEN);
        aBodyReanim->AssignRenderGroupToPrefix("CherryBomb_righteye11", RENDER_GROUP_HIDDEN);
        aBodyReanim->AssignRenderGroupToPrefix("CherryBomb_right3", RENDER_GROUP_HIDDEN);


        if (!mEnemy)
        {
            mX += 25;
        }
        else
        {
            mX -= 25;
        }
        
        break;
    }
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            mDoSpecialCountdown = 100;

            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        }

        break;
    }
    case SeedType::SEED_POTATOMINE:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = 12.0f;

        if (IsInPlay())
        {
            aBodyReanim->AssignRenderGroupToTrack("anim_glow", RENDER_GROUP_HIDDEN);
            mStateCountdown = 1500;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_armed");
            mState = PlantState::STATE_POTATO_ARMED;
        }

        break;
    }
    case SeedType::SEED_GRAVEBUSTER:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_land");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mState = PlantState::STATE_GRAVEBUSTER_LANDING;
            mApp->PlayFoley(FoleyType::FOLEY_GRAVEBUSTERCHOMP);
        }

        break;
    }
    case SeedType::SEED_SUNSHROOM:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mFrameBasePose = 6;

        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(10) - 5;
        }
        else if (mIsAsleep)
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        else
            aBodyReanim->SetFramesForLayer("anim_bigidle");

        mState = PlantState::STATE_SUNSHROOM_SMALL;
        mStateCountdown = 12000;

        break;
    }
    case SeedType::SEED_SPORE_SHROOM:
    case SeedType::SEED_SEASHROOM:
    {
        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(6) - 3;
        }
        break;
    }
    case SeedType::SEED_PUFFSHROOM:
    {
        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(6) - 3;
            mDoSpecialCountdown = -1;
            mState = STATE_READY;
        }
        break;
    }
    case SeedType::SEED_PUMPKINSHELL:
    {
        mPlantHealth = 4000;
        mWidth = 120;

        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToTrack("Pumpkin_back", 1);
        break;
    }
    case SeedType::SEED_BONKCHOY:
    {
        mPlantHealth = 1125;
        mState = PlantState::STATE_READY;
        break;
    }
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_PLANTERN:
    {
        mStateCountdown = 50;

        if (!mIsPreview)
        {
            if (!IsOnBoard() || mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN){
                AddAttachedParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_FOG + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
            }
            if (IsInPlay() && mApp && mApp->mGameScene == SCENE_PLAYING){
                mApp->PlaySample(Sexy::SOUND_PLANTERN);
            }
        }
        
        break;
    }
    case SeedType::SEED_PLASMAWOOD:
    case SeedType::SEED_TORCHWOOD:
    {
        mPlantHealth = 2000;
        break;
    }
    case SeedType::SEED_MARIGOLD:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        break;
    case SeedType::SEED_CACTUS:
        mState = PlantState::STATE_CACTUS_LOW;
        break;
    case SeedType::SEED_INSTANTCOFFEE:
        mDoSpecialCountdown = 100;
        break;
    case SeedType::SEED_BRAVEYSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_COBCANNON:
        if (IsInPlay())
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 500;

            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetFramesForLayer("anim_unarmed_idle");
            mIgnoreBlink = true;
        }
        break;
    case SeedType::SEED_KERNELPULT:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
        break;
    case SeedType::SEED_MAGNETSHROOM:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_SPIKEROCK:
        mPlantHealth = 450;
        TOD_ASSERT(aBodyReanim);
        break;
    case SeedType::SEED_SPROUT:
        break;
    case SeedType::SEED_WATERPOT:
    {
        if (IsInPlay())
        {
            mState = PlantState::STATE_FLOWERPOT_INVULNERABLE;
            mStateCountdown = 100;
        }
        aBodyReanim->SetFramesForLayer("anim_waterpot");
        break;
    }

    case SeedType::SEED_FLYINGPOT:
    {
        if (IsInPlay())
        {
            mState = PlantState::STATE_FLOWERPOT_INVULNERABLE;
            mStateCountdown = 100;
        }
        aBodyReanim->SetFramesForLayer("anim_airraid");
        break;
    }
    
    case SeedType::SEED_FLOWERPOT:
        if (IsInPlay())
        {
            mState = PlantState::STATE_FLOWERPOT_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_LILYPAD:
        if (IsInPlay())
        {
            mState = PlantState::STATE_LILYPAD_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_TANGLEKELP:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
    }
    
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (theSeedType == SeedType::SEED_WALLNUT || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_MARIGOLD))
    {
        mPlantHealth *= 2;
    }

    mPlantMaxHealth = mPlantHealth;

    if (mIsPreview)
        return;

    if (mSeedType != SeedType::SEED_FLOWERPOT && IsOnBoard() && !Plant::IsFlying(mSeedType))
    {
        TOD_ASSERT(mBoard);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot)
            mApp->ReanimationGet(aFlowerPot->mBodyReanimID)->mAnimRate = 0.0f;
    }

    if (mSeedType != SeedType::SEED_WATERPOT && IsOnBoard() && !Plant::IsFlying(mSeedType))
    {
        TOD_ASSERT(mBoard);
        Plant* aWaterPot = mBoard->GetWaterPotAt(mPlantCol, mRow);
        if (aWaterPot)
            mApp->ReanimationGet(aWaterPot->mBodyReanimID)->mAnimRate = 0.0f;
    }
}

//0x45E7C0
int Plant::CalcRenderOrder()
{
    PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
    RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

    SeedType aSeedType = mSeedType;
    if (mSeedType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
        aSeedType = mImitaterType;

    int rowOffset = 0;

    if (!mIsPreview && (mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (mSeedType == SEED_WALLNUT || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_GIANT_WALLNUT || mSeedType == SEED_EXPLODE_O_NUT)))
    {
        aLayer = RenderLayer::RENDER_LAYER_PROJECTILE;
    }
    else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_PUMPKIN;
    }
    else if (IsFlying(aSeedType))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_FLYER;

        if (aSeedType == SEED_INSTANTCOFFEE)
            rowOffset = 1;
    }
    else if ((aSeedType == SeedType::SEED_FLOWERPOT || aSeedType == SeedType::SEED_WATERPOT || aSeedType == SeedType::SEED_FLYINGPOT))
        anOrder = PLANT_ORDER::PLANT_ORDER_UNDERLILYPAD;
    else if (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;

    return Board::MakeRenderOrder(aLayer, mRow + rowOffset, anOrder * 5 - mX + 800);
}

//0x45E860
void Plant::SetSleeping(bool theIsAsleep)
{
    if (mIsAsleep == theIsAsleep || NotOnGround())
        return;

    mIsAsleep = theIsAsleep;
    if (theIsAsleep)
    {
        float aPosX = mX + 50.0f;
        float aPosY = mY + PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow) + 40.0f;
        if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM)
            aPosX += 12.0f;
        else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM)
            aPosY -= 20.0f;
        else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            aPosY -= 12.0f;

        Reanimation* aSleepReanim = mApp->AddReanimation(aPosX, aPosY, mRenderOrder + 2, ReanimationType::REANIM_SLEEPING);
        aSleepReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aSleepReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
        aSleepReanim->mAnimTime = RandRangeFloat(0.0f, 0.9f);
        mSleepingReanimID = mApp->ReanimationGetID(aSleepReanim);
    }
    else
    {
        mApp->RemoveReanimation(mSleepingReanimID);
        mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if (theIsAsleep)
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        }
        else if (aBodyReanim->TrackExists("anim_sleep"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_sleep");
            aBodyReanim->mAnimTime = aAnimTime;
        }
        else
        {
            aBodyReanim->mAnimRate = 1.0f;
        }

        EndBlink();
    }
    else
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigidle");
        }
        else if (aBodyReanim->TrackExists("anim_idle"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimTime = aAnimTime;
        }

        if (mSeedType == SEED_FIRESHROOM && mBackbodyReanimID == REANIMATIONID_NULL)
        {
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, REANIM_FIRESHROOM);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = 12;
            aHeadReanim->SetFramesForLayer("anim_fire");
            mBackbodyReanimID = mApp->ReanimationGetID(aHeadReanim);
           // aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "Fireshroom_fire");

            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("FireShroom_cap");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 0, 0, 0, 1, 1);
        }

        if (aBodyReanim->mAnimRate < 2.0f && IsInPlay())
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
    }
}

//0x45EB10
int Plant::GetDamageRangeFlags(PlantWeapon thePlantWeapon)
{
    switch (mSeedType)
    {
    case SeedType::SEED_CACTUS:
        return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 1 : 2;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_CHERRYHOVERBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_COBCANNON:
    case SeedType::SEED_DOOMSHROOM:
        return 127;
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_KERNELPULT:
    case SeedType::SEED_WINTERMELON:
    case SeedType::SEED_FROST_CABBAGE:
    case SeedType::SEED_SLINGPEA:
    case SeedType::SEED_SPORE_SHROOM:
    case SeedType::SEED_BOMBBERRY:
    case SeedType::SEED_SHOOTINGSTAR:
        return 13;
    case SeedType::SEED_POTATOMINE:
        return 77;
    case SeedType::SEED_SQUASH:
        return 13;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_ICYFUMESHROOM:
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_GLOOMSHROOM:
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
        return 9;
    case SeedType::SEED_CATTAIL:
        return 11;
    case SeedType::SEED_TANGLEKELP:
        return 5;
    case SeedType::SEED_GIANT_WALLNUT:
        return 17;
    default:
        return 1;
    }
}

//0x45EBD0
bool Plant::IsOnHighGround()
{
    return mBoard && mBoard->mGridSquareType[mPlantCol][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

//0x45EC00
void Plant::SpikeRockTakeDamage()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    SpikeweedAttack();

    mPlantHealth -= 50;
    if (mPlantHealth <= 300)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike3", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 150)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike2", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

bool Plant::IsSpiky()
{
    return mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK;
}

//0x45ED00
void Plant::DoRowAreaDamage(int theDamage, unsigned int theDamageFlags)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    if (mEnemy)
        aAttackRect.mX -= (mX - aAttackRect.mX) * 2;

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mEnemy == mEnemy)
            continue;

        int aDiffY = (aPlant->mRow - mRow);
        if (mSeedType == SeedType::SEED_GLOOMSHROOM)
        {
            if (aDiffY < -1 || aDiffY > 1)
                continue;
        }
        else if (aDiffY)
            continue;

        Rect aPlantRect = aPlant->GetPlantRect();
        if (GetRectOverlap(aAttackRect, aPlantRect) > 0)
        {

            aPlant->mPlantHealth -= theDamage;
            aPlant->mEatenFlashCountdown = 25;
            mApp->PlayFoley(FoleyType::FOLEY_SPLAT);

            if (mSeedType == SEED_ICYFUMESHROOM && !aPlant->CanBeChilled())
                aPlant->ApplyChill(false);
        }
        

    }
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        unsigned int damageFlag = aZombie->mZombieType == ZOMBIE_GLASSDOOR ? 0 : theDamageFlags;

        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (mSeedType == SeedType::SEED_GLOOMSHROOM)
        {
            if (aDiffY < -1 || aDiffY > 1)
                continue;
        }
        else if (aDiffY)
            continue;

        if (aZombie->mOnHighGround == IsOnHighGround() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
            {
                int aDamage = theDamage;
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_ZAMBONI || aZombie->mZombieType == ZombieType::ZOMBIE_CATAPULT) &&
                    (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE)))
                {
                    aDamage = 1800;

                    if (mSeedType == SeedType::SEED_SPIKEROCK)
                    {
                        SpikeRockTakeDamage();
                    }
                    else
                    {
                        Die();
                    }
                }

                aZombie->TakeDamage(aDamage, damageFlag);
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);

                if (mSeedType == SEED_ICYFUMESHROOM && damageFlag == theDamageFlags)
                    aZombie->ApplyChill(false);
            }
        }
    }
}

//0x45EEA0
TodParticleSystem* Plant::AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect)
{
    TodParticleSystem* aParticle = mApp->ParticleTryToGet(mParticleID);

    if (aParticle)
        aParticle->ParticleSystemDie();

    TodParticleSystem* aNewParticle = mApp->AddTodParticle(thePosX, thePosY, theRenderPosition, theEffect);
    if (aNewParticle)
        mParticleID = mApp->ParticleGetID(aNewParticle);

    return aNewParticle;
}

//0x45EF10
bool Plant::FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon)
{
    Zombie* aZombie = FindTargetZombie(theRow, thePlantWeapon);
    Plant* aPlant = FindTargetPlant(theRow, thePlantWeapon);

    if (aZombie == nullptr && aPlant == nullptr)
        return false;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

    if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        aHeadReanim2->StartBlend(20);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 35.0f;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_shooting");
        mShootingCounter = 26;
    }
    else if (aHeadReanim && aHeadReanim->TrackExists("anim_shooting"))
    {
        aHeadReanim->StartBlend(20);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim->mAnimRate = 35.0f;
        aHeadReanim->SetFramesForLayer("anim_shooting");

        mShootingCounter = 33;
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_LEFTPEATER)
        {
            aHeadReanim->mAnimRate = 45.0f;
            mShootingCounter = 26;
        }
        else if (mSeedType == SeedType::SEED_GATLINGPEA)
        {
            aHeadReanim->mAnimRate = 38.0f;
            mShootingCounter = 100;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        PlayBodyReanim("anim_shootinghigh", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mShootingCounter = 23;
    }
    else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 14.0f);
        mShootingCounter = 200;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        mShootingCounter = 50;
    }
    else if (aBodyReanim && aBodyReanim->TrackExists("anim_shooting"))
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);

        switch (mSeedType)
        {
        case SeedType::SEED_ICYFUMESHROOM:
        case SeedType::SEED_FUMESHROOM:     mShootingCounter = 50;  break;
        case SeedType::SEED_PUFFSHROOM:     mShootingCounter = 29;  break;
        case SeedType::SEED_SPORE_SHROOM:
        case SeedType::SEED_BRAVEYSHROOM:
        case SeedType::SEED_SCAREDYSHROOM:  mShootingCounter = 25;  break;
        case SeedType::SEED_BOMBBERRY:
        case SeedType::SEED_FROST_CABBAGE:
        case SeedType::SEED_CABBAGEPULT:    mShootingCounter = 32;  break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:    mShootingCounter = 36;  break;
        case SeedType::SEED_KERNELPULT:
        {
            if (Sexy::Rand(4) == 0)
            {
                aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_NORMAL);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_HIDDEN);
                mState = PlantState::STATE_KERNELPULT_BUTTER;
            }

            mShootingCounter = 30;
            break;
        }
        case SeedType::SEED_CACTUS:         mShootingCounter = 35;  break;
        default:                            mShootingCounter = 29;  break;
        }
    }
    else{
        void* target = aZombie;
        if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
            target = aPlant;
        Fire(target, theRow, thePlantWeapon);
    }

    return true;
}

//0x45F2A0
void Plant::LaunchThreepeater()
{
    int rowAbove = mRow - 1;
    int rowBelow = mRow + 1;

    if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) ||
        (mBoard->RowCanHaveZombies(rowAbove) && (FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY) || FindTargetPlant(rowAbove, PlantWeapon::WEAPON_PRIMARY))) ||
        (mBoard->RowCanHaveZombies(rowBelow) && (FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY) || FindTargetPlant(rowBelow, PlantWeapon::WEAPON_PRIMARY))))
    {
        Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (mBoard->RowCanHaveZombies(rowBelow))
        {
            aHeadReanim1->StartBlend(10);
            aHeadReanim1->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim1->mAnimRate = 20.0f;
            aHeadReanim1->SetFramesForLayer("anim_shooting1");
        }

        aHeadReanim2->StartBlend(10);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 20.0f;
        aHeadReanim2->SetFramesForLayer("anim_shooting2");

        if (mBoard->RowCanHaveZombies(rowAbove))
        {
            aHeadReanim3->StartBlend(10);
            aHeadReanim3->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim3->mAnimRate = 20.0f;
            aHeadReanim3->SetFramesForLayer("anim_shooting3");
        }

        mShootingCounter = 35;
    }
}

void Plant::LaunchSlingpea()
{
    int maxRows = mBoard->StageHas6Rows() ? 6 : 5;
    int shootCounter = 3;
    while (shootCounter > 0) {
        Zombie* zombiesPerRow[6] = { nullptr };
        int rowC = 0;
        if (rand() % 2 == 0) {
            for (int r = mRow; r >= 0; --r) {
                zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                rowC++;
            }

            for (int r = mRow + 1; r < maxRows; ++r) {
                zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                rowC++;
            }
        }
        else {
            for (int r = mRow; r < maxRows; ++r) {
                zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                rowC++;
            }

            for (int r = mRow - 1; r >= 0; --r) {
                zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                rowC++;
            }
        }

        bool zombiesExists = false;
        int index = 0;
        while (shootCounter > 0 && index < maxRows)
        {
            Zombie* tZombie = zombiesPerRow[index];
            if (tZombie == nullptr || tZombie->mDead || tZombie->mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || tZombie->mZombiePhase == PHASE_POLEVAULTER_IN_VAULT || tZombie->mZombiePhase == PHASE_BALLOON_POPPING)
            {
                ++index;
                continue;
            }
            --shootCounter;
            zombiesExists = true;
            ++index;
        }

        if (!zombiesExists) break;

        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mApp->PlayFoley(FOLEY_SLINGPEALOB);
        mShootingCounter = 32;
    }
}

//0x45F470
bool Plant::FindStarFruitTarget()
{
    if (mRecentlyEatenCountdown > 0)
        return true;

    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    int aCenterStarX = mX + 40;
    int aCenterStarY = mY + 40;

    if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
    {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mEnemy == mEnemy)   continue;

            Rect aPlantRect = aPlant->GetPlantRect();

            if (aPlant->mRow == mRow)
            {
                if (aPlant->mEnemy)
                {
                    if (aPlantRect.mX - aPlantRect.mWidth < aCenterStarX)
                        return true;
                }
                else
                {
                    if (aPlantRect.mX + aPlantRect.mWidth < aCenterStarX)
                        return true;
                }
            }
            else
            {
                float aProjectileTime = Distance2D(aCenterStarX, aCenterStarY, aPlantRect.mX + aPlantRect.mWidth / 2, aPlantRect.mY + aPlantRect.mHeight / 2) / 3.33f;
                int aPlantHitX = 0;

                if (aPlant->mEnemy)
                    aPlantHitX = aPlant->mX + aPlantRect.mWidth / 2;
                else
                    aPlantHitX = aPlant->mX - aPlantRect.mWidth / 2;

                if ((aPlantHitX + aPlantRect.mWidth > aCenterStarX) && (aPlantHitX < aCenterStarX))
                    return true;

                int aCenterPlantX = 0;
                if (aPlant->mEnemy)
                    aCenterPlantX = aPlantHitX - aPlantRect.mWidth / 2;
                else
                    aCenterPlantX = aPlantHitX + aPlantRect.mWidth / 2;
                int aCenterPlantY = aPlantRect.mY + aPlantRect.mHeight / 2;
                float angle = RAD_TO_DEG(atan2(aCenterPlantY - aCenterStarY, aCenterPlantX - aCenterStarX));
                if (abs(aPlant->mRow - mRow) < 2)
                {
                    if ((angle > 20.0f && angle < 40.0f) || (angle < -25.0f && angle > -45.0f))
                        return true;
                }
                else
                {
                    if ((angle > 25.0f && angle < 35.0f) || (angle < -28.0f && angle > -38.0f))
                        return true;
                }
            }

        }
    }
    else {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (aZombie->EffectedByDamage(aDamageRangeFlags))
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS && mPlantCol >= 5)
                    return true;

                if (aZombie->mRow == mRow)
                {
                    if (aZombieRect.mX + aZombieRect.mWidth < aCenterStarX)
                        return true;
                }
                else
                {
                    if (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
                        aZombieRect.mX += 10;

                    float aProjectileTime = Distance2D(aCenterStarX, aCenterStarY, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2) / 3.33f;
                    int aZombieHitX = aZombie->ZombieTargetLeadX(aProjectileTime) - aZombieRect.mWidth / 2;
                    if ((aZombieHitX + aZombieRect.mWidth > aCenterStarX) && (aZombieHitX < aCenterStarX))
                        return true;

                    int aCenterZombieX = aZombieHitX + aZombieRect.mWidth / 2;
                    int aCenterZombieY = aZombieRect.mY + aZombieRect.mHeight / 2;
                    float angle = RAD_TO_DEG(atan2(aCenterZombieY - aCenterStarY, aCenterZombieX - aCenterStarX));
                    if (abs(aZombie->mRow - mRow) < 2)
                    {
                        if ((angle > 20.0f && angle < 40.0f) || (angle < -25.0f && angle > -45.0f))
                            return true;
                    }
                    else
                    {
                        if ((angle > 25.0f && angle < 35.0f) || (angle < -28.0f && angle > -38.0f))
                            return true;
                    }
                }
            }
        }
    }

    return false;
}
//0x45F6E0
void Plant::LaunchStarFruit()
{
    if (FindStarFruitTarget())
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
}

//0x45F720
void Plant::StarFruitFire()
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);

    float aShootAngleX = cos(DEG_TO_RAD(30.0f)) * 3.33f;
    float aShootAngleY = sin(DEG_TO_RAD(30.0f)) * 3.33f;

    for (int i = 0; i < 5; i++)
    {
        ProjectileType projType = PROJECTILE_STAR;
        if (mSeedType == SEED_STARCICLE)
            projType = PROJECTILE_STARCICLE;

        float aOriginY = mY + 25;
        aOriginY += PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

        Projectile* aProjectile = mBoard->AddProjectile(mX + (mEnemy ? 12.5f : 25), aOriginY, mRenderOrder - 1, mRow, projType);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
        aProjectile->mEnemy = aProjectile->mBackwards = mEnemy;

        switch (i)
        {
        case 0:     aProjectile->mVelX = -3.33f * (mEnemy ? -1 : 1);         aProjectile->mVelY = 0.0f;             break;
        case 1:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = 3.33f;             break;
        case 2:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = -3.33f;            break;
        case 3:     aProjectile->mVelX = aShootAngleX * (mEnemy ? -1 : 1);  aProjectile->mVelY = aShootAngleY;      break;
        case 4:     aProjectile->mVelX = aShootAngleX * (mEnemy ? -1 : 1);  aProjectile->mVelY = -aShootAngleY;     break;
        default:    TOD_ASSERT();                                                               break;
        }
    }
}

//0x45F8A0
void Plant::UpdateShooter()
{
    if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mSeedType == SEED_PEASHOOTER)
    {
        if (mState == STATE_EXTREMELY_DEHYDRATED || mState == STATE_DEHYDRATED && mPlantAge % 2 == 0)
            return;
    }

    mLaunchCounter--;

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mBerserkCounter > 0)
    {
        for (int i = 0; i < 3; i++) 
            mLaunchCounter--;
    }

    if (mLaunchCounter <= 0)
    {
        mLaunchCounter = mLaunchRate - Sexy::Rand(15);

        if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            LaunchThreepeater();
        }
        else if (mSeedType == SeedType::SEED_SLINGPEA)
        {
            LaunchSlingpea();
        }
        else if (mSeedType == SeedType::SEED_SHOOTINGSTAR)
        {
            bool doAttack = false;
            for (int gridY = 0; gridY < MAX_GRID_SIZE_Y; gridY++) {
                Zombie* aZombie = FindTargetZombie(gridY, WEAPON_SECONDARY);
                if (aZombie) {
                    doAttack = true;
                    break;
                }
            }
            if (doAttack)
            {
                PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
                mShootingCounter = 40;
            }
        }
        else if (mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_STARCICLE)
        {
            LaunchStarFruit();
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_CACTUS)
        {
            if (mState == PlantState::STATE_CACTUS_HIGH)
            {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            else if (mState == PlantState::STATE_CACTUS_LOW)
            {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }
        else 
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }

    if (mLaunchCounter == 50 && mSeedType == SeedType::SEED_CATTAIL)
    {
        FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
    }
    if (mLaunchCounter == 25)
    {
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
    }
}

//0x45F980
bool Plant::MakesSun()
{
    return mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_PRIMAL_SUNFLOWER || mSeedType == SeedType::SEED_MOONFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_SUNSHROOM;
}

bool Plant::MakesSun(SeedType mSeedType)
{
    return mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_SUNSHROOM;
}

bool Plant::IsShooter(SeedType mSeedType)
{
    return  mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_REPEATER ||  mSeedType == SeedType::SEED_SNOWPEA ||
        mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_FUMESHROOM ||
        mSeedType == SeedType::SEED_THREEPEATER ||
        mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_STARFRUIT ||
        mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT;
}

bool Plant::IsRoofShooter(SeedType mSeedType)
{
    return mSeedType == SeedType::SEED_FUMESHROOM ||
        mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT;
}

bool Plant::IsInsta(SeedType mSeedType)
{
    return mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_CHOMPER ||
        mSeedType == SeedType::SEED_HYPNOSHROOM || mSeedType == SeedType::SEED_ICESHROOM || mSeedType == SeedType::SEED_DOOMSHROOM ||
        mSeedType == SeedType::SEED_SQUASH;
}

bool Plant::IsTank(SeedType mSeedType)
{
    return mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_TORCHWOOD || mSeedType == SEED_FIRESHROOM;
}

//0x45F9A0
void Plant::UpdateProductionPlant()
{
    if (!IsInPlay() || mApp->IsIZombieLevel() || mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO)
        return;

    if (mBoard->HasLevelAwardDropped())
        return;

    if (mButteredCounter > 0 || mIceTrapCounter > 0)
        return;

    if (mSeedType == SeedType::SEED_MARIGOLD && mBoard->mCurrentWave == mBoard->mNumWaves)
    {
        if (mState != PlantState::STATE_MARIGOLD_ENDING)
        {
            mState = PlantState::STATE_MARIGOLD_ENDING;
            mStateCountdown = 6000;
        }
        else if (mStateCountdown <= 0)
            return;
    }

    if (LawnApp::IsLastStandLevel(mApp->mGameMode) && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT)
        return;

    mLaunchCounter--;
    if (mLaunchCounter <= 100)
    {
        int aFlashCountdown = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
        mEatenFlashCountdown = max(mEatenFlashCountdown, aFlashCountdown);
    }

    if (mSeedType == SEED_MOONFLOWER )
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim)
        {
            if (mLaunchCounter == 80)
            {
                aBodyReanim->PlayReanim("anim_bloom", REANIM_PLAY_ONCE_AND_HOLD, 20, 35);
            }
            else if (aBodyReanim->mLoopCount > 0)
            {
                aBodyReanim->PlayReanim("anim_idle", REANIM_PLAY_ONCE_AND_HOLD, 20, RandRangeFloat(10.0f, 15.0f) * 2);
            }
        }
    }

    if (mSeedType == SEED_AMPLIFLOWER ) {
        if (mState == STATE_SINGING && mSingingCounter == 0)
            mState = STATE_READY;
    }

    if (mLaunchCounter <= 0)
    {
        mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

        if (mSeedType == SeedType::SEED_SUNSHROOM)
        {
            if (mState == PlantState::STATE_SUNSHROOM_SMALL)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
        }
        if (mSeedType == SeedType::SEED_MOONFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_SUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_PRIMAL_SUNFLOWER) {
            mBoard->AddCoin(mX, mY, CoinType::COIN_LARGESUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(mX, mY, (Sexy::Rand(100) < 10) ? CoinType::COIN_GOLD : CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
        }
        else if (mSeedType == SeedType::SEED_AMPLIFLOWER)
        {
            bool allowedToSing = false;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++) 
                {
                    if (j == 0 && i == 0) continue;

                    Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol + i, mRow + j, TOPPLANT_ONLY_NORMAL_POSITION);

                    if (aPlant && aPlant->mSeedType != SEED_AMPLIFLOWER){
                        aPlant->mAmplifiedCounter = 750;
                        aPlant->mPlantAmplifierID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
                        allowedToSing = true;
                    }
                }
            }

            if (allowedToSing) {
                mSingingCounter = 750;
                mState = STATE_SINGING;
            }
        }

        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME)
        {
            if (mSeedType == SeedType::SEED_SUNFLOWER)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else if (mSeedType == SeedType::SEED_MARIGOLD)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
        }
    }
}

//0x45FB70
void Plant::UpdateSunShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SUNSHROOM_SMALL)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
            mState = PlantState::STATE_SUNSHROOM_GROWING;
            mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
        }

        UpdateProductionPlant();
    }
    else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_bigidle", ReanimLoopType::REANIM_LOOP, 10, RandRangeFloat(12.0f, 15.0f));
            mState = PlantState::STATE_SUNSHROOM_BIG;
        }
    }
    else
    {
        UpdateProductionPlant();
    }
}

//0x45FC70
void Plant::UpdateGraveBuster()
{
    if (mState == PlantState::STATE_GRAVEBUSTER_LANDING)
    {
        if (mApp->ReanimationGet(mBodyReanimID)->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
            mStateCountdown = 400;
            mState = PlantState::STATE_GRAVEBUSTER_EATING;
            AddAttachedParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER);
        }
    }
    else if (mState == PlantState::STATE_GRAVEBUSTER_EATING && mStateCountdown == 0)
    {
        GridItem* aGraveStone = mBoard->GetGraveStoneAt(mPlantCol, mRow);
        if (aGraveStone)
        {
            aGraveStone->GridItemDie();
            mBoard->mGravesCleared++;
        }

        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER_DIE);
        Die();
        mBoard->DropLootPiece(mX + 40, mY, 12);
    }
}

//0x45FD90
void Plant::PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (theBlendTime > 0)
        aBodyReanim->StartBlend(theBlendTime);
    if (theAnimRate > 0.0f)
        aBodyReanim->mAnimRate = theAnimRate;

    aBodyReanim->mLoopType = theLoopType;
    aBodyReanim->mLoopCount = 0;
    aBodyReanim->SetFramesForLayer(theTrackName);
}

//0x45FE20
void Plant::UpdatePotato()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (mState == PlantState::STATE_NOTREADY)
    {
        if (mStateCountdown == 0)
        {
            mApp->AddTodParticle(mX + mWidth / 2, mY + mHeight / 2, mRenderOrder, ParticleEffect::PARTICLE_POTATO_MINE_RISE);
            PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
            mState = PlantState::STATE_POTATO_RISING;
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
        }
    }
    else if (mState == PlantState::STATE_POTATO_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            float aRate = RandRangeFloat(12.0f, 15.0f);
            PlayBodyReanim("anim_armed", ReanimLoopType::REANIM_LOOP, 0, aRate);

            Reanimation* aLightReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, GetPlantDefinition(mSeedType).mReanimationType);
            aLightReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aLightReanim->mAnimRate = aRate - 2.0f;
            aLightReanim->SetFramesForLayer("anim_glow");
            aLightReanim->mFrameCount = 10;
            aLightReanim->ShowOnlyTrack("anim_glow");
            aLightReanim->SetTruncateDisappearingFrames("anim_glow", false);
            mLightReanimID = mApp->ReanimationGetID(aLightReanim);
            aLightReanim->AttachToAnotherReanimation(aBodyReanim, "anim_light");

            mState = PlantState::STATE_POTATO_ARMED;
            mBlinkCountdown = 400 + Sexy::Rand(4000);
        }
    }
    else if (mState == PlantState::STATE_POTATO_ARMED)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            DoSpecial();
        }
        else
        {
            Reanimation* aLightReanim = mApp->ReanimationTryToGet(mLightReanimID);
            if (aLightReanim)
            {
                aLightReanim->mFrameCount = TodAnimateCurve(200, 50, DistanceToClosestZombie(), 10, 3, TodCurves::CURVE_LINEAR);
            }
        }
    }
}

//0x460060
void Plant::UpdateTanglekelp()
{
    if (mState != PlantState::STATE_TANGLEKELP_GRABBING)
    {
        Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
        if (aZombie)
        {
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
            mState = PlantState::STATE_TANGLEKELP_GRABBING;
            mStateCountdown = 100;
            aZombie->PoolSplash(false);

            float aVinesPosX = -13.0f;
            float aVinesPosY = 15.0f;
            if (aZombie->mZombieType == ZombieType::ZOMBIE_SNORKEL)
            {
                aVinesPosX = -43.0f;
                aVinesPosY = 55.0f;
            }
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aVinesPosX = -20.0f;
                aVinesPosY = 37.0f;
            }
            Reanimation* aGrabReanim = aZombie->AddAttachedReanim(aVinesPosX, aVinesPosY, ReanimationType::REANIM_TANGLEKELP);
            if (aGrabReanim)
            {
                aGrabReanim->SetFramesForLayer("anim_grab");
                aGrabReanim->mAnimRate = 24.0f;
                aGrabReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            }

            mTargetZombieID = mBoard->ZombieGetID(aZombie);
        }
    }
    else
    {
        if (mStateCountdown == 50)
        {
            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                if (aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth < 1800)
                    aZombie->DragUnder();

                aZombie->PoolSplash(false);
            }
        }

        if (mStateCountdown == 20)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 23, mY + 7, aRenderPosition, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.3f, 1.3f);

            mApp->AddTodParticle(mX + 31, mY + 64 - PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow), aRenderPosition, ParticleEffect::PARTICLE_PLANTING_POOL);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (mStateCountdown == 0)
        {
            Die();

            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                if (aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth > 1800)
                {
                    aZombie->TakeDamage(1800, 0U);
                }
                else
                {
                    aZombie->DieWithLoot();
                }
            }
        }
    }
}

//0x460320
void Plant::SpikeweedAttack()
{
    TOD_ASSERT(IsSpiky());

    if (mState != PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
        mApp->PlaySample(SOUND_THROW);
        
        mState = PlantState::STATE_SPIKEWEED_ATTACKING;
        mStateCountdown = 100;
    }
}

//0x460370
void Plant::UpdateSpikeweed()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_NOTREADY;
        }
        else if (mSeedType == SeedType::SEED_SPIKEROCK)
        {
            if (mStateCountdown == 69 || mStateCountdown == 33)
            {
                DoRowAreaDamage(20, 33U);
            }
        }
        else if (mStateCountdown == 75)
        {
            DoRowAreaDamage(20, 33U);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(RandRangeFloat(12.0f, 15.0f));
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        SpikeweedAttack();
    }
}

//0x460420
void Plant::UpdateScaredyShroom()
{
    if (mShootingCounter > 0)
        return;

    bool aHasZombieNearby = false;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying() && aDiffY <= 0 && GetCircleRectOverlap(mX, mY + 20.0f, 120, aZombieRect))
        {
            aHasZombieNearby = true;
            break;
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_LOWERING;
            PlayBodyReanim("anim_scared", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 10.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_SCARED;
            PlayBodyReanim("anim_scaredidle", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_SCARED)
    {
        if (!aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_RAISING;

            float aAnimRate = RandRangeFloat(7.0f, 12.0f);
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, aAnimRate);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_RAISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayIdleAnim(aAnimRate);
        }
    }

    if (mState != PlantState::STATE_READY)
    {
        mLaunchCounter = mLaunchRate;
    }
}

//0x460610
void Plant::UpdateTorchwood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if (mEnemy != aProjectile->mEnemy)
            continue;

        if (aProjectile->mRow == mRow && 
             (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA || aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToFireball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToPea(mPlantCol);
                }
            }
        }
    }
}

void Plant::UpdatePlasmawood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if (aProjectile->mRow == mRow &&
            (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL || aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA || aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL || aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToPlasmaball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToFireball(mPlantCol);
                }
            }
        }
    }
}


//0x4606F0
void Plant::DoSquashDamage()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > (aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL ? -20 : 0))
            {
                aZombie->TakeDamage(1800, aZombie->mZombieType == ZOMBIE_GLASSDOOR ? 0 : 18U);
            }
        }
    }
}

//0x4607E0
Zombie* Plant::FindSquashTarget()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    int aClosestRange = 0;
    Zombie* aClosestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {

        if (IsOnHighGround() != aZombie->IsOnHighGround())
        {
            continue;
        }

        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) &&
            aZombie->mHasHead && !aZombie->IsTangleKelpTarget() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();

            if ((
                aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && aZombieRect.mX < mX + 20
                ) || (
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_IN_VAULT &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_SNORKEL_INTO_POOL &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_INTO_POOL &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_RIDING &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_IN_JUMP &&
                    !aZombie->IsBobsledTeamWithSled()
                    ))
            {
                int aRange = -GetRectOverlap(aAttackRect, aZombieRect);
                if (aRange <= (aZombie->mIsEating ? 110 : 70))
                {
                    int aPlantX = aAttackRect.mX;
                    if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_POST_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT ||
                        aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || aZombie->mZombieType == ZombieType::ZOMBIE_IMP ||
                        aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL || mApp->IsScaryPotterLevel())
                    {
                        aPlantX = aAttackRect.mX - 60;
                    }

                    if (aZombie->IsWalkingBackwards()  || aZombieRect.mX + aZombieRect.mWidth >= aPlantX || aZombieRect.mX <= aPlantX + aAttackRect.mWidth)
                    {
                        if (mBoard->ZombieGetID(aZombie) == mTargetZombieID)
                            return aZombie;  // 是锁定的目标僵尸，则直接返回该僵尸

                        if (aClosestZombie == nullptr || aRange < aClosestRange)
                        {
                            aClosestZombie = aZombie;
                            aClosestRange = aRange;
                        }
                    }
                }
            }
        }
    }

    return aClosestZombie;
}

void Plant::UpdateCherryHoverBomb() {
    mY = mBoard->GridToPixelY(mPlantCol, mRow);
    mY -= TodAnimateCurve(100, 0, mDoSpecialCountdown, 100, 0, CURVE_EASE_IN);
}

void Plant::UpdateExplodeONut() {
    if (mExplodePulse > 0)
        mExplodePulse--;
    if (mExplodePulse == 0 && mPlantHealth < mPlantMaxHealth / 3){
        mExplodePulse = 100;
    }
}

void Plant::UpdateSeedRoot() {
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim) {
        if (mState == STATE_SEEDROOT_DIVING && aBodyReanim->mLoopCount > 0) {
            PlayBodyReanim("anim_digging", REANIM_PLAY_ONCE_AND_HOLD, 0, 20);
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
            float mHeightOffset = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
            mApp->AddTodParticle(mX + 44, mY + mHeightOffset + 74, mRenderOrder + 1, ParticleEffect::PARTICLE_DIGGER_RISE);
            Reanimation* aDirtReanim = mApp->AddReanimation(mX - 3, mY + mHeightOffset + 53, mRenderOrder + 1, ReanimationType::REANIM_DIGGER_DIRT);
            aDirtReanim->mAnimRate = 24.0f;
            mState = STATE_SEEDROOT_DIGGING;
            mIgnoreBlink = true;
        }
        else if (mState == STATE_SEEDROOT_DIGGING) {
            EndBlink();
            if (mRootTime == mRootMaxTime) {
                mIgnoreBlink = false;
                PlayBodyReanim("anim_idle", REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
                mState = STATE_READY;
            }
            else if (aBodyReanim->mLoopCount > 0) {
                PlayBodyReanim("anim_digging", REANIM_PLAY_ONCE_AND_HOLD, 10, 20);
                mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
                float mHeightOffset = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
                mApp->AddTodParticle(mX + 44, mY + mHeightOffset + 74, mRenderOrder + 1, ParticleEffect::PARTICLE_DIGGER_RISE);
                Reanimation* aDirtReanim = mApp->AddReanimation(mX - 3, mY + mHeightOffset + 53, mRenderOrder + 1, ReanimationType::REANIM_DIGGER_DIRT);
                aDirtReanim->mAnimRate = 24.0f;
            }
        }
    }
}

void Plant::UpdateArticNut() {
    if (mPlantAge % 200 == 0) {
        float pY = mY + 90 + RandRangeFloat(-25, 0);
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow + 1, 0);
        mApp->AddTodParticle(mX + 45 + RandRangeFloat(-50, 50), pY, aRenderPosition, ParticleEffect::PARTICLE_ICEBALL_TRAIL);
    }

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie)) {
        if (GetCircleRectOverlap(mX, mY, 115, aZombie->GetZombieRect()) && aZombie->mRow == mRow && aZombie->EffectedByDamage(127U)){
            aZombie->ApplyChill(false);
        }
    }
}

void Plant::UpdateIcebergLettuce() {

    if (mState == PlantState::STATE_DOINGSPECIAL)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        int aRowDist = aZombie->mRow - mRow;

        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
            aRowDist = 0;

        if (aRowDist == 0 && GetCircleRectOverlap(mX + mWidth / 2.0f, mY + mHeight / 2.0f, 30, aZombieRect))
        {
            mIgnoreBlink = true;
            mState = STATE_DOINGSPECIAL;
            mDoSpecialCountdown = 100;
            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mAnimRate = 12;
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        }        
    }
    
}

//0x4609D0
void Plant::UpdateSquash()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    if (mState == PlantState::STATE_NOTREADY)
    {
        Zombie* aZombie = FindSquashTarget();
        if (aZombie)
        {
            mTargetZombieID = mBoard->ZombieGetID(aZombie);
            mTargetX = aZombie->ZombieTargetLeadX(0.0f) - mWidth / 2;
            mState = PlantState::STATE_SQUASH_LOOK;
            mStateCountdown = 80;
            PlayBodyReanim(mTargetX < mX ? "anim_lookleft" : "anim_lookright", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
        }
    }
    else if (mState == PlantState::STATE_SQUASH_LOOK)
    {
        if (mStateCountdown <= 0)
        {
            PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
            mStateCountdown = 30;
        }
    }
    else if (mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (mStateCountdown <= 0)
        {
            Zombie* aZombie = FindSquashTarget();
            if (aZombie)
            {
                mTargetX = aZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
            }

            mState = PlantState::STATE_SQUASH_RISING;
            mStateCountdown = 50;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        }
    }
    else
    {
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
        int aDestY = mBoard->GridToPixelY(aTargetCol, mRow) + 8;

        if (mState == PlantState::STATE_SQUASH_RISING)
        {
            mX = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mStartRow), mTargetX, TodCurves::CURVE_EASE_IN_OUT);
            mY = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelY(mPlantCol, mStartRow), aDestY - 120, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 0)
            {
                PlayBodyReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
                mState = PlantState::STATE_SQUASH_FALLING;
                mStateCountdown = 10;
            }
        }
        else if (mState == PlantState::STATE_SQUASH_FALLING)
        {
            mY = TodAnimateCurve(10, 0, mStateCountdown, aDestY - 120, aDestY, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 5)
            {
                DoSquashDamage();
            }

            if (mStateCountdown == 0)
            {
                if (mBoard->IsPoolSquare(aTargetCol, mRow))
                {
                    mApp->AddReanimation(mX - 11, mY + 20, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    mApp->PlaySample(SOUND_ZOMBIESPLASH);

                    Die();
                }
                else
                {
                    mState = PlantState::STATE_SQUASH_DONE_FALLING;
                    mStateCountdown = 100;

                    mBoard->ShakeBoard(1, 4);
                    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                    float aOffsetY = mBoard->StageHasRoof() ? 69.0f : 80.0f;
                    mApp->AddTodParticle(mX + 40, mY + aOffsetY, mRenderOrder + 4, ParticleEffect::PARTICLE_DUST_SQUASH);
                }
            }
        }
        else if (mState == PlantState::STATE_SQUASH_DONE_FALLING)
        {
            if (mStateCountdown == 0)
            {
                Die();
            }
        }
    }
}

//0x460DD0
void Plant::UpdateDoomShroom()
{
    if (mIsAsleep || mState == PlantState::STATE_DOINGSPECIAL)
        return;

    mState = PlantState::STATE_DOINGSPECIAL;
    mDoSpecialCountdown = 100;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    aBodyReanim->SetFramesForLayer("anim_explode");
    aBodyReanim->mAnimRate = 23.0f;
    aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    aBodyReanim->SetShakeOverride("DoomShroom_head1", 1.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head2", 2.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head3", 2.0f);
    mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
}

void Plant::UpdateIceShroom()
{
    if (!mIsAsleep && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        mDoSpecialCountdown = 100;
    }
}

void Plant::UpdateEinsteinApple()
{
    if (!mIsAsleep && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        mDoSpecialCountdown = 100;
    }
}

//0x460F00
void Plant::UpdateBlover()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }

    if (mState != PlantState::STATE_DOINGSPECIAL && mStateCountdown == 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_WINDNESDAY && aBodyReanim->ShouldTriggerTimedEvent(0.5f))
    {
        DoSpecial();
    }
}

void Plant::UpdateHurrikale()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }

    if (mState != PlantState::STATE_DOINGSPECIAL && mStateCountdown == 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_WINDNESDAY && aBodyReanim->ShouldTriggerTimedEvent(0.5f))
    {
        DoSpecial();
    }
}

void Plant::UpdateFlowerPot()
{
    if (mState == PlantState::STATE_FLOWERPOT_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

void Plant::UpdateLilypad()
{
    if (mState == PlantState::STATE_LILYPAD_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

//0x460F60
void Plant::UpdateCoffeeBean()
{
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            Die();
        }
    }
}

//0x460FA0
void Plant::UpdateUmbrella()
{
    if (mState == PlantState::STATE_UMBRELLA_TRIGGERED)
    {
        if (mStateCountdown == 0)
        {
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
            mState = PlantState::STATE_UMBRELLA_REFLECTING;
        }
    }
    else if (mState == PlantState::STATE_UMBRELLA_REFLECTING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(0.0f);
            mState = PlantState::STATE_NOTREADY;
            mRenderOrder = CalcRenderOrder();
        }
    }
}

//0x461020
void Plant::UpdateCobCannon()
{
    if (mState == PlantState::STATE_COBCANNON_ARMING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_COBCANNON_LOADING;
            mIgnoreBlink = true;
            EndBlink();
            PlayBodyReanim("anim_charge", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_LOADING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.5f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_SHOOP);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_READY;
            PlayIdleAnim(12.0f);
            mIgnoreBlink = false;
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_READY)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aCobTrack = aBodyReanim->GetTrackInstanceByName("CobCannon_cob");
        aCobTrack->mTrackColor = GetFlashingColor(mBoard->mMainCounter, 75);
    }
    else if (mState == PlantState::STATE_COBCANNON_FIRING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.48f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_COB_LAUNCH);
        }
    }
}

//0x4611F0
void Plant::UpdateCactus()
{
    if (mShootingCounter > 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_CACTUS_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_HIGH;
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mLaunchCounter = 1;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) == nullptr)
        {
            mState = PlantState::STATE_CACTUS_LOWERING;
            PlayBodyReanim("anim_lower", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        }
    }
    else if (mState == PlantState::STATE_CACTUS_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_LOW;
            PlayIdleAnim(0.0f);
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        mState = PlantState::STATE_CACTUS_RISING;
        PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
    }
}

//0x461320
void Plant::UpdateChomper()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_CHOMPER_BITING;
            mStateCountdown = 70;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING)
    {
        if (mStateCountdown == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_BIGCHOMP);

            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            bool doBite = false;
            if (aZombie)
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_BOSS){
                    doBite = true;
                }

                if (mSeedType == SEED_SUPERCHOMPER && aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR && doBite)
                    doBite = false;
            }
            bool doMiss = false;
            if (aZombie == nullptr)
            {
                doMiss = true;
            }
            else if (!aZombie->IsImmobilizied())
            {
                if (aZombie->IsBouncingPogo() ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    doMiss = true;
                }
            }

            if (doBite)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aZombie->TakeDamage(40, 0U);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else if (doMiss)
            {
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else
            {
                aZombie->DieWithLoot();
                mState = PlantState::STATE_CHOMPER_BITING_GOT_ONE;
            }

            //Super Chomper
            if (mSeedType == SEED_SUPERCHOMPER){
                Zombie* mZombie = nullptr;
                while (mBoard->IterateZombies(mZombie))
                {
                    if (GetRectOverlap(GetPlantAttackRect(), mZombie->GetZombieRect()) >= -60 && mZombie->mRow == mRow && mZombie->EffectedByDamage(9U))
                    {
                        bool doBite = false;
                        if (mZombie)
                        {
                            if (mZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ||
                                mZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                            {
                                doBite = true;
                            }
                        }
                        bool doMiss = false;
                        if (mZombie == nullptr)
                        {
                            doMiss = true;
                        }
                        else if (!mZombie->IsImmobilizied())
                        {
                            if (mZombie->IsBouncingPogo() ||
                                mZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || mZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                                doMiss = true;

                        }

                        if (doBite)
                            mZombie->TakeDamage(40, 0U);
                        else
                            mZombie->DieWithLoot();
                    }
                }
            }
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING_GOT_ONE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_chew", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mState = PlantState::STATE_CHOMPER_DIGESTING;
            if (mSeedType==SEED_SUPERCHOMPER)
                mStateCountdown = 2000;
            else
                mStateCountdown = 3000; // 40000
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_DIGESTING)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_swallow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mState = PlantState::STATE_CHOMPER_SWALLOWING;
        }
    }
    else if ((mState == PlantState::STATE_CHOMPER_SWALLOWING || mState == PlantState::STATE_CHOMPER_BITING_MISSED) && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        mState = PlantState::STATE_READY;
    }
}

void Plant::UpdateFireShroom()
{
    if (mLaunchCounter > 0)
    {
        mLaunchCounter--;
    }
    
    if (mLaunchCounter == 0)
    {
        bool hasAttacked = false;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->EffectedByDamage(1))
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                int aRowDist = aZombie->mRow - mRow;

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                    aRowDist = 0;

                if (aRowDist <= 1 && aRowDist >= -1 && GetCircleRectOverlap(mX + mWidth / 2.0f, mY + mHeight / 2.0f, 115, aZombieRect))
                {
                    hasAttacked = true;
                    aZombie->TakeDamage(20, 2U);
                    aZombie->RemoveColdEffects();
                    
                    float aPosX = aZombie->ZombieTargetLeadX(50);
                    float aPosY = mBoard->GetPosYBasedOnRow(aPosX + 10.0f, aZombie->mRow) - 10.0f;

                    if (aZombie->IsOnHighGround())
                        aPosY -= HIGH_GROUND_HEIGHT;

                    Reanimation* aFwoosh = mApp->AddReanimation(aPosX, aPosY, mBoard->MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, aZombie->mRow, 1), ReanimationType::REANIM_JALAPENO_FIRE);
                    aFwoosh->mAnimRate = 24.0f;

                    float aScale = RandRangeFloat(0.65f, 0.85f);
                    float aFlip = Rand(2) ? 1.0f : -1.0f;
                    aFwoosh->OverrideScale(aScale * aFlip, 1);
                }
            }
        }

        if (hasAttacked)
        {
            mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
            mLaunchCounter = mLaunchRate - Sexy::Rand(15);
        }
    } 
}


void Plant::UpdateBonkChoy() 
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY || mState == PlantState::STATE_READY_BACKWARDS){
        Zombie* aZombie = FindTargetZombie(mRow, WEAPON_PRIMARY);
        if (aZombie) {
            bool backwards = aZombie->ZombieTargetLeadX(50) < mX + 40;
            if (Rand(10) == 0 || aZombie->mBodyHealth - 10 < aZombie->mBodyMaxHealth / 3 && aZombie->CanLoseBodyParts()) {
                PlayBodyReanim("anim_uppercut", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 40);
                mState = (backwards ? PlantState::STATE_UPPERCUTING_BACKWARDS : PlantState::STATE_UPPERCUTING);
            }
            else {
                PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 40);
                mState = (backwards ? PlantState::STATE_PUNCHING_BACKWARDS : PlantState::STATE_PUNCHING);
            }

            EndBlink();
            mIgnoreBlink = true;
            //mApp->PlayFoley(FOLEY_SWING);
        }
        else if (mState == PlantState::STATE_READY_BACKWARDS && aBodyReanim->mLoopCount > 0) {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_PUNCHING || mState == PlantState::STATE_UPPERCUTING || mState == PlantState::STATE_PUNCHING_BACKWARDS || mState == PlantState::STATE_UPPERCUTING_BACKWARDS){

        bool backwards = mState == PlantState::STATE_PUNCHING_BACKWARDS || mState == PlantState::STATE_UPPERCUTING_BACKWARDS;
        bool uppercutting = mState == PlantState::STATE_UPPERCUTING || mState == PlantState::STATE_UPPERCUTING_BACKWARDS;

        if (aBodyReanim->ShouldTriggerTimedEvent(.56f)){
            if (mState == PlantState::STATE_PUNCHING || mState == PlantState::STATE_PUNCHING_BACKWARDS)
                mApp->PlayFoley(FOLEY_PUNCHED);
            else
                mApp->PlayFoley(FOLEY_UPPERCUTED);

            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);

            bool doMiss = false;
            if (aZombie == nullptr){
                doMiss = true;
            }
            else if (!aZombie->IsImmobilizied()){
                if (aZombie->IsBouncingPogo() ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                    doMiss = true;
            }

            if (aZombie) {
                if (aZombie->mHelmType == HELMTYPE_PAIL || aZombie->mHelmType == HELMTYPE_DIVER){
                    mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
                }
                else if (aZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || aZombie->mHelmType == HELMTYPE_DIGGER || aZombie->mHelmType == HELMTYPE_FOOTBALL){
                    mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
                }
                aZombie->TakeDamage(10, (backwards || uppercutting ? 2U : 0U));

                if (uppercutting && aZombie->mZombieType != ZOMBIE_GARGANTUAR && aZombie->mZombieType != ZOMBIE_REDEYE_GARGANTUAR && aZombie->mZombieType != ZOMBIE_ZAMBONI && aZombie->mZombieType != ZOMBIE_BUNGEE && aZombie->mZombieType != ZOMBIE_CATAPULT && aZombie->mZombiePhase != PHASE_BOBSLED_SLIDING)
                    aZombie->mPosX += (backwards ? -10 : 10);
            }
        }
        if (aBodyReanim->mLoopCount > 0){
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            mState = (backwards ? PlantState::STATE_READY_BACKWARDS : PlantState::STATE_READY);
            mIgnoreBlink = false;
        }
    }
}

//0x4615D0
MagnetItem* Plant::GetFreeMagnetItem()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType == MagnetItemType::MAGNET_ITEM_NONE)
            {
                return &mMagnetItems[i];
            }
        }

        return nullptr;
    }

    return &mMagnetItems[0];
}

//0x460610
void Plant::MagnetShroomAttactItem(Zombie* theZombie)
{
    mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
    mStateCountdown = 1500;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

    MagnetItem* aMagnetItem = GetFreeMagnetItem();
    if (theZombie->mHelmType == HelmType::HELMTYPE_PAIL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_bucket", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_PAIL_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_DIVER)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("Zombie_diver", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("Zombie_diver", RENDER_GROUP_HIDDEN);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_DIVER1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_DIVER1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_DIVER_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("zombie_football_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX += 37.0f;
        aMagnetItem->mPosY -= 60.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_BLACKFOOTBALL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("zombie_football_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX += 37.0f;
        aMagnetItem->mPosY -= 60.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;

        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_BLACK_FOOTBALL_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        if (!theZombie->mIsEating)
        {
            TOD_ASSERT(theZombie->mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL);
            theZombie->StartWalkAnim(0);
        }
        theZombie->GetTrackPosition("anim_screendoor", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_DOOR_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();

        aMagnetItem->mPosX = theZombie->mPosX + 31.0f;
        aMagnetItem->mPosY = theZombie->mPosY + 20.0f;
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_LADDER_1 + aDamageIndex);
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_POGO)
    {
        theZombie->PogoBreak(16U);
        // ZombieDrawPosition aDrawPos;
        // theZombie->GetDrawPos(aDrawPos);
        theZombie->GetTrackPosition("Zombie_pogo_stick", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX += 40.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY += 84.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = theZombie->mHasArm ? MagnetItemType::MAGNET_ITEM_POGO_1 : MagnetItemType::MAGNET_ITEM_POGO_3;
    }
    else if (theZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
    {
        theZombie->StopZombieSound();
        theZombie->PickRandomSpeed();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        theZombie->ReanimShowPrefix("Zombie_jackbox_box", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("Zombie_jackbox_handle", RENDER_GROUP_HIDDEN);
        theZombie->GetTrackPosition("Zombie_jackbox_box", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX;
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        theZombie->DiggerLoseAxe();
        theZombie->GetTrackPosition("Zombie_digger_pickaxe", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 45.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_PICK_AXE;
    }

    if (theZombie->mHelmetReanimID != REANIMATIONID_NULL)
    {
        Reanimation* aHelmReanim = mApp->ReanimationTryToGet(theZombie->mHelmetReanimID);
        if (aHelmReanim)
            aHelmReanim->ReanimationDie();
        theZombie->mHelmetReanimID = REANIMATIONID_NULL;
    }
}

//0x461CD0
bool Plant::DrawMagnetItemsOnTop()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                return true;
            }
        }

        return false;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            MagnetItem* aMagnetItem = &mMagnetItems[i];
            if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
                if (aVectorToPlant.Magnitude() > 20.0f)
                {
                    return true;
                }
            }
        }

        return false;
    }
}

//0x461D90
void Plant::UpdateMagnetShroom()
{
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            if (aVectorToPlant.Magnitude() > 20.0f)
            {
                aMagnetItem->mPosX += aVectorToPlant.x * 0.05f;
                aMagnetItem->mPosY += aVectorToPlant.y * 0.05f;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 30, aAnimRate);
            if (mApp->IsIZombieLevel())
            {
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

            mMagnetItems[0].mItemType = MagnetItemType::MAGNET_ITEM_NONE;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_nonactive_idle2", ReanimLoopType::REANIM_LOOP, 20, 2.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
        }
    }
    else
    {
        float aClosestDistance = 0.0f;
        Zombie* aClosestZombie = nullptr;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            int aDiffY = aZombie->mRow - mRow;
            Rect aZombieRect = aZombie->GetZombieRect();

            if (aZombie->mMindControlled)
                continue;

            if (!aZombie->mHasHead)
                continue;

            if (aZombie->mZombieHeight != ZombieHeight::HEIGHT_ZOMBIE_NORMAL || aZombie->mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE)
                continue;

            if (aZombie->IsDeadOrDying())
                continue;

            if (aZombieRect.mX > BOARD_WIDTH || aDiffY > 2 || aDiffY < -2)
                continue;

            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING ||
                aZombie->mZombieType == ZombieType::ZOMBIE_POGO)
            {
                if (!aZombie->mHasObject)
                    continue;
            }
            else if (!(aZombie->mHelmType == HelmType::HELMTYPE_PAIL ||
                aZombie->mHelmType == HelmType::HELMTYPE_DIVER ||
                aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING) || 
                aZombie->mHelmType == HELMTYPE_BLACKFOOTBALL)
                continue;

            int aRadius = aZombie->mIsEating ? 320 : 270;
            if (GetCircleRectOverlap(mX +20.0f, mY +40.0f, aRadius, aZombieRect))
            {
                float aDistance = Distance2D(mX, mY, aZombieRect.mX, aZombieRect.mY);
                aDistance += abs(aDiffY) * 80.0f;

                if (aClosestZombie == nullptr || aDistance < aClosestDistance)
                {
                    aClosestZombie = aZombie;
                    aClosestDistance = aDistance;
                }
            }
        }

        if (aClosestZombie)
        {
            MagnetShroomAttactItem(aClosestZombie);
            return;
        }

        ////////////////////

        float aClosestLadderDist = 0.0f;
        GridItem* aClosestLadder = nullptr;
        
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
            {
                int aDiffX = abs(aGridItem->mGridX - mPlantCol);
                int aDiffY = abs(aGridItem->mGridY - mRow);
                int aSquareDistance = max(aDiffX, aDiffY);
                if (aSquareDistance <= 2)
                {
                    float aDistance = aSquareDistance + aDiffY * 0.05f;
                    if (aClosestLadder == nullptr || aDistance < aClosestLadderDist)
                    {
                        aClosestLadder = aGridItem;
                        aClosestLadderDist = aDistance;
                    }
                }
            }
        }

        if (aClosestLadder)
        {
            mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
            mStateCountdown = 1500;
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

            aClosestLadder->GridItemDie();

            MagnetItem* aMagnetItem = GetFreeMagnetItem();
            aMagnetItem->mPosX = mBoard->GridToPixelX(aClosestLadder->mGridX, aClosestLadder->mGridY) + 40;
            aMagnetItem->mPosY = mBoard->GridToPixelY(aClosestLadder->mGridX, aClosestLadder->mGridY);
            aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 10.0f;
            aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
            aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LADDER_PLACED;
        }
    }
}

//0x462390
Coin* Plant::FindGoldMagnetTarget()
{
    Coin* aClosestCoin = nullptr;
    float aClosestDistance = 0.0f;

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if ((aCoin->IsMoney() || aCoin->mType == COIN_SUN || aCoin->mType == COIN_SMALLSUN || aCoin->mType == COIN_LARGESUN) && aCoin->mCoinMotion != CoinMotion::COIN_MOTION_FROM_PRESENT && !aCoin->mIsBeingCollected && aCoin->mCoinAge >= 50)
        {
            float aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aCoin->mPosX + aCoin->mWidth / 2, aCoin->mPosY + aCoin->mHeight / 2);
            if (aClosestCoin == nullptr || aDistance < aClosestDistance)
            {
                aClosestCoin = aCoin;
                aClosestDistance = aDistance;
            }
        }
    }

    return aClosestCoin;
}

//0x4624B0
void Plant::GoldMagnetFindTargets()
{
    if (GetFreeMagnetItem() == nullptr)
    {
        TOD_ASSERT();
        return;
    }

    for (;;)
    {
        MagnetItem* aMagnetItem = GetFreeMagnetItem();
        if (aMagnetItem == nullptr)
            break;

        Coin* aCoin = FindGoldMagnetTarget();
        if (aCoin == nullptr)
            break;

        aMagnetItem->mPosX = aCoin->mPosX + 15.0f;
        aMagnetItem->mPosY = aCoin->mPosY + 15.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
        aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;

        switch (aCoin->mType)
        {
        case CoinType::COIN_SILVER:     aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SILVER_COIN;   break;
        case CoinType::COIN_GOLD:       aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_GOLD_COIN;     break;
        case CoinType::COIN_DIAMOND:    aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_DIAMOND;       break;
        default:                        TOD_ASSERT();                                                       return;
        }
        
        aCoin->Die();
    }
}

//0x4625D0
bool Plant::IsAGoldMagnetAboutToSuck()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_GOLD_MAGNET && aPlant->mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(aPlant->mBodyReanimID);
            if (aBodyReanim->mAnimTime < 0.5f)
            {
                return true;
            }
        }
    }

    return false;
}

//0x462680
void Plant::UpdateGoldMagnetShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    bool aIsSuckingCoin = false;
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            float aDistance = aVectorToPlant.Magnitude();
            if (aDistance < 20.0f)
            {
                CoinType aCoinType;
                switch (aMagnetItem->mItemType)
                {
                case MagnetItemType::MAGNET_ITEM_SILVER_COIN:   aCoinType = CoinType::COIN_SILVER;      break;
                case MagnetItemType::MAGNET_ITEM_GOLD_COIN:     aCoinType = CoinType::COIN_GOLD;        break;
                case MagnetItemType::MAGNET_ITEM_DIAMOND:       aCoinType = CoinType::COIN_DIAMOND;     break;
                }

                int aValue = Coin::GetCoinValue(aCoinType);
                mApp->mPlayerInfo->AddCoins(aValue);
                mBoard->mCoinsCollected += aValue;
                mApp->PlayFoley(FoleyType::FOLEY_COIN);
                
                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
            else
            {
                float aSpeed = TodAnimateCurveFloatTime(30.0f, 0.0f, aDistance, 0.02f, 0.05f, TodCurves::CURVE_LINEAR);
                aMagnetItem->mPosX += aVectorToPlant.x * aSpeed;
                aMagnetItem->mPosY += aVectorToPlant.y * aSpeed;
                aIsSuckingCoin = true; 
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            GoldMagnetFindTargets();
        }

        if (aBodyReanim->mLoopCount > 0 && !aIsSuckingCoin)
        {
            PlayIdleAnim(14.0f);
            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = RandRangeInt(200, 300);
        }
    }
    else if (!IsAGoldMagnetAboutToSuck() && Sexy::Rand(50) == 0 && FindGoldMagnetTarget())
    {
        mBoard->ShowCoinBank();
        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    }
}

//0x4629F0
void Plant::RemoveEffects()
{
    mApp->RemoveParticle(mParticleID);
    mApp->RemoveReanimation(mBodyReanimID);
    mApp->RemoveReanimation(mBackbodyReanimID);
    mApp->RemoveReanimation(mHeadReanimID);
    mApp->RemoveReanimation(mHeadReanimID2);
    mApp->RemoveReanimation(mHeadReanimID3);
    mApp->RemoveReanimation(mLightReanimID);
    mApp->RemoveReanimation(mBlinkReanimID);
    mApp->RemoveReanimation(mSleepingReanimID);
}

//0x462B80
void Plant::Squish()
{
    if (NotOnGround())
        return;

    if (!mIsAsleep)
    {
        if (mSeedType == SeedType::SEED_CHERRYHOVERBOMB ||mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO ||
            mSeedType == SeedType::SEED_DOOMSHROOM || mSeedType == SeedType::SEED_ICESHROOM || mSeedType == SeedType::SEED_EXPLODE_O_NUT || 
            mSeedType == SEED_ICEBERG_LETTUCE)
        {
            DoSpecial();
            return;
        }
        else if (mSeedType == SeedType::SEED_POTATOMINE && mState != PlantState::STATE_NOTREADY)
        {
            DoSpecial();
            return;
        }
    }

    if (mSeedType == SeedType::SEED_SQUASH && mState != PlantState::STATE_NOTREADY)
        return;


    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow, 8);
    mSquished = true;
    mDisappearCountdown = 500;
    mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
    RemoveEffects();

    GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
    if (aLadder)
    {
        aLadder->GridItemDie();
    }

    if (mApp->IsIZombieLevel())
    {
        mBoard->mChallenge->IZombiePlantDropRemainingSun(this);
    }
}

//0x462CE0
void Plant::UpdateBowling()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim && aBodyReanim->TrackExists("_ground"))
    {
        float aSpeed = aBodyReanim->GetTrackVelocity("_ground");
        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSpeed *= 2;
        }

        mX -= aSpeed;

        if (mBoard->StageHasRoof())
        {
            float aSlopeOffset = -105 + mPlantCol * 20;
            if (mX < 440.0f)
                aSlopeOffset += (440.0f - mX) * 0.25f;
            
            mBowlingSlope = aSlopeOffset;
        }

        if (mX > 800)
            Die();
    }

    if (mState == PlantState::STATE_BOWLING_UP)
    {
        mY -= 2;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN)
    {
        mY += 2;
    }

    int aDistToGrid = mBoard->GridToPixelY(mPlantCol, mRow) - mY;
    if (aDistToGrid < -2 || aDistToGrid > 2)
         return;

    PlantState aNewState = mState;
    if (mState == PlantState::STATE_BOWLING_UP && mRow <= 0)
    {
        aNewState = PlantState::STATE_BOWLING_DOWN;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN && mRow >= (mBoard->StageHas6Rows() ? 5 : 4))
    {
        aNewState = PlantState::STATE_BOWLING_UP;
    }

    Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
    if (aZombie)
    {
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;

        if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            mBoard->KillAllPlantsInRadius(mRow, aPosX, aPosY + mBowlingSlope, 115, 1, true, aDamageRangeFlags, mEnemy);
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY + mBowlingSlope, 115, 1, true, aDamageRangeFlags);
            mApp->AddTodParticle(aPosX, aPosY + mBowlingSlope, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();

            return;
        }
        else if (mSeedType == SEED_ARTIC_NUT)
        {
            mApp->PlayFoley(FOLEY_FROZEN);
            aZombie->HitIceTrap();
        }

        mApp->PlayFoley(FoleyType::FOLEY_BOWLINGIMPACT);
        mBoard->ShakeBoard(1, -2);

        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aZombie->TakeDamage(1800, 0U);
            if (aZombie->mAltitude < 0);
                aZombie->mAltitude = 0;
        }
        else if (aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR && mState != PlantState::STATE_NOTREADY)
        {
            aZombie->TakeDamage(1800, 0U);
        }
        else if (aZombie->mShieldType != ShieldType::SHIELDTYPE_NONE)
        {
            aZombie->TakeShieldDamage(400, 0U);
        }
        else if (aZombie->mHelmType != HelmType::HELMTYPE_NONE)
        {
            if (aZombie->mHelmType == HelmType::HELMTYPE_PAIL)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            }
            else if (aZombie->mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE)
            {
                mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
            }
            
            aZombie->TakeHelmDamage(900, 0U);
        }
        else if (aZombie->mBodyHealth >= 500)
        {
            aZombie->TakeDamage(500, 0U);
        }
        else
        {
            aZombie->TakeDamage(1800, 0U);
        }

        if ((!mApp->IsFirstTimeAdventureMode() || mApp->mPlayerInfo->GetLevel() > 10) && mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_ARTIC_NUT)
        {
            mLaunchCounter++;
            if (mLaunchCounter == 2)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 3)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 4)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter >= 5)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
            }
        }

        if (mSeedType != SeedType::SEED_GIANT_WALLNUT)
        {
            if (mRow == (mBoard->StageHas6Rows() ? 5 : 4) || mState == PlantState::STATE_BOWLING_DOWN)
            {
                aNewState = PlantState::STATE_BOWLING_UP;
            }
            else if (mRow == 0 || mState == PlantState::STATE_BOWLING_UP)
            {
                aNewState = PlantState::STATE_BOWLING_DOWN;
            }
            else
            {
                aNewState = Sexy::Rand(2) ? PlantState::STATE_BOWLING_UP : PlantState::STATE_BOWLING_DOWN;
            }
        }
    }

    if (aNewState == PlantState::STATE_BOWLING_UP)
    {
        mRow--;
        mState = PlantState::STATE_BOWLING_UP;
        mRenderOrder = CalcRenderOrder();
    }
    else if (aNewState == PlantState::STATE_BOWLING_DOWN)
    {
        mState = PlantState::STATE_BOWLING_DOWN;
        mRenderOrder = CalcRenderOrder();
        mRow++;
    }
}

//0x463150
void Plant::UpdateAbilities()
{
    if (!IsInPlay())
        return;

    if (mState == PlantState::STATE_DOINGSPECIAL || mSquished)
    {
        mDisappearCountdown--;
        if (mDisappearCountdown < 0 && mSeedType != SEED_EINSTEINAPPLE)
        {
            Die();
            return;
        }
    }

    if (mWakeUpCounter > 0)
    {
        mWakeUpCounter--;
        if (mWakeUpCounter == 60)
        {
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
        }
        if (mWakeUpCounter == 0)
        {
            SetSleeping(false);
        }
    }

    if (mIsAsleep || mSquished || mOnBungeeState != PlantOnBungeeState::NOT_ON_BUNGEE)
        return;
    
    UpdateShooting();

    if (mStateCountdown > 0)
        mStateCountdown--;

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mBerserkCounter > 0)
        mBerserkCounter--;

    if ((mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (mSeedType == SEED_WALLNUT || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_GIANT_WALLNUT || mSeedType == SEED_EXPLODE_O_NUT)) && !mIsPreview)
    {
        UpdateBowling();
        return;
    }

    if (mSeedType == SeedType::SEED_SQUASH)                                                     UpdateSquash();
    else if (mSeedType == SeedType::SEED_ICEBERG_LETTUCE)                                       UpdateIcebergLettuce();
    else if (mSeedType == SeedType::SEED_FIRESHROOM)                                            UpdateFireShroom();
    else if (mSeedType == SeedType::SEED_MAGNIFYINGGRASS)                                      UpdateMagnifyingGrass();
    else if (mSeedType == SeedType::SEED_SEEDROOT)                                              UpdateSeedRoot();
    else if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)                                         UpdateExplodeONut();
    else if (mSeedType == SeedType::SEED_ARTIC_NUT)                                             UpdateArticNut();
    else if (mSeedType == SeedType::SEED_CHERRYHOVERBOMB)                                      UpdateCherryHoverBomb();
    else if (mSeedType == SeedType::SEED_DOOMSHROOM)                                            UpdateDoomShroom();
    else if (mSeedType == SeedType::SEED_ICESHROOM)                                             UpdateIceShroom();
    else if (mSeedType == SeedType::SEED_EINSTEINAPPLE)                                        UpdateEinsteinApple();
    else if (mSeedType == SeedType::SEED_BONKCHOY)                                              UpdateBonkChoy();
    else if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER)   UpdateChomper();
    else if (mSeedType == SeedType::SEED_HURRIKALE)                                             UpdateHurrikale();
    else if (mSeedType == SeedType::SEED_BLOVER)                                                UpdateBlover();
    else if (mSeedType == SeedType::SEED_FLOWERPOT  || mSeedType == SeedType::SEED_WATERPOT || mSeedType == SeedType::SEED_FLYINGPOT)     
                                                                                                UpdateFlowerPot();
    else if (mSeedType == SeedType::SEED_LILYPAD)                                               UpdateLilypad();
    else if (mSeedType == SeedType::SEED_IMITATER)                                              UpdateImitater();
    else if (mSeedType == SeedType::SEED_INSTANTCOFFEE)                                        UpdateCoffeeBean();
    else if (mSeedType == SeedType::SEED_UMBRELLA)                                              UpdateUmbrella();
    else if (mSeedType == SeedType::SEED_COBCANNON)                                             UpdateCobCannon();
    else if (mSeedType == SeedType::SEED_CACTUS)                                                UpdateCactus();
    else if (mSeedType == SeedType::SEED_MAGNETSHROOM)                                          UpdateMagnetShroom();
    else if (mSeedType == SeedType::SEED_GOLD_MAGNET)                                           UpdateGoldMagnetShroom();
    else if (mSeedType == SeedType::SEED_SUNSHROOM)                                             UpdateSunShroom();
    else if (MakesSun() || mSeedType == SeedType::SEED_MARIGOLD || mSeedType == SeedType::SEED_AMPLIFLOWER)                                UpdateProductionPlant();
    else if (mSeedType == SeedType::SEED_GRAVEBUSTER)                                           UpdateGraveBuster();
    else if (mSeedType == SeedType::SEED_PLASMAWOOD)                                           UpdatePlasmawood();
    else if (mSeedType == SeedType::SEED_TORCHWOOD)                                             UpdateTorchwood();
    else if (mSeedType == SeedType::SEED_POTATOMINE)                                            UpdatePotato();
    else if (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK)    UpdateSpikeweed();
    else if (mSeedType == SeedType::SEED_TANGLEKELP)                                            UpdateTanglekelp();
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)                                         UpdateScaredyShroom();
    else if (mSeedType == SEED_SUNBLOOM)
    {
        if (mDoSpecialCountdown <= 200)
        {
            if (mDoSpecialCountdown % 50 == 0)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }    
        }
    }

    if (mSubclass == PlantSubClass::SUBCLASS_SHOOTER)
    {
        UpdateShooter();
    }
    if (mDoSpecialCountdown > 0)
    {
        mDoSpecialCountdown--;
        if (mDoSpecialCountdown == 0)
        {
            DoSpecial();
        }
    }

    if (mSeedType == SEED_SEEDROOT && mRootSeedType != SEED_NONE) {
        if (mRootTime < mRootMaxTime)
            mRootTime++;
    }
}

void Plant::UpdateMagnifyingGrass() {
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    if (aBodyReanim) {
        bool hasLight = (mBoard->FindPlanternPlant(mPlantCol, mRow) || !mBoard->StageIsNight()) && mBoard->CanTakeSunMoney(50);
        if (mState == STATE_MAGNIFYING_GRASS_ATTACKING)
        {
            if (aBodyReanim->ShouldTriggerTimedEvent(0.60f))
            {
                Projectile* aProjectile = mBoard->AddProjectile(mX + 44, mY, mRenderOrder - 1, mRow, PROJECTILE_MAGNIFYING_GRASS);
                //aProjectile->mPlantParentID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
                aProjectile->mDamageRangeFlags = 1;
            }

            if (aBodyReanim->mLoopCount > 0)
            {
                if (hasLight)
                {
                    mState = STATE_READY;
                    PlayBodyReanim("anim_idle", REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
                }
                else 
                {
                    mState = STATE_MAGNIFYING_GRASS_NO_LIGHT;
                    PlayBodyReanim("anim_no_light_idle", REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
                }
            }
        }
        else if (mState == STATE_READY) 
        {
            if (!hasLight)
            {
                mState = STATE_MAGNIFYING_GRASS_NO_LIGHT;
                aBodyReanim->StartBlend(20);
                aBodyReanim->mLoopType = REANIM_LOOP;
                aBodyReanim->mLoopCount = 0;
                aBodyReanim->GetFramesForLayer("anim_no_light_idle", aBodyReanim->mFrameStart, aBodyReanim->mFrameCount);
                
            }
        }
        else if (mState == STATE_MAGNIFYING_GRASS_NO_LIGHT)
        {
            if (hasLight) 
            {
                mState = STATE_READY;
                aBodyReanim->StartBlend(20);
                aBodyReanim->mLoopType = REANIM_LOOP;
                aBodyReanim->mLoopCount = 0;
                aBodyReanim->GetFramesForLayer("anim_idle", aBodyReanim->mFrameStart, aBodyReanim->mFrameCount);
            }
        }
    }
}

//0x463420
bool Plant::IsPartOfUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow) || mBoard->IsValidCobCannonSpot(mPlantCol - 1, mRow);
    }

    return IsUpgradableTo(theUpgradedType);
}

//0x463470
bool Plant::IsUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_GATLINGPEA && mSeedType == SeedType::SEED_REPEATER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_WINTERMELON && mSeedType == SeedType::SEED_MELONPULT)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_TWINSUNFLOWER && mSeedType == SeedType::SEED_SUNFLOWER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_SPIKEROCK && mSeedType == SeedType::SEED_SPIKEWEED)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow);
    }
    if (theUpgradedType == SeedType::SEED_GOLD_MAGNET && mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_GLOOMSHROOM && mSeedType == SeedType::SEED_FUMESHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_PLASMAWOOD && mSeedType == SeedType::SEED_TORCHWOOD)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_SUPERCHOMPER && mSeedType == SeedType::SEED_CHOMPER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_SHOOTINGSTAR && mSeedType == SeedType::SEED_STARFRUIT)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_BRAVEYSHROOM && mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_CATTAIL && mSeedType == SeedType::SEED_LILYPAD)
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        return aPlant == nullptr || aPlant->mSeedType != SeedType::SEED_CATTAIL;
    }
    return false;
}

void Plant::ApplyChill(bool theIsIceTrap) {
    if (mChilledCounter == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
    }

    int aChillTime = 2000;
    if (theIsIceTrap)
    {
        aChillTime = 4000;
    }
    mChilledCounter = max(aChillTime, mChilledCounter);
}

void Plant::HitIceTrap(unsigned int duration){
    bool cold = false;
    if (mChilledCounter > 0 || mIceTrapCounter != 0)
    {
        cold = true;
    }

    ApplyChill(true);

    if (mBoard->IsPoolSquare(mPlantCol, mRow) || mBoard->GetWaterPotAt(mPlantCol, mRow))
    {
        mIceTrapCounter = duration;
    }
    else if (cold)
    {
        mIceTrapCounter = RandRangeInt(duration, duration + duration * 0.33);
    }
    else
    {
        mIceTrapCounter = RandRangeInt(duration + duration * 0.33, duration * 2);
    }

    mPlantHealth -= 20;
    mEatenFlashCountdown = 25;
}

//0x4635C0
void Plant::UpdateReanimColor()
{
    bool doIgnore = !IsOnBoard() && !mIsPlayer;

    if (doIgnore)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    Color aColorOverride;

   // SeedType aSeedType2 = mBoard->mSeedBank->mSeedPackets[mBoard->P1SeedBankID].mPacketType;

    bool isOnGlove = false;
    if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE){
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mBoard->mCursorObject->mGlovePlantID);
        isOnGlove = mApp->mGameMode == GAMEMODE_CHALLENGE_ZEN_GARDEN ? aPlant && aPlant->mPlantCol == mPlantCol && aPlant->mRow == mRow : aPlant == this;
    }

    bool playerHeldUpgrade = false;

    for (int i = 0; i < mBoard->mMaxPlayers; i++) {
        PlayerController* mPlayer = mBoard->mPlayers[i];

        if (mPlayer->mControlType == CONTROL_MOUSE)
            continue;

        Plant* mPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mPlayer->mGlovePlantID);
        if (mPlant && !isOnGlove)
            isOnGlove = mPlant == this;

        if (!playerHeldUpgrade){
            SeedType mSeed = mBoard->mSeedBank->mSeedPackets[mPlayer->mSeedBankIndex].mPacketType;
            playerHeldUpgrade = mBoard->mSeedBank->mSeedPackets[mPlayer->mSeedBankIndex].mActive && IsPartOfUpgradableTo(mSeed) && mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(mSeed, SeedType::SEED_NONE))
                && mBoard->CanPlantAt(mPlantCol, mRow, mSeed) == PLANTING_OK;
        }

    }

    if ((IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PLANTING_OK) || playerHeldUpgrade) {// || IsPartOfUpgradableTo(aSeedType2) && mBoard->mSeedBank->mSeedPackets[mBoard->P1SeedBankID].mActive && mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(mBoard->mSeedBank->mSeedPackets[mBoard->P1SeedBankID].mPacketType, SeedType::SEED_NONE))) {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (aSeedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PLANTING_OK){
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    //else if (mSeedType == SeedType::SEED_EXPLODE_O_NUT){
    //    aColorOverride = Color(255, 64, 64);
    //}
    else{
        aColorOverride = Color::White;
    }

    if (isOnGlove) {
        aColorOverride.mRed /= 2;
        aColorOverride.mGreen /= 2;
        aColorOverride.mBlue /= 2;
    }

    aBodyReanim->mColorOverride = aColorOverride;

    Color aSpotLightColor = Color::White;
    if (mAmplifiedCounter > 0 && mPlantAmplifierID != PLANTID_NULL)
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mPlantAmplifierID);
        if (aPlant)
        {
            switch (aPlant->mPlantAge >= 700 ? aPlant->mPlantAge / 100 * 7 % 5 : 0)
            {
            case 0:
                aSpotLightColor = Color(250, 250, 160);
                break;
            case 1:
                aSpotLightColor = Color(114, 234, 170);
                break;
            case 2:
                aSpotLightColor = Color(216, 126, 202);
                break;
            case 3:
                aSpotLightColor = Color(90, 110, 140);
                break;
            case 4:
                aSpotLightColor = Color(240, 90, 130);
                break;
            }
        }
    }

    if (mState == STATE_BURNED){
        aBodyReanim->mColorOverride = Color(0, 0, 0);
        aBodyReanim->mExtraAdditiveColor = Color::Black;
        aBodyReanim->mEnableExtraAdditiveDraw = false;
    }
    else if (mHighlighted){
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 196);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
        if (mImitaterType == SeedType::SEED_IMITATER)
            aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 92);
    }
    else if (mBeghouledFlashCountdown > 0){
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mEatenFlashCountdown > 0){
        int aGrayness = ClampInt(mEatenFlashCountdown * 3, 0, mImitaterType == SeedType::SEED_IMITATER ? 128 : 255);
        aBodyReanim->mExtraAdditiveColor = Color(aGrayness, aGrayness, aGrayness);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mAmplifiedCounter > 0) {
        int anAlpha = TodAnimateCurve(50, 0, mAmplifiedCounter % 50, 0, 64, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(aSpotLightColor.mRed, aSpotLightColor.mGreen, aSpotLightColor.mBlue, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mExplodePulse > 0) {
        int anAlpha = TodAnimateCurve(50, 0, mExplodePulse % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mMindControlled)
    {
        aBodyReanim->mColorOverride = ZOMBIE_MINDCONTROLLED_COLOR;
        /*if (mSeedType == SEED_EXPLODE_O_NUT) {
            aBodyReanim->mColorOverride.mGreen = max(aBodyReanim->mColorOverride.mGreen - 191, 0);
            aBodyReanim->mColorOverride.mBlue = max(aBodyReanim->mColorOverride.mBlue - 191, 0);
        }*/
        aBodyReanim->mExtraAdditiveColor = aBodyReanim->mColorOverride;
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mChilledCounter > 0 || mIceTrapCounter > 0)
    {
        aBodyReanim->mColorOverride = Color(75, 75, 255);
        /*if (mSeedType == SEED_EXPLODE_O_NUT) {
            aBodyReanim->mColorOverride.mGreen = max(aBodyReanim->mColorOverride.mGreen - 191, 0);
            aBodyReanim->mColorOverride.mBlue = max(aBodyReanim->mColorOverride.mBlue - 191, 0);
        }*/
        aBodyReanim->mExtraAdditiveColor = aBodyReanim->mColorOverride;
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else{
        aBodyReanim->mEnableExtraAdditiveDraw = false;
    }
    
    if (mBeghouledFlashCountdown > 0){
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraOverlayDraw = true;
    }
    else if (mAmplifiedCounter > 0) {
        int anAlpha = TodAnimateCurve(50, 0, mAmplifiedCounter % 50, 0, 64, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraOverlayColor = Color(aSpotLightColor.mRed, aSpotLightColor.mGreen, aSpotLightColor.mBlue, anAlpha);
        aBodyReanim->mEnableExtraOverlayDraw = true;
    }
    else if (mExplodePulse > 0) {
        int anAlpha = TodAnimateCurve(50, 0, mExplodePulse % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraOverlayDraw = true;
    }
    else{
        aBodyReanim->mEnableExtraOverlayDraw = false;
    }

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE){
        if (mBerserkCounter > 0) {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mHeadReanimID);
            if (aHeadReanim){
                aHeadReanim->mColorOverride.mGreen /= 2;
                aHeadReanim->mColorOverride.mBlue /= 2;
            }

            aBodyReanim->mColorOverride.mGreen /= 2;
            aBodyReanim->mColorOverride.mBlue /= 2;
        }
        else if (mState == STATE_EXTREMELY_DEHYDRATED) {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mHeadReanimID);
            if (aHeadReanim){
                aHeadReanim->mColorOverride.mRed /= 2;
                aHeadReanim->mColorOverride.mGreen /= 2;
                aHeadReanim->mColorOverride.mBlue /= 2;
            }

            aBodyReanim->mColorOverride.mRed /= 2;
            aBodyReanim->mColorOverride.mGreen /= 2;
            aBodyReanim->mColorOverride.mBlue /= 2;

            int anAlpha = TodAnimateCurve(50, 0, mPlantAge % 50, 0, 64, TodCurves::CURVE_BOUNCE);
            aBodyReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
            aBodyReanim->mEnableExtraOverlayDraw = true;
            if (aHeadReanim){
                aHeadReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
                aHeadReanim->mEnableExtraOverlayDraw = true;
            }
        }
        else if (mState == STATE_DEHYDRATED) {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mHeadReanimID);
            if (aHeadReanim) {
                aHeadReanim->mColorOverride.mRed /= 1.5f;
                aHeadReanim->mColorOverride.mGreen /= 1.5f;
                aHeadReanim->mColorOverride.mBlue /= 1.5f;
            }

            aBodyReanim->mColorOverride.mRed /= 1.5f;
            aBodyReanim->mColorOverride.mGreen /= 1.5f;
            aBodyReanim->mColorOverride.mBlue /= 1.5f;
        }
    }

    aBodyReanim->PropogateColorToAttachments();
}

bool Plant::IsOnBoard()
{
    if (!mIsOnBoard)
        return false;

    TOD_ASSERT(mBoard);
    return true;
}

//0x4638F0
bool Plant::IsInPlay()
{
    return IsOnBoard() && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM && !mIsPreview;
}

//0x463920
void Plant::UpdateReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    UpdateReanimColor();

    float aOffsetX = mShakeOffsetX;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    float aScaleX = 1.0f, aScaleY = 1.0f;
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_MARIGOLD))
    {
        aScaleX = 1.5f;
        aScaleY = 1.5f;
        aOffsetX -= 20.0f;
        aOffsetY -= 40.0f;
    }
    if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aScaleX = 2.0f;
        aScaleY = 2.0f;
       
        aOffsetX -= 76.0f - 33.0f; //33.0f funcust
        aOffsetY -= 64.0f;
    }
    if (mSeedType == SeedType::SEED_INSTANTCOFFEE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 10.0f;
    }
    if (mSeedType == SeedType::SEED_POTATOMINE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    if (mState == PlantState::STATE_GRAVEBUSTER_EATING)
    {
        aOffsetY += TodAnimateCurveFloat(400, 0, mStateCountdown, 0.0f, 30.0f, TodCurves::CURVE_LINEAR);
    }
    if (mWakeUpCounter > 0)
    {
        float aScaleFactor = TodAnimateCurveFloat(70, 0, mWakeUpCounter, 1.0f, 0.8f, TodCurves::CURVE_EASE_SIN_WAVE);
        aScaleY *= aScaleFactor;
        aOffsetY += 80.0f - 80.0f * aScaleFactor;
    }

    if (mSeedType == SeedType::SEED_LEFTPEATER && !mEnemy || mSeedType != SeedType::SEED_LEFTPEATER && mEnemy){
        aOffsetX += 85.0f * aScaleX;
        aScaleX *= -1.0f;
    }

    if (mSeedType == SEED_BONKCHOY && (mState == STATE_PUNCHING_BACKWARDS || mState == STATE_UPPERCUTING_BACKWARDS || mState == STATE_READY_BACKWARDS)) {
        aOffsetX += 85.0f * aScaleX;
        aScaleX *= -1.0f;
    }

    aBodyReanim->Update();

    /*if (mSeedType == SEED_COBCANNON && IsOnBoard())
    {
        Reanimation* aHEADReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        float direction = (float)atan2((double)mBoard->GridToPixelY(mPlantCol + 1, mRow) - mBoard->GridToPixelY(mPlantCol, mRow), (double)mBoard->GridToPixelX(mPlantCol + 1, mRow) - mBoard->GridToPixelX(mPlantCol, mRow));
        aHEADReanim->mOverlayMatrix.m10 = sin(direction);
        aHEADReanim->mOverlayMatrix.m01 = -sin(direction);
        aHEADReanim->mOverlayMatrix.m00 = cos(direction);
        aHEADReanim->mOverlayMatrix.m11 = cos(direction);

        float st1 = cos(direction);
        float st0 = sin(direction);
        float st2 = 1.0f;
        float value28 = 40.0f;

        st2 = st1 - st2;
        st2 += st0;
        st2 *= value28;
        st2 = -st2;
        aHEADReanim->mOverlayMatrix.m12 = st2;
        aHEADReanim->mOverlayMatrix.m12 += aOffsetY;
        st2 = 1.0f;
        st2 -= st1;
        st2 += st0;
        st2 *= value28;
        aHEADReanim->mOverlayMatrix.m02 = st2;
        aHEADReanim->mOverlayMatrix.m02 += aOffsetX;
    }*/
    

    if (mPottedPlantIndex != -1)
    {
        PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mPottedPlantIndex];

        if (aPottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT)
        {
            aOffsetX += 85.0f * aScaleX;
            aScaleX *= -1.0f;
        }

        float aOffsetXStart, aOffsetXEnd;
        float aOffsetYStart, aOffsetYEnd;
        float aScaleStart, aScaleEnd;
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 20.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 40.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.5f;
        }
        else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 10.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 20.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.75f;
        }
        else
        {
            aOffsetXStart = 10.0f;
            aOffsetXEnd = 0.0f;
            aOffsetYStart = 20.0f;
            aOffsetYEnd = 0.0f;
            aScaleStart = 0.75f;
            aScaleEnd = 1.0f;
        }

        float aAnimatedOffsetX = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetXStart, aOffsetXEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedOffsetY = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetYStart, aOffsetYEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedScale = TodAnimateCurveFloat(100, 0, mStateCountdown, aScaleStart, aScaleEnd, TodCurves::CURVE_LINEAR);

        aOffsetX += aAnimatedOffsetX * aScaleX;
        aOffsetY += aAnimatedOffsetY * aScaleY;
        aScaleX *= aAnimatedScale;
        aScaleY *= aAnimatedScale;
        aOffsetX += mApp->mZenGarden->ZenPlantOffsetX(aPottedPlant);
        aOffsetY += mApp->mZenGarden->PlantPottedDrawHeightOffset(mSeedType, aScaleY);
    }

    aBodyReanim->SetPosition(aOffsetX, aOffsetY);
    aBodyReanim->OverrideScale(aScaleX, aScaleY);
}

//0x463E40
void Plant::Update()
{
    bool doUpdate = false;
    if (IsOnBoard() && mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && (mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (mSeedType == SEED_WALLNUT || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_GIANT_WALLNUT || mSeedType == SEED_EXPLODE_O_NUT)))
        doUpdate = true;
    else if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        doUpdate = true;
    else if (IsOnBoard() && mBoard->mCutScene->ShouldRunUpsellBoard())
        doUpdate = true;
    else if (!IsOnBoard() || mApp->mGameScene == GameScenes::SCENE_PLAYING)
        doUpdate = true;

    if (mState == STATE_BURNED) {
        doUpdate = false;
        UpdateBurn();
    }

    if (doUpdate)
    {
        mPlantAge++;

        if (mIceTrapCounter > 0)
        {
            mIceTrapCounter--;
 
            if (mIceTrapCounter == 0) {
                mIceTrapCounter = 0;
                AddAttachedParticle(mX + 75, mY + 106, (int)RenderLayer::RENDER_LAYER_PARTICLE, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
            }
        }

        if (mButteredCounter > 0) {
            mButteredCounter--;
        }
        if (mRecentlyEatenCountdown > 0)
        {
            mRecentlyEatenCountdown--;
        }
        if (mEatenFlashCountdown > 0)
        {
            mEatenFlashCountdown--;
        }
        if (mBeghouledFlashCountdown > 0)
        {
            mBeghouledFlashCountdown--;
        }

        if (mAmplifiedCounter > 0)
        {
            mAmplifiedCounter--;

            if (mPlantAmplifierID != PLANTID_NULL) {
                Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mPlantAmplifierID);
                if (!aPlant)    mAmplifiedCounter = 0;
                
            }
        }
        if (mAmplifiedCounter == 0 && mPlantAmplifierID != PLANTID_NULL) {
            mPlantAmplifierID = PLANTID_NULL;
        }

        if (mSingingCounter > 0) {
            mSingingCounter--;
        }
        if (mChilledCounter > 0)
        {
            mChilledCounter--;
        }

        if (mButteredCounter > 0 || mIceTrapCounter > 0 || mChilledCounter % 2 == 1)
        {
            UpdateReanimColor();
            return;
        }

        UpdateAbilities();
        Animate();

        if (mAmplifiedCounter > 0) {
            UpdateAbilities();
        }

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL && !mIsPreview && mBoard)
        {
            bool doHailDMG = mPlantAge % 300 == 0;
            if (doHailDMG) {
                bool hasUmbrella = false;

                for (int i = -1; i <= 1; i++)
                {
                    if (hasUmbrella) break;
                    for (int j = -1; j <= 1; j++)
                    {
                        if (hasUmbrella) break;

                        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol + i, mRow + j, TOPPLANT_ONLY_NORMAL_POSITION);
                        hasUmbrella = aPlant && aPlant->mSeedType == SEED_UMBRELLA;
                    }
                }

                if (!hasUmbrella && mSeedType != SEED_UMBRELLA) {
                    mEatenFlashCountdown = max(mEatenFlashCountdown, 25);
                    mPlantHealth -= 10 + (int)(mBoard->mCurrentWave / mBoard->mNumWaves * 30);
                }
                else if (mSeedType == SEED_UMBRELLA) {
                    mEatenFlashCountdown = max(mEatenFlashCountdown, 25);
                    mPlantHealth -= 5 + (int)(mBoard->mCurrentWave / mBoard->mNumWaves * 15);
                }
            }
        }
        else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mSeedType == SEED_PEASHOOTER && mBoard) {
            if (mHeatWaveTimer > 0)
                mHeatWaveTimer--;

            Plant* aLilyPad = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
            if (aLilyPad || mBerserkCounter > 0) {
                if (mState != STATE_NOTREADY && mBerserkCounter == 0)
                    mApp->PlaySample(Sexy::SOUND_WATERING);
                mState = STATE_NOTREADY;
                mHeatWaveTimer = 2500;
            }else if (mHeatWaveTimer == 0 && mState == STATE_NOTREADY) {
                mState = STATE_DEHYDRATED;
                mHeatWaveTimer = RandRangeInt(1500, 2500);
            }
            else if (mHeatWaveTimer == 0 && mState == STATE_DEHYDRATED) {
                mState = STATE_EXTREMELY_DEHYDRATED;
            }
        }

        if (mPlantHealth <= 0){            
            Die();
        }

        UpdateReanim();
    }
}

//0x463EF0
bool Plant::NotOnGround()
{
    if (mSeedType == SeedType::SEED_SQUASH)
    {
        if (mState == PlantState::STATE_SQUASH_RISING || mState == PlantState::STATE_SQUASH_FALLING || mState == PlantState::STATE_SQUASH_DONE_FALLING)
            return true;
    }

    return mSquished || mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE || mDead;
}

//0x463F30
Reanimation* Plant::AttachBlinkAnim(Reanimation* theReanimBody)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(mSeedType);
    LawnApp* aApp = (LawnApp*)gSexyAppBase;
    Reanimation* aAnimToAttach = theReanimBody;
    const char* aTrackToPlay = "anim_blink";
    const char* aTrackToAttach = nullptr;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_ARTIC_NUT || mSeedType == SeedType::SEED_TALLNUT ||
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        int aHit = Rand(10);
        if (aHit < 1 && theReanimBody->TrackExists("anim_blink_twitch"))
        {
            aTrackToPlay = "anim_blink_twitch";
        }
        else
        {
            aTrackToPlay = aHit < 7 ? "anim_blink_twice" : "anim_blink_thrice";
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        int aHit = Rand(3);
        if (aHit == 0)
        {
            aTrackToPlay = "anim_blink1";
            aTrackToAttach = "anim_face1";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head1");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (aHit == 1)
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head2");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else
        {
            aTrackToPlay = "anim_blink3";
            aTrackToAttach = "anim_face3";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head3");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID);
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID2);
        }
    }
    else if (mIsPlayer) {
        theReanimBody = mApp->ReanimationTryToGet(mHeadReanimID);
        aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID);
    }
    else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
        }
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER || mSeedType == SeedType::SEED_GATLINGPEA)
    {
        if (theReanimBody->TrackExists("anim_stem"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_stem");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (theReanimBody->TrackExists("anim_idle"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_idle");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }

    if (aAnimToAttach == nullptr)
    {
        TodTrace("Missing head anim");
        return nullptr;
    }

    if (!theReanimBody->TrackExists(aTrackToPlay))
        return nullptr;

    Reanimation* aBlinkReanim = aApp->mEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aPlantDef.mReanimationType);
    aBlinkReanim->SetFramesForLayer(aTrackToPlay);
    aBlinkReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD;
    aBlinkReanim->mAnimRate = 15.0f;
    aBlinkReanim->mColorOverride = theReanimBody->mColorOverride;

    if (aTrackToAttach && aAnimToAttach->TrackExists(aTrackToAttach))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, aTrackToAttach);
    }
    else if (aAnimToAttach->TrackExists("anim_face"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_face");
    }
    else if (aAnimToAttach->TrackExists("anim_idle"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_idle");
    }
    else
    {
        TodTrace("Missing anim_idle for blink");
    }

    aBlinkReanim->mFilterEffect = theReanimBody->mFilterEffect;
    return aBlinkReanim;
}

//0x464230
void Plant::DoBlink()
{
    mBlinkCountdown = 400 + Rand(400);

    if (mIgnoreBlink)
        return;

    if (NotOnGround() || mShootingCounter != 0)
        return;

    if (mSeedType == SeedType::SEED_POTATOMINE && mState != PlantState::STATE_POTATO_ARMED)
        return;

    if (mState == PlantState::STATE_CACTUS_RISING || mState == PlantState::STATE_CACTUS_HIGH || mState == PlantState::STATE_CACTUS_LOWERING ||
        mState == PlantState::STATE_MAGNETSHROOM_SUCKING || mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
        return;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if ((mSeedType == SeedType::SEED_TALLNUT && aBodyReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_TALLNUT_CRACKED2) ||
        (mSeedType == SeedType::SEED_GARLIC && aBodyReanim->GetImageOverride("anim_face") == IMAGE_REANIM_GARLIC_BODY3))
        return;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_ARTIC_NUT || mSeedType == SeedType::SEED_TALLNUT ||
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        mBlinkCountdown = 1000 + Rand(1000);
    }

    if (mIsPlayer) {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mHeadReanimID);
        if (aHeadReanim)
        {
            Reanimation* aBlinkReanim = AttachBlinkAnim(aHeadReanim);
            if (aBlinkReanim)
            {
                mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
            }
            aHeadReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
            return;
        }
    }

    Reanimation* aBlinkReanim = AttachBlinkAnim(aBodyReanim);
    if (aBlinkReanim)
    {
        mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
    }
    aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
}

//0x464390
void Plant::EndBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        mApp->RemoveReanimation(mBlinkReanimID);
        mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_NORMAL);
        }
    }
}

//0x464410
void Plant::UpdateBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        Reanimation* aBlinkReanim = mApp->ReanimationTryToGet(mBlinkReanimID);
        if (aBlinkReanim == nullptr || aBlinkReanim->mLoopCount > 0)
        {
            EndBlink();
        }
    }

    if (mIsAsleep)
        return;

    if (mBlinkCountdown > 0)
    {
        mBlinkCountdown--;
        if (mBlinkCountdown == 0)
        {
            DoBlink();
        }
    }
}

void Plant::AnimateFireshroom() 
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        if (mPlantHealth < mPlantMaxHealth / 3)
        {
            if (aBodyReanim->GetImageOverride("anim_face") != IMAGE_REANIM_FIRESHROOM_BODY2)
            {
                aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_FIRESHROOM_BODY2);
            }
            if (aBodyReanim->GetImageOverride("FireShroom_cap") != IMAGE_REANIM_FIRESHROOM_CAP3)
            {
                aBodyReanim->SetImageOverride("FireShroom_cap", IMAGE_REANIM_FIRESHROOM_CAP3);
            }

        }
        else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
        {
            if (aBodyReanim->GetImageOverride("FireShroom_cap") != IMAGE_REANIM_FIRESHROOM_CAP2)
            {
                aBodyReanim->SetImageOverride("FireShroom_cap", IMAGE_REANIM_FIRESHROOM_CAP2);
            }
        }
        else
        {
            aBodyReanim->SetImageOverride("anim_face", nullptr);
            aBodyReanim->SetImageOverride("FireShroom_cap", nullptr);
        }
    }
}

void Plant::AnimatePuffshroom()
{
    if (mState != STATE_PUFFSHROOM_FADING  || mApp->mGameMode == GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
    {
        return;
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    Image* aHead2 = IMAGE_REANIM_PUFFSHROOM_DECAY1;
    Image* aHead3 = IMAGE_REANIM_PUFFSHROOM_DECAY2;
    const char* aTrackToOverride = "PuffShroom_head";

    Image* aImageOverride = aBodyReanim->GetImageOverride(aTrackToOverride);
    if (mDoSpecialCountdown < 4000)
    {
        if (aImageOverride != aHead3)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aHead3);
          
        }
    }
    else if (mDoSpecialCountdown < 8000)
    {
        if (aImageOverride != aHead2)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aHead2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride(aTrackToOverride, nullptr);
    }
}

//0x464480
void Plant::AnimateNuts()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    Image* aCracked1;
    Image* aCracked2;
    const char* aTrackToOverride;
    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_GIANT_WALLNUT){
        aCracked1 = IMAGE_REANIM_WALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_WALLNUT_CRACKED2;

        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_EXPLODE_O_NUT) {
        aCracked1 = IMAGE_REANIM_EXPLODE_O_NUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_EXPLODE_O_NUT_CRACKED2;

        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_DUMMY) {
        aCracked1 = IMAGE_REANIM_DUMMY_CRACKED1;
        aCracked2 = IMAGE_REANIM_DUMMY_CRACKED2;

        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_ARTIC_NUT) {
        aCracked1 = IMAGE_REANIM_ICENUT_BODY2;
        aCracked2 = IMAGE_REANIM_ICENUT_BODY3;
        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aCracked1 = IMAGE_REANIM_TALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_TALLNUT_CRACKED2;
        aTrackToOverride = "anim_idle";
    }
    else return;

    int aPosX = mX + 40;
    int aPosY = mY + 10;
    if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aPosY -= 32;
    }

    Image* aImageOverride = aBodyReanim->GetImageOverride(aTrackToOverride);
    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != aCracked2)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked2);
            TodParticleSystem* partic = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);

            if (mSeedType == SEED_EXPLODE_O_NUT)
                partic->OverrideColor(nullptr, Color(255, 64, 64));
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != aCracked1)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked1);
            TodParticleSystem* partic = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);

            if (mSeedType == SEED_EXPLODE_O_NUT)
                partic->OverrideColor(nullptr, Color(255, 64, 64));
        }
    }
    else
    {
        aBodyReanim->SetImageOverride(aTrackToOverride, nullptr);
    }

    if (IsInPlay() && !mApp->IsIZombieLevel())
    {
        if (mRecentlyEatenCountdown > 0)
        {
            aBodyReanim->mAnimRate = 0.1f;
            return;
        }

        if (aBodyReanim->mAnimRate < 1.0f && mOnBungeeState != PlantOnBungeeState::RISING_WITH_BUNGEE)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

//0x464680
void Plant::AnimateGarlic()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("anim_face");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY3)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY3);
            aBodyReanim->AssignRenderGroupToPrefix("Garlic_stem", RENDER_GROUP_HIDDEN);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY2)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_face", nullptr);
    }
}

//0x464760
void Plant::AnimatePumpkin()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("Pumpkin_front");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE3)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE3);
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE1)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE1);
    }
    else
    {
        aBodyReanim->SetImageOverride("Pumpkin_front", nullptr);
    }
}

//0x464820
void Plant::UpdateShooting()
{
    if (NotOnGround() || mShootingCounter == 0)
        return;

    mShootingCounter--;

    if ((mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM) && mShootingCounter == 15)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        TodParticleSystem* partic = AddAttachedParticle(mX + 85, mY + PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow) + 31, aRenderPosition, ParticleEffect::PARTICLE_FUMECLOUD);

        if (mSeedType == SeedType::SEED_ICYFUMESHROOM)
        {
            partic->OverrideImage(nullptr, IMAGE_ICY_FUMESHROOM_CLOUD);
            mApp->PlayFoley(FOLEY_SNOW_PEA_SPARKLES);
        }
    }

    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        if (mShootingCounter == 136 || mShootingCounter == 108 || mShootingCounter == 80 || mShootingCounter == 52)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);
        }
        if (mShootingCounter == 126 || mShootingCounter == 98 || mShootingCounter == 70 || mShootingCounter == 42)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51 || mShootingCounter == 68){
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        if (mShootingCounter == 19)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie)
            {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
        }
    }
    else if (mShootingCounter == 1)
    {
        if (mSeedType == SeedType::SEED_SLINGPEA) {
            int maxRows = mBoard->StageHas6Rows() ? 6 : 5;  
            int shootCounter = 3;
            while (shootCounter > 0) {
                Zombie* zombiesPerRow[6] = { nullptr };
                int rowC = 0;
                if (rand() % 2 == 0) {
                    for (int r = mRow; r >= 0; --r) {
                        zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                        rowC++;
                    }
                  
                    for (int r = mRow + 1; r < maxRows; ++r) {
                        zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                        rowC++;
                    }
                }
                else {
                    for (int r = mRow; r < maxRows; ++r) {
                        zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                        rowC++;
                    }
                    
                    for (int r = mRow - 1; r >= 0; --r) {
                        zombiesPerRow[rowC] = FindTargetZombie(r, PlantWeapon::WEAPON_PRIMARY);
                        rowC++;
                    }
                }

                bool zombiesExists = false;
                int index = 0;
                while(shootCounter > 0 && index < maxRows)
                {
                    Zombie* tZombie = zombiesPerRow[index];
                    if (tZombie == nullptr || tZombie->mDead || tZombie->mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || tZombie->mZombiePhase == PHASE_POLEVAULTER_IN_VAULT || tZombie->mZombiePhase == PHASE_BALLOON_POPPING)
                    {
                        ++index;
                        continue;
                    }
                    Fire(tZombie, tZombie->mRow);
                    --shootCounter;
                    zombiesExists = true;
                    ++index;
                }

               if (!zombiesExists) break;
            }
        }
        else if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD){
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD){
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD){
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            Reanimation* aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation* aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && mLaunchCounter <= 1)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD){
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }
        else if (mState == PlantState::STATE_CACTUS_LOW){
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType==SeedType::SEED_FROST_CABBAGE || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON 
            || mSeedType == SeedType::SEED_SPORE_SHROOM || mSeedType == SeedType::SEED_BOMBBERRY)
        {
            PlantWeapon aPlantWeapon = PlantWeapon::WEAPON_PRIMARY;
            if (mState == PlantState::STATE_KERNELPULT_BUTTER)
            {
                Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_NORMAL);
                mState = PlantState::STATE_NOTREADY;
                aPlantWeapon = PlantWeapon::WEAPON_SECONDARY;
            }

            void* aTarget = FindTargetZombie(mRow, aPlantWeapon);
            Plant* aPlant = FindTargetPlant(mRow, aPlantWeapon);
            if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
                aTarget = aPlant;
            Fire(aTarget, mRow, aPlantWeapon);
        }
        else{
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }

        return;
    }

    if (mShootingCounter != 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (aHeadReanim2->mLoopCount > 0)
        {
            if (aHeadReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim->StartBlend(20);
                aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim->SetFramesForLayer("anim_head_idle1");
                aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            }

            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_head_idle2");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;

            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim3->StartBlend(20);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim3->SetFramesForLayer("anim_head_idle3");
                aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
            }
            
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);

        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        }

        if (aHeadReanim2->mLoopCount > 0)
        {
            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
        }

        return;
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);

            aBodyReanim->mAnimRate = aBodyReanim->mDefinition->mFPS;
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0.0f;
            }

            return;
        }
    }
    else if (aHeadReanim)
    {
        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 3000;
            PlayBodyReanim("anim_unarmed_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            return;
        }
    }
    else if (aBodyReanim && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        return;
    }

    mShootingCounter = 1;
}

//0x464DB0
void Plant::Animate()
{
    if ((mSeedType == SeedType::SEED_CHERRYHOVERBOMB || mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO) && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
    {
        mShakeOffsetX = RandRangeFloat(-1.0f, 1.0f);
        mShakeOffsetY = RandRangeFloat(-1.0f, 1.0f);
    }

    if (mSquished)
    {
        mFrame = 0;
        return;
    }

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_DUMMY || mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_ARTIC_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        AnimateNuts();
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        AnimatePuffshroom();
    }
    else if (mSeedType == SeedType::SEED_GARLIC)
    {
        AnimateGarlic();
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        AnimatePumpkin();
    }
    else if (mSeedType == SEED_FIRESHROOM) {
        AnimateFireshroom();
    }

    UpdateBlink();

    if (mAnimPing)
    {
        if (mAnimCounter < mFrameLength * mNumFrames - 1)
        {
            mAnimCounter++;
        }
        else
        {
            mAnimPing = false;
            mAnimCounter -= mFrameLength;
        }
    }
    else if (mAnimCounter > 0)
    {
        mAnimCounter--;
    }
    else
    {
        mAnimPing = true;
        mAnimCounter += mFrameLength;
    }
    mFrame = mAnimCounter / mFrameLength;
}

//0x464EF0
float PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale)
{
    float aHeightOffset = -5.0f * theFlowerPotScale;
    float aScaleOffsetFix = 0.0f;

    switch (theSeedType)
    {
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_PLANTERN:
        aHeightOffset -= 5.0f;
        break;
    case SeedType::SEED_BRAVEYSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
        aHeightOffset += 5.0f;
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SUNSHROOM:
    case SeedType::SEED_PUFFSHROOM:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_HYPNOSHROOM:
    case SeedType::SEED_MAGNETSHROOM:
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SLINGPEA:
    case SeedType::SEED_SUNFLOWER:
    case SeedType::SEED_MARIGOLD:
    case SeedType::SEED_FROST_CABBAGE:
    case SeedType::SEED_BOMBBERRY:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_TANGLEKELP:
    case SeedType::SEED_BLOVER:
    case SeedType::SEED_SPIKEWEED:
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_POTATOMINE:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_LILYPAD:
        aScaleOffsetFix -= 16.0f;
        break;
    case SeedType::SEED_INSTANTCOFFEE:
        aScaleOffsetFix -= 20.0f;
        break;
    }

    return aHeightOffset + (theFlowerPotScale * aScaleOffsetFix - aScaleOffsetFix);
}

//0x465040
float PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow)
{
    float aHeightOffset = 0.0f;

    bool doFloating = false;
    if (Plant::IsFlying(theSeedType))
    {
        doFloating = false;
    }
    else if (theBoard == nullptr)
    {
        if (Plant::IsAquatic(theSeedType))
        {
            doFloating = true;
        }
    }
    else if (theBoard && (theBoard->StageHasPool() && theBoard->IsPoolSquare(theCol, theRow) || theBoard->GetWaterPotAt(theCol, theRow) && thePlant && (theSeedType == SEED_CATTAIL || theSeedType == SEED_SEASHROOM || theSeedType == SEED_TANGLEKELP)))
    {
        doFloating = true;
    }

    if (gLawnApp->mGameMode == GAMEMODE_CHALLENGE_ZEN_GARDEN && gLawnApp->mBoard && gLawnApp->mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
    {
        doFloating = Plant::IsAquatic(theSeedType);
    }

    if (thePlant && (!thePlant->IsInPlay() && Plant::IsFlying(thePlant->mSeedType) && thePlant->mSeedType != SEED_INSTANTCOFFEE)) {
        if (thePlant->mSquished) {
            return 0;
        }

        doFloating = true;
    }
    
    if (thePlant && (thePlant->mSeedType == SEED_SHOOTINGSTAR || thePlant->mSeedType == SEED_CHERRYBOMB || thePlant->mSeedType == SEED_FLYINGPOT || Plant::IsFlying(thePlant->mSeedType) && thePlant->mSeedType != SEED_INSTANTCOFFEE)) {
        if (thePlant->mSquished) {
            return 0;
        }

        doFloating = true;
    }
    
    bool notPreviewPots = thePlant && !Plant::IsFlying(thePlant->mSeedType) && thePlant->mSeedType != SEED_INSTANTCOFFEE && (thePlant->mIsPreview && thePlant->mSeedType != SEED_FLOWERPOT && thePlant->mSeedType != SEED_FLYINGPOT && thePlant->mSeedType != SEED_WATERPOT || !thePlant->mIsPreview) || !thePlant;
    bool highFloating = false;

    if (theBoard && (thePlant == nullptr || !thePlant->mSquished) && notPreviewPots)
    {
        Plant* aPot = theBoard->GetFlowerPotAt(theCol, theRow);
        if (aPot && theSeedType != SeedType::SEED_FLOWERPOT)
        {
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f);
        }

        Plant* aWaterPot = theBoard->GetWaterPotAt(theCol, theRow);
        if (aPot  && theSeedType != SeedType::SEED_WATERPOT)
        {
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f);
        }

        Plant* aFlyingPot = theBoard->GetFlyingPotAt(theCol, theRow);
        if (aFlyingPot && theSeedType != SeedType::SEED_FLYINGPOT)
        {
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f) - 30;
            doFloating = true;
            highFloating = true;
        }
    }

    if (thePlant && (thePlant->mSeedType == SEED_SHOOTINGSTAR || thePlant->mSeedType == SEED_CHERRYBOMB || thePlant->mSeedType == SEED_FLYINGPOT || Plant::IsFlying(thePlant->mSeedType) && thePlant->mSeedType != SEED_INSTANTCOFFEE))
    {
        highFloating = true;
    }

    if (doFloating)
    {
        int aCounter;
        if (theBoard)
        {
            if (thePlant && thePlant->mApp && thePlant->mApp->mGameScene != SCENE_PLAYING)
            {
                int mCutsceneTime = theBoard->mCutScene->mCutsceneTime;
                int mEndCutscene = 6000 + theBoard->mCutScene->mLawnMowerTime + theBoard->mCutScene->mSodTime + theBoard->mCutScene->mGraveStoneTime + theBoard->mCutScene->mCrazyDaveTime + theBoard->mCutScene->mFogTime + theBoard->mCutScene->mBossTime + theBoard->mCutScene->mReadySetPlantTime;

                aCounter = TodAnimateCurveFloat(mEndCutscene - 1000, mEndCutscene, mCutsceneTime, gLawnApp->mAppCounter % 200, theBoard->mMainCounter % 200, CURVE_LINEAR);
            }
            else
            {
                aCounter = theBoard->mMainCounter;
            }
        }
        else
        {
            aCounter = gLawnApp->mAppCounter;
        }

        float aPos = theRow * PI + theCol * 0.25f * PI;
        float aTime = aCounter * 2.0f * PI / 200.0f;
        float aFloatingHeight = sin(aPos + aTime) * 2.0f;
        if (highFloating)
            aFloatingHeight *= 2.0f;
        aHeightOffset += aFloatingHeight;
    }

    if (theSeedType == SeedType::SEED_FLOWERPOT || theSeedType == SeedType::SEED_WATERPOT)
    {
        aHeightOffset += 26.0f;
    }
    else if (theSeedType == SeedType::SEED_FLYINGPOT)
    {
        aHeightOffset += -4;
    }
    else if (theSeedType == SeedType::SEED_LILYPAD)
    {
        aHeightOffset += 25.0f;
    }
    else if (theSeedType == SeedType::SEED_STARFRUIT || theSeedType == SeedType::SEED_STARCICLE || theSeedType == SeedType::SEED_SHOOTINGSTAR)
    {
        aHeightOffset += 10.0f;
    }
    else if (theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aHeightOffset += 24.0f;
    }
    else if (theSeedType == SeedType::SEED_SEASHROOM)
    {
        aHeightOffset += 28.0f;
    }
    else if (theSeedType == SeedType::SEED_INSTANTCOFFEE)
    {
        aHeightOffset -= 20.0f;
    }
    //else if (Plant::IsFlying(theSeedType))
    //{
    //    aHeightOffset -= 30.0f;
    //}
    else if (theSeedType == SeedType::SEED_CACTUS)
    {
        return aHeightOffset;
    }
    else if (theSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aHeightOffset += 15.0f;
    }
    else if (theSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aHeightOffset += 5.0f;
    }
    else if (theSeedType == SeedType::SEED_SCAREDYSHROOM || theSeedType == SeedType::SEED_BRAVEYSHROOM)
    {
        aHeightOffset -= 14.0f;
    }
    else if (theSeedType == SeedType::SEED_GRAVEBUSTER)
    {
        aHeightOffset -= 40.0f;
    }
    else if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
    {
        int aBottomRow = 4;
        if (theBoard && theBoard->StageHas6Rows())
        {
            aBottomRow = 5;
        }

        if (theSeedType == SeedType::SEED_SPIKEROCK)
        {
            aHeightOffset += 6.0f;
        }

        if ((theBoard && (theBoard->GetFlowerPotAt(theCol, theRow) || theBoard->GetWaterPotAt(theCol, theRow) || theBoard->GetFlyingPotAt(theCol, theRow))) && gLawnApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            aHeightOffset += 5.0f;
        }
        else if (theBoard && theBoard->StageHasRoof())
        {
            aHeightOffset += 15.0f;
        }
        else if (theBoard && theBoard->IsPoolSquare(theCol, theRow))
        {
            aHeightOffset += 0.0f;
        }
        else if (theRow == aBottomRow && theCol <= 9)
        {
            aHeightOffset += 12.0f;
        }
        else
        {
            aHeightOffset += 15.0f;
        }
    }
    else if (theSeedType == SEED_CATTAIL)
    {
        aHeightOffset += 7.5f;
    }
    
    if (theBoard && theBoard->GetWaterPotAt(theCol, theRow) && theSeedType != SEED_WATERPOT && notPreviewPots)
    {
        aHeightOffset -= 15;
        if (theSeedType == SEED_SEASHROOM || theSeedType == SEED_TANGLEKELP)
            aHeightOffset -= 7.5f;
    }

    if (thePlant && thePlant->mIsPreview) {
        if (theSeedType == SeedType::SEED_FLOWERPOT || theSeedType == SeedType::SEED_WATERPOT) {
            aHeightOffset -= 12.5f;
        }
        else if (theSeedType == SeedType::SEED_FLYINGPOT)
        {
            aHeightOffset -= 42.5f;
        }
        else if (theSeedType == SeedType::SEED_LILYPAD) {
            aHeightOffset -= 12.5f;
        }
    }

    

    return aHeightOffset;
}

//0x465380
void Plant::GetPeaHeadOffset(int& theOffsetX, int& theOffsetY)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    int aTrackIndex = 0;
    if (aBodyReanim->TrackExists("anim_stem"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_stem");
    }
    else if(aBodyReanim->TrackExists("anim_idle"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_idle");
    }

    ReanimatorTransform aTransform;
    aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
    theOffsetX = aTransform.mTransX;
    theOffsetY = aTransform.mTransY;
}

//0x465460
void Plant::DrawMagnetItems(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];

        float drawX = 0;
        float drawY = 0;

        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            int aCelRow = 0, aCelCol = 0;
            Image* aImage = nullptr;
            float aScale = 0.8f;

            if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_FOOTBALL_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_HELMET;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_FOOTBALL_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_FOOTBALL_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIVER_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIVER1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIVER_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIVER2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIVER_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIVER3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
            }
            else if (aMagnetItem->mItemType >= MagnetItemType::MAGNET_ITEM_POGO_1 && aMagnetItem->mItemType <= MagnetItemType::MAGNET_ITEM_POGO_3)
            {
                aCelCol = (int)aMagnetItem->mItemType - (int)MagnetItemType::MAGNET_ITEM_POGO_1;
                aImage = IMAGE_ZOMBIEPOGO;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_PLACED)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_5;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX)
            {
                aImage = IMAGE_REANIM_ZOMBIE_JACKBOX_BOX;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PICK_AXE)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SILVER_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_SILVER_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_GOLD_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_GOLD_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIAMOND)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else
            {
                TOD_ASSERT();
            }

            if (aScale == 1.0f)
            {
                g->DrawImageCel(aImage, aMagnetItem->mPosX - mX + aOffsetX + drawX, aMagnetItem->mPosY - mY + aOffsetY + drawY, aCelCol, aCelRow);
            }
            else
            {
                TodDrawImageCelScaledF(g, aImage, aMagnetItem->mPosX - mX + aOffsetX + drawX, aMagnetItem->mPosY - mY + aOffsetY + drawY, aCelCol, aCelRow, aScale, aScale);
            }
        }
    }
}

Image* Plant::GetImage(SeedType theSeedType)
{
    Image** aImages = GetPlantDefinition(theSeedType).mPlantImage;
    return aImages ? aImages[0] : nullptr;
}

//0x465680
void Plant::DrawShadow(Sexy::Graphics* g, float theOffsetX, float theOffsetY)
{
    if (mSeedType == SeedType::SEED_LILYPAD || mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_SHOOTINGSTAR || mSeedType == SeedType::SEED_STARCICLE || mSeedType == SeedType::SEED_TANGLEKELP ||
        mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_COBCANNON || mSeedType == SeedType::SEED_SPIKEWEED ||
        mSeedType == SeedType::SEED_SPIKEROCK || mSeedType == SeedType::SEED_GRAVEBUSTER || mSeedType == SeedType::SEED_CATTAIL || mSeedType == SeedType::SEED_SEEDROOT || 
        mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE)
        return;

    if (Plant::IsFlying(mSeedType))
    {
        if (IsOnBoard()) {
            if (mApp->mGameMode == GAMEMODE_CHALLENGE_ZEN_GARDEN) 
            {
                return;
            }

            PlantsOnLawn aPlantOnLawn;
            mBoard->GetPlantsOnLawn(mPlantCol, mRow, &aPlantOnLawn);
            if ((aPlantOnLawn.mNormalPlant || aPlantOnLawn.mPadPlant || aPlantOnLawn.mPotPlant || aPlantOnLawn.mPumpkinPlant) && Plant::IsFlying(mSeedType) && mSeedType != SEED_INSTANTCOFFEE)
            {
                return;
            }
        }
    }

    if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mZenGarden->mGardenType == GardenType::GARDEN_MAIN)
        return;

    if (mIsPreview)  return;

    int aShadowType = 0;
    float aShadowOffsetX = -3.0f;
    float aShadowOffsetY = 51.0f;
    float aScale = 1.0f;

    if (mBoard && mBoard->StageIsNight())
    {
        aShadowType = 1;
    }

    if (mSeedType == SeedType::SEED_SQUASH)
    {
        if (mBoard)
        {
            aShadowOffsetY += mBoard->GridToPixelY(mPlantCol, mRow) - mY;
        }
        aShadowOffsetY += 5.0f;
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aScale = 0.5f;
        aShadowOffsetY = 42.0f;
    }
    else if (mSeedType == SeedType::SEED_SUNSHROOM)
    {
        aShadowOffsetY = 42.0f;
        if (mState == PlantState::STATE_SUNSHROOM_SMALL)
        {
            aScale = 0.5f;
        }
        else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            aScale = 0.5f + 0.5f * aBodyReanim->mAnimTime;
        }
    }
    else if (mSeedType == SeedType::SEED_UMBRELLA)
    {
        aScale = 0.5f;
        if (mEnemy)
            aShadowOffsetX = 7.0f;
        else
            aShadowOffsetX = -7.0f;
        aShadowOffsetY = 52.0f;
    }
    else if (mSeedType == SeedType::SEED_SUNBEAN) {
        aScale = 0.75f;
    }
    else if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM || mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        aScale = 1.3f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_BOMBBERRY || mSeedType == SeedType::SEED_SPORE_SHROOM || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_FROST_CABBAGE || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT)
    {
        aShadowOffsetX = 0.0f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM)
    {
        if (mEnemy)
            aShadowOffsetX = 9.0f;
        else
            aShadowOffsetX = -9.0f;
        aShadowOffsetY = 55.0f;
    }
    else if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER)
    {
        if (mEnemy)
            aShadowOffsetX = 21.0f;
        else
            aShadowOffsetX = -21.0f;
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_WATERPOT)
    {
        if (mEnemy)
            aShadowOffsetX = 4.0f;
        else
            aShadowOffsetX = -4.0f;
        aShadowOffsetY = 46.0f;
    }
    else if (mSeedType == SeedType::SEED_FLYINGPOT)
    {
        if (mEnemy)
            aShadowOffsetX = 4.0f;
        else
            aShadowOffsetX = -4.0f;
        aShadowOffsetY = 46.0f;
        aShadowOffsetY -= PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow) - 22;
    }
    else if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aShadowOffsetY = 54.0f;
        aScale = 1.3f;
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aShadowOffsetY = 46.0f;
        aScale = 1.4f;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        if (mEnemy)
            aShadowOffsetX = 8.0f;
        else
            aShadowOffsetX = -8.0f;
        aShadowOffsetY = 50.0f;
    }
    else if (mSeedType == SeedType::SEED_PLANTERN)
    {
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_INSTANTCOFFEE)
    {
        aShadowOffsetY = 71.0f;
    }
    else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
       // aShadowOffsetX = -33.0f;
        aShadowOffsetY = 56.0f;
        aScale = 1.7f;
    }else if (mSeedType == SEED_CHERRYHOVERBOMB) {
        if (!IsInPlay())
        {
            aShadowOffsetY -= PlantDrawHeightOffset(mBoard, this, SEED_CHERRYHOVERBOMB, mPlantCol, mRow) - 32;
        }

        aScale = TodAnimateCurveFloat(100, 0, mDoSpecialCountdown, 0, 1, CURVE_EASE_IN);
        if (mEnemy)
            aShadowOffsetX += 25;
        else
            aShadowOffsetX -= 25;
        aShadowOffsetY += TodAnimateCurveFloat(100, 0, mDoSpecialCountdown, 100, 0, CURVE_EASE_IN);
    }
    else if (mSeedType == SEED_STINGER) {
        aScale = 0.6f;
        if (mEnemy)
            aShadowOffsetX -= 6;
        else
            aShadowOffsetX += 6;
        aShadowOffsetY -= PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    }
    else if (mSeedType == SEED_ICEBERG_LETTUCE) {
        aScale = 0.75f;
    }

    if (Plant::IsFlying(mSeedType))
    {
        aShadowOffsetY += 10.0f;
        if (mBoard && (mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_NORMAL_POSITION) || mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_PUMPKIN)))
            return;
    }

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
    {
        if (mApp->mGameScene == SCENE_PLAYING && mBoard->mChallenge->mChallengeStateCounter <= 500 && mBoard->mChallenge->mChallengeStateCounter >= 0)
        {
            g->SetColorizeImages(true);
            g->SetColor(Color(255, 255, 255, 255 - 255 * (500 - mBoard->mChallenge->mChallengeStateCounter) / 500));
            if (mBoard->StageIsNight())
                TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
            else
                TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);

            g->SetColorizeImages(true);
            g->SetColor(Color(255, 255, 255, 255 * (500 - mBoard->mChallenge->mChallengeStateCounter) / 500));
            if (!mBoard->StageIsNight())
                TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
            else
                TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
            g->SetColorizeImages(false);
        }
        else
            TodDrawImageCelCenterScaledF(g, aShadowType == 0 ? IMAGE_PLANTSHADOW : IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
    else {
        TodDrawImageCelCenterScaledF(g, aShadowType == 0 ? IMAGE_PLANTSHADOW : IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
}

//0x465A90
void Plant::Draw(Graphics* g)
{
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL_REVERSE)
        return;

    g->mTransY += mBowlingSlope;

    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

    if (Plant::IsFlying(mSeedType) && mSquished)
    {
        aOffsetY += 30.0f;
    }

    Color aSpotLightColor;
    if (mState == STATE_SINGING) {
        switch (mPlantAge >= 700 ? mPlantAge / 100 * 7 % 5 : 0)
        {
        case 0:
            aSpotLightColor = Color(250, 250, 160);
            break;
        case 1:
            aSpotLightColor = Color(114, 234, 170);
            break;
        case 2:
            aSpotLightColor = Color(216, 126, 202);
            break;
        case 3:
            aSpotLightColor = Color(90, 110, 140);
            break;
        case 4:
            aSpotLightColor = Color(240, 90, 130);
            break;
        }
        g->SetColorizeImages(true);
        g->SetColor(aSpotLightColor);
        TodDrawImageScaledF(g, IMAGE_SPOTLIGHT2, -61, 30 + aOffsetY, 4.0f, 4.0f);
        g->SetColorizeImages(false);

    }

    if (mIceTrapCounter > 0) {
        TodDrawImageScaledF(g, IMAGE_ICETRAP2, (!mEnemy ? 73 : 12), 50 + aOffsetY, (!mEnemy ? -1.0f : 1.0f), 1.0f);
    }

    /*if (IsOnBoard() && mSeedType == SEED_LILYPAD && mBoard && mBoard->mPlantRow[mRow] == PLANTROW_POOL) {
        Graphics reflection(*g);
        reflection.mTransY = 42.5f;
        reflection.mColor.mAlpha = 64;
        reflection.mScaleY = -1;


        Plant* mNormalPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_DIGGING_ORDER);
        if (mNormalPlant && mNormalPlant != this)
        {
            int aOffsetX, aOffsetY, aWidth, aHeight;
            mApp->mReanimatorCache->GetPlantImageSize(mSeedType, aOffsetX, aOffsetY, aWidth, aHeight);
            aWidth += 120;
            aHeight += 120;
            MemoryImage* aMemoryImage = mApp->mReanimatorCache->MakeBlankMemoryImage(aWidth, aHeight);
            Graphics aMemoryGraphics(aMemoryImage);
            aMemoryGraphics.SetLinearBlend(true);

            aMemoryGraphics.mTransX -= mX - mNormalPlant->mX - 60;
            aMemoryGraphics.mTransY -= mY - mNormalPlant->mY - 60;

            if (mNormalPlant->BeginDraw(&aMemoryGraphics)) {
                mNormalPlant->Draw(&aMemoryGraphics);
                mNormalPlant->EndDraw(&aMemoryGraphics);
            }

            if (!mApp->Is3dAccel() && reflection.mScaleX == 1.0f && reflection.mScaleY == 1.0f)
                reflection.DrawImage(aMemoryImage, mX - mNormalPlant->mX + 15 - 60 + aOffsetX, mY +- mNormalPlant->mY + 20 - 60 + aOffsetY, aWidth, aHeight);
            else
                TodDrawImageScaledF(&reflection, aMemoryImage, mX - mNormalPlant->mX + 15 - 60 + aOffsetX * reflection.mScaleX, mY - mNormalPlant->mY + 20 - 60 + aOffsetY * reflection.mScaleY, reflection.mScaleX, reflection.mScaleY);

            aMemoryImage->DeleteSWBuffers();
            aMemoryImage->Delete3DBuffers();
            aMemoryImage->DeleteExtraBuffers();
            aMemoryImage->DeleteNativeData();
            delete aMemoryImage;
            aMemoryImage = nullptr;
        }

        Plant* mPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_PUMPKIN);
        if (mPumpkin && mPumpkin != this)
        {
            if (mPumpkin->BeginDraw(&aMemoryGraphics)) {
                mPumpkin->Draw(&aMemoryGraphics);
                mPumpkin->EndDraw(&aMemoryGraphics);
            }
        }
    }*/

    int aImageIndex = mFrame;
    Image* aPlantImage = Plant::GetImage(mSeedType);

    if (mSquished)
    {
        if (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_FLYINGPOT || mSeedType == SeedType::SEED_WATERPOT)
        {
            aOffsetY -= 15.0f;
        }
        if (mSeedType == SeedType::SEED_INSTANTCOFFEE)
        {
            aOffsetY -= 20.0f;
        }

        g->SetScale(1.0f, 0.25f, 0.0f, 0.0f);
        DrawSeedType(g, mSeedType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, 60.0f + aOffsetY);
        g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
    }
    else
    {
        bool aDrawPumpkinBack = false;
        Plant* aPumpkin = nullptr;

        if (IsOnBoard())
        {
            aPumpkin = mBoard->GetPumpkinAt(mPlantCol, mRow);
            if (aPumpkin)
            {
                Plant* aPlantInPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
                if (aPlantInPumpkin)
                {
                    if (aPlantInPumpkin->mRenderOrder > aPumpkin->mRenderOrder || aPlantInPumpkin->mOnBungeeState == GETTING_GRABBED_BY_BUNGEE)
                    {
                        aPlantInPumpkin = nullptr;
                    }
                }

                if (aPlantInPumpkin == this)
                {
                    aDrawPumpkinBack = true;
                }
                if (aPlantInPumpkin == nullptr && mSeedType == SeedType::SEED_PUMPKINSHELL)
                {
                    aDrawPumpkinBack = true;
                }
            }
            else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
            {
                aDrawPumpkinBack = true;
                aPumpkin = this;
            }
        }
        else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
        {
            aDrawPumpkinBack = true;
            aPumpkin = this;
        }

        DrawShadow(g, aOffsetX, aOffsetY);

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL && mSeedType == SEED_UMBRELLA)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                aBodyReanim->mColorOverride = Color(255, 255, 255, 128);
            }
        }

        if (Plant::IsFlying(mSeedType))
        {
            int aCounter;
            if (IsOnBoard())
            {
                aCounter = mBoard->mMainCounter;
            }
            else
            {
                aCounter = mApp->mAppCounter;
            }

            float aTime = (mRow * 97 + mPlantCol * 61 + aCounter) * 0.03f;
            float aWave = sin(aTime) * 2.0f;
            aOffsetY += aWave;
        }

        if (aDrawPumpkinBack)
        {
            Reanimation* aPumpkinReanim = mApp->ReanimationGet(aPumpkin->mBodyReanimID);
            Graphics aPumpkinGraphics(*g);
            aPumpkinGraphics.mTransX += aPumpkin->mX - mX;
            aPumpkinGraphics.mTransY += aPumpkin->mY - mY;
            aPumpkinReanim->DrawRenderGroup(&aPumpkinGraphics, 1);
        }

        aOffsetX += mShakeOffsetX;
        aOffsetY += mShakeOffsetY;
        if (IsInPlay() && mApp->IsIZombieLevel())
        {
            mBoard->mChallenge->IZombieDrawPlant(g, this);
        }
        else if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                /*if (!mApp->Is3dAccel() && mSeedType == SeedType::SEED_FLOWERPOT && IsOnBoard() &&
                    aBodyReanim->mAnimRate == 0.0f && aBodyReanim->IsAnimPlaying("anim_idle")){
                    mApp->mReanimatorCache->DrawCachedPlant(g, aOffsetX, aOffsetY, mSeedType, DrawVariation::VARIATION_NORMAL);
                }
                else if (!mApp->Is3dAccel() && mSeedType == SeedType::SEED_WATERPOT && IsOnBoard() &&
                    aBodyReanim->mAnimRate == 0.0f && aBodyReanim->IsAnimPlaying("anim_idle")){
                    mApp->mReanimatorCache->DrawCachedPlant(g, aOffsetX, aOffsetY, mSeedType, DrawVariation::VARIATION_ZEN_GARDEN_WATER);
                }
                else*/
                //{  

                aBodyReanim->Draw(g);

                //}
            }
        }
        else
        {
            SeedType aSeedType = SeedType::SEED_NONE;
            if (mBoard)
            {
                aSeedType = mBoard->GetSeedTypeInCursor();
            }

            if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (aSeedType == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mBoard && mBoard->mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }

            TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
            g->SetColorizeImages(false);
            if (mHighlighted)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 196));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
            else if (mEatenFlashCountdown > 0)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, ClampInt(mEatenFlashCountdown * 3, 0, 255)));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
        }

        if (mSeedType == SeedType::SEED_MAGNETSHROOM && !DrawMagnetItemsOnTop())
        {
            DrawMagnetItems(g);
        }

        if (mSeedType == SEED_SEEDROOT && mRootSeedType != SEED_NONE) {
            float percentDark = (float)(mRootMaxTime - mRootTime) / (float)mRootMaxTime;
            int grayness = 255;
            if (!mBoard->CanTakeSunMoney(Plant::GetCost(mRootSeedType)) || percentDark > 1.0f || !mBoard->PlantingRequirementsMet(mRootSeedType))
                grayness = 128;
            Graphics seedrootG(*g);
            seedrootG.mTransX += 15;
            seedrootG.mTransY -= 10;
            DrawSeedPacket(&seedrootG, 0, aOffsetY, mRootSeedType, mRootImitaterType, percentDark, grayness, true, true, false);

            /*if (mRootTime == mRootMaxTime) {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

                for (int i = 0; i < 2; i++) {
                    Coin* aCoin = mBoard->mCoins.DataArrayAlloc();
                    aCoin->CoinInitialize(mX, mY, COIN_USABLE_SEED_PACKET, COIN_MOTION_FROM_PLANT);
                    aCoin->mUsableSeedType = mRootSeedType;
                    aCoin->mUsableImitaterSeedType = mRootImitaterType;
                }
                mRootSeedType = mImitaterType = SEED_NONE;
            }*/
        }
    }

    if (mButteredCounter > 0){
        TodDrawImageScaledF(g, IMAGE_REANIM_CORNPULT_BUTTER_SPLAT, (!mEnemy ? 73 : 12), -12 + aOffsetY, (!mEnemy ? -1.0f : 1.0f), 1.0f);
    }

    if (mIceTrapCounter > 0) {
        TodDrawImageScaledF(g, IMAGE_ICETRAP, (!mEnemy ? 73 : 12), 50 + aOffsetY, (!mEnemy ? -1.0f : 1.0f), 1.0f);
    }

    if (mState == STATE_SINGING) {
        g->SetColorizeImages(true);
        g->SetColor(aSpotLightColor);
        TodDrawImageScaledF(g, IMAGE_SPOTLIGHT, -61, -510 + aOffsetY, 4.0f, 4.0f);
        g->SetColorizeImages(false);
    }

    g->mTransY -= mBowlingSlope;
}

//0x4660B0
void Plant::DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY)
{
    Graphics aSeedG(*g);
    int aCelRow = 0;
    int aCelCol = 2;
    float aOffsetX = 0.0f;
    float aOffsetY = 0.0f;
    SeedType aSeedType = theSeedType;
    DrawVariation aDrawVariation = theDrawVariation;

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        aSeedType = theImitaterType;
        aDrawVariation = DrawVariation::VARIATION_IMITATER;
        if (theImitaterType == SeedType::SEED_HYPNOSHROOM || theImitaterType == SeedType::SEED_SQUASH || theImitaterType == SeedType::SEED_POTATOMINE ||
            theImitaterType == SeedType::SEED_GARLIC || theImitaterType == SeedType::SEED_LILYPAD)
            aDrawVariation = DrawVariation::VARIATION_IMITATER_LESS;
    }
    else if (theDrawVariation == DrawVariation::VARIATION_NORMAL && theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aDrawVariation = DrawVariation::VARIATION_AQUARIUM;
    }

    if (((LawnApp*)gSexyAppBase)->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME &&
        (aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_SUNFLOWER || aSeedType == SeedType::SEED_MARIGOLD))
    {
        aSeedG.mScaleX *= 1.5f;
        aSeedG.mScaleY *= 1.5f;
        aOffsetX = -20.0f;
        aOffsetY = -40.0f;
    }

    if (aSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += aSeedG.mScaleX * 80.0f;
        aSeedG.mScaleX *= -1.0f;
    }

    if (Challenge::IsZombieSeedType(aSeedType))
    {
        ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedType);
        gLawnApp->mReanimatorCache->DrawCachedZombie(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aZombieType);
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(aSeedType);

        if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
        {
            gLawnApp->mReanimatorCache->DrawCachedPlant(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aSeedType, aDrawVariation);
        }
        else
        {
            if (aSeedType == SeedType::SEED_KERNELPULT)
            {
                aCelRow = 2;
            }
            else if (aSeedType == SeedType::SEED_TWINSUNFLOWER)
            {
                aCelRow = 1;
            }

            Image* aPlantImage = Plant::GetImage(aSeedType);
            if (aPlantImage->mNumCols <= 2)
            {
                aCelCol = aPlantImage->mNumCols - 1;
            }

            TodDrawImageCelScaledF(&aSeedG, aPlantImage, thePosX + aOffsetX, thePosY + aOffsetY, aCelCol, aCelRow, aSeedG.mScaleX, aSeedG.mScaleY);
        }
    }
}

//0x466390
void Plant::MouseDown(int x, int y, int theClickCount)
{
    if (theClickCount < 0 || !IsOnBoard())
        return;

    if (mSeedType == SEED_MAGNIFYINGGRASS && mState == STATE_READY) {
        if (mBoard->TakeSunMoney(50)) {
            PlayBodyReanim("anim_shooting", REANIM_LOOP, 20, 12);
            mState = STATE_MAGNIFYING_GRASS_ATTACKING;
        }
    }

    if (mSeedType == SEED_SEEDROOT && mRootSeedType != SEED_NONE && mRootTime == mRootMaxTime && mBoard->CanTakeSunMoney(Plant::GetCost(mRootSeedType)) &&  mBoard->PlantingRequirementsMet(mRootSeedType)) {
        mBoard->ClearCursor();
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR;
        mBoard->mCursorObject->mSeedBankIndex = -1;
        mBoard->mCursorObject->mType = mRootSeedType;
        mBoard->mCursorObject->mImitaterType = mRootImitaterType;
        mBoard->mCursorObject->mRootPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
    }

    if (mSeedType == SEED_EINSTEINAPPLE)
    {
        //mBoard->LoadGame(GetAppDataFolder() + StrFormat("userdata\\game%d_", mApp->mPlayerInfo->mId) + "timeApple.dat");
        mApp->PlayFoley(FOLEY_EXPLOSION);
        Die();
    }

    if (mState == PlantState::STATE_COBCANNON_READY)
    {
        mBoard->ClearCursor();
        mBoard->mCursorObject->mType = SeedType::SEED_NONE;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_COBCANNON_TARGET;
        mBoard->mCursorObject->mSeedBankIndex = -1;
        mBoard->mCursorObject->mCoinID = CoinID::COINID_NULL;
        mBoard->mCursorObject->mCobCannonPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
        mBoard->mCobCannonCursorDelayCounter = 30;
        mBoard->mCobCannonMouseX = x;
        mBoard->mCobCannonMouseY = y;
    }

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE){
        mBoard->mCursorObject->mType = this->mSeedType;
        mBoard->mCursorObject->mImitaterType = this->mImitaterType;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE;
        mBoard->mCursorObject->mGlovePlantID = (PlantID) mBoard->mPlants.DataArrayGetID(this);
    }
}

//0x466420
void Plant::IceZombies()
{
    if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mEnemy == mEnemy || aPlant->mIsDummy) continue;

            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
            if (!aPlant->CanBeChilled())
            {
                aPlant->mIceTrapCounter = 600;
                aPlant->ApplyChill(true);
            }
            aPlant->mPlantHealth -= 20;
            aPlant->mEatenFlashCountdown = 25;
        }
    }
    else {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            aZombie->HitIceTrap();
        }
    }
   
    mBoard->mIceTrapCounter = 300;
    TodParticleSystem* aPoolSparklyParticle = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleID);
    if (aPoolSparklyParticle)
    {
        aPoolSparklyParticle->mDontUpdate = false;
    }

    TodParticleSystem* aPoolSparklyParticle2 = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleIDExtra2);
    if (aPoolSparklyParticle2)
    {
        aPoolSparklyParticle2->mDontUpdate = false;
    }

    TodParticleSystem* aPoolSparklyParticle3 = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleIDExtra3);
    if (aPoolSparklyParticle3)
    {
        aPoolSparklyParticle3->mDontUpdate = false;
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie)
    {
        aBossZombie->BossDestroyFireball();
    }
}

//0x4664B0
void Plant::BurnRow(int theRow)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (mEnemy == aPlant->mEnemy || aPlant->mRow != theRow || aPlant->mIsDummy) continue;
            aPlant->ApplyBurn();
        }
    }
    else {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(aDamageRangeFlags))
            {
                aZombie->RemoveColdEffects();
                aZombie->ApplyBurn();
            }
        }
    }

    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridY == theRow && aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
        {
            aGridItem->GridItemDie();
        }
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie && aBossZombie->mFireballRow == theRow)
    {
        // 注：原版中将 Zombie::BossDestroyIceballInRow(int) 函数改为了 Zombie::BossDestroyIceball()，冰球是否位于目标行的判断则移动至此处进行
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}


//0x4665B0
void Plant::BlowAwayFliers(int theX, int theRow)
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying())
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (aZombie->IsFlying())
            {
                aZombie->mBlowingAway = true;
            }
        }
    }

    mApp->PlaySample(SOUND_BLOVER);
    mBoard->mFogBlownCountDown = 4000;
}

//0x466650
void Plant::KillAllPlantsNearDoom()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && aPlant->mPlantCol == mPlantCol)
        {
            aPlant->Die();
        }
    }
}

//0x4666A0
void Plant::DoSpecial()
{
    if (mState == STATE_BURNED)
        return;

    int aPosX = mX + mWidth / 2;
    int aPosY = mY + mHeight / 2;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    switch (mSeedType)
    {
    case SeedType::SEED_ICEBERG_LETTUCE:
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aRowDist = aZombie->mRow - mRow;

            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                aRowDist = 0;

            if (aRowDist <= 1 && aRowDist >= -1 && GetCircleRectOverlap(mX + mWidth / 2.0f, mY + mHeight / 2.0f, 115, aZombieRect))
            {
                aZombie->HitIceTrap();
            }
        }

        Die();
        break;
    }
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SUNBLOOM:
    {
        Die();
        break;
    }
    case SeedType::SEED_HURRIKALE:
    {
        if (mState != PlantState::STATE_DOINGSPECIAL)
        {
            mState = PlantState::STATE_DOINGSPECIAL;
            BlowAwayFliers(mX, mRow);
            if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
                Plant* aPlant = nullptr;
                while (mBoard->IteratePlants(aPlant)) {
                    if (aPlant->mEnemy == mEnemy) continue;
                    if (aPlant->mRow == mRow && !aPlant->CanBeChilled()) {
                        aPlant->ApplyChill(false);
                    }
                }
            }else
            {
                Zombie* aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie)) {
                    if (aZombie->mRow == mRow && aZombie->mZombieType != ZOMBIE_BOSS && aZombie->mZombieType != ZOMBIE_ZAMBONI && aZombie->mZombieType != ZOMBIE_CATAPULT) {
                        aZombie->ApplyChill(false);
                        aZombie->mHurrikaleTime = 300;
                    }
                }
            }
        }
        
        break;
    }
    case SeedType::SEED_BLOVER:
    {
        if (mState != PlantState::STATE_DOINGSPECIAL)
        {
            mState = PlantState::STATE_DOINGSPECIAL;
            BlowAwayFliers(mX, mRow);
        }
        break;
    }
    case SeedType::SEED_CHERRYHOVERBOMB:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        int offsetX = 25;
        if (mEnemy)
            offsetX = -25;

        mBoard->KillAllPlantsInRadius(mRow, aPosX - offsetX, aPosY, 12.5f, 1, true, aDamageRangeFlags, mEnemy);
        mBoard->KillAllZombiesInRadius(mRow, aPosX - offsetX, aPosY, 12.5f, 1, true, aDamageRangeFlags);
        TodParticleSystem* bombPart = mApp->AddTodParticle(aPosX - offsetX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_CHERRYBOMB:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);

        mBoard->KillAllPlantsInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags, mEnemy);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags);

        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_DOOMSHROOM:
    {
        mApp->PlaySample(SOUND_DOOMSHROOM);

        mBoard->KillAllPlantsInRadius(mRow, aPosX, aPosY, 250, 3, true, aDamageRangeFlags, mEnemy);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 250, 3, true, aDamageRangeFlags);
        KillAllPlantsNearDoom();

        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
        mBoard->AddACrater(mPlantCol, mRow)->mGridItemCounter = 18000;
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_JALAPENO:
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        mBoard->DoFwoosh(mRow);
        mBoard->ShakeBoard(3, -4);

        BurnRow(mRow);
        if (mBoard->mIceTimer[mRow] > 20)
            mBoard->mIceTimer[mRow] = 20;

        Die();
        break;
    }
    case SeedType::SEED_UMBRELLA:
    {
        if (mState != PlantState::STATE_UMBRELLA_TRIGGERED && mState != PlantState::STATE_UMBRELLA_REFLECTING)
        {
            mState = PlantState::STATE_UMBRELLA_TRIGGERED;
            mStateCountdown = 5;

            PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
        }

        break;
    }
    case SeedType::SEED_ICESHROOM:
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        IceZombies();
        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

        Die();
        break;
    }
    case SeedType::SEED_EINSTEINAPPLE:
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        mBoard->SaveCurrentTime("timeApple");
        Die();
        break;
    }
    case SeedType::SEED_POTATOMINE:
    {
        aPosX = mX + mWidth / 2 - 20;
        aPosY = mY + mHeight / 2;

        mApp->PlaySample(SOUND_POTATO_MINE);
        mBoard->KillAllPlantsInRadius(mRow, aPosX, aPosY, 60, 0, false, aDamageRangeFlags, mEnemy);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 60, 0, false, aDamageRangeFlags);

        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_INSTANTCOFFEE:
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        if (aPlant && aPlant->mIsAsleep)
        {
            aPlant->mWakeUpCounter = 100;
            aPlant->mCoffeebeaned = true;
        }

        mState = PlantState::STATE_DOINGSPECIAL;
        PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
        mApp->PlayFoley(FoleyType::FOLEY_COFFEE);
        break;
    }
    case SeedType::SEED_EXPLODE_O_NUT:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);

        int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
        mBoard->KillAllPlantsInRadius(mRow, aPosX, aPosY, 115, (mApp->IsWallnutBowlingLevel() ? 1 : 0), true, aDamageRangeFlags, mEnemy);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, (mApp->IsWallnutBowlingLevel() ? 1 : 0), true, aDamageRangeFlags);
        mApp->AddTodParticle(aPosX, aPosY , (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);
        Die();
        break;
    }
    }
}

//0x466B80
void Plant::ImitaterMorph()
{
    Plant* aPlant = mBoard->AddPlant(mPlantCol, mRow, mImitaterType, SeedType::SEED_IMITATER);
    aPlant->mEnemy = mEnemy;

    Die();
    
    FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
    if (mImitaterType == SeedType::SEED_HYPNOSHROOM || mImitaterType == SeedType::SEED_SQUASH || mImitaterType == SeedType::SEED_POTATOMINE ||
        mImitaterType == SeedType::SEED_GARLIC || mImitaterType == SeedType::SEED_LILYPAD)
        aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->mFilterEffect = aFilter;
    }
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(aPlant->mHeadReanimID);
    if (aHeadReanim)
    {
        aHeadReanim->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID2);
    if (aHeadReanim2)
    {
        aHeadReanim2->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID3);
    if (aHeadReanim3)
    {
        aHeadReanim3->mFilterEffect = aFilter;
    }
}

//0x466CA0
void Plant::UpdateImitater()
{
    if (mState != PlantState::STATE_IMITATER_MORPHING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_IMITATER_MORPHING;
            PlayBodyReanim("anim_explode", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 26.0f);
        }
    }
    else
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.8f))
        {
            mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_IMITATER_MORPH);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            ImitaterMorph();
        }
    }
}

//0x466D50
void Plant::CobCannonFire(int theTargetX, int theTargetY)
{
    TOD_ASSERT(mState == PlantState::STATE_COBCANNON_READY);

    mState = PlantState::STATE_COBCANNON_FIRING;
    mShootingCounter = 206;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);

    mTargetX = theTargetX - 47.0f;
    mTargetY = theTargetY;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("CobCannon_Cob");
    aTrackInstance->mTrackColor = Color::White;
}

bool Plant::CanBeChilled() {
    return mSeedType == SEED_ICESHROOM || mSeedType == SEED_SNOWPEA || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_WINTERMELON || mSeedType == SEED_FROST_CABBAGE || mSeedType == SEED_STARCICLE || mSeedType == SEED_ICYFUMESHROOM;
}

void Plant::Fire(void* theTarget, int theRow, PlantWeapon thePlantWeapon)
{
    Plant* theTargetPlant = (Plant*)theTarget;
    Zombie* theTargetZombie = (Zombie*)theTarget;

    if (mSeedType == SEED_PUFFSHROOM && mState != STATE_PUFFSHROOM_FADING) {
        mState = STATE_PUFFSHROOM_FADING;
        mDoSpecialCountdown = 12000;
    }

    if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM)
    {
        DoRowAreaDamage(20, 2U);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        DoRowAreaDamage(20, 2U);
        return;
    }
    if (mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_STARCICLE)
    {
        StarFruitFire();
        return;
    }
    if (mSeedType == SeedType::SEED_SHOOTINGSTAR) {
        mApp->PlayFoley(FOLEY_ASRIEL_SHOOTINGSTAR);
        unsigned int aDamageRangeFlags = GetDamageRangeFlags(WEAPON_SECONDARY);
        mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        for (int gridY = 0; gridY < MAX_GRID_SIZE_Y; gridY++) {
            Zombie* aZombie = FindTargetZombie(gridY, WEAPON_PRIMARY);
            if (aZombie) {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (aZombie->EffectedByDamage(aDamageRangeFlags))
                {
                    Projectile* aProjectile = mBoard->AddProjectile(aZombie->ZombieTargetLeadX(50.0f) - 330, aZombieRect.mY - 300, mRenderOrder - 1, theRow, PROJECTILE_SHOOTING_STAR);
                    //aProjectile->mPlantParentID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
                    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
                    aProjectile->mEnemy = mEnemy;
                    aProjectile->mMotionType = MOTION_LOBBED;
                    aProjectile->mVelX = 3.33f;
                    aProjectile->mVelY = 0.0f;
                    aProjectile->mVelZ = 3.33f;
                    aProjectile->mAccZ = 0.0f;
                    aProjectile->mRow = aZombie->mRow;
                }
            }
        }
        return;
    }

    ProjectileType aProjectileType;
    switch (mSeedType)
    {
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_GATLINGPEA:
    case SeedType::SEED_LEFTPEATER:
        aProjectileType = ProjectileType::PROJECTILE_PEA;
        break;
    case SeedType::SEED_SLINGPEA:
        aProjectileType = ProjectileType::PROJECTILE_SLINGPEA;
        break;
    case SeedType::SEED_FROST_CABBAGE:
        aProjectileType = ProjectileType::PROJECTILE_FROSTCABBAGE;
        break;
    case SeedType::SEED_SNOWPEA:
        aProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
        break;
    case SeedType::SEED_SEASHROOM:
        aProjectileType = ProjectileType::PROJECTILE_SEAPUFF;
        break;
    case SeedType::SEED_BRAVEYSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_PUFFSHROOM:
        aProjectileType = ProjectileType::PROJECTILE_PUFF;
        break;
    case SeedType::SEED_SPORE_SHROOM:
        aProjectileType = ProjectileType::PROJECTILE_SPORE;
        break;
    case SeedType::SEED_CACTUS:
        aProjectileType = (thePlantWeapon == WEAPON_SECONDARY ? ProjectileType::PROJECTILE_CACTUS_SPIKE : ProjectileType::PROJECTILE_SPIKE);
        break;
    case SeedType::SEED_STINGER:
    case SeedType::SEED_CATTAIL:
        aProjectileType = ProjectileType::PROJECTILE_SPIKE;
        break;
    case SeedType::SEED_CABBAGEPULT:
        aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        break;
    case SeedType::SEED_BOMBBERRY:
        aProjectileType = ProjectileType::PROJECTILE_BOMBBERRY;
        break;
    case SeedType::SEED_KERNELPULT:
        aProjectileType = ProjectileType::PROJECTILE_KERNEL;
        break;
    case SeedType::SEED_MELONPULT:
        aProjectileType = ProjectileType::PROJECTILE_MELON;
        break;
    case SeedType::SEED_WINTERMELON:
        aProjectileType = ProjectileType::PROJECTILE_WINTERMELON;
        break;
    case SeedType::SEED_COBCANNON:
        aProjectileType = ProjectileType::PROJECTILE_COBBIG;
        break;
    default:
        TOD_ASSERT();
        break;
    }

    if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectileType = ProjectileType::PROJECTILE_BUTTER;
    }



    if (mSeedType == SEED_SLINGPEA)
    {
        //Nothing
    }
    else
    {
        mApp->PlayFoley(FoleyType::FOLEY_THROW);
    }


    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_FROST_CABBAGE)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SNOW_PEA_SPARKLES);
    }
    else if (mSeedType == SeedType::SEED_SPORE_SHROOM) 
    {
        mApp->PlayFoley(FoleyType::FOLEY_SPORESHROOM_SHOOT);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        mApp->PlayFoley(FoleyType::FOLEY_PUFF);
    }

    int aOriginX, aOriginY;

    if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        if (mEnemy)
            aOriginX = mX - 40;
        else
            aOriginX = mX + 40;
        aOriginY = mY + 40;
    }
    else if (mSeedType == SEED_SPORE_SHROOM) {
        if (mEnemy)
            aOriginX = mX - 40;
        else
            aOriginX = mX + 40;
        aOriginY = mY - 40;
    }
    else if (mSeedType == SeedType::SEED_SEASHROOM)
    {
        if (mEnemy)
            aOriginX = mX - 45;
        else
            aOriginX = mX + 45;
        aOriginY = mY + 63;
    }
    else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_FROST_CABBAGE)
    {
        if (mEnemy)
            aOriginX = mX - 5;
        else
            aOriginX = mX + 5;
        aOriginY = mY - 12;
    }
    else if (mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        if (mEnemy)
            aOriginX = mX - 25;
        else
            aOriginX = mX + 25;
        aOriginY = mY - 46;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        if (mEnemy)
            aOriginX = mX - 20;
        else
            aOriginX = mX + 20;
        aOriginY = mY - 3;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
    {
        if (mEnemy)
            aOriginX = mX - 19;
        else
            aOriginX = mX + 19;
        aOriginY = mY - 37;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        if (mEnemy)
            aOriginX = mX - 12;
        else
            aOriginX = mX + 12;
        aOriginY = mY - 56;
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || mSeedType == SEED_SLINGPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        if (mEnemy)
            aOriginX = mX - aOffsetX - 24;
        else
            aOriginX = mX + aOffsetX + 24;
        aOriginY = mY + aOffsetY - 33;

        if (mSeedType == SEED_SLINGPEA)
        {
            aOriginX = mX + 5;
            aOriginY = mY - 12;
        }
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        if (mEnemy)
            aOriginX = mX + aOffsetX + 24 - 42.5f;
        else
            aOriginX = mX - aOffsetX - 24 + 42.5f;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        if (mEnemy)
            aOriginX = mX - aOffsetX - 34;
        else
            aOriginX = mX + aOffsetX + 34;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginY = mY + aOffsetY - 33;

        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            if (mEnemy)
                aOriginX = mX - aOffsetX + 64;
            else
                aOriginX = mX + aOffsetX - 64;
        }
        else
        {
            if (mEnemy)
                aOriginX = mX - aOffsetX - 24;
            else
                aOriginX = mX + aOffsetX + 24;
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        if (mEnemy)
            aOriginX = mX - 45;
        else
            aOriginX = mX + 45;
        aOriginY = mY + 10;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM)
    {
        if (mEnemy)
            aOriginX = mX - 29;
        else
            aOriginX = mX + 29;
        aOriginY = mY + 21;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        if (thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
        {
            if (mEnemy)
                aOriginX = mX - 93;
            else 
                aOriginX = mX + 93;
            aOriginY = mY - 50;
        }
        else
        {
            if (mEnemy)
                aOriginX = mX - 70;
            else 
                aOriginX = mX + 70;
            aOriginY = mY + 23;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        if (mEnemy)
            aOriginX = mX + 44;
        else
            aOriginX = mX - 44;
        aOriginY = mY - 184;
    }
    else if (mSeedType == SeedType::SEED_STINGER) {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim){
            ReanimatorTransform aTransform;
            aBodyReanim->GetCurrentTransform(aBodyReanim->FindTrackIndex("Stinger_mouth"), &aTransform);
            aOriginX = mX + aTransform.mTransX + 24;
            aOriginY = mY + aTransform.mTransY;
        }
    }
    else
    {
        if (mEnemy)
            aOriginX = mX - 10;
        else 
            aOriginX = mX + 10;
        aOriginY = mY + 5;
    }

    /*if (mBoard->GetFlowerPotAt(mPlantCol, mRow) || mBoard->GetWaterPotAt(mPlantCol, mRow) || mBoard->GetFlyingPotAt(mPlantCol, mRow))
    {
        aOriginY -= 5;
    }*/

    aOriginY += PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

    if (mSeedType == SeedType::SEED_SNOWPEA)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        if (mEnemy)
            mApp->AddTodParticle(aOriginX - 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
        else
            mApp->AddTodParticle(aOriginX + 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        if (mEnemy)
            mApp->AddTodParticle(aOriginX - 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
        else
            mApp->AddTodParticle(aOriginX + 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    else if (mSeedType == SeedType::SEED_SPORE_SHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        if (mEnemy)
            mApp->AddTodParticle(aOriginX - 27, aOriginY - 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
        else
            mApp->AddTodParticle(aOriginX + 27, aOriginY - 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        if (mEnemy)
            mApp->AddTodParticle(aOriginX - 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
        else
            mApp->AddTodParticle(aOriginX + 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }

    if (mIsPlayer) {
        int offAvg = (mSeedType == SEED_GATLINGPEA ? 34 : 24);
        int aOffsetX, aOffsetY;

        Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

        int aTrackIndex = 0;
        if (aHeadReanim->TrackExists("anim_stem"))
        {
            aTrackIndex = aHeadReanim->FindTrackIndex("anim_stem");
        }
        else if (aHeadReanim->TrackExists("anim_idle"))
        {
            aTrackIndex = aHeadReanim->FindTrackIndex("anim_idle");
        }

        ReanimatorTransform aTransform;
        aHeadReanim->GetCurrentTransform(aTrackIndex, &aTransform);
        aOffsetX = aTransform.mTransX;
        aOffsetY = aTransform.mTransY;

        if (mEnemy)
            aOriginX = mX - aOffsetX - offAvg;
        else
            aOriginX = mX + aOffsetX + offAvg;
        aOriginY = mY + aOffsetY - 33;
    }

    if (mEnemy) {
        aOriginX += 42.5f;
    }

    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aProjectileType);
    //aProjectile->mPlantParentID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);

    if (mSeedType == SEED_STINGER) {
        Zombie* targetZombie = FindTargetZombie(mRow, WEAPON_PRIMARY);
        if (targetZombie && targetZombie->IsFlying() && !targetZombie->IsOnHighGround())
            aProjectile->mDamageRangeFlags = 2;
        aProjectile->mIgnoreGround = true;
    }

    aProjectile->mEnemy = mEnemy;
    //aProjectile->mBackwards = aProjectile->mEnemy;

    if (mSeedType == SEED_CACTUS)
    {
        if (thePlantWeapon == WEAPON_PRIMARY)
            aProjectile->mIgnoreGround = true;
    }

    if (mIsPlayer)  aProjectile->mIgnoreGround = true;

    if (mSeedType == SeedType::SEED_SPORE_SHROOM || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT ||
        mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SEED_FROST_CABBAGE || mSeedType == SEED_BOMBBERRY)
    {
        float aRangeX, aRangeY;

        if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
            if (theTargetPlant){
                Rect aPlantRect = theTargetPlant->GetPlantRect();
     
                aRangeX = theTargetPlant->mX - aOriginX;
                aRangeY = aPlantRect.mY - aOriginY;
            }
        }
        else if (theTargetZombie)
        {
            Rect aZombieRect = theTargetZombie->GetZombieRect();
            aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30;
            aRangeY = aZombieRect.mY - aOriginY;

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING){
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_POGO && theTargetZombie->mHasObject){
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL){
                aRangeX -= 40.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS){
                aRangeY = mBoard->GridToPixelY(8, mRow) - aOriginY;
            }
        }
        else{
            if (mEnemy){
                aRangeX = aOriginX - 700.0f;
            }
            else{
                aRangeX = 700.0f - aOriginX;
            }
            aRangeY = 0.0f;
        }

        if (!aProjectile->mBackwards && aRangeX < 40.0f && aProjectile->mBackwards && aRangeX > 40.0f){
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = aRangeX / 120.0f;
        aProjectile->mVelY = 0.0f;
        aProjectile->mVelZ = aRangeY / 120.0f - 14;
        aProjectile->mAccZ = 0.115f;
    }
    else if (mSeedType == SeedType::SEED_SLINGPEA)
    {
        float aRangeX, aRangeY, aRangeZ;
        if (theTargetZombie)
        {
            Rect aZombieRect = theTargetZombie->GetZombieRect();
            aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30.0f + RandRangeFloat(-7.f, 7.f);
            float lawnHeight = mBoard->StageHas6Rows() || mBoard->StageHasRoof() ? 85.0f : 100.0f;
            float rowDiff = mRow - theTargetZombie->mRow;
            aRangeY = aZombieRect.mY - aOriginY + rowDiff;
            aRangeZ = aZombieRect.mY - aOriginY + rowDiff * lawnHeight;

            if (mBoard->StageHasRoof()) {
                aRangeY += 100;
            }

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_POGO && theTargetZombie->mHasObject)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
            {
                aRangeX -= 40.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
            {
                aRangeZ = mBoard->GridToPixelY(8, mRow) - aOriginY;
            }
        }
        else if (theTargetPlant) {
            Rect aPlantRect = theTargetPlant->GetPlantRect();
            aRangeX = theTargetPlant->mX - aOriginX - 30.0f + RandRangeFloat(-7.f, 7.f);
            float lawnHeight = mBoard->StageHas6Rows() || mBoard->StageHasRoof() ? 85.0f : 100.0f;
            float rowDiff = mRow - theTargetZombie->mRow;
            aRangeY = aPlantRect.mY - aOriginY + rowDiff;
            aRangeZ = aPlantRect.mY - aOriginY + rowDiff * lawnHeight;
        }
        else
        {
            aRangeX = 700.0f - aOriginX;
            aRangeY = 0.0f;
            aRangeZ = 0.0f;
        }

        //if (mEnemy)
        //   aRangeX *= -1;

        if (aRangeX < 40.0f)
        {
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = aRangeX / 120.0f;
        aProjectile->mVelY = aRangeY / 120.0f;
        aProjectile->mVelZ = aRangeZ / 120.0f - 14;
        aProjectile->mAccZ = 0.115f;
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        if (theRow < mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = -3.0f;
            aProjectile->mShadowY += 80.0f;
        }
        else if (theRow > mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = 3.0f;
            aProjectile->mShadowY -= 80.0f;
        }

        if (mEnemy)
            aProjectile->mBackwards = true;
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY || (mSeedType == SeedType::SEED_LEFTPEATER && !mEnemy || mSeedType != SeedType::SEED_LEFTPEATER && mEnemy))
    {
        aProjectile->mBackwards = true;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aProjectile->mVelX = 2.0f;
        aProjectile->mMotionType = ProjectileMotion::MOTION_HOMING;
        aProjectile->mTargetZombieID = mBoard->ZombieGetID(theTargetZombie);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aProjectile->mVelX = 0.001f;
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelY = 0.0f;
        aProjectile->mAccZ = 0.0f;
        aProjectile->mVelZ = -8.0f;
        aProjectile->mCobTargetX = mTargetX - 40;
        aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
    }
}

//0x4675C0
Zombie* Plant::FindTargetZombie(int theRow, PlantWeapon thePlantWeapon)
{
    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Zombie* aBestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (mBoard->mBackground == BACKGROUND_HIGHGROUND)
        {
            if ((IsOnHighGround() != aZombie->IsOnHighGround() &&
                (mSeedType == SEED_CHOMPER || mSeedType == SEED_SUPERCHOMPER || mSeedType == SEED_SQUASH)) ||
                (IsOnHighGround() && !aZombie->IsOnHighGround() &&
                    mSeedType != SEED_CHOMPER && mSeedType != SEED_SUPERCHOMPER && mSeedType != SEED_SQUASH))
            {
                continue;
            }

        }

        int aRowDeviation = aZombie->mRow - theRow;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
        {
            aRowDeviation = 0;
        }

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget())
        {
            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_BONKCHOY || mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER || mSeedType == SeedType::SEED_TANGLEKELP)
            {
                continue;
            }
        }

        if (aZombie->IsDeadOrDying()) {
            if (mSeedType == SEED_BONKCHOY)
            {
                continue;
            }
        }

        bool needPortalCheck = false;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2)
        {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_CACTUS || 
                mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_THREEPEATER ||
                mSeedType == SeedType::SEED_STINGER)
                needPortalCheck = true;
        }

        if (mSeedType != SeedType::SEED_CATTAIL)
        {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            {
                if (aRowDeviation < -1 || aRowDeviation > 1)
                {
                    continue;
                }
            }
            else if (needPortalCheck)
            {
                if (!mBoard->mChallenge->CanTargetZombieWithPortals(mPlantCol, theRow, aZombie))
                {
                    continue;
                }
            }
            else if (aRowDeviation)
            {
                continue;
            }
        }

        if (mSeedType == SEED_STINGER )
        {
            aDamageRangeFlags = (aZombie->IsFlying() && !aZombie->IsOnHighGround() ? 2 : 1);
        }

        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            int aExtraRange = 0;

            if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER)
            {
                if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
                {
                    aAttackRect.mX += 20;
                    aAttackRect.mWidth -= 20;
                }

                if (aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING || (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol == mPlantCol))
                {
                    continue;
                }

                if (aZombie->mIsEating || mState == PlantState::STATE_CHOMPER_BITING)
                {
                    aExtraRange = 60;
                }
            }

            if (mSeedType == SeedType::SEED_POTATOMINE)
            {
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mHasObject) ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    continue;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
                {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40;  // 原版经典土豆地雷 Bug 及“四撑杆引雷”的原理
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != mPlantCol)
                {
                    continue;
                }

                if (aZombie->mIsEating)
                {
                    aExtraRange = 30;
                }
            }

            PlantsOnLawn aPlantOnLawn;
            mBoard->GetPlantsOnLawn(mPlantCol, mRow, &aPlantOnLawn);

            if ((mSeedType == SeedType::SEED_EXPLODE_O_NUT && aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT) ||
                (mSeedType == SeedType::SEED_TANGLEKELP && !aZombie->mInPool && (aPlantOnLawn.mPotPlant && aPlantOnLawn.mPotPlant->mSeedType != SEED_WATERPOT || !aPlantOnLawn.mPotPlant)))
            {
                continue;
            }

            Rect aZombieRect = aZombie->GetZombieRect();
            if (!needPortalCheck && GetRectOverlap(aAttackRect, aZombieRect) < -aExtraRange)
            {
                continue;
            }

            ////////////////////

            int aWeight = -aZombieRect.mX;
            if (mSeedType == SeedType::SEED_CATTAIL || mSeedType == SeedType::SEED_BONKCHOY)
            {
                aWeight = -Distance2D(mX + 40.0f, mY + 40.0f, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2);
                if (aZombie->IsFlying() && mSeedType != SeedType::SEED_BONKCHOY)
                {
                    aWeight += 10000;  // 优先攻击飞行单位
                }
            }

            if (aBestZombie == nullptr || aWeight > aHighestWeight)
            {
                aHighestWeight = aWeight;
                aBestZombie = aZombie;
            }
        }
    }

    return aBestZombie;
}

Plant* Plant::FindTargetPlant(int theRow, PlantWeapon thePlantWeapon)
{
    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Plant* aBestPlant = nullptr;

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (mEnemy == aPlant->mEnemy)   
            continue;

        if (aPlant->mState == STATE_BURNED)
            continue;

        int aRowDeviation = aPlant->mRow - theRow;

        bool needPortalCheck = false;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2)
        {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_CACTUS ||
                mSeedType == SeedType::SEED_STINGER || mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_THREEPEATER)
                needPortalCheck = true;
        }

        if (mSeedType != SeedType::SEED_CATTAIL)
        {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            {
                if (aRowDeviation < -1 || aRowDeviation > 1)
                {
                    continue;
                }
            }
            else if (needPortalCheck)
            {
                if (!mBoard->mChallenge->CanTargetPlantWithPortals(this, aPlant))
                {
                    continue;
                }
            }
            else if (aRowDeviation)
            {
                continue;
            }
        }

        int aExtraRange = 0;

        Rect aPlantRect = aPlant->GetPlantRect();
        bool checkOverlap = (mEnemy ? GetRectOverlap(aAttackRect, aPlantRect) > -aExtraRange : GetRectOverlap(aAttackRect, aPlantRect) < -aExtraRange);

        if (!needPortalCheck && checkOverlap){
            continue;
        }

        int aWeight = (mEnemy ? aPlantRect.mX : -aPlantRect.mX);

        if (aBestPlant == nullptr || aWeight > aHighestWeight){
            aHighestWeight = aWeight;
            aBestPlant = aPlant;
        }
        
    }
    return aBestPlant;
}
//0x4678B0
int Plant::DistanceToClosestZombie()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    int aClosestDistance = 1000;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aDistance = -GetRectOverlap(aAttackRect, aZombieRect);
            if (aDistance < aClosestDistance)
            {
                aClosestDistance = max(aDistance, 0);
            }
        }
    }

    return aClosestDistance;
}

//0x4679B0
void Plant::Die()
{
    if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && mIsDummy){
        mApp->PlayFoley(FOLEY_ART_CHALLENGE);
        mApp->AddTodParticle(mX + 40, mY + 40, RENDER_LAYER_TOP, PARTICLE_PRESENT_PICKUP);
    }

    if (IsOnBoard() && mSeedType == SeedType::SEED_TANGLEKELP)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aZombie)
        {
            aZombie->DieWithLoot();
        }
    }

    mDead = true;
    RemoveEffects();

    if (!Plant::IsFlying(mSeedType) && IsOnBoard())
    {
        GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
        if (aLadder)
        {
            aLadder->GridItemDie();
        }
    }

    if (IsOnBoard())
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);

        if (aFlowerPot && aTopPlant == aFlowerPot)
        {
            Reanimation* aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
        Plant* aWaterPot = mBoard->GetWaterPotAt(mPlantCol, mRow);
        if (aWaterPot && aTopPlant == aWaterPot)
        {
            Reanimation* aPotReanim = mApp->ReanimationGet(aWaterPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }

        if (mSeedType == SEED_PLANTERN && mBoard->StageHasDarkness())
        {
            mBoard->mUpdateDarkness = true;
        }
    }
}

PlantDefinition& GetPlantDefinition(SeedType theSeedType)
{
    //TOD_ASSERT(gPlantDefs[theSeedType].mSeedType == theSeedType);
    //TOD_ASSERT(theSeedType >= 0 && theSeedType < (int)SeedType::NUM_SEED_TYPES);
    
    return gPlantDefs[theSeedType];
}

//0x467B00
int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType)
{
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
    {
        if (theSeedType == SeedType::SEED_REPEATER)
        {
            return 1000;
        }
        else if (theSeedType == SeedType::SEED_FUMESHROOM)
        {
            return 500;
        }
        else if (theSeedType == SeedType::SEED_TALLNUT)
        {
            return 250;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE)
        {
            return 100;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
        {
            return 200;
        }
    }

    switch (theSeedType)
    {
    case SeedType::SEED_SLOT_MACHINE_SUN:           return 0;
    case SeedType::SEED_SLOT_MACHINE_DIAMOND:       return 0;
    case SeedType::SEED_ZOMBIQUARIUM_SNORKLE:       return 100;
    case SeedType::SEED_ZOMBIQUARIUM_TROPHY:        return 1000;
    case SeedType::SEED_ZOMBIE_NORMAL:              return 50;
    case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:        return 75;
    case SeedType::SEED_ZOMBIE_POLEVAULTER:         return 75;
    case SeedType::SEED_ZOMBIE_PAIL:                return 125;
    case SeedType::SEED_ZOMBIE_LADDER:              return 150;
    case SeedType::SEED_ZOMBIE_DIGGER:              return 125;
    case SeedType::SEED_ZOMBIE_BUNGEE:              return 125;
    case SeedType::SEED_ZOMBIE_FOOTBALL:            return 175;
    case SeedType::SEED_ZOMBIE_BALLOON:             return 150;
    case SeedType::SEED_ZOMBIE_SCREEN_DOOR:         return 100;
    case SeedType::SEED_ZOMBONI:                    return 175;
    case SeedType::SEED_ZOMBIE_POGO:                return 200;
    case SeedType::SEED_ZOMBIE_DANCER:              return 350;
    case SeedType::SEED_ZOMBIE_GARGANTUAR:          return 300;
    case SeedType::SEED_ZOMBIE_IMP:                 return 50;
    default:
    {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
            return aPlantDef.mSeedCost;
        }
        else
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
            return aPlantDef.mSeedCost;
        }
    }
    }
}

//0x467C00
SexyString Plant::GetNameString(SeedType theSeedType, SeedType theImitaterType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aName = StrFormat(_S("[%s]"), aPlantDef.mPlantName);
    SexyString aTranslatedName = TodStringTranslate(StringToSexyStringFast(aName));

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aImitaterDef = GetPlantDefinition(theImitaterType);
        std::string aImitaterName = StrFormat(_S("[%s]"), aImitaterDef.mPlantName);
        std::string aTranslatedImitaterName = TodStringTranslate(StringToSexyStringFast(aImitaterName));
        return StrFormat(_S("%s %s"), aTranslatedName.c_str(), aTranslatedImitaterName.c_str());
    }

    return aTranslatedName;
}

//0x467DB0
SexyString Plant::GetToolTip(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aToolTip = StrFormat(_S("[%s_TOOLTIP]"), aPlantDef.mPlantName);
    return TodStringTranslate(aToolTip);
}

//0x467E30
int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType)
{
    if (Challenge::IsZombieSeedType(theSeedType))
    {
        return 0;
    }

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
        return aPlantDef.mRefreshTime;
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
        return aPlantDef.mRefreshTime;
    }
}

//0x467E60
bool Plant::IsNocturnal(SeedType theSeedtype)
{
    return
        theSeedtype == SeedType::SEED_PUFFSHROOM ||
        theSeedtype == SeedType::SEED_SEASHROOM ||
        theSeedtype == SeedType::SEED_SUNSHROOM ||
        theSeedtype == SeedType::SEED_FUMESHROOM ||
        theSeedtype == SeedType::SEED_ICYFUMESHROOM ||
        theSeedtype == SeedType::SEED_HYPNOSHROOM ||
        theSeedtype == SeedType::SEED_DOOMSHROOM ||
        theSeedtype == SeedType::SEED_ICESHROOM ||
        theSeedtype == SeedType::SEED_MAGNETSHROOM ||
        theSeedtype == SeedType::SEED_SCAREDYSHROOM ||
        theSeedtype == SeedType::SEED_SPORE_SHROOM ||
        theSeedtype == SeedType::SEED_GLOOMSHROOM ||
        theSeedtype == SeedType::SEED_FIRESHROOM ||
        theSeedtype == SeedType::SEED_BRAVEYSHROOM;
}

//0x467EA0
bool Plant::IsAquatic(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_LILYPAD ||
        theSeedType == SeedType::SEED_TANGLEKELP ||
        theSeedType == SeedType::SEED_SEASHROOM ||
        theSeedType == SeedType::SEED_CATTAIL;
}

bool Plant::IsFlying(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_INSTANTCOFFEE ||
           theSeedtype == SeedType::SEED_STINGER;
}

//0x467EC0
bool Plant::IsUpgrade(SeedType theSeedtype)
{
    return 
        theSeedtype == SeedType::SEED_GATLINGPEA || 
        theSeedtype == SeedType::SEED_WINTERMELON || 
        theSeedtype == SeedType::SEED_TWINSUNFLOWER || 
        theSeedtype == SeedType::SEED_SPIKEROCK || 
        theSeedtype == SeedType::SEED_COBCANNON || 
        theSeedtype == SeedType::SEED_GOLD_MAGNET || 
        theSeedtype == SeedType::SEED_GLOOMSHROOM || 
        theSeedtype == SeedType::SEED_CATTAIL ||
        theSeedtype == SeedType::SEED_PLASMAWOOD ||
        theSeedtype == SeedType::SEED_SUPERCHOMPER ||
        theSeedtype == SeedType::SEED_SHOOTINGSTAR ||
        theSeedtype == SeedType::SEED_BRAVEYSHROOM ||
        /*Premium Plants*/
        theSeedtype == SeedType::SEED_DUMMY ||
        theSeedtype == SeedType::SEED_FLYINGPOT;
}

//0x467EF0
Rect Plant::GetPlantRect()
{
    Rect aRect;
    if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aRect = Rect(mX + 10, mY, mWidth, mHeight);
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aRect = Rect(mX, mY, 140, 80);
    }
    else
    {
        aRect = Rect(mX + 10, mY, mWidth - 20, mHeight);
    }

    return aRect;
}

//0x467F90
Rect Plant::GetPlantAttackRect(PlantWeapon thePlantWeapon)
{
    Rect aRect;
    if (mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (mSeedType == SEED_WALLNUT || mSeedType == SEED_ARTIC_NUT || mSeedType == SEED_GIANT_WALLNUT || mSeedType == SEED_EXPLODE_O_NUT))
    {
        if (mSeedType == SEED_GIANT_WALLNUT)
            aRect = Rect(mX + 25, mY, mWidth - 20 + 25, mHeight);
        else
            aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && mSeedType == SeedType::SEED_SPLITPEA)
    {
        aRect = Rect(0, mY, mX + 16, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_BRAVEYSHROOM))
    {
        aRect = Rect(0, mY, mX + 16, mHeight);
    }
    else 
    {
        switch (mSeedType)
        {
        case SeedType::SEED_LEFTPEATER:     aRect = Rect((mEnemy ? 800 : 0), mY, mX, mHeight);               break;
        case SeedType::SEED_SQUASH:         aRect = Rect(mX + 20, mY, mWidth - 35, mHeight);               break;
        case SeedType::SEED_BONKCHOY:       aRect = Rect(mX - 80, mY, mWidth + 160, mHeight);              break;
        case SeedType::SEED_SUPERCHOMPER:
        case SeedType::SEED_CHOMPER:        aRect = Rect(mX + 80, mY, 40, mHeight);               break;
        case SeedType::SEED_SPIKEWEED:
        case SeedType::SEED_SPIKEROCK:      aRect = Rect(mX + 20, mY, mWidth - 50, mHeight);               break;
        case SeedType::SEED_POTATOMINE:     aRect = Rect(mX, mY, mWidth - 25, mHeight);               break;
        case SeedType::SEED_PLASMAWOOD:
        case SeedType::SEED_TORCHWOOD:      aRect = Rect(mX + 50, mY, 30, mHeight);               break;
        case SeedType::SEED_PUFFSHROOM:
        case SeedType::SEED_SEASHROOM:      aRect = Rect(mX + 60, mY, 230, mHeight);               break;
        case SeedType::SEED_ICYFUMESHROOM:
        case SeedType::SEED_FUMESHROOM:     aRect = Rect(mX + 60, mY, 340, mHeight);               break;
        case SeedType::SEED_GLOOMSHROOM:    aRect = Rect(mX - 80, mY - 80, 240, 240);                   break;
        case SeedType::SEED_TANGLEKELP:     aRect = Rect(mX, mY, mWidth, mHeight);               break;
        case SeedType::SEED_CATTAIL:        aRect = Rect(-BOARD_WIDTH, -BOARD_HEIGHT, BOARD_WIDTH * 2, BOARD_HEIGHT * 2);      break;
        default:                            aRect = Rect(mX + 60, mY, BOARD_WIDTH, mHeight);               break;
        }
    }

    return aRect;
}

//0x4681E0
void Plant::PreloadPlantResources(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        ReanimatorEnsureDefinitionLoaded(aPlantDef.mReanimationType, true);
    }

    if (theSeedType == SeedType::SEED_CHERRYBOMB || theSeedType == SeedType::SEED_CHERRYHOVERBOMB)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED, true);
    }
    else if (theSeedType == SeedType::SEED_JALAPENO)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_TORCHWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_FIRE_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_PLASMAWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_PLASMA_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_PLASMA_FIRE, true);
    }
    else if (Plant::IsNocturnal(theSeedType))
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_SLEEPING, true);
    }
}

//0x468280
void Plant::PlayIdleAnim(float theRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        if (mSeedType == SEED_UMBRELLA && mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL){
            PlayBodyReanim("anim_cover", ReanimLoopType::REANIM_LOOP, 20, theRate);
        }
        else {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, theRate);
        }
       
        if (mApp->IsIZombieLevel())
        {
            aBodyReanim->mAnimRate = 0.0f;
        }
    }
}

void Plant::ApplyBurn() {
    if (mDead || mState == STATE_BURNED)
        return;

    if (mIceTrapCounter > 0) {
        AddAttachedParticle(mX + 75, mY + 106, (int)RenderLayer::RENDER_LAYER_PARTICLE, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
    }

    mChilledCounter = 0;
    mIceTrapCounter = 0;
    mButteredCounter = 0;
    mAmplifiedCounter = 0;
    mBerserkCounter = 0;
    mBurnCounter = 300;
    mEatenFlashCountdown = 0;
    mState = STATE_BURNED;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim) aBodyReanim->mAnimRate = 0.0f;

    mApp->RemoveReanimation(mBlinkReanimID);

    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (aHeadReanim) aHeadReanim->mAnimRate = 0.0f;

    Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(mHeadReanimID2);
    if (aHeadReanim2) aHeadReanim2->mAnimRate = 0.0f;

    Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(mHeadReanimID3);
    if (aHeadReanim3) aHeadReanim3->mAnimRate = 0.0f;

    mApp->RemoveReanimation(mLightReanimID);
    mApp->RemoveReanimation(mSleepingReanimID);

    UpdateReanimColor();

}

void Plant::UpdateBurn() {
    mBurnCounter--;
    if (mBurnCounter == 0){
        Die();
    }
}
