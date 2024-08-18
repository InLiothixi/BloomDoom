#include "Plant.h"
#include "Board.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "GridItem.h"
#include "LawnMower.h"
#include "Challenge.h"
#include "Projectile.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "System/Music.h"
#include "Widget/AlmanacDialog.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/TodCommon.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/TodParticle.h"

#include "../Sexy.TodLib/EffectSystem.h"
#include "CursorObject.h"

ZombieDefinition gZombieDefs[NUM_ZOMBIE_TYPES] = {  //0x69DA80
    { ZOMBIE_NORMAL,            REANIM_ZOMBIE,              1,      1,      1,      4000,   _S("ZOMBIE") },
    { ZOMBIE_FLAG,              REANIM_ZOMBIE,              1,      1,      1,      0,      _S("FLAG_ZOMBIE") },
    { ZOMBIE_TRAFFIC_CONE,      REANIM_ZOMBIE,              2,      3,      1,      4000,   _S("CONEHEAD_ZOMBIE") },
    { ZOMBIE_POLEVAULTER,       REANIM_POLEVAULTER,         2,      6,      5,      2000,   _S("POLE_VAULTING_ZOMBIE") },
    { ZOMBIE_PAIL,              REANIM_ZOMBIE,              4,      8,      1,      3000,   _S("BUCKETHEAD_ZOMBIE") },
    { ZOMBIE_NEWSPAPER,         REANIM_ZOMBIE_NEWSPAPER,    2,      11,     1,      1000,   _S("NEWSPAPER_ZOMBIE") },
    { ZOMBIE_DOOR,              REANIM_ZOMBIE,              4,      13,     5,      3500,   _S("SCREEN_DOOR_ZOMBIE") },
    { ZOMBIE_FOOTBALL,          REANIM_ZOMBIE_FOOTBALL,     7,      16,     5,      2000,   _S("FOOTBALL_ZOMBIE") },
    { ZOMBIE_DANCER,            REANIM_DANCER,              5,      18,     5,      1000,   _S("DANCING_ZOMBIE") },
    { ZOMBIE_BACKUP_DANCER,     REANIM_BACKUP_DANCER,       1,      18,     1,      0,      _S("BACKUP_DANCER") },
    { ZOMBIE_DUCKY_TUBE,        REANIM_ZOMBIE,              1,      21,     5,      0,      _S("DUCKY_TUBE_ZOMBIE") },
    { ZOMBIE_SNORKEL,           REANIM_SNORKEL,             3,      23,     10,     2000,   _S("SNORKEL_ZOMBIE") },
    { ZOMBIE_ZAMBONI,           REANIM_ZOMBIE_ZAMBONI,      7,      26,     10,     2000,   _S("ZOMBONI") },
    { ZOMBIE_BOBSLED,           REANIM_BOBSLED,             3,      26,     10,     2000,   _S("ZOMBIE_BOBSLED_TEAM") },
    { ZOMBIE_DOLPHIN_RIDER,     REANIM_ZOMBIE_DOLPHINRIDER, 3,      28,     10,     1500,   _S("DOLPHIN_RIDER_ZOMBIE") },
    { ZOMBIE_JACK_IN_THE_BOX,   REANIM_JACKINTHEBOX,        3,      31,     10,     1000,   _S("JACK_IN_THE_BOX_ZOMBIE") },
    { ZOMBIE_BALLOON,           REANIM_BALLOON,             2,      33,     10,     2000,   _S("BALLOON_ZOMBIE") },
    { ZOMBIE_DIGGER,            REANIM_DIGGER,              4,      36,     10,     1000,   _S("DIGGER_ZOMBIE") },
    { ZOMBIE_POGO,              REANIM_POGO,                4,      38,     10,     1000,   _S("POGO_ZOMBIE") },
    { ZOMBIE_YETI,              REANIM_YETI,                4,      40,     1,      1,      _S("ZOMBIE_YETI") },
    { ZOMBIE_BUNGEE,            REANIM_BUNGEE,              3,      41,     10,     1000,   _S("BUNGEE_ZOMBIE") },
    { ZOMBIE_LADDER,            REANIM_LADDER,              4,      43,     10,     1000,   _S("LADDER_ZOMBIE") },
    { ZOMBIE_CATAPULT,          REANIM_CATAPULT,            5,      46,     10,     1500,   _S("CATAPULT_ZOMBIE") },
    { ZOMBIE_GARGANTUAR,        REANIM_GARGANTUAR,          10,     48,     15,     1500,   _S("GARGANTUAR") },
    { ZOMBIE_IMP,               REANIM_IMP,                 1,      48,     15,     0,      _S("IMP") },
    { ZOMBIE_WRAITH,            REANIM_WRAITH,              2,      6,      5,      2000,   _S("WRAITH") },
    { ZOMBIE_NINJAIMP,          REANIM_NINJAIMP,            2,      11,     1,      1000,   _S("NINJA_IMP") },
    { ZOMBIE_BOSS,              REANIM_BOSS,                10,     50,     1,      0,      _S("BOSS") },
    { ZOMBIE_PEA_HEAD,          REANIM_ZOMBIE,              1,      99,     1,      4000,   _S("ZOMBIE_PEASHOOTER_HEAD") },
    { ZOMBIE_CHERRYBOMB_HEAD,   REANIM_ZOMBIE,              3,      99,     10,     1000,   _S("ZOMBIE_CHERRYBOMB_HEAD") },
    { ZOMBIE_WALLNUT_HEAD,      REANIM_ZOMBIE,              4,      99,     1,      3000,   _S("ZOMBIE_WALLNUT_HEAD") },
    { ZOMBIE_POTATOMINE_HEAD,   REANIM_ZOMBIE,              2,      99,     5,      2000,   _S("ZOMBIE_POTATOMINE_HEAD") },
    { ZOMBIE_REPEATER_HEAD,     REANIM_ZOMBIE,              3,      99,     1,      3000,   _S("ZOMBIE_REPEATER_HEAD") },
    { ZOMBIE_JALAPENO_HEAD,     REANIM_ZOMBIE,              3,      99,     10,     1000,   _S("ZOMBIE_JALAPENO_HEAD") },
    { ZOMBIE_GATLING_HEAD,      REANIM_ZOMBIE,              3,      99,     10,     2000,   _S("ZOMBIE_GATLING_PEA_HEAD") },
    { ZOMBIE_SQUASH_HEAD,       REANIM_ZOMBIE,              3,      99,     10,     2000,   _S("ZOMBIE_GATLING_PEA_HEAD") },
    { ZOMBIE_TALLNUT_HEAD,      REANIM_ZOMBIE,              4,      99,     10,     2000,   _S("ZOMBIE_TALLNUT_HEAD") },
    { ZOMBIE_HYPNOSHROOM_HEAD,  REANIM_ZOMBIE,              4,      99,     10,     2000,   _S("ZOMBIE_HYPNO_SHROOM_HEAD") },
    { ZOMBIE_EXPLODE_O_NUT_HEAD,REANIM_ZOMBIE,              4,      99,     10,     2000,   _S("ZOMBIE_EXPLODE_O_NUT_HEAD") },
    { ZOMBIE_REDEYE_GARGANTUAR, REANIM_GARGANTUAR,          10,     48,     15,     6000,   _S("REDEYED_GARGANTUAR") },
    { ZOMBIE_BLACK_FOOTBALL,    REANIM_ZOMBIE_BLACKFOOTBALL,7,      16,     5,      8000,   _S("BLACKFOOTBALL_ZOMBIE")},
    { ZOMBIE_DIVER,             REANIM_SNORKEL,             3,      23,     10,     8000,   _S("DIVER_ZOMBIE")},
    { ZOMBIE_SHARK_RIDER,       REANIM_ZOMBIE_DOLPHINRIDER, 3,      28,     10,     6000,   _S("SHARK_RIDER_ZOMBIE")},
    { ZOMBIE_DISCO,             REANIM_ZOMBIE_DISCO,        5,      18,     5,      4000,   _S("DISCO_ZOMBIE")},
    { ZOMBIE_DISCO_BACKUP,      REANIM_ZOMBIE_DISCO_BACKUP,  1,     18,     1,        0,    _S("BACKUP_DISCO_ZOMBIE")},
    { ZOMBIE_GLASSDOOR,         REANIM_ZOMBIE,              4,      13,     5,     14000,   _S("GLASS_DOOR_ZOMBIE") },
};

static ZombieType gBossZombieList[] = {  //0x69DE1C
    ZombieType::ZOMBIE_TRAFFIC_CONE,
    ZombieType::ZOMBIE_PAIL,
    ZombieType::ZOMBIE_FOOTBALL,
    ZombieType::ZOMBIE_POLEVAULTER,
    ZombieType::ZOMBIE_JACK_IN_THE_BOX,
    ZombieType::ZOMBIE_LADDER,
    ZombieType::ZOMBIE_ZAMBONI,
    ZombieType::ZOMBIE_CATAPULT,
    ZombieType::ZOMBIE_POGO,
    ZombieType::ZOMBIE_NEWSPAPER,
    ZombieType::ZOMBIE_DOOR,
    ZombieType::ZOMBIE_GARGANTUAR
};

ZombieDefinition& GetZombieDefinition(ZombieType theZombieType)
{
    TOD_ASSERT(theZombieType >= 0 && theZombieType < NUM_ZOMBIE_TYPES);
    TOD_ASSERT(gZombieDefs[theZombieType].mZombieType == theZombieType);

    return gZombieDefs[theZombieType];
}

//0x522510
Zombie::Zombie()
{
}

//0x522580
void Zombie::ZombieInitialize(int theRow, ZombieType theType, bool theVariant, Zombie* theParentZombie, int theFromWave)
{
    TOD_ASSERT(theType >= 0 && theType <= ZombieType::NUM_ZOMBIE_TYPES);

    mFromWave = theFromWave;
    mRow = theRow;
    mPosX = 780 + Rand(ZOMBIE_START_RANDOM_OFFSET); 
    mPosY = GetPosYBasedOnRow(theRow);
    mVelX = 0.0f;
    mVelZ = 0.0f;
    mWidth = 120;
    mHeight = 120;
    mFrame = 0;
    mUpdateTwice = false;
    mGiant = false;
    mTiny = false;
    mWalkBack = false;
    mPrevFrame = 0;
    mZombieType = theType;
    mVariant = theVariant;
    mIsEating = false;
    mReverseCounter = 0;
    mJustGotShotCounter = 0;
    mShieldJustGotShotCounter = 0;
    mHelmetJustGotShotCounter = 0;
    mShieldRecoilCounter = 0;
    mHelmetRecoilCounter = 0;
    mChilledCounter = 0;
    mIceTrapCounter = 0;
    mButteredCounter = 0;
    mMindControlled = false;
    mBlowingAway = false;
    mHasHead = true;
    mHasArm = true;
    mHasObject = false;
    mInPool = false;
    mOnHighGround = false;
    mHelmType = HelmType::HELMTYPE_NONE;
    mShieldType = ShieldType::SHIELDTYPE_NONE;
    mYuckyFace = false;
    mYuckyFaceCounter = 0;
    mAnimCounter = 0;
    mGroanCounter = RandRangeInt(300, 400);
    mAnimTicksPerFrame = 12;
    mAnimFrames = 12;
    mZombieAge = 0;
    mTargetCol = -1;
    mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
    mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
    mPhaseCounter = 0;
    mHitUmbrella = false;
    mDroppedLoot = false;
    mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
    mZombieRect = Rect(36, 0, 42, 115);
    mZombieAttackRect = Rect(50, 0, 20, 115);
    mPlayingSong = false;
    mZombieFade = -1;
    mFlatTires = false;
    mScaleZombie = 1.0f;
    mUseLadderCol = -1;
    mShieldHealth = 0;
    mHasDucky = false;
    mHelmHealth = 0;
    mAltitude = 0.0f;
    mFlyingHealth = 0;
    mOriginalAnimRate = 0.0f;
    mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
    mSummonCounter = 0;
    mBossStompCounter = -1;
    mBossBungeeCounter = -1;
    mBossHeadCounter = -1;
    mBodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mTargetPlantID = PlantID::PLANTID_NULL;
    mBossMode = 0;
    mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
    mSpecialHeadReanimID = ReanimationID::REANIMATIONID_NULL;
    mFlagReanimID = ReanimationID::REANIMATIONID_NULL;
    mPlantHeadID = ReanimationID::REANIMATIONID_NULL;
    mPlantHeadOriginalAnimRate = 0;
    mTargetRow = -1;
    mFireballRow = -1;
    mIsFireBall = false;
    mMoweredReanimID = ReanimationID::REANIMATIONID_NULL;
    mLastPortalX = -1;
    isImmuneToGarlic = RandRangeInt(0,1000) == 0;
    mDanceSwitchRows = 0;
    mCannotBeHammered = false;
    mIsSunbeaned = false;
    mSunProduced = 0;
    mHelmetReanimID = ReanimationID::REANIMATIONID_NULL;
    mExtraReanimID = ReanimationID::REANIMATIONID_NULL;

    mBlinkCountdown = 0;
    mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

    mPlantHeadType = SEED_NONE;

    for (int i = 0; i < MAX_ZOMBIE_FOLLOWERS; i++)
    {
        mFollowerZombieID[i] = ZombieID::ZOMBIEID_NULL;
    }
    if (mBoard && mBoard->IsFlagWave(mFromWave))
    {
        mPosX += 40.0f;
    }
    PickRandomSpeed();
    mBodyHealth = 270;

    const ZombieDefinition& aZombieDef = GetZombieDefinition(mZombieType);
    RenderLayer aRenderLayer = RenderLayer::RENDER_LAYER_ZOMBIE;
    int aRenderOffset = 4;
    if (aZombieDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        LoadReanim(aZombieDef.mReanimationType);
    }

    switch (theType)
    {
    case ZombieType::ZOMBIE_NORMAL:  //0x5227E9
    {
        LoadPlainZombieReanim();
        mIsAllergicToNuts = RandRangeInt(0, 1000) == 0;

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING) {
            mHelmType = HelmType::HELMTYPE_DIGGER;
            mHelmHealth = 100;
            ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);

            Reanimation* aHelmetReanim = mApp->AddReanimation(0, 0, 0, REANIM_DIGGER);
            aHelmetReanim->mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
            aHelmetReanim->SetFramesForLayer("anim_idle");
            aHelmetReanim->ShowOnlyTrack("Zombie_digger_hardhat");
            aHelmetReanim->mAnimRate = 0.0f;

            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHelmetReanim, 0, 0);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, -5, -5, 0, 1.25f, 1.25f);

            mHelmetReanimID = mApp->ReanimationGetID(aHelmetReanim);
            AttachHelmet();
        }
        break;
    }

    case ZombieType::ZOMBIE_DUCKY_TUBE:  //0x5227F8
        LoadPlainZombieReanim();
        break;

    case ZombieType::ZOMBIE_TRAFFIC_CONE:  //0x52280B
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_cone", RENDER_GROUP_NORMAL);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = HelmType::HELMTYPE_TRAFFIC_CONE;
        mHelmHealth = 370;
        AttachHelmet();
        break;

    case ZombieType::ZOMBIE_PAIL:  //0x5228A4
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = HelmType::HELMTYPE_PAIL;
        mHelmHealth = 1100;
        AttachHelmet();
        break;

    case ZombieType::ZOMBIE_GLASSDOOR:
    case ZombieType::ZOMBIE_DOOR:  //0x522939
        mShieldType = ShieldType::SHIELDTYPE_DOOR;
        mShieldHealth = 1100;

        LoadPlainZombieReanim();
        AttachShield();

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING)
        {
            ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
            ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
            mHelmType = HelmType::HELMTYPE_PAIL;
            mHelmHealth = 1100;
            AttachHelmet();
        }
        break;

    case ZombieType::ZOMBIE_YETI:  //0x522963
        mBodyHealth = 1350;
        mPhaseCounter = RandRangeInt(1500, 2000);
        mHasObject = true;
        mZombieAttackRect = Rect(20, 0, 50, 115);
        break;

    case ZombieType::ZOMBIE_LADDER:  //0x52299B
        mBodyHealth = 500;
        mShieldHealth = 500;
        mShieldType = ShieldType::SHIELDTYPE_LADDER;
        mZombieAttackRect = Rect(10, 0, 50, 115);
        if (IsOnBoard())
        {
            mZombiePhase = ZombiePhase::PHASE_LADDER_CARRYING;
            StartWalkAnim(0);
        }
        AttachShield();
        break;

    case ZombieType::ZOMBIE_BUNGEE:  //0x522A10
    {
        mBodyHealth = 450;
        mAnimFrames = 4;
        mAltitude = BUNGEE_ZOMBIE_HEIGHT + RandRangeInt(0, 150);
        mVelX = 0.0f;

        if (IsOnBoard())
        {
            PickBungeeZombieTarget(-1);

            if (mDead)
            {
                return;
            }

            mZombiePhase = ZombiePhase::PHASE_BUNGEE_DIVING;
        }
        else
        {
            mZombiePhase = ZombiePhase::PHASE_BUNGEE_CUTSCENE;
            mPhaseCounter = RandRangeInt(0, 200);
        }

        PlayZombieReanim("anim_drop", ReanimLoopType::REANIM_LOOP, 0, 24.0f);
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_bungi_rightarm_lower2", RENDER_GROUP_ARMS);
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_bungi_rightarm_hand2", RENDER_GROUP_ARMS);
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_bungi_leftarm_lower2", RENDER_GROUP_ARMS);
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_bungi_leftarm_hand2", RENDER_GROUP_ARMS);
        aBodyReanim->SetTruncateDisappearingFrames(nullptr, false);

        aRenderLayer = RenderLayer::RENDER_LAYER_GRAVE_STONE;
        aRenderOffset = 7;
        mZombieRect = Rect(-20, 22, 110, 94);
        mZombieAttackRect = Rect(0, 0, 0, 0);
        mVariant = false;
        break;
    }
    
    case ZombieType::ZOMBIE_BLACK_FOOTBALL:
    case ZombieType::ZOMBIE_FOOTBALL:  //0x522B6E
    {
        mZombieRect = Rect(50, 0, 57, 115);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = (mZombieType == HELMTYPE_BLACKFOOTBALL ? HelmType::HELMTYPE_BLACKFOOTBALL : HelmType::HELMTYPE_FOOTBALL);
        mHelmHealth = 1400;
        mAnimTicksPerFrame = 6;
        mVariant = false;
        
        AttachHelmet();
        break;
    }
    case ZombieType::ZOMBIE_DIGGER:  //0x522BCC
    {
        mHelmType = HelmType::HELMTYPE_DIGGER;
        mHelmHealth = 100;
        mVariant = false;
        mHasObject = true;
        mZombieRect = Rect(50, 0, 28, 115);
        
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        aBodyReanim->SetTruncateDisappearingFrames(nullptr, false);
        
        if (!IsOnBoard())
        {
            mZombiePhase = ZombiePhase::PHASE_DIGGER_CUTSCENE;
        }
        else
        {
            mZombiePhase = ZombiePhase::PHASE_DIGGER_TUNNELING;
            AddAttachedParticle(60, 100, ParticleEffect::PARTICLE_DIGGER_TUNNEL);
            aRenderOffset = 7;
            PlayZombieReanim("anim_dig", ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME, 0, 12.0f);
            PickRandomSpeed();
        }

        AttachHelmet();

        break;
    }

    case ZombieType::ZOMBIE_POLEVAULTER:  //0x522CB4
        mBodyHealth = 500;
        mAnimTicksPerFrame = 6;
        mZombiePhase = ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT;
        mHasObject = true;
        mVariant = false;
        mPosX = 780 + 70 + Rand(10);

        if (IsOnBoard())
        {
            PlayZombieReanim("anim_run", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
            PickRandomSpeed();
        }

        if (mApp->IsWallnutBowlingLevel())
            mZombieAttackRect = Rect(-229, 0, 270, 115);
        else
            mZombieAttackRect = Rect(-29, 0, 70, 115);
        

        break;

    case ZombieType::ZOMBIE_DOLPHIN_RIDER:  //0x522D5C
        mBodyHealth = 500;
        mAnimTicksPerFrame = 6;
        mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING;
        mVariant = false;
        if (IsOnBoard())
        {
            PlayZombieReanim("anim_walkdolphin", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
            PickRandomSpeed();
        }
        SetupWaterTrack("zombie_dolphinrider_whitewater");
        SetupWaterTrack("zombie_dolphinrider_dolphininwater");
        break;

    case ZombieType::ZOMBIE_GARGANTUAR:
    case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:  //0x523D10
    {
        mIsInstantDeath = RandRangeInt(0, 1000) == 0;
        mWidth = 180;
        mHeight = 180;
        mBodyHealth = 3000;
        mAnimFrames = 24;
        mAnimTicksPerFrame = 8;
        mPosX = 780 + 45 + Rand(10);
        mZombieRect = Rect(-17, -38, 125, 154);
        mZombieAttackRect = Rect(-30, -38, 89, 154);
        mVariant = false;
        aRenderOffset = 8;
        mHasObject = true;

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING) {
            mHelmType = HelmType::HELMTYPE_FOOTBALL;
            mHelmHealth = 1400;
            Reanimation* aHelmetReanim = mApp->AddReanimation(0, 0, 0, REANIM_ZOMBIE_FOOTBALL);
            aHelmetReanim->mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
            aHelmetReanim->SetFramesForLayer("anim_idle");
            aHelmetReanim->ShowOnlyTrack("zombie_football_helmet");
            aHelmetReanim->mAnimRate = 0.0f;

            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_gargantua_jaw");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHelmetReanim, 0, 0);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, -75, -45, 0, 1.5f, 1.5f);

            mHelmetReanimID = mApp->ReanimationGetID(aHelmetReanim);
            AttachHelmet();
        }

        int aPoleHit = Rand(100);
        int aPoleVariant;
        if (!IsOnBoard() || mBoard->mLevel == 48)
        {
            aPoleVariant = 0;
        }
        else
        {
            aPoleVariant = aPoleHit < 10 ? 2 : aPoleHit < 35 ? 1 : 0;
        }

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aPoleVariant == 2)
        {
            aBodyReanim->SetImageOverride("Zombie_gargantuar_telephonepole", IMAGE_REANIM_ZOMBIE_GARGANTUAR_ZOMBIE);
        }
        else if (aPoleVariant == 1)
        {
            aBodyReanim->SetImageOverride("Zombie_gargantuar_telephonepole", IMAGE_REANIM_ZOMBIE_GARGANTUAR_DUCKXING);
        }

        if (mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
            mBodyHealth *= 2;
            aBodyReanim->SetImageOverride("anim_head1", IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE);
        }

        break;
    }

    case ZombieType::ZOMBIE_ZAMBONI:  //0x522DD3
        mBodyHealth = 1350;
        mAnimFrames = 2;
        mAnimTicksPerFrame = 8;
       mPosX = 780  + Rand(10);
        aRenderOffset = 8;
        PlayZombieReanim("anim_drive", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        mZombieRect = Rect(0, -13, 153, 140);
        mZombieAttackRect = Rect(10, -13, 133, 140);
        mVariant = false;
        break;

    case ZombieType::ZOMBIE_CATAPULT:  //0x522E82
        mBodyHealth = 850;
        mPosX = 780  + 25 + Rand(10);
        mSummonCounter = 20;
        if (IsOnBoard())
        {
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 0, 5.5f);
        }
        else
        {
            PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 8.0f);
        }
        mZombieRect = Rect(0, -13, 153, 140);
        mZombieAttackRect = Rect(10, -13, 133, 140);
        mVariant = false;
        break;

    case ZombieType::ZOMBIE_DIVER:
    case ZombieType::ZOMBIE_SNORKEL:  //0x522F43
        mZombieRect = Rect(12, 0, 62, 115);
        mZombieAttackRect = Rect(-5, 0, 55, 115);
        SetupWaterTrack("Zombie_snorkle_whitewater");
        SetupWaterTrack("Zombie_snorkle_whitewater2");
        mVariant = false;
        mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING;  
        ReanimShowTrack("Zombie_diver", RENDER_GROUP_HIDDEN);
        if (mZombieType == ZOMBIE_DIVER)
        {
            mHelmHealth = 1100;
            mHelmType = HELMTYPE_DIVER;
            ReanimShowTrack("Zombie_diver", RENDER_GROUP_NORMAL);
            AttachHelmet();
        }
        break;

    case ZombieType::ZOMBIE_JACK_IN_THE_BOX:  //0x522FBC
    {
        mBodyHealth = 500;
        mAnimTicksPerFrame = 6;

        int aDistance = 450 + Rand(300);
        if (Rand(20) == 0)  // 早爆的概率
        {
            aDistance /= 3;
        }
        mPhaseCounter = (int)(aDistance / mVelX) * ZOMBIE_LIMP_SPEED_FACTOR;
        mZombieAttackRect = Rect(20, 0, 50, 115);

        if (mApp->IsScaryPotterLevel())
        {
            mPhaseCounter = 10;
        }
        if (IsOnBoard())
        {
            mZombiePhase = ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING;
        }

        break;
    }

    case ZombieType::ZOMBIE_BOBSLED:  //0x523071
    {
        aRenderOffset = 6;

        if (theParentZombie)
        {
            int aPosition = 0;
            while (aPosition < NUM_BOBSLED_FOLLOWERS && theParentZombie->mFollowerZombieID[aPosition] != ZombieID::ZOMBIEID_NULL)
            {
                aPosition++;
            }
            TOD_ASSERT(aPosition < 3);
            theParentZombie->mFollowerZombieID[aPosition] = mBoard->ZombieGetID(this);
            mRelatedZombieID = mBoard->ZombieGetID(theParentZombie);

            mPosX = theParentZombie->mPosX + (aPosition + 1) * 50;
            if (aPosition == 0)
            {
                aRenderOffset = 1;
                mAltitude = 9.0f;
            }
            else if (aPosition == 1)
            {
                aRenderOffset = 2;
                mAltitude = -7.0f;
            }
            else
            {
                aRenderOffset = 0;
                mAltitude = 9.0f;
            }
        }
        else
        {
            mPosX = 780 + 80;
            mZombieRect = Rect(-50, 0, 275, 115);
            mHelmType = HelmType::HELMTYPE_BOBSLED;
            mHelmHealth = 300;
            mAltitude = -10.0f;
        }

        mVelX = 0.6f;
        mZombiePhase = ZombiePhase::PHASE_BOBSLED_SLIDING;
        mPhaseCounter = 500;
        mVariant = false;

        if (mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE)
        {
            PlayZombieReanim("anim_jump", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 20.0f);
            mApp->ReanimationGet(mBodyReanimID)->mAnimTime = 1.0f;
            mAltitude = 18.0f;
        }
        else if (IsOnBoard())
        {
            PlayZombieReanim("anim_push", ReanimLoopType::REANIM_LOOP, 0, 30.0f);
        }

        break;
    }

    case ZombieType::ZOMBIE_FLAG:  //0x5231E8
    {
        mHasObject = true;
        LoadPlainZombieReanim();

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        Reanimation* aFlagReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FLAG);
        aFlagReanim->PlayReanim("Zombie_flag", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mFlagReanimID = mApp->ReanimationGetID(aFlagReanim);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_flaghand");
        AttachReanim(aTrackInstance->mAttachmentID, aFlagReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;

        mPosX = 780;
        break;
    }

    case ZombieType::ZOMBIE_POGO:  //0x5232A6
        mVariant = false;
        mZombiePhase = ZombiePhase::PHASE_POGO_BOUNCING;
        mPhaseCounter = Rand(POGO_BOUNCE_TIME) + 1;
        mHasObject = true;
        mBodyHealth = 500;
        mZombieAttackRect = Rect(10, 0, 30, 115);
        PlayZombieReanim("anim_pogo", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 40.0f);
        mApp->ReanimationGet(mBodyReanimID)->mAnimTime = 1.0f;
        break;

    case ZombieType::ZOMBIE_NEWSPAPER:  //0x52333D
        mZombieAttackRect = Rect(20, 0, 50, 115);
        mZombiePhase = ZombiePhase::PHASE_NEWSPAPER_READING;
        mShieldType = ShieldType::SHIELDTYPE_NEWSPAPER;
        mShieldHealth = 150;
        mVariant = false;
        AttachShield();
        break;

    case ZombieType::ZOMBIE_BALLOON:  //0x523397
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        aBodyReanim->SetTruncateDisappearingFrames(nullptr, false);

        if (IsOnBoard())
        {
            mAltitude = 25.0f;
            if (IsOnHighGround())
                mAltitude -= HIGH_GROUND_HEIGHT;

            mZombiePhase = ZombiePhase::PHASE_BALLOON_FLYING;
            PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, aBodyReanim->mAnimRate);
        }
        else
        {
            float aAnimRate = RandRangeFloat(8.0f, 10.0f);
            SetAnimRate(aAnimRate);
        }

        Reanimation* aPropellerReanim = mApp->AddReanimation(0.0f, 0.0f, 0, aZombieDef.mReanimationType);
        aPropellerReanim->SetFramesForLayer("Propeller");
        aPropellerReanim->mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
        aPropellerReanim->AttachToAnotherReanimation(aBodyReanim, "hat");

        mFlyingHealth = 20;

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING) {
            mHelmType = HelmType::HELMTYPE_BLACKFOOTBALL;
            mHelmHealth = 1400;
            Reanimation* aHelmetReanim = mApp->AddReanimation(0, 0, 0, REANIM_ZOMBIE_BLACKFOOTBALL);
            aHelmetReanim->mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
            aHelmetReanim->SetFramesForLayer("anim_idle");
            aHelmetReanim->ShowOnlyTrack("zombie_football_helmet");
            aHelmetReanim->mAnimRate = 0.0f;

            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_balloon_top");
            AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHelmetReanim, 0, 0);
            TodScaleRotateTransformMatrix(aAttachEffect->mOffset, -10, -25, 0, 1, 1);

            mHelmetReanimID = mApp->ReanimationGetID(aHelmetReanim);
            AttachHelmet();
        }


        mZombieRect = Rect(36, 30, 42, 115);
        mZombieAttackRect = Rect(20, 30, 50, 115);
        mVariant = false;
        break;
    }

    case ZombieType::ZOMBIE_DISCO:
    case ZombieType::ZOMBIE_DANCER:  //0x5234DF
        if (!IsOnBoard())
        {
            PlayZombieReanim("anim_moonwalk", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        }
        else
        {
            mZombiePhase = ZombiePhase::PHASE_DANCER_DANCING_IN;
            mVelX = 0.5f;
            mPhaseCounter = 300 + Rand(12);
            PlayZombieReanim("anim_moonwalk", ReanimLoopType::REANIM_LOOP, 0, 24.0f);
        }

        mBodyHealth = 500;
        mVariant = false;
        break;

    case ZombieType::ZOMBIE_DISCO_BACKUP:
    case ZombieType::ZOMBIE_BACKUP_DANCER:  //0x523541
        if (!IsOnBoard())
        {
            PlayZombieReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        }
        mZombiePhase = ZombiePhase::PHASE_DANCER_DANCING_LEFT;
        mVariant = false;
        break;

    case ZombieType::ZOMBIE_IMP:  //0x523576
        /*if (!IsOnBoard()) {
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        }*/

        if (mApp->IsIZombieLevel()){
            mBodyHealth = 70;
        }
        break;

    case ZombieType::ZOMBIE_NINJAIMP: 
    {
        if (IsOnBoard())
        {
            mApp->PlayFoley(FoleyType::FOLEY_SWING);
            bool aAllowPlanternTarget = true;
            if (CountNinjaImpsTargetingPlanterns() == mBoard->CountPlantByType(SEED_PLANTERN) - 1)
            {
                aAllowPlanternTarget = false;
            }

            TodWeightedGridArray aPicks[MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y];
            int aPickCount = 0;

            for (int x = 2; x < MAX_GRID_SIZE_X; x++)
            {

                for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
                {
                    int aWeight = 1;
                    if (mBoard->GetGraveStoneAt(x, y) || mBoard->mGridSquareType[x][y] == GridSquareType::GRIDSQUARE_DIRT || mBoard->mGridSquareType[x][y] == GRIDSQUARE_POOL)
                    {
                        continue;
                    }

                    Plant* aPlant = mBoard->GetTopPlantAt(x, y, PlantPriority::TOPPLANT_BUNGEE_ORDER);
                    if (aPlant)
                    {
                        if (!aAllowPlanternTarget && aPlant->mSeedType == SEED_PLANTERN)
                        {
                            continue;
                        }

                        if (aPlant->mSeedType == SeedType::SEED_GRAVEBUSTER || aPlant->mSeedType == SeedType::SEED_COBCANNON)
                        {
                            continue;
                        }

                        aWeight = 10000;

                        if ((mBoard->StageHasFog() || mBoard->StageHasDarkness()) && aPlant->mSeedType == SEED_PLANTERN) {
                            aWeight *= 50;
                        }
                        else if (mApp->mGameMode == GAMEMODE_CHALLENGE_POGO_PARTY && aPlant->mSeedType == SEED_MAGNETSHROOM)
                        {
                            aWeight *= 50;
                        }
                        else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOBSLED_BONANZA && aPlant->mSeedType == SEED_SQUASH)
                        {
                            aWeight *= 50;
                        }

                    }

                    if (!mBoard->BungeeIsTargetingCell(x, y))
                    {
                        aPicks[aPickCount].mX = x;
                        aPicks[aPickCount].mY = y;
                        aPicks[aPickCount].mWeight = aWeight;
                        aPickCount++;
                    }
                }

            }

            if (aPickCount == 0)
            {
                DieNoLoot();
                return;
            }

            TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(aPicks, aPickCount);
            mTargetCol = aGrid->mX;
            SetRow(aGrid->mY);
            mPosY = GetPosYBasedOnRow(mRow);

            float targetX = mBoard->GridToPixelX(mTargetCol, mRow);
            float aThrowingDistance = mPosX - targetX - 85;

            mAltitude = 88.0f * mScaleZombie;
            mRenderOrder = mRenderOrder + 1;
            mZombiePhase = ZombiePhase::PHASE_IMP_GETTING_THROWN;
            mVelX = 3.0f;
            mVelZ = 0.5f * (aThrowingDistance / mVelX) * THOWN_ZOMBIE_GRAVITY;
            PlayZombieReanim("anim_thrown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 18.0f);
            UpdateReanim();
            mApp->PlayFoley(FoleyType::FOLEY_IMP);
        }
        break;
    }
    
    case ZombieType::ZOMBIE_BOSS:  //0x5235BE
        mPosX = 0;
        mPosY = 0;
        mZombieRect = Rect(700, 80, 90, 430);
        mZombieAttackRect = Rect(0, 0, 0, 0);
        aRenderLayer = RenderLayer::RENDER_LAYER_TOP;
        mBodyHealth = mApp->IsAdventureMode() ? 40000 : 60000;
        if (IsOnBoard())
        {
            mSummonCounter = 500;
            mBossHeadCounter = 5000;

            if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
            {
                mBodyHealth = 40000;
                mBossHeadCounter += 2000;
            }

            if (mBoard->mIsVeteran)
            {
                mBodyHealth *= 1.5f;
            }
           
            if (mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID){
                mBodyHealth = 15000;
                PlayZombieReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
                mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT;
            }
            else {
                PlayZombieReanim("anim_enter", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12.0f);
                mZombiePhase = ZombiePhase::PHASE_BOSS_ENTER;
            }
            
        }
        else
        {
            PlayZombieReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        }
        BossSetupReanim();
        break;
    
    case ZombieType::ZOMBIE_PEA_HEAD:  //0x52369B
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head2", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

        if (IsOnBoard())
        {
            aBodyReanim->SetFramesForLayer("anim_walk2");
        }

        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        aTrackInstance->mImageOverride = IMAGE_BLANK;
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PEASHOOTER);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);

        mPhaseCounter = 150;
        mVariant = false;

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING) {
            ReanimShowPrefix("anim_cone", RENDER_GROUP_NORMAL);
            ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
            mHelmType = HelmType::HELMTYPE_TRAFFIC_CONE;
            mHelmHealth = 370;
            AttachHelmet();
        }

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_PEASHOOTER;

        break;
    }

    case ZombieType::ZOMBIE_REPEATER_HEAD:  //0x52369B
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head2", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

        if (IsOnBoard())
        {
            aBodyReanim->SetFramesForLayer("anim_walk2");
        }

        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        aTrackInstance->mImageOverride = IMAGE_BLANK;
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_REPEATER);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);

        mPhaseCounter = 150;
        mVariant = false;

        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING) {
            ReanimShowPrefix("anim_cone", RENDER_GROUP_NORMAL);
            ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
            mHelmType = HelmType::HELMTYPE_TRAFFIC_CONE;
            mHelmHealth = 370;
            AttachHelmet();
        }

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_REPEATER;

        break;
    }
    
    case ZombieType::ZOMBIE_WALLNUT_HEAD:  //0x523719
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_WALLNUT);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 50.0f, 0.0f, 0.2f, -0.8f, 0.8f);

        mHelmType = HelmType::HELMTYPE_WALLNUT;
        mHelmHealth = 1100;
        mVariant = false;

        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        mPlantHeadType = SEED_WALLNUT;

        break;
    }

    case ZombieType::ZOMBIE_TALLNUT_HEAD:  //0x523842
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_TALLNUT);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 37.0f, 0.0f, 0.2f, -0.8f, 0.8f);

        mHelmType = HelmType::HELMTYPE_TALLNUT;
        mHelmHealth = 2200;
        mVariant = false;
        mPosX += 30.0f;

        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        mPlantHeadType = SEED_TALLNUT;

        break;
    }

    case ZombieType::ZOMBIE_CHERRYBOMB_HEAD:  //0x523977
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_CHERRYBOMB);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55, -30, 0.4f, -1, 1);

        mVariant = false;
        int aDistance = 275 + Rand(175);
        mPhaseCounter = (int)(aDistance / mVelX) * ZOMBIE_LIMP_SPEED_FACTOR;

        /*mShieldType = ShieldType::SHIELDTYPE_DOOR;
        mShieldHealth = 1100;
        LoadPlainZombieReanim();
        AttachShield();

        ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = HelmType::HELMTYPE_PAIL;
        mHelmHealth = 1100;
        AttachHelmet();*/

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_CHERRYBOMB;

        break;
    }

    case ZombieType::ZOMBIE_POTATOMINE_HEAD:  //0x523977
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_POTATOMINE);

        if (IsOnBoard())
        {
            aHeadReanim->PlayReanim("anim_glow", ReanimLoopType::REANIM_LOOP, 0, 12);
            mPhaseCounter = 1500;
        }
        else
        {
            aHeadReanim->PlayReanim("anim_armed", ReanimLoopType::REANIM_LOOP, 0, 12);
            //mState = PlantState::STATE_POTATO_ARMED;
        }

        mPlantHeadOriginalAnimRate = 12;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55, -30, 0.4f, -0.75, 0.75f);

        mVariant = false;

        /*mShieldType = ShieldType::SHIELDTYPE_DOOR;
        mShieldHealth = 1100;
        LoadPlainZombieReanim();
        AttachShield();

        ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = HelmType::HELMTYPE_PAIL;
        mHelmHealth = 1100;
        AttachHelmet();*/

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_POTATOMINE;

        break;
    }

    case ZombieType::ZOMBIE_JALAPENO_HEAD:  //0x523977
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_JALAPENO);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55.0f, -5.0f, 0.2f, -1.0f, 1.0f);

        mVariant = false;
        mBodyHealth = 500;
        int aDistance = 275 + Rand(175);
        mPhaseCounter = (int)(aDistance / mVelX) * ZOMBIE_LIMP_SPEED_FACTOR;

        /*mShieldType = ShieldType::SHIELDTYPE_DOOR;
        mShieldHealth = 1100;
        LoadPlainZombieReanim();
        AttachShield();

        ReanimShowPrefix("anim_bucket", RENDER_GROUP_NORMAL);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        mHelmType = HelmType::HELMTYPE_PAIL;
        mHelmHealth = 1100;
        AttachHelmet();*/

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_JALAPENO;

        break;
    }
    case ZombieType::ZOMBIE_GATLING_HEAD:  //0x523ABA
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head2", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (IsOnBoard())
        {
            aBodyReanim->SetFramesForLayer("anim_walk2");
        }

        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        aTrackInstance->mImageOverride = IMAGE_BLANK;
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_GATLINGPEA);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);

        mPhaseCounter = 150;
        mVariant = false;

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_GATLINGPEA;

        break;
    }
    case ZombieType::ZOMBIE_SQUASH_HEAD:  //0x523BE7
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head2", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (IsOnBoard())
        {
            aBodyReanim->SetFramesForLayer("anim_walk2");
        }

        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        aTrackInstance->mImageOverride = IMAGE_BLANK;
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SQUASH);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55.0f, -15.0f, 0.2f, -0.75f, 0.75f);

        mZombiePhase = ZombiePhase::PHASE_SQUASH_PRE_LAUNCH;
        mVariant = false;

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_SQUASH;

        break;
    }
    case ZombieType::ZOMBIE_HYPNOSHROOM_HEAD:  //0x523BE7
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head2", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (IsOnBoard())
        {
            aBodyReanim->SetFramesForLayer("anim_walk2");
        }

        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        aTrackInstance->mImageOverride = IMAGE_BLANK;
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_HYPNOSHROOM);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55.0f, -15.0f, 0.2f, -0.75f, 0.75f);        
        mVariant = false;

        mBlinkCountdown = 400 + Sexy::Rand(400);
        mPlantHeadType = SEED_HYPNOSHROOM;

        break;
    }

    case ZombieType::ZOMBIE_EXPLODE_O_NUT_HEAD:  //0x523719
    {
        LoadPlainZombieReanim();
        ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_tie", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Zombie_body");
        Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_EXPLODE_O_NUT);
        aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPlantHeadID = mApp->ReanimationGetID(aHeadReanim);
        AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 0.0f, 0.0f);
        aBodyReanim->mFrameBasePose = 0;
        TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 50.0f, 0.0f, 0.2f, -0.8f, 0.8f);

        mHelmType = HelmType::HELMTYPE_WALLNUT;
        mHelmHealth = 550;
        mVariant = false;

        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        mPlantHeadType = SEED_EXPLODE_O_NUT;

        break;
    }

    }

    if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
    {
        float aAnimRate = RandRangeFloat(8.0f, 10.0f);
        PlayZombieReanim("anim_aquarium_swim", ReanimLoopType::REANIM_LOOP, 0, aAnimRate);

        mZombieHeight = ZombieHeight::HEIGHT_ZOMBIQUARIUM;
        mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_DRIFT;
        mPhaseCounter = 200;
        mBodyHealth = 200;
        mSummonCounter = RandRangeInt(200, 400);
    }

    if ((mApp->IsLittleTroubleLevel()|| mTiny) && (IsOnBoard() || theFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE))
    {
        mScaleZombie = 0.5f;
        mBodyHealth /= 4;
        mHelmHealth /= 4;
        mShieldHealth /= 4;
        mFlyingHealth /= 4;
    }

    if ((mApp->IsBigTroubleLevel() || mGiant) && (IsOnBoard() || theFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE))
    {
        mScaleZombie = 2.0f;
        mBodyHealth *= 4;
        mHelmHealth *= 4;
        mShieldHealth *= 4;
        mFlyingHealth *= 4;
    }

    UpdateAnimSpeed();
    if (mVariant && !IsZombotany(mZombieType))
    {
        ReanimShowPrefix("anim_tongue", RENDER_GROUP_NORMAL);
    }

    mBodyMaxHealth = mBodyHealth;
    mHelmMaxHealth = mHelmHealth;
    mShieldMaxHealth = mShieldHealth;
    mFlyingMaxHealth = mFlyingHealth;
    mDead = false;
    mX = (int)mPosX;
    mY = (int)mPosY;

    mRenderOrder = Board::MakeRenderOrder(aRenderLayer, mRow, aRenderOffset);
    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM)
    {
        mBodyMaxHealth = 300;
    }

    if (IsOnBoard())
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER && mZombieType != ZOMBIE_BOSS && mZombieType != ZOMBIE_BUNGEE)
        {
            mPosX += BOARD_ADDITIONAL_WIDTH;
            mX = (int)mPosX;
        }
        

        PlayZombieAppearSound();
        StartZombieSound();
    }

    UpdateReanim();
}

//0x524040
void Zombie::SetupDoorArms(Reanimation* aReanim, bool theShow)
{
    int aArmGroup = RENDER_GROUP_NORMAL;
    int aDoorGroup = RENDER_GROUP_HIDDEN;
    if (theShow)
    {
        aArmGroup = RENDER_GROUP_HIDDEN;
        aDoorGroup = RENDER_GROUP_NORMAL;
    }

    aReanim->AssignRenderGroupToPrefix("Zombie_outerarm_hand", aArmGroup);
    aReanim->AssignRenderGroupToPrefix("Zombie_outerarm_lower", aArmGroup);
    aReanim->AssignRenderGroupToPrefix("Zombie_outerarm_upper", aArmGroup);
    aReanim->AssignRenderGroupToPrefix("anim_innerarm", aArmGroup);
    aReanim->AssignRenderGroupToPrefix("Zombie_outerarm_screendoor", aDoorGroup);
    aReanim->AssignRenderGroupToPrefix("Zombie_innerarm_screendoor", aDoorGroup);
    aReanim->AssignRenderGroupToPrefix("Zombie_innerarm_screendoor_hand", aDoorGroup);
}

//0x5240B0
void Zombie::SetupReanimLayers(Reanimation* aReanim, ZombieType theZombieType)
{
    aReanim->AssignRenderGroupToPrefix("anim_cone", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("anim_screendoor", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("Zombie_flaghand", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("Zombie_duckytube", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("anim_tongue", RENDER_GROUP_HIDDEN);
    aReanim->AssignRenderGroupToPrefix("Zombie_mustache", RENDER_GROUP_HIDDEN);
    SetupDoorArms(aReanim, false);

    if (theZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE)
    {
        aReanim->AssignRenderGroupToPrefix("anim_cone", RENDER_GROUP_NORMAL);
        aReanim->AssignRenderGroupToPrefix("anim_hair", RENDER_GROUP_HIDDEN);
    }
    else if (theZombieType == ZombieType::ZOMBIE_PAIL)
    {
        aReanim->AssignRenderGroupToPrefix("anim_bucket", RENDER_GROUP_NORMAL);
        aReanim->AssignRenderGroupToPrefix("anim_hair", RENDER_GROUP_HIDDEN);
    }
    else if (theZombieType == ZombieType::ZOMBIE_DOOR || theZombieType == ZombieType::ZOMBIE_GLASSDOOR)
    {
        SetupDoorArms(aReanim, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_NEWSPAPER)
    {
        aReanim->AssignRenderGroupToPrefix("Zombie_paper_paper", RENDER_GROUP_HIDDEN);
    }
    else if (theZombieType == ZombieType::ZOMBIE_FLAG)
    {
        aReanim->AssignRenderGroupToPrefix("anim_innerarm", RENDER_GROUP_HIDDEN);
        aReanim->AssignRenderGroupToTrack("Zombie_flaghand", RENDER_GROUP_NORMAL);
        aReanim->AssignRenderGroupToTrack("Zombie_innerarm_screendoor", RENDER_GROUP_NORMAL);
    }
    else if (theZombieType == ZombieType::ZOMBIE_DUCKY_TUBE)
    {
        aReanim->AssignRenderGroupToPrefix("Zombie_duckytube", RENDER_GROUP_NORMAL);
        
    }
}

//0x524210
void Zombie::ShowDoorArms(bool theShow)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim)
    {
        SetupDoorArms(aBodyReanim, theShow);
        if (!mHasArm)
        {
            ReanimShowPrefix("Zombie_outerarm_lower", RENDER_GROUP_HIDDEN);
            ReanimShowPrefix("Zombie_outerarm_hand", RENDER_GROUP_HIDDEN);
        }
    }
}

//0x524280
void Zombie::ReanimIgnoreClipRect(const char* theTrackName, bool theIgnoreClipRect)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    for (int i = 0; i < aBodyReanim->mDefinition->mTrackCount; i++)
    {
        if (stricmp(aBodyReanim->mDefinition->mTracks[i].mName, theTrackName) == 0)
        {
            aBodyReanim->mTrackInstances[i].mIgnoreClipRect = theIgnoreClipRect;
        }
    }
}

//0x524310
void Zombie::ReanimReenableClipping()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    for (int i = 0; i < aBodyReanim->mDefinition->mTrackCount; i++)
    {
        aBodyReanim->mTrackInstances[i].mIgnoreClipRect = false;
    }
}

void Zombie::ReanimDisableClipping()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    for (int i = 0; i < aBodyReanim->mDefinition->mTrackCount; i++)
    {
        aBodyReanim->mTrackInstances[i].mIgnoreClipRect = true;
    }
}

//0x524370
void Zombie::LoadPlainZombieReanim()
{
    mZombieAttackRect = Rect(20, 0, 50, 115);
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    SetupReanimLayers(aBodyReanim, mZombieType);
    if (mBoard)
    {
        EnableMustache(mBoard->mMustacheMode);
        EnableFuture(mBoard->mFutureMode);
    }

    if ((mBoard && mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL) || mZombieType == ZombieType::ZOMBIE_DUCKY_TUBE)
    {
        ReanimShowPrefix("zombie_duckytube", RENDER_GROUP_NORMAL);
        ReanimIgnoreClipRect("Zombie_duckytube", true);
        ReanimIgnoreClipRect("Zombie_outerarm_hand", true);
        ReanimIgnoreClipRect("Zombie_innerarm3", true);
        ReanimIgnoreClipRect("anim_screendoor", true);
        SetupWaterTrack("Zombie_whitewater");
        SetupWaterTrack("Zombie_whitewater2");
        mHasDucky = true;
    }
}

//0x524470
Reanimation* Zombie::LoadReanim(ReanimationType theReanimationType)
{
    Reanimation* aBodyReanim = mApp->AddReanimation(0.0f, 0.0f, 0, theReanimationType);
    mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
    aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    aBodyReanim->mIsAttachment = true;

    if (!IsOnBoard())
    {
        if (Rand(4) > 0 && aBodyReanim->TrackExists("anim_idle2"))
        {
            float aRanimRate = RandRangeFloat(12.0f, 24.0f);
            PlayZombieReanim("anim_idle2", ReanimLoopType::REANIM_LOOP, 0, aRanimRate);
        }
        else if (aBodyReanim->TrackExists("anim_idle"))
        {
            float aRanimRate = RandRangeFloat(12.0f, 18.0f);
            PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, aRanimRate);
        }

        aBodyReanim->mAnimTime = RandRangeFloat(0.0f, 0.99f);
    }
    else
    {
        StartWalkAnim(0);
    }

    return aBodyReanim;
}

//0x524590
int Zombie::CountBungeesTargetingSunFlowers()
{
    int aCount = 0;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying() && aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != -1)
        {
            Plant* aPlant = mBoard->GetTopPlantAt(aZombie->mTargetCol, aZombie->mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
            if (aPlant && aPlant->MakesSun())
            {
                aCount++;
            }
        }
    }

    return aCount;
}

int Zombie::CountBungeesTargetingPlanterns()
{
    int aCount = 0;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying() && aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != -1)
        {
            Plant* aPlant = mBoard->GetTopPlantAt(aZombie->mTargetCol, aZombie->mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
            if (aPlant && aPlant->mSeedType == SEED_PLANTERN)
            {
                aCount++;
            }
        }
    }

    return aCount;
}

int Zombie::CountNinjaImpsTargetingPlanterns()
{
    int aCount = 0;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying() && aZombie->mZombieType == ZombieType::ZOMBIE_NINJAIMP && aZombie->mTargetCol != -1)
        {
            Plant* aPlant = mBoard->GetTopPlantAt(aZombie->mTargetCol, aZombie->mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
            if (aPlant && aPlant->mSeedType == SEED_PLANTERN)
            {
                aCount++;
            }
        }
    }

    return aCount;
}

//0x5246A0
void Zombie::PickBungeeZombieTarget(int theColumn)
{
    bool aAllowSunFlowerTarget = true;
    if (CountBungeesTargetingSunFlowers() == mBoard->CountSunFlowers() - 1)
    {
        aAllowSunFlowerTarget = false;
    }

    bool aAllowPlanternTarget = true;
    if (CountBungeesTargetingPlanterns() == mBoard->CountPlantByType(SEED_PLANTERN) - 1)
    {
        aAllowPlanternTarget = false;
    }

    TodWeightedGridArray aPicks[MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y];
    int aPickCount = 0;

    for (int x = 0; x < MAX_GRID_SIZE_X; x++)
    {
        if (theColumn == -1 || theColumn == x)  // 限制仅能在 theColumn 列寻找目标，除非 theColumn 为 -1
        {
            for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
            {
                int aWeight = 1;
                if (mBoard->GetGraveStoneAt(x, y) || mBoard->mGridSquareType[x][y] == GridSquareType::GRIDSQUARE_DIRT)
                {
                    continue;
                }

                Plant* aPlant = mBoard->GetTopPlantAt(x, y, PlantPriority::TOPPLANT_BUNGEE_ORDER);
                if (aPlant)
                {
                    if (!aAllowSunFlowerTarget && aPlant->MakesSun())
                    {
                        continue;
                    }


                    if (!aAllowPlanternTarget && aPlant->mSeedType == SEED_PLANTERN)
                    {
                        continue;
                    }
     
                    if (aPlant->mSeedType == SeedType::SEED_GRAVEBUSTER || aPlant->mSeedType == SeedType::SEED_COBCANNON)
                    {
                        continue;
                    }

                    aWeight = 10000;

                    if ((mBoard->StageHasFog() || mBoard->StageHasDarkness()) && aPlant->mSeedType == SEED_PLANTERN) {
                        aWeight *= 50;
                    }
                    else if (mApp->mGameMode == GAMEMODE_CHALLENGE_POGO_PARTY && (aPlant->mSeedType == SEED_MAGNETSHROOM || aPlant->mSeedType == SEED_TALLNUT))
                    {
                        aWeight *= 50;
                    }
                    else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOBSLED_BONANZA && (aPlant->mSeedType == SEED_SPIKEROCK || aPlant->mSeedType == SEED_SPIKEWEED))
                    {
                        aWeight *= 50;
                    }

                }

                if (!mBoard->BungeeIsTargetingCell(x, y))
                {
                    aPicks[aPickCount].mX = x;
                    aPicks[aPickCount].mY = y;
                    aPicks[aPickCount].mWeight = aWeight;
                    aPickCount++;
                }
            }
        }
    }

    if (aPickCount == 0)
    {
        DieNoLoot();
        return;
    }

    TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(aPicks, aPickCount);
    mTargetCol = aGrid->mX;
    SetRow(aGrid->mY);
    mPosX = mBoard->GridToPixelX(mTargetCol, mRow);
    mPosY = GetPosYBasedOnRow(mRow);
}

//0x524970
void Zombie::BungeeDropZombie(Zombie* theDroppedZombie, int theGridX, int theGridY)
{
    mTargetCol = theGridX;
    SetRow(theGridY);
    mPosX = mBoard->GridToPixelX(mTargetCol, mRow);
    mPosY = GetPosYBasedOnRow(mRow);
    PlayZombieReanim("anim_raise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 36.0f);
    mRelatedZombieID = mBoard->ZombieGetID(theDroppedZombie);

    theDroppedZombie->mPosX = mPosX - 15.0f;
    theDroppedZombie->SetRow(theGridY);
    theDroppedZombie->mPosY = GetPosYBasedOnRow(theGridY);
    theDroppedZombie->mZombieHeight = ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED;
    theDroppedZombie->PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
    theDroppedZombie->mRenderOrder = mRenderOrder + 1;
}


//0x524A70
void Zombie::PickRandomSpeed()
{
    if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN)
    {
        mVelX = 0.3f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
    {
        mVelX = 0.2f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
    {
        if (mApp->IsIZombieLevel())
        {
            mVelX = 0.23f;
        }
        else
        {
            mVelX = 0.12f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_IMP && mApp->IsIZombieLevel())
    {
        mVelX = 0.9f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_YETI_RUNNING)
    {
        mVelX = 0.8f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        mVelX = 0.4f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP ||
        mZombieType == ZombieType::ZOMBIE_POGO || mZombieType == ZombieType::ZOMBIE_FLAG)
    {
        mVelX = 0.45f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT || 
        mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER || mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX)
    {
        mVelX = RandRangeFloat(0.66f, 0.68f);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING || mZombieType == ZombieType::ZOMBIE_SQUASH_HEAD || mZombieType == ZombieType::ZOMBIE_HYPNOSHROOM_HEAD)
    {
        mVelX = RandRangeFloat(0.79f, 0.81f);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MAD || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN)
    {
        mVelX = RandRangeFloat(0.89f, 0.91f);
    }
    else
    {
        mVelX = RandRangeFloat(0.23f, 0.32f);
        if (mVelX < 0.3f)
        {
            mAnimTicksPerFrame = 12;
        }
        else
        {
            mAnimTicksPerFrame = 15;
        }
    }

    if ((mApp->IsBigTroubleLevel() || mGiant))
    {
        ApplyAnimRate(mOriginalAnimRate * 0.75f);
        mVelX *= 0.75f;
    }

    //if (IsZombotany(mZombieType))
    //    mVelX *= 1.64f;

    //if (mBoard && mBoard->mChallenge->mBloodMooning)  
     //   mVelX *= 2;

    UpdateAnimSpeed();
}

//0x524C70
void Zombie::BungeeStealTarget()
{
    PlayZombieReanim("anim_grab", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);

    Plant* aPlant = mBoard->GetTopPlantAt(mTargetCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
    if (aPlant && !aPlant->NotOnGround())
    {
        TOD_ASSERT(aPlant->mSeedType != SeedType::SEED_GRAVEBUSTER);

        if (aPlant->mSeedType != SeedType::SEED_COBCANNON && aPlant->mSeedType != SeedType::SEED_GRAVEBUSTER)
        {
            mTargetPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(aPlant);
            aPlant->mOnBungeeState = PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);
        }
    }
}

//0x524D70
void Zombie::BungeeLiftTarget()
{
    PlayZombieReanim("anim_raise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 36.0f);
    
    Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mTargetPlantID);
    if (aPlant == nullptr)
        return;

//#ifdef DO_FIX_BUGS
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie != this && aZombie->mTargetPlantID == mTargetPlantID)
        {
            aZombie->mTargetPlantID = PlantID::PLANTID_NULL;  // 修复类似于 IZ 蹦极刷阳光的 Bug
        }
    }
//#endif

    aPlant->mOnBungeeState = PlantOnBungeeState::RISING_WITH_BUNGEE;
    mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
    
    Reanimation* aPlantReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aPlantReanim)
    {
        aPlantReanim->mAnimRate = 0.1f;
    }

    if (aPlant->mSeedType == SeedType::SEED_CATTAIL && mBoard->GetTopPlantAt(mTargetCol, mRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN))
    {
        mBoard->NewPlant(mTargetCol, mRow, SeedType::SEED_LILYPAD, SeedType::SEED_NONE);
    }

    if (mApp->IsIZombieLevel())
    {
        mBoard->mChallenge->IZombiePlantDropRemainingSun(aPlant);
    }
}

//0x524EF0
void Zombie::BungeeLanding()
{
    if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING && mAltitude < 1500.0f && !mApp->IsFinalBossLevel())
    {
        if (mRelatedZombieID != ZOMBIEID_NULL && mBoard->ZombieGet(mRelatedZombieID)->mZombieType != ZOMBIE_BACKUP_DANCER || mRelatedZombieID == ZOMBIEID_NULL)
        {
            mApp->PlayFoley(FoleyType::FOLEY_BUNGEE_SCREAM);
            mZombiePhase = ZombiePhase::PHASE_BUNGEE_DIVING_SCREAMING;
        }
    }

    if (mAltitude > 40.0f)
        return;
    
    Plant* aPlant = mBoard->FindUmbrellaPlant(mTargetCol, mRow);
    if (aPlant && mApp->mGameMode != GAMEMODE_CHALLENGE_HAIL)
    {
        mApp->PlaySample(SOUND_BOING);
        mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);

        aPlant->DoSpecial();

        mZombiePhase = ZombiePhase::PHASE_BUNGEE_RISING;
        mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
        mHitUmbrella = true;

        return;
    }

    if (mAltitude > 0.0f)
        return;

    mAltitude = 0.0f;
    Zombie* aZombie = mBoard->ZombieTryToGet(mRelatedZombieID);
    if (aZombie)  // 存在关联的僵尸时，释放空投的僵尸
    {
        aZombie->mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
        aZombie->StartWalkAnim(0);

        mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
        mZombiePhase = ZombiePhase::PHASE_BUNGEE_RISING;
        PlayZombieReanim("anim_raise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 36.0f);
    }
    else  // 不存在关联的僵尸时，开始偷取植物
    {
        mZombiePhase = ZombiePhase::PHASE_BUNGEE_AT_BOTTOM;
        mPhaseCounter = 300;
        PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 5, 24.0f);
        mApp->ReanimationGet(mBodyReanimID)->mAnimTime = 0.5f;
    }
}

//0x525180
void Zombie::UpdateZombieBungee()
{
    if (IsDeadOrDying() || IsImmobilizied())
        return;

    if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING || mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING_SCREAMING)
    {
        float aOldAltitude = mAltitude;
        mAltitude -= 8.0f;
        if (mAltitude <= BUNGEE_ZOMBIE_HEIGHT - 404.0f && aOldAltitude > BUNGEE_ZOMBIE_HEIGHT - 404.0f && mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
        {
            mApp->PlayFoley(FoleyType::FOLEY_GRASSSTEP);  // 靶子扎地的音效
        }

        BungeeLanding();
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_AT_BOTTOM)
    {
        if (mPhaseCounter <= 0)
        {
            BungeeStealTarget();
            mZombiePhase = ZombiePhase::PHASE_BUNGEE_GRABBING;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_GRABBING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            BungeeLiftTarget();
            mZombiePhase = ZombiePhase::PHASE_BUNGEE_RISING;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_HIT_OUCHY)
    {
        if (mPhaseCounter <= 0)
        {
            DieWithLoot();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_RISING)
    {
        mAltitude += 8.0f;
        if (mAltitude >= 600.0f)
        {
            DieNoLoot();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_CUTSCENE)
    {
        mAltitude = TodAnimateCurve(200, 0, mPhaseCounter, 40, 0, TodCurves::CURVE_SIN_WAVE);
        if (mPhaseCounter <= 0)
        {
            mPhaseCounter = 200;
        }
    }

    mX = (int)mPosX;
    mY = (int)mPosY;
}

//0x525350
void Zombie::PogoBreak(unsigned int theDamageFlags)
{
    if (!mHasObject)
        return;

    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
    {
        //ZombieDrawPosition aDrawPos;
        //GetDrawPos(aDrawPos);

        float aPosX, aPosY;
        GetTrackPosition("Zombie_pogo_stick", aPosX, aPosY);
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_POGO);
        OverrideParticleScale(aParticle);
    }

    TOD_ASSERT(mZombiePhase != ZombiePhase::PHASE_ZOMBIE_DYING && mZombiePhase != ZombiePhase::PHASE_ZOMBIE_BURNED && !mDead);

    mZombieHeight = ZombieHeight::HEIGHT_FALLING;
    mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
    StartWalkAnim(0);
    mZombieRect = Rect(36, 17, 42, 115);
    mZombieAttackRect = Rect(20, 17, 50, 115);
    mShieldHealth = 0;
    mShieldType = ShieldType::SHIELDTYPE_NONE;
    mHasObject = false;
}

//0x525460
bool Zombie::IsBouncingPogo()
{
    return mZombiePhase >= ZombiePhase::PHASE_POGO_BOUNCING && mZombiePhase <= ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_7;
}

//0x525480
void Zombie::UpdateZombiePogo()
{
    if (mApp->IsIZombieLevel() && mBoard->mChallenge->IZombieGetBrainTarget(this))
    {
        mShieldType = ShieldType::SHIELDTYPE_NONE;
        PogoBreak(0U);
    }

    if (IsDeadOrDying() || IsImmobilizied() || !IsBouncingPogo() || mZombieHeight == ZombieHeight::HEIGHT_IN_TO_CHIMNEY)
        return;

    float aHeight = 40.0f;
    if (mZombiePhase >= ZombiePhase::PHASE_POGO_HIGH_BOUNCE_1 && mZombiePhase <= ZombiePhase::PHASE_POGO_HIGH_BOUNCE_6)
    {
        aHeight = 50.0f + 20.0f * (mZombiePhase - ZombiePhase::PHASE_POGO_HIGH_BOUNCE_1);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_2)
    {
        aHeight = 90.0f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_7)
    {
        aHeight = 170.0f;
    }

    aHeight *= mScaleZombie;

    mAltitude = TodAnimateCurveFloat(POGO_BOUNCE_TIME, 0, mPhaseCounter, 9.0f, aHeight + 9.0f, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
    mFrame = ClampInt(3 - mAltitude / 3, 0, 3);

    if (mPhaseCounter == 7)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        aBodyReanim->mAnimTime = 0.0f;
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    }
    if (IsOnBoard() && mPhaseCounter == 5)
    {
        mApp->PlayFoley(FoleyType::FOLEY_POGO_ZOMBIE);
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_UP_TO_HIGH_GROUND)
    {
        mAltitude += HIGH_GROUND_HEIGHT;
        mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
    }
    else if (mZombieHeight == ZombieHeight::HEIGHT_DOWN_OFF_HIGH_GROUND)
    {
        mOnHighGround = false;
        mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
    }
    else if (mOnHighGround)
    {
        mAltitude += HIGH_GROUND_HEIGHT;
    }

    if (mZombiePhase == ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_2 && mPhaseCounter == 70)
    {
        Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
        if (aPlant && mApp->mGameMode != GAMEMODE_CHALLENGE_BIG_TROUBLE && ( aPlant->mSeedType == SeedType::SEED_TALLNUT || aPlant->mSeedType == SeedType::SEED_GIANT_WALLNUT))
        {
            mApp->PlayFoley(FoleyType::FOLEY_BONK);
            mApp->AddTodParticle(aPlant->mX + 60, aPlant->mY - 20, mRenderOrder + 1, ParticleEffect::PARTICLE_TALL_NUT_BLOCK);

            mShieldType = ShieldType::SHIELDTYPE_NONE;
            PogoBreak(0U);
            return;
        }
    }

    if (mPhaseCounter != 0)
        return;

    Plant* aPlant = nullptr;
    if (IsOnBoard())
    {
        aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
    }
    if (aPlant == nullptr)
    {
        mZombiePhase = ZombiePhase::PHASE_POGO_BOUNCING;

        PickRandomSpeed();
        mPhaseCounter = POGO_BOUNCE_TIME;
        return;
    }

    if (mZombiePhase == ZombiePhase::PHASE_POGO_HIGH_BOUNCE_1)
    {
        mZombiePhase = ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_2;
        mVelX = (mX - aPlant->mX + 60) / (float)POGO_BOUNCE_TIME;  // 速度 = 跳跃距离 / 跳跃时间
        mPhaseCounter = POGO_BOUNCE_TIME;
    }
    else
    {
        mZombiePhase = ZombiePhase::PHASE_POGO_HIGH_BOUNCE_1;
        mVelX = 0.0f;
        mPhaseCounter = POGO_BOUNCE_TIME;
    }
}

//0x525730
void Zombie::ZombieCatapultFire(Plant* thePlant)
{
    float aOriginX = mPosX + 113.0f;
    float aOriginY = mPosY - 44.0f;
    int aTargetX, aTargetY;
    if (thePlant)
    {
        aTargetX = thePlant->mX;
        aTargetY = thePlant->mY;
    }
    else
    {
        aTargetX = mPosX - 300.0f;
        aTargetY = 0.0f;
    }

    mApp->PlayFoley(FoleyType::FOLEY_BASKETBALL);

    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_BASKETBALL);
    float aRangeX = aOriginX - aTargetX - 20.0f;

    float aRangeY = aTargetY - aOriginY;
    if (aRangeX < 40.0f)
    {
        aRangeX = 40.0f;
    }
    aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
    aProjectile->mVelX = -aRangeX / 120.0f;
    aProjectile->mVelY = 0.0f;
    aProjectile->mVelZ = aRangeY / 120.0f - 14;
    aProjectile->mAccZ = 0.115f;
    aProjectile->mEnemy = true;
}

//0x525890
Plant* Zombie::FindCatapultTarget()
{
    Plant* aTarget = nullptr;

    for (int i = 0; i < mBoard->mMaxPlayers; i++) {
        PlantID playerPlantID = mBoard->mPlayers[i]->mPlayerPlantID;
        
        if (playerPlantID != PLANTID_NULL) {
            Plant* mPlayerPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int) playerPlantID);
            if (mPlayerPlant && mPlayerPlant->mSeedType != SEED_NONE && mPlayerPlant->mRow == mRow) {
                return mPlayerPlant;
            }
        }
    }

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && mX >= aPlant->mX + 100 && !aPlant->NotOnGround() && !aPlant->IsSpiky())
        {
            if (aTarget == nullptr || aPlant->mPlantCol < aTarget->mPlantCol)
            {
                aTarget = mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
            }
        }
    }

    return aTarget;
}

//0x5259D0
void Zombie::UpdateZombieCatapult()
{
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_NORMAL)
    {
        if (mPosX <= 650 && FindCatapultTarget() && mSummonCounter > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_CATAPULT_LAUNCHING;
            mPhaseCounter = 300;
            PlayZombieReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_CATAPULT_LAUNCHING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.545f))
        {
            Plant* aPlant = FindCatapultTarget();
            ZombieCatapultFire(aPlant);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mSummonCounter--;
            if (mSummonCounter == 4)
            {
                ReanimShowTrack("Zombie_catapult_basketball", RENDER_GROUP_HIDDEN);
            }
            else if (mSummonCounter == 3)
            {
                ReanimShowTrack("Zombie_catapult_basketball2", RENDER_GROUP_HIDDEN);
            }
            else if (mSummonCounter == 2)
            {
                ReanimShowTrack("Zombie_catapult_basketball3", RENDER_GROUP_HIDDEN);
            }
            else if (mSummonCounter == 1)
            {
                ReanimShowTrack("Zombie_catapult_basketball4", RENDER_GROUP_HIDDEN);
            }

            if (mSummonCounter == 0)
            {
                PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 20, 6.0f);
                mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            }
            else
            {
                PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, 12.0f);
                mZombiePhase = ZombiePhase::PHASE_CATAPULT_RELOADING;
            }
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_CATAPULT_RELOADING && mPhaseCounter == 0)
    {
        Plant* aPlant = FindCatapultTarget();
        if (aPlant)
        {
            mZombiePhase = ZombiePhase::PHASE_CATAPULT_LAUNCHING;
            mPhaseCounter = 300;
            PlayZombieReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        }
        else
        {
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 20, 6.0f);
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        }
    }
}

//0x525B60
void Zombie::LandFlyer(unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY) && mZombiePhase == ZombiePhase::PHASE_BALLOON_FLYING)
    {
        mApp->PlaySample(SOUND_BALLOON_POP);
        mZombiePhase = ZombiePhase::PHASE_BALLOON_POPPING;
        PlayZombieReanim("anim_pop", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
    }

    if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
    {
        DieWithLoot();
    }
    else
    {
        mZombieHeight = ZombieHeight::HEIGHT_FALLING;
    }
}

//0x525BE0
void Zombie::UpdateZombieFlyer()
{
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY && mPosX < 720.0f)
    {
        if (mZombieType == ZOMBIE_BALLOON && mZombiePhase == PHASE_BALLOON_FLYING)
        {
            float offsetY = (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0);
            mAltitude -= 0.1f;
            if (mAltitude < -25.0f - offsetY)
            {
                LandFlyer(0U);
            }
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_BALLOON_POPPING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_BALLOON_WALKING;
            StartWalkAnim(0);
        }
    }

    if (mApp->IsIZombieLevel() && mZombiePhase == ZombiePhase::PHASE_BALLOON_FLYING && mBoard->mChallenge->IZombieGetBrainTarget(this))
    {
        LandFlyer(0U);
    }
}

//0x525CE0
void Zombie::UpdateZombieNewspaper()
{
    if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MADDENING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_NEWSPAPER_MAD;
            if (mBoard->CountZombiesOnScreen() <= 10 && mHasHead)
            {
                mApp->PlayFoley(FoleyType::FOLEY_NEWSPAPER_RARRGH);
            }

            StartWalkAnim(20);
            aBodyReanim->SetImageOverride("anim_head1", IMAGE_REANIM_ZOMBIE_PAPER_MADHEAD);
        }
    }
}

//0x525DC0
void Zombie::UpdateZombiePolevaulter()
{
    if (mZombiePhase == PHASE_ZOMBIE_DYING)
        return;

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && mHasHead && mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL)
    {
        Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
        if (aPlant)
        {
            if (mBoard->GetLadderAt(aPlant->mPlantCol, aPlant->mRow))
            {
                float aPlantX = mBoard->GridToPixelX(aPlant->mPlantCol, aPlant->mRow) + 40;
                if (aPlantX > mPosX && mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL && mUseLadderCol != aPlant->mPlantCol)
                {
                    mZombieHeight = ZombieHeight::HEIGHT_UP_LADDER;
                    mUseLadderCol = aPlant->mPlantCol;
                }
                return;
            }

            mZombiePhase = ZombiePhase::PHASE_POLEVAULTER_IN_VAULT;
            PlayZombieReanim("anim_jump", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);

            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            float aAnimDuration = aBodyReanim->mFrameCount / aBodyReanim->mAnimRate * 100.0f;
            int aJumpDistance = mX - aPlant->mX - 80;
            if (mApp->IsWallnutBowlingLevel())
                aJumpDistance = 0;
            mVelX = aJumpDistance / aAnimDuration;
            mHasObject = false;
        }

        if (mApp->IsIZombieLevel() && mBoard->mChallenge->IZombieGetBrainTarget(this))
        {
            mZombiePhase = ZombiePhase::PHASE_POLEVAULTER_POST_VAULT;
            StartWalkAnim(0);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

        bool aJumpEnds = false;
        if (aBodyReanim->mAnimTime > 0.6f && aBodyReanim->mAnimTime <= 0.7f)
        {
            Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
            if (aPlant && mApp->mGameMode != GAMEMODE_CHALLENGE_BIG_TROUBLE && (aPlant->mSeedType == SeedType::SEED_TALLNUT || aPlant->mSeedType == SeedType::SEED_GIANT_WALLNUT))
            {
                mApp->PlayFoley(FoleyType::FOLEY_BONK);
                aJumpEnds = true;
                mApp->AddTodParticle(aPlant->mX + 60, aPlant->mY - 20, mRenderOrder + 1, ParticleEffect::PARTICLE_TALL_NUT_BLOCK);

                mZombieHeight = ZombieHeight::HEIGHT_FALLING;
                mPosX = aPlant->mX;
                mPosY -= 30.0f;
            }
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            aJumpEnds = true;
            mPosX -= 150.0f * mScaleZombie;
        }
        if (aBodyReanim->ShouldTriggerTimedEvent(0.2f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_GRASSSTEP);
        }
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_POLEVAULT);
        }

        if (aJumpEnds)
        {
            mX = (int)mPosX;
            mZombiePhase = ZombiePhase::PHASE_POLEVAULTER_POST_VAULT;
            mZombieAttackRect = Rect(50, 0, 20, 115);
            StartWalkAnim(0);
        }
        else
        {
            float aOldPosX = mPosX;
            mPosX -= 150.0f * mScaleZombie * aBodyReanim->mAnimTime;
            mPosY = GetPosYBasedOnRow(mRow);
            mPosX = aOldPosX;
        }
    }
}

//0x526190
bool Zombie::IsTanglekelpTarget()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mSeedType == SeedType::SEED_TANGLEKELP && aPlant->mTargetZombieID == mBoard->ZombieGetID(this))
        {
            return true;
        }
    }

    return false;
}

//0x5261E0
void Zombie::UpdateZombieDolphinRider()
{
    if (IsTangleKelpTarget())
        return;

    bool aBackwards = IsWalkingBackwards();
    if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING && !aBackwards)
    {
        if (mX > 700 && mX <= 720 && (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON))
        {
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_INTO_POOL;
            PlayZombieReanim("anim_jumpinpool", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 16.0f);
        }
        else if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL && mX < 700)
        {
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_INTO_POOL;
            PlayZombieReanim("anim_ride", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.56f))
        {
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 83, mY + 73, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.2f, 0.8f);
            TodParticleSystem* part = mApp->AddTodParticle(mX - 46, mY + 115, mRenderOrder + 1, ParticleEffect::PARTICLE_PLANTING_POOL);
            part->OverrideScale(nullptr, mScaleZombie);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mPosX -= 70.0f * mScaleZombie;
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_RIDING;
            mInPool = true;
            mZombieAttackRect = Rect(-29, 0, 70, 115);
            PlayZombieReanim("anim_ride", ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME, 0, 12.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
            return;

        if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_NORMAL)
        {
            mAltitude = -40.0f;
            mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN;

            PoolSplash(false);
            StartWalkAnim(0);
            PickRandomSpeed();
        }
        else if (mX <= 10 && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
        {
            mAltitude = -40.0f;
            mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING;
            
            PoolSplash(false);
            PlayZombieReanim("anim_walkdolphin", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
            PickRandomSpeed();
            return;
        }

        if (mHasHead && !IsTanglekelpTarget())
        {
            Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
            if (aPlant)
            {
                mApp->PlayFoley(FoleyType::FOLEY_DOLPHIN_BEFORE_JUMPING);
                mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);

                mVelX = 0.5f;
                mZombiePhase = ZombiePhase::PHASE_DOLPHIN_IN_JUMP;
                mPhaseCounter = DOLPHIN_JUMP_TIME;
                PlayZombieReanim("anim_dolphinjump", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 10.0f);
            }
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        mAltitude = TodAnimateCurveFloat(DOLPHIN_JUMP_TIME, 0, mPhaseCounter, 0.0f, 10.0f, TodCurves::CURVE_LINEAR);
        
        bool aJumpEnds = false;
        if (aBodyReanim->ShouldTriggerTimedEvent(0.3f))
        {
            Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_VAULT);
            if (aPlant && mApp->mGameMode != GAMEMODE_CHALLENGE_BIG_TROUBLE && (aPlant->mSeedType == SeedType::SEED_TALLNUT || aPlant->mSeedType == SeedType::SEED_GIANT_WALLNUT))
            {
                mApp->PlayFoley(FoleyType::FOLEY_BONK);
                aJumpEnds = true;
                TodParticleSystem* part = mApp->AddTodParticle(aPlant->mX + 60, aPlant->mY - 20, mRenderOrder + 1, ParticleEffect::PARTICLE_TALL_NUT_BLOCK);
                part->OverrideScale(nullptr, mScaleZombie);
                mZombieHeight = ZombieHeight::HEIGHT_FALLING;
                mPosX = aPlant->mX + 25.0f ;
                mAltitude = 30.0f;
            }
        }
        else if (aBodyReanim->ShouldTriggerTimedEvent(0.49f))
        {
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 63, mY + 73, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.2f, 0.8f);
            TodParticleSystem* part = mApp->AddTodParticle(mX - 26, mY + 115, mRenderOrder + 1, ParticleEffect::PARTICLE_PLANTING_POOL);
            part->OverrideScale(nullptr, mScaleZombie);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
            mVelX = 0.0f;
        }
        else if (aBodyReanim->mLoopCount > 0)
        {
            aJumpEnds = true;
            mPosX -= 94.0f * mScaleZombie;
            mAltitude = 0.0f;
        }

        if (aJumpEnds)
        {
            mZombieAttackRect = Rect(30, 0, 30, 115);
            mZombieRect = Rect(20, 0, 42, 115);
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL;
            StartWalkAnim(0);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
            return;

        if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_NORMAL || mX <= 10 && !aBackwards || mX > 680 && aBackwards)
        {
            mAltitude = -40.0f;
            mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN;

            PoolSplash(false);
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
            PickRandomSpeed();
            StartWalkAnim(0);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN)
    {
        if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
        {
            mAltitude = 0.0f;
            mZombieHeight = ZombieHeight::HEIGHT_IN_TO_POOL;
            mZombiePhase = ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL;

            PoolSplash(false);
            StartWalkAnim(0);
            PickRandomSpeed();
        }
    }
}

//0x526720
void Zombie::UpdateZombieSnorkel()
{
    bool aBackwards = IsWalkingBackwards();
    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING && !aBackwards)
    {
        if (mX > 700 && mX <= 720 && (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON))
        {
            mVelX = 0.2f;
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_INTO_POOL;
            PlayZombieReanim("anim_jumpinpool", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 16.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        mAltitude = TodAnimateCurveFloat(0, 1000, aBodyReanim->mAnimTime * 1000, 0.0f, 10.0f, TodCurves::CURVE_LINEAR);

        if (aBodyReanim->ShouldTriggerTimedEvent(0.83f))
        {
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 47, mY + 73, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.2f, 0.8f);
            mApp->AddTodParticle(mX - 10, mY + 115, mRenderOrder + 1, ParticleEffect::PARTICLE_PLANTING_POOL);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL;
            mInPool = true;
            PlayZombieReanim("anim_swim", ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME, 0, 12.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
    {
        if (!mHasHead)
        {
            TakeDamage(1800, 9U);
        }
        else if (((mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_NORMAL && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON) || mX <= 25) && !aBackwards)
        {
            mAltitude = -90.0f;
            mPosX -= 15.0f;
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING;
            mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
            mInPool = false;
            PoolSplash(false);
            StartWalkAnim(0);
        }
        else if (mX > 640 &&  mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_NORMAL && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON && aBackwards)
        {
            mAltitude = -90.0f;
            mPosX += 15.0f;
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING;
            mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
            mInPool = false;
            PoolSplash(false);
            StartWalkAnim(0);
        }
        else if (mIsEating)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_UP_TO_EAT;
            PlayZombieReanim("anim_uptoeat", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
        }
        else {
            ApplyAnimRate(mOriginalAnimRate);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_UP_TO_EAT)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (!mIsEating)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_DOWN_FROM_EAT;
            PlayZombieReanim("anim_uptoeat", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, -24.0f);
        }
        else if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_EATING_IN_POOL;
            PlayZombieReanim("anim_eat", ReanimLoopType::REANIM_LOOP, 0, 0.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_EATING_IN_POOL)
    {
        if (!mIsEating)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_DOWN_FROM_EAT;
            PlayZombieReanim("anim_uptoeat", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, -24.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_DOWN_FROM_EAT)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL;
            PlayZombieReanim("anim_swim", ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME, 0, 12.0f);
            PickRandomSpeed();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING)
    {
        if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
        {
            mAltitude = 0.0f;
            mZombieHeight = ZombieHeight::HEIGHT_IN_TO_POOL;
            mZombiePhase = ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL;
            PoolSplash(false);
            StartWalkAnim(0);
        }
    }
}

//0x526AE0
void Zombie::UpdateZombieJackInTheBox()
{
    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
    {
        if (mPhaseCounter <= 0 && mHasHead)
        {
            mPhaseCounter = 110;
            mZombiePhase = ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING;

            StopZombieSound();
            mApp->PlaySample(SOUND_BOING);
            PlayZombieReanim("anim_pop", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING)
    {
        int plntRadius =  JackInTheBoxPlantRadius * mScaleZombie;
        int zombRadius = JackInTheBoxZombieRadius * mScaleZombie;

        if (mPhaseCounter == 80)
        {
            mApp->PlayFoley(FoleyType::FOLEY_JACK_SURPRISE);
        }

        if (mPhaseCounter <= 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);

            int aPosX = mX + mWidth / 2;
            int aPosY = mY + mHeight / 2 - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0);
            if (mMindControlled)
            {
                mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, zombRadius, 1, true, 127);
            }
            else
            {
                mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, zombRadius, 1, true, 255);
                mBoard->KillAllPlantsInRadius(aPosX, aPosY, plntRadius);
            }

            TodParticleSystem* particle = mApp->AddTodParticle(aPosX, aPosY, Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 0), ParticleEffect::PARTICLE_JACKEXPLODE);
            particle->OverrideScale(nullptr, mScaleZombie);
            mBoard->ShakeBoard(4, -6);
            DieNoLoot();

            if (mApp->IsScaryPotterLevel())
            {
                mBoard->mChallenge->ScaryPotterJackExplode(aPosX, aPosY);
            }
        }
    }
}

//0x526D10
void Zombie::UpdateZombieGargantuar()
{
    if (mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_SMASHING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.64f))
        {
//#ifdef DO_FIX_BUGS
            if (mMindControlled)  // 魅惑巨人砸僵尸
            {
                Zombie* aZombie = FindZombieTarget();
                if (aZombie)
                {
                    int aDamage = mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ? 1000 : 500;
                    aZombie->TakeDamage(aDamage, 0U);
                }
            }
            else
            {
                Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
                if (aPlant)
                {
                    if (aPlant->mSeedType == SeedType::SEED_SPIKEROCK)
                    {
                        TakeDamage(20, 32U);
                        aPlant->SpikeRockTakeDamage();

                        if (aPlant->mPlantHealth <= 0)
                        {
                            if ((mApp->IsBigTroubleLevel() || mGiant))
                            {
                                SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);

                                for (int curCol = -2; curCol < 0; curCol++)
                                {
                                    SquishAllInSquare(aPlant->mPlantCol + curCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                                }
                            }
                            else {
                                SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                            }
                        }
                    }
                    else
                    {
                        if ((mApp->IsBigTroubleLevel() || mGiant))
                        {
                            SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);

                            for (int curCol = -2; curCol < 1; curCol++)
                            {
                                SquishAllInSquare(aPlant->mPlantCol + curCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                            }
                        }
                        else {
                            SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                        }
                    }
                }

                Zombie* aZombie = FindZombieTarget();
                if (aZombie)
                {
                    int aDamage = mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ? 1000 : 500;
                    aZombie->TakeDamage(aDamage, 0U);
                }

                if (mApp->IsScaryPotterLevel())
                {
                    int aGridX = mBoard->PixelToGridX(mPosX, mPosY);
                    GridItem* aScaryPot = mBoard->GetScaryPotAt(aGridX, mRow);
                    if (aScaryPot)
                    {
                        mBoard->mChallenge->ScaryPotterOpenPot(aScaryPot);
                    }
                }

                if (mApp->IsIZombieLevel())
                {
                    GridItem* aBrain = mBoard->mChallenge->IZombieGetBrainTarget(this);
                    if (aBrain)
                    {
                        mBoard->mChallenge->IZombieSquishBrain(aBrain);
                    }
                }
            }
/*#else
            Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
            if (aPlant)
            {
                if (aPlant->mSeedType == SeedType::SEED_SPIKEROCK)
                {
                    TakeDamage(20, 32U);
                    aPlant->SpikeRockTakeDamage();
                    if (aPlant->mPlantHealth <= 0)
                    {
                        SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                    }
                }
                else
                {
                    SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                }
            }

            if (mApp->IsScaryPotterLevel())
            {
                int aGridX = mBoard->PixelToGridX(mPosX, mPosY);
                GridItem* aScaryPot = mBoard->GetScaryPotAt(aGridX, mRow);
                if (aScaryPot)
                {
                    mBoard->mChallenge->ScaryPotterOpenPot(aScaryPot);
                }
            }

            if (mApp->IsIZombieLevel())
            {
                GridItem* aBrain = mBoard->mChallenge->IZombieGetBrainTarget(this);
                if (aBrain)
                {
                    mBoard->mChallenge->IZombieSquishBrain(aBrain);
                }
            }
#endif*/

            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
            mBoard->ShakeBoard(0, 3);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            StartWalkAnim(20);
        }

        return;
    }
    
    float aThrowingDistance = mPosX - 360.0f * mScaleZombie;
    if (mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_THROWING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.74f))
        {
            mHasObject = false;
            ReanimShowPrefix("Zombie_imp", RENDER_GROUP_HIDDEN);
            ReanimShowTrack("Zombie_gargantuar_whiterope", RENDER_GROUP_HIDDEN);
            mApp->PlayFoley(FoleyType::FOLEY_SWING);

            Zombie* aZombieImp = mBoard->AddZombie(ZombieType::ZOMBIE_IMP, mFromWave);
            if (aZombieImp == nullptr)
                return;
            
            float aMinThrowDistance = 40.0f * mScaleZombie;
            if (mBoard->StageHasRoof())
            {
                aThrowingDistance -= 180.0f * mScaleZombie;
                aMinThrowDistance = -140.0f * mScaleZombie;
            }
            if (aThrowingDistance < aMinThrowDistance)
            {
                aThrowingDistance = aMinThrowDistance;
            }
            else if (aThrowingDistance > 140.0f * mScaleZombie)
            {
                aThrowingDistance -= RandRangeFloat(0.0f, 100.0f * mScaleZombie);
            }

            aZombieImp->mPosX = mPosX - 133.0f * mScaleZombie;
            aZombieImp->mPosY = GetPosYBasedOnRow(mRow);
            aZombieImp->SetRow(mRow);
            aZombieImp->mVariant = false;
            aZombieImp->mAltitude = 88.0f * mScaleZombie;
            aZombieImp->mRenderOrder = mRenderOrder + 1;
            aZombieImp->mZombiePhase = ZombiePhase::PHASE_IMP_GETTING_THROWN;
//#ifdef DO_FIX_BUGS
            aZombieImp->mScaleZombie = mScaleZombie;
            aZombieImp->mBodyHealth *= mScaleZombie * mScaleZombie;
            aZombieImp->mBodyMaxHealth *= mScaleZombie * mScaleZombie;

            if (mMindControlled)
            {
                aZombieImp->mPosX = mPosX + mWidth;
                aZombieImp->StartMindControlled();
                aZombieImp->mVelX = -3.0f;
            }
            else
            {
                aZombieImp->mVelX = 3.0f;
            }
/*#else
            aZombieImp->mVelX = 3.0f;
#endif*/
            aZombieImp->mChilledCounter = mChilledCounter;
            aZombieImp->mVelZ = 0.5f * (aThrowingDistance / aZombieImp->mVelX) * THOWN_ZOMBIE_GRAVITY;
            aZombieImp->PlayZombieReanim("anim_thrown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 18.0f);
            aZombieImp->UpdateReanim();
            mApp->PlayFoley(FoleyType::FOLEY_IMP);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            StartWalkAnim(20);
        }

        return;
    }

    if (IsImmobilizied() || !mHasHead)
        return;
    
    if (mHasObject && mBodyHealth < mBodyMaxHealth / 2 && aThrowingDistance > 40.0f)
    {
        mZombiePhase = ZombiePhase::PHASE_GARGANTUAR_THROWING;
        PlayZombieReanim("anim_throw", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        return;
    }

/*#ifdef DO_FIX_BUGS
    bool doSmash = FindZombieTarget();
#else*/
    bool doSmash = false;
    Plant* aPlantTarget = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
    Zombie* zombTarget = FindZombieTarget();

    if (aPlantTarget || zombTarget)
    {
        doSmash = true;
    }
    else if (mApp->IsScaryPotterLevel())
    {
        int aGridX = mBoard->PixelToGridX(mPosX, mPosY);
        if (mBoard->GetScaryPotAt(aGridX, mRow))
        {
            doSmash = true;
        }
    }
    else if (mApp->IsIZombieLevel())
    {
        if (mBoard->mChallenge->IZombieGetBrainTarget(this))
        {
            doSmash = true;
        }
    }
//#endif

    if (doSmash)
    {
        mZombiePhase = ZombiePhase::PHASE_GARGANTUAR_SMASHING;
        mApp->PlayFoley(FoleyType::FOLEY_LOW_GROAN);
        PlayZombieReanim("anim_smash", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 16.0f);
    }
}

//0x5272E0
void Zombie::UpdateZombieImp()
{
    if (mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN)
    {
        mVelZ -= THOWN_ZOMBIE_GRAVITY;
        mAltitude += mVelZ;
        mPosX -= mVelX;

        float aDiffY = GetPosYBasedOnRow(mRow) - mPosY;
        mPosY += aDiffY;
        mAltitude += aDiffY;
        float offsetY = 0;
        if (IsOnHighGround())
            offsetY = HIGH_GROUND_HEIGHT;
        if (mAltitude <= 0.0f + offsetY)
        {
            mAltitude = 0.0f + offsetY;
            mZombiePhase = ZombiePhase::PHASE_IMP_LANDING;
            PlayZombieReanim("anim_land", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_IMP_LANDING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            StartWalkAnim(0);
        }
    }
}

//0x5273D0
void Zombie::UpdateZombiePeaHead()
{
    if (!mHasHead)
        return;

    if (mPhaseCounter == 35)
    {
        EndBlink();
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mPlantHeadOriginalAnimRate = 35.0f;
    }
    else if (mPhaseCounter == 0)
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mApp->PlayFoley(FoleyType::FOLEY_THROW);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("anim_head1");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);

        float aOriginX = mPosX + aTransform.mTransX - 9.0f;
        float aOriginY = mPosY + aTransform.mTransY + 6.0f - mAltitude;
        //#ifdef DO_FIX_BUGS
        if (mMindControlled)  // 魅惑修复
        {
            aOriginX += 90.0f * mScaleZombie;
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mDamageRangeFlags = 1;
            //aProjectile->mZombieParentID = (ZombieID)mBoard->mZombies.DataArrayGetID(this);
        }
        else
        {
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mEnemy = true;
            aProjectile->mBackwards = true;
            // aProjectile->mZombieParentID = (ZombieID)mBoard->mZombies.DataArrayGetID(this);
            /*
             Plant* theTargetPlant = nullptr;
             Plant* aPlant = nullptr;

             while (mBoard->IteratePlants(aPlant))
             {
                 if (aPlant->mRow == mRow && !aPlant->mIsInSelector && mX >= aPlant->mX + 100 && !aPlant->NotOnGround() && !aPlant->IsSpiky())
                 {
                     if (theTargetPlant == nullptr || aPlant->mPlantCol > theTargetPlant->mPlantCol)
                     {
                         theTargetPlant = mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
                     }
                 }
             }

             float aRangeX, aRangeY;
             if (theTargetPlant)
             {
                 Rect aPlantRect = theTargetPlant->GetPlantRect();
                 aRangeX = aPlantRect.mX - aOriginX + 30;
                 aRangeY = aPlantRect.mY - aOriginY;
             }
             else {
                 aRangeX = LAWN_XMIN - aOriginX + 30;
                 aRangeY = 0;
             }

             if (aRangeY > 40.0f)
                 aRangeY = 40.0f;



             aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
             aProjectile->mVelX = aRangeX / 120.0f;
             aProjectile->mVelY = 0.0f;
             aProjectile->mVelZ = aRangeY / 120.0f - 7.0f;
             aProjectile->mAccZ = 0.115f;
             */
        }
        /*#else
                Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_ZOMBIE_PEA);
                aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        #endif*/

        mPhaseCounter = 150;
    }
}

void Zombie::UpdateZombieRepeaterHead()
{
    if (!mHasHead)
        return;

    if (mPhaseCounter == 35 || mPhaseCounter == 60)
    {
        EndBlink();
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mPlantHeadOriginalAnimRate = 45;
    }
    else if ((mPhaseCounter == 0 || mPhaseCounter == 25) && (mChilledCounter > 0 && mZombieAge % 2 == 1 || !mChilledCounter))
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        mApp->PlayFoley(FoleyType::FOLEY_THROW);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("anim_head1");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);

        float aOriginX = mPosX + aTransform.mTransX - 9.0f;
        float aOriginY = mPosY + aTransform.mTransY + 6.0f - mAltitude;
        //#ifdef DO_FIX_BUGS
        if (mMindControlled)  // 魅惑修复
        {
            aOriginX += 90.0f * mScaleZombie;
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mDamageRangeFlags = 1;
            //aProjectile->mZombieParentID = (ZombieID)mBoard->mZombies.DataArrayGetID(this);
        }
        else
        {
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mEnemy = true;
            aProjectile->mBackwards = true;
            // aProjectile->mZombieParentID = (ZombieID)mBoard->mZombies.DataArrayGetID(this);
            /*
                Plant* theTargetPlant = nullptr;
                Plant* aPlant = nullptr;

                while (mBoard->IteratePlants(aPlant))
                {
                    if (aPlant->mRow == mRow && !aPlant->mIsInSelector && mX >= aPlant->mX + 100 && !aPlant->NotOnGround() && !aPlant->IsSpiky())
                    {
                        if (theTargetPlant == nullptr || aPlant->mPlantCol > theTargetPlant->mPlantCol)
                        {
                            theTargetPlant = mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
                        }
                    }
                }

                float aRangeX, aRangeY;
                if (theTargetPlant)
                {
                    Rect aPlantRect = theTargetPlant->GetPlantRect();
                    aRangeX = aPlantRect.mX - aOriginX + 30;
                    aRangeY = aPlantRect.mY - aOriginY;
                }
                else {
                    aRangeX = LAWN_XMIN - aOriginX + 30;
                    aRangeY = 0;
                }

                if (aRangeY > 40.0f)
                    aRangeY = 40.0f;



                aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
                aProjectile->mVelX = aRangeX / 120.0f;
                aProjectile->mVelY = 0.0f;
                aProjectile->mVelZ = aRangeY / 120.0f - 7.0f;
                aProjectile->mAccZ = 0.115f;
                */
        }
        /*#else
                Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_ZOMBIE_PEA);
                aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        #endif*/

        if (mPhaseCounter == 0)
        {
            mPhaseCounter = 150;
            aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 15.0f);
            mPlantHeadOriginalAnimRate = 15.0f;
        }
    }
}

void Zombie::BurnRow(int theRow)  // 此函数专用于在定义了 DO_FIX_BUGS 时修复火爆辣椒僵尸的 Bug
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(127))
        {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
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
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}

//0x5275C0
void Zombie::UpdateZombieJalapenoHead()
{
    if (!mHasHead)
        return;

    if (mPhaseCounter == 100)
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->mAnimRate = 15;
        mPlantHeadOriginalAnimRate = 15;
        aHeadReanim->SetFramesForLayer("anim_explode");
        mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    }
    else if (mPhaseCounter == 0)
    {
        mNoCoin = true;

        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);
        mBoard->DoFwoosh(mRow);
        mBoard->ShakeBoard(3, -4);
        
//#ifdef DO_FIX_BUGS
        if (mMindControlled)
        {
            BurnRow(mRow);
        }
        else
        {
            Plant* aPlant = nullptr;
            while (mBoard->IteratePlants(aPlant))
            {
                //Rect aPlantRect = aPlant->GetPlantRect();
                if (aPlant->mRow == mRow && !aPlant->NotOnGround())
                {
                    mBoard->mPlantsEaten++;
                    //aPlant->Die();
                    aPlant->ApplyBurn();
                }
            }
        }

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->ReanimationDie();
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;

        TakeDamage(1800, 9U);

        //DieNoLoot();
/*#else
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            //Rect aPlantRect = aPlant->GetPlantRect();
            if (aPlant->mRow == mRow && !aPlant->NotOnGround())
            {
                mBoard->mPlantsEaten++;
                aPlant->Die();
            }
        }
        DieNoLoot();
#endif*/
    }
}

void Zombie::UpdateZombieCherryBombHead()
{
    if (!mHasHead)
        return;

    if (mPhaseCounter == 100)
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->mAnimRate = 15;
        mPlantHeadOriginalAnimRate = 15;
        aHeadReanim->SetFramesForLayer("anim_explode");
        mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    }
    else if (mPhaseCounter == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        mNoCoin = true;

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("anim_head1");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);

        float aOriginX = mPosX + aTransform.mTransX;
        float aOriginY = mPosY + aTransform.mTransY;

        int aPosX = aOriginX + 40;
        int aPosY = aOriginY + 40 - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0);

        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mEnemy != mMindControlled)
                continue;

            if (GetCircleRectOverlap(aPosX, aPosY, JackInTheBoxPlantRadius * mScaleZombie, aPlant->GetPlantRect()))
            {
                aPlant->ApplyBurn();
            }

        }

        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->ReanimationDie();
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;

        TakeDamage(1800, 9U);

        //DieNoLoot();
/*#else
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            //Rect aPlantRect = aPlant->GetPlantRect();
            if (aPlant->mRow == mRow && !aPlant->NotOnGround())
            {
                mBoard->mPlantsEaten++;
                aPlant->Die();
            }
        }
        DieNoLoot();
#endif*/
    }
}

//0x527750
void Zombie::UpdateZombieGatlingHead()
{
    if (!mHasHead)
        return;

    if (mPhaseCounter == 100)
    {
        EndBlink();
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 38.0f);
        mPlantHeadOriginalAnimRate = 38.0f;
    }
    else if ((mPhaseCounter == 18 || mPhaseCounter == 35 || mPhaseCounter == 51 || mPhaseCounter == 68) && (mChilledCounter > 0 && mZombieAge % 2 == 1 || !mChilledCounter))
    {
        mApp->PlayFoley(FoleyType::FOLEY_THROW);

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->mAnimRate = 38.0f;
        mPlantHeadOriginalAnimRate = 38.0f;

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("anim_head1");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);

        float aOriginX = mPosX + aTransform.mTransX - 9.0f;
        float aOriginY = mPosY + aTransform.mTransY + 6.0f - mAltitude;
//#ifdef DO_FIX_BUGS
        if (mMindControlled)  // 魅惑修复
        {
            aOriginX += 90.0f * mScaleZombie;
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mDamageRangeFlags = 1;
        }
        else
        {
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mBackwards = true;
            aProjectile->mEnemy = true;
        }
/*#else
        Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_ZOMBIE_PEA);
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
#endif*/
    }
    else if (mPhaseCounter == 0)
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 15.0f);
        mPlantHeadOriginalAnimRate = 15.0f;
        mPhaseCounter = 150;
    }
}

//0x527AA0
void Zombie::UpdateZombieSquashHead()
{
    if (mHasHead && mIsEating && mZombiePhase == ZombiePhase::PHASE_SQUASH_PRE_LAUNCH)
    {
        StopEating();
        PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, 12.0f);
        mHasHead = false;

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->PlayReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        aHeadReanim->mRenderOrder = mRenderOrder + 1;
        aHeadReanim->SetPosition(mPosX + 6.0f, mPosY - 21.0f);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
        AttachmentDetach(aTrackInstance->mAttachmentID);
        aHeadReanim->OverrideScale(0.75f, 0.75f);
        aHeadReanim->mOverlayMatrix.m10 = 0.0f;

        mZombiePhase = ZombiePhase::PHASE_SQUASH_RISING;
        mPhaseCounter = 95;
    }

    if (mZombiePhase == ZombiePhase::PHASE_SQUASH_RISING)
    {
        int aDestX = mBoard->GridToPixelX(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow);
//#ifdef DO_FIX_BUGS
        if (mMindControlled)
        {
            Zombie* aZombie = FindZombieTarget();
            if (aZombie)
            {
                aDestX = aZombie->ZombieTargetLeadX(0.0f);
            }
            else
            {
                aDestX += 90.0f * mScaleZombie;
            }
        }
//#endif
        int aPosX = TodAnimateCurve(50, 20, mPhaseCounter, 0, aDestX - mPosX, TodCurves::CURVE_EASE_IN_OUT);
        int aPosY = TodAnimateCurve(50, 20, mPhaseCounter, 0, -20, TodCurves::CURVE_EASE_IN_OUT);

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->SetPosition(mPosX + aPosX + 6.0f, mPosY + aPosY - 21.0f);

        if (mPhaseCounter == 0)
        {
            aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
            aHeadReanim->PlayReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
            mZombiePhase = ZombiePhase::PHASE_SQUASH_FALLING;
            mPhaseCounter = 10;
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_SQUASH_FALLING)
    {
        int aPosY = TodAnimateCurve(10, 0, mPhaseCounter, -20, 74, TodCurves::CURVE_LINEAR);
        int aDestX = mBoard->GridToPixelX(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow);
//#ifdef DO_FIX_BUGS
        if (mMindControlled)
        {
            Zombie* aZombie = FindZombieTarget();
            if (aZombie)
            {
                aDestX = aZombie->ZombieTargetLeadX(0.0f);
            }
            else
            {
                aDestX += 90.0f * mScaleZombie;
            }
        }
//#endif

        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->SetPosition(mPosX + 6.0f + aDestX - mPosX, mPosY - 21.0f + aPosY);
        
        if (mPhaseCounter == 2)
        {
//#ifdef DO_FIX_BUGS
            if (mMindControlled)  // 魅惑修复
            {
                Rect aAttackRect(aDestX - 73, mPosY + 4, 65, 90);  // 具体数值未实测，待定

                Zombie* aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie))
                {
                    if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(13U))
                    {
                        Rect aZombieRect = aZombie->GetZombieRect();
                        if (GetRectOverlap(aAttackRect, aZombieRect) > (aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL || aZombie->mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL ? -20 : 0))
                        {
                            aZombie->TakeDamage(1800, 18U);
                        }
                    }
                }
            }
            else
            {
                SquishAllInSquare(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow, ZombieAttackType::ATTACKTYPE_CHEW);
            }
/*#else
            SquishAllInSquare(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow, ZombieAttackType::ATTACKTYPE_CHEW);
#endif*/
        }

        if (mPhaseCounter == 0)
        {
            mZombiePhase = ZombiePhase::PHASE_SQUASH_DONE_FALLING;
            mPhaseCounter = 100;

            mBoard->ShakeBoard(1, 4);
            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_SQUASH_DONE_FALLING && mPhaseCounter == 0)
    {
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->ReanimationDie();
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;

        TakeDamage(1800, 9U);
    }
}

//0x527F20
void Zombie::BobsledCrash()
{
    mAltitude = 0.0f;
    mZombieRect = Rect(36, 0, 42, 115);
    mZombiePhase = ZombiePhase::PHASE_BOBSLED_CRASHING;
    mPhaseCounter = BOBSLED_CRASH_TIME;
    StartWalkAnim(0);

    Reanimation* aLeaderReanim = mApp->ReanimationGet(mBodyReanimID);
    for (int i = 0; i < NUM_BOBSLED_FOLLOWERS; i++)
    {
        Zombie* aFollowerZombie = mBoard->ZombieGet(mFollowerZombieID[i]);
        aFollowerZombie->mZombiePhase = ZombiePhase::PHASE_BOBSLED_CRASHING;
        aFollowerZombie->mPhaseCounter = BOBSLED_CRASH_TIME;
        aFollowerZombie->mPosY = GetPosYBasedOnRow(mRow);
        aFollowerZombie->mAltitude = 0.0f;
        aFollowerZombie->StartWalkAnim(0);

        Reanimation* aFollowerReanim = mApp->ReanimationGet(aFollowerZombie->mBodyReanimID);
        if (aFollowerReanim)
        {
            aFollowerZombie->mVelX = mVelX;
            aFollowerReanim->mAnimTime = RandRangeFloat(0.0f, 1.0f);
            aFollowerReanim->mAnimRate = aLeaderReanim->mAnimRate;
        }
    }
}

//0x528050
void Zombie::UpdateZombieBobsled()
{
    if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING)
    {
        if (mPhaseCounter == 0)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            if (GetBobsledPosition() == 0)
            {
                for (int i = 0; i < NUM_BOBSLED_FOLLOWERS; i++)
                {
                    Zombie* aZombie = mBoard->ZombieGet(mFollowerZombieID[i]);
                    aZombie->mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
                    mFollowerZombieID[i] = ZombieID::ZOMBIEID_NULL;
                    aZombie->PickRandomSpeed();
                }
                PickRandomSpeed();
            }
        }
        return;
    }

    if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_SLIDING)
    {
        if (mPhaseCounter == 0)
        {
            mZombiePhase = ZombiePhase::PHASE_BOBSLED_BOARDING;
            PlayZombieReanim("anim_jump", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 20.0f);
        }
    }
    else
    {
        if (mZombiePhase != ZombiePhase::PHASE_BOBSLED_BOARDING)
            return;

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aCounter = aBodyReanim->mAnimTime * 50.0f;
        int aPosition = GetBobsledPosition();
        if (aPosition == 1 || aPosition == 3)
        {
            mAltitude = TodAnimateCurveFloat(0, 50, aCounter, 8.0f, 18.0f, TodCurves::CURVE_LINEAR);
        }
        else
        {
            mAltitude = TodAnimateCurveFloat(0, 50, aCounter, -9.0f, 18.0f, TodCurves::CURVE_LINEAR);
        }
    }

    mBoard->mIceTimer[mRow] = max(500, mBoard->mIceTimer[mRow]);
    if (mPosX + 10.0f < mBoard->mIceMinX[mRow] && GetBobsledPosition() == 0)
    {
        TakeDamage(6, 8U);
    }
}

//0x528240
void Zombie::DiggerLoseAxe()
{
    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
    {
        mZombiePhase = ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE;
        mPhaseCounter = 200;
        SetAnimRate(0.0f);
        UpdateAnimSpeed();
        AttachmentDetachCrossFadeParticleType(mAttachmentID, ParticleEffect::PARTICLE_DIGGER_TUNNEL, nullptr);
        StopZombieSound();
    }

    mHasObject = false;
    ReanimShowTrack("Zombie_digger_pickaxe", RENDER_GROUP_HIDDEN);
    ReanimShowTrack("Zombie_digger_dirt", RENDER_GROUP_HIDDEN);
}

//0x528310
void Zombie::UpdateZombieDigger()
{
    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
    {
        float targetLocX = 10.0f;
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BORDER_TREATY)    targetLocX = 268.0f;

        if (mPosX < targetLocX)
        {
            mAltitude = -120.0f;
            mZombiePhase = ZombiePhase::PHASE_DIGGER_RISING;
            mPhaseCounter = 130;
            PlayZombieReanim("anim_drill", ReanimLoopType::REANIM_LOOP, 0, 20.0f);

            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
            AttachmentDetachCrossFadeParticleType(mAttachmentID, ParticleEffect::PARTICLE_DIGGER_TUNNEL, nullptr);
            StopZombieSound();

            mApp->AddTodParticle(mPosX + 60.0f, mPosY + 118.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_DIGGER_RISE);
            Reanimation* aDirtReanim = mApp->AddReanimation(mPosX + 13.0f, mPosY + 97.0f, mRenderOrder + 1, ReanimationType::REANIM_DIGGER_DIRT);
            aDirtReanim->mAnimRate = 24.0f;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING)
    {
        if (mPhaseCounter > 40)
        {
            mAltitude = TodAnimateCurve(130, 40, mPhaseCounter, -120, 20, TodCurves::CURVE_EASE_OUT);
        }
        else
        {
            mAltitude = TodAnimateCurve(30, 0, mPhaseCounter, 20, 0, TodCurves::CURVE_EASE_IN);
        }
        
        if (mPhaseCounter == 30)
        {
            PlayZombieReanim("anim_landing", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12.0f);
        }
        
        if (mPhaseCounter == 0)
        {
            mAltitude = 0.0f;
            mZombiePhase = ZombiePhase::PHASE_DIGGER_STUNNED;
            PlayZombieReanim("anim_dizzy", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE)
    {
        if (mPhaseCounter == 150)
        {
            AddAttachedReanim(23, 93, ReanimationType::REANIM_ZOMBIE_SURPRISE);
        }

        if (mPhaseCounter == 0)
        {
            mAltitude = -120.f;
            mZombiePhase = ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE;
            mPhaseCounter = 130;
            PlayZombieReanim("anim_landing", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 0.0f);

            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
            mApp->AddTodParticle(mPosX + 60.0f, mPosY + 118.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_DIGGER_RISE);
            Reanimation* aDirtReanim = mApp->AddReanimation(mPosX + 13.0f, mPosY + 97.0f, mRenderOrder + 1, ReanimationType::REANIM_DIGGER_DIRT);
            aDirtReanim->mAnimRate = 24.0f;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE)
    {
        if (mPhaseCounter > 40)
        {
            mAltitude = TodAnimateCurve(130, 40, mPhaseCounter, -120, 20, TodCurves::CURVE_EASE_OUT);
        }
        else
        {
            mAltitude = TodAnimateCurve(30, 0, mPhaseCounter, 20, 0, TodCurves::CURVE_EASE_IN);
        }
        
        if (mPhaseCounter == 30)
        {
            PlayZombieReanim("anim_landing", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        }
        
        if (mPhaseCounter == 0)
        {
            mAltitude = 0.0f;
            mZombiePhase = ZombiePhase::PHASE_DIGGER_WALKING_WITHOUT_AXE;
            StartWalkAnim(20);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 1)
        {
            mZombiePhase = ZombiePhase::PHASE_DIGGER_WALKING;
            StartWalkAnim(20);
        }
    }
}

//0x528760
ZombieID Zombie::SummonBackupDancer(int theRow, int thePosX)
{
    if (!mBoard->RowCanHaveZombieType(theRow, ZombieType::ZOMBIE_BACKUP_DANCER))
        return ZombieID::ZOMBIEID_NULL;

    Zombie* aZombie = mBoard->AddZombie(mZombieType == ZOMBIE_DISCO ? ZombieType::ZOMBIE_DISCO_BACKUP : ZombieType::ZOMBIE_BACKUP_DANCER, mFromWave);
    if (aZombie == nullptr)
        return ZombieID::ZOMBIEID_NULL;

    aZombie->mPosX = thePosX;
    aZombie->mPosY = GetPosYBasedOnRow(theRow);
    aZombie->SetRow(theRow);
    aZombie->mX = (int)aZombie->mPosX;
    aZombie->mY = (int)aZombie->mPosY;
    aZombie->mAltitude = ZOMBIE_BACKUP_DANCER_RISE_HEIGHT;
    aZombie->mZombiePhase = ZombiePhase::PHASE_DANCER_RISING;
    aZombie->mPhaseCounter = 150;
    aZombie->mRelatedZombieID = mBoard->ZombieGetID(this);

    aZombie->SetAnimRate(0.0f);
    aZombie->mMindControlled = mMindControlled;

    
    //if (!mBoard->StageHasRoof())
    {
        int aParticleX = (int)aZombie->mPosX + 60;
        int aParticleY = (int)aZombie->mPosY + 110;
        if (aZombie->IsOnHighGround())
        {
            aParticleY -= HIGH_GROUND_HEIGHT;
        }

        int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, theRow, 0);
        mApp->AddTodParticle(aParticleX, aParticleY, aRenderOrder, ParticleEffect::PARTICLE_DANCER_RISE);
        mApp->PlayFoley(FoleyType::FOLEY_GRAVESTONE_RUMBLE);
    }
    //else {
        //mBoard->BungeeDropZombieTo(aZombie->mPosX, aZombie->mPosY, aZombie, theRow);
   // }

    return mBoard->ZombieGetID(aZombie);
}

//0x528970
void Zombie::SummonBackupDancers()
{
    if (!mHasHead)
        return;

    for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
    {
        if (mBoard->ZombieTryToGet(mFollowerZombieID[i]) == nullptr)
        {
            int aRow, aPosX;
            switch (i)
            {
            case 0:     aRow = mRow - 1;    aPosX = mPosX;          break;
            case 1:     aRow = mRow + 1;    aPosX = mPosX;          break;
            case 2:     aRow = mRow;        aPosX = mPosX - 100;    break;
            case 3:     aRow = mRow;        aPosX = mPosX + 100;    break;
            default:    /*Nothing*/                                 break;
            }

            mFollowerZombieID[i] = SummonBackupDancer(aRow, aPosX);
        }
    }
}

//0x528A50
bool Zombie::NeedsMoreBackupDancers()
{
    for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
    {
        if (mBoard->ZombieTryToGet(mFollowerZombieID[i]) == nullptr)
        {
            if (i == 0 && !mBoard->RowCanHaveZombieType(mRow - 1, ZombieType::ZOMBIE_BACKUP_DANCER))
            {
                continue;
            }

            if (i == 1 && !mBoard->RowCanHaveZombieType(mRow + 1, ZombieType::ZOMBIE_BACKUP_DANCER))
            {
                continue;
            }

            return true;
        }
    }

    return false;
}

//0x528B00
void Zombie::PlayZombieReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    aBodyReanim->PlayReanim(theTrackName, theLoopType, theBlendTime, theAnimRate);
    if (theAnimRate != 0.0f)
    {
        mOriginalAnimRate = theAnimRate;
    }
    UpdateAnimSpeed();
}

//0x528B80
void Zombie::UpdateZombieBackupDancer()
{
    if (mIsEating)
        return;

    if (mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
    {
        mAltitude = TodAnimateCurve(150, 0, mPhaseCounter, ZOMBIE_BACKUP_DANCER_RISE_HEIGHT, 0, TodCurves::CURVE_LINEAR);

        if (mPhaseCounter != 0)
            return;

        if (IsOnHighGround())
        {
            mAltitude = HIGH_GROUND_HEIGHT;
        }
    }

    ZombiePhase aDancerPhase = GetDancerPhase();
    if (aDancerPhase != mZombiePhase)
    {
        switch (aDancerPhase)
        {
        case ZombiePhase::PHASE_DANCER_DANCING_LEFT:
            mZombiePhase = aDancerPhase;
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
            break;

        case ZombiePhase::PHASE_DANCER_WALK_TO_RAISE:
            mZombiePhase = aDancerPhase;
            PlayZombieReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 10, 18.0f);
            mApp->ReanimationTryToGet(mBodyReanimID)->mAnimTime = 0.6f;
            break;

        case ZombiePhase::PHASE_DANCER_RAISE_LEFT_1:
        case ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1:
        case ZombiePhase::PHASE_DANCER_RAISE_LEFT_2:
        case ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2:
            mZombiePhase = aDancerPhase;
            PlayZombieReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 10, 18.0f);
            break;
        }
    }
}

//0x528CA0
void Zombie::UpdateZombieDancer()
{
    if (mIsEating)
        return;

    if (mSummonCounter > 0)
    {
        mSummonCounter--;
        if (mSummonCounter == 0)
        {
            if (GetDancerFrame() == 12 && mHasHead && mPosX < 700.0f)
            {
                mZombiePhase = ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_WITH_LIGHT;
                PlayZombieReanim("anim_point", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            }
            else
            {
                mSummonCounter = 1;
            }
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN)
    {
        if (mHasHead && mPhaseCounter == 0)
        {
            mZombiePhase = ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS;
            PlayZombieReanim("anim_point", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            PickRandomSpeed();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS || mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_WITH_LIGHT)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            if (mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS && mBoard->CountZombiesOnScreen() <= 15)
            {
                mApp->PlayFoley(FoleyType::FOLEY_DANCER);
            }

            SummonBackupDancers();
            mZombiePhase = ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_HOLD;
            mPhaseCounter = 200;
        }
    }
    else
    {
        if (mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_HOLD)
        {
            if (mPhaseCounter != 0)
                return;

            mZombiePhase = ZombiePhase::PHASE_DANCER_DANCING_LEFT;
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
        }

        ZombiePhase aDancerPhase = GetDancerPhase();
        if (aDancerPhase != mZombiePhase)
        {
            switch (aDancerPhase)
            {
            case ZombiePhase::PHASE_DANCER_DANCING_LEFT:
                mZombiePhase = aDancerPhase;
                PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
                break;

            case ZombiePhase::PHASE_DANCER_WALK_TO_RAISE:
                mZombiePhase = aDancerPhase;
                PlayZombieReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 10, 18.0f);
                mApp->ReanimationTryToGet(mBodyReanimID)->mAnimTime = 0.6f;
                break;

            case ZombiePhase::PHASE_DANCER_RAISE_LEFT_1:
            case ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1:
            case ZombiePhase::PHASE_DANCER_RAISE_LEFT_2:
            case ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2:
                mZombiePhase = aDancerPhase;
                PlayZombieReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 10, 18.0f);
                break;
            }
        }

        if (mHasHead && mSummonCounter == 0 && NeedsMoreBackupDancers())
        {
            mSummonCounter = 100;
        }
    }
}

//0x528EC0
void Zombie::UpdateZombieRiseFromGrave()
{
    if (mInPool)
    {
        mAltitude = TodAnimateCurve(50, 0, mPhaseCounter, -150, -40, TodCurves::CURVE_LINEAR) * mScaleZombie;
    }
    else
    {
        mAltitude = TodAnimateCurve(50, 0, mPhaseCounter, -200, 0, TodCurves::CURVE_LINEAR);
    }

    if (mPhaseCounter == 0)
    {
        mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;

        if (IsOnHighGround())
        {
            mAltitude = HIGH_GROUND_HEIGHT;
        }

        if (mInPool)
        {
            ReanimIgnoreClipRect("Zombie_duckytube", true);
            ReanimIgnoreClipRect("Zombie_whitewater", true);
            ReanimIgnoreClipRect("Zombie_outerarm_hand", true);
            ReanimIgnoreClipRect("Zombie_innerarm3", true);
            ReanimIgnoreClipRect("anim_screendoor", true);
        }
    }
}

void Zombie::DragUnder()
{
    mZombieHeight = ZombieHeight::HEIGHT_DRAGGED_UNDER;
    StopEating();
    ReanimReenableClipping();
}

//0x528F70
bool Zombie::ZombiquariumFindClosestBrain()
{
    if (mBoard->HasLevelAwardDropped() || mBodyHealth > 150)
        return false;

    GridItem* aBrainClosest = nullptr;
    float aDistanceClosest = 0.0f;
    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_BRAIN && aGridItem->mGridItemCounter >= 15)
        {
            float aDistance = Distance2D(aGridItem->mPosX + 15.0f, aGridItem->mPosY + 15.0f, mPosX + 50.0f, mPosY + 40.0f);
            if (aBrainClosest == nullptr || aDistance < aDistanceClosest)
            {
                aDistanceClosest = aDistance;
                aBrainClosest = aGridItem;
            }
        }
    }

    if (aBrainClosest)
    {
        if (aDistanceClosest < 50.0f)
        {
            aBrainClosest->GridItemDie();
            mApp->PlayFoley(FoleyType::FOLEY_SLURP);

            mBodyHealth += 200;
            if (mBodyHealth > mBodyMaxHealth)
            {
                mBodyHealth = mBodyMaxHealth;
            }

            PlayZombieReanim("anim_aquarium_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_BITE;
            mPhaseCounter = 200;
            return false;
        }

        float aRangeY = aBrainClosest->mPosY + 15.0f - (mPosY + 40.0f);
        float aRangeX = aBrainClosest->mPosX + 15.0f - (mPosX + 50.0f);
        mVelZ = atan2(aRangeY, aRangeX);
        if (mVelZ < 0.0f)
        {
            mVelZ += (int)(PI * 2);
        }

        mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_ACCEL;
        return true;
    }

    return false;
}

//0x5291E0
void Zombie::UpdateZombiquarium()
{
    if (IsDeadOrDying())
        return;

    float& num2 = mVelX;
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIQUARIUM_BITE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            float aAnimRate = RandRangeFloat(8.0f, 10.0f);
            PlayZombieReanim("anim_aquarium_swim", ReanimLoopType::REANIM_LOOP, 20, aAnimRate);

            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_DRIFT;
            mPhaseCounter = 100;
        }
    }
    else if (!ZombiquariumFindClosestBrain() && mPhaseCounter == 0)
    {
        int aPhaseHit = Rand(7);
        if (aPhaseHit <= 4)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_ACCEL;
            mVelZ = RandRangeFloat(0.0f, PI * 2);
            mPhaseCounter = RandRangeInt(300, 1000);
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        }
        /*else if (aPhaseHit == 4)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_DRIFT;
            mVelZ = PI * 1.5f;
            mPhaseCounter = RandRangeInt(300, 1000);
          aBodyReanim->mAnimRate = RandRangeFloat(8.0f, 10.0f);
        }*/
        else if (aPhaseHit == 5)
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_BACK_AND_FORTH;
            mVelZ = 0.0f;
            mPhaseCounter = RandRangeInt(300, 1000);
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        }
        else
        {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIQUARIUM_BACK_AND_FORTH;
            mVelZ = PI;
            mPhaseCounter = RandRangeInt(300, 1000);
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        }
    }

    float aVelX = cos(mVelZ);
    float aVelY = sin(mVelZ);
    bool aIsOutOfBounds = false;
    if (mPosX < 0.0f && aVelX < 0.0f)
    {
        aIsOutOfBounds = true;
    }
    else if (mPosX > 680.0f && aVelX > 0.0f)
    {
        aIsOutOfBounds = true;
    }
    else if (mPosY < 100.0f && aVelY < 0.0f)
    {
        aIsOutOfBounds = true;
    }
    else if (mPosY > 400.0f && aVelY > 0.0f)
    {
        aIsOutOfBounds = true;
    }

    float aMaxSpeed = 0.5f;
    if (aIsOutOfBounds)
    {
        aMaxSpeed = mVelX * 0.3f;
        mPhaseCounter = min(100, mPhaseCounter);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIQUARIUM_ACCEL)
    {
        aMaxSpeed = 0.5f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIQUARIUM_BACK_AND_FORTH)
    {
        if (mPosX < 200.0f && aVelX < 0.0f)
        {
            mVelZ = 0.0f;
        }

        if (mPosX > 550.0f || aVelX > 0.0f)
        {
            mVelZ = PI;
        }

        aMaxSpeed = 0.3f;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIQUARIUM_DRIFT || mZombiePhase == ZombiePhase::PHASE_ZOMBIQUARIUM_BITE)
    {
        aMaxSpeed = 0.05f;
    }

    mVelX = min(aMaxSpeed, mVelX + 0.01f);
    aVelX *= mVelX;
    aVelY *= mVelX;
    mPosX += aVelX;
    mPosY += aVelY;

    if (!mBoard->HasLevelAwardDropped())
    {
        if (mSummonCounter > 0)
        {
            mSummonCounter--;
            if (mSummonCounter == 0)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(mX + 50, mY + 40, (mBoard->mIsVeteran ? CoinType::COIN_SMALLSUN : CoinType::COIN_SUN), CoinMotion::COIN_MOTION_FROM_PLANT);
                mSummonCounter = RandRangeInt(1000, 1500);
            }
        }

        if (mZombieAge % 100 == 0)
        {
            TakeDamage(10, 8U);
            if (IsDeadOrDying())
            {
                mApp->PlaySample(SOUND_ZOMBAQUARIUM_DIE);
            }
        }
    }
}

//0x5295F0
void Zombie::UpdateZombiePool()
{
    if (mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL)
    {
        mAltitude++;
        if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
        {
            mAltitude++;
        }

        if (mAltitude >= 0.0f)
        {
            mAltitude = 0.0f;
            mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
            mInPool = false;
        }
    }
    else if (mZombieHeight == ZombieHeight::HEIGHT_IN_TO_POOL)
    {
        mAltitude--;
        int aDepth = -40 * mScaleZombie;
        if (mAltitude <= aDepth)
        {
            mAltitude = aDepth;
            mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
            StartWalkAnim(0);
        }
    }
    else if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER)
    {
        mAltitude--;
    }
}

//0x5296E0
void Zombie::UpdateZombieHighGround()
{
    if (mZombieType == ZombieType::ZOMBIE_POGO)
        return;

    if (mZombieHeight == ZombieHeight::HEIGHT_UP_TO_HIGH_GROUND)
    {
        float offsetY = 0;
        if (IsFlying())
            offsetY = 25;

        mAltitude++;
        if (mAltitude >= HIGH_GROUND_HEIGHT + offsetY)
        {
            mAltitude = HIGH_GROUND_HEIGHT + offsetY;
            mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
        }
    }
    else if (mZombieHeight == ZombieHeight::HEIGHT_DOWN_OFF_HIGH_GROUND)
    {
        mAltitude--;
        if (mAltitude <= 0.0f)
        {
            mAltitude = 0.0f;
            mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
            mOnHighGround = false;
        }
    }
}

//0x529770
void Zombie::UpdateZombieFalling()
{
    mAltitude--;
    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
    {
        mAltitude--;
    }

    int aGroundHeight = 0;
    if (IsOnHighGround())
    {
        aGroundHeight = HIGH_GROUND_HEIGHT;
    }
    if (mAltitude <= aGroundHeight)
    {
        mAltitude = aGroundHeight;
        mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;
    }
}

//0x5297F0
void Zombie::OverrideParticleScale(TodParticleSystem* aParticle)
{
    if (aParticle)
    {
        aParticle->OverrideScale(nullptr, mScaleZombie);
    }
}

//0x529810
void Zombie::OverrideParticleColor(TodParticleSystem* aParticle)
{
    if (aParticle)
    {
        if (mMindControlled)
        {
            aParticle->OverrideColor(nullptr, ZOMBIE_MINDCONTROLLED_COLOR);
            aParticle->OverrideExtraAdditiveDraw(nullptr, true);
        }
        else if (mChilledCounter > 0 || mIceTrapCounter > 0)
        {
            aParticle->OverrideColor(nullptr, Color(75, 75, 255, 255));
            aParticle->OverrideExtraAdditiveDraw(nullptr, true);
        }
    }
}

void Zombie::UpdateBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        Reanimation* aBlinkReanim = mApp->ReanimationTryToGet(mBlinkReanimID);
        if (aBlinkReanim == nullptr || aBlinkReanim->mLoopCount > 0)
        {
            EndBlink();
        }
    }

    if (mBlinkCountdown > 0)
    {
        mBlinkCountdown--;
        if (mBlinkCountdown == 0)
        {
            DoBlink();
        }
    }
}

void Zombie::DoBlink()
{
    mBlinkCountdown = 400 + Rand(400);

    if (IsOnBoard() && (
        mZombieType == ZOMBIE_PEA_HEAD && mPhaseCounter <= 35 ||
        mZombieType == ZOMBIE_PEA_HEAD && mPhaseCounter <= 35 ||
        mZombieType == ZOMBIE_GATLING_HEAD && mPhaseCounter <= 100
        ))
        return;

    EndBlink();


    Reanimation* aTargetReanim = mApp->ReanimationTryToGet(mPlantHeadID);
    if (aTargetReanim == nullptr)
        return;

    if ((mZombieType == ZOMBIE_TALLNUT_HEAD && aTargetReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_TALLNUT_CRACKED2) /* ||
        (mSeedType == SeedType::SEED_GARLIC && aTargetReanim->GetImageOverride("anim_face") == IMAGE_REANIM_GARLIC_BODY3)*/)
        return;


    if (mZombieType == ZOMBIE_WALLNUT_HEAD || mZombieType == ZOMBIE_EXPLODE_O_NUT_HEAD || mZombieType == ZOMBIE_TALLNUT_HEAD)
    {
        mBlinkCountdown = 1000 + Rand(1000);
    }

    Reanimation* aBlinkReanim = AttachBlinkAnim(aTargetReanim);
    if (aBlinkReanim)
    {
        mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
    }
    aTargetReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
}

void Zombie::EndBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        mApp->RemoveReanimation(mBlinkReanimID);
        mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

        Reanimation* aTargetReanim = mApp->ReanimationTryToGet(mPlantHeadID);
        if (aTargetReanim)
        {
            aTargetReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_NORMAL);
        }
    }
}

Reanimation* Zombie::AttachBlinkAnim(Reanimation* theReanimBody) {
    SeedType mSeedType = mPlantHeadType;
    ReanimationType targetReanim = GetPlantDefinition(mSeedType).mReanimationType;
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
    /*else if (mSeedType == SeedType::SEED_THREEPEATER)
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
    }*/

    if (aAnimToAttach == nullptr)
    {
        TodTrace("Missing head anim");
        return nullptr;
    }

    if (!theReanimBody->TrackExists(aTrackToPlay))
        return nullptr;

    Reanimation* aBlinkReanim = aApp->mEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, targetReanim);
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

//0x529870
void Zombie::DropFlag()
{
    if (mZombieType != ZombieType::ZOMBIE_FLAG || !mHasObject)
        return;

    mApp->RemoveReanimation(mFlagReanimID);
    ReanimShowPrefix("anim_innerarm", RENDER_GROUP_NORMAL);
    ReanimShowTrack("Zombie_flaghand", RENDER_GROUP_HIDDEN);
    ReanimShowTrack("Zombie_innerarm_screendoor", RENDER_GROUP_HIDDEN);
    mHasObject = false;

    float aFlagPosX, aFlagPosY;
    GetTrackPosition("Zombie_flaghand", aFlagPosX, aFlagPosY);
    TodParticleSystem* aParticle = mApp->AddTodParticle(aFlagPosX + 6.0f, aFlagPosY - 45.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_FLAG);
    OverrideParticleColor(aParticle);
    OverrideParticleScale(aParticle);
}

//0x529970
void Zombie::DropPole()
{
    if (mZombieType != ZombieType::ZOMBIE_POLEVAULTER)
        return;

    ReanimShowPrefix("Zombie_polevaulter_innerarm", RENDER_GROUP_HIDDEN);
    ReanimShowPrefix("Zombie_polevaulter_innerhand", RENDER_GROUP_HIDDEN);
    ReanimShowPrefix("Zombie_polevaulter_pole", RENDER_GROUP_HIDDEN);
}

bool Zombie::CanLoseBodyParts()
{
    return 
        mZombieType != ZombieType::ZOMBIE_WRAITH &&
        mZombieType != ZombieType::ZOMBIE_ZAMBONI && 
        mZombieType != ZombieType::ZOMBIE_BUNGEE && 
        mZombieType != ZombieType::ZOMBIE_CATAPULT && 
        (mZombieType != ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_GARGANTUAR && mIsInstantDeath) &&
        mZombieType != ZombieType::ZOMBIE_REDEYE_GARGANTUAR && 
        mZombieType != ZombieType::ZOMBIE_BOSS && 
        mZombieHeight != ZombieHeight::HEIGHT_ZOMBIQUARIUM && 
        !IsFlying() && 
        !IsBobsledTeamWithSled();
}

void Zombie::SetupReanimForLostHead()
{
    ReanimShowPrefix("anim_head", RENDER_GROUP_HIDDEN);
    ReanimShowPrefix("anim_hair", RENDER_GROUP_HIDDEN);
    ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);

    //FUNCUST
    ReanimShowPrefix("Zombie_gargantua_jaw", RENDER_GROUP_HIDDEN);
}

//0x529A30
void Zombie::DropHead(unsigned int theDamageFlags)
{
    if (!CanLoseBodyParts() || !mHasHead)
        return;

    if (mButteredCounter > 0)
    {
        mButteredCounter = 0;
        UpdateAnimSpeed();
    }

    mHasHead = false;
    SetupReanimForLostHead();
    if (TestBit(theDamageFlags, DamageFlags::DAMAGE_DOESNT_LEAVE_BODY) && !mIsInstantDeath)
    {
        return;
    }

    if (Zombie::IsZombotany(mZombieType))
    {
        Reanimation* mPlantHeadReanim = mApp->ReanimationTryToGet(mPlantHeadID);
        if (mPlantHeadReanim)
            mPlantHeadReanim->ReanimationDie();
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;
        return;
    }

    int aRenderOrder = mRenderOrder + 1;
    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    float aPosX = mPosX + aDrawPos.mImageOffsetX + aDrawPos.mHeadX + 11.0f;
    float aPosY = mPosY + aDrawPos.mImageOffsetY + aDrawPos.mHeadY + aDrawPos.mBodyY + 21.0f;
    if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
    {       
        GetTrackPosition("anim_head1", aPosX, aPosY);
    }

    ParticleEffect aEffect = ParticleEffect::PARTICLE_ZOMBIE_HEAD;
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED){
        aEffect = ParticleEffect::PARTICLE_MOWERED_ZOMBIE_HEAD;
    }
    else if (mInPool){
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_HEAD_POOL;
    }
    if (mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO){
        aRenderOrder = mRenderOrder - 1;
    }
    if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER){
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_NEWSPAPER_HEAD;
    }
    else if (mZombieType == ZombieType::ZOMBIE_POGO){
        PogoBreak(theDamageFlags);
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_POGO_HEAD;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BALLOON){
        ReanimShowPrefix("anim_hat", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("hat", RENDER_GROUP_HIDDEN);
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_BALLOON_HEAD;
    }
    else if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER){
        DropPole();
    }
    else if (mZombieType == ZombieType::ZOMBIE_FLAG)
    {
        DropFlag();
    }
    else if (mZombieType == ZombieType::ZOMBIE_DISCO) {
        ReanimShowPrefix("Zombie_disco_chops", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_disco_glasses", RENDER_GROUP_HIDDEN);
    }
    else if (mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP) {
        ReanimShowPrefix("Zombie_disco_chops", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_backup_stash", RENDER_GROUP_HIDDEN);
    }

    TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, aEffect);
    OverrideParticleColor(aParticle);
    OverrideParticleScale(aParticle);
   
    if (aParticle)
    {
        if (mZombieType == ZombieType::ZOMBIE_DANCER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEDANCERHEAD);
        }else if (mZombieType == ZombieType::ZOMBIE_DISCO)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEDISCOHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER)
        {
            ReanimShowPrefix("anim_earing", RENDER_GROUP_HIDDEN);
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEBACKUPDANCERHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP) {
        }
        else if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEBOBSLEDHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_LADDER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIELADDERHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_IMP)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEIMPHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEFOOTBALLHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEPOLEVAULTERHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_SNORKLE_HEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_DIGGER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEDIGGERHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEDOLPHINRIDERHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_YETI)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEYETIHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEGARGANTUARHEAD);
        }
        else if (mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEREDEYEGARGANTUARHEAD);
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mBoard->mMustacheMode && aBodyReanim->TrackExists("Zombie_mustache"))
    {
        ReanimShowPrefix("Zombie_mustache", RENDER_GROUP_HIDDEN);

        TodParticleSystem* aMustacheParticle = mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, ParticleEffect::PARTICLE_ZOMBIE_MUSTACHE);
        OverrideParticleColor(aMustacheParticle);
        OverrideParticleScale(aMustacheParticle);

        Image* aMustacheImage = aBodyReanim->GetImageOverride("Zombie_mustache");
        if (aMustacheParticle && aMustacheImage)
        {
            aMustacheParticle->OverrideImage(nullptr, aMustacheImage);
        }
    }
    if (mBoard->mFutureMode)
    {
        Image* aHeadImage = aBodyReanim->GetImageOverride("anim_head1");
        int aFrame = -1;
        if (aHeadImage)
        {
            if (aHeadImage == IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1)
            {
                aFrame = 0;
            }
            else if (aHeadImage == IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2)
            {
                aFrame = 1;
            }
            else if (aHeadImage == IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3)
            {
                aFrame = 2;
            }
            else if (aHeadImage == IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4)
            {
                aFrame = 3;
            }
        }

        if (aFrame != -1)
        {
            TodParticleSystem* aSunglassParticle = mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, ParticleEffect::PARTICLE_ZOMBIE_SUNGLASS);
            OverrideParticleColor(aSunglassParticle);
            OverrideParticleScale(aSunglassParticle);
            if (aSunglassParticle)
            {
                aSunglassParticle->OverrideFrame(nullptr, aFrame);
            }
        }
    }
    if (mBoard->mPinataMode && mZombiePhase != ZombiePhase::PHASE_ZOMBIE_MOWERED)
    {
        TodParticleSystem* aPinataParticle = mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, ParticleEffect::PARTICLE_ZOMBIE_PINATA);
        OverrideParticleScale(aParticle);
    }

    mApp->PlayFoley(FoleyType::FOLEY_LIMBS_POP);
}

void Zombie::SetupReanimForLostArm(unsigned int theDamageFlags)
{
    switch (mZombieType)
    {
    case ZombieType::ZOMBIE_BLACK_FOOTBALL:
    case ZombieType::ZOMBIE_FOOTBALL:
        ReanimShowPrefix("Zombie_football_leftarm_lower", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_football_leftarm_hand", RENDER_GROUP_HIDDEN);
        break;
    case ZombieType::ZOMBIE_NEWSPAPER:
        ReanimShowTrack("Zombie_paper_hands", RENDER_GROUP_HIDDEN);
        ReanimShowTrack("Zombie_paper_leftarm_lower", RENDER_GROUP_HIDDEN);
        break;
    case ZombieType::ZOMBIE_POLEVAULTER:
        ReanimShowTrack("Zombie_polevaulter_outerarm_lower", RENDER_GROUP_HIDDEN);
        ReanimShowTrack("Zombie_outerarm_hand", RENDER_GROUP_HIDDEN);
        break;
    default:
        ReanimShowPrefix("Zombie_outerarm_lower", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("Zombie_outerarm_hand", RENDER_GROUP_HIDDEN);
        break;
    }

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    float aPosX = mPosX + aDrawPos.mImageOffsetX + 45.0f;
    float aPosY = mPosY + aDrawPos.mImageOffsetY + aDrawPos.mBodyY + 78.0f;
    if (IsWalkingBackwards())
    {
        aPosX += 36.0f;
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        switch (mZombieType)
        {
        case ZombieType::ZOMBIE_FOOTBALL:
            GetTrackPosition("Zombie_football_leftarm_hand", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_football_leftarm_upper", IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_BLACK_FOOTBALL:
            GetTrackPosition("Zombie_football_leftarm_hand", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_football_leftarm_upper", IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_LEFTARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_NEWSPAPER:
            GetTrackPosition("Zombie_paper_leftarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_paper_leftarm_upper", IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_POLEVAULTER:
            GetTrackPosition("Zombie_polevaulter_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_polevaulter_outerarm_upper", IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_BALLOON:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_outerarm_upper", IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_IMP:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_imp_outerarm_upper", IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE);
            break;
        case ZombieType::ZOMBIE_DIGGER:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_digger_outerarm_upper", IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_BOBSLED:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_dolphinrider_outerarm_upper", IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_JACK_IN_THE_BOX:
            GetTrackPosition("Zombie_jackbox_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_jackbox_outerarm_lower", IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2);
            break;
        case ZombieType::ZOMBIE_DIVER:
        case ZombieType::ZOMBIE_SNORKEL:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_snorkle_outerarm_upper", IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_DOLPHIN_RIDER:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_dolphinrider_outerarm_upper", IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_POGO:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_outerarm_upper", IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2);
            aBodyReanim->SetImageOverride("Zombie_pogo_stickhands", IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2);
            aBodyReanim->SetImageOverride("Zombie_pogo_stick", IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2);
            aBodyReanim->SetImageOverride("Zombie_pogo_stick2", IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2);
            break;
        case ZombieType::ZOMBIE_FLAG:
        {
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_outerarm_upper", IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2);
            break;
        }
        case ZombieType::ZOMBIE_DISCO:
            GetTrackPosition("Zombie_disco_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_Jackson_outerarm_upper", IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_DANCER:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_Jackson_outerarm_upper", IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_DISCO_BACKUP:
            GetTrackPosition("Zombie_disco_outerarm_lower", aPosX, aPosY);
            break;
        case ZombieType::ZOMBIE_BACKUP_DANCER:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            break;
        case ZombieType::ZOMBIE_LADDER:
            GetTrackPosition("Zombie_outerarm_hand", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_ladder_outerarm_upper", IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2);
            break;
        case ZombieType::ZOMBIE_YETI:
            GetTrackPosition("Zombie_outerarm_hand", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_yeti_outerarm_upper", IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2);
            break;
        default:
            GetTrackPosition("Zombie_outerarm_lower", aPosX, aPosY);
            aBodyReanim->SetImageOverride("Zombie_outerarm_upper", IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2);
            break;
        }
    }

    if (!mInPool && !TestBit(theDamageFlags, DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
    {
        ParticleEffect aEffect = ParticleEffect::PARTICLE_ZOMBIE_ARM;
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED)
        {
            aEffect = ParticleEffect::PARTICLE_MOWERED_ZOMBIE_ARM;
        }

        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, aEffect);
        OverrideParticleColor(aParticle);
        OverrideParticleScale(aParticle);

        if (aParticle)
        {
            switch (mZombieType)
            {
            case ZombieType::ZOMBIE_FOOTBALL:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_HAND);
                break;
            case ZombieType::ZOMBIE_BLACK_FOOTBALL:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_LEFTARM_HAND);
                break;
            case ZombieType::ZOMBIE_NEWSPAPER:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_LOWER);
                break;
            case ZombieType::ZOMBIE_DANCER:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_HAND);
                break;
            case ZombieType::ZOMBIE_BACKUP_DANCER:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_DANCER_INNERARM_HAND);
                break;
            case ZombieType::ZOMBIE_BOBSLED:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_HAND);
                break;
            case ZombieType::ZOMBIE_IMP:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_IMP_ARM2);
                break;
            case ZombieType::ZOMBIE_YETI:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_HAND);
                break;
            case ZombieType::ZOMBIE_JACK_IN_THE_BOX:
                aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEJACKBOXARM);
                break;
            case ZombieType::ZOMBIE_DIGGER:
                aParticle->OverrideImage(nullptr, IMAGE_ZOMBIEDIGGERARM);
                break;
            case ZombieType::ZOMBIE_POLEVAULTER:
            case ZombieType::ZOMBIE_BALLOON:
            case ZombieType::ZOMBIE_DOLPHIN_RIDER:
            case ZombieType::ZOMBIE_POGO:
            case ZombieType::ZOMBIE_LADDER:
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_OUTERARM_HAND);
                break;
            }
        }
    }
}

//0x529EF0
void Zombie::DropArm(unsigned int theDamageFlags)
{
    if (!CanLoseBodyParts())
    {
        return;
    }

    if (mShieldType == ShieldType::SHIELDTYPE_DOOR || mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER || mZombieType == ZOMBIE_GARGANTUAR)
    {
        return;
    }
    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_READING)
    {
        return;
    }
    if (!mHasArm)
    {
        return;
    }

    mHasArm = false;
    SetupReanimForLostArm(theDamageFlags);
    mApp->PlayFoley(FoleyType::FOLEY_LIMBS_POP);
}

//0x52A470
void Zombie::UpdateDamageStates(unsigned int theDamageFlags)
{
    if (!CanLoseBodyParts())
        return;

    if (mHasArm && mBodyHealth < 2 * mBodyMaxHealth / 3 && mBodyHealth > 0)
    {
        DropArm(theDamageFlags);
    }

    if (mZombieType == ZOMBIE_GARGANTUAR)
    {
        if (mHasHead && mBodyHealth <= 0)
        {
            DropHead(theDamageFlags);
            DropLoot();
            StopZombieSound();

            if (mBoard->HasLevelAwardDropped())
            {
                PlayDeathAnim(theDamageFlags);
            }

            if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
            {
                DieNoLoot();
            }
        }
        return;
    }

    if (mHasHead && mBodyHealth < mBodyMaxHealth / 3)
    {
        DropHead(theDamageFlags);
        DropLoot();
        StopZombieSound();

        if (mBoard->HasLevelAwardDropped())
        {
            PlayDeathAnim(theDamageFlags);
        }

        if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
        {
            DieNoLoot();
        }
    }
}

//0x52A580
float Zombie::ZombieTargetLeadX(float theTime)
{
    float aSpeed = mVelX;
    if (mChilledCounter > 0)
    {
        aSpeed *= CHILLED_SPEED_FACTOR;
    }
    if (IsWalkingBackwards())
    {
        aSpeed = -aSpeed;
    }
    if (ZombieNotWalking())
    {
        aSpeed = 0.0f;
    }

    Rect aZombieRect = GetZombieRect();
    float aCurrentPosX = aZombieRect.mX + aZombieRect.mWidth / 2;
    float aDisplacementX = aSpeed * theTime;
    return aCurrentPosX - aDisplacementX;
}

//0x52A610
bool Zombie::ZombieNotWalking()
{
    if (mIsEating || IsImmobilizied())
    {
        return true;
    }

    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || 
        mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MADDENING ||
        mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_THROWING || 
        mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_SMASHING ||
        mZombiePhase == ZombiePhase::PHASE_CATAPULT_LAUNCHING || 
        mZombiePhase == ZombiePhase::PHASE_CATAPULT_RELOADING ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_WITH_LIGHT ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_HOLD || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING ||
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || 
        mZombiePhase == ZombiePhase::PHASE_IMP_LANDING ||
        mZombiePhase == ZombiePhase::PHASE_LADDER_PLACING || 
        mZombieHeight == ZombieHeight::HEIGHT_IN_TO_CHIMNEY ||
        mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED || 
        mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM ||
        mZombieType == ZombieType::ZOMBIE_BUNGEE || 
        mZombieType == ZombieType::ZOMBIE_BOSS ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_LEFT_1 || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_WALK_TO_RAISE ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1 || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_LEFT_2 ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2 ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2)
    {
        return true;
    }

    if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        Zombie* aLeader = nullptr;
        if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER)
        {
            aLeader = this;
        }
        else
        {
            aLeader = mBoard->ZombieTryToGet(mRelatedZombieID);
        }

        if (aLeader)
        {
            if (aLeader->IsImmobilizied() || aLeader->mIsEating)
            {
                return true;
            }

            for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
            {
                Zombie* aDancer = mBoard->ZombieTryToGet(aLeader->mFollowerZombieID[i]);
                if (aDancer && (aDancer->IsImmobilizied() || aDancer->mIsEating))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//0x52A7B0
void Zombie::UpdateZamboni()
{
    if (mPosX > 400.0f && !mFlatTires)
    {
        mVelX = TodAnimateCurveFloat(700, 300, mPosX, 0.25f, 0.05f, TodCurves::CURVE_LINEAR);
    }
    else if (mFlatTires && mVelX > 0.0005f)
    {
        mVelX -= 0.0005f;
    }

    int anIceX = mPosX + 118;
    if (mBoard->StageHasRoof())
    {
        anIceX = max(anIceX, 500 );
    }
    else
    {
        anIceX = max(anIceX, 25);
    }
    if (anIceX < mBoard->mIceMinX[mRow])
    {
        mBoard->mIceMinX[mRow] = anIceX;
    }
    if (anIceX < 800)
    {
        mBoard->mIceTimer[mRow] = 3000;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA)
        {
            mBoard->mIceTimer[mRow] = INT_MAX;
        }
    }
}

//0x52A8E0
void Zombie::UpdateYeti()
{
    if (mMindControlled || !mHasHead || IsDeadOrDying())
        return;

    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_NORMAL && mPhaseCounter == 0)
    {
        mZombiePhase = ZombiePhase::PHASE_YETI_RUNNING;
        mHasObject = false;
        PickRandomSpeed();
    }
}

//0x52A930
void Zombie::UpdateLadder()
{
    if (mMindControlled || !mHasHead || IsDeadOrDying())
        return;

    if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING && mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL)
    {
        if (FindPlantTarget(ZombieAttackType::ATTACKTYPE_LADDER))
        {
            StopEating();
            mZombiePhase = ZombiePhase::PHASE_LADDER_PLACING;
            PlayZombieReanim("anim_placeladder", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_LADDER_PLACING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_LADDER);
            if (aPlant)
            {
                mBoard->AddALadder(aPlant->mPlantCol, aPlant->mRow);
                mApp->PlaySample(SOUND_LADDER_ZOMBIE);
                mZombieHeight = ZombieHeight::HEIGHT_UP_LADDER;
                mUseLadderCol = aPlant->mPlantCol;
                DetachShield();
            }
            else
            {
                mZombiePhase = ZombiePhase::PHASE_LADDER_CARRYING;
                StartWalkAnim(0);
            }
        }
    }
}

//0x52AA40
void Zombie::UpdateZombieWalking()
{
    if (ZombieNotWalking() || mYuckyFace || hasDiarrhea)
        return;

    float aSpeed;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        if (IsBouncingPogo() || mZombiePhase == ZombiePhase::PHASE_BALLOON_FLYING || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING || 
            mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL || mZombieType == ZombieType::ZOMBIE_CATAPULT)
        {
            aSpeed = mVelX;
            if (IsMovingAtChilledSpeed())
            {
                aSpeed *= CHILLED_SPEED_FACTOR;
            }
        }
        else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || 
            IsBobsledTeamWithSled() || mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL)
        {
            aSpeed = mVelX;
        }
        else if (aBodyReanim->TrackExists("_ground"))
        {
            aSpeed = aBodyReanim->GetTrackVelocity("_ground") * mScaleZombie;
        }
        else
        {
            aSpeed = mVelX;
            if (IsMovingAtChilledSpeed())
            {
                aSpeed *= CHILLED_SPEED_FACTOR;
            }
        }

        if (IsWalkingBackwards() || mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN)
        {
            mPosX += aSpeed;
        }
        else
        {
            mPosX -= aSpeed;
        }

        if ((mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL) && mFromWave != Zombie::ZOMBIE_WAVE_WINNER)
        {
            if (aBodyReanim->ShouldTriggerTimedEvent(0.03f))
            {
                mApp->AddTodParticle(mX + 81, mY + 106, mRenderOrder - 1, ParticleEffect::PARTICLE_DUST_FOOT);
            }
            if (aBodyReanim->ShouldTriggerTimedEvent(0.61f))
            {
                mApp->AddTodParticle(mX + 87, mY + 110, mRenderOrder - 1, ParticleEffect::PARTICLE_DUST_FOOT);
            }
        }
        if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
        {
            if (aBodyReanim->ShouldTriggerTimedEvent(0.16f))
            {
                mApp->AddTodParticle(mX + 81, mY + 106, mRenderOrder - 1, ParticleEffect::PARTICLE_DUST_FOOT);
            }
            if (aBodyReanim->ShouldTriggerTimedEvent(0.67f))
            {
                mApp->AddTodParticle(mX + 87, mY + 110, mRenderOrder - 1, ParticleEffect::PARTICLE_DUST_FOOT);
            }
        }
    }
    else
    {
        bool doWalk = false;
        if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT ||
            mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
            mZombieType == ZombieType::ZOMBIE_DISCO ||
            mZombieType == ZombieType::ZOMBIE_DANCER ||
            mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP ||
            mZombieType == ZombieType::ZOMBIE_BOBSLED ||
            mZombieType == ZombieType::ZOMBIE_POGO ||
            mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER ||
            mZombieType == ZombieType::ZOMBIE_BALLOON)
        {
            doWalk = true;
        }
        else if ((mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER) && mInPool)
        {
            doWalk = true;
        }
        else if (mFrame >= 0 && mFrame <= 2)
        {
            doWalk = true;
        }
        else if (mFrame >= 6 && mFrame <= 8)
        {
            doWalk = true;
        }

        if (doWalk)
        {
            aSpeed = mVelX;
            if (IsMovingAtChilledSpeed())
            {
                aSpeed *= CHILLED_SPEED_FACTOR;
            }

            if (mReverseCounter > 0)    aSpeed *= -1;

            if (IsWalkingBackwards())
            {
                mPosX += aSpeed;
            }
            else
            {
                mPosX -= aSpeed;
            }
        }
    }

    if (mZombieType == ZOMBIE_BALLOON && mZombiePhase == PHASE_BALLOON_FLYING && mApp->mGameMode == GAMEMODE_CHALLENGE_WINDNESDAY)
    {
        mPosX -= aSpeed * 1.75f;
    }
}

Plant* Zombie::IsStandingOnSpikeweed()
{
    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT)
        return nullptr;

    Rect aZombieRect = GetZombieRect();

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && aPlant->IsSpiky() && !aPlant->NotOnGround() && (!mOnHighGround || aPlant->IsOnHighGround()))
        {
            Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
            if (GetRectOverlap(aPlantAttackRect, aZombieRect) > 0)
            {
                return aPlant;
            }
        }
    }

    return nullptr;
}

void Zombie::CheckForZombieStep()
{
    if ((mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT) && !mFlatTires)
    {
        CheckSquish(ZombieAttackType::ATTACKTYPE_DRIVE_OVER);
    }
}

//0x52AD30
void Zombie::UpdateZombiePosition()
{
    if (mZombieType == ZombieType::ZOMBIE_BUNGEE || mZombieType == ZombieType::ZOMBIE_BOSS || 
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM)
        return;

    UpdateZombieWalking();
    CheckForZombieStep();

    if (mBlowingAway)
    {
        mPosX += 10;
        if (mX > 850)
        {
            DieWithLoot();
            return;
        }
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL)
    {
        float aDesiredY = GetPosYBasedOnRow(mRow);
        if (mPosY < aDesiredY)
        {
            mPosY += min(aDesiredY - mPosY, 1.0f);
        }
        else if (mPosY > aDesiredY)
        {
            mPosY -= min(mPosY - aDesiredY, 1.0f);
        }
    }
}

//0x52AE40
bool Zombie::IsOnBoard()
{
    if (mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE || mFromWave == Zombie::ZOMBIE_WAVE_UI)
    {
        return false;
    }

    TOD_ASSERT(mBoard);
    return true;
}

void Zombie::UpdateBurn()
{
    mPhaseCounter--;
    if (mPhaseCounter == 0)
    {
        DieWithLoot();
    }
}

//0x52AE60
void Zombie::Update()
{
    TOD_ASSERT(!mDead);

    mZombieAge++;
    bool doUpdate = false;
    if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        doUpdate = true;
    }
    else if (IsOnBoard() && mBoard->mCutScene->ShouldRunUpsellBoard())
    {
        doUpdate = true;
    }
    else if (mApp->mGameScene == GameScenes::SCENE_PLAYING || !IsOnBoard() || mFromWave == Zombie::ZOMBIE_WAVE_WINNER)
    {
        doUpdate = true;
    }  

    if (doUpdate)
    {
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
        {
            UpdateBurn();
        }
        else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED)
        {
            UpdateMowered();
        }
        else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING)
        {
            UpdateDeath();
            UpdateZombieWalking();
        }
        else
        {
            if (mIsSunbeaned && mSunProduced < 300) {
                int currentHealth = mBodyHealth;
                if (mHelmHealth > 0)
                    currentHealth += mHelmHealth;
                if (mShieldHealth > 0)
                    currentHealth += mShieldHealth;
                int lostHealth = mPrevHealth - currentHealth;
                
                int smallSuns = floor(lostHealth / 15);
                int normalSuns = floor(lostHealth / 30);
                int largeSuns = floor(lostHealth / 60);

                smallSuns = max(0, smallSuns - normalSuns);
                normalSuns = max(0, normalSuns - largeSuns);

                for (int i = 0; i < smallSuns; i++) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    mSunProduced += 15;
                    mPrevHealth -= 15;
                }

                for (int i = 0; i < normalSuns; i++) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    mSunProduced += 25;
                    mPrevHealth -= 30;
                }

                for (int i = 0; i < largeSuns; i++) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_LARGESUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    mSunProduced += 50;
                    mPrevHealth -= 60;
                }
            }

            if (mPhaseCounter > 0 && !IsImmobilizied())
            {
                if (mChilledCounter > 0 && mZombieAge % 2 == 1 || mChilledCounter == 0)
                {
                    mPhaseCounter--;
                }
            }

            if (mHurrikaleTime > 0)
                mHurrikaleTime--;

            if ((mApp->mGameMode == GAMEMODE_CHALLENGE_DANCE_PARTY || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON) && !IsImmobilizied() && mBoard && mApp->mGameScene == SCENE_PLAYING)
            {
                if (mChilledCounter > 0 && mZombieAge % 2 == 1 || mChilledCounter == 0)
                {
                    if (mDanceSwitchRows > 0) {
                        --mDanceSwitchRows;
                    }
                    else if (mDanceSwitchRows <= 0 && mHasHead) {
                        mDanceSwitchRows = 750;
                        if (mIsEating)
                            StopEating();
                        bool aIsPool = mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON;
                        bool aCanGoUp = mBoard->RowCanHaveZombies(mRow - 1) && !(mBoard->mPlantRow[mRow - 1] == PlantRowType::PLANTROW_POOL ^ aIsPool);
                        bool aCanGoDown = mBoard->RowCanHaveZombies(mRow + 1) && !(mBoard->mPlantRow[mRow + 1] == PlantRowType::PLANTROW_POOL ^ aIsPool);

                        int direction = 0;

                        if (avoidRow != -1)
                        {
                            aCanGoUp = aCanGoUp && mRow - 1 != avoidRow;
                            aCanGoDown = aCanGoDown && mRow + 1 != avoidRow;
                            avoidRow = -1;
                        }

                        if (aCanGoUp && aCanGoDown)
                            direction = RandRangeInt(-1, 1);
                        else if (aCanGoUp || aCanGoDown)
                            direction = aCanGoUp ? -1 : 1;
                        else
                            return;

                        if (direction != 0 && mHasHead)
                            EnableDance(true);

                        SetRow(mRow + direction);
                    }
                }
            }

            if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
            {
                if (mBoard->mCutScene->ShowZombieWalking()){
                    UpdateZombieChimney();
                    UpdateZombieWalking();
                }
                else
                {
                    float targetPositionX = 0;
                    float targetPositionY = 0;

                    if (mBoard->mBackground == BackgroundType::BACKGROUND_1_DAY || mBoard->mBackground == BackgroundType::BACKGROUND_2_NIGHT ||
                        mBoard->mBackground == BackgroundType::BACKGROUND_3_POOL || mBoard->mBackground == BackgroundType::BACKGROUND_4_FOG)
                    {
                        targetPositionY = 290.0f;

                        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
                        {
                            targetPositionY += 30.0f;
                        }
                        else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
                        {
                            targetPositionY += 15.0f;
                        }
                    }
                    else if (mBoard->mBackground == BackgroundType::BACKGROUND_5_ROOF || mBoard->mBackground == BackgroundType::BACKGROUND_6_BOSS)
                    {
                        targetPositionX = -180.0f;
                        targetPositionY = 50.0f;

                        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
                        {
                            targetPositionY += 5.0f;
                        }
                        else if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
                        {
                            targetPositionX -= 14.0f;
                        }
                        else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
                        {
                            targetPositionX -= 28.0f;
                        }
                    }

                    if (mBoard->StageHasRoof())
                    {

                        int aEdgeX = BOARD_EDGE;
                        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
                        {
                            aEdgeX = -150;
                        }
                        else if (mZombieType == ZombieType::ZOMBIE_CATAPULT || mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL || mZombieType == ZombieType::ZOMBIE_ZAMBONI)
                        {
                            aEdgeX = -175;
                        }
                        else if (mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
                        {
                            aEdgeX = -130;
                        }

                        if (mBoard->mCutScene->mCutsceneTime <= 500)
                        {
                            mPosX = TodAnimateCurveFloatTime(0, 500, mBoard->mCutScene->mCutsceneTime, mGameOverStartX, aEdgeX / 1.5f, CURVE_LINEAR);
                            mPosY = TodAnimateCurveFloatTime(0, 500, mBoard->mCutScene->mCutsceneTime, mGameOverStartY, LAWN_YMIN + 85, CURVE_LINEAR);
                        }
                        else if (mBoard->mCutScene->mCutsceneTime <= 1000)
                        {
                            mPosX = TodAnimateCurveFloatTime(500, 1000, mBoard->mCutScene->mCutsceneTime, aEdgeX / 1.5f, aEdgeX, CURVE_LINEAR);
                            mPosY = TodAnimateCurveFloatTime(500, 1000, mBoard->mCutScene->mCutsceneTime, LAWN_YMIN + 85, targetPositionY - 25, CURVE_LINEAR);
                        }
                        else if (mBoard->mCutScene->mCutsceneTime <= 1370)
                        {
                            mPosX = TodAnimateCurveFloatTime(1000, 1370, mBoard->mCutScene->mCutsceneTime, aEdgeX, targetPositionX, CURVE_LINEAR);
                        }
                        else if (mBoard->mCutScene->mCutsceneTime <= 1500)
                        {
                            mPosY = TodAnimateCurveFloatTime(1370, 1500, mBoard->mCutScene->mCutsceneTime, targetPositionY - 25, targetPositionY, CURVE_LINEAR);
                        }

                        if (mBoard->mCutScene->mCutsceneTime == 1370)
                        {
                            mZombieHeight = ZombieHeight::HEIGHT_IN_TO_CHIMNEY;
                            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, 0, 2);
                            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                            if (aBodyReanim && aBodyReanim->TrackExists("anim_idle") && mZombieType != ZombieType::ZOMBIE_POLEVAULTER)
                            {
                                PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                            }
                        }
                        
                    }
                    else{
                        mPosY = TodAnimateCurveFloatTime(0, 1500, mBoard->mCutScene->mCutsceneTime, mGameOverStartY, targetPositionY, CURVE_LINEAR);
                    }
                }

            }
            else if (IsOnBoard())
            {
                UpdatePlaying();
            }

            if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
            {
                UpdateZombieBungee();
            }
            if (mZombieType == ZombieType::ZOMBIE_POGO)
            {
                UpdateZombiePogo();
            }

            Animate();
        }

        mJustGotShotCounter--;
        if (mShieldJustGotShotCounter > 0)
        {
            mShieldJustGotShotCounter--;
        }
        if (mHelmetJustGotShotCounter > 0)
        {
            mHelmetJustGotShotCounter--;
        }
        if (mShieldRecoilCounter > 0)
        {
            mShieldRecoilCounter--;
        }
        if (mHelmetRecoilCounter > 0) 
        {
            mHelmetRecoilCounter--;
        }
        if (mZombieFade > 0)
        {
            mZombieFade--;
            if (mZombieFade == 0)
            {
                DieNoLoot();
            }
        }

        mX = (int)mPosX;
        mY = (int)mPosY;

        AttachmentUpdateAndMove(mAttachmentID, mPosX, mPosY);
        UpdateReanim();
    }
}

//0x52B010
void Zombie::UpdateClimbingLadder()
{
    float aDistOffGround = mAltitude;
    if (mOnHighGround)
    {
        aDistOffGround -= HIGH_GROUND_HEIGHT;
    }
    int aLadderOriginX = mBoard->PixelToGridXKeepOnBoard(mX + 5 + aDistOffGround * 0.5f, mY);
    if (mBoard->GetLadderAt(aLadderOriginX, mRow) == nullptr)
    {
        mZombieHeight = ZombieHeight::HEIGHT_FALLING;
        return;
    }

    mAltitude += 0.8f;
    if (mVelX < 0.5f)
    {
        mPosX -= 0.5f;
    }

    float aTargetHeight = 90.0f;
    if (mOnHighGround)
    {
        aTargetHeight += HIGH_GROUND_HEIGHT;
    }
    if (mAltitude >= aTargetHeight)
    {
        mZombieHeight = ZombieHeight::HEIGHT_FALLING;
    }
}

//0x52B110
void Zombie::UpdateActions()
{
    if (mZombieHeight == ZombieHeight::HEIGHT_UP_LADDER)
    {
        UpdateClimbingLadder();
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM)
    {
        UpdateZombiquarium();
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL || mZombieHeight == ZombieHeight::HEIGHT_IN_TO_POOL || mInPool)
    {
        UpdateZombiePool();
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_UP_TO_HIGH_GROUND || mZombieHeight == ZombieHeight::HEIGHT_DOWN_OFF_HIGH_GROUND)
    {
        UpdateZombieHighGround();
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_FALLING)
    {
        UpdateZombieFalling();
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_IN_TO_CHIMNEY)
    {
        UpdateZombieChimney();
    }

    if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
    {
        UpdateZombiePolevaulter();
    }
    if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        UpdateZombieCatapult();
    }
    if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER)
    {
        UpdateZombieDolphinRider();
    }
    if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        UpdateZombieSnorkel();
    }
    if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        UpdateZombieFlyer();
    }
    if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER)
    {
        UpdateZombieNewspaper();
    }
    if (mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        UpdateZombieDigger();
    }
    if (mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX)
    {
        UpdateZombieJackInTheBox();
    }
    if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        UpdateZombieGargantuar();
    }
    if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
    {
        UpdateZombieBobsled();
    }
    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        UpdateZamboni();
    }
    if (mZombieType == ZombieType::ZOMBIE_LADDER)
    {
        UpdateLadder();
    }
    if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        UpdateYeti();
    }
    if (mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO)
    {
        UpdateZombieDancer();
    }
    if (mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        UpdateZombieBackupDancer();
    }
    if (mZombieType == ZombieType::ZOMBIE_IMP || mZombieType == ZombieType::ZOMBIE_NINJAIMP)
    {
        UpdateZombieImp();
    }
    if (mZombieType == ZombieType::ZOMBIE_WRAITH) {
        UpdateZombieWraith();
    }
    if (mZombieType == ZombieType::ZOMBIE_PEA_HEAD)
    {
        UpdateZombiePeaHead();
    }
    if (mZombieType == ZombieType::ZOMBIE_REPEATER_HEAD)
    {
        UpdateZombieRepeaterHead();
    }
    if (mZombieType == ZombieType::ZOMBIE_CHERRYBOMB_HEAD)
    {
        UpdateZombieCherryBombHead();
    }
    if (mZombieType == ZombieType::ZOMBIE_JALAPENO_HEAD)
    {
        UpdateZombieJalapenoHead();
    }
    if (mZombieType == ZombieType::ZOMBIE_GATLING_HEAD)
    {
        UpdateZombieGatlingHead();
    }
    if (mZombieType == ZombieType::ZOMBIE_SQUASH_HEAD)
    {
        UpdateZombieSquashHead();
    }
}

void Zombie::UpdateZombieWraith()
{
    if (mZombiePhase == PHASE_ZOMBIE_BURNED || IsDeadOrDying())
        return;

    Rect aZombieRect = GetZombieRect();
    int aGridX = mBoard->PixelToGridXKeepOnBoard(aZombieRect.mX + 42.5f, aZombieRect.mY + 42.5f);
    bool revealed = false;
    
    if (mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT)
    {
        float dX = mBoard->mCursorObject->mX - mX - 42.5f;
        float dY = mBoard->mCursorObject->mY - mY - 42.5f;
        float distance = sqrt(dX * dX + dY * dY);
        revealed = distance <= 127;
    }
    else
    {
        revealed = mBoard->FindPlanternPlant(aGridX, mRow, 1);;
    }
   
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim){
        if (revealed)
        {
            if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_INTANGIBLE) {
                PlayZombieReanim("anim_gasp", REANIM_PLAY_ONCE_AND_HOLD, 20, 20);
                mZombiePhase = PHASE_ZOMBIE_WRAITH_GASP;
            }
            else if (mZombiePhase == PHASE_ZOMBIE_WRAITH_GASP && aBodyReanim->mLoopCount > 0) {
                mZombiePhase = PHASE_ZOMBIE_NORMAL;
                if (mIsEating)
                    StopEating();
                StartWalkAnim(20);
            }
        }
        else {
            if (mZombiePhase != PHASE_ZOMBIE_INTANGIBLE && mZombiePhase != PHASE_ZOMBIE_WRAITH_GASP){
                mZombiePhase = PHASE_ZOMBIE_INTANGIBLE;
                if (mIsEating)
                    StopEating();

                aBodyReanim->StartBlend(20);
                aBodyReanim->mLoopType = REANIM_LOOP;
                aBodyReanim->mLoopCount = 0;
                aBodyReanim->GetFramesForLayer("anim_walk", aBodyReanim->mFrameStart, aBodyReanim->mFrameCount);

            }
        }
    }
}

//0x52B280
void Zombie::CheckForBoardEdge()
{
    if (IsWalkingBackwards() && mPosX > 850.0f)
    {
        DieNoLoot();
        return;
    }

    int aEdgeX = BOARD_EDGE;
    if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
    {
        aEdgeX = -150;
    }
    else if (mZombieType == ZombieType::ZOMBIE_CATAPULT || mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL || mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        aEdgeX = -175;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        aEdgeX = -130;
    }

    if (mX <= aEdgeX && mHasHead)
    {
        if (mApp->IsIZombieLevel())
        {
            TakeBodyDamage(999999, 0U);
            //DieNoLoot();
        }
        else
        {
            mBoard->ZombiesWon(this);
        }
    }
    if (mX <= aEdgeX + 70 && !mHasHead)
    {
        TakeDamage(1800, 9U);
    }
}

//0x52B340
void Zombie::UpdatePlaying()
{
    TOD_ASSERT(mBodyHealth > 0 || mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING);

    mGroanCounter--;
    int aZombiesCount = mBoard->mZombies.mSize;
    if (mGroanCounter == 0 && Rand(aZombiesCount) == 0 && mHasHead && mZombieType != ZombieType::ZOMBIE_BOSS && !mBoard->HasLevelAwardDropped())
    {
        float aPitch = 0.0f;
        if ((mApp->IsLittleTroubleLevel()|| mTiny))
        {
            aPitch = RandRangeFloat(40.0f, 50.0f);
        }

        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR)
        {
            mApp->PlayFoley(FoleyType::FOLEY_LOW_GROAN);
        }
        else if (mVariant)
        {
            mApp->PlayFoleyPitch(FoleyType::FOLEY_BRAINS, aPitch);
        }
        else if (mApp->mSukhbirMode)
        {
            mApp->PlayFoleyPitch(FoleyType::FOLEY_SUKHBIR, aPitch);
        }
        else
        {
            mApp->PlayFoleyPitch(FoleyType::FOLEY_GROAN, aPitch);
        }

        mGroanCounter = Rand(1000) + 500;
    }

    if (mIceTrapCounter > 0)
    {
        mIceTrapCounter--;
        if (mIceTrapCounter == 0)
        {
            RemoveIceTrap();
            AddAttachedParticle(75, 106, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
        }
    }
    if (mChilledCounter > 0)
    {
        mChilledCounter--;
        if (mChilledCounter == 0)
        {
            UpdateAnimSpeed();
        }
    }
    if (mButteredCounter > 0)
    {
        mButteredCounter--;
        if (mButteredCounter == 0)
        {
            RemoveButter();
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE)
    {
        UpdateZombieRiseFromGrave();
        return;
    }

    if (mHurrikaleTime > 0 && mX < 850) {
        mPosX += 1;
    }

    if (!IsImmobilizied())
    {
        UpdateActions();
        UpdateZombiePosition();
        CheckIfPreyCaught();
        CheckForPool();
        CheckForHighGround();
        CheckForBoardEdge();

        if (hasDiarrhea)
        {
            /*diarrheaCount++;
            if (diarrheaCount > 200)
            {
                float posX = mPosX + mWidth / 2.0f, posY = mPosY + mHeight / 2.0f;
                mApp->AddTodParticle(posX, posY, (int)RenderLayer::RENDER_LAYER_PLANT, ParticleEffect::PARTICLE_POWIE);
                Zombie* aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie)) {
                    if (GetCircleRectOverlap(posX, posY, 90, aZombie->GetZombieRect())) {
                         aZombie->avoidRow = mRow;
                         aZombie->mYuckyFace = true;
                         aZombie->mYuckyFaceCounter = 0;
                         aZombie->UpdateAnimSpeed();
                         
                    }
                }
               
                TakeDamage(9999, 0);
                hasDiarrhea = false;
                diarrheaCount = 0;
                PickRandomSpeed();
            }*/
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        UpdateBoss();
    }

    if (!IsDeadOrDying() && mFromWave != Zombie::ZOMBIE_WAVE_WINNER)
    {
        bool isDying = !mHasHead;
        if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT)
        {
            if (mBodyHealth < 200)
            {
                isDying = true;
            }
        }

        if (isDying)
        {
            int aDamage = 1;
            if (mZombieType == ZombieType::ZOMBIE_YETI)
            {
                aDamage = 10;
            }
            if (mBodyMaxHealth >= 500)
            {
                aDamage = 3;
            }

            if (Rand(5) == 0)
            {
                TakeDamage(aDamage, 9U);
            }
        }
    }
}

//0x52B560
bool Zombie::HasYuckyFaceImage()
{
    if (mBoard->mFutureMode)
        return false;

    return 
        mZombieType == ZombieType::ZOMBIE_NORMAL || 
        mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE || 
        mZombieType == ZombieType::ZOMBIE_PAIL || 
        mZombieType == ZombieType::ZOMBIE_FLAG || 
        mZombieType == ZombieType::ZOMBIE_DOOR || 
        mZombieType == ZombieType::ZOMBIE_GLASSDOOR ||
        mZombieType == ZombieType::ZOMBIE_DUCKY_TUBE || 
        mZombieType == ZombieType::ZOMBIE_DANCER || 
        mZombieType == ZombieType::ZOMBIE_DISCO ||
        mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP || 
        mZombieType == ZombieType::ZOMBIE_NEWSPAPER || 
        mZombieType == ZombieType::ZOMBIE_POLEVAULTER;
}

//0x52B5B0
void Zombie::ShowYuckyFace(bool theShow)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if (HasYuckyFaceImage())
    {
        if (theShow)
        {
            aBodyReanim->SetImageOverride("anim_head1", IMAGE_REANIM_ZOMBIE_HEAD_GROSSOUT);
            aBodyReanim->AssignRenderGroupToTrack("anim_head2", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("anim_head_jaw", RENDER_GROUP_HIDDEN);
            aBodyReanim->AssignRenderGroupToTrack("anim_tongue", RENDER_GROUP_HIDDEN);
        }
        else if (mHasHead)
        {
            aBodyReanim->SetImageOverride("anim_head1", nullptr);
            aBodyReanim->AssignRenderGroupToTrack("anim_head2", RENDER_GROUP_NORMAL);
            aBodyReanim->AssignRenderGroupToTrack("anim_head_jaw", RENDER_GROUP_NORMAL);
            if (mVariant && !IsZombotany(mZombieType))
            {
                aBodyReanim->AssignRenderGroupToTrack("anim_tongue", RENDER_GROUP_NORMAL);
            }
        }
    }
}

//0x52B6A0
void Zombie::UpdateYuckyFace()
{
    mYuckyFaceCounter++;
    if (mYuckyFaceCounter > 20 && mYuckyFaceCounter < 170 && !HasYuckyFaceImage())
    {
        StopEating();
        mYuckyFaceCounter = 170;
        if (mBoard->CountZombiesOnScreen() <= 5 && mHasHead)
        {
            mApp->PlayFoley(FoleyType::FOLEY_YUCK);
        }
        else if (mBoard->CountZombiesOnScreen() <= 10 && mHasHead && Rand(2) == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_YUCK);
        }
    }

    if (mYuckyFaceCounter > 270)
    {
        ShowYuckyFace(false);
        mYuckyFace = false;
        mYuckyFaceCounter = 0;
        StartWalkAnim(20);
        return;
    }

    if (mYuckyFaceCounter == 70)
    {
        StopEating();
        ShowYuckyFace(true);
        if (mBoard->CountZombiesOnScreen() <= 5 && mHasHead)
        {
            mApp->PlayFoley(FoleyType::FOLEY_YUCK);
        }
        else if (mBoard->CountZombiesOnScreen() <= 10 && mHasHead && Rand(2) == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_YUCK);
        }
    }

    if (mYuckyFaceCounter == 170)
    {
        bool aIsPool = mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON;
        bool aCanGoUp = mBoard->RowCanHaveZombies(mRow - 1) && !(mBoard->mPlantRow[mRow - 1] == PlantRowType::PLANTROW_POOL ^ aIsPool);
        bool aCanGoDown = mBoard->RowCanHaveZombies(mRow + 1) && !(mBoard->mPlantRow[mRow + 1] == PlantRowType::PLANTROW_POOL ^ aIsPool);

        int direction = 0;

        if (avoidRow != -1)
        {
            aCanGoUp = aCanGoUp && mRow - 1 != avoidRow;
            aCanGoDown = aCanGoDown && mRow + 1 != avoidRow;
            avoidRow = -1;
        }

        if (aCanGoUp && aCanGoDown)
        {
            direction = Rand(2) == 0 ? -1 : 1;
        }
        else if (aCanGoUp || aCanGoDown)
        {
            direction = aCanGoUp ? -1 : 1;
        }
       
        if (direction != 0)
            StartWalkAnim(20);
        
        SetRow(mRow + direction);
    }
}

//0x52B940
void Zombie::AnimateChewSound()
{
    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_UP_TO_EAT)
        return;

    Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
    if (aPlant)
    {
        if (aPlant->mSeedType == SeedType::SEED_HYPNOSHROOM && !aPlant->mIsAsleep)
        {
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
            aPlant->Die();

            StartMindControlled();
            mApp->AddTodParticle(mPosX + 60.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MIND_CONTROL);
            TrySpawnLevelAward();

            mVelX = 0.17f;
            mAnimTicksPerFrame = 18;
            UpdateAnimSpeed();
        }
        else if (aPlant->mSeedType == SeedType::SEED_GARLIC && !isImmuneToGarlic)
        {
            if (!mYuckyFace)
            {
                mYuckyFace = true;
                mYuckyFaceCounter = 0;
                UpdateAnimSpeed();
                mApp->PlayFoley(FoleyType::FOLEY_CHOMP);
            }
        }
        else
        {
            if (aPlant->mSeedType == SeedType::SEED_WALLNUT || aPlant->mSeedType == SeedType::SEED_FIRESHROOM || aPlant->mSeedType == SeedType::SEED_TALLNUT || aPlant->mSeedType == SeedType::SEED_PUMPKINSHELL || aPlant->mSeedType == SeedType::SEED_GIANT_WALLNUT || aPlant->mSeedType == SeedType::SEED_EXPLODE_O_NUT)
            {
                mApp->PlayFoley(FoleyType::FOLEY_CHOMP_SOFT);
            }
            else
            {
                mApp->PlayFoley(FoleyType::FOLEY_CHOMP);
            }
        }
    }
    else
    {
        if (mMindControlled)
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHOMP_SOFT);
        }
        else
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHOMP);
        }
    }
}

//0x52BB40
void Zombie::AnimateChewEffect()
{
    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_UP_TO_EAT)
        return;

    if (mApp->IsIZombieLevel())
    {
        GridItem* aBrain = mBoard->mChallenge->IZombieGetBrainTarget(this);
        if (aBrain)
        {
            aBrain->mTransparentCounter = max(aBrain->mTransparentCounter, 25);
            return;
        }
    }

    Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
    if (aPlant)
    {
        if (mIsAllergicToNuts && (aPlant->mSeedType == SEED_WALLNUT || aPlant->mSeedType == SEED_TALLNUT || aPlant->mSeedType == SEED_GIANT_WALLNUT || aPlant->mSeedType == SEED_EXPLODE_O_NUT || aPlant->mSeedType == SEED_ARTIC_NUT)) {
            mBodyHealth = 0;
            PlayDeathAnim(0U);
        }

        if (aPlant->mSeedType == SeedType::SEED_WALLNUT || aPlant->mSeedType == SeedType::SEED_FIRESHROOM || aPlant->mSeedType == SeedType::SEED_ARTIC_NUT || aPlant->mSeedType == SeedType::SEED_EXPLODE_O_NUT || aPlant->mSeedType == SeedType::SEED_TALLNUT || aPlant->mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);
            ZombieDrawPosition aDrawPos;
            GetDrawPos(aDrawPos);

            float aPosX = mPosX + 37.0f;
            float aPosY = mPosY + 40.0f + aDrawPos.mBodyY;

            if (mZombiePhase == PHASE_SNORKEL_EATING_IN_POOL || mZombiePhase == PHASE_DOLPHIN_WALKING_IN_POOL)
            {
                aPosX -= 7.0f;
                aPosY += 70.0f;
            }
            else if (IsWalkingBackwards())
            {
                aPosX += 47.0f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_BALLOON)
            {
                aPosY += 47.0f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_IMP || mZombieType == ZombieType::ZOMBIE_NINJAIMP)
            {
                aPosX += 24.0f;
                aPosY += 40.0f;
            }

            TodParticleSystem* aPrticle = mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, ParticleEffect::PARTICLE_WALLNUT_EAT_SMALL);
            if (aPlant->mSeedType == SeedType::SEED_EXPLODE_O_NUT || aPlant->mSeedType == SeedType::SEED_TORCHWOOD) {
                aPrticle->OverrideColor(nullptr, Color(255, 64, 64));
            }
        }

        aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);
        if (aPlant->mSeedType == SEED_TORCHWOOD) {
            TakeDamage(20, 2U);
            RemoveColdEffects();
            mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
            Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY + 20, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
            aFireReanim->mAnimRate = 24.0f;
            aFireReanim->OverrideScale(0.7f, 0.4f);
        }
        else if (aPlant->mSeedType == SEED_PLASMAWOOD) {
            TakeDamage(40, 2U);
            RemoveColdEffects();
            mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
            Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY + 20, mRenderOrder + 1, ReanimationType::REANIM_PLASMA_FIRE);
            aFireReanim->mAnimRate = 24.0f;
            aFireReanim->OverrideScale(0.7f, 0.4f);
        }
    }

    Zombie* aZombie = FindZombieTarget();
    if (aZombie) {
        int aFrameLength = mChilledCounter > 0 ? 12 : 6;

        if (aZombie->mShieldHealth > 0){
            aZombie->mShieldJustGotShotCounter = 25;;
            aZombie->mShieldRecoilCounter = 12;
            if (aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR || aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER)
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            
        }
        else if (aZombie->mHelmHealth > 0)
        {
            aZombie->mHelmetJustGotShotCounter = 25;
            aZombie->mHelmetRecoilCounter = 12;
            if (aZombie->mHelmType == HELMTYPE_PAIL || aZombie->mHelmType == HELMTYPE_DIVER)
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            else if (aZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || aZombie->mHelmType == HELMTYPE_DIGGER || aZombie->mHelmType == HELMTYPE_FOOTBALL)
                mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
        }
        else
        {
            aZombie->mJustGotShotCounter = max(aZombie->mJustGotShotCounter, 25);
        }
    }
}

//0x52BCA0
void Zombie::Animate()
{
    mPrevFrame = mFrame;
    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || 
        mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MADDENING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED || 
        IsImmobilizied())
        return;
    
    if (mReverseCounter > 0)    mReverseCounter--;
    else if (mReverseCounter == 0)      UpdateAnimSpeed();
    mAnimCounter = mAnimCounter + (mReverseCounter > 0 ? -1 : 1);

    if (mYuckyFace)
    {
        UpdateYuckyFace();
    }

    if (IsZombotany(mZombieType))
    {
        UpdateBlink();
    }

    if (mIsEating && mHasHead)
    {
        int aFrameLength = 6;
        if (mChilledCounter > 0)
        {
            aFrameLength = 12;
        }
        if (mAnimCounter >= mAnimFrames * aFrameLength)
        {
            mAnimCounter = aFrameLength;
        }
        mFrame = mAnimCounter / aFrameLength;

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim)
        {
            float aLeftHandTime = 0.14f;
            float aRightHandTime = 0.68f;
            if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
            {
                aLeftHandTime = 0.38f;
                aRightHandTime = 0.8f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER || mZombieType == ZombieType::ZOMBIE_LADDER)
            {
                aLeftHandTime = 0.42f;
                aRightHandTime = 0.42f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX)
            {
                aLeftHandTime = 0.53f;
                aRightHandTime = 0.53f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
            {
                aLeftHandTime = 0.33f;
                aRightHandTime = 0.83f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_IMP || mZombieType == ZombieType::ZOMBIE_NINJAIMP)
            {
                aLeftHandTime = 0.33f;
                aRightHandTime = 0.79f;
            }

            if (aBodyReanim->ShouldTriggerTimedEvent(aLeftHandTime) || aBodyReanim->ShouldTriggerTimedEvent(aRightHandTime))
            {
                AnimateChewSound();
                AnimateChewEffect();
            }
        }
        else
        {
            if (mAnimCounter == 4 * aFrameLength)
            {
                AnimateChewSound();
            }
            if (mAnimCounter == 7 * aFrameLength)
            {
                AnimateChewEffect();
            }
        }
    }
    else
    {
        if (mAnimCounter >= mAnimFrames * mAnimTicksPerFrame)
        {
            mAnimCounter = 0;
        }
        mFrame = mAnimCounter / mAnimTicksPerFrame;
    }
}

//0x52BE80
void Zombie::DrawZombie(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    // 此函数仅 0.1.1 内测版有效，0.9.9 内测版及正式版中正常不会调用

    switch (mZombieType)
    {
    case ZombieType::ZOMBIE_NORMAL:
    case ZombieType::ZOMBIE_FLAG:
    case ZombieType::ZOMBIE_TRAFFIC_CONE:
    case ZombieType::ZOMBIE_PAIL:
    case ZombieType::ZOMBIE_NEWSPAPER:
    case ZombieType::ZOMBIE_GLASSDOOR:
    case ZombieType::ZOMBIE_DOOR:
    case ZombieType::ZOMBIE_BLACK_FOOTBALL:
    case ZombieType::ZOMBIE_FOOTBALL:
    case ZombieType::ZOMBIE_DOLPHIN_RIDER:
    case ZombieType::ZOMBIE_LADDER:
    //case ZombieType::ZOMBIE_DOG_WALKER:
        DrawZombieWithParts(g, theDrawPos);
        break;

    //case ZombieType::ZOMBIE_DOG:
    //    DrawZombiePart(g, IMAGE_ZOMBIEDOG, mIsEating ? ZombieParts::PART_HEAD : ZombieParts::PARTS_BODY, mFrame, theDrawPos);
    //    break;

    //case ZombieType::ZOMBIE_PROPELLER:
    //    DrawZombiePart(g, IMAGE_PROPELLERZOMBIE, ZombieParts::PARTS_BODY, mFrame, theDrawPos);
    //    break;

    //case ZombieType::ZOMBIE_POLEVAULTER:
    //case ZombieType::ZOMBIE_DANCER:
    //case ZombieType::ZOMBIE_BACKUP_DANCER:
    //case ZombieType::ZOMBIE_DUCKY_TUBE:
    //case ZombieType::ZOMBIE_SNORKEL:
    //case ZombieType::ZOMBIE_ZAMBONI:
    //case ZombieType::ZOMBIE_BOBSLED:
    //case ZombieType::ZOMBIE_JACK_IN_THE_BOX:
    //case ZombieType::ZOMBIE_BALLOON:
    //case ZombieType::ZOMBIE_DIGGER:
    //case ZombieType::ZOMBIE_POGO:
    //case ZombieType::ZOMBIE_YETI:
    //case ZombieType::ZOMBIE_BUNGEE:
    //case ZombieType::ZOMBIE_CATAPULT:
    //case ZombieType::ZOMBIE_GARGANTUAR:
    //case ZombieType::ZOMBIE_IMP:
    //case ZombieType::ZOMBIE_BOSS:
    //    TOD_ASSERT();
    //    break;

    default:
        TOD_ASSERT();
        break;
    }
}

//0x52BEE0
bool Zombie::IsWalkingBackwards()
{
    if (mMindControlled || mWalkBack)
        return true;

    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM)
    {
        if (mVelZ < 1.5707964f || mVelZ > 4.712389f)
        {
            return mVelZ != 0;
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_DIGGER && mApp->mGameMode != GAMEMODE_CHALLENGE_BORDER_TREATY)
    {
        if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED || mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
        {
            return true;
        }
        else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED)
        {
            return mHasObject;
        }

        return false;
    }

    return mZombieType == ZombieType::ZOMBIE_YETI && !mHasObject;
}

//0x52BF60
void Zombie::DrawZombiePart(Graphics* g, Image* theImage, int theFrame, int theRow, const ZombieDrawPosition& theDrawPos)
{
    // 此函数仅 0.1.1 内测版有效，0.9.9 内测版及正式版中正常不会调用

    int aCelWidth = theImage->GetCelWidth();
    int aCelHeight = theImage->GetCelHeight();
    float anOffsetX = theDrawPos.mImageOffsetX;
    float anOffsetY = theDrawPos.mImageOffsetY + theDrawPos.mBodyY;
    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT)
    {
        anOffsetX -= 120.0f;
        anOffsetY -= 120.0f;
    }
    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
    {
        anOffsetY += 50.0f;
    }
    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        anOffsetY -= 19.0f;
    }

    float aDrawHeight = aCelHeight;
    if (theDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT)
    {
        aDrawHeight = ClampFloat(aCelHeight - theDrawPos.mClipHeight, 0.0f, aCelHeight);
    }

    int anAlpha = 255;
    if (mZombieFade >= 0)
    {
        anAlpha = ClampInt(255 * mZombieFade / 10, 0, 255);
        g->SetColorizeImages(true);
        g->SetColor(Color(255, 255, 255, anAlpha));
    }

    bool aMirror = false;
    if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN || mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_LEFT)
    {
        int aFrame = GetDancerFrame();
        if (!mIsEating && (aFrame == 12 || aFrame == 13 || aFrame == 14 || aFrame == 18 || aFrame == 19 || aFrame == 20))
        {
            aMirror = true;
            anOffsetX -= 30.0f;
        }
    }
    if (aMirror)
    {
        anOffsetX = -anOffsetX;
    }

    Rect aSrcRect(theFrame * aCelWidth, theRow * aCelHeight, aCelWidth, aDrawHeight);
    Rect aDestRect(anOffsetX, anOffsetY, aCelWidth, aDrawHeight);
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
    {
        if (mMindControlled)
            aMirror = true;
        
        g->SetColorizeImages(true);
        g->SetColor(Color::Black);
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);
    }
    else if (mMindControlled)
    {
        aMirror = true;
        g->SetColorizeImages(true);
        Color aMincontrolledColor = ZOMBIE_MINDCONTROLLED_COLOR;
        aMincontrolledColor.mAlpha = anAlpha;
        g->SetColor(aMincontrolledColor);
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);

        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }
    else if (mChilledCounter > 0 || mIceTrapCounter > 0)
    {
        g->SetColorizeImages(true);
        g->SetColor(Color(75, 75, 255, anAlpha));
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);

        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }
    else
    {
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);
    }

    if (mJustGotShotCounter > 0)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        int aGrayness = mJustGotShotCounter * 10;
        g->SetColor(Color(aGrayness, aGrayness, aGrayness, 255));
        g->DrawImageMirror(theImage, aDestRect, aSrcRect, aMirror);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }

    g->SetColorizeImages(false);
}

void Zombie::DrawZombieHead(Graphics* g, const ZombieDrawPosition& theDrawPos, int theFrame)
{
    // 此函数仅 0.1.1 内测版有效，0.9.9 内测版及正式版中正常不会调用

    /*
    if (mYuckyFace)
    {
        DrawZombiePart(g, IMAGE_ZOMBIE, mFrame, ZombieParts::PART_HEAD_YUCKY, theDrawPos);
        return;
    }

    if (mIsEating)
    {
        DrawZombiePart(g, IMAGE_ZOMBIE, mFrame, ZombieParts::PART_HEAD_EATING, theDrawPos);

        if (mHelmHealth == 0)
        {
            DrawZombiePart(g, IMAGE_ZOMBIE, 5, ZombieParts::PART_HAIR, theDrawPos);
        }
    }
    else
    {
        DrawZombiePart(g, IMAGE_ZOMBIE, mFrame, ZombieParts::PART_HEAD, theDrawPos);

        if (mVariant)
        {
            DrawZombiePart(g, IMAGE_ZOMBIE, theFrame, ZombieParts::PART_TONGUE, theDrawPos);
        }

        if (mHelmHealth == 0)
        {
            DrawZombiePart(g, IMAGE_ZOMBIE, theFrame, ZombieParts::PART_HAIR, theDrawPos);
        }
    }
    */
}

void Zombie::DrawZombieWithParts(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    // 此函数仅 0.1.1 内测版有效，0.9.9 内测版及正式版中正常不会调用

    /*
    int aFrame = mIsEating ? 0 : mFrame;
    DrawZombiePart(g, IMAGE_ZOMBIE, aFrame, ZombieParts::PARTS_BODY, theDrawPos);

    if (mHasArm && mBodyReanimID == ReanimationID::REANIMATIONID_NULL)
    {
        if (mZombiePhase == ZombiePhase::PHASE_WALKING_DOG && mHasHead)
        {
            ZombieDrawPosition theDrawPosLeash = theDrawPos;
            theDrawPosLeash.mImageOffsetX -= 14.0f;
            theDrawPosLeash.mImageOffsetY += 10.0f;
            DrawZombiePart(g, IMAGE_ZOMBIE, aFrame, ZombieParts::PART_ARM_LEASH, theDrawPosLeash);
        }
        else
        {
            DrawZombiePart(g, IMAGE_ZOMBIE, aFrame, ZombieParts::PART_ARM, theDrawPos);
        }
    }

    if (mHasHead && mBodyReanimID == ReanimationID::REANIMATIONID_NULL)
    {
        DrawZombieHead(g, theDrawPos, aFrame);
    }
    */
}

//0x52C310
void Zombie::UpdateReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr || aBodyReanim->mDead)
        return;

    if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        if (GetBodyDamageIndex() == 2 || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING)
        {
            Reanimation* aReanim = mApp->ReanimationGet(mBodyReanimID);
            Image* aPoleImage = aReanim->GetCurrentTrackImage("Zombie_catapult_pole");
            if (aPoleImage == IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_WITHBALL && mSummonCounter != 0)
            {
                aReanim->SetImageOverride("Zombie_catapult_pole", IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE_WITHBALL);
            }
            else
            {
                aReanim->SetImageOverride("Zombie_catapult_pole", IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE);
            }
        }
        else if (mSummonCounter == 0)
        {
            aBodyReanim->SetImageOverride("Zombie_catapult_pole", IMAGE_REANIM_ZOMBIE_CATAPULT_POLE);
        }
    }

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    float anOffsetX = aDrawPos.mImageOffsetX + 15.0f;
    float anOffsetY = aDrawPos.mImageOffsetY + aDrawPos.mBodyY - 28.0f + 20.0f;
    if ((mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT) && mZombiePhase != ZombiePhase::PHASE_ZOMBIE_BURNED)
    {
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING)
        {
            float aShakeRange = TodAnimateCurveFloatTime(0.7f, 1.0f, aBodyReanim->mAnimTime, 0.0f, 1.0f, TodCurves::CURVE_EASE_OUT);
            anOffsetX += RandRangeFloat(-aShakeRange, aShakeRange);
            anOffsetY += RandRangeFloat(-aShakeRange, aShakeRange);
        }
        else if (mBodyHealth < 200)
        {
            anOffsetX += RandRangeFloat(-1.0f, 1.0f);
            anOffsetY += RandRangeFloat(-1.0f, 1.0f);
        }
    }
    if ((mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType ==  ZombieType::ZOMBIE_BLACK_FOOTBALL) && mScaleZombie < 1.0f)
    {
        anOffsetY += 20.0f - mScaleZombie * 20.0f;
    }

    bool anOpposite = false;
    if (IsWalkingBackwards())
    {
        anOpposite = true;
    }
    if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        anOpposite = false;

        if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN || mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1 || 
            mZombiePhase == ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2)
        {
            if (!mIsEating)
            {
                anOpposite = true;
            }
        }

        if (mMindControlled)
        {
            anOpposite = !anOpposite;
        }
    }

    if (anOpposite)
    {
        anOffsetX += 90.0f * mScaleZombie;
    }

    if (IsZombotany(mZombieType))
    {
        if (IsOnBoard())
        {

            if ((mZombieType == ZOMBIE_JALAPENO_HEAD || mZombieType == ZOMBIE_CHERRYBOMB_HEAD) && mPhaseCounter > 0 && mPhaseCounter <= 100)
            {
                anOffsetX += RandRangeFloat(-1.0f, 1.0f);
                anOffsetY += RandRangeFloat(-1.0f, 1.0f);
            }
        }
    }

    aBodyReanim->mOverlayMatrix.m10 = 0.0f;
    aBodyReanim->mOverlayMatrix.m20 = 0.0f;
    aBodyReanim->mOverlayMatrix.m11 = 0.0f;
    aBodyReanim->mOverlayMatrix.m21 = 0.0f;
    aBodyReanim->OverrideScale(mScaleZombie, mScaleZombie);
    aBodyReanim->SetPosition(anOffsetX + 30.0f - mScaleZombie * 30.0f, anOffsetY + 120.0f - mScaleZombie * 120.0f);
    if (anOpposite)
    {
        aBodyReanim->mOverlayMatrix.m00 = -mScaleZombie;
    }

    Reanimation* aMoweredReanim = mApp->ReanimationTryToGet(mMoweredReanimID);
    if (aMoweredReanim)
    {
        aMoweredReanim->Update();

        SexyTransform2D aOverlayMatrix;
        aMoweredReanim->GetAttachmentOverlayMatrix(0, aOverlayMatrix);
        aOverlayMatrix.m00 *= aBodyReanim->mOverlayMatrix.m00;
        aOverlayMatrix.m10 *= aBodyReanim->mOverlayMatrix.m00;
        aOverlayMatrix.m01 *= aBodyReanim->mOverlayMatrix.m11;
        aOverlayMatrix.m11 *= aBodyReanim->mOverlayMatrix.m11;
        aOverlayMatrix.m02 *= aBodyReanim->mOverlayMatrix.m00;
        aOverlayMatrix.m12 *= aBodyReanim->mOverlayMatrix.m11;
        aOverlayMatrix.m02 += aBodyReanim->mOverlayMatrix.m11;
        aOverlayMatrix.m12 += aBodyReanim->mOverlayMatrix.m12;
        aBodyReanim->mOverlayMatrix = aOverlayMatrix;
    }

    aBodyReanim->Update();
    aBodyReanim->PropogateColorToAttachments();
}

//0x52C6D0
void Zombie::DrawBobsledReanim(Graphics* g, const ZombieDrawPosition& theDrawPos, bool theBeforeZombie)
{
    int aPosition = GetBobsledPosition();
    bool aDrawFront = false;
    bool aDrawBack = false;
    Zombie* aZombieLeader;
    if (mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE)
    {
        aZombieLeader = this;
    }
    else
    {
        if (aPosition == -1)
        {
            return;
        }
        if (aPosition == 0)
        {
            aZombieLeader = this;
        }
        else
        {
            aZombieLeader = mBoard->ZombieGet(mRelatedZombieID);
        }
    }

    if (mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE)
    {
        if (theBeforeZombie)  // 选卡界面中，依次绘制雪橇背面、僵尸本体和雪橇正面
        {
            aDrawBack = true;
        }
        else
        {
            aDrawFront = true;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING)
    {
        if (aPosition == 0 && !theBeforeZombie)  // 雪橇损坏后，在绘制领头僵尸结束后绘制雪橇背面和雪橇正面
        {
            aDrawFront = true;
            aDrawBack = true;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_SLIDING || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
    {
        if (aPosition == 2 && theBeforeZombie)  // 推行雪橇时或雪橇化为灰烬后，在绘制第 2 只僵尸之前绘制雪橇背面和雪橇正面
        {
            aDrawFront = true;
            aDrawBack = true;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_BOARDING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mAnimTime < 0.5f)  // 起跳在空中时，绘制第 2 只僵尸之前绘制雪橇背面和雪橇正面
        {
            if (aPosition == 2 && theBeforeZombie)
            {
                aDrawFront = true;
                aDrawBack = true;
            }
        }
        else if (aPosition == 0 && !theBeforeZombie)  // 进入雪橇后，绘制领头僵尸之后绘制雪橇正面
        {
            aDrawFront = true;
        }
        else if (aPosition == 3 && theBeforeZombie)  // 进入雪橇后，绘制末尾僵尸之前绘制雪橇背面
        {
            aDrawBack = true;
        }
    }

    float aOffsetX = aZombieLeader->mPosX + theDrawPos.mImageOffsetX - mPosX - 76.0f;
    float aOffsetY = 15.0f;
    int aBobsledDamageStatus;
    if (mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING)
    {
        aBobsledDamageStatus = 3;
        int aAlpha = TodAnimateCurve(30, 0, mPhaseCounter, 255, 0, TodCurves::CURVE_LINEAR);
        aOffsetX += (BOBSLED_CRASH_TIME - mPhaseCounter) * mVelX / ZOMBIE_LIMP_SPEED_FACTOR;  // 还原至雪橇损坏时的位置
        aOffsetX -= TodAnimateCurveFloat(BOBSLED_CRASH_TIME, 0, mPhaseCounter, 0.0f, 50.0f, TodCurves::CURVE_EASE_OUT);  // 计算雪橇惯性产生的横向位移
        aOffsetY += TodAnimateCurveFloat(BOBSLED_CRASH_TIME, 75, mPhaseCounter, 5.0f, 10.0f, TodCurves::CURVE_LINEAR);
        if (aAlpha != 255)
        {
            g->SetColorizeImages(true);
            g->SetColor(Color(255, 255, 255, aAlpha));
        }
    }
    else
    {
        aBobsledDamageStatus = aZombieLeader->GetHelmDamageIndex();
    }
   
    Image* aImage;
    if (aBobsledDamageStatus == 0)
    {
        aImage = IMAGE_ZOMBIE_BOBSLED1;
    }
    else if (aBobsledDamageStatus == 1)
    {
        aImage = IMAGE_ZOMBIE_BOBSLED2;
    }
    else if (aBobsledDamageStatus == 2)
    {
        aImage = IMAGE_ZOMBIE_BOBSLED3;
    }
    else
    {
        aImage = IMAGE_ZOMBIE_BOBSLED4;
    }

    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
    {
        g->SetColorizeImages(true);
        g->SetColor(Color::Black);
    }

    if (aDrawBack && aBobsledDamageStatus != 3)
    {
        g->DrawImageF(IMAGE_ZOMBIE_BOBSLED_INSIDE, aOffsetX, aOffsetY);
    }
    if (aDrawFront)
    {
        g->DrawImageF(aImage, aOffsetX, aOffsetY);
    }

    if (aZombieLeader->mHelmetJustGotShotCounter > 0)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        int aGrayness = aZombieLeader->mHelmetJustGotShotCounter * 10;
        g->SetColor(Color(aGrayness, aGrayness, aGrayness, 255));

        if (aDrawBack && aBobsledDamageStatus != 3)
        {
            g->DrawImageF(IMAGE_ZOMBIE_BOBSLED_INSIDE, aOffsetX, aOffsetY);
        }
        if (aDrawFront)
        {
            g->DrawImageF(aImage, aOffsetX, aOffsetY);
        }

        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }

    g->SetColorizeImages(false);
}

//0x52CA90
void Zombie::DrawBungeeReanim(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    float anOffsetY = theDrawPos.mBodyY + theDrawPos.mImageOffsetY + 14.0f;
    DrawBungeeCord(g, -22, anOffsetY);
    aBodyReanim->Draw(g);

    Zombie* aDroppedZombie = mBoard->ZombieTryToGet(mRelatedZombieID);
    if (aDroppedZombie)
    {
        Graphics aDropGraphics(*g);
        aDropGraphics.mTransY -= mAltitude;
        aDropGraphics.mTransX += aDroppedZombie->mPosX - mPosX;
        //aDropGraphics.Translate(aDroppedZombie->mPosX - mPosX, -mAltitude);
        
        ZombieDrawPosition aDroppedDrawPos;
        aDroppedZombie->GetDrawPos(aDroppedDrawPos);
        aDroppedZombie->DrawReanim(&aDropGraphics, aDroppedDrawPos, RENDER_GROUP_NORMAL);
    }
    else
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mTargetPlantID);
        if (aPlant)
        {
            Graphics aPlantGraphics(*g);
            aPlantGraphics.mTransY += 30.0f - mAltitude;
            if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_RISING)
            {
                if (aPlant->mSeedType == SeedType::SEED_SPIKEWEED || aPlant->mSeedType == SeedType::SEED_SPIKEROCK)
                {
                    aPlantGraphics.mTransY -= 34.0f;
                }
            }
            if (aPlant->mPlantCol <= 4 && mBoard->StageHasRoof())
            {
                aPlantGraphics.mTransY += 10;
            }

            aPlant->Draw(&aPlantGraphics);
        }
    }

    aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_ARMS);
}

//0x52CCA0
void Zombie::DrawBungeeTarget(Graphics* g)
{
    if (!IsOnBoard() || mApp->IsFinalBossLevel())
        return;
    
    if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_HIT_OUCHY || mZombiePhase == ZombiePhase::PHASE_BUNGEE_RISING)
        return;
    
    if (mRelatedZombieID != ReanimationID::REANIMATIONID_NULL)
        return;
    
    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    float aTargetX = mX + 10.0f;
    float aTargetY = mY + 60.0f + aDrawPos.mBodyY + aDrawPos.mImageOffsetY;
    if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING || mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING_SCREAMING)
    {
        aTargetX += TodAnimateCurveFloat(BUNGEE_ZOMBIE_HEIGHT, BUNGEE_ZOMBIE_HEIGHT - 400, (int)mAltitude, 30.0f, 0.0f, TodCurves::CURVE_LINEAR);
        aTargetY += TodAnimateCurveFloat(BUNGEE_ZOMBIE_HEIGHT, BUNGEE_ZOMBIE_HEIGHT - 400, (int)mAltitude, -600.0f, 0.0f, TodCurves::CURVE_LINEAR);
    }

    g->DrawImageF(IMAGE_BUNGEETARGET, aTargetX, aTargetY + mAltitude);
}

//0x52CDF0
void Zombie::DrawDancerReanim(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    Color aSpotLightColor;
    bool aDrawSpotLight = false;
    if (mZombiePhase != ZombiePhase::PHASE_DANCER_DANCING_IN && mZombiePhase != ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS && 
        mZombiePhase != ZombiePhase::PHASE_ZOMBIE_NORMAL && mZombiePhase != ZombiePhase::PHASE_ZOMBIE_DYING && mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON)
    {
        aDrawSpotLight = true;

        switch (mZombieAge >= 700 ? mZombieAge / 100 * 7 % 5 : 0)
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
        TodDrawImageScaledF(g, IMAGE_SPOTLIGHT2, -30.0f, 60.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), 4.0f, 4.0f);
        g->SetColorizeImages(false);
    }

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    aBodyReanim->Draw(g);

    if (aDrawSpotLight)
    {
        g->SetColorizeImages(true);
        g->SetColor(aSpotLightColor);
        TodDrawImageScaledF(g, IMAGE_SPOTLIGHT, -30.0f, -480.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), 4.0f, 4.0f);
        g->SetColorizeImages(false);
    }
}

//0x52D020
void Zombie::DrawReanim(Graphics* g, const ZombieDrawPosition& theDrawPos, int theBaseRenderGroup)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
    {
        TodTrace("Missing zombie reanimation");
        return;
    }

    if (theDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT)
    {
        float aDrawHeight = 120.0f - theDrawPos.mClipHeight + 71.0f;
        g->SetClipRect(theDrawPos.mImageOffsetX - 200.0f, theDrawPos.mImageOffsetY + theDrawPos.mBodyY - 78.0f, 520, aDrawHeight);
    }

    int aFadeAlpha = 255;
    if (mZombieFade >= 0)
    {
        aFadeAlpha = ClampInt(255 * mZombieFade / 10, 0, 255);
    }

    Color aColorOverride(255, 255, 255, aFadeAlpha);
    Color aExtraAdditiveColor = Color::Black;
    bool aEnableExtraAdditiveDraw = false;


    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
    {
        aColorOverride = Color(0, 0, 0, aFadeAlpha);
        aExtraAdditiveColor = Color::Black;
        aEnableExtraAdditiveDraw = false;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BOSS && mZombiePhase != ZombiePhase::PHASE_ZOMBIE_DYING && mBodyHealth < mBodyMaxHealth / BOSS_FLASH_HEALTH_FRACTION)
    {
        int aGrayness = TodAnimateCurve(0, 39, mBoard->mMainCounter % 40, 155, 255, TodCurves::CURVE_BOUNCE);
        if (mChilledCounter > 0 || mIceTrapCounter > 0)
        {
            int aColdColor = TodAnimateCurve(0, 39, mBoard->mMainCounter % 40, 65, 75, TodCurves::CURVE_BOUNCE);
            aColorOverride = Color(aColdColor, aColdColor, aGrayness, aFadeAlpha);
        }
        else
        {
            aColorOverride = Color(aGrayness, aGrayness, aGrayness, aFadeAlpha);
        }

        aExtraAdditiveColor = Color::Black;
        aEnableExtraAdditiveDraw = false;
    }
    else if (mMindControlled)
    {
        aColorOverride = ZOMBIE_MINDCONTROLLED_COLOR;
        aColorOverride.mAlpha = aFadeAlpha;
        aExtraAdditiveColor = aColorOverride;
        aEnableExtraAdditiveDraw = true;
    }
    else if (mChilledCounter > 0 || mIceTrapCounter > 0)
    {
        aColorOverride = Color(75, 75, 255, aFadeAlpha);
        aExtraAdditiveColor = aColorOverride;
        aEnableExtraAdditiveDraw = true;
    }
    else if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM && mBodyHealth < 100)
    {
        aColorOverride = Color(100, 150, 25, aFadeAlpha);
        aExtraAdditiveColor = aColorOverride;
        aEnableExtraAdditiveDraw = true;
    }
    if (mJustGotShotCounter > 0 && !IsBobsledTeamWithSled())
    {
        int aGrayness = mJustGotShotCounter * 10;
        Color aHighlightColor(aGrayness, aGrayness, aGrayness, 255);
        aExtraAdditiveColor = ColorAdd(aHighlightColor, aExtraAdditiveColor);
        aEnableExtraAdditiveDraw = true;
    }
    
    /*if (diarrheaCount > 0) {
        float aGreenness = diarrheaCount * 1.275f;
        Color aHighlightColor(0, aGreenness / 2.0f, 0, 255);
        aExtraAdditiveColor = ColorAdd(aHighlightColor, aExtraAdditiveColor);
        aEnableExtraAdditiveDraw = true;
        aBodyReanim->mColorOverride = Color(255 - diarrheaCount / 1.5f, 255, 255 - diarrheaCount / 1.5f, 255);
        aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
        aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;
        aBodyReanim->DrawRenderGroup(g, theBaseRenderGroup);
    }*/

    aBodyReanim->mColorOverride = aColorOverride;
    aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
    aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;

    if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
    {
        DrawBobsledReanim(g, theDrawPos, true);
        aBodyReanim->DrawRenderGroup(g, theBaseRenderGroup);
        DrawBobsledReanim(g, theDrawPos, false);
    }
    else if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
    {
        DrawBungeeReanim(g, theDrawPos);
    }
    else if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER)
    {
        DrawDancerReanim(g, theDrawPos);
    }
    else
    {
        aBodyReanim->DrawRenderGroup(g, theBaseRenderGroup);
    }

    if (mShieldType != ShieldType::SHIELDTYPE_NONE)
    {
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
        {
            aColorOverride = Color(0, 0, 0, aFadeAlpha);
            aExtraAdditiveColor = Color::Black;
            aEnableExtraAdditiveDraw = false;
        }
        else if (mMindControlled)
        {
            aColorOverride = ZOMBIE_MINDCONTROLLED_COLOR;
            aColorOverride.mAlpha = aFadeAlpha;
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }
        else if (mChilledCounter > 0 || mIceTrapCounter > 0)
        {
            aColorOverride = Color(75, 75, 255, aFadeAlpha);
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }
        if (mShieldJustGotShotCounter > 0)
        {
            int aGrayness = mShieldJustGotShotCounter * 10;
            Color aHighlightColor(aGrayness, aGrayness, aGrayness, 255);
            aExtraAdditiveColor = ColorAdd(aHighlightColor, aExtraAdditiveColor);
            aEnableExtraAdditiveDraw = true;
        }

        aBodyReanim->mColorOverride = aColorOverride;
        aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
        aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;
        
    
        float aShieldHitOffset = 0.0f;
        if (mShieldRecoilCounter > 0)
            aShieldHitOffset = TodAnimateCurveFloat(12, 0, mShieldRecoilCounter, 3.0f, 0.0f, TodCurves::CURVE_LINEAR);

        g->mTransX += aShieldHitOffset;
       
        aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_SHIELD);

        g->mTransX -= aShieldHitOffset;
    }

    if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER || mShieldType == ShieldType::SHIELDTYPE_DOOR || mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        aBodyReanim->mColorOverride = aColorOverride;
        aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
        aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;
        aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_OVER_SHIELD);
    }

    if (mShieldType != ShieldType::SHIELDTYPE_NONE)
    {
        aColorOverride = Color(255, 255, 255, aFadeAlpha);
        aExtraAdditiveColor = Color::Black;
        aEnableExtraAdditiveDraw = false;

        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED){
            aColorOverride = Color(0, 0, 0, aFadeAlpha);
            aExtraAdditiveColor = Color::Black;
            aEnableExtraAdditiveDraw = false;
        }
        else if (mMindControlled){
            aColorOverride = ZOMBIE_MINDCONTROLLED_COLOR;
            aColorOverride.mAlpha = aFadeAlpha;
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }
        else if (mChilledCounter > 0 || mIceTrapCounter > 0){
            aColorOverride = Color(75, 75, 255, aFadeAlpha);
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }

        if (mJustGotShotCounter > 0)
        {
            int aGrayness = mJustGotShotCounter * 10;
            Color aHighlightColor(aGrayness, aGrayness, aGrayness, 255);
            aExtraAdditiveColor = ColorAdd(aHighlightColor, aExtraAdditiveColor);
            aEnableExtraAdditiveDraw = true;
        }

        aBodyReanim->mColorOverride = aColorOverride;
        aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
        aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;
        aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_SHIELD_ARM);
    }

    if (mHelmType != HELMTYPE_NONE && mHelmType != HELMTYPE_WALLNUT && mHelmType != HELMTYPE_TALLNUT) {
        if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
        {
            aColorOverride = Color(0, 0, 0, aFadeAlpha);
            aExtraAdditiveColor = Color::Black;
            aEnableExtraAdditiveDraw = false;
        }
        else if (mMindControlled)
        {
            aColorOverride = ZOMBIE_MINDCONTROLLED_COLOR;
            aColorOverride.mAlpha = aFadeAlpha;
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }
        else if (mChilledCounter > 0 || mIceTrapCounter > 0)
        {
            aColorOverride = Color(75, 75, 255, aFadeAlpha);
            aExtraAdditiveColor = aColorOverride;
            aEnableExtraAdditiveDraw = true;
        }
        if (mHelmetJustGotShotCounter > 0)
        {
            int aGrayness = mHelmetJustGotShotCounter * 10;
            Color aHighlightColor(aGrayness, aGrayness, aGrayness, 255);
            aExtraAdditiveColor = ColorAdd(aHighlightColor, aExtraAdditiveColor);
            aEnableExtraAdditiveDraw = true;
        }

        aBodyReanim->mColorOverride = aColorOverride;
        aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
        aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;

        if ((mHelmType == HELMTYPE_DIVER || mHelmType == HELMTYPE_TRAFFIC_CONE || mHelmType == HELMTYPE_FOOTBALL || mHelmType == HELMTYPE_BLACKFOOTBALL || mHelmType == HELMTYPE_PAIL || mHelmType == HELMTYPE_DIGGER) && mHelmType != HELMTYPE_WALLNUT && mHelmType != HELMTYPE_TALLNUT)
        {
            float aHelmetHitOffset = 0.0f;
            if (mHelmetRecoilCounter > 0)
                aHelmetHitOffset = TodAnimateCurveFloat(12, 0, mHelmetRecoilCounter, 3.0f, 0.0f, TodCurves::CURVE_LINEAR);

            g->mTransX += aHelmetHitOffset;

            aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_HELMET);
            aBodyReanim->mColorOverride = aColorOverride;
            aBodyReanim->mExtraAdditiveColor = aExtraAdditiveColor;
            aBodyReanim->mEnableExtraAdditiveDraw = aEnableExtraAdditiveDraw;
            aBodyReanim->DrawRenderGroup(g, RENDER_GROUP_OVER_HELMET);

            g->mTransX -= aHelmetHitOffset;
        }
    }

    g->ClearClipRect();
}

//0x52D6D0
int Zombie::GetHelmDamageIndex()
{
    if (mHelmHealth < mHelmMaxHealth / 3)
    {
        return 2;
    }

    if (mHelmHealth < mHelmMaxHealth * 2 / 3)
    {
        return 1;
    }

    return 0;
}

//0x52D710
int Zombie::GetBodyDamageIndex(){
    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        if (mBodyHealth < mBodyMaxHealth / 2)
        {
            return 2;
        }

        if (mBodyHealth < mBodyMaxHealth * 4 / 5)
        {
            return 1;
        }

        return 0;
    }
    else if (mZombieType == ZOMBIE_FLAG)
    {
        if (mBodyHealth < mBodyMaxHealth * 1.5f / 3)
        {
            return 2;
        }

        if (mBodyHealth < mBodyMaxHealth * 2.5f / 3)
        {
            return 1;
        }

        return 0;
    }
     else
     {
         if (mBodyHealth < mBodyMaxHealth / 3)
         {
             return 2;
         }

         if (mBodyHealth < mBodyMaxHealth * 2 / 3)
         {
             return 1;
         }

         return 0;
    }
}

//0x52D780
int Zombie::GetShieldDamageIndex()
{
    if (mShieldHealth < mShieldMaxHealth / 3)
    {
        return 2;
    }

    if (mShieldHealth < mShieldMaxHealth * 2 / 3)
    {
        return 1;
    }

    return 0;
}

//0x52D7C0
void Zombie::DrawBungeeCord(Graphics* g, int theOffsetX, int theOffsetY)
{
    if (mZombiePhase == PHASE_ZOMBIE_BURNED)
        return;

    int aCordCelHeight = IMAGE_BUNGEECORD->GetCelHeight() * mScaleZombie;
    float aPosX, aPosY;
    GetTrackPosition("Zombie_bungi_body", aPosX, aPosY);

    bool aSetClip = false;
    if (IsOnBoard() && mApp->IsFinalBossLevel())
    {
        Zombie* aBoss = mBoard->GetBossZombie();

        int aClipAmount = 55;
        if (aBoss->mZombiePhase == ZombiePhase::PHASE_BOSS_BUNGEES_LEAVE)
        {
            Reanimation* aBossReanim = mApp->ReanimationGet(aBoss->mBodyReanimID);
            aClipAmount = TodAnimateCurveFloatTime(0.0f, 0.2f, aBossReanim->mAnimTime, 55.0f, 0.0f, TodCurves::CURVE_LINEAR);
        }

        if (mTargetCol > aBoss->mTargetCol)
        {
            g->SetClipRect(Rect(-g->mTransX, aClipAmount - g->mTransY, BOARD_WIDTH, BOARD_HEIGHT));
            aSetClip = true;
        }
    }

    for (float y = aPosY - aCordCelHeight; y > -aCordCelHeight - BOARD_OFFSET_Y; y -= aCordCelHeight)
    {
        TodDrawImageScaledF(g, IMAGE_BUNGEECORD, theOffsetX + 61.0f - 4.0f / mScaleZombie, y - mPosY, mScaleZombie, mScaleZombie);
    }

    if (aSetClip)
    {
        g->ClearClipRect();
    }
}

//0x52D9E0
void Zombie::GetDrawPos(ZombieDrawPosition& theDrawPos)
{
    theDrawPos.mImageOffsetX = mPosX - mX;
    theDrawPos.mImageOffsetY = mPosY - mY;

    if (mIsEating)
    {
        theDrawPos.mHeadX = 47;
        theDrawPos.mHeadY = 4;
    }
    else
    {
        switch (mFrame)
        {
        case 0:
            theDrawPos.mHeadX = 50;
            theDrawPos.mHeadY = 2;
            break;
        case 1:
            theDrawPos.mHeadX = 49;
            theDrawPos.mHeadY = 1;
            break;
        case 2:
            theDrawPos.mHeadX = 49;
            theDrawPos.mHeadY = 2;
            break;
        case 3:
            theDrawPos.mHeadX = 48;
            theDrawPos.mHeadY = 4;
            break;
        case 4:
            theDrawPos.mHeadX = 48;
            theDrawPos.mHeadY = 5;
            break;
        case 5:
            theDrawPos.mHeadX = 48;
            theDrawPos.mHeadY = 4;
            break;
        case 6:
            theDrawPos.mHeadX = 48;
            theDrawPos.mHeadY = 2;
            break;
        case 7:
            theDrawPos.mHeadX = 49;
            theDrawPos.mHeadY = 1;
            break;
        case 8:
            theDrawPos.mHeadX = 49;
            theDrawPos.mHeadY = 2;
            break;
        case 9:
            theDrawPos.mHeadX = 50;
            theDrawPos.mHeadY = 4;
            break;
        case 10:
            theDrawPos.mHeadX = 50;
            theDrawPos.mHeadY = 5;
            break;
        default:
            theDrawPos.mHeadX = 50;
            theDrawPos.mHeadY = 4;
            break;
        }
    }

    theDrawPos.mArmY = theDrawPos.mHeadY / 2;

    switch (mZombieType)
    {
    case ZombieType::ZOMBIE_BLACK_FOOTBALL:
    case ZombieType::ZOMBIE_FOOTBALL:
        theDrawPos.mImageOffsetY -= 16.0f;
        break;
    case ZombieType::ZOMBIE_YETI:
        theDrawPos.mImageOffsetY -= 20.0f;
        break;
    case ZombieType::ZOMBIE_CATAPULT:
        theDrawPos.mImageOffsetX -= 25.0f;
        theDrawPos.mImageOffsetY -= 18.0f;
        break;
    case ZombieType::ZOMBIE_POGO:
        theDrawPos.mImageOffsetY += 16.0f;
        break;
    case ZombieType::ZOMBIE_BALLOON:
        theDrawPos.mImageOffsetY += 17.0f;
        break;
    case ZombieType::ZOMBIE_POLEVAULTER:
        theDrawPos.mImageOffsetX -= 6.0f;
        theDrawPos.mImageOffsetY -= 11.0f;
        break;
    case ZombieType::ZOMBIE_ZAMBONI:
        theDrawPos.mImageOffsetX += 68.0f;
        theDrawPos.mImageOffsetY -= 23.0f;
        break;
    case ZombieType::ZOMBIE_GARGANTUAR:
    case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
        theDrawPos.mImageOffsetY -= 8.0f;
        break;
    case ZombieType::ZOMBIE_BOBSLED:
        theDrawPos.mImageOffsetY -= 12.0f;
        break;
    }

    if (mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE)
    {
        theDrawPos.mBodyY = -mAltitude;

        if (mInPool)
        {
            theDrawPos.mClipHeight = theDrawPos.mBodyY;
        }
        else
        {
            float aHeightLimit = min(mPhaseCounter, 40.0f);
            theDrawPos.mClipHeight = theDrawPos.mBodyY + aHeightLimit;
        }

        if (IsOnHighGround())
        {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }

        return;
    }
    
    if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER)
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;

        if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

            if (aBodyReanim->mAnimTime >= 0.56f && aBodyReanim->mAnimTime <= 0.65f)  // 跳上海豚的起跳过程
            {
                theDrawPos.mClipHeight = 0.0f;
            }
            else if (aBodyReanim->mAnimTime >= 0.75f)  // 跳上海豚的下落过程
            {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            }
        }
        else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
        {
            theDrawPos.mImageOffsetX += 70.0f;  // 额外 70 像素的横坐标偏移用于弥补跳上海豚后的 mPosX -= 70.0f

            if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER)
            {
                theDrawPos.mClipHeight = -mAltitude - 15.0f;
            }
            else
            {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            }
        }
        else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP)
        {
            theDrawPos.mImageOffsetX += 70.0f + mAltitude;

            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            if (aBodyReanim->mAnimTime <= 0.06f)  // 起跳出水之前
            {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            }
            else if (aBodyReanim->mAnimTime >= 0.5f && aBodyReanim->mAnimTime <= 0.76f) // 起跳过程中（脱离水面后至重新入水前）
            {
                theDrawPos.mClipHeight = -13.0f;
            }
        }
        else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING && mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
        {
            theDrawPos.mImageOffsetY += 50.0f;  // 额外 50 像素的横坐标偏移用于弥补跳跃过程中前进的距离

            if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING)
            {
                theDrawPos.mClipHeight = -mAltitude + 44.0f;
            }
            else if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER)
            {
                theDrawPos.mClipHeight = -mAltitude + 36.0f;
            }
        }
        else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING && mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL)
        {
            theDrawPos.mClipHeight = -mAltitude;
        }
        else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN && mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL)
        {
            theDrawPos.mClipHeight = -mAltitude;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;

        if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            if (aBodyReanim->mAnimTime >= 0.8f)  // 入水后
            {
                theDrawPos.mClipHeight = -10.0f;
            }
        }
        else if (mInPool)
        {
            theDrawPos.mClipHeight = -mAltitude - 5.0f;
            theDrawPos.mClipHeight += 20.0f - 20.0f * mScaleZombie;
        }
    }
    else if (mInPool)
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = -mAltitude - 7.0f;
        theDrawPos.mClipHeight += 10.0f - 10.0f * mScaleZombie;

        if (mIsEating)
        {
            theDrawPos.mClipHeight += 7.0f;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = -mAltitude;

        if (IsOnHighGround())
        {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE)
    {
        theDrawPos.mBodyY = -mAltitude;

        if (mPhaseCounter > 20)
        {
            theDrawPos.mClipHeight = -mAltitude;
        }
        else
        {
            theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mImageOffsetX -= 18.0f;

        if (IsOnHighGround())
        {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }

        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
    }
    else
    {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
    }
}

//0x52DF90
int Zombie::GetDancerFrame()
{
    if (mFromWave == Zombie::ZOMBIE_WAVE_UI || IsImmobilizied())
        return 0;

    int aFrameLength = 20;
    int aFramesCount = 23;
    if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN)
    {
        aFramesCount = 11;
        aFrameLength = 10;
    }

//#ifdef DO_FIX_BUGS
    if (mBoard)
    {
        return (mBoard->mMainCounter % (aFrameLength * aFramesCount)) / aFrameLength;  // 修复“女仆秘籍”
    }
    else
    {
        return (mApp->mAppCounter % (aFrameLength * aFramesCount)) / aFrameLength;
    }
/*#else
    return (mApp->mAppCounter % (aFrameLength * aFramesCount)) / aFrameLength;
#endif*/
}

//0x52DFE0
ZombiePhase Zombie::GetDancerPhase()
{
    int aFrame = GetDancerFrame();

    if (mZombieType == ZOMBIE_DISCO || mZombieType == ZOMBIE_DISCO_BACKUP) {
        return
            aFrame <= 11 ? ZombiePhase::PHASE_DANCER_DANCING_LEFT :
            aFrame <= 12 ? ZombiePhase::PHASE_DANCER_WALK_TO_RAISE :
            /*aFrame <= 15 ? ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1 :*/
            aFrame <= 18 ? ZombiePhase::PHASE_DANCER_RAISE_LEFT_1 :
            /*aFrame <= 21 ? ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2 :*/
            ZombiePhase::PHASE_DANCER_RAISE_LEFT_2;
    }

    return
        aFrame <= 11 ? ZombiePhase::PHASE_DANCER_DANCING_LEFT :
        aFrame <= 12 ? ZombiePhase::PHASE_DANCER_WALK_TO_RAISE :
        aFrame <= 15 ? ZombiePhase::PHASE_DANCER_RAISE_RIGHT_1 :
        aFrame <= 18 ? ZombiePhase::PHASE_DANCER_RAISE_LEFT_1 :
        aFrame <= 21 ? ZombiePhase::PHASE_DANCER_RAISE_RIGHT_2 : ZombiePhase::PHASE_DANCER_RAISE_LEFT_2;
}

//0x52E020
void Zombie::DrawIceTrap(Graphics* g, const ZombieDrawPosition& theDrawPos, bool theFront)
{
    if (mInPool || mZombieType == ZombieType::ZOMBIE_BOSS)
        return;

    float aOffsetX = 46.0f;
    float aOffsetY = theDrawPos.mBodyY + 92.0f;
    float aScale = 1.0f;
    switch (mZombieType)
    {
    case ZombieType::ZOMBIE_POGO:
        aOffsetX -= 10.0f;
        aOffsetY += 20.0f;
        break;
    case ZombieType::ZOMBIE_GARGANTUAR:
    case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
        aOffsetX -= 20.0f;
        aOffsetY -= 7.0f;
        aScale = 1.6f;
        break;
    case ZombieType::ZOMBIE_BUNGEE:
        aOffsetX -= 45.0f;
        aOffsetY -= 23.0f;
        aScale = 1.2f;
        break;
    case ZombieType::ZOMBIE_DIGGER:
        aOffsetX -= 27.0f;
        break;
    case ZombieType::ZOMBIE_CATAPULT:
        aOffsetX += 32.0f;
        break;
    case ZombieType::ZOMBIE_BALLOON:
        aOffsetX -= 9.0f;
        aOffsetY += 27.0f;
        break;
    }

    TodDrawImageScaledF(g, theFront ? IMAGE_ICETRAP : IMAGE_ICETRAP2, aOffsetX, aOffsetY, aScale, aScale);
}

//0x52E150
void Zombie::DrawButter(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    float aOffsetX = mPosX + theDrawPos.mImageOffsetX + theDrawPos.mHeadX + 11.0f;
    float aOffsetY = mPosY + theDrawPos.mImageOffsetY + theDrawPos.mHeadY + theDrawPos.mBodyY + 21.0f;
    float aScale = 1.0f;
    if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MADDENING)
    {
        GetTrackPosition("anim_head_look", aOffsetX, aOffsetY);
    }
    else if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        GetTrackPosition("Zombie_catapult_driver_head", aOffsetX, aOffsetY);
    }
    else if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        GetTrackPosition("anim_head1", aOffsetX, aOffsetY);
    }
    aOffsetX -= mPosX + 29.0f;
    aOffsetY -= mPosY + 36.0f;

    switch (mZombieType)
    {
    case ZombieType::ZOMBIE_POGO:
        aOffsetY -= 5.0f;
        break;
    case ZombieType::ZOMBIE_GARGANTUAR:
    case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
        aOffsetX -= 5.0f;
        aOffsetY -= 15.0f;
        aScale = 1.2f;
        break;
    case ZombieType::ZOMBIE_SQUASH_HEAD:
        aOffsetX += 6.0f;
        aOffsetY -= 9.0f;
        break;
    case ZombieType::ZOMBIE_WALLNUT_HEAD:
        aOffsetX -= 6.0f;
        aOffsetY -= 1.0f;
        break;
    case ZombieType::ZOMBIE_TALLNUT_HEAD:
        aOffsetX -= 24.0f;
        aOffsetY -= 39.0f;
        break;
    }

    TodDrawImageScaledF(g, IMAGE_REANIM_CORNPULT_BUTTER_SPLAT, aOffsetX, aOffsetY, aScale, aScale);
}

//0x52E2E0
void Zombie::Draw(Graphics* g)
{
    if (mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED)
        return;

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);

    if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON && !SetupDrawZombieWon(g))
        return;

    if (mIceTrapCounter > 0)
    {
        DrawIceTrap(g, aDrawPos, false);
    }
    if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL || mFromWave == Zombie::ZOMBIE_WAVE_UI)
    {
        if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
        {
            DrawReanim(g, aDrawPos, RENDER_GROUP_NORMAL);
        }
        else
        {
            DrawZombie(g, aDrawPos);
        }
    }
    if (mIceTrapCounter > 0)
    {
        DrawIceTrap(g, aDrawPos, true);
    }
    if (mButteredCounter > 0)
    {
        DrawButter(g, aDrawPos);
    }

    if (mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
    {
        Graphics theParticleGraphics(*g);
        MakeParentGraphicsFrame(&theParticleGraphics);
        theParticleGraphics.mTransY += aDrawPos.mBodyY;

        if (aDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT)
        {
            float aDrawHeight = 120.0f - aDrawPos.mClipHeight + 21.0f;
            theParticleGraphics.ClipRect(mX + aDrawPos.mImageOffsetX - 400.0f, mY + aDrawPos.mImageOffsetY - 28.0f, 920, aDrawHeight);
        }

        AttachmentDraw(mAttachmentID, &theParticleGraphics, false);
    }

    g->ClearClipRect();
}

//0x52E4C0
bool Zombie::CanTargetPlant(Plant* thePlant, ZombieAttackType theAttackType)
{
    if ((mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && (thePlant->mSeedType == SEED_WALLNUT || thePlant->mSeedType == SEED_ARTIC_NUT || thePlant->mSeedType == SEED_GIANT_WALLNUT || thePlant->mSeedType == SEED_EXPLODE_O_NUT)) && theAttackType != ZombieAttackType::ATTACKTYPE_VAULT)
        return false;

    if (thePlant->mSeedType == SEED_CHERRYHOVERBOMB || thePlant->mSeedType == SEED_ICEBERG_LETTUCE)
        return false;

    if (thePlant->NotOnGround() || thePlant->mSeedType == SeedType::SEED_TANGLEKELP)
        return false;

    if (thePlant->mState == STATE_BURNED)
        return false;

    if (!mInPool && mBoard->IsPoolSquare(thePlant->mPlantCol, thePlant->mRow))
        return false;

    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
    {
        return thePlant->mSeedType == SeedType::SEED_POTATOMINE && thePlant->mState == PlantState::STATE_NOTREADY;
    }

    if (thePlant->IsSpiky())
    {
        return 
            mZombieType == ZombieType::ZOMBIE_GARGANTUAR || 
            mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || 
            mZombieType == ZombieType::ZOMBIE_ZAMBONI || 
            mBoard->IsPoolSquare(thePlant->mPlantCol, thePlant->mRow) || 
            mBoard->GetFlowerPotAt(thePlant->mPlantCol, thePlant->mRow) ||
            mBoard->GetWaterPotAt(thePlant->mPlantCol, thePlant->mRow);  // 扶梯僵尸给花盆上的地刺/地刺王搭梯的原理
    }

    if (theAttackType == ZombieAttackType::ATTACKTYPE_DRIVE_OVER)
    {
        if (thePlant->mSeedType == SeedType::SEED_CHERRYBOMB || thePlant->mSeedType == SeedType::SEED_JALAPENO || 
            thePlant->mSeedType == SeedType::SEED_BLOVER || thePlant->mSeedType == SeedType::SEED_SQUASH)
        {
            return false;
        }
        if (thePlant->mSeedType == SeedType::SEED_DOOMSHROOM || thePlant->mSeedType == SeedType::SEED_ICESHROOM)
        {
            return thePlant->mIsAsleep;
        }
    }

    if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING || mZombiePhase == ZombiePhase::PHASE_LADDER_PLACING)
    {
        bool aPlaceLadder = false;
        if (thePlant->mSeedType == SeedType::SEED_WALLNUT || thePlant->mSeedType == SeedType::SEED_ARTIC_NUT || thePlant->mSeedType == SeedType::SEED_EXPLODE_O_NUT || thePlant->mSeedType == SeedType::SEED_FIRESHROOM || thePlant->mSeedType == SeedType::SEED_TALLNUT ||  thePlant->mSeedType == SeedType::SEED_GIANT_WALLNUT || thePlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
        {
            aPlaceLadder = true;
        }

        if (mBoard->GetLadderAt(thePlant->mPlantCol, thePlant->mRow))
        {
            aPlaceLadder = false;
        }

        if ((theAttackType == ZombieAttackType::ATTACKTYPE_CHEW && aPlaceLadder) || (theAttackType == ZombieAttackType::ATTACKTYPE_LADDER && !aPlaceLadder))
        {
            return false;
        }
    }

    if (theAttackType == ZombieAttackType::ATTACKTYPE_CHEW)
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_EATING_ORDER);
        if (aTopPlant != thePlant && aTopPlant && CanTargetPlant(aTopPlant, theAttackType))
        {
            return false;
        }
    }

    if (theAttackType == ZombieAttackType::ATTACKTYPE_VAULT)
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        if (aTopPlant != thePlant && aTopPlant && CanTargetPlant(aTopPlant, theAttackType))
        {
            return false;
        }
    }

    return true;
}

//0x52E780
Plant* Zombie::FindPlantTarget(ZombieAttackType theAttackType)
{
    Rect aAttackRect = GetZombieAttackRect();

    for (int i = 0; i < mBoard->mMaxPlayers; i++) {
        PlantID playerPlantID = mBoard->mPlayers[i]->mPlayerPlantID;

        if (playerPlantID != PLANTID_NULL) {
            Plant* mPlayerPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int)playerPlantID);
            if (mPlayerPlant) {
                if (mPlayerPlant->mEnemy != mMindControlled || mPlayerPlant->mMindControlled != mMindControlled)
                    continue;

                if (mPlayerPlant->mRow == mRow)
                {
                    Rect aPlantRect = mPlayerPlant->GetPlantRect();
                    if (GetRectOverlap(aAttackRect, aPlantRect) >= 20 && CanTargetPlant(mPlayerPlant, theAttackType))
                    {
                        return mPlayerPlant;
                    }
                }
            }
        }
    }

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (mBoard->mBackground == BACKGROUND_HIGHGROUND)
        {
            if (IsOnHighGround() != aPlant->IsOnHighGround())
                continue;
        }

        //FUN CUSTOM


        if (aPlant->mEnemy != mMindControlled || aPlant->mMindControlled != mMindControlled)
            continue;

        if (aPlant->mRow == mRow)
        {
            Rect aPlantRect = aPlant->GetPlantRect();
            if (GetRectOverlap(aAttackRect, aPlantRect) >= 20 && CanTargetPlant(aPlant, theAttackType))
            {
                return aPlant;
            }
        }
    }

    return nullptr;
}

//0x52E840
Zombie* Zombie::FindZombieTarget()
{
    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
        return nullptr;

    Rect aAttackRect = GetZombieAttackRect();

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (mMindControlled != aZombie->mMindControlled && 
            !aZombie->IsFlying() && 
            aZombie->mZombiePhase != ZombiePhase::PHASE_DIGGER_TUNNELING && 
            aZombie->mZombiePhase != ZombiePhase::PHASE_BUNGEE_DIVING && 
            aZombie->mZombiePhase != ZombiePhase::PHASE_BUNGEE_DIVING_SCREAMING && 
            aZombie->mZombiePhase != ZombiePhase::PHASE_BUNGEE_RISING && 
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_ENTER &&
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_IDLE &&
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_DROP_RV &&
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_BUNGEES_ENTER &&
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_BUNGEES_DROP &&
            aZombie->mZombiePhase != ZombiePhase::PHASE_BOSS_BUNGEES_LEAVE &&
            aZombie->mZombieHeight != ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED && 
            !aZombie->IsDeadOrDying() && 
            (aZombie->mRow == mRow || aZombie->mZombieType == ZOMBIE_BOSS))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aOverlap = GetRectOverlap(aAttackRect, aZombieRect);
            if (aOverlap >= 20 || (aOverlap > 0 && aZombie->mIsEating))
            {
                return aZombie;
            }
        }
    }

    return nullptr;
}

//0x52E920
void Zombie::SquishAllInSquare(int theX, int theY, ZombieAttackType theAttackType)
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == theY && aPlant->mPlantCol == theX)
        {
            if (theAttackType == ZombieAttackType::ATTACKTYPE_DRIVE_OVER && aPlant->IsSpiky())
            {
                continue;
            }

            if (aPlant->mSeedType != SeedType::SEED_SPIKEROCK)
            {
                mBoard->mPlantsEaten++;
                aPlant->Squish();
            }
        }
    }

    for (int i = 0; i < mBoard->mMaxPlayers; i++) {
        PlantID playerPlantID = mBoard->mPlayers[i]->mPlayerPlantID;

        if (playerPlantID != PLANTID_NULL) {
            Plant* mPlayerPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int)playerPlantID);
            if (mPlayerPlant && mPlayerPlant->mRow == theY && mPlayerPlant->mPlantCol == theX &&
                theAttackType != ZombieAttackType::ATTACKTYPE_DRIVE_OVER && mPlayerPlant->IsSpiky() && mPlayerPlant->mSeedType != SeedType::SEED_SPIKEROCK) {
                mBoard->mPlantsEaten++;
                mPlayerPlant->Die();
            }
        }
    }
}

//0x52E9A0
void Zombie::ZamboniDeath(unsigned int theDamageFlags)
{
    if (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE))
    {
        mFlatTires = true;
        mApp->PlayFoley(FoleyType::FOLEY_TIRE_POP);
        mZombiePhase = ZombiePhase::PHASE_ZOMBIE_DYING;
        mApp->AddTodParticle(mPosX + 29.0f, mPosY + 114.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_TIRE);
        mVelX = 0.0f;

        if (Rand(4) == 0 && mPosX < 600.0f)
        {
            PlayZombieReanim("anim_wheelie2", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 10.0f);
            mPhaseCounter = 280;
        }
        else
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            TodParticleSystem* aParticle = mApp->AddTodParticle(0.0f, 0.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), 0, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
            if (aParticle)
            {
                aBodyReanim->AttachParticleToTrack("zombie_zamboni_1", aParticle, 35.0f, 85.0f);
            }

            mPhaseCounter = 280;
            PlayZombieReanim("anim_wheelie1", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
        }
    }
    else
    {
        mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_EXPLOSION);
        DieWithLoot();
        mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
    }
}

//0x52EC00
void Zombie::CatapultDeath(unsigned int theDamageFlags)
{
    if (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE))
    {
        mApp->PlayFoley(FoleyType::FOLEY_TIRE_POP);
        mZombiePhase = ZombiePhase::PHASE_ZOMBIE_DYING;
        mApp->AddTodParticle(mPosX + 29.0f, mPosY + 114.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_TIRE);
        mVelX = 0.0f;

        AddAttachedParticle(47, 77, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
        mPhaseCounter = 280;
        PlayZombieReanim("anim_bounce", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
    }
    else
    {
        mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_CATAPULT_EXPLOSION);
        DieWithLoot();
        mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
    }
}

//0x52EDB0
void Zombie::CheckSquish(ZombieAttackType theAttackType)
{
    Rect aAttackRect = GetZombieAttackRect();

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow)
        {
            Rect aPlantRect = aPlant->GetPlantRect();
            if (GetRectOverlap(aAttackRect, aPlantRect) >= 20 && CanTargetPlant(aPlant, theAttackType) && !aPlant->IsSpiky())
            {
                SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, theAttackType);
                break;
            }
        }
    }

    if (mApp->IsIZombieLevel())
    {
        GridItem* aBrain = mBoard->mChallenge->IZombieGetBrainTarget(this);
        if (aBrain)
        {
            mBoard->mChallenge->IZombieSquishBrain(aBrain);
        }
    }
}

//0x52EEF0
bool Zombie::IsImmobilizied()
{
    return mIceTrapCounter > 0 || mButteredCounter > 0;
}

//0x52EF10
bool Zombie::IsMovingAtChilledSpeed()
{
    if (mChilledCounter > 0)
        return true;

    if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        Zombie* aLeader;
        if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER)
        {
            aLeader = this;
        }
        else
        {
            aLeader = mBoard->ZombieTryToGet(mRelatedZombieID);
        }

        if (aLeader)
        {
            if (aLeader->mChilledCounter > 0)
            {
                return true;
            }

            for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
            {
                Zombie* aDancer = mBoard->ZombieTryToGet(aLeader->mFollowerZombieID[i]);
                if (aDancer && aDancer->mChilledCounter > 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//0x52EFD0
void Zombie::SetAnimRate(float theAnimRate)
{
    mOriginalAnimRate = theAnimRate;
    ApplyAnimRate(theAnimRate);
}

//0x52EFF0
void Zombie::ApplyAnimRate(float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->mAnimRate = IsMovingAtChilledSpeed() ? theAnimRate * 0.5f : theAnimRate;
    }

    Reanimation* aPlantReanim = mApp->ReanimationTryToGet(mPlantHeadID);
    if (aPlantReanim)
    {
        aPlantReanim->mAnimRate = IsMovingAtChilledSpeed() ? mPlantHeadOriginalAnimRate * 0.5f : mPlantHeadOriginalAnimRate;
    }

    Reanimation* aFlagReanim = mApp->ReanimationTryToGet(mFlagReanimID);
    if (aFlagReanim)
    {
        aFlagReanim->mAnimRate = IsMovingAtChilledSpeed() ? theAnimRate * 0.5f : theAnimRate;
    }

}

//0x52F050
void Zombie::UpdateAnimSpeed()
{
    if (!IsOnBoard())
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if (IsImmobilizied() || (mYuckyFace && mYuckyFaceCounter < 170) || hasDiarrhea)
    {
        mPlantHeadOriginalAnimRate = 0.0f;
        ApplyAnimRate(0.0f);
        return;
    }

    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_UP_TO_EAT || mZombiePhase == ZombiePhase::PHASE_SNORKEL_DOWN_FROM_EAT || IsDeadOrDying())
    {
        ApplyAnimRate(mOriginalAnimRate);
        return;
    }

    if (mIsEating)
    {
        if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER || mZombieType == ZombieType::ZOMBIE_BALLOON || mZombieType == ZombieType::ZOMBIE_NINJAIMP || mZombieType == ZombieType::ZOMBIE_IMP ||
            mZombieType == ZombieType::ZOMBIE_DIGGER || mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER
            || mZombieType == ZombieType::ZOMBIE_YETI)
        {
            ApplyAnimRate(20.0f);
        }
        else
        {
            ApplyAnimRate(36.0f);
        }
    }
    else 
    {
        if (ZombieNotWalking() || IsBobsledTeamWithSled() || mZombieType == ZombieType::ZOMBIE_CATAPULT || 
            mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING || mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
        {
             ApplyAnimRate(mOriginalAnimRate);
        }
        else if (aBodyReanim->TrackExists("_ground"))
        {
            ReanimatorTrack* aTrack = &aBodyReanim->mDefinition->mTracks[aBodyReanim->FindTrackIndex("_ground")];
            float aDistance = aTrack->mTransforms[aBodyReanim->mFrameStart + aBodyReanim->mFrameCount - 1].mTransX - aTrack->mTransforms[aBodyReanim->mFrameStart].mTransX;
            if (aDistance >= 1e-6f)
            {
                float aOneOverSpeed = aBodyReanim->mFrameCount / aDistance;
                float aAnimRate = mVelX * aOneOverSpeed * 47.0f / mScaleZombie;
                ApplyAnimRate(aAnimRate);
            }
        }
    }

    if (mReverseCounter > 0) {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim && aBodyReanim->mAnimRate > 0)
        {
            aBodyReanim->mAnimRate *= -1;
        }
    }
}

void Zombie::ConvertToNormalZombie()
{
    StopZombieSound();
    mPosY = GetPosYBasedOnRow(mRow);
    mX = (int)mPosX;
    mY = (int)mPosY;

    mZombieType = ZombieType::ZOMBIE_NORMAL;
    mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
    mZombieAttackRect = Rect(50, 0, 20, 115);

    mAnimFrames = 12;
    mAnimTicksPerFrame = 12;
    mPhaseCounter = 0;

    PickRandomSpeed();
}

//0x52F250
void Zombie::StartEating()
{
    if (mZombiePhase == PHASE_ZOMBIE_WRAITH_GASP || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_INTANGIBLE)
        return;

    if (mIsEating)
        return;

    mIsEating = true;

    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
        return;

    if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING)
    {
        PlayZombieReanim("anim_laddereat", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MAD)
    {
        PlayZombieReanim("anim_eat_nopaper", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
    }
    else
    {
        if ((mZombieType == ZOMBIE_DOLPHIN_RIDER || mZombieType == ZOMBIE_SHARK_RIDER) && mZombiePhase == PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN)
        {
            PlayZombieReanim("anim_eat_land", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
        }
        else if (mZombieType != ZombieType::ZOMBIE_SNORKEL && mZombieType != ZombieType::ZOMBIE_DIVER)
        {
            PlayZombieReanim("anim_eat", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
        }

        if (mShieldType == ShieldType::SHIELDTYPE_DOOR)
        {
            ShowDoorArms(false);
        }
    }
}

//0x52F2E0
void Zombie::StartWalkAnim(int theBlendTime)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    PickRandomSpeed();
    if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING)
    {
        PlayZombieReanim("anim_ladderwalk", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
    }
    else if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MAD)
    {
        PlayZombieReanim("anim_walk_nopaper", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
    }
    else if (mInPool && mZombieHeight != ZombieHeight::HEIGHT_IN_TO_POOL && mZombieHeight != ZombieHeight::HEIGHT_OUT_OF_POOL && aBodyReanim->TrackExists("anim_swim"))
    {
        //if (mZombieType == ZOMBIE_DOOR)     theBlendTime = 10;
        PlayZombieReanim("anim_swim", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
    }
    else if ((mZombieType == ZombieType::ZOMBIE_NORMAL || mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE || mZombieType == ZombieType::ZOMBIE_PAIL) && (mBoard->mDanceMode || mApp->mGameMode == GAMEMODE_CHALLENGE_DANCE_PARTY || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON))
    {
        PlayZombieReanim("anim_dance", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
    }
    else if (mZombieType == ZOMBIE_WRAITH) {
        if (mZombiePhase == PHASE_ZOMBIE_INTANGIBLE && aBodyReanim->TrackExists("anim_walk")) {
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
        }
        else if (mZombiePhase == PHASE_ZOMBIE_NORMAL && aBodyReanim->TrackExists("anim_walk_tang")) {
            PlayZombieReanim("anim_walk_tang", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
        }
    }
    else
    {
        int aWalkAnimVariant = Rand(2);
        if (IsZombotany(mZombieType) || mZombieType == ZombieType::ZOMBIE_FLAG)
        {
            aWalkAnimVariant = 0;
        }

        if (aWalkAnimVariant == 0 && aBodyReanim->TrackExists("anim_walk2"))
        {
            PlayZombieReanim("anim_walk2", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
        }
        else if (aBodyReanim->TrackExists("anim_walk"))
        {
            PlayZombieReanim("anim_walk", ReanimLoopType::REANIM_LOOP, theBlendTime, 0.0f);
        }
    }

    mCannotBeHammered = false;
    if (mDropVelocityChange)
    {
        mVelX = mPostDropVelocityX;
        UpdateAnimSpeed();
    }

}

//0x52F440
void Zombie::StopEating()
{
    if (!mIsEating)
        return;

    mIsEating = false;
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
        return;

    if (aBodyReanim && mZombieType != ZombieType::ZOMBIE_SNORKEL && mZombieType != ZombieType::ZOMBIE_DIVER)
    {
        StartWalkAnim(20);
    }

    if (mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        ShowDoorArms(true);
    }

    UpdateAnimSpeed();
}

//0x52F4C0
void Zombie::CheckIfPreyCaught()
{
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_INTANGIBLE ||
        mZombieType == ZombieType::ZOMBIE_BUNGEE ||
        mZombieType == ZombieType::ZOMBIE_GARGANTUAR ||
        mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ||
        mZombieType == ZombieType::ZOMBIE_ZAMBONI ||
        mZombieType == ZombieType::ZOMBIE_CATAPULT ||
        mZombieType == ZombieType::ZOMBIE_BOSS || 
        IsBouncingPogo() || 
        IsBobsledTeamWithSled() ||
        mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT ||
        mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT || 
        mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MADDENING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED || 
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || 
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || 
        mZombiePhase == ZombiePhase::PHASE_IMP_LANDING || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_WITH_LIGHT || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_SNAPPING_FINGERS_HOLD || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || 
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING || 
        mZombiePhase == ZombiePhase::PHASE_LADDER_PLACING || 
        mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED || 
        mZombieHeight == ZombieHeight::HEIGHT_UP_LADDER || 
        mZombieHeight == ZombieHeight::HEIGHT_IN_TO_POOL || 
        mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL || 
        IsTangleKelpTarget() || 
        mZombieHeight == ZombieHeight::HEIGHT_FALLING || 
        !mHasHead || 
        IsFlying())
        return;
    
    int aTicksBetweenEats = TICKS_BETWEEN_EATS;
    if (mChilledCounter > 0)
        aTicksBetweenEats *= 2;

    if (mZombieAge % aTicksBetweenEats != 0)
        return;

    Zombie* aZombie = FindZombieTarget();
    if (aZombie)
    {
        EatZombie(aZombie);
        return;
    }

    if (!mMindControlled)
    {
        Plant* aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);

        if (aPlant)
        {
            EatPlant(aPlant);
            return;
        }
    }
    
    if (mApp->IsIZombieLevel() && mBoard->mChallenge->IZombieEatBrain(this))
    {
        return;
    }

    if (mIsEating)
    {
        StopEating();
    }
}

//0x52F6D0
void Zombie::PoolSplash(bool theInToPoolSound)
{
    float aOffsetX = 23.0f;
    float aOffsetY = 78.0f;
    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
    {
        aOffsetX -= 37.0f;
        aOffsetY -= -8.0f;
    }

    mApp->AddReanimation(mX + aOffsetX, mY + aOffsetY, mRenderOrder + 1, ReanimationType::REANIM_SPLASH)->OverrideScale(0.8f, 0.8f);
    mApp->AddTodParticle(mX + aOffsetX + 37.0f, mY + aOffsetY + 42.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PLANTING_POOL);

    if (theInToPoolSound)
    {
        mApp->PlayFoley(FoleyType::FOLEY_ZOMBIESPLASH);
    }
    else
    {
        mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);
    }
}

//0x52F870
void Zombie::CheckForPool()
{
    if ((!Zombie::ZombieTypeCanGoInPool(mZombieType) ||
        mZombieType == ZombieType::ZOMBIE_NORMAL ||
        mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE ||
        mZombieType == ZombieType::ZOMBIE_PAIL) &&
        !mHasDucky && !IsFlying())
    {
        bool aIsPoolSquare =
            mBoard->IsPoolSquare(mBoard->PixelToGridX(mX + 75, mY), mRow) &&
            mBoard->IsPoolSquare(mBoard->PixelToGridX(mX + 45, mY), mRow) &&
            mX < 680;

        if (aIsPoolSquare)
        {
            PoolSplash(true);
            DieNoLoot();
        }

        return;
    }

    if (!Zombie::ZombieTypeCanGoInPool(mZombieType) || !mHasDucky || IsFlying())
    {
        return;
    }

    if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        return;
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_IN_TO_POOL || mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL)
    {
        return;
    }

    bool aIsPoolSquare = 
        mBoard->IsPoolSquare(mBoard->PixelToGridX(mX + 75, mY), mRow) && 
        mBoard->IsPoolSquare(mBoard->PixelToGridX(mX + 45, mY), mRow) && 
        mX < 680;

    if (!mInPool && aIsPoolSquare)
    {
        if (mBoard->mIceTrapCounter > 0)
        {
            mIceTrapCounter = mBoard->mIceTrapCounter;
            ApplyChill(true);
        }
        else
        {
            mZombieHeight = ZombieHeight::HEIGHT_IN_TO_POOL;
            mInPool = true;
            PoolSplash(true);
        }
    }
    else if (mInPool && !aIsPoolSquare)
    {
        mZombieHeight = ZombieHeight::HEIGHT_OUT_OF_POOL;
        StartWalkAnim(0);
        PoolSplash(false);
    }
}

//0x52F9C0
bool Zombie::IsOnHighGround()
{
    return IsOnBoard() && mBoard->mGridSquareType[mBoard->PixelToGridXKeepOnBoard(mX + 75, mY)][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

//0x52FA20
void Zombie::CheckForHighGround()
{
    if (mZombieHeight != ZombieHeight::HEIGHT_ZOMBIE_NORMAL || mZombieType == ZombieType::ZOMBIE_BUNGEE)
        return;

    bool aIsHighGround = IsOnHighGround();
    if (!mOnHighGround && aIsHighGround)
    {
        mZombieHeight = ZombieHeight::HEIGHT_UP_TO_HIGH_GROUND;
        mOnHighGround = true;
    }
    else if (mOnHighGround && !aIsHighGround)
    {
        mZombieHeight = ZombieHeight::HEIGHT_DOWN_OFF_HIGH_GROUND;
    }
}

//0x52FA60
void Zombie::StartMindControlled()
{
    mApp->PlaySample(SOUND_MINDCONTROLLED);
    mMindControlled = !mMindControlled;
    mLastPortalX = -1;
    mChilledCounter = 0;

    if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER)
    {
        for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
        {
            mFollowerZombieID[i] = ZombieID::ZOMBIEID_NULL;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        Zombie* aLeader = mBoard->ZombieTryToGet(mRelatedZombieID);
        if (aLeader)
        {
            ZombieID aId = mBoard->ZombieGetID(this);
            for (int i = 0; i < NUM_BACKUP_DANCERS; i++)
            {
                if (aLeader->mFollowerZombieID[i] == aId)
                {
                    aLeader->mFollowerZombieID[i] = ZombieID::ZOMBIEID_NULL;
                    break;
                }
            }
        }

        mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
    }
    else
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mRelatedZombieID);
        if (aZombie)
        {
            aZombie->mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
            mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
        }
    }
}

//0x52FB40
void Zombie::EatPlant(Plant* thePlant)
{
    if (mApp->mGameMode == GAMEMODE_CHALLENGE_SQUIRREL)
        return;

    if (mZombieType == ZOMBIE_HYPNOSHROOM_HEAD) {
        mApp->PlaySample(SOUND_MINDCONTROLLED);
        thePlant->mMindControlled = !thePlant->mMindControlled;
        thePlant->mEnemy = thePlant->mMindControlled;
        Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
        aHeadReanim->ReanimationDie();
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;

        TakeDamage(1800, 9U);
        return;
    }

    if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN)
    {
        mPhaseCounter = 1;
        return;
    }

    if (mYuckyFace)
        return;

    if (mBoard->GetLadderAt(thePlant->mPlantCol, thePlant->mRow) && mZombieType != ZombieType::ZOMBIE_DIGGER)  // 矿工僵尸无视梯子
    {
        StopEating();

        if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL && mUseLadderCol != thePlant->mPlantCol)
        {
            mZombieHeight = ZombieHeight::HEIGHT_UP_LADDER;
            mUseLadderCol = thePlant->mPlantCol;
        }

        return;
    }

    StartEating();
    if (thePlant->mSeedType == SeedType::SEED_JALAPENO || 
        thePlant->mSeedType == SeedType::SEED_CHERRYBOMB || 
        thePlant->mSeedType == SeedType::SEED_DOOMSHROOM ||
        thePlant->mSeedType == SeedType::SEED_ICESHROOM || 
        thePlant->mSeedType == SeedType::SEED_HYPNOSHROOM || 
        thePlant->mState == PlantState::STATE_FLOWERPOT_INVULNERABLE ||
        thePlant->mState == PlantState::STATE_LILYPAD_INVULNERABLE || 
        thePlant->mState == PlantState::STATE_SQUASH_LOOK || 
        thePlant->mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (!thePlant->mIsAsleep)
        {
            return;
        }
    }
    if (thePlant->mSeedType == SeedType::SEED_POTATOMINE && thePlant->mState != PlantState::STATE_NOTREADY)
    {
        return;
    }

    bool triggered = false;
    if (thePlant->mSeedType == SeedType::SEED_BLOVER)
    {
        triggered = true;
    }
    if (thePlant->mSeedType == SeedType::SEED_ICESHROOM  && !thePlant->mIsAsleep)
    {
        triggered = true;
    }
    if (triggered)
    {
        thePlant->DoSpecial();
        return;
    }

    if (mChilledCounter > 0 && mZombieAge % 2 == 1)
        return;

    if (mApp->IsIZombieLevel() && thePlant->mSeedType == SeedType::SEED_SUNFLOWER)
    {
        int aStageBeforeChew = thePlant->mPlantHealth / 40;
        int aStageAfterChew = (thePlant->mPlantHealth - DAMAGE_PER_EAT) / 40;
        if (aStageAfterChew < aStageBeforeChew || thePlant->mPlantHealth - DAMAGE_PER_EAT <= 0)  // 若本次啃食令植物血量下降了至少 1 个阶段
        {
            mBoard->AddCoin(thePlant->mX, thePlant->mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
    }

    thePlant->mPlantHealth -= DAMAGE_PER_EAT;
    thePlant->mRecentlyEatenCountdown = 50;

    if (mApp->IsIZombieLevel() && mJustGotShotCounter < -500)
    {
        if (thePlant->mSeedType == SeedType::SEED_EXPLODE_O_NUT  || thePlant->mSeedType == SeedType::SEED_WALLNUT || thePlant->mSeedType == SeedType::SEED_ARTIC_NUT || thePlant->mSeedType == SeedType::SEED_FIRESHROOM || thePlant->mSeedType == SeedType::SEED_TALLNUT || thePlant->mSeedType == SeedType::SEED_PUMPKINSHELL|| thePlant->mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            thePlant->mPlantHealth -= DAMAGE_PER_EAT;
        }
    }

    if (thePlant->mPlantHealth <= 0)
    {
        mApp->PlaySample(SOUND_GULP);
        mBoard->mPlantsEaten++;

        if (thePlant->mSeedType == SEED_SUNBEAN) {
            mIsSunbeaned = true;

            mPrevHealth = mBodyHealth;
            if (mHelmHealth > 0)
                mPrevHealth += mHelmHealth;
            if (mShieldHealth > 0)
                mPrevHealth += mShieldHealth;

        }
        
        if (thePlant->mSeedType == SEED_EXPLODE_O_NUT)
            thePlant->DoSpecial();
        else
            thePlant->Die();

        mBoard->mChallenge->ZombieAtePlant(this, thePlant);

        if (mBoard->mLevel >= 2 && mBoard->mLevel <= 4 && mApp->IsFirstTimeAdventureMode())
        {
            if (thePlant->mPlantCol > 4 && mBoard->mPlants.mSize < 15 && thePlant->mSeedType == SeedType::SEED_PEASHOOTER)
            {
                mBoard->DisplayAdvice(_S("[ADVICE_PEASHOOTER_DIED]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_PEASHOOTER_DIED);
            }
        }

        hasDiarrhea = thePlant->mSeedType == SeedType::SEED_CHILIBEAN;
        if (hasDiarrhea)
        {
            if (mZombiePhase != PHASE_ZOMBIE_DYING)
                ApplyAnimRate(0.0f);
        }
    }
}

//0x52FE10
void Zombie::EatZombie(Zombie* theZombie)
{
    theZombie->TakeDamageForced(DAMAGE_PER_EAT, 9U); //9U

    StartEating();

    if (theZombie->mZombieType == ZOMBIE_HYPNOSHROOM_HEAD) {
        StartMindControlled();
        theZombie->TakeDamage(1800, 9U);
    }

    if (theZombie->mBodyHealth <= 0){
        mApp->PlaySample(SOUND_GULP);
    }
}

//0x52FE50
bool Zombie::TrySpawnLevelAward()
{
    if (!IsOnBoard() || mBoard->HasLevelAwardDropped() || mBoard->mLevelComplete || mDroppedLoot || mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
    {
        return false;
    }

    if (mApp->IsFinalBossLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID || mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
    {
        if (mZombieType != ZombieType::ZOMBIE_BOSS)
        {
            return false;
        }
    }
    else if (mApp->IsScaryPotterLevel())
    {
        if (!mBoard->mChallenge->ScaryPotterIsCompleted())
        {
            return false;
        }
    }
    else if (mApp->IsContinuousChallenge() || mBoard->mCurrentWave < mBoard->mNumWaves || mBoard->AreEnemyZombiesOnScreen())
    {
        return false;
    }
    
    if (mApp->IsWhackAZombieLevel() && mBoard->mZombieCountDown > 0)
    {
        return false;
    }

    mBoard->mLevelAwardSpawned = true;
    mApp->mBoardResult = BoardResult::BOARDRESULT_WON;
    mBoard->HideSpeedButton();
    Rect aZombieRect = GetZombieRect();
    int aCenterX = aZombieRect.mX + aZombieRect.mWidth / 2;
    int aCenterY = aZombieRect.mY + aZombieRect.mHeight / 2;

    //if (!mBoard->IsSurvivalStageWithRepick())
    //{
    //    mBoard->RemoveAllZombies();
   // }

    CoinType aCoinType;
    if (mApp->IsScaryPotterLevel() && !mBoard->IsFinalScaryPotterStage())
    {
        aCoinType = CoinType::COIN_NONE;
        mBoard->mChallenge->PuzzlePhaseComplete(mBoard->PixelToGridXKeepOnBoard(mPosX + 75, mPosY), mRow);
    }
    else if (mApp->IsAdventureMode() && mBoard->mLevel <= 60)
    {
        if (mBoard->mLevel == 9 || mBoard->mLevel == 19 || mBoard->mLevel == 29 || mBoard->mLevel == 39 || mBoard->mLevel == 49)
        {
            aCoinType = CoinType::COIN_NOTE;
        }
        else if (mBoard->mLevel == 60)
        {
            aCoinType = mApp->HasFinishedAdventure() ? CoinType::COIN_AWARD_MONEY_BAG : CoinType::COIN_AWARD_SILVER_SUNFLOWER;
        }
        else if (mApp->HasFinishedAdventure())
        {
            aCoinType = CoinType::COIN_AWARD_MONEY_BAG;
        }
        else if (mBoard->mLevel == 4)
        {
            aCoinType = CoinType::COIN_SHOVEL;
        }
        else if (mBoard->mLevel == 14)
        {
            aCoinType = CoinType::COIN_ALMANAC;
        }
        else if (mBoard->mLevel == 24)
        {
            aCoinType = CoinType::COIN_CARKEYS;
        }
        else if (mBoard->mLevel == 34)
        {
            aCoinType = CoinType::COIN_TACO;
        }
        else if (mBoard->mLevel == 44)
        {
            aCoinType = CoinType::COIN_WATERING_CAN;
        }
        else
        {
            aCoinType = CoinType::COIN_FINAL_SEED_PACKET;
        }
    }
    else if (mBoard->IsSurvivalStageWithRepick())
    {
        aCoinType = CoinType::COIN_NONE;
        mBoard->FadeOutLevel();
    }
    else if (mBoard->IsLastStandStageWithRepick())
    {
        aCoinType = CoinType::COIN_NONE;

        mBoard->FadeOutLevel();
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
        for (int i = 0; i < 10; i++)
        {
            mBoard->AddCoin(aCenterX + i * 5, aCenterY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_COIN);
        }
    }
    else if (!mApp->IsAdventureMode())
    {
        if (mApp->HasBeatenChallengeVeteran(mApp->mGameMode) && mBoard->mIsVeteran)
        {
            aCoinType = CoinType::COIN_AWARD_MONEY_BAG;
        }
        else if (mApp->TrophiesNeedForDiamondSunflower() == 1)
        {
            aCoinType = CoinType::COIN_AWARD_DIAMOND_SUNFLOWER;
        }
        else if (mBoard->mIsVeteran)
        {
            aCoinType = CoinType::COIN_DIAMOND_TROPHY;
        }
        else if (mApp->HasBeatenChallenge(mApp->mGameMode))
        {
            aCoinType = CoinType::COIN_AWARD_MONEY_BAG;
        }
        else if (mApp->TrophiesNeedForGoldSunflower() == 1)
        {
            aCoinType = CoinType::COIN_AWARD_GOLD_SUNFLOWER;
        }
        else
        {
            aCoinType = CoinType::COIN_TROPHY;
        }
    }
    else
    {
        aCoinType = CoinType::COIN_AWARD_MONEY_BAG;
    }

    CoinMotion aCoinMotion = CoinMotion::COIN_MOTION_COIN;
    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        aCoinMotion = CoinMotion::COIN_MOTION_FROM_BOSS;
    }

    if (aCoinType != CoinType::COIN_NONE)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
        mBoard->AddCoin(aCenterX, aCenterY, aCoinType, aCoinMotion);
    }

    mDroppedLoot = true;
    return true;
}

//0x530170
void Zombie::DropLoot()
{
    if (!IsOnBoard() || mApp->IsIZombieLevel() || mNoCoin)
        return;

    AlmanacPlayerDefeatedZombie(mZombieType);
    if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        mBoard->mKilledYeti = true;
    }

    TrySpawnLevelAward();
    if (mDroppedLoot || mBoard->HasLevelAwardDropped() || !mBoard->CanDropLoot())
        return;

    mDroppedLoot = true;
    int aZombieValue = GetZombieDefinition(mZombieType).mZombieValue;
    if (((mApp->IsLittleTroubleLevel()|| mTiny) || (mApp->IsBigTroubleLevel() || mGiant)) && Rand(4) != 0)
    {
        return;
    }
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM || mApp->IsIZombieLevel())
    {
        return;
    }

    Rect aZombieRect = GetZombieRect();
    int aCenterX = aZombieRect.mX + aZombieRect.mWidth / 2;
    int aCenterY = aZombieRect.mY + aZombieRect.mHeight / 4;
    if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
        mBoard->AddCoin(aCenterX - 20, aCenterY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
        mBoard->AddCoin(aCenterX - 30, aCenterY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
        mBoard->AddCoin(aCenterX - 40, aCenterY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
        mBoard->AddCoin(aCenterX - 50, aCenterY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
    }
    else
    {
        mBoard->DropLootPiece(aCenterX, aCenterY, aZombieValue);
    }
}

//0x5302F0
void Zombie::DieWithLoot()
{
    DieNoLoot();
    DropLoot();
}

//0x530310
void Zombie::BobsledDie()
{
    if (!IsBobsledTeamWithSled() || !IsOnBoard())
        return;

    Zombie* aLeaderZombie;
    if (mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
    {
        aLeaderZombie = this;
    }
    else
    {
        aLeaderZombie = mBoard->ZombieGet(mRelatedZombieID);
    }

    if (!aLeaderZombie->mDead)
    {
        aLeaderZombie->DieNoLoot();
    }
    for (int i = 0; i < NUM_BOBSLED_FOLLOWERS; i++)
    {
        Zombie* aZombie = mBoard->ZombieGet(aLeaderZombie->mFollowerZombieID[i]);
        if (!aZombie->mDead)
        {
            aZombie->DieNoLoot();
        }
    }
}

//0x5303A0
void Zombie::BobsledBurn()
{
    if (!IsBobsledTeamWithSled())
        return;

    Zombie* aLeaderZombie;
    if (mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
    {
        aLeaderZombie = this;
    }
    else
    {
        aLeaderZombie = mBoard->ZombieGet(mRelatedZombieID);
    }

    aLeaderZombie->ApplyBurn();
    for (int i = 0; i < NUM_BOBSLED_FOLLOWERS; i++)
    {
        mBoard->ZombieGet(aLeaderZombie->mFollowerZombieID[i])->DieNoLoot();
    }
}

//0x530410
void Zombie::BungeeDropPlant()
{
    if (mZombiePhase == ZombiePhase::PHASE_BUNGEE_GRABBING)
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet(mTargetPlantID);
        if (aPlant)
        {
            if (aPlant->mOnBungeeState == PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
            {
                aPlant->mOnBungeeState = PlantOnBungeeState::NOT_ON_BUNGEE;
            }
            else if (aPlant->mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE)
            {
                aPlant->Die();
            }

            mTargetPlantID = PlantID::PLANTID_NULL;
        }
    }
}

//0x530480
void Zombie::BungeeDie()
{
    BungeeDropPlant();

    if (mBoard)  // 原版没有这个判断，因为 mBoard 为空时 DataArrayTryToGet() 不会实际用到 mBoard，此处为了确保安全就加上了这个判断
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mTargetPlantID);
        if (aPlant)
        {
            mBoard->mPlantsEaten++;
            aPlant->Die();
        }
    }

    Zombie* aZombie = mBoard->ZombieTryToGet(mRelatedZombieID);
    if (aZombie && !aZombie->mDead)
    {
        aZombie->DieNoLoot();
    }
}

//0x530510
void Zombie::DieNoLoot()
{
    StopZombieSound();
    AttachmentDie(mAttachmentID);
    mApp->RemoveReanimation(mBodyReanimID);
    mApp->RemoveReanimation(mMoweredReanimID);
    mApp->RemoveReanimation(mSpecialHeadReanimID);
    mApp->RemoveReanimation(mFlagReanimID);
    mApp->RemoveReanimation(mPlantHeadID);
    mApp->RemoveReanimation(mHelmetReanimID);
    mApp->RemoveReanimation(mExtraReanimID);

    if (mIsSunbeaned && mSunProduced < 300) {
        int currentHealth = 0;
        int lostHealth = mPrevHealth;

        int smallSuns = floor(lostHealth / 15);
        int normalSuns = floor(lostHealth / 30);
        int largeSuns = floor(lostHealth / 60);

        smallSuns = max(0, smallSuns - normalSuns);
        normalSuns = max(0, normalSuns - largeSuns);

        for (int i = 0; i < smallSuns; i++) {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mSunProduced += 15;
            mPrevHealth -= 15;
        }

        for (int i = 0; i < normalSuns; i++) {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mSunProduced += 25;
            mPrevHealth -= 30;
        }

        for (int i = 0; i < largeSuns; i++) {
            mBoard->AddCoin(mX, mY, CoinType::COIN_LARGESUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mSunProduced += 50;
            mPrevHealth -= 60;
        }
    }

    mDead = true;
    TrySpawnLevelAward();
    if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
    {
        BobsledDie();
    }
    if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
    {
        BungeeDie();
    }
    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        BossDie();
    }
}

//0x530640
void Zombie::PlayZombieAppearSound()
{
    if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER)
    {
        mApp->PlayFoley(FoleyType::FOLEY_DOLPHIN_APPEARS);
    }
    else if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        mApp->PlayFoley(FoleyType::FOLEY_BALLOONINFLATE);
    }
    else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        mApp->PlayFoley(FoleyType::FOLEY_ZAMBONI);
    }
}

//0x530770
void Zombie::StartZombieSound()
{
    if (mPlayingSong)
        return;

    if ((mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING) && mHasHead)
    {
        mApp->PlayFoley(FoleyType::FOLEY_JACKINTHEBOX);
        mPlayingSong = true;
    }
    else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING)
    {
        mApp->PlayFoley(FoleyType::FOLEY_DIGGER);
        mPlayingSong = true;
    }
}

//0x530850
void Zombie::StopZombieSound()
{
    if (mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        bool aStopSound = false;

        if (mBoard)
        {
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->mHasHead && !aZombie->IsDeadOrDying() && aZombie->IsOnBoard() && 
                    (aZombie->mZombieType == ZombieType::ZOMBIE_DISCO || aZombie->mZombieType == ZombieType::ZOMBIE_DANCER || aZombie->mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP))
                {
                    aStopSound = true;
                    break;
                }
            }
        }

        if (aStopSound)
        {
            mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_DANCER);
        }
    }

    if (mPlayingSong)
    {
        mPlayingSong = false;

        if (mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX)
        {
            mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_JACKINTHEBOX);
        }
        else if (mZombieType == ZombieType::ZOMBIE_DIGGER)
        {
            mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_DIGGER);
        }
    }
}

//0x530950
void Zombie::ApplyChill(bool theIsIceTrap)
{
    if (!CanBeChilled())
        return;

    if (mChilledCounter == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
    }

    int aChillTime = 1000;
    if (theIsIceTrap)
    {
        aChillTime = 2000;
    }
    mChilledCounter = max(aChillTime, mChilledCounter);

    UpdateAnimSpeed();
}

//0x530A00
void Zombie::DropShield(unsigned int theDamageFlags)
{
    if (mShieldType == ShieldType::SHIELDTYPE_NONE)
        return;

    //ZombieDrawPosition aDrawPos;
    //GetDrawPos(aDrawPos);
    if (mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        DetachShield();
        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
        {
            float aPosX, aPosY;
            GetTrackPosition("anim_screendoor", aPosX, aPosY);
            TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_DOOR);

            if (mZombieType == ZOMBIE_GLASSDOOR) 
                aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_GLASSDOOR3);

            OverrideParticleScale(aParticle);
        }
    }
    else if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER)
    {
        StopEating();
        if (mYuckyFace)
        {
            ShowYuckyFace(false);
            mYuckyFace = false;
            mYuckyFaceCounter = 0;
        }

        mZombiePhase = ZombiePhase::PHASE_NEWSPAPER_MADDENING;
        PlayZombieReanim("anim_gasp", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 8.0f);
        DetachShield();

        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
        {
            float aPosX, aPosY;
            GetTrackPosition("Zombie_paper_paper", aPosX, aPosY);
            TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_NEWSPAPER);
            OverrideParticleScale(aParticle);
        }

        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY) && !TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD))
        {
            mApp->PlayFoley(FoleyType::FOLEY_NEWSPAPER_RIP);
            AddAttachedReanim(-11, 0, ReanimationType::REANIM_ZOMBIE_SURPRISE);
        }
    }
    else if (mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        DetachShield();
        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
        {
            float aPosX = mPosX + 31.0f;
            float aPosY = mPosY + 80.0f;
            TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_LADDER);
            OverrideParticleScale(aParticle);
        }
    }

    mShieldType = ShieldType::SHIELDTYPE_NONE;
}

//0x530BC0
int Zombie::TakeShieldDamage(int theDamage, unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH)){
        mShieldJustGotShotCounter = 25;
    }

    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH) && !TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY))
    {
        mShieldRecoilCounter = 12;
        if (mShieldType == ShieldType::SHIELDTYPE_DOOR || mShieldType == ShieldType::SHIELDTYPE_LADDER)
        {
            mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
        }
    }

    int aDamageIndexBeforeDamage = GetShieldDamageIndex();
    int aDamageActual = min(mShieldHealth, theDamage);
    int aDamageRemaining = theDamage - aDamageActual;
    mShieldHealth -= aDamageActual;
    if (mShieldHealth == 0)
    {
        DropShield(theDamageFlags);
        return aDamageRemaining;
    }

    int aDamageIndexAfterDamage = GetShieldDamageIndex();
    if (aDamageIndexAfterDamage != aDamageIndexBeforeDamage)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (mShieldType == ShieldType::SHIELDTYPE_DOOR && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("anim_screendoor", IMAGE_REANIM_ZOMBIE_SCREENDOOR2);

            if (mZombieType == ZOMBIE_GLASSDOOR) 
                aBodyReanim->SetImageOverride("anim_screendoor", IMAGE_REANIM_ZOMBIE_GLASSDOOR2);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_DOOR && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("anim_screendoor", IMAGE_REANIM_ZOMBIE_SCREENDOOR3);

            if (mZombieType == ZOMBIE_GLASSDOOR)
                aBodyReanim->SetImageOverride("anim_screendoor", IMAGE_REANIM_ZOMBIE_GLASSDOOR3);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("Zombie_paper_paper", IMAGE_REANIM_ZOMBIE_PAPER_PAPER2);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("Zombie_paper_paper", IMAGE_REANIM_ZOMBIE_PAPER_PAPER3);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_LADDER && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("Zombie_ladder_1", IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_LADDER && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetImageOverride("Zombie_ladder_1", IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2);
        }
    }

    return aDamageRemaining;
}

//0x530E30
void Zombie::DropHelm(unsigned int theDamageFlags)
{
    if (mHelmType == HelmType::HELMTYPE_NONE)
        return;

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    float aPosX = mPosX + aDrawPos.mImageOffsetX + aDrawPos.mHeadX + 14.0f;
    float aPosY = mPosY + aDrawPos.mImageOffsetY + aDrawPos.mHeadY + aDrawPos.mBodyY + 18.0f;
    ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
    if (mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE)
    {
        GetTrackPosition("anim_cone", aPosX, aPosY);
        ReanimShowPrefix("anim_cone", RENDER_GROUP_HIDDEN);
        if (!IsZombotany(mZombieType))
            ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_TRAFFIC_CONE;
    }
    else if (mHelmType == HelmType::HELMTYPE_PAIL)
    {
        GetTrackPosition("anim_bucket", aPosX, aPosY);
        ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_PAIL;
    }
    else if (mHelmType == HelmType::HELMTYPE_FOOTBALL || mHelmType == HelmType::HELMTYPE_BLACKFOOTBALL)
    {
        GetTrackPosition("zombie_football_helmet", aPosX, aPosY);
        ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
             
        if (mHelmType == HELMTYPE_BLACKFOOTBALL)
            aEffect = ParticleEffect::PARTICLE_BLACKZOMBIE_HELMET; 
        else 
            aEffect = ParticleEffect::PARTICLE_ZOMBIE_HELMET;
    }
    else if (mHelmType == HelmType::HELMTYPE_DIVER)
    {
        GetTrackPosition("Zombie_diver", aPosX, aPosY);
        ReanimShowPrefix("Zombie_diver", RENDER_GROUP_HIDDEN);

        aEffect = ParticleEffect::PARTICLE_ZOMBIE_HELMET;
    }
    else if (mHelmType == HelmType::HELMTYPE_DIGGER)
    {
        if (mZombieType == ZOMBIE_NORMAL)
        {
            ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
        }

        GetTrackPosition("Zombie_digger_hardhat", aPosX, aPosY);
        ReanimShowTrack("Zombie_digger_hardhat", RENDER_GROUP_HIDDEN);
        aEffect = ParticleEffect::PARTICLE_ZOMBIE_HEADLIGHT;
    }
    else if (mHelmType == HelmType::HELMTYPE_BOBSLED && !TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
    {
        BobsledCrash();
    }

    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY) && aEffect != ParticleEffect::PARTICLE_NONE)
    {
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, aEffect);
        OverrideParticleScale(aParticle);

        if (mHelmType == HelmType::HELMTYPE_DIVER)  aParticle->OverrideImage(nullptr, IMAGE_REANIM_ZOMBIE_DIVER3);
    }

    if (mHelmetReanimID != REANIMATIONID_NULL)
    {
        Reanimation* aHelmReanim = mApp->ReanimationTryToGet(mHelmetReanimID);
        if (aHelmReanim)
            aHelmReanim->ReanimationDie();
        mHelmetReanimID = REANIMATIONID_NULL;
    }

    mHelmType = HelmType::HELMTYPE_NONE;
}

//0x530FE0
int Zombie::TakeHelmDamage(int theDamage, unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
    {
        if (mHelmType == HELMTYPE_TALLNUT || mHelmType == HELMTYPE_WALLNUT) {
            mJustGotShotCounter = 25;
        }
        else {
            mHelmetJustGotShotCounter = 25;
            mHelmetRecoilCounter = 12;
        }
    }

    int aDamageIndexBeforeDamage = GetHelmDamageIndex();
    int aDamageActual = min(mHelmHealth, theDamage);
    int aDamageRemaining = theDamage - aDamageActual;
    mHelmHealth -= aDamageActual;

    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_SHORT_ICETRAP))
    {
        HitIceTrap(150);
    }
    else if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_FREEZE))
    {
        ApplyChill(false);
    }

    if (mHelmHealth == 0)
    {
        DropHelm(theDamageFlags);
        return aDamageRemaining;
    }

    int aDamageIndexAfterDamage = GetHelmDamageIndex();
    if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        Reanimation* targetReanim = aBodyReanim;
        Reanimation* mHelmetReanim = mApp->ReanimationTryToGet(mHelmetReanimID);
        if (mHelmetReanim)
            targetReanim = mHelmetReanim;

        if (mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE && aDamageIndexAfterDamage == 1 && aBodyReanim)
        {
            targetReanim->SetImageOverride("anim_cone", IMAGE_REANIM_ZOMBIE_CONE2);
        }
        else if (mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE && aDamageIndexAfterDamage == 2 && aBodyReanim)
        {
            targetReanim->SetImageOverride("anim_cone", IMAGE_REANIM_ZOMBIE_CONE3);
        }
        else if (mHelmType == HelmType::HELMTYPE_PAIL && aDamageIndexAfterDamage == 1)
        {
            targetReanim->SetImageOverride("anim_bucket", IMAGE_REANIM_ZOMBIE_BUCKET2);
        }
        else if (mHelmType == HelmType::HELMTYPE_PAIL && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(targetReanim);
            targetReanim->SetImageOverride("anim_bucket", IMAGE_REANIM_ZOMBIE_BUCKET3);
        }
        else if (mHelmType == HelmType::HELMTYPE_DIGGER && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(targetReanim);
            targetReanim->SetImageOverride("Zombie_digger_hardhat", IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2);
        }
        else if (mHelmType == HelmType::HELMTYPE_DIGGER && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(targetReanim);
            targetReanim->SetImageOverride("Zombie_digger_hardhat", IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3);
        }
        else if (mHelmType == HelmType::HELMTYPE_FOOTBALL && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(targetReanim);
            
            targetReanim->SetImageOverride("zombie_football_helmet", IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2);
        }
        else if (mHelmType == HelmType::HELMTYPE_BLACKFOOTBALL && aDamageIndexAfterDamage == 1)
        {
            TOD_ASSERT(targetReanim);

            targetReanim->SetImageOverride("zombie_football_helmet", IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_HELMET2);
        }
        else if (mHelmType == HelmType::HELMTYPE_FOOTBALL && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(targetReanim);

            targetReanim->SetImageOverride("zombie_football_helmet", IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3);
        }
        else if (mHelmType == HelmType::HELMTYPE_BLACKFOOTBALL && aDamageIndexAfterDamage == 2)
        {
            TOD_ASSERT(targetReanim);
            targetReanim->SetImageOverride("zombie_football_helmet", IMAGE_REANIM_ZOMBIE_BLACKFOOTBALL_HELMET3);
        }
        else if (mHelmType == HelmType::HELMTYPE_WALLNUT && aDamageIndexAfterDamage == 1)
        {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
            aHeadReanim->SetImageOverride("anim_face", IMAGE_REANIM_WALLNUT_CRACKED1);
        }
        else if (mHelmType == HelmType::HELMTYPE_WALLNUT && aDamageIndexAfterDamage == 2)
        {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
            aHeadReanim->SetImageOverride("anim_face", IMAGE_REANIM_WALLNUT_CRACKED2);
        }
        else if (mHelmType == HelmType::HELMTYPE_TALLNUT && aDamageIndexAfterDamage == 1)
        {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
            aHeadReanim->SetImageOverride("anim_idle", IMAGE_REANIM_TALLNUT_CRACKED1);
        }
        else if (mHelmType == HelmType::HELMTYPE_TALLNUT && aDamageIndexAfterDamage == 2)
        {
            Reanimation* aHeadReanim = mApp->ReanimationGet(mPlantHeadID);
            aHeadReanim->SetImageOverride("anim_idle", IMAGE_REANIM_TALLNUT_CRACKED2);
        }
        else if (mHelmType == HelmType::HELMTYPE_DIVER && aDamageIndexAfterDamage == 1)
        {
            targetReanim->SetImageOverride("Zombie_diver", IMAGE_REANIM_ZOMBIE_DIVER2);
        }
        else if (mHelmType == HelmType::HELMTYPE_DIVER && aDamageIndexAfterDamage == 2)
        {
            targetReanim->SetImageOverride("Zombie_diver", IMAGE_REANIM_ZOMBIE_DIVER3);
        }
    }
    return aDamageRemaining;
}

int Zombie::TakeFlyingDamage(int theDamage, unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
    {
        mJustGotShotCounter = 25;
    }

    int aDamageActual = min(mFlyingHealth, theDamage);
    int aDamageRemaining = theDamage - aDamageActual;
    mFlyingHealth -= aDamageActual;
    if (mFlyingHealth == 0)
    {
        LandFlyer(theDamageFlags);
    }

    return aDamageRemaining;
}

//0x5312D0
void Zombie::TakeBodyDamage(int theDamage, unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
        mJustGotShotCounter = 25;
    
    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_SHORT_ICETRAP))
    {
        HitIceTrap(150);
    }
    else if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_FREEZE))
    {
        ApplyChill(false);
    }
    
    int aBodyHealthOrigin = mBodyHealth;
    int aDamageIndexBeforeDamage = GetBodyDamageIndex();
    mBodyHealth -= theDamage;
    int aDamageIndexAfterDamage = GetBodyDamageIndex();

    if (mZombieType == ZombieType::ZOMBIE_FLAG)
    {
        if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
        {
            Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mFlagReanimID);

            if (aDamageIndexAfterDamage == 1)
            {
                aHeadReanim->SetImageOverride("Zombie_flag", IMAGE_REANIM_ZOMBIE_FLAG2);
            }
            else if (aDamageIndexAfterDamage == 2)
            {
                aHeadReanim->SetImageOverride("Zombie_flag", IMAGE_REANIM_ZOMBIE_FLAG3);
            }
        }
        UpdateDamageStates(theDamageFlags);
    }
    else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
            mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
        

        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_SPIKE))
        {
            aBodyReanim->SetImageOverride("Zombie_zamboni_1", IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2);
            aBodyReanim->SetImageOverride("Zombie_zamboni_2", IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2);
            ZamboniDeath(theDamageFlags);
        }
        else if (mBodyHealth <= 0)
        {
            ZamboniDeath(theDamageFlags);
        }
        else if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
        {
            if (aDamageIndexAfterDamage == 1)
            {
                aBodyReanim->SetImageOverride("Zombie_zamboni_1", IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1);
                aBodyReanim->SetImageOverride("Zombie_zamboni_2", IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1);
            }
            else if (aDamageIndexAfterDamage == 2)
            {
                aBodyReanim->SetImageOverride("Zombie_zamboni_1", IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2);
                aBodyReanim->SetImageOverride("Zombie_zamboni_2", IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2);
                AddAttachedParticle(27, 72, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
            }
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_SPIKE) || mBodyHealth <= 0)
        {
            aBodyReanim->SetImageOverride("Zombie_catapult_siding", IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE);
            CatapultDeath(theDamageFlags);
        }
        else if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
        {
            if (aDamageIndexAfterDamage == 1)
            {
                aBodyReanim->SetImageOverride("Zombie_catapult_siding", IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE);
            }
            else if (aDamageIndexAfterDamage == 2)
            {
                AddAttachedParticle(47, 77, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
            }
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
        {
            if (aDamageIndexAfterDamage == 1)
            {
                aBodyReanim->SetImageOverride("Zombie_gargantua_body1", IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2);
                aBodyReanim->SetImageOverride("Zombie_gargantuar_outerarm_lower", IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2);
            }
            else if (aDamageIndexAfterDamage == 2)
            {
                aBodyReanim->SetImageOverride("Zombie_gargantua_body1", IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3);
                aBodyReanim->SetImageOverride("Zombie_gargantuar_outerleg_foot", IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2);
                aBodyReanim->SetImageOverride("Zombie_gargantuar_outerarm_lower", IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2);
                if (mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
                {
                    aBodyReanim->SetImageOverride("anim_head1", IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE);
                }
                else
                {
                    aBodyReanim->SetImageOverride("anim_head1", IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2);
                }
            }
        }

        if (mIsInstantDeath)
        {
            UpdateDamageStates(theDamageFlags);
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
            mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
        

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aDamageIndexBeforeDamage != aDamageIndexAfterDamage)
        {
            if (aDamageIndexAfterDamage == 1)
            {
                aBodyReanim->SetImageOverride("Boss_head", IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1);
                aBodyReanim->SetImageOverride("Boss_jaw", IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1);
                aBodyReanim->SetImageOverride("Boss_outerarm_hand", IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1);
                aBodyReanim->SetImageOverride("Boss_outerarm_thumb2", IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1);
                aBodyReanim->SetImageOverride("Boss_innerleg_foot", IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1);
            }
            else if (aDamageIndexAfterDamage == 2)
            {
                aBodyReanim->SetImageOverride("Boss_head", IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2);
                aBodyReanim->SetImageOverride("Boss_jaw", IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2);
                aBodyReanim->SetImageOverride("Boss_outerarm_hand", IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2);
                aBodyReanim->SetImageOverride("Boss_outerarm_thumb2", IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2);
                aBodyReanim->SetImageOverride("Boss_outerleg_foot", IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2);
                ApplyBossSmokeParticles(true);
            }
        }

        if (aBodyHealthOrigin >= mBodyMaxHealth / BOSS_FLASH_HEALTH_FRACTION && mBodyHealth < mBodyMaxHealth / BOSS_FLASH_HEALTH_FRACTION)
        {
            mApp->AddTodParticle(770.0f, 260.0f, Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 0), ParticleEffect::PARTICLE_BOSS_EXPLOSION);
            mApp->PlayFoley(FoleyType::FOLEY_BOSS_EXPLOSION_SMALL);
            ApplyBossSmokeParticles(true);
        }

        if (mBodyHealth <= 0)
        {
            mBodyHealth = 1;
        }
    }
    else
    {
        UpdateDamageStates(theDamageFlags);
    }

    if (mBodyHealth <= 0)
    {
        mBodyHealth = 0;
        PlayDeathAnim(theDamageFlags);
        DropLoot();
    }
}

//0x5317C0
void Zombie::TakeDamage(int theDamage, unsigned int theDamageFlags)
{
    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || IsDeadOrDying())
        return;

    if (theDamage == 0)
    {
        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_SHORT_ICETRAP))
        {
            HitIceTrap(150);
        }
        else if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_FREEZE))
        {
            ApplyChill(false);
        }

        return;
    }

    int aDamageRemaining = theDamage;

    if (IsFlying())
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_BRUTAL_EVENING && mHelmType != HelmType::HELMTYPE_NONE) {
            aDamageRemaining = TakeHelmDamage(aDamageRemaining, theDamageFlags);
        }
        else if (aDamageRemaining > 0)
        {
            aDamageRemaining = TakeFlyingDamage(aDamageRemaining, theDamageFlags);
        }
    }
    if (aDamageRemaining > 0 && mShieldType != ShieldType::SHIELDTYPE_NONE && !TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD))
    {
        aDamageRemaining = TakeShieldDamage(aDamageRemaining, theDamageFlags);
        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY))
        {
            aDamageRemaining = theDamage;
        }
    }
    if (aDamageRemaining > 0 && mHelmType != HelmType::HELMTYPE_NONE)
    {
        aDamageRemaining = TakeHelmDamage(aDamageRemaining, theDamageFlags);
    }
    if (aDamageRemaining > 0)
    {
        TakeBodyDamage(aDamageRemaining, theDamageFlags);
    }
}

void Zombie::TakeDamageForced(int theDamage, unsigned int theDamageFlags)
{
    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || IsDeadOrDying())
        return;

    int aDamageRemaining = theDamage;

    if (IsFlying())
    {
        aDamageRemaining = TakeFlyingDamage(aDamageRemaining, theDamageFlags);
    }
    if (aDamageRemaining > 0 && mShieldType != ShieldType::SHIELDTYPE_NONE)
    {
        aDamageRemaining = TakeShieldDamage(aDamageRemaining, theDamageFlags);
        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY))
        {
            aDamageRemaining = theDamage;
        }
    }
    if (aDamageRemaining > 0 && mHelmType != HelmType::HELMTYPE_NONE)
    {
        aDamageRemaining = TakeHelmDamage(aDamageRemaining, theDamageFlags);
    }
    if (aDamageRemaining > 0)
    {
        TakeBodyDamage(aDamageRemaining, theDamageFlags);
    }
}

void Zombie::TakeBurnDamage(int theDamage, unsigned int theDamageFlags)
{
    if (mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || IsDeadOrDying())
        return;

    int aDamageRemaining = theDamage;

    if (IsFlying())
    {
        aDamageRemaining = TakeFlyingDamage(aDamageRemaining, theDamageFlags);
    }
    if (aDamageRemaining > 0 && mShieldType != ShieldType::SHIELDTYPE_NONE && !TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD))
    {
        aDamageRemaining = TakeShieldDamage(aDamageRemaining, theDamageFlags);
        if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY))
        {
            aDamageRemaining = theDamage;
        }
    }
    if (aDamageRemaining > 0 && mHelmType != HelmType::HELMTYPE_NONE)
    {
        aDamageRemaining = TakeHelmDamage(aDamageRemaining, theDamageFlags);
    }
    if (aDamageRemaining > 0)
    {
        if (mBodyHealth - aDamageRemaining > 0)
            TakeBodyDamage(aDamageRemaining, theDamageFlags);
        else
            ApplyBurn();
    }
    
}

//0x531880
float Zombie::GetPosYBasedOnRow(int theRow)
{
    if (!IsOnBoard())
        return 0.0f;

    if (IsOnHighGround())
    {
        if (mAltitude < HIGH_GROUND_HEIGHT)
        {
            mZombieHeight = ZombieHeight::HEIGHT_UP_TO_HIGH_GROUND;
        }
        mOnHighGround = true;
    }

    float aPosY = mBoard->GetPosYBasedOnRow(mPosX + 40.0f, theRow) - 30.0f;
    if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        aPosY -= 30.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_POGO)
    {
        aPosY -= 16.0f;
    }

    return aPosY;
}

//0x531930
Zombie::~Zombie()
{
    AttachmentDie(mAttachmentID);
    StopZombieSound();
}

//0x531990
bool Zombie::CanBeChilled()
{
    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || IsBobsledTeamWithSled() || 
        mZombieType == ZombieType::ZOMBIE_YETI)
        return false;

    if (IsDeadOrDying())
        return false;

    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE ||
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
        return false;

    if (mMindControlled)
        return false;

    return
        mZombieType != ZombieType::ZOMBIE_BOSS ||
        mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT ||
        mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT ||
        mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_SPIT;
}

//0x531A10
bool Zombie::CanBeFrozen()
{
    if (!CanBeChilled())
        return false;

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT ||
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL ||
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP ||
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL ||
        IsFlying() ||
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN ||
        mZombiePhase == ZombiePhase::PHASE_IMP_LANDING ||
        mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING ||
        mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING ||
        mZombiePhase == ZombiePhase::PHASE_SQUASH_RISING ||
        mZombiePhase == ZombiePhase::PHASE_SQUASH_FALLING ||
        mZombiePhase == ZombiePhase::PHASE_SQUASH_DONE_FALLING ||
        IsBouncingPogo())
        return false;
        
    return mZombieType != ZombieType::ZOMBIE_BUNGEE || mZombiePhase == ZombiePhase::PHASE_BUNGEE_AT_BOTTOM;
}

//0x531A80
bool Zombie::EffectedByDamage(unsigned int theDamageRangeFlags)
{
    if (!TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_DYING) && IsDeadOrDying())
    {
        return false;
    }

    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_ONLY_MINDCONTROLLED))
    {
        if (!mMindControlled)
        {
            return false;
        }
    }
    else if (mMindControlled)
    {
        return false;
    }

    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_INTANGIBLE) return false;

    if (mZombieType == ZombieType::ZOMBIE_BUNGEE && mZombiePhase != ZombiePhase::PHASE_BUNGEE_AT_BOTTOM && mZombiePhase != ZombiePhase::PHASE_BUNGEE_GRABBING)
    {
        return false;  // 蹦极僵尸只有在停留时才会受到攻击
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED)
    {
        return false;  // 被空投的过程中不会受到攻击
    }

    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_ENTER && aBodyReanim->mAnimTime < 0.5f)
        {
            return false;
        }
        if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_LEAVE && aBodyReanim->mAnimTime > 0.5f)
        {
            return false;
        }

        if (mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT && 
            mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT && 
            mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_SPIT)
        {
            return false;  // 僵王博士只有在低头状态下才会受到攻击
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_BOBSLED && GetBobsledPosition() > 0)
    {
        return false;  // 存在雪橇时，只有领头僵尸会受到攻击
    }

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || 
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || 
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_BALLOON_POPPING || 
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || 
        mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
    {
        return TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_OFF_GROUND);
    }

    if (mZombieType != ZombieType::ZOMBIE_BOBSLED && GetZombieRect().mX > WIDE_BOARD_WIDTH)
    {
        return false;  // 除雪橇僵尸小队外，场外的僵尸不会受到攻击
    }

    bool submerged = (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER) && mInPool && !mIsEating;
    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_SUBMERGED) && submerged)
    {
        return true;
    }

    bool underground = mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING;
    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_UNDERGROUND) && underground)
    {
        return true;
    }

    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_FLYING) && IsFlying())
    {
        return true;
    }

    return TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_GROUND) && !IsFlying() && !submerged && !underground;
}

void Zombie::SetRow(int theRow)
{
    TOD_ASSERT(theRow >= 0 && theRow <= MAX_GRID_SIZE_Y);

    mRow = theRow;
    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, mRow, 4);
}

//0x531C90
void Zombie::RiseFromGrave(int theCol, int theRow)
{
    //TOD_ASSERT(mZombiePhase == ZombiePhase::PHASE_ZOMBIE_NORMAL);

    mPosX = mBoard->GridToPixelX(theCol, mRow) - 25;
    mPosY = GetPosYBasedOnRow(theRow);
    SetRow(theRow);
    mX = (int)mPosX;
    mY = (int)mPosY;
    mAltitude = CLIP_HEIGHT_OFF;
    mZombiePhase = ZombiePhase::PHASE_RISING_FROM_GRAVE;
    mPhaseCounter = 150;
    
    if (mBoard->StageHasPool())
    {
        mAltitude = -150.0f;
        mInPool = true;
        mPhaseCounter = 50;
        mZombieHeight = ZombieHeight::HEIGHT_ZOMBIE_NORMAL;

        StartWalkAnim(0);
        ReanimIgnoreClipRect("Zombie_duckytube", false);
        ReanimIgnoreClipRect("Zombie_whitewater", false);
        ReanimIgnoreClipRect("Zombie_outerarm_hand", false);
        ReanimIgnoreClipRect("Zombie_innerarm3", false);
        ReanimIgnoreClipRect("anim_screendoor", false);

        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        TodParticleSystem* aParticle = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        OverrideParticleScale(aParticle);

        if (mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE && aParticle)
        {
            aBodyReanim->AttachParticleToTrack("anim_cone", aParticle, 37.0f, 20.0f);
        }
        else if (mZombieType == ZombieType::ZOMBIE_PAIL && aParticle)
        {
            aBodyReanim->AttachParticleToTrack("anim_bucket", aParticle, 37.0f, 20.0f);
        }
        else if (aParticle)
        {
            aBodyReanim->AttachParticleToTrack("anim_head1", aParticle, 30.0f, 20.0f);
        }

        TodParticleSystem* aParticle2 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        if (aParticle2)
        {
            OverrideParticleScale(aParticle2);
            aBodyReanim->AttachParticleToTrack("Zombie_outerarm_upper", aParticle2, 5.0f, 5.0f);
        }

        TodParticleSystem* aParticle3 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        if (aParticle3)
        {
            OverrideParticleScale(aParticle3);
            aBodyReanim->AttachParticleToTrack("Zombie_duckytube", aParticle3, 77.0f, 20.0f);
        }

        PoolSplash(false);
    }
    else
    {
        int aParticleX = mPosX + 60;
        int aParticleY = mPosY + 110;
        if (IsOnHighGround())
        {
            aParticleY -= HIGH_GROUND_HEIGHT;
        }

        int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, theRow, 0);
        if (mApp->IsWhackAZombieLevel())
        {
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
            mApp->AddTodParticle(aParticleX, aParticleY, aRenderOrder, ParticleEffect::PARTICLE_WHACK_A_ZOMBIE_RISE);
        }
        else
        {
            mApp->PlayFoley(FoleyType::FOLEY_GRAVESTONE_RUMBLE);
            mApp->AddTodParticle(aParticleX, aParticleY, aRenderOrder, ParticleEffect::PARTICLE_ZOMBIE_RISE);
        }
    }
}

bool Zombie::IsZombotany(ZombieType theZombieType)
{
    return
        theZombieType == ZombieType::ZOMBIE_PEA_HEAD || 
        theZombieType == ZombieType::ZOMBIE_REPEATER_HEAD ||
        theZombieType == ZombieType::ZOMBIE_WALLNUT_HEAD || 
        theZombieType == ZombieType::ZOMBIE_CHERRYBOMB_HEAD ||
        theZombieType == ZombieType::ZOMBIE_TALLNUT_HEAD || 
        theZombieType == ZombieType::ZOMBIE_JALAPENO_HEAD || 
        theZombieType == ZombieType::ZOMBIE_GATLING_HEAD ||
        theZombieType == ZombieType::ZOMBIE_SQUASH_HEAD ||
        theZombieType == ZombieType::ZOMBIE_HYPNOSHROOM_HEAD ||
        theZombieType == ZombieType::ZOMBIE_EXPLODE_O_NUT_HEAD;
}

//0x5320B0
bool Zombie::ZombieTypeCanGoInPool(ZombieType theZombieType)
{
    return 
        theZombieType == ZombieType::ZOMBIE_NORMAL ||
        theZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE || 
        theZombieType == ZombieType::ZOMBIE_PAIL || 
        theZombieType == ZombieType::ZOMBIE_FLAG || 
        theZombieType == ZombieType::ZOMBIE_SNORKEL || 
        theZombieType == ZombieType::ZOMBIE_DIVER ||
        theZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER || 
        theZombieType == ZombieType::ZOMBIE_SHARK_RIDER ||
        IsZombotany(theZombieType);
}

bool Zombie::ZombieTypeCanGoOnHighGround(ZombieType theZombieType)
{
    return theZombieType != ZombieType::ZOMBIE_ZAMBONI && theZombieType != ZombieType::ZOMBIE_BOBSLED;
}

//0x5320B0
Rect Zombie::GetZombieRect()
{
    Rect aZombieRect = mZombieRect;
    if (IsWalkingBackwards())
    {
        aZombieRect.mX = mWidth - aZombieRect.mX - aZombieRect.mWidth;
    }

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    aZombieRect.Offset(mX, mY + aDrawPos.mBodyY);
    if (aDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT)
    {
        aZombieRect.mHeight -= aDrawPos.mClipHeight;
    }

    return aZombieRect;
}

//0x532140
Rect Zombie::GetZombieAttackRect()
{
    Rect aAttackRect = mZombieAttackRect;
    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP)
    {
        aAttackRect = Rect(-40, 0, 100, 115);
    }

    if (IsWalkingBackwards())
    {
        aAttackRect.mX = mWidth - aAttackRect.mX - aAttackRect.mWidth;
    }

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    aAttackRect.Offset(mX, mY + aDrawPos.mBodyY);
    if (aDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT)
    {
        aAttackRect.mHeight -= aDrawPos.mClipHeight;
    }

    return aAttackRect;
}

//0x5321F0
TodParticleSystem* Zombie::AddAttachedParticle(int thePosX, int thePosY, ParticleEffect theEffect)
{
    if (mDead)
        return nullptr;

    if (IsFullOfAttachments(mAttachmentID))
        return nullptr;

    TodParticleSystem* aParticle = mApp->AddTodParticle(mX + thePosX, mY + thePosY, 0, theEffect);
    if (aParticle)
    {
        AttachParticle(mAttachmentID, aParticle, thePosX, thePosY);
    }

    return aParticle;
}

//0x5322C0
Reanimation* Zombie::AddAttachedReanim(int thePosX, int thePosY, ReanimationType theReanimType)
{
    if (mDead)
        return nullptr;

    Reanimation* aReanim = mApp->AddReanimation(mX + thePosX, mY + thePosY, 0, theReanimType);
    if (aReanim)
    {
        AttachReanim(mAttachmentID, aReanim, thePosX, thePosY);
    }

    return aReanim;
}

//0x532350
void Zombie::RemoveIceTrap()
{
    mIceTrapCounter = 0;
    if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        BalloonPropellerHatSpin(true);
    }

    UpdateAnimSpeed();
    StartZombieSound();
}

void Zombie::HitIceTrap()
{
    HitIceTrap(300);
}
//0x5323C0
void Zombie::HitIceTrap(unsigned int duration)
{
    bool cold = false;
    if (mChilledCounter > 0 || mIceTrapCounter != 0)
    {
        cold = true;
    }

    ApplyChill(true);
    if (!CanBeFrozen())
        return;
    
    if (mInPool)
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

    StopZombieSound();
    if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        BalloonPropellerHatSpin(false);
    }
    if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_SPIT)
    {
        mBoard->RemoveParticleByType(ParticleEffect::PARTICLE_ZOMBIE_BOSS_FIREBALL);
    }

    TakeDamage(20, 1U);
    UpdateAnimSpeed();
}

//0x5324B0
bool Zombie::IsTangleKelpTarget()
{
    if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER)
        return true;

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mSeedType == SeedType::SEED_TANGLEKELP && aPlant->mTargetZombieID == mBoard->ZombieGetID(this))
        {
            return true;
        }
    }

    return false;
}

bool Zombie::IsSquashTarget(Plant* theExcept)
{
    ZombieID anId = mBoard->ZombieGetID(this);

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant != theExcept &&  aPlant->mSeedType == SeedType::SEED_SQUASH && aPlant->mTargetZombieID == anId)
        {
            return true;
        }
    }

    return false;
}

bool Zombie::IsFireResistant()
{
    return 
        mZombieType == ZombieType::ZOMBIE_CATAPULT || 
        mZombieType == ZombieType::ZOMBIE_ZAMBONI || 
        mShieldType == ShieldType::SHIELDTYPE_DOOR || 
        mShieldType == ShieldType::SHIELDTYPE_LADDER;
}

//0x532510
void Zombie::BalloonPropellerHatSpin(bool theSpinning)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aHatTrackInstance = aBodyReanim->GetTrackInstanceByName("hat");
    Reanimation* aPropellerReanim = FindReanimAttachment(aHatTrackInstance->mAttachmentID);
    if (aPropellerReanim)
    {
        if (theSpinning)
        {
            aPropellerReanim->mAnimRate = aPropellerReanim->mDefinition->mFPS;
        }
        else
        {
            aPropellerReanim->mAnimRate = 0.0f;
        }
    }
}

//0x532570
void Zombie::RemoveButter()
{
    if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        BalloonPropellerHatSpin(true);
    }

    if (Zombie::IsZombotany(mZombieType))
    {
        Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mPlantHeadID);
        if (aHeadReanim)
        {
            if (mZombieType == ZombieType::ZOMBIE_PEA_HEAD && aHeadReanim->IsAnimPlaying("anim_shooting"))
            {
                aHeadReanim->mAnimRate = 35.0f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_REPEATER_HEAD && aHeadReanim->IsAnimPlaying("anim_shooting"))
            {
                aHeadReanim->mAnimRate = 45.0f;
            }
            else if (mZombieType == ZombieType::ZOMBIE_GATLING_HEAD && aHeadReanim->IsAnimPlaying("anim_shooting"))
            {
                aHeadReanim->mAnimRate = 38.0f;
            }
            else
            {
                aHeadReanim->mAnimRate = 15.0f;
            }
        }
    }

    UpdateAnimSpeed();
    StartZombieSound();
}

//0x5326D0
void Zombie::ApplyButter()
{
    if (!mHasHead || !CanBeFrozen())
        return;

    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_BOSS || IsTangleKelpTarget() || IsBobsledTeamWithSled() || IsFlying())
        return;

    mButteredCounter = 400;
    Zombie* aZombie = mBoard->ZombieTryToGet(mRelatedZombieID);
    if (aZombie)
    {
        aZombie->mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
        mRelatedZombieID = ZombieID::ZOMBIEID_NULL;
    }

    if (mZombieType == ZombieType::ZOMBIE_POGO)
    {
        mAltitude = 0.0f;
        if (mOnHighGround)
        {
            mAltitude += HIGH_GROUND_HEIGHT;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        BalloonPropellerHatSpin(false);
    }
    else if (Zombie::IsZombotany(mZombieType))
    {
        Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mPlantHeadID);
        if (aHeadReanim)
        {
            aHeadReanim->mAnimRate = 0.0f;
        }
    }

    UpdateAnimSpeed();
    StopZombieSound();
}

//0x5327E0
void Zombie::MowDown()
{
    if (mDead || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED || mZombieType == ZombieType::ZOMBIE_BOSS)
        return;

    if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_CATAPULT_EXPLOSION);
        mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
        DieWithLoot();
        return;
    }

    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_EXPLOSION);
        mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
        DieWithLoot();
        return;
    }

    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || 
        mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || 
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || 
        mZombieType == ZombieType::ZOMBIE_GARGANTUAR || 
        mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || 
        mZombieType == ZombieType::ZOMBIE_BUNGEE || 
        mZombieType == ZombieType::ZOMBIE_DIGGER || 
        mZombieType == ZombieType::ZOMBIE_NINJAIMP ||
        mZombieType == ZombieType::ZOMBIE_IMP || 
        mZombieType == ZombieType::ZOMBIE_YETI || 
        mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER || 
        IsBobsledTeamWithSled() || 
        IsFlying() || 
        mInPool)
    {
        Reanimation* aPuffReanim = mApp->AddReanimation(mPosX - 73.0f, mPosY - 56.0f, mRenderOrder + 2, ReanimationType::REANIM_PUFF);
        aPuffReanim->SetFramesForLayer("anim_puff");
        mApp->AddTodParticle(mPosX + 110.0f, mPosY + 0.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MOWER_CLOUD);

        if (mBoard->mPlantRow[mRow] != PlantRowType::PLANTROW_POOL)
        {
            DropHead(0U);
            DropArm(0U);
            DropHelm(0U);
            DropShield(0U);
        }

        DieWithLoot();
        return;
    }

    if (mIceTrapCounter > 0)
    {
        RemoveIceTrap();
    }
    if (mButteredCounter > 0)
    {
        mButteredCounter = 0;
    }

    DropShield(0U);
    DropHelm(0U);

    if (mZombieType == ZombieType::ZOMBIE_FLAG)
    {
        DropFlag();
    }  
    else if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
    {
        DropPole();
    }
    else if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER || mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        DropHead(0U);
    }
    else if (mZombieType == ZombieType::ZOMBIE_POGO)
    {
        DropHead(0U);
        mAltitude = 0.0f;
    }

    Reanimation* aMoweredReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder, ReanimationType::REANIM_LAWN_MOWERED_ZOMBIE);
    aMoweredReanim->mAnimRate = 8.0f;
    aMoweredReanim->mIsAttachment = false;
    aMoweredReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    mMoweredReanimID = mApp->ReanimationGetID(aMoweredReanim);
    mZombiePhase = ZombiePhase::PHASE_ZOMBIE_MOWERED;
    DropLoot();
}

//0x532B40
void Zombie::RemoveColdEffects()
{
    if (mIceTrapCounter > 0)
    {
        RemoveIceTrap();
    }

    if (mChilledCounter > 0)
    {
        mChilledCounter = 0;
        UpdateAnimSpeed();
    }
}

//0x532B70
void Zombie::ApplyBurn()
{
    if (mDead || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED)
        return;

    if (mBodyHealth + mHelmHealth + mShieldHealth > 1800 || mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        TakeDamage(1800, 18U);
        return;
    }

    if (mZombieType == ZombieType::ZOMBIE_SQUASH_HEAD && !mHasHead)
    {
        mApp->RemoveReanimation(mPlantHeadID);
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;
    }

    if (mIceTrapCounter > 0)
    {
        RemoveIceTrap();
    }
    if (mButteredCounter > 0)
    {
        mButteredCounter = 0;
    }

    AttachmentDetachCrossFadeParticleType(mAttachmentID, ParticleEffect::PARTICLE_ZAMBONI_SMOKE, nullptr);
    BungeeDropPlant();

    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || 
        mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || 
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || 
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || 
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || 
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING || 
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || 
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || 
        mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED || mInPool) 
    {
        DieWithLoot();
    }
    else if (mZombieType == ZOMBIE_BUNGEE || mZombieType == ZOMBIE_YETI || Zombie::IsZombotany(mZombieType) || IsBobsledTeamWithSled() || IsFlying() || !mHasHead)
    {
        SetAnimRate(0.0f);
        Reanimation* aHeadReanim = mApp->ReanimationTryToGet(Zombie::IsZombotany(mZombieType) ? mPlantHeadID : mSpecialHeadReanimID);
        if (aHeadReanim)
        {
            aHeadReanim->mAnimRate = 0.0f;
        }

        Reanimation* aFlagReanim = mApp->ReanimationTryToGet(mFlagReanimID);
        if (aFlagReanim)
        {
            aFlagReanim->mAnimRate = 0.0f;
        }

        mZombiePhase = ZombiePhase::PHASE_ZOMBIE_BURNED;
        mPhaseCounter = 300;
        mJustGotShotCounter = 0;
        DropLoot();

        if (mZombieType == ZombieType::ZOMBIE_BALLOON)
        {
            BalloonPropellerHatSpin(false);
        }
    }
    else
    {
        ReanimationType aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED;
        float aCharredPosX = mPosX + 22.0f;
        float aCharredPosY = mPosY - 10.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0);
        if (mZombieType == ZombieType::ZOMBIE_BALLOON)
        {
            aCharredPosY += 31.0f;
        }
        if (mZombieType == ZombieType::ZOMBIE_IMP || mZombieType == ZombieType::ZOMBIE_NINJAIMP)
        {
            aCharredPosX -= 6.0f;
            aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED_IMP;
        }
        if (mZombieType == ZombieType::ZOMBIE_DIGGER)
        {
            if (IsWalkingBackwards())
            {
                aCharredPosX += 14.0f;
            }
            aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED_DIGGER;
        }
        if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
        {
            aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED_ZAMBONI;
            aCharredPosX += 61.0f;
            aCharredPosY -= 16.0f;
        }
        if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
        {
            aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED_CATAPULT;
            aCharredPosX -= 36.0f;
            aCharredPosY -= 20.0f;
        }
        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
            aReanimType = ReanimationType::REANIM_ZOMBIE_CHARRED_GARGANTUAR;
            aCharredPosX -= 15.0f;
            aCharredPosY -= 10.0f;
        }

        Reanimation* aCharredReanim = mApp->AddReanimation(aCharredPosX, aCharredPosY, mRenderOrder, aReanimType);
        aCharredReanim->mAnimRate *= RandRangeFloat(0.9f, 1.1f);
        if (mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING_WITHOUT_AXE)
        {
            aCharredReanim->SetFramesForLayer("anim_crumble_noaxe");
        }
        else if (mZombieType == ZombieType::ZOMBIE_DIGGER)
        {
            aCharredReanim->SetFramesForLayer("anim_crumble");
        }
        else if ((mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) && !mHasObject)
        {
            aCharredReanim->SetImageOverride("impblink", IMAGE_BLANK);
            aCharredReanim->SetImageOverride("imphead", IMAGE_BLANK);
        }

        if (mScaleZombie != 1.0f)
        {
            aCharredReanim->mOverlayMatrix.m00 = mScaleZombie;
            aCharredReanim->mOverlayMatrix.m11 = mScaleZombie;
            aCharredReanim->mOverlayMatrix.m02 += 20.0f - mScaleZombie * 20.0f;
            aCharredReanim->mOverlayMatrix.m12 += 120.0f - mScaleZombie * 120.0f;
            aCharredReanim->OverrideScale(mScaleZombie, mScaleZombie);
        }

        if (IsWalkingBackwards())
        {
            aCharredReanim->OverrideScale(-mScaleZombie, mScaleZombie);
            aCharredReanim->mOverlayMatrix.m02 += 60.0f * mScaleZombie;
        }

        DieWithLoot();
    }

    if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
    {
        BobsledBurn();
    }
}

void Zombie::AttachHelmet() {
    const char* aTrackName;
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mHelmType == HELMTYPE_TRAFFIC_CONE)
    {
        aTrackName = "anim_cone";
    }
    else if (mHelmType == HELMTYPE_PAIL)
    {
        aTrackName = "anim_bucket";
    }
    else if (mHelmType == HELMTYPE_FOOTBALL)
    {
        aTrackName = "zombie_football_helmet";
    }
    else if (mHelmType == HELMTYPE_DIGGER)
    {
        aTrackName = "Zombie_digger_hardhat";
    }
    else if (mHelmType == HELMTYPE_BOBSLED) {

    }
    else if (mHelmType == HELMTYPE_DIVER) {
       aTrackName = "Zombie_diver";
    }

    aBodyReanim->AssignRenderGroupToTrack(aTrackName, RENDER_GROUP_HELMET);
}
//0x533000
void Zombie::AttachShield()
{
    const char* aTrackName;
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        ShowDoorArms(true);
        ReanimShowPrefix("Zombie_outerarm_screendoor", RENDER_GROUP_SHIELD_ARM);
        aTrackName = "anim_screendoor";

        if (mZombieType == ZOMBIE_GLASSDOOR)
            aBodyReanim->SetImageOverride(aTrackName, IMAGE_REANIM_ZOMBIE_GLASSDOOR1);
    }
    else if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER)
    {
        ReanimShowPrefix("Zombie_paper_hands", RENDER_GROUP_SHIELD_ARM);
        aTrackName = "Zombie_paper_paper";
    }
    else if (mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        ReanimShowPrefix("Zombie_outerarm", RENDER_GROUP_SHIELD_ARM);
        aTrackName = "Zombie_ladder_1";
    }

    if (aTrackName)
        aBodyReanim->AssignRenderGroupToTrack(aTrackName, RENDER_GROUP_SHIELD);
}

//0x5330E0
void Zombie::DetachShield()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        if (mShieldType == ShieldType::SHIELDTYPE_DOOR)
        {
            ShowDoorArms(false);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_NEWSPAPER)
        {
            ReanimShowPrefix("Zombie_paper_hands", RENDER_GROUP_NORMAL);
        }
        else if (mShieldType == ShieldType::SHIELDTYPE_LADDER)
        {
//#ifdef DO_FIX_BUGS
            if (mHasArm)  // 修复扶梯僵尸搭梯后断臂重生的 Bug
            {
                ReanimShowPrefix("Zombie_outerarm", RENDER_GROUP_NORMAL);
            }
/*#else
            ReanimShowPrefix("Zombie_outerarm", RENDER_GROUP_NORMAL);
#endif*/
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            if (mIsEating)
            {
                PlayZombieReanim("anim_eat", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
            }
            else
            {
                StartWalkAnim(0);
            }
        }
        else
        {
            TOD_ASSERT();
        }
    }

    mShieldType = ShieldType::SHIELDTYPE_NONE;
    mShieldHealth = 0;
}

//0x5331C0
void Zombie::ReanimShowPrefix(const char* theTrackPrefix, int theRenderGroup)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->AssignRenderGroupToPrefix(theTrackPrefix, theRenderGroup);
    }
}

//0x533200
void Zombie::ReanimShowTrack(const char* theTrackName, int theRenderGroup)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->AssignRenderGroupToTrack(theTrackName, theRenderGroup);
    }
}

//0x533240
void Zombie::PlayDeathAnim(unsigned int theDamageFlags)
{
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr || !aBodyReanim->TrackExists("anim_death"))
    {
        DieNoLoot();
        return;
    }

    /*if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER && mZombiePhase != ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
    {
        DieNoLoot();
        return;
    }*/

    if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING)
    {
        DieNoLoot();
        return;
    }

    if (mIceTrapCounter > 0)
    {
        AddAttachedParticle(75, 106, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
        mIceTrapCounter = 0;
    }
    if (mButteredCounter > 0)
    {
        mButteredCounter = 0;
    }
    if (mYuckyFace)
    {
        ShowYuckyFace(false);
        mYuckyFace = false;
        mYuckyFaceCounter = 0;
    }

    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_LEAVE_BODY))
    {
        if (mZombieType != ZombieType::ZOMBIE_BOSS && mZombieType != ZombieType::ZOMBIE_GARGANTUAR && mZombieType != ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
            DieNoLoot();
            return;
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_POGO)
    {
        mAltitude = 0.0f;
    }

    AttachmentReanimTypeDie(mAttachmentID, ReanimationType::REANIM_ZOMBIE_SURPRISE);
    StopEating();

    if (mShieldType != ShieldType::SHIELDTYPE_NONE)
    {
        DropShield(1U);
    }
    if (mZombieType == ZombieType::ZOMBIE_SQUASH_HEAD && !mHasHead)
    {
        mApp->RemoveReanimation(mPlantHeadID);
        mPlantHeadID = ReanimationID::REANIMATIONID_NULL;
    }

    mVelX = 0.0f;

    const char* aDeathTrackName = "anim_death";

    if (mZombiePhase == PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN)
    {
        mAltitude = 0;
        ReanimDisableClipping();
        aDeathTrackName = "anim_death_land";
    }

    mZombiePhase = ZombiePhase::PHASE_ZOMBIE_DYING;

    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM)
    {
        PlayZombieReanim("anim_aquarium_death", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 14.0f);
        return;
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_UP_LADDER)
    {
        mZombieHeight = ZombieHeight::HEIGHT_FALLING;
    }

    float aDeathAnimRate;
    if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
    {
        aDeathAnimRate = 24.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        aDeathAnimRate = 14.0f;

        if (mIsInstantDeath)
        {
            aDeathTrackName = "anim_instant_death";
        }
        else
        {
            mApp->PlayFoley(FoleyType::FOLEY_GARGANTUDEATH);
        }
        
    }
    else if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        aDeathAnimRate = 14.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        aDeathAnimRate = 18.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        aDeathAnimRate = 14.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        aDeathAnimRate = 18.0f;

        BossDie();
        Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
        aHeadReanim->PlayReanim("anim_death", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aDeathAnimRate);
    }
    else
    {
        aDeathAnimRate = RandRangeFloat(24.0f, 30.0f);
    }

    int aDeathAnimHit = Rand(100);
    bool aCanDoSuperLongDeath = mApp->HasFinishedAdventure() || mBoard->mLevel > 5;


    if (mInPool && aBodyReanim->TrackExists("anim_waterdeath"))
    {
        aDeathTrackName = "anim_waterdeath";
        ReanimIgnoreClipRect("Zombie_duckytube", false);
    }
    else if (aDeathAnimHit == 99 && aBodyReanim->TrackExists("anim_superlongdeath") && aCanDoSuperLongDeath && mChilledCounter == 0 && mBoard->CountZombiesOnScreen() <= 5)
    {
        aDeathAnimRate = 14.0f;
        aDeathTrackName = "anim_superlongdeath";
    }
    else if (aDeathAnimHit > 50 && aBodyReanim->TrackExists("anim_death2"))
    {
        aDeathTrackName = "anim_death2";
    }


    if (mIsAllergicToNuts && mHasHead) {
        aDeathAnimRate = 14.0f;
        aDeathTrackName = "anim_walnutdeath";
    }


    PlayZombieReanim(aDeathTrackName, ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aDeathAnimRate);
    ReanimShowPrefix("anim_tongue", RENDER_GROUP_HIDDEN);
}

//0x5335A0
void Zombie::DoDaisies()
{
    if (IsWalkingBackwards())
        return;

    if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
        return;

    if (mZombieType == ZombieType::ZOMBIE_BOBSLED || mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT)
        return;

    if (mBoard->StageHasRoof())
        return;

    float aOffsetX = 20.0f;
    float aOffsetY = 100.0f;
    if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL  || mZombieType == ZombieType::ZOMBIE_DISCO || mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_DISCO_BACKUP)
    {
        aOffsetX += 160.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_POGO)
    {
        aOffsetY += 120.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        aOffsetY += 30.0f;
        aOffsetX += 110.0f;
    }
    if (mBoard->StageHasGraveStones())
    {
        aOffsetY += 15.0f;
    }

    int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow, 5);
    mApp->AddTodParticle(mX + aOffsetX, mY + aOffsetY, aRenderPosition, ParticleEffect::PARTICLE_ZOMBIE_DAISIES);
    
}

//0x5336B0
void Zombie::UpdateDeath()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
    {
        DieNoLoot();
        return;
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_FALLING)
    {
        UpdateZombieFalling();
    }
    if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.89f))
        {
            mBoard->ShakeBoard(0, 3);
        }
        else if (aBodyReanim->ShouldTriggerTimedEvent(0.98f))
        {
            mBoard->ShakeBoard(0, 1);
        }
    }

    if (!mInPool)
    {
        float aFallTime;
        switch (mZombieType)
        {
        case ZombieType::ZOMBIE_DIVER:
        case ZombieType::ZOMBIE_SNORKEL:
        case ZombieType::ZOMBIE_ZAMBONI:
        case ZombieType::ZOMBIE_DOLPHIN_RIDER:
        case ZombieType::ZOMBIE_BUNGEE:
        case ZombieType::ZOMBIE_CATAPULT:
        case ZombieType::ZOMBIE_IMP:
        case ZombieType::ZOMBIE_NINJAIMP:
        case ZombieType::ZOMBIE_BOSS:
            aFallTime = -1.0f;
            break;

        case ZombieType::ZOMBIE_NORMAL:
        case ZombieType::ZOMBIE_FLAG:
        case ZombieType::ZOMBIE_TRAFFIC_CONE:
        case ZombieType::ZOMBIE_PAIL:
        case ZombieType::ZOMBIE_GLASSDOOR:
        case ZombieType::ZOMBIE_DOOR:
        case ZombieType::ZOMBIE_PEA_HEAD:
        case ZombieType::ZOMBIE_REPEATER_HEAD:
        case ZombieType::ZOMBIE_WALLNUT_HEAD:
        case ZombieType::ZOMBIE_CHERRYBOMB_HEAD:
        case ZombieType::ZOMBIE_TALLNUT_HEAD:
        case ZombieType::ZOMBIE_JALAPENO_HEAD:
        case ZombieType::ZOMBIE_GATLING_HEAD:
        case ZombieType::ZOMBIE_SQUASH_HEAD:
        case ZombieType::ZOMBIE_HYPNOSHROOM_HEAD:
        case ZombieType::ZOMBIE_EXPLODE_O_NUT_HEAD:
        case ZombieType::ZOMBIE_DUCKY_TUBE:
            if (aBodyReanim->IsAnimPlaying("anim_superlongdeath"))
            {
                aFallTime = 0.788f;
            }
            else if (aBodyReanim->IsAnimPlaying("anim_death2"))
            {
                aFallTime = 0.71f;
            }
            else
            {
                aFallTime = 0.77f;
            }
            break;

        case ZombieType::ZOMBIE_POLEVAULTER:
            aFallTime = 0.68f;
            break;

        case ZombieType::ZOMBIE_BLACK_FOOTBALL:
        case ZombieType::ZOMBIE_FOOTBALL:
            aFallTime = 0.52f;
            break;

        case ZombieType::ZOMBIE_NEWSPAPER:
            aFallTime = 0.63f;
            break;

        case ZombieType::ZOMBIE_DISCO_BACKUP:
        case ZombieType::ZOMBIE_DISCO:
        case ZombieType::ZOMBIE_DANCER:
        case ZombieType::ZOMBIE_BACKUP_DANCER:
            aFallTime = 0.83f;
            break;

        case ZombieType::ZOMBIE_BOBSLED:
            aFallTime = 0.81f;
            break;

        case ZombieType::ZOMBIE_JACK_IN_THE_BOX:
            aFallTime = 0.64f;
            break;

        case ZombieType::ZOMBIE_BALLOON:
            aFallTime = 0.68f;
            break;

        case ZombieType::ZOMBIE_DIGGER:
            aFallTime = 0.85f;
            break;

        case ZombieType::ZOMBIE_POGO:
            aFallTime = 0.84f;
            break;

        case ZombieType::ZOMBIE_YETI:
            aFallTime = 0.68f;
            break;

        case ZombieType::ZOMBIE_LADDER:
            aFallTime = 0.62f;
            break;

        case ZombieType::ZOMBIE_GARGANTUAR:
        case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
            aFallTime = 0.86f;
            break;

        default:
            aFallTime = -1.0f;
            break;
        }

        if (aFallTime > 0 && aBodyReanim->ShouldTriggerTimedEvent(aFallTime))
        {
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_FALLING);
            if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
            {
                mApp->PlayFoley(FoleyType::FOLEY_THUMP);
            }

            if (mBoard->mDaisyMode)
            {
               DoDaisies();
            }
        }
    }
    
    if (mZombieType == ZombieType::ZOMBIE_BOSS)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.1f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.12f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.15f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.19f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.2f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.26f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.3f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.4f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.42f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.5f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.58f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.61f) || 
            aBodyReanim->ShouldTriggerTimedEvent(0.71f))
        {
            float aExplosionPosX = RandRangeFloat(600.0f, 750.0f);
            float aExplosionPosY = RandRangeFloat(50.0f, 300.0f);
            mApp->AddTodParticle(aExplosionPosX, aExplosionPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_BOSS_EXPLOSION);
            mApp->PlayFoley(FoleyType::FOLEY_BOSS_EXPLOSION_SMALL);
        }

        Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mSpecialHeadReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.93f))
        {
            mBoard->ShakeBoard(1, 2);
            mApp->PlayFoley(FoleyType::FOLEY_BOSS_EXPLOSION_SMALL);
            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
        }

        if (aBodyReanim->ShouldTriggerTimedEvent(0.99f))
        {
            aHeadReanim->PlayReanim("anim_flag", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        }

        if (aHeadReanim->IsAnimPlaying("anim_flag") && aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->PlayReanim("anim_flag_loop", ReanimLoopType::REANIM_LOOP, 20, 17.0f);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            DropLoot();
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI && mPhaseCounter > 0)
    {
        mPhaseCounter--;
        if (mPhaseCounter == 0)
        {
            aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim->IsTrackShowing("anim_wheelie2"))
            {
                mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_EXPLOSION2);
            }
            else
            {
                mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f - (IsOnHighGround() ? HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_ZAMBONI_EXPLOSION);
            }

            DieWithLoot();
            mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        if (mChilledCounter > 0 && mZombieAge % 2 == 1 || mChilledCounter == 0)
            mPhaseCounter--;

        if (mPhaseCounter == 0)
        {
            mApp->AddTodParticle(mPosX + 80.0f, mPosY + 60.0f - (IsOnHighGround() ?  HIGH_GROUND_HEIGHT : 0), mRenderOrder + 1, ParticleEffect::PARTICLE_CATAPULT_EXPLOSION);
            DieWithLoot();
            mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
        }
    }
    else if (mZombieFade == -1 && aBodyReanim->mLoopCount > 0 && mZombieType != ZombieType::ZOMBIE_BOSS)
    {
        mZombieFade = mInPool ? 10 : 100;
    }
}

//0x533F10
void Zombie::UpdateMowered()
{
    Reanimation* aMoweredReanim = mApp->ReanimationTryToGet(mMoweredReanimID);
    if (aMoweredReanim == nullptr || aMoweredReanim->mLoopCount > 0)
    {
        DropHead(0U);
        DropArm(0U);
        DieWithLoot();
    }
}

//0x533F70
bool Zombie::HasShadow()
{
    if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE ||
        mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
        mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE ||
        mZombiePhase == ZombiePhase::PHASE_DANCER_RISING ||
        mZombiePhase == ZombiePhase::PHASE_BOBSLED_BOARDING ||
        mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT ||
        mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL ||
        mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL)
        return false;

    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI ||
        mZombieType == ZombieType::ZOMBIE_CATAPULT ||
        mZombieType == ZombieType::ZOMBIE_BOSS)
        return false;

    if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
    {
        if (!IsOnBoard() || mHitUmbrella)
        {
            return false;
        }
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER ||
        mZombieHeight == ZombieHeight::HEIGHT_IN_TO_CHIMNEY ||
        mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED)
        return false;

    if (mInPool)
        return false;

    if (Zombie::IsOnBoard() && mApp->mGameMode == GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
        return false;

    return mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL || mFromWave == Zombie::ZOMBIE_WAVE_UI;
}

//0x534040
bool Zombie::SetupDrawZombieWon(Graphics* g)
{
    if (mFromWave != Zombie::ZOMBIE_WAVE_WINNER)
        return true;

    //if (!mBoard->mCutScene->ShowZombieWalking())
    //   return false;

    switch (mBoard->mBackground)
    {
    case BackgroundType::BACKGROUND_1_DAY:
    case BackgroundType::BACKGROUND_2_NIGHT:
        g->ClipRect(-123 - mX , -mY , BOARD_WIDTH, BOARD_HEIGHT);
        break;
    case BackgroundType::BACKGROUND_3_POOL:
    case BackgroundType::BACKGROUND_4_FOG:
        g->ClipRect(-172 - mX , -mY , BOARD_WIDTH, BOARD_HEIGHT);
        break;
    case BackgroundType::BACKGROUND_5_ROOF:
    case BackgroundType::BACKGROUND_6_BOSS:
        if (mBoard->mCutScene->mCutsceneTime > 1500)
            g->ClipRect(-220 - mX , -mY , BOARD_WIDTH, 187);
        break;
    }

    return true;
}

//0x5340C0
void Zombie::DrawShadow(Graphics* g)
{
    if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON && !SetupDrawZombieWon(g))
        return;

    if (mZombiePhase == PHASE_ZOMBIE_INTANGIBLE)
        return;

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);

    int aShadowType = 0;
    float aShadowOffsetX = aDrawPos.mImageOffsetX;
    float aShadowOffsetY = aDrawPos.mImageOffsetY + aDrawPos.mBodyY;
    float aScale = mScaleZombie;
    aShadowOffsetX += mScaleZombie * 20.0f - 20.0f;

    if (IsOnBoard() && mBoard->StageIsNight())
    {
        aShadowType = 1;
    }

    if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX -= 11.0f * mScaleZombie;
        }
        else
        {
            aShadowOffsetX += 20.0f + 21.0f * mScaleZombie;
        }
        aShadowOffsetY += 16.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER)
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 5.0f;
        }
        else
        {
            aShadowOffsetX += 29.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += -5.0f;
        }
        else
        {
            aShadowOffsetX += 36.0f;
        }
        aShadowOffsetY += 11.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BOBSLED)
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 13.0f;
        }
        else
        {
            aShadowOffsetX += 20.0f;
        }
        aShadowOffsetY += 13.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_IMP || mZombieType == ZombieType::ZOMBIE_NINJAIMP)
    {
        aScale *= 0.6f;
        aShadowOffsetY += 7.0f;

        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 13.0f;
        }
        else
        {
            aShadowOffsetX += 25.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        aShadowOffsetY += 5.0f;
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 14.0f;
        }
        else
        {
            aShadowOffsetX += 17.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_DIVER)
    {
        aShadowOffsetY += 5.0f;
        if (IsWalkingBackwards())
        {
            aShadowOffsetX -= 2.0f;
        }
        else
        {
            aShadowOffsetX += 35.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER)
    {
        aShadowOffsetY += 11.0f;
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 15.0f;
        }
        else
        {
            aShadowOffsetX += 19.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_YETI)
    {
        aShadowOffsetY += 20.0f;
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 20.0f;
        }
        else
        {
            aShadowOffsetX += 3.0f;
        }
    }
    else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        aScale *= 1.5f;
        aShadowOffsetX += 27.0f;
        aShadowOffsetY += 7.0f;
    }
    else if (mApp->ReanimationTryToGet(mBodyReanimID) != nullptr)
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX += 11.0f;
        }
        else
        {
            aShadowOffsetX += 23.0f;
        }
    }
    else
    {
        if (IsWalkingBackwards())
        {
            aShadowOffsetX -= 2.0f;
        }
        else
        {
            aShadowOffsetX += 35.f;
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_NEWSPAPER)
    {
        aShadowOffsetY += 4.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BALLOON)
    {
        aShadowOffsetY += 13.0f;
    }
    else if (mZombieType == ZombieType::ZOMBIE_BUNGEE)
    {
        aShadowOffsetX -= 12.0f;
        aScale = TodAnimateCurveFloat(BUNGEE_ZOMBIE_HEIGHT - 1000, 100, mAltitude, 0.1f, 1.5f, TodCurves::CURVE_LINEAR);
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_UP_LADDER || mZombieHeight == ZombieHeight::HEIGHT_FALLING || 
        mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || mZombieType == ZombieType::ZOMBIE_BUNGEE || IsBouncingPogo() || IsFlying())
    {
        aShadowOffsetY += mAltitude;
        if (mOnHighGround)
        {
            aShadowOffsetY -= HIGH_GROUND_HEIGHT;
        }
    }

    if (mInPool)
    {
        TodDrawImageCenterScaledF(g, IMAGE_WHITEWATER_SHADOW, aShadowOffsetX, aShadowOffsetY + 67.0f, aScale, aScale);
    }
    else
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
        {
            if (mApp->mGameScene == SCENE_PLAYING && mBoard->mChallenge->mChallengeStateCounter <= 500 && mBoard->mChallenge->mChallengeStateCounter >= 0)
            {
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 255 - 255 * (500 - mBoard->mChallenge->mChallengeStateCounter ) / 500));
                if (mBoard->StageIsNight())
                    TodDrawImageCenterScaledF(g, IMAGE_PLANTSHADOW2, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);
                else
                    TodDrawImageCenterScaledF(g, IMAGE_PLANTSHADOW, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);

                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 255 * (500 - mBoard->mChallenge->mChallengeStateCounter) / 500));
                if (!mBoard->StageIsNight())
                    TodDrawImageCenterScaledF(g, IMAGE_PLANTSHADOW2, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);
                else
                    TodDrawImageCenterScaledF(g, IMAGE_PLANTSHADOW, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);
                g->SetColorizeImages(false);
            }
            else
                TodDrawImageCenterScaledF(g, aShadowType == 0 ? IMAGE_PLANTSHADOW : IMAGE_PLANTSHADOW2, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);
        }
        else {
            TodDrawImageCenterScaledF(g, aShadowType == 0 ? IMAGE_PLANTSHADOW : IMAGE_PLANTSHADOW2, aShadowOffsetX, aShadowOffsetY + 92.0f, aScale, aScale);
        }
    }

    g->ClearClipRect();
}

//0x5345F0
void Zombie::GetTrackPosition(const char* theTrackName, float& thePosX, float& thePosY)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
    {
        thePosX = mPosX;
        thePosY = mPosY;
        return;
    }

    int aTrackIndex = aBodyReanim->FindTrackIndex(theTrackName);
    SexyTransform2D aMatrix;
    aBodyReanim->GetTrackMatrix(aTrackIndex, aMatrix);
    thePosX = aMatrix.m02 + mPosX;
    thePosY = aMatrix.m12 + mPosY;
}

//0x534680
bool Zombie::IsFlying()
{
    return mZombiePhase == ZombiePhase::PHASE_BALLOON_FLYING || mZombiePhase == ZombiePhase::PHASE_BALLOON_POPPING;
}

//0x5346A0
int Zombie::GetBobsledPosition()
{
    if (mZombieType != ZombieType::ZOMBIE_BOBSLED)
    {
        return -1;
    }

    if (mRelatedZombieID == ZombieID::ZOMBIEID_NULL && mFollowerZombieID[0] == ZombieID::ZOMBIEID_NULL)
    {
        return -1;
    }

    if (mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
    {
        return 0;
    }

    ZombieID anId = mBoard->ZombieGetID(this);
    Zombie* aLeaderZombie = mBoard->ZombieGet(mRelatedZombieID);
    for (int i = 0; i < NUM_BOBSLED_FOLLOWERS; i++)
    {
        if (aLeaderZombie->mFollowerZombieID[i] == anId)
        {
            return i + 1;
        }
    }

    TOD_ASSERT();
}

bool Zombie::IsBobsledTeamWithSled()
{
    return GetBobsledPosition() != -1;
}

//0x534700
bool Zombie::IsDeadOrDying()
{
    return 
        mDead || 
        mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || 
        mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || 
        mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED;
}

//0x534730
void Zombie::UpdateZombieChimney()
{
    if (mBoard->mBackground == BackgroundType::BACKGROUND_5_ROOF || mBoard->mBackground == BackgroundType::BACKGROUND_6_BOSS)
    {
        mPosY = 250;
        mAltitude = TodAnimateCurve(4000, 5000, mBoard->mCutScene->mCutsceneTime, 200, 0, TodCurves::CURVE_EASE_IN);
    }
}

//0x534780
void Zombie::WalkIntoHouse()
{
    AttachmentDetachCrossFadeParticleType(mAttachmentID, ParticleEffect::PARTICLE_ZAMBONI_SMOKE, nullptr);
    mFromWave = Zombie::ZOMBIE_WAVE_WINNER;
    ReanimReenableClipping();

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
    {
        mZombiePhase = ZombiePhase::PHASE_POLEVAULTER_POST_VAULT;
        StartWalkAnim(0);
    }

    if (mBoard->mBackground == BackgroundType::BACKGROUND_1_DAY || mBoard->mBackground == BackgroundType::BACKGROUND_2_NIGHT ||
        mBoard->mBackground == BackgroundType::BACKGROUND_3_POOL || mBoard->mBackground == BackgroundType::BACKGROUND_4_FOG)
    {
        //mPosY = 290.0f;
        mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, 2, 0);

        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
           // mPosY += 30.0f;
        }
        else if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
        {
            mPosX += 35.0f;
        }
        else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
        {
            //mPosY += 15.0f;
        }

        if (mBoard->StageHasPool())
        {
            if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
            {
                mPosX -= 10.0f;
            }
            else
            {
                mPosX -= 80.0f;
            }
        }
    }
    else if (mBoard->mBackground == BackgroundType::BACKGROUND_5_ROOF || mBoard->mBackground == BackgroundType::BACKGROUND_6_BOSS)
    {
       // mPosX = -180.0f;
       // mPosY = 250.0f;

        //mZombieHeight = ZombieHeight::HEIGHT_IN_TO_CHIMNEY;
        //mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, 0, 2);

        if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
           // mPosY += 5.0f;
        }
        else if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_BLACK_FOOTBALL)
        {
            //mPosX -= 14.0f;
        }
        else if (mZombieType == ZombieType::ZOMBIE_ZAMBONI)
        {
            //mPosX -= 28.0f;
        }

        //Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        //if (aBodyReanim && aBodyReanim->TrackExists("anim_idle") && mZombieType != ZombieType::ZOMBIE_POLEVAULTER)
        //{
        //   PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
        //}
    }
}

//0x534920
void Zombie::BossPlayIdle()
{
    mZombiePhase = ZombiePhase::PHASE_BOSS_IDLE;
    mPhaseCounter = RandRangeInt(100, 200);
    PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 6.0f);
}

//0x534960
void Zombie::DrawBossFireBall(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    MakeParentGraphicsFrame(g);

    Reanimation* aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim)
    {
        aFireBallReanim->DrawRenderGroup(g, RENDER_GROUP_NORMAL);

        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        aFireBallReanim->DrawRenderGroup(g, RENDER_GROUP_BOSS_FIREBALL_ADDITIVE);

        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        aFireBallReanim->DrawRenderGroup(g, RENDER_GROUP_BOSS_FIREBALL_TOP);
    }
}

//0x5349E0
void Zombie::DrawBossBackArm(Graphics* g, const ZombieDrawPosition& theDrawPos)
{
    float aImageOffsetX = 0.0f;
    float aImageOffsetY = 0.0f;
    if (mZombiePhase == PHASE_BOSS_DROP_RV)
    {
        aImageOffsetY = (mTargetRow - 1) * 85.0f - mTargetCol * 20.0f;
        aImageOffsetX = mTargetCol * 80.0f;
    }
    else if (mZombiePhase == PHASE_BOSS_BUNGEES_ENTER || mZombiePhase == PHASE_BOSS_BUNGEES_DROP || mZombiePhase == PHASE_BOSS_BUNGEES_LEAVE)
    {
        aImageOffsetX = mTargetCol * 80.0f - 23.0f;
    }

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    aBodyReanim->mOverlayMatrix.m02 += aImageOffsetX;
    aBodyReanim->mOverlayMatrix.m12 += aImageOffsetY;
    DrawReanim(g, theDrawPos, RENDER_GROUP_BOSS_BACK_ARM);
    aBodyReanim->mOverlayMatrix.m02 -= aImageOffsetX;
    aBodyReanim->mOverlayMatrix.m12 -= aImageOffsetY;
}

//0x534AC0
void Zombie::BossRVAttack()
{
    RemoveColdEffects();
    mZombiePhase = ZombiePhase::PHASE_BOSS_DROP_RV;
//#ifdef DO_FIX_BUGS
    mTargetRow = RandRangeInt(0, mBoard->StageHas6Rows() ? 4 : 3);  // 泳池僵王兼容
/*#else
    mTargetRow = RandRangeInt(0, 3);
#endif
    mTargetCol = RandRangeInt(0, 2);*/

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    aBodyReanim->SetImageOverride("Boss_RV", nullptr);

    PlayZombieReanim("anim_RV_1", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 16.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

//0x534B90
void Zombie::BossRVLanding()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow >= mTargetRow &&  aPlant->mRow <= mTargetRow + 1 && aPlant->mPlantCol >= mTargetCol && aPlant->mPlantCol <= mTargetCol + 2)
        {
            aPlant->Squish();
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    aBodyReanim->SetImageOverride("Boss_RV", IMAGE_REANIM_ZOMBIE_BOSS_RV2);

    mBoard->ShakeBoard(1, 2);
    mApp->PlaySample(SOUND_RVTHROW);

    mSummonCounter = 500;
    mBossHeadCounter = 5000;

    if (mBossMode >= 1)
    {
        mBossStompCounter = 4000;
    }
    if (mBossMode >= 2)
    {
        mBossBungeeCounter = 6500;
    }
}

//0x534C70
void Zombie::BossSpawnAttack()
{
    RemoveColdEffects();
    mZombiePhase = ZombiePhase::PHASE_BOSS_SPAWNING;
    if (mBossMode == 0)
    {
        mSummonCounter = RandRangeInt(450, 550);
    }
    else if (mBossMode == 1)
    {
        mSummonCounter = RandRangeInt(350, 450);
    }
    else if (mBossMode == 2)
    {
        mSummonCounter = RandRangeInt(150, 250);
    }

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
        mSummonCounter += 1000;

    mTargetRow = mBoard->PickRowForNewZombie(ZombieType::ZOMBIE_NORMAL);

    const char* aTrackName;
    switch (mTargetRow)
    {
    case 0:     aTrackName = "anim_spawn_1";    break;
    case 1:     aTrackName = "anim_spawn_2";    break;
    case 2:     aTrackName = "anim_spawn_3";    break;
    case 3:     aTrackName = "anim_spawn_4";    break;
    case 4:     aTrackName = "anim_spawn_5";    break;
//#ifdef DO_FIX_BUGS
    default:    aTrackName = "anim_spawn_5";    break;  // 泳池场景放僵尸崩溃的一种妥协的修复方式（不修改动画时）
/*#else
    default:    TOD_ASSERT();                   break;
#endif*/
    }
    PlayZombieReanim(aTrackName, ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

//0x534DC0
void Zombie::BossSpawnContact()
{
    ZombieType aZombieType;

    int zombieAge = mZombieAge;

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
        zombieAge -= 15000;

    if (mBoard->mIsVeteran)
        zombieAge += 3500;

    if (zombieAge < 3500)
    {
        aZombieType = ZombieType::ZOMBIE_NORMAL;
    }
    else if (zombieAge < 8000)
    {
        aZombieType = ZombieType::ZOMBIE_TRAFFIC_CONE;
    }
    else if (zombieAge < 12500)
    {
        aZombieType = ZombieType::ZOMBIE_PAIL;
    }
    else
    {
        int aZombieTypeCount = LENGTH(gBossZombieList);
        if (mTargetRow == 0)
        {
            TOD_ASSERT(gBossZombieList[aZombieTypeCount - 1] == ZombieType::ZOMBIE_GARGANTUAR);
            aZombieTypeCount--;
        }

        aZombieType = (ZombieType)TodPickFromArray((int*)gBossZombieList, aZombieTypeCount);
    }

    Zombie* aZombie = mBoard->AddZombieInRow(aZombieType, mTargetRow, 0);
    aZombie->mPosX = 600.0f;
}

//0x534E30
void Zombie::BossStompAttack()
{
    RemoveColdEffects();
    mZombiePhase = ZombiePhase::PHASE_BOSS_STOMPING;
    mBossStompCounter = RandRangeInt(5500, 6500);

    int aRowsCount = 0;
    int aRowArray[4];
    for (int i = 0; i < 4; i++)
    {
        if (BossCanStompRow(i))
        {
            aRowArray[aRowsCount] = i;
            aRowsCount++;
        }
    }

    if (aRowsCount == 0)
        return;

    mTargetRow = TodPickFromArray(aRowArray, aRowsCount);

    const char* aTrackName;
    switch (mTargetRow)
    {
    case 0:     aTrackName = "anim_stomp_1";    break;
    case 1:     aTrackName = "anim_stomp_2";    break;
    case 2:     aTrackName = "anim_stomp_3";    break;
    case 3:     aTrackName = "anim_stomp_4";    break;
    default:    TOD_ASSERT();                   break;
    }
    PlayZombieReanim(aTrackName, ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

//0x534F60
bool Zombie::BossCanStompRow(int theRow)
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mRow >= theRow && aPlant->mRow <= theRow + 1 && aPlant->mPlantCol >= 5)
        {
            return true;
        }
    }
    return false;
}

//0x534FF0
void Zombie::BossStompContact()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow >= mTargetRow  && aPlant->mRow <= mTargetRow + 1 && aPlant->mPlantCol >= 5)
        {
            aPlant->Squish();
        }
    }

    mBoard->ShakeBoard(1, 4);
    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
}

//0x5350C0
void Zombie::BossBungeeAttack()
{
    RemoveColdEffects();
    mZombiePhase = ZombiePhase::PHASE_BOSS_BUNGEES_ENTER;
    mBossBungeeCounter = RandRangeInt(4000, 5000);
    mTargetCol = RandRangeInt(0, 2);

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
    {
        mBossBungeeCounter += 2000;
    }

    PlayZombieReanim("anim_bungee_1_enter", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
    mApp->PlayFoley(FoleyType::FOLEY_BUNGEE_SCREAM);
}

//0x5351F0
void Zombie::BossBungeeSpawn()
{
    mZombiePhase = ZombiePhase::PHASE_BOSS_BUNGEES_DROP;

    for (int i = 0; i < NUM_BOSS_BUNGEES; i++)
    {
        Zombie* aZombie = mBoard->AddZombieInRow(ZombieType::ZOMBIE_BUNGEE, 0, 0);
        aZombie->PickBungeeZombieTarget(mTargetCol + i);
        aZombie->mAltitude = aZombie->mPosY - 30.0f;
        mFollowerZombieID[i] = mBoard->ZombieGetID(aZombie);
    }
}

//0x5352B0
void Zombie::BossBungeeLeave()
{
    mZombiePhase = ZombiePhase::PHASE_BOSS_BUNGEES_LEAVE;

    for (int i = 0; i < NUM_BOSS_BUNGEES; i++)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mFollowerZombieID[i]);
        if (aZombie && aZombie->mButteredCounter > 0)
        {
            aZombie->DieWithLoot();
        }
    }

    PlayZombieReanim("anim_bungee_1_leave", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
}

//0x535340
bool Zombie::BossAreBungeesDone()
{
    int aBungeesRemaining = 0;
    for (int i = 0; i < NUM_BOSS_BUNGEES; i++)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mFollowerZombieID[i]);
        if (aZombie)
        {
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_BUNGEE_RISING)
            {
                return true;
            }
            aBungeesRemaining++;
        }
    }

    return aBungeesRemaining == 0;
}

//0x5353A0
void Zombie::BossHeadAttack()
{
    mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_ENTER;
    mBossHeadCounter = RandRangeInt(4000, 5000);

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
    {
        mBossHeadCounter += 2000;
    }

    PlayZombieReanim("anim_head_enter", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

//0x535440
void Zombie::BossHeadSpit()
{
    Reanimation* aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim)
    {
        aFireBallReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_SPIT;
//#ifdef DO_FIX_BUGS
    mFireballRow = RandRangeInt(0, mBoard->StageHas6Rows() ? 5 : 4);  // 泳池僵王兼容
/*#else
    mFireballRow = RandRangeInt(0, 4);
#endif*/
    mIsFireBall = RandRangeInt(0, 1) == 0;
    
    const char* aTrackName;
    switch (mFireballRow)
    {
    case 0:     aTrackName = "anim_head_attack_1";      break;
    case 1:     aTrackName = "anim_head_attack_2";      break;
    case 2:     aTrackName = "anim_head_attack_3";      break;
    case 3:     aTrackName = "anim_head_attack_4";      break;
    case 4:     aTrackName = "anim_head_attack_5";      break;
//#ifdef DO_FIX_BUGS
    default:    aTrackName = "anim_head_attack_5";      break;  // 泳池场景吐球的一种妥协的修复方式（不修改动画时）
/*#else
    default:    TOD_ASSERT();                           break;
#endif*/
    }
    PlayZombieReanim(aTrackName, ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mIsFireBall)
    {
        aBodyReanim->SetImageOverride("Boss_eyeglow_red", nullptr);
        aBodyReanim->SetImageOverride("Boss_mouthglow_red", nullptr);
    }
    else
    {
        aBodyReanim->SetImageOverride("Boss_eyeglow_red", IMAGE_REANIM_ZOMBIE_BOSS_EYEGLOW_BLUE);
        aBodyReanim->SetImageOverride("Boss_mouthglow_red", IMAGE_REANIM_ZOMBIE_BOSS_MOUTHGLOW_BLUE);
    }

    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mSpecialHeadReanimID);
    aHeadReanim->PlayReanim("anim_drive", ReanimLoopType::REANIM_LOOP, 20, 36.0f);
}

//0x535630
void Zombie::BossDestroyIceballInRow(int theRow)
{
    //if (theRow != mFireballRow)  // 此判断在原版中被移动至调用处进行，故参数的 theRow 亦被删去
    //    return;

    Reanimation* aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim && !mIsFireBall)
    {
        float aPosX = aFireBallReanim->mOverlayMatrix.m02 + 80.0f;
        float aPosY = aFireBallReanim->mOverlayMatrix.m12 + 80.0f;
        mApp->AddTodParticle(aPosX, aPosY, 400000, ParticleEffect::PARTICLE_ICEBALL_DEATH);

        aFireBallReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
        mBoard->RemoveParticleByType(ParticleEffect::PARTICLE_ICEBALL_TRAIL);
    }
}

//0x5356D0
void Zombie::BossDestroyFireball()
{
    Reanimation* aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim && mIsFireBall)
    {
        float aPosX = aFireBallReanim->mOverlayMatrix.m02 + 80.0f;
        float aPosY = aFireBallReanim->mOverlayMatrix.m12 + 40.0f;
        for (int i = 0; i < 6; i++)
        {
            float aAngle = 2 * PI * i / 6 + PI / 2;
            Reanimation* aReanim = mApp->AddReanimation(aPosX + 60.0f * sin(aAngle), aPosY + 60.0f * cos(aAngle), 400000, ReanimationType::REANIM_JALAPENO_FIRE);
            aReanim->mAnimTime = 0.2f;
            aReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME;
            aReanim->mAnimRate = RandRangeFloat(20.0f, 25.0f);
        }

        aFireBallReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
        mBoard->RemoveParticleByType(ParticleEffect::PARTICLE_FIREBALL_TRAIL);
    }
}

//0x535870
void Zombie::BossHeadSpitEffect()
{
    if (mIsFireBall)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("Boss_jaw");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
        float aFlamePosX = mPosX + aTransform.mTransX + 100.0f ;
        float aFlamePosY = mPosY + aTransform.mTransY + 50.0f ;
        mApp->AddTodParticle(aFlamePosX, aFlamePosY, mRenderOrder + 2, ParticleEffect::PARTICLE_ZOMBIE_BOSS_FIREBALL);
    }
    else
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndex("Boss_jaw");
        ReanimatorTransform aTransform;
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
        float aFlamePosX = mPosX + aTransform.mTransX + 100.0f ;
        float aFlamePosY = mPosY + aTransform.mTransY + 50.0f ;
        TodParticleSystem* aParticle = mApp->AddTodParticle(aFlamePosX, aFlamePosY, mRenderOrder + 2, ParticleEffect::PARTICLE_ZOMBIE_BOSS_FIREBALL);
        if (aParticle)
        {
            aParticle->OverrideImage(nullptr, IMAGE_ZOMBIE_BOSS_ICEBALL_PARTICLES);
        }
    }

    mApp->PlayFoley(FoleyType::FOLEY_BOSS_BOULDER_ATTACK);
}

//0x535A70
void Zombie::BossHeadSpitContact()
{
    TOD_ASSERT(!mApp->ReanimationTryToGet(mBossFireBallReanimID));

    float aPosY = mBoard->GetPosYBasedOnRow(550.0f , mFireballRow) - 90.0f;
    Reanimation* aFireBallReanim;
    if (mIsFireBall)
    {
        aFireBallReanim = mApp->AddReanimation(455.0f  , aPosY, mRenderOrder + 1, ReanimationType::REANIM_BOSS_FIREBALL);
        aFireBallReanim->PlayReanim("anim_form", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 16.0f);
        aFireBallReanim->mIsAttachment = true;
        aFireBallReanim->AssignRenderGroupToTrack("additive", RENDER_GROUP_BOSS_FIREBALL_ADDITIVE);
        aFireBallReanim->AssignRenderGroupToTrack("superglow", RENDER_GROUP_BOSS_FIREBALL_ADDITIVE);
    }
    else
    {
        aFireBallReanim = mApp->AddReanimation(455.0f , aPosY , mRenderOrder + 1, ReanimationType::REANIM_BOSS_ICEBALL);
        aFireBallReanim->PlayReanim("anim_form", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 16.0f);
        aFireBallReanim->mIsAttachment = true;
        aFireBallReanim->AssignRenderGroupToTrack("ice_highlight", RENDER_GROUP_BOSS_FIREBALL_ADDITIVE);
    }

    mBossFireBallReanimID = mApp->ReanimationGetID(aFireBallReanim);
    mApp->ReanimationTryToGet(mSpecialHeadReanimID)->PlayReanim("anim_laugh", ReanimLoopType::REANIM_LOOP, 20, 18.0f);
    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

//0x535C60
void Zombie::UpdateBossFireball()
{
    Reanimation* aFireballReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireballReanim == nullptr)
        return;

    float aSpeed = aFireballReanim->GetTrackVelocity("_ground");
    aFireballReanim->mOverlayMatrix.m02 -= aSpeed;
    float aPosX = aFireballReanim->mOverlayMatrix.m02;
    float aPosY = mBoard->GetPosYBasedOnRow(aPosX + 75.0f, mFireballRow) - 90.0f;
    aFireballReanim->mOverlayMatrix.m12 = aPosY;

    if (aPosX < -180.0f)
    {
        aFireballReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    SquishAllInSquare(mBoard->PixelToGridX(aPosX + 75, aPosY), mFireballRow, ZombieAttackType::ATTACKTYPE_DRIVE_OVER);

    LawnMower* aLawnMower = nullptr;
    while (mBoard->IterateLawnMowers(aLawnMower))
    {
        if (aLawnMower->mMowerState != LawnMowerState::MOWER_SQUISHED && aLawnMower->mRow == mFireballRow && 
            aLawnMower->mPosX > aPosX && aLawnMower->mPosX < aPosX + 50.0f)
        {
            aLawnMower->SquishMower();
        }
    }

    if (mIsFireBall)
    {
        if (aFireballReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && aFireballReanim->mLoopCount > 0)
        {
            aFireballReanim->PlayReanim("anim_role", ReanimLoopType::REANIM_LOOP, 0, 2.0f);
            aFireballReanim->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mFireballRow, 0);
        }

        if (aFireballReanim->mLoopType == ReanimLoopType::REANIM_LOOP && Rand(10) == 0)
        {
            float aBallPosX = aPosX + 100.0f + RandRangeFloat(0.0f, 20.0f);
            float aBallPosY = mBoard->GetPosYBasedOnRow(aBallPosX - 40.0f, mFireballRow) + 90.0f + RandRangeFloat(-50.0f, 0.0f);
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mFireballRow, 6);
            mApp->AddTodParticle(aBallPosX, aBallPosY, aRenderPosition, ParticleEffect::PARTICLE_FIREBALL_TRAIL);
        }
    }
    else
    {
        if (aFireballReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && aFireballReanim->mLoopCount > 0)
        {
            aFireballReanim->PlayReanim("anim_role", ReanimLoopType::REANIM_LOOP, 0, 2.0f);
            aFireballReanim->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mFireballRow, 0);
        }

        if (aFireballReanim->mLoopType == ReanimLoopType::REANIM_LOOP && Rand(10) == 0)
        {
            float aBallPosX = aPosX + 100.0f + RandRangeFloat(0.0f, 20.0f) ;
            float aBallPosY = mBoard->GetPosYBasedOnRow(aBallPosX - 40.0f, mFireballRow) + 90.0f + RandRangeFloat(-50.0f, 0.0f);
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mFireballRow, 6);
            mApp->AddTodParticle(aBallPosX, aBallPosY, aRenderPosition, ParticleEffect::PARTICLE_ICEBALL_TRAIL);
        }
    }

    aFireballReanim->Update();
}

//0x535FB0
void Zombie::BossStartDeath()
{
    mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_LEAVE;
    PlayZombieReanim("anim_head_leave", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);

    mApp->AddTodParticle(700.0f, 150.0f, 400000, ParticleEffect::PARTICLE_BOSS_EXPLOSION);
    mApp->PlaySample(SOUND_BOSSEXPLOSION);
    mApp->PlayFoley(FoleyType::FOLEY_GARGANTUDEATH);

    BossDie();
}

//0x536080
void Zombie::UpdateBoss()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
    {
        if (mApp->mGameMode != GAMEMODE_CHALLENGE_AIR_RAID)
        {
            if (aBodyReanim->ShouldTriggerTimedEvent(0.24f) || aBodyReanim->ShouldTriggerTimedEvent(0.79f))
            {
                mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                mBoard->ShakeBoard(1, 4);
            }
        }
        return;
    }

    Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
    UpdateBossFireball();
    if (mIceTrapCounter == 0)
    {
        if (mSummonCounter > 0)
        {
            mSummonCounter--;
        }
        if (mBossBungeeCounter > 0)
        {
            mBossBungeeCounter--;
        }
        if (mBossStompCounter > 0)
        {
            mBossStompCounter--;
        }
        if (mBossHeadCounter > 0)
        {
            mBossHeadCounter--;
        }

        if (mChilledCounter > 0)
        {
            aHeadReanim->mAnimRate = 6.0f;
        }
        else 
        {
            if (aHeadReanim && aHeadReanim->mAnimRate == 0.0f)
                aHeadReanim->mAnimRate = 12.0f;
        }
    }
    else
    {
        aHeadReanim->mAnimRate = 0.0f;
    }

    if (mZombiePhase == ZombiePhase::PHASE_BOSS_ENTER)
    {
        BossPlayIdle();
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_IDLE)
    {
        if (mBodyHealth == 1)
        {
            PlayDeathAnim(0U);
            return;
        }

        if (mPhaseCounter > 0)
            return;

        int aDamageIndex = GetBodyDamageIndex();
        if (aDamageIndex != mBossMode)
        {
            mBossMode = aDamageIndex;
            if (mBossMode == 1)  // 进入一级损伤后，立即释放一次蹦极僵尸
            {
                BossBungeeAttack();
            }
            else  // 进入二级损伤后，立即进行一次砸车
            {
                BossRVAttack();
            }
        }
        else if (mBossStompCounter == 0)
        {
            BossStompAttack();
        }
        else if (mBossBungeeCounter == 0)
        {
            if (Rand(mApp->IsAdventureMode() ? 4 : 2) == 0)  // 1/2 概率砸车（冒险模式为 1/4 概率），否则释放蹦极僵尸
            {
                mBossBungeeCounter = RandRangeInt(4000, 5000);
                BossRVAttack();
            }
            else
            {
                BossBungeeAttack();
            }
        }
        else if (mBossHeadCounter == 0)
        {
            BossHeadAttack();
        }
        else if (mSummonCounter == 0)
        {
            BossSpawnAttack();
        }
        else
        {
            mPhaseCounter = RandRangeInt(100, 200);
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_SPAWNING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.6f))
        {
            BossSpawnContact();
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            BossPlayIdle();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_STOMPING)
    {
        float aTrigger = 0.5f;
        if (mTargetRow >= 2)
        {
            aTrigger = 0.55f;
        }
        if (aBodyReanim->ShouldTriggerTimedEvent(aTrigger))
        {
            BossStompContact();
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            BossPlayIdle();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_BUNGEES_ENTER)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            BossBungeeSpawn();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_BUNGEES_DROP)
    {
        if (BossAreBungeesDone())
        {
            BossBungeeLeave();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_BUNGEES_LEAVE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            BossPlayIdle();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_DROP_RV)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.65f))
        {
            BossRVLanding();
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            BossPlayIdle();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_ENTER)
    {
        if (GetBodyDamageIndex() == 2 && aBodyReanim->ShouldTriggerTimedEvent(0.37f))
        {
            ApplyBossSmokeParticles(true);
        }

        if (aBodyReanim->ShouldTriggerTimedEvent(0.55f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT;
            PlayZombieReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
            mPhaseCounter = 500;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT)
    {
        if (mBodyHealth == 1)
        {
            BossStartDeath();
        }
        else if (mPhaseCounter == 0)
        {
            BossHeadSpit();
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_SPIT)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.37f))
        {
            BossHeadSpitEffect();
        }

        if (aBodyReanim->ShouldTriggerTimedEvent(0.42f))
        {
            BossHeadSpitContact();
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            aHeadReanim = mApp->ReanimationTryToGet(mSpecialHeadReanimID);
            aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, 18.0f);
            mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT;
            PlayZombieReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 100, 12.0f);
            mPhaseCounter = 300;
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT)
    {
        if (mBodyHealth == 1)
        {
            BossStartDeath();
        }
        else if (mPhaseCounter == 0)
        {
            if (mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID){
                //To be implemented
            }
            else {
                mZombiePhase = ZombiePhase::PHASE_BOSS_HEAD_LEAVE;
                PlayZombieReanim("anim_head_leave", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12.0f);
            }
            
        }
    }
    else if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_LEAVE)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.23f))
        {
            mChilledCounter = 0;
            UpdateAnimSpeed();
        }

        if (aBodyReanim->ShouldTriggerTimedEvent(0.48f) || aBodyReanim->ShouldTriggerTimedEvent(0.8f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            ApplyBossSmokeParticles(false);
            BossPlayIdle();
        }
    }
    else
    {
        TOD_ASSERT();
    }
}

//0x5366D0
void Zombie::BossDie()
{
    if (!IsOnBoard())
        return;

    Reanimation* aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim)
    {
        aFireBallReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;

        BossDestroyIceballInRow(mTargetRow);
        BossDestroyFireball();
    }

    mApp->mMusic->FadeOut(200);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie != this && !aZombie->IsDeadOrDying())
        {
            //aZombie->DieWithLoot();
            int totalHP = aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth;
            aZombie->TakeDamage(totalHP, 0);      
        }
    }

    RemoveColdEffects();
}

//0x5367F0
void Zombie::BossSetupReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    aBodyReanim->AssignRenderGroupToPrefix("Boss_innerleg", RENDER_GROUP_BOSS_BACK_LEG);
    aBodyReanim->AssignRenderGroupToPrefix("Boss_outerleg", RENDER_GROUP_BOSS_FRONT_LEG);
    aBodyReanim->AssignRenderGroupToPrefix("Boss_body2", RENDER_GROUP_BOSS_FRONT_LEG);
    aBodyReanim->AssignRenderGroupToPrefix("Boss_innerarm", RENDER_GROUP_BOSS_BACK_ARM);
    aBodyReanim->AssignRenderGroupToPrefix("Boss_RV", RENDER_GROUP_BOSS_BACK_ARM);

    Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_BOSS_DRIVER);
    aHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 18.0f);
    mSpecialHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Boss_head2");
    AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aHeadReanim, 28.0f, -84.0f);
    aBodyReanim->mFrameBasePose = 0;
    aAttachEffect->mOffset.m00 = 1.2f;
    aAttachEffect->mOffset.m11 = 1.2f;
    aAttachEffect->mDontDrawIfParentHidden = true;
}

//0x536940
void Zombie::DrawBossPart(Graphics* g, BossPart theBossPart)
{
    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);

    switch (theBossPart)
    {
    case BossPart::BOSS_PART_BACK_LEG:      DrawReanim(g, aDrawPos, RENDER_GROUP_BOSS_BACK_LEG);    break;
    case BossPart::BOSS_PART_FRONT_LEG:     DrawReanim(g, aDrawPos, RENDER_GROUP_BOSS_FRONT_LEG);   break;
    case BossPart::BOSS_PART_MAIN:          DrawReanim(g, aDrawPos, 0);                             break;
    case BossPart::BOSS_PART_BACK_ARM:      DrawBossBackArm(g, aDrawPos);                           break;
    case BossPart::BOSS_PART_FIREBALL:      DrawBossFireBall(g, aDrawPos);                          break;
    default:                                                                                        break;
    }
}

//0x5369E0
void Zombie::PreloadZombieResources(ZombieType theZombieType)
{
    const ZombieDefinition& aZombieDef = GetZombieDefinition(theZombieType);
    if (aZombieDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        ReanimatorEnsureDefinitionLoaded(aZombieDef.mReanimationType, true);
    }

    if (theZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_DIGGER_DIRT, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED_DIGGER, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_BOSS)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_BOSS_DRIVER, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_BOSS_FIREBALL, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_BOSS_ICEBALL, true);

        for (int i = 0; i < LENGTH(gBossZombieList); i++)
        {
            const ZombieDefinition& aDef = GetZombieDefinition(gBossZombieList[i]);
            ReanimatorEnsureDefinitionLoaded(aDef.mReanimationType, true);
        }
    }
    else if (theZombieType == ZombieType::ZOMBIE_DANCER)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_BACKUP_DANCER, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_DISCO)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_DISCO_BACKUP, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_GARGANTUAR || theZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_IMP, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED_IMP, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED_GARGANTUAR, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_ZAMBONI)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_IMP, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED_ZAMBONI, true);
    }
    else if (theZombieType == ZombieType::ZOMBIE_CATAPULT)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED_CATAPULT, true);
    }

    ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_PUFF, true);
    ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED, true);
    ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_LAWN_MOWERED_ZOMBIE, true);
}

//0x536B00
void Zombie::ApplyBossSmokeParticles(bool theEnable)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("Boss_head");
    AttachmentDetachCrossFadeParticleType(aTrackInstance->mAttachmentID, ParticleEffect::PARTICLE_ZAMBONI_SMOKE, nullptr);

    if (theEnable)
    {
        TodParticleSystem* aParticle1 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
        TodParticleSystem* aParticle2 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
        if (aParticle1)
        {
            AttachEffect* aAttachEffect = aBodyReanim->AttachParticleToTrack("Boss_head", aParticle1, 120.0f, 30.0f);
            aAttachEffect->mDontDrawIfParentHidden = true;
            aAttachEffect->mDontPropogateColor = true;
        }
        if (aParticle2)
        {
            AttachEffect* aAttachEffect = aBodyReanim->AttachParticleToTrack("Boss_head", aParticle2, 205.0f, 58.0f);
            aAttachEffect->mDontDrawIfParentHidden = true;
            aAttachEffect->mDontPropogateColor = true;
        }

        if (mBodyHealth < mBodyMaxHealth / BOSS_FLASH_HEALTH_FRACTION)
        {
            TodParticleSystem* aParticle3 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZAMBONI_SMOKE);
            if (aParticle3)
            {
                AttachEffect* aAttachEffect = aBodyReanim->AttachParticleToTrack("Boss_head", aParticle3, 193.0f, 27.0f);
                aAttachEffect->mDontDrawIfParentHidden = true;
                aAttachEffect->mDontPropogateColor = true;
            }
        }
    }
}

//0x536C90
void Zombie::EnableMustache(bool theEnableMustache)
{
    if (mFromWave == Zombie::ZOMBIE_WAVE_UI)
        return;

    if (!mHasHead || Zombie::IsZombotany(mZombieType))
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr || !aBodyReanim->TrackExists("Zombie_mustache"))
        return;

    if (theEnableMustache)
    {
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_mustache", RENDER_GROUP_NORMAL);

        switch (RandRangeInt(1, 3))
        {
        case 1:     aBodyReanim->SetImageOverride("Zombie_mustache", nullptr);                          break;
        case 2:     aBodyReanim->SetImageOverride("Zombie_mustache", IMAGE_REANIM_ZOMBIE_MUSTACHE2);    break;
        case 3:     aBodyReanim->SetImageOverride("Zombie_mustache", IMAGE_REANIM_ZOMBIE_MUSTACHE3);    break;
        }
    }
    else
    {
        aBodyReanim->AssignRenderGroupToPrefix("Zombie_mustache", RENDER_GROUP_HIDDEN);
    }
}

//0x536D90
void Zombie::EnableFuture(bool theEnableFuture)
{
    if (mFromWave == Zombie::ZOMBIE_WAVE_UI || Zombie::IsZombotany(mZombieType))
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr || aBodyReanim->mReanimationType != ReanimationType::REANIM_ZOMBIE)
        return;

    if (theEnableFuture)
    {
        Image* aImage;
        switch ((unsigned int)mBoard->ZombieGetID(this) % 4)
        {
        case 0:     aImage = IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1;      break;
        case 1:     aImage = IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2;      break;
        case 2:     aImage = IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3;      break;
        case 3:     aImage = IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4;      break;
        default:    TOD_ASSERT();                                       break;
        }
        aBodyReanim->SetImageOverride("anim_head1", aImage);
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_head1", nullptr);
    }
}

void Zombie::EnableDance(bool theEnableDance)
{
    if (!IsOnBoard() && !theEnableDance)
        return;

    if (ZombieNotWalking() || IsDeadOrDying())
        return;

    if (mZombieType == ZombieType::ZOMBIE_NORMAL || mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE || mZombieType == ZombieType::ZOMBIE_PAIL)
    {
        StartWalkAnim(theEnableDance ? 20.0f : 0);
    }
}

//0x536EA0
void Zombie::SetupWaterTrack(const char* theTrackName)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName(theTrackName);
    aTrackInstance->mIgnoreExtraAdditiveColor = true;
    aTrackInstance->mIgnoreColorOverride = true;
    aTrackInstance->mIgnoreClipRect = true;
}