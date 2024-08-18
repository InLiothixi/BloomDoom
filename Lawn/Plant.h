#pragma once

#include <string>
#include "GameObject.h"

#define MAX_MAGNET_ITEMS 5

enum PlantSubClass
{
    SUBCLASS_NORMAL = 0,
    SUBCLASS_SHOOTER = 1
};

enum PlantWeapon
{
    WEAPON_PRIMARY,
    WEAPON_SECONDARY
};

enum PlantOnBungeeState
{
    NOT_ON_BUNGEE,
    GETTING_GRABBED_BY_BUNGEE,
    RISING_WITH_BUNGEE
};

enum PlantState
{
    STATE_NOTREADY,
    STATE_READY,
    STATE_DOINGSPECIAL,
    STATE_SQUASH_LOOK,
    STATE_SQUASH_PRE_LAUNCH,
    STATE_SQUASH_RISING,
    STATE_SQUASH_FALLING,
    STATE_SQUASH_DONE_FALLING,
    STATE_GRAVEBUSTER_LANDING,
    STATE_GRAVEBUSTER_EATING,
    STATE_CHOMPER_BITING,
    STATE_CHOMPER_BITING_GOT_ONE,
    STATE_CHOMPER_BITING_MISSED,
    STATE_CHOMPER_DIGESTING,
    STATE_CHOMPER_SWALLOWING,
    STATE_POTATO_RISING,
    STATE_POTATO_ARMED,
    STATE_POTATO_MASHED,
    STATE_SPIKEWEED_ATTACKING,
    STATE_SPIKEWEED_ATTACKING_2,
    STATE_SCAREDYSHROOM_LOWERING,
    STATE_SCAREDYSHROOM_SCARED,
    STATE_SCAREDYSHROOM_RAISING,
    STATE_SUNSHROOM_SMALL,
    STATE_SUNSHROOM_GROWING,
    STATE_SUNSHROOM_BIG,
    STATE_MAGNETSHROOM_SUCKING,
    STATE_MAGNETSHROOM_CHARGING,
    STATE_BOWLING_UP,
    STATE_BOWLING_DOWN,
    STATE_CACTUS_LOW,
    STATE_CACTUS_RISING,
    STATE_CACTUS_HIGH,
    STATE_CACTUS_LOWERING,
    STATE_TANGLEKELP_GRABBING,
    STATE_COBCANNON_ARMING,
    STATE_COBCANNON_LOADING,
    STATE_COBCANNON_READY,
    STATE_COBCANNON_FIRING,
    STATE_KERNELPULT_BUTTER,
    STATE_UMBRELLA_TRIGGERED,
    STATE_UMBRELLA_REFLECTING,
    STATE_IMITATER_MORPHING,
    STATE_ZEN_GARDEN_WATERED,
    STATE_ZEN_GARDEN_NEEDY,
    STATE_ZEN_GARDEN_HAPPY,
    STATE_MARIGOLD_ENDING,
    STATE_FLOWERPOT_INVULNERABLE,
    STATE_LILYPAD_INVULNERABLE,
    STATE_DEHYDRATED,
    STATE_EXTREMELY_DEHYDRATED,
    STATE_PUNCHING,
    STATE_UPPERCUTING,
    STATE_PUNCHING_BACKWARDS,
    STATE_UPPERCUTING_BACKWARDS,
    STATE_READY_BACKWARDS,
    STATE_BURNED,
    STATE_SINGING,
    STATE_SEEDROOT_DIVING,
    STATE_SEEDROOT_DIGGING,
    STATE_MAGNIFYING_GRASS_NO_LIGHT,
    STATE_MAGNIFYING_GRASS_ATTACKING,
    STATE_PUFFSHROOM_FADING
};

enum PLANT_LAYER
{
    PLANT_LAYER_BELOW = -1,
    PLANT_LAYER_MAIN,
    PLANT_LAYER_REANIM,
    PLANT_LAYER_REANIM_HEAD,
    PLANT_LAYER_REANIM_BLINK,
    PLANT_LAYER_ON_TOP,
    NUM_PLANT_LAYERS
};

enum PLANT_ORDER
{
    PLANT_ORDER_UNDERLILYPAD,
    PLANT_ORDER_LILYPAD,
    PLANT_ORDER_NORMAL,
    PLANT_ORDER_PUMPKIN,
    PLANT_ORDER_FLYER,
    PLANT_ORDER_CHERRYBOMB
};

enum MagnetItemType
{
    MAGNET_ITEM_NONE,
    MAGNET_ITEM_PAIL_1,
    MAGNET_ITEM_PAIL_2,
    MAGNET_ITEM_PAIL_3,
    MAGNET_ITEM_FOOTBALL_HELMET_1,
    MAGNET_ITEM_FOOTBALL_HELMET_2,
    MAGNET_ITEM_FOOTBALL_HELMET_3,
    MAGNET_ITEM_BLACK_FOOTBALL_HELMET_1,
    MAGNET_ITEM_BLACK_FOOTBALL_HELMET_2,
    MAGNET_ITEM_BLACK_FOOTBALL_HELMET_3,
    MAGNET_ITEM_DIVER_1,
    MAGNET_ITEM_DIVER_2,
    MAGNET_ITEM_DIVER_3,
    MAGNET_ITEM_DOOR_1,
    MAGNET_ITEM_DOOR_2,
    MAGNET_ITEM_DOOR_3,
    //MAGNET_ITEM_PROPELLER,
    MAGNET_ITEM_POGO_1,
    MAGNET_ITEM_POGO_2,
    MAGNET_ITEM_POGO_3,
    MAGNET_ITEM_JACK_IN_THE_BOX,
    MAGNET_ITEM_LADDER_1,
    MAGNET_ITEM_LADDER_2,
    MAGNET_ITEM_LADDER_3,
    MAGNET_ITEM_LADDER_PLACED,
    MAGNET_ITEM_SILVER_COIN,
    MAGNET_ITEM_GOLD_COIN,
    MAGNET_ITEM_DIAMOND,
    MAGNET_ITEM_PICK_AXE
};

class MagnetItem
{
public:
    float                   mPosX;                          //+0x0
    float                   mPosY;                          //+0x4
    float                   mDestOffsetX;                   //+0x8
    float                   mDestOffsetY;                   //+0xC
    MagnetItemType          mItemType;                      //+0x10
};

class Coin;
class Zombie;
class Reanimation;
class TodParticleSystem;

class Plant : public GameObject
{
public:
    SeedType                mSeedType;                      //+0x24
    int                     mPlantCol;                      //+0x28
    bool                    mFacingLeft;                    //+0x29
    int                     mAnimCounter;                   //+0x2C
    int                     mFrame;                         //+0x30
    int                     mFrameLength;                   //+0x34
    int                     mNumFrames;                     //+0x38
    PlantState              mState;                         //+0x3C
    int                     mPlantHealth;                   //+0x40
    int                     mPlantMaxHealth;                //+0x44
    int                     mSubclass;                      //+0x48
    int                     mDisappearCountdown;            //+0x4C
    int                     mDoSpecialCountdown;            //+0x50
    int                     mStateCountdown;                //+0x54
    int                     mLaunchCounter;                 //+0x58
    int                     mLaunchRate;                    //+0x5C
    Rect                    mPlantRect;                     //+0x60
    Rect                    mPlantAttackRect;               //+0x70
    int                     mTargetX;                       //+0x80
    int                     mTargetY;                       //+0x84
    int                     mStartRow;                      //+0x88
    ParticleSystemID        mParticleID;                    //+0x8C
    int                     mShootingCounter;               //+0x90
    ReanimationID           mBodyReanimID;                  //+0x94
    ReanimationID           mBackbodyReanimID;
    ReanimationID           mHeadReanimID;                  //+0x98
    ReanimationID           mHeadReanimID2;                 //+0x9C
    ReanimationID           mHeadReanimID3;                 //+0xA0
    ReanimationID           mBlinkReanimID;                 //+0xA4
    ReanimationID           mLightReanimID;                 //+0xA8
    ReanimationID           mSleepingReanimID;              //+0xAC
    int                     mBlinkCountdown;                //+0xB0
    int                     mRecentlyEatenCountdown;        //+0xB4
    int                     mEatenFlashCountdown;           //+0xB8
    int                     mBeghouledFlashCountdown;       //+0xBC
    float                   mShakeOffsetX;                  //+0xC0
    float                   mShakeOffsetY;                  //+0xC4
    MagnetItem              mMagnetItems[MAX_MAGNET_ITEMS]; //+0xC8
    ZombieID                mTargetZombieID;                //+0x12C
    int                     mWakeUpCounter;                 //+0x130
    PlantOnBungeeState      mOnBungeeState;                 //+0x134
    SeedType                mImitaterType;                  //+0x138
    int                     mPottedPlantIndex;              //+0x13C
    bool                    mAnimPing;                      //+0x140
    bool                    mDead;                          //+0x141
    bool                    mSquished;                      //+0x142
    bool                    mIsAsleep;                      //+0x143
    bool                    mIsOnBoard;                     //+0x144
    bool                    mHighlighted;                   //+0x145
    int                     mHeatWaveTimer;
    int                     mLastPortalX;
    bool                    mCoffeebeaned;
    float                   mBowlingSlope;
    int                     mAmplifiedCounter;

    bool                    mIsPlayer = false;
    bool                    mIsPreview = false;

    // Plant vs Plant
    bool                    mEnemy = false;

    // Heat Wave
    int                     mBerserkCounter;

    //Qol
    int                     mPlantAge;

    bool                    mDarken;

    SeedType                mRootSeedType;
    SeedType                mRootImitaterType;
    int                     mRootMaxTime;
    int                     mRootTime;

    int                     mExplodePulse;

    //PVP
    bool                    mMindControlled;
    int                     mChilledCounter;                            //+0xAC
    int                     mButteredCounter;                           //+0xB0
    int                     mIceTrapCounter;                            //+0xB4
    int                     mBurnCounter;
    bool                    mIgnoreBlink;
    bool                    mIsDummy;
    int                     mSingingCounter;
    PlantID                 mPlantAmplifierID;
    

public:
    Plant();

    void                    PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType);
    void                    Update();
    void                    Animate();
    void                    Draw(Graphics* g);
    void                    MouseDown(int x, int y, int theClickCount);
    void                    DoSpecial();
    void                    Fire(void* theTarget, int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Zombie*                 FindTargetZombie(int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Plant*                  FindTargetPlant(int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    void                    Die();
    void                    UpdateProductionPlant();
    void                    UpdateShooter();
    bool                    FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    void                    LaunchThreepeater();
    void                    LaunchSlingpea();
    static Image*           GetImage(SeedType theSeedType);
    static int              GetCost(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    static SexyString       GetNameString(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    static SexyString       GetToolTip(SeedType theSeedType);
    static int              GetRefreshTime(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    static /*inline*/ bool  IsNocturnal(SeedType theSeedtype);
    static /*inline*/ bool  IsAquatic(SeedType theSeedType);
    static /*inline*/ bool  IsFlying(SeedType theSeedtype);
    static /*inline*/ bool  IsUpgrade(SeedType theSeedtype);
    void                    UpdateAbilities();
    void                    Squish();
    void                    DoRowAreaDamage(int theDamage, unsigned int theDamageFlags);
    int                     GetDamageRangeFlags(PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Rect                    GetPlantRect();
    Rect                    GetPlantAttackRect(PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Zombie*                 FindSquashTarget();
    void                    UpdateSquash();
    void                    UpdateIcebergLettuce();
    /*inline*/ bool         NotOnGround();
    void                    DoSquashDamage();
    void                    BurnRow(int theRow);
    void                    IceZombies();
    void                    BlowAwayFliers(int theX, int theRow);
    void                    UpdateGraveBuster();
    TodParticleSystem*      AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect);
    void                    GetPeaHeadOffset(int& theOffsetX, int& theOffsetY);
    /*inline*/ bool         MakesSun();
    static bool             MakesSun(SeedType theSeedType);
    static void             DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY);
    void                    KillAllPlantsNearDoom();
    bool                    IsOnHighGround();
    void                    UpdateCherryHoverBomb();
    void                    UpdateArticNut();
    void                    UpdateTorchwood();
    void                    UpdatePlasmawood();
    void                    LaunchStarFruit();
    bool                    FindStarFruitTarget();
    void                    UpdateBonkChoy();
    void                    UpdateFireShroom();
    void                    UpdateChomper();
    void                    DoBlink();
    void                    UpdateBlink();
    void                    PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void                    UpdateMagnetShroom();
    MagnetItem*             GetFreeMagnetItem();
    void                    DrawMagnetItems(Graphics* g);
    void                    UpdateDoomShroom();
    void                    UpdateSeedRoot();
    void                    UpdateExplodeONut();
    void                    UpdateIceShroom();
    void                    UpdateEinsteinApple();
    void                    UpdatePotato();
    void                    UpdateMagnifyingGrass();
    int                     CalcRenderOrder();
    void                    AnimateNuts();
    void                    AnimatePuffshroom();
    void                    AnimateFireshroom();
    void                    SetSleeping(bool theIsAsleep);
    void                    UpdateShooting();
    void                    DrawShadow(Graphics* g, float theOffsetX, float theOffsetY);
    void                    UpdateScaredyShroom();
    int                     DistanceToClosestZombie();
    void                    UpdateSpikeweed();
    void                    MagnetShroomAttactItem(Zombie* theZombie);
    void                    UpdateSunShroom();
    void                    UpdateBowling();
    void                    AnimatePumpkin();
    void                    UpdateHurrikale();
    void                    UpdateBlover();
    void                    UpdateCactus();
    void                    StarFruitFire();
    void                    UpdateTanglekelp();
    Reanimation*            AttachBlinkAnim(Reanimation* theReanimBody);
    void                    UpdateReanimColor();
    bool                    IsUpgradableTo(SeedType theUpgradedType);
    bool                    IsPartOfUpgradableTo(SeedType theUpgradedType);
    void                    UpdateCobCannon();
    void                    CobCannonFire(int theTargetX, int theTargetY);
    void                    UpdateGoldMagnetShroom();
    /*inline*/ bool         IsOnBoard();
    void                    RemoveEffects();
    void                    UpdateCoffeeBean();
    void                    UpdateUmbrella();
    void                    EndBlink();
    void                    AnimateGarlic();
    Coin*                   FindGoldMagnetTarget();
    void                    SpikeweedAttack();
    void                    ImitaterMorph();
    void                    UpdateImitater();
    void                    UpdateReanim();
    void                    SpikeRockTakeDamage();
    bool                    IsSpiky();
    static /*inline*/ void  PreloadPlantResources(SeedType theSeedType);
    /*inline*/ bool         IsInPlay();
    void                    UpdateNeedsFood() { ; }
    void                    PlayIdleAnim(float theRate);
    void                    UpdateFlowerPot();
    void                    UpdateLilypad();
    void                    GoldMagnetFindTargets();
    bool                    IsAGoldMagnetAboutToSuck();
    bool                    DrawMagnetItemsOnTop();
    void                    ApplyBurn();
    void                    UpdateBurn();
    void                    ApplyChill(bool theIsIceTrap);
    void                    HitIceTrap(unsigned int duration);
    bool                    CanBeChilled();
    static bool             IsShooter(SeedType theSeedType);
    static bool             IsRoofShooter(SeedType theSeedType);
    static bool             IsInsta(SeedType theSeedType);
    static bool             IsTank(SeedType theSeedType);
};

float                       PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow);
float                       PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale);

class PlantDefinition
{
public:
    SeedType                mSeedType;          //+0x0
    Image**                 mPlantImage;        //+0x4
    ReanimationType         mReanimationType;   //+0x8
    int                     mPacketIndex;       //+0xC
    int                     mSeedCost;          //+0x10
    int                     mRefreshTime;       //+0x14
    PlantSubClass           mSubClass;          //+0x18
    int                     mLaunchRate;        //+0x1C
    const SexyChar*         mPlantName;         //+0x20
};
extern PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES];

/*inline*/ PlantDefinition& GetPlantDefinition(SeedType theSeedType);
