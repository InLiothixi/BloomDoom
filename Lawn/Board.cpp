#include <time.h>
#include "Bush.h"
#include "ZenGarden.h"
#include "BoardInclude.h"
#include "System/Music.h"
#include "System/SaveGame.h"
#include "Widget/LawnDialog.h"
#include "System/PlayerInfo.h"
#include "System/PoolEffect.h"
#include "System/PopDRMComm.h"
#include "System/TypingCheck.h"
#include "Widget/StoreScreen.h"
#include "Widget/AwardScreen.h"
#include "../Sexy.TodLib/Trail.h"
#include "Widget/ChallengeScreen.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "Widget/SeedChooserScreen.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../SexyAppFramework/Dialog.h"
#include "../SexyAppFramework/MTRand.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../SexyAppFramework/SysFont.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../Sexy.TodLib/TodStringFile.h"
#include "../SexyAppFramework/ImageFont.h"
#include "../SexyAppFramework/BassLoader.h"
#include "../SexyAppFramework/SoundManager.h"
#include "../SexyAppFramework/ButtonWidget.h"
#include "../SexyAppFramework/WidgetManager.h"
#include "../SexyAppFramework/SoundInstance.h"

#include "System/ReanimationLawn.h"
#include "../SexyAppFramework/MemoryImage.h"
#include "../Sexy.TodLib/ReanimAtlas.h"

#define SEXY_PERF_ENABLED
#include "../SexyAppFramework/PerfTimer.h"

//#define SEXY_MEMTRACE
//#include "../SexyAppFramework/memmgr.h"

bool gShownMoreSunTutorial = false;
float daisyTimer = 0;

//0x407B50
Board::Board(LawnApp* theApp)
{
	mApp = theApp;
	mApp->mMusic->mApp = mApp;
	mApp->mBoard = this;

	mX = BOARD_ADDITIONAL_WIDTH;
	mY = BOARD_OFFSET_Y;

	mHitOffsetX = BOARD_ADDITIONAL_WIDTH;
	mHitOffsetY = BOARD_OFFSET_Y;

	TodHesitationTrace("preboard");
	mExtraPlants.DataArrayInitialize(1024U, "extraplants");
	mMaxPlayers = 1;

	if (mApp->HasSecondarySeedBank())
		mMaxPlayers = 2;

	mPlayerController.DataArrayInitialize(4, "players");
	for (int i = 0; i < mMaxPlayers; i++)
	{
		mPlayers[i] = mPlayerController.DataArrayAlloc();
		mPlayers[i]->Initialize();
		mPlayers[i]->mPlayerID = i;
		if (i == 0) {
			mPlayers[i]->mX = GridToPixelX(0, 0) + 33;
			mPlayers[i]->mY = GridToPixelY(0, 0);
			mPlayers[i]->mAbilityMaxTime = 300;
			mPlayers[i]->mControlType = CONTROL_MOUSE;
		}
		else if (i == 1) {
			mPlayers[i]->mX = GridToPixelX(8, 0) + 33;
			mPlayers[i]->mY = GridToPixelY(8, 0);
			mPlayers[i]->mKeyUp = 'w';
			mPlayers[i]->mKeyLeft = 'a';
			mPlayers[i]->mKeyDown = 's';
			mPlayers[i]->mKeyRight = 'd';
			mPlayers[i]->mKeyLB = 'q';
			mPlayers[i]->mKeyRB = 'e';
			mPlayers[i]->mKeyA = 'z';
			mPlayers[i]->mKeyB = 'x';
			mPlayers[i]->mKeyX = 'c';
			mPlayers[i]->mAbilityType = ABILITY_BUTTER;
			mPlayers[i]->mAbilityMaxTime = 900;
		}
		else if (i == 2) {
			mPlayers[i]->mX = GridToPixelX(0, StageHas6Rows() ? 5 : 4) + 33;
			mPlayers[i]->mY = GridToPixelY(0, StageHas6Rows() ? 5 : 4) + (StageHasRoof() ? 15 : 0);
			mPlayers[i]->mAbilityType = ABILITY_GLOVE;

			mPlayers[i]->mKeyUp = 't';
			mPlayers[i]->mKeyLeft = 'f';
			mPlayers[i]->mKeyDown = 'g';
			mPlayers[i]->mKeyRight = 'h';
			mPlayers[i]->mKeyLB = 'r';
			mPlayers[i]->mKeyRB = 'y';
			mPlayers[i]->mKeyA = 'v';
			mPlayers[i]->mKeyB = 'b';
			mPlayers[i]->mKeyX = 'n';
			mPlayers[i]->mAbilityMaxTime = 900;
		}
		else if (i == 3) {
			mPlayers[i]->mX = GridToPixelX(8, StageHas6Rows() ? 5 : 4) + 33;
			mPlayers[i]->mY = GridToPixelY(8, StageHas6Rows() ? 5 : 4) + (StageHasRoof() ? 15 : 0);
			mPlayers[i]->mAbilityType = ABILITY_HAMMER;
			mPlayers[i]->mKeyUp = 'i';
			mPlayers[i]->mKeyLeft = 'j';
			mPlayers[i]->mKeyDown = 'k';
			mPlayers[i]->mKeyRight = 'l';
			mPlayers[i]->mKeyLB = 'u';
			mPlayers[i]->mKeyRB = 'o';
			mPlayers[i]->mKeyA = 'm';
			mPlayers[i]->mKeyB = 'p';
			mPlayers[i]->mKeyX = '.';
			mPlayers[i]->mAbilityMaxTime = 300;
		}
		mPlayers[i]->mControllerType = CONTROLLER_PLANT;
		mPlayers[i]->mControllerSpeed = 5;
	}
	mZombies.DataArrayInitialize(1024U, "zombies");
	mBushes.DataArrayInitialize(6U, "bushes");
	mBushesCheap.DataArrayInitialize(6U, "bushescheap");
	for (int i = 0; i < 6; i++) {
		mBushList[i] = mBushes.DataArrayAlloc();
		mBushCheapList[i] = mBushesCheap.DataArrayAlloc();
	}
	mPlants.DataArrayInitialize(1024U, "plants");
	mProjectiles.DataArrayInitialize(1024U, "projectiles");
	mCoins.DataArrayInitialize(1024U, "coins");
	mLawnMowers.DataArrayInitialize(32U, "lawnmowers");
	mGridItems.DataArrayInitialize(128U, "griditems");

	TodHesitationTrace("board dataarrays");
	mApp->mEffectSystem->EffectSystemFreeAll();
	mBoardRandSeed = mApp->mAppRandSeed;

	if (mApp->IsSurvivalMode())
		mBoardRandSeed = Rand();
	
	mCoinBankFadeCount = 0;
	mLevel = 0;
	mCursorObject = new CursorObject();
	mCursorPreview = new CursorPreview();
	mAdvice = new MessageWidget(mApp);
	mSeedBank = new SeedBank();
	mSeedBankP2 = new SeedBank();
	mSeedBankP2->misP2 = true;
	mCutScene = new CutScene();
	mSpecialGraveStoneX = -1;
	mSpecialGraveStoneY = -1;
	mSunProduced = 0;
	mDarknessCounter = 0;
	mUpdateDarkness = true;

	//mPreviewImage = mApp->mReanimatorCache->MakeBlankMemoryImage(BOARD_WIDTH, BOARD_HEIGHT);

	for (int i = 0; i < MAX_GRID_SIZE_X; i++)
	{
		for (int j = 0; j < MAX_GRID_SIZE_Y; j++)
		{
			mGridSquareType[i][j] = GridSquareType::GRIDSQUARE_GRASS;
			mFogGridCelLook[i][j] = mGridCelLook[i][j] = Rand(20);
			mGridCelOffset[i][j][0] = Rand(10) - 5;
			mGridCelOffset[i][j][1] = Rand(10) - 5;
		}

		for (int k = 0; k < MAX_GRID_SIZE_Y + 4; k++)
		{
			mFogGridCelFog[i][k] = mGridCelFog[i][k] = 0;
		}
	}
	for (int i = MAX_GRID_SIZE_X ; i < MAX_GRID_SIZE_X + 4; i++)
	{
		for (int j = 0; j < MAX_GRID_SIZE_Y; j++)
		{
			mFogGridCelLook[i][j] = Rand(20);
		}

		for (int k = 0; k < MAX_GRID_SIZE_Y + 4; k++)
		{
			mFogGridCelFog[i][k] = 0;
		}
	}
	mFogOffset = 0.0f;
	mSunCountDown = 0;
	mShakeCounter = 0;
	mShakeAmountX = 0;
	mShakeAmountY = 0;
	mRoofTreeOffset = mRoofPoleOffset = WIDE_BOARD_WIDTH - BOARD_ADDITIONAL_WIDTH + 70;

	//ENDS
	mPaused = false;

	mIsP1Keyboard = true;
	mLevelAwardSpawned = false;
	mFlagRaiseCounter = 0;
	mIceTrapCounter = 0;
	mLevelComplete = false;
	mBoardFadeOutCounter = -1;
	mNextSurvivalStageCounter = 0;
	mScoreNextMowerCounter = 0;
	mProgressMeterWidth = 0;
	mPoolSparklyParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
	mPoolSparklyParticleIDExtra2 = ParticleSystemID::PARTICLESYSTEMID_NULL;
	mPoolSparklyParticleIDExtra3 = ParticleSystemID::PARTICLESYSTEMID_NULL;
	mFogBlownCountDown = 0;
	mFwooshCountDown = 0;
	mTimeStopCounter = 0;
	mCobCannonCursorDelayCounter = 0;
	mCobCannonMouseX = 0;
	mCobCannonMouseY = 0;
	mDroppedFirstCoin = false;
	mBonusLawnMowersRemaining = 0;
	mEnableGraveStones = false;
	mHelpIndex = AdviceType::ADVICE_NONE;
	mEffectCounter = 0;
	mDrawCount = 0;
	mRiseFromGraveCounter = 0;
	mFinalWaveSoundCounter = 0;
	mKilledYeti = false;
	mTriggeredLawnMowers = 0;
	mPlayTimeActiveLevel = 0;
	mPlayTimeInactiveLevel = 0;
	mMaxSunPlants = 0;
	mStartDrawTime = 0;
	mIntervalDrawTime = 0;
	mIntervalDrawCountStart = 0;
	mPreloadTime = 0;
	mGameID = _time32(nullptr);
	mMinFPS = 1000.0f;
	mGravesCleared = 0;
	mPlantsEaten = 0;
	mPlantsShoveled = 0;
	mCoinsCollected = 0;
	mDiamondsCollected = 0;
	mPottedPlantsCollected = 0;
	mChocolateCollected = 0;

	for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < 12; x++)
		{
			mFwooshID[y][x] = ReanimationID::REANIMATIONID_NULL;
		}
	}
	mPrevMouseX = -1;
	mPrevMouseY = -1;
	mFinalBossKilled = false;
	mMustacheMode = mApp->mMustacheMode;
	mSuperMowerMode = mApp->mSuperMowerMode;
	mFutureMode = mApp->mFutureMode;
	mPinataMode = mApp->mPinataMode;
	mDanceMode = mApp->mDanceMode;
	mDaisyMode = mApp->mDaisyMode;
	mSukhbirMode = mApp->mSukhbirMode;
	mShowShovel = false;
	mToolTip = new ToolTipWidget();
	mDebugFont = new SysFont("Arial Unicode MS", 10, true, false, false);
	mBackground = BackgroundType::BACKGROUND_1_DAY;
	mMainCounter = 0;
	mTutorialState = TutorialState::TUTORIAL_OFF;
	mTutorialTimer = -1;
	mTutorialParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
	mChallenge = new Challenge();
	mClip = false;
	mDebugTextMode = DebugTextMode::DEBUG_TEXT_NONE;
	mIsSpeeding = false;
	mMenuButton = new GameButton(0);
	mMenuButton->mDrawStoneButton = true;
	mMenuButton->mParentWidget = this;
	mStoreButton = nullptr;
	mIgnoreMouseUp = false;

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		mMenuButton->SetLabel(_S("[MAIN_MENU_BUTTON]"));
		mMenuButton->Resize(628 + BOARD_ADDITIONAL_WIDTH, -10 - BOARD_OFFSET_Y, 163, 46);

		mStoreButton = new GameButton(1);
		mStoreButton->mButtonImage = IMAGE_ZENSHOPBUTTON;
		mStoreButton->mOverImage = IMAGE_ZENSHOPBUTTON_HIGHLIGHT;
		mStoreButton->mDownImage = IMAGE_ZENSHOPBUTTON_HIGHLIGHT;
		mStoreButton->mParentWidget = this;
		mStoreButton->Resize(678 + BOARD_ADDITIONAL_WIDTH, 33 - BOARD_OFFSET_Y, IMAGE_ZENSHOPBUTTON->mWidth, 40);
	}
	else
	{
		mMenuButton->SetLabel(_S("[MENU_BUTTON]"));
		mMenuButton->Resize(681 + BOARD_ADDITIONAL_WIDTH, -10 - BOARD_OFFSET_Y, 117, 46);

		/*if (mMaxPlayers > 1)
			mMenuButton->mX += 75.0f;*/
	}

	mSpeedButton = new GameButton(0);
	mSpeedButton->Resize(mMenuButton->mX - 58, mMenuButton->mY, 58, 46);
	mSpeedButton->mButtonImage = IMAGE_SPEEDBUTTON;
	mSpeedButton->mParentWidget = this;

	if (LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		mStoreButton = new GameButton(1);
		mStoreButton->mDrawStoneButton = true;
		mStoreButton->mBtnNoDraw = true;
		mStoreButton->mDisabled = true;
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
		mStoreButton = new GameButton(1);
		mStoreButton->mDrawStoneButton = true;
		mStoreButton->SetLabel(_S("Shout!"));
		mStoreButton->Resize(319, -BOARD_OFFSET_Y, 163, 46);
		mStoreButton->mParentWidget = this;
		mStoreButton->mDisabled = true;
		mStoreButton->mBtnNoDraw = true;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_UPSELL)
	{
		mMenuButton->SetLabel(_S("[MAIN_MENU_BUTTON]"));
		mMenuButton->Resize(628 + BOARD_ADDITIONAL_WIDTH, -10 - BOARD_OFFSET_Y, 163, 46);

		mStoreButton = new GameButton(1);
		mStoreButton->mDrawStoneButton = true;
		mStoreButton->mBtnNoDraw = true;
		mStoreButton->SetLabel(_S("[GET_FULL_VERSION_BUTTON]"));
	}
}

//0x408670、0x408690
Board::~Board()
{
	delete mAdvice;
	delete mCursorObject;
	delete mCursorPreview;
	delete mSeedBank;

	if (mSeedBankP2)
	{
		delete mSeedBankP2;
	}

	if (mMenuButton)
	{
		delete mMenuButton;
	}
	if (mStoreButton)
	{
		delete mStoreButton;
	}
	if (mSpeedButton)
	{
		delete mSpeedButton;
	}

	mExtraPlants.DataArrayDispose();
	mPlayerController.DataArrayDispose();
	mZombies.DataArrayDispose();
	mBushes.DataArrayDispose();
	mBushesCheap.DataArrayDispose();
	mPlants.DataArrayDispose();
	mExtraPlants.DataArrayDispose();
	mProjectiles.DataArrayDispose();
	mCoins.DataArrayDispose();
	mLawnMowers.DataArrayDispose();
	mGridItems.DataArrayDispose();
	
	if (mToolTip)
	{
		delete mToolTip;
	}
	if (mDebugFont)
	{
		delete mDebugFont;
	}

	delete mCutScene;
	delete mChallenge;
}

void BoardInitForPlayer()
{
	gShownMoreSunTutorial = false;
}

//0x408A70
void Board::DisposeBoard()
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
		mApp->mZenGarden->LeaveGarden();
	if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
		mChallenge->TreeOfWisdomLeave();

	mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_RAIN);
	mApp->mZenGarden->mBoard = nullptr;
	mApp->CrazyDaveDie();
	mApp->mEffectSystem->EffectSystemFreeAll();
}

//0x408B00
bool Board::AreEnemyZombiesOnScreen()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mHasHead && !aZombie->IsDeadOrDying() && !aZombie->mMindControlled)
		{
			return true;
		}
	}
	return false;
}


//0x408B60
int Board::CountZombiesOnScreen()
{
	int aCount = 0;
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mHasHead && !aZombie->IsDeadOrDying() && !aZombie->mMindControlled && aZombie->IsOnBoard())
		{
			aCount++;
		}
	}
	return aCount;
}

int Board::CountPlantsOnScreen()
{
	int aCount = 0;
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mState != STATE_BURNED && aPlant->IsOnBoard())
		{
			aCount++;
		}
	}
	return aCount;
}

//0x408BF0
int Board::CountUntriggerLawnMowers()
{
	int aCount = 0;
	LawnMower* aLawnMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		if (aLawnMower->mMowerState != LawnMowerState::MOWER_TRIGGERED && aLawnMower->mMowerState != LawnMowerState::MOWER_SQUISHED)
		{
			aCount++;
		}
	}
	return aCount;
}

//0x408C30
void Board::TryToSaveGame()
{
	std::string aFileName = GetSavedGameName(mApp->mGameMode, mApp->mPlayerInfo->mId);

	if (NeedSaveGame())
	{
		if (mBoardFadeOutCounter > 0)
		{
			CompleteEndLevelSequenceForSaving();
			return;
		}

		MkDir(GetAppDataFolder() + "userdata");
		mApp->mMusic->GameMusicPause(true);
		LawnSaveGame(this, aFileName);
		mApp->ClearUpdateBacklog();
		SurvivalSaveScore();
	}
}

void Board::SaveCurrentTime(std::string value)
{
	MkDir(GetAppDataFolder() + "userdata");
	//mApp->mMusic->GameMusicPause(true);
	
	LawnSaveGame(this, GetAppDataFolder() + StrFormat("userdata\\game%d_", mApp->mPlayerInfo->mId) + value + ".dat");
	mApp->ClearUpdateBacklog();
	SurvivalSaveScore();
}

//0x408DA0
bool Board::NeedSaveGame()
{
	return 
		mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL && 
		mApp->mGameMode != GameMode::GAMEMODE_UPSELL && 
		mApp->mGameMode != GameMode::GAMEMODE_INTRO && 
		mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && 
		mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM && 
		mApp->mGameScene == GameScenes::SCENE_PLAYING;
}

void Board::SaveGame(const string& theFileName)
{ 
	LawnSaveGame(this, theFileName);
}

void Board::ResetFPSStats()
{
	DWORD aTickCount = GetTickCount();
	mStartDrawTime = aTickCount;
	mIntervalDrawTime = aTickCount;
	mDrawCount = 1;
	mIntervalDrawCountStart = 1;
}

//0x408DE0
bool Board::LoadGame(const string& theFileName)
{
	if (!LawnLoadGame(this, theFileName))
		return false;

	LoadBackgroundImages();
	mApp->ClearUpdateBacklog();
	ResetFPSStats();

	HideSpeedButton();
	if (mSpeedButton && mApp->mGameMode != GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GAMEMODE_TREE_OF_WISDOM)
	{
		mSpeedButton->mBtnNoDraw = false;
		mSpeedButton->mButtonImage = IMAGE_SPEEDBUTTON;
	}

	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
		mSpeedButton->mDisabled = mSpeedButton->mBtnNoDraw = true;
	}

	UpdateLayers();
	return true;
}

//0x408E40
GridItem* Board::GetGridItemAt(GridItemType theGridItemType, int theGridX, int theGridY)
{
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridX == theGridX && aGridItem->mGridY == theGridY && aGridItem->mGridItemType == theGridItemType)
		{
			return aGridItem;
		}
	}
	return nullptr;
}

//0x408E90
GridItem* Board::GetRake()
{
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_RAKE)
		{
			return aGridItem;
		}
	}
	return nullptr;
}

GridItem* Board::GetCraterAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_CRATER, theGridX, theGridY);
}

GridItem* Board::GetGraveStoneAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_GRAVESTONE, theGridX, theGridY);
}

GridItem* Board::GetLadderAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_LADDER, theGridX, theGridY);
}

GridItem* Board::GetScaryPotAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_SCARY_POT, theGridX, theGridY);
}

GridItem* Board::GetSquirrelAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_SQUIRREL, theGridX, theGridY);
}

GridItem* Board::GetZenToolAt(int theGridX, int theGridY)
{
	return GetGridItemAt(GridItemType::GRIDITEM_ZEN_TOOL, theGridX, theGridY);
}

//0x408ED0
bool Board::CanAddGraveStoneAt(int theGridX, int theGridY)
{
	if (mBackground == BackgroundType::BACKGROUND_GRAVEYARD)
	{
		//None
	}
    else if (mGridSquareType[theGridX][theGridY] != GridSquareType::GRIDSQUARE_GRASS && mGridSquareType[theGridX][theGridY] != GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		return false;
	}

	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridX == theGridX && aGridItem->mGridY == theGridY)
		{
			if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE || 
				aGridItem->mGridItemType == GridItemType::GRIDITEM_CRATER || 
				aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
				return false;
		}
	}
	return true;
}

int Board::MakeRenderOrder(RenderLayer theRenderLayer, int theRow, int theLayerOffset)
{
	return theRow * (int)RenderLayer::RENDER_LAYER_ROW_OFFSET + theRenderLayer + theLayerOffset;
}

//0x408F40
GridItem* Board::AddALadder(int theGridX, int theGridY)
{
	GridItem* aLadder = mGridItems.DataArrayAlloc();
	aLadder->mGridItemType = GridItemType::GRIDITEM_LADDER;
	aLadder->mRenderOrder = MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, theGridY, 800);
	aLadder->mGridX = theGridX;
	aLadder->mGridY = theGridY;
	return aLadder;
}

//0x408F80
GridItem* Board::AddACrater(int theGridX, int theGridY)
{
	GridItem* aCrater = mGridItems.DataArrayAlloc();
	aCrater->mGridItemType = GridItemType::GRIDITEM_CRATER;
	aCrater->mRenderOrder = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, theGridY, 1);
	aCrater->mGridX = theGridX;
	aCrater->mGridY = theGridY;
	return aCrater;
}

GridItem* Board::AddAGraveStone(int theGridX, int theGridY)
{
	GridItem* aGraveStone = mGridItems.DataArrayAlloc();
	aGraveStone->mGridItemType = GridItemType::GRIDITEM_GRAVESTONE;
	aGraveStone->mGridItemCounter = -Rand(50);
	aGraveStone->mRenderOrder = MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, theGridY, 3);
	aGraveStone->mGridX = theGridX;
	aGraveStone->mGridY = theGridY;
	return aGraveStone;
}

//0x408FC0
void Board::AddGraveStones(int theGridX, int theCount, MTRand& theLevelRNG)
{
	TOD_ASSERT(theCount <= MAX_GRID_SIZE_Y);

	// 这里姑且加一个原版没有的、对于本列能否生成墓碑的判断
	// 如果没有这个判断，当本列不存在足够多的格子可以放置墓碑时，游戏会卡死
	GridItem* aGridItem = nullptr;
	//bool aAllowGraveStone[MAX_GRID_SIZE_Y] = { false };
	int aGridAllowGraveStonesCount = 0;
	int maxY = MAX_GRID_SIZE_Y;
	if (mBackground == BackgroundType::BACKGROUND_GRAVEYARD)
		maxY = 5;

	for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
	{
		if (CanAddGraveStoneAt(theGridX, y))
		{
			aGridAllowGraveStonesCount++;
		}
	}
	theCount = min(theCount, aGridAllowGraveStonesCount);

	int i = 0;
	while (i < theCount)
	{
		int aGridY = theLevelRNG.Next((unsigned long)maxY);
		//if (aAllowGraveStone[aGridY])
		//{
		//	aAllowGraveStone[aGridY] = false;
		//	GridItem* aGraveStone = AddAGraveStone(theGridX, aGridY);
		//	++i;
		//}
		// 上述写法虽然效率更高，但当 AddAGraveStone() 函数被修改后，不能保证 aAllowGraveStone 仍然有效
		// 故这里仍然采用如下的原版的写法，仅在上面对 theCount 进行修正
		if (CanAddGraveStoneAt(theGridX, aGridY))
		{
			GridItem* aGraveStone = AddAGraveStone(theGridX, aGridY);
			++i;
		}
	}
}

//0x409050
int Board::GetNumWavesPerFlag()
{
	return (mApp->IsFirstTimeAdventureMode() && mNumWaves < 10) ? mNumWaves : (mIsVeteran ? (int)round(10 * GetVeteranMultiplier()) : 10);
}

//0x409080
bool Board::IsFlagWave(int theWaveNumber)
{
	if (mApp->IsFirstTimeAdventureMode() && mLevel == 1)
		return false;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL || mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
		return false;

	int aWavesPerFlag = GetNumWavesPerFlag();

	return theWaveNumber % aWavesPerFlag == aWavesPerFlag - 1;
}

//0x4090F0
void ZombiePickerInitForWave(ZombiePicker* theZombiePicker)
{
	memset(theZombiePicker, 0, sizeof(ZombiePicker));
}

//0x409170
void ZombiePickerInit(ZombiePicker* theZombiePicker)
{
	ZombiePickerInitForWave(theZombiePicker);
	memset(theZombiePicker->mAllWavesZombieTypeCount, 0, sizeof(theZombiePicker->mAllWavesZombieTypeCount));
}

void Board::AnimateBush(int row) {
	if (!mApp->Is3dAccel() || mApp->mGameMode == GAMEMODE_CHALLENGE_INVISIGHOUL)
		return;

	Bush* aBush = mBushList[row];
	if (aBush)
		aBush->AnimateBush();
}

//0x409240
void Board::PutZombieInWave(ZombieType theZombieType, int theWaveNumber, ZombiePicker* theZombiePicker)
{
	TOD_ASSERT(theWaveNumber < MAX_ZOMBIE_WAVES && theZombiePicker->mZombieCount < MAX_ZOMBIES_IN_WAVE);
	
	mZombiesInWave[theWaveNumber][theZombiePicker->mZombieCount++] = theZombieType;
	if (theZombiePicker->mZombieCount < MAX_ZOMBIES_IN_WAVE || theZombieType == ZOMBIE_INVALID)
	{
		mZombiesInWave[theWaveNumber][theZombiePicker->mZombieCount] = ZombieType::ZOMBIE_INVALID;
		if (theZombieType == ZOMBIE_INVALID)
			return;
	}
	theZombiePicker->mZombiePoints -= GetZombieDefinition(theZombieType).mZombieValue;
	theZombiePicker->mZombieTypeCount[theZombieType]++;
	theZombiePicker->mAllWavesZombieTypeCount[theZombieType]++;
}

//0x409290
void Board::PutInMissingZombies(int theWaveNumber, ZombiePicker* theZombiePicker)
{
	for (ZombieType aZombieType = ZombieType::ZOMBIE_NORMAL; aZombieType < ZombieType::NUM_ZOMBIE_TYPES; aZombieType = (ZombieType)((int)aZombieType + 1))
	{
		if (theZombiePicker->mZombieTypeCount[(int)aZombieType] <= 0 && aZombieType != ZombieType::ZOMBIE_YETI && CanZombieSpawnOnLevel(aZombieType, mLevel))
		{
			PutZombieInWave(aZombieType, theWaveNumber, theZombiePicker);
		}
	}
}


//0x4092E0
void Board::PickZombieWaves()
{
	// ====================================================================================================
	// ▲ 设定关卡总波数
	// ====================================================================================================
	if (mApp->IsAdventureMode())
	{
		if (mApp->IsWhackAZombieLevel())
		{
			mNumWaves = 8;
		}
		else
		{
			mNumWaves = gZombieWaves[ClampInt(mLevel - 1, 0, 59)]; //49
			if (!mApp->IsFirstTimeAdventureMode() && !mApp->IsMiniBossLevel())
			{
				mNumWaves = mNumWaves < 10 ? 20 : mNumWaves;
			}
		}
	}
	else
	{ //EDITWAVE
		GameMode aGameMode = mApp->mGameMode;
		if (mApp->IsSurvivalMode() || LawnApp::IsLastStandLevel(aGameMode))
		{
			mNumWaves = GetNumWavesPerSurvivalStage();
		}
		else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || aGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || 
			mApp->IsSquirrelLevel() || aGameMode == GameMode::GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		{
			mNumWaves = 0;
		}
		else if (mApp->IsShovelLevel())
		{
			mNumWaves = 10;
		}
		else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2)
		{
			mNumWaves = 12;
		}
		else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_HEAT_WAVE)
		{
			mNumWaves = 15;
		}
		else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING || aGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVE_DANGER || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY || aGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY || aGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN || aGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL_REVERSE ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_NONE_SHALL_PASS || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS ||
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY ||
			mApp->IsBigTroubleLevel())
		{
			mNumWaves = 20;
		}
		else if (mApp->IsStormyNightLevel() || mApp->IsLittleTroubleLevel() || mApp->IsBungeeBlitzLevel() ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN || aGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2 ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2 || aGameMode == GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_3 || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2 || aGameMode == GameMode::GAMEMODE_CHALLENGE_POGO_PARTY || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_DARK_STORMY_NIGHT || aGameMode == GameMode::GAMEMODE_CHALLENGE_METAL_FESTIVAL ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE || aGameMode == GameMode::GAMEMODE_CHALLENGE_BOMB_ALL || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_BORDER_TREATY || aGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED_2 || aGameMode == GameMode::GAMEMODE_CHALLENGE_LIGHTS_OUT ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_INFLATION || aGameMode == GameMode::GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT || 
			aGameMode == GameMode::GAMEMODE_CHALLENGE_BLIND_FAITH || aGameMode == GameMode::GAMEMODE_CHALLENGE_BLOOD_MOON ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_BRUTAL_EVENING || aGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY ||
			aGameMode == GameMode::GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS_2)
		{
			mNumWaves = 30;
		}
		else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_EVERYONE_IS_DEAD)
		{
			mNumWaves = 100;
		}
		else
		{
			mNumWaves = 40;
		}
	}

	if (mIsVeteran)
	{
		mNumWaves *= GetVeteranMultiplier();
	}

	// ====================================================================================================
	// ▲ 一些准备工作
	// ====================================================================================================
	ZombiePicker aZombiePicker;
	ZombiePickerInit(&aZombiePicker);
	ZombieType aIntroZombieType = GetIntroducedZombieType();
	TOD_ASSERT(mNumWaves <= MAX_ZOMBIE_WAVES);

	// ====================================================================================================
	// ▲ 遍历每一波并填充每波的出怪列表
	// ====================================================================================================
	for (int aWave = 0; aWave < mNumWaves; aWave++)
	{
		ZombiePickerInitForWave(&aZombiePicker);
		mZombiesInWave[aWave][0] = ZombieType::ZOMBIE_INVALID;

		bool aIsFlagWave = IsFlagWave(aWave);
		bool aIsFinalWave = aWave == mNumWaves - 1;

		if ((mApp->IsBungeeBlitzLevel()) && aIsFlagWave)
		{
			// 蹦极闪电战关卡的每大波固定刷出 5 只蹦极僵尸
			for (int _i = 0; _i < 5; _i++)
				PutZombieInWave(ZombieType::ZOMBIE_BUNGEE, aWave, &aZombiePicker);

			if (!aIsFinalWave)
				continue;
		}

		// ------------------------------------------------------------------------------------------------
		// △ 计算该波的僵尸总点数
		// ------------------------------------------------------------------------------------------------
		int& aZombiePoints = aZombiePicker.mZombiePoints;
		// 根据关卡计算本波的基础僵尸点数
		if (LawnApp::IsLastStandLevel(mApp->mGameMode))
		{
			aZombiePoints = (mChallenge->mSurvivalStage * GetNumWavesPerSurvivalStage() + aWave + 10) * 2 / 5 + 1;
		}
		else if (mApp->IsSurvivalMode() && mChallenge->mSurvivalStage > 0)
		{
			aZombiePoints = (mChallenge->mSurvivalStage * GetNumWavesPerSurvivalStage() + aWave) * 2 / 5 + 1;
		}
		else if (mApp->IsAdventureMode() && mApp->HasFinishedAdventure() && mLevel != 5)
		{
			aZombiePoints = aWave * 2 / 5 + 1;
		}
		else
		{
			aZombiePoints = aWave / 3 + 1;
		}

		// 旗帜波的特殊调整
		if (aIsFlagWave)
		{
			int aPlainZombiesNum = min(aZombiePoints, 8);
			aZombiePoints *= 2.5f;

			if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2)
			{
				if (mApp->mGameMode == GAMEMODE_CHALLENGE_POGO_PARTY)
				{
					for (int _i = 0; _i < aPlainZombiesNum; _i++)
					{
						PutZombieInWave(ZombieType::ZOMBIE_POGO, aWave, &aZombiePicker);
					}
				}
				else if (mZombieAllowed[ZombieType::ZOMBIE_NORMAL] && 
					mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_WE_SMASH && 
					mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_METAL_FESTIVAL)
				{
					for (int _i = 0; _i < aPlainZombiesNum; _i++)
					{
						PutZombieInWave(ZombieType::ZOMBIE_NORMAL, aWave, &aZombiePicker);
					}
				}

				PutZombieInWave(ZombieType::ZOMBIE_FLAG, aWave, &aZombiePicker);
			}
		}

		// 部分关卡的多倍出怪
		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN || mApp->mGameMode == GAMEMODE_CHALLENGE_BUTTERED_POPCORN)
		{
			aZombiePoints *= 6.0f;
		}
		else if (mApp->IsLittleTroubleLevel() || mApp->IsWallnutBowlingLevel())
		{
			aZombiePoints *= 4.0f;
		}
		else if (mApp->IsMiniBossLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_OPEN_FIRE || mApp->IsStormyNightLevel())
		{
			aZombiePoints *= 3.0f;
		}
		else if (mApp->IsBigTroubleLevel() || mApp->IsShovelLevel() || mApp->IsBungeeBlitzLevel() || 
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || 
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2 || 
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL ||
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_WE_SMASH)
		{
			aZombiePoints *= 2.0f;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL) {
			aZombiePoints *= 1.5f;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
		{
			aZombiePoints /= 2.0f;
		}
		
		// ------------------------------------------------------------------------------------------------
		// △ 向出怪列表中加入固定刷出的僵尸
		// ------------------------------------------------------------------------------------------------
		// 部分新出现的僵尸会在特定波固定刷出
		if (aIntroZombieType != ZombieType::ZOMBIE_INVALID && aIntroZombieType != ZombieType::ZOMBIE_DUCKY_TUBE)
		{
			bool aSpawnIntro = false;
			if ((aIntroZombieType == ZombieType::ZOMBIE_DIGGER || aIntroZombieType == ZombieType::ZOMBIE_BALLOON))
			{
				if (aWave + 1== 7 || aIsFinalWave)
				{
					aSpawnIntro = true;
				}
			}
			else if (aIntroZombieType == ZombieType::ZOMBIE_YETI)
			{
				if (aWave == mNumWaves / 2 && !mApp->mSawYeti)
				{
					aSpawnIntro = true;
				}
			}
			else if (aWave == mNumWaves / 2 || aIsFinalWave)
			{
				aSpawnIntro = true;
			}

			if (aSpawnIntro)
			{
				PutZombieInWave(aIntroZombieType, aWave, &aZombiePicker);
			}
		}

		// 5-10 关卡的最后一波加入一只伽刚特尔
		if (mLevel == 50 && aIsFinalWave)
		{
			PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
		}
		if (mLevel == 59 && aIsFinalWave)
		{
			PutZombieInWave(ZombieType::ZOMBIE_REDEYE_GARGANTUAR, aWave, &aZombiePicker);
			PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
		}
		// 冒险模式关卡的最后一波会出现本关卡可能出现的所有僵尸
		if (mApp->IsAdventureMode() && aIsFinalWave)
		{
			PutInMissingZombies(aWave, &aZombiePicker);
		}
		// 柱子关卡的特殊出怪
		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
		{
			// 每大波的第 5 小波，固定出现 10 只扶梯僵尸
			if (aWave % 10 == 5)
			{
				for (int _i = 0; _i < 10; _i++)
				{
					PutZombieInWave(ZombieType::ZOMBIE_LADDER, aWave, &aZombiePicker);
				}
			}

			// 每大波的第 8 小波，固定出现 10 只玩偶匣僵尸
			if (aWave % 10 == 8)
			{
				for (int _i = 0; _i < 10; _i++)
				{
					PutZombieInWave(ZombieType::ZOMBIE_JACK_IN_THE_BOX, aWave, &aZombiePicker);
				}
			}

			// 第 19/29 小波，固定出现 3/5 只伽刚特尔
			if (aWave == 19)
			{
				for (int _i = 0; _i < 3; _i++)
				{
					PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
				}
			}
			if (aWave == 29)
			{
				for (int _i = 0; _i < 5; _i++)
				{
					PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
				}
			}
		}else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL){
			if (aWave == 29){
				PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
				PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
			}
		}else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
			if (aWave == 7) {
				PutZombieInWave(ZombieType::ZOMBIE_ZAMBONI, aWave, &aZombiePicker);
			}else if (aWave == 14) {
				PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, aWave, &aZombiePicker);
			}
		}
		
		// ------------------------------------------------------------------------------------------------
		// △ 剩余的僵尸点数用于向列表中补充随机僵尸
		// ------------------------------------------------------------------------------------------------
		while (aZombiePoints > 0 && aZombiePicker.mZombieCount < MAX_ZOMBIES_IN_WAVE)
		{
			ZombieType aZombieType = PickZombieType(aZombiePoints, aWave, &aZombiePicker);
			PutZombieInWave(aZombieType, aWave, &aZombiePicker);
		}
	}
}

//0x40A110
int Board::GetLevelRandSeed()
{
	int aRndSeed = mApp->mPlayerInfo->mId + mBoardRandSeed;
	if (mApp->IsAdventureMode())
	{
		aRndSeed += mApp->mPlayerInfo->mFinishedAdventure * 101 + mLevel;
	}
	else
	{
		aRndSeed += mChallenge->mSurvivalStage * 101 + mApp->mGameMode;
	}
	return aRndSeed;
}

//0x40A160
void Board::LoadBackgroundImages()
{
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
	{
		TodLoadResources("DelayLoad_Background6");
		TodLoadResources("DelayLoad_Background5");
		return;
	}

	if (StageHasRain() || StageHasFog())
	{
		TodLoadResources("DelayLoad_Background4");
	}

	if (StageHasDarkness())
	{
		mDarknessImage = mApp->mReanimatorCache->MakeBlankMemoryImage(BOARD_WIDTH, BOARD_HEIGHT);
		mDarknessDarkerImage = mApp->mReanimatorCache->MakeBlankMemoryImage(BOARD_WIDTH, BOARD_HEIGHT);
		mDarknessImage->FillRect(Rect(0, 0, mDarknessImage->mWidth, mDarknessImage->mHeight), Color::Black, 0);
		mDarknessDarkerImage->FillRect(Rect(0, 0, mDarknessImage->mWidth, mDarknessImage->mHeight), Color::Black, 0);
	}

	switch (mBackground)
	{
	case BackgroundType::BACKGROUND_1_DAY:
	{
		TodLoadResources("DelayLoad_Background1");
		if ((mApp->IsAdventureMode() && mLevel <= 4) || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED)
			TodLoadResources("DelayLoad_BackgroundUnsodded");
		break;
	}

	case BackgroundType::BACKGROUND_2_NIGHT:
		TodLoadResources("DelayLoad_Background2");
		break;

	case BackgroundType::BACKGROUND_3_POOL:
		TodLoadResources("DelayLoad_Background3");
		break;

	case BackgroundType::BACKGROUND_4_FOG:
		TodLoadResources("DelayLoad_Background4");
		break;

	case BackgroundType::BACKGROUND_5_ROOF:
		TodLoadResources("DelayLoad_Background5");
		break;

	case BackgroundType::BACKGROUND_6_BOSS:
		TodLoadResources("DelayLoad_Background6");
		break;
	
	case BackgroundType::BACKGROUND_GREENHOUSE:
		TodLoadResources("DelayLoad_GreenHouseGarden");
		TodLoadResources("DelayLoad_GreenHouseOverlay");
		break;

	case BackgroundType::BACKGROUND_TREEOFWISDOM:
		ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_TREEOFWISDOM, true);
		break;

	case BackgroundType::BACKGROUND_ZOMBIQUARIUM:
		TodLoadResources("DelayLoad_Zombiquarium");
		TodLoadResources("DelayLoad_GreenHouseOverlay");
		break;

	case BackgroundType::BACKGROUND_MUSHROOM_GARDEN:
		TodLoadResources("DelayLoad_MushroomGarden");
		break;

	case BackgroundType::BACKGROUND_3_POOLWIDE:
	{
		TodLoadResources("DelayLoad_Background3");
		TodLoadResources("DelayLoad_Background3Wide");
		break;
	}

	case BackgroundType::BACKGROUND_3_UNSODDED:
	{
		TodLoadResources("DelayLoad_Background3");
		TodLoadResources("DelayLoad_Background3Unsodded");
		break;
	}

	case BackgroundType::BACKGROUND_3_GRASS:
	{
		TodLoadResources("DelayLoad_Background3");
		TodLoadResources("DelayLoad_Background3Grass");
		break;
	}

	case BackgroundType::BACKGROUND_HIGHGROUND:
	{
		TodLoadResources("DelayLoad_Background4");
		TodLoadResources("DelayLoad_BackgroundHighground");
		break;
	}

	case BackgroundType::BACKGROUND_6_AIRRAID:
	{
		mChallenge->mAirRaidCLOUDTime = RandRangeInt(0, 1920);
		mChallenge->mAirRaidSTARTime = RandRangeInt(0, 1920);
		TodLoadResources("DelayLoad_Background6AirRaid");
		break;
	}

	case BackgroundType::BACKGROUND_GRAVEYARD:
	{
		TodLoadResources("DelayLoad_BackgroundGraveyard");
		break;
	}

	default:
		TOD_ASSERT();
		break;
	}
	
}

//0x40A550
void Board::PickBackground()
{
	switch (mApp->mGameMode)
	{
	case GameMode::GAMEMODE_ADVENTURE:
		if (mLevel <= 1 * LEVELS_PER_AREA)
		{
			mBackground = BackgroundType::BACKGROUND_1_DAY;
		}
		else if (mLevel <= 2 * LEVELS_PER_AREA)
		{
			mBackground = BackgroundType::BACKGROUND_2_NIGHT;
		}
		else if (mLevel <= 3 * LEVELS_PER_AREA)
		{
			mBackground = BackgroundType::BACKGROUND_3_POOL;
		}
		else if (mApp->IsScaryPotterLevel())
		{
			mBackground = BackgroundType::BACKGROUND_2_NIGHT;
		}
		else if (mLevel <= 4 * LEVELS_PER_AREA)
		{
			mBackground = BackgroundType::BACKGROUND_4_FOG;
		}
		else if (mLevel <= 5 * LEVELS_PER_AREA)
		{
			mBackground = BackgroundType::BACKGROUND_5_ROOF;
		}
		else if (mLevel < FINAL_LEVEL || mLevel == FINAL_LEVEL)
		{
			mBackground = BackgroundType::BACKGROUND_6_BOSS;
		}
		else
		{
			mBackground = BackgroundType::BACKGROUND_1_DAY;
		}
		break;

	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_1:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_1:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_1:
	case GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY:
	case GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING:
	case GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE:
	case GameMode::GAMEMODE_CHALLENGE_SEEING_STARS:
	case GameMode::GAMEMODE_CHALLENGE_ART_SEEING_NUTS:
	case GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY:
	case GameMode::GAMEMODE_CHALLENGE_UNSODDED:
	case GameMode::GAMEMODE_CHALLENGE_BIG_TIME:
	case GameMode::GAMEMODE_CHALLENGE_SEEING_FLOWERS:
	case GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL:
	case GameMode::GAMEMODE_CHALLENGE_CAN_YOU_DIG_IT:
	case GameMode::GAMEMODE_CHALLENGE_SQUIRREL:
	case GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_DAY:
	case GameMode::GAMEMODE_CHALLENGE_BORDER_TREATY:
	case GameMode::GAMEMODE_CHALLENGE_NONE_SHALL_PASS:
	case GameMode::GAMEMODE_CHALLENGE_FLOOR_IS_LAVA:
	case GameMode::GAMEMODE_CHALLENGE_BOMB_ALL:
		mBackground = BackgroundType::BACKGROUND_1_DAY;
		break;

	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_2:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_2:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_2:
	case GameMode::GAMEMODE_CHALLENGE_BEGHOULED:
	case GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST:
	case GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT:
	case GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE:
	case GameMode::GAMEMODE_SCARY_POTTER_1:
	case GameMode::GAMEMODE_SCARY_POTTER_2:
	case GameMode::GAMEMODE_SCARY_POTTER_3:
	case GameMode::GAMEMODE_SCARY_POTTER_4:
	case GameMode::GAMEMODE_SCARY_POTTER_5:
	case GameMode::GAMEMODE_SCARY_POTTER_6:
	case GameMode::GAMEMODE_SCARY_POTTER_7:
	case GameMode::GAMEMODE_SCARY_POTTER_8:
	case GameMode::GAMEMODE_SCARY_POTTER_9:
	case GameMode::GAMEMODE_SCARY_POTTER_ENDLESS:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9:
	case GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS:
	case GameMode::GAMEMODE_CHALLENGE_SURPRISE_ATTACK:
	case GameMode::GAMEMODE_CHALLENGE_DANCE_PARTY:
	case GameMode::GAMEMODE_CHALLENGE_HAIL:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_NIGHT:
	case GameMode::GAMEMODE_CHALLENGE_RENEWABLE_ENERGY:
	case GameMode::GAMEMODE_CHALLENGE_METAL_FESTIVAL:
	case GameMode::GAMEMODE_CHALLENGE_BRUTAL_EVENING:
	case GameMode::GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT:
	case GameMode::GAMEMODE_CHALLENGE_GRAVE_DANGER:
		mBackground = BackgroundType::BACKGROUND_2_NIGHT;
		break;

	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_3:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_3:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_3:
	case GameMode::GAMEMODE_CHALLENGE_LITTLE_TROUBLE:
	case GameMode::GAMEMODE_CHALLENGE_SLOWMO:
	case GameMode::GAMEMODE_CHALLENGE_SPEED:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_POOL:
	case GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2:
	case GameMode::GAMEMODE_UPSELL:
	case GameMode::GAMEMODE_INTRO:
	case GameMode::GAMEMODE_CHALLENGE_HEAT_WAVE:
	case GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN:
	case GameMode::GAMEMODE_CHALLENGE_INFLATION:
	case GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA:
		mBackground = BackgroundType::BACKGROUND_3_POOL;
		break;

	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_4:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_4:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_4:
	case GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS:
	case GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL:
	case GameMode::GAMEMODE_CHALLENGE_DARK_STORMY_NIGHT:
	case GameMode::GAMEMODE_CHALLENGE_BLIND_FAITH:
	case GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_FOG:
	case GameMode::GAMEMODE_CHALLENGE_EVERYONE_IS_DEAD:
	case GameMode::GAMEMODE_CHALLENGE_LIGHTS_OUT:
		mBackground = BackgroundType::BACKGROUND_4_FOG;
		break;

	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_5:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_5:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_5:
	case GameMode::GAMEMODE_CHALLENGE_COLUMN:
	case GameMode::GAMEMODE_CHALLENGE_POGO_PARTY:
	case GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY:
	case GameMode::GAMEMODE_CHALLENGE_BUNGEE_BLITZ:
	case GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_3:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_ROOF:
	case GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE_2:
		mBackground = BackgroundType::BACKGROUND_5_ROOF;
		break;

	case GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS_2:
	case GameMode::GAMEMODE_CHALLENGE_BIG_TROUBLE:
	case GameMode::GAMEMODE_CHALLENGE_DR_ZOMBOSS_REVENGE:
	case GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_6:
	case GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_6:
	case GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_6:
	case GameMode::GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES:
	case GameMode::GAMEMODE_CHALLENGE_BLOOD_MOON:
	case GameMode::GAMEMODE_CHALLENGE_LAST_STAND_GLOOM:
	case GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2:
	case GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON:
	case GameMode::GAMEMODE_CHALLENGE_STORMY_NIGHT_2:
	case GameMode::GAMEMODE_CHALLENGE_WE_SMASH:
		mBackground = BackgroundType::BACKGROUND_6_BOSS;
		break;

	case GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM:
		mBackground = BackgroundType::BACKGROUND_ZOMBIQUARIUM;
		break;

	case GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN:
		mBackground = BackgroundType::BACKGROUND_GREENHOUSE;
		break;

	case GameMode::GAMEMODE_TREE_OF_WISDOM:
		mBackground = BackgroundType::BACKGROUND_TREEOFWISDOM;
		break;

	case GameMode::GAMEMODE_CHALLENGE_POOL_PARTY:
		mBackground = BackgroundType::BACKGROUND_3_POOLWIDE;
		break;

	case GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2:
	case GameMode::GAMEMODE_CHALLENGE_SPEEDY_LANES:
		mBackground = BackgroundType::BACKGROUND_3_GRASS;
		break;

	case GameMode::GAMEMODE_CHALLENGE_HIGHGROUND:
		mBackground = BackgroundType::BACKGROUND_HIGHGROUND;
		break;

	case GameMode::GAMEMODE_CHALLENGE_UNSODDED_2:
		mBackground = BackgroundType::BACKGROUND_3_UNSODDED;
		break;

	case GameMode::GAMEMODE_CHALLENGE_AIR_RAID:
		mBackground = BackgroundType::BACKGROUND_6_AIRRAID;
		break;

	case GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER:
		mBackground = BackgroundType::BACKGROUND_GRAVEYARD;
		break;
	default:
		//TOD_ASSERT();
		break;
	}
	LoadBackgroundImages();

	if (mBackground == BackgroundType::BACKGROUND_1_DAY || mBackground == BackgroundType::BACKGROUND_GREENHOUSE || mBackground == BackgroundType::BACKGROUND_TREEOFWISDOM)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;

		if (mApp->IsAdventureMode() && mApp->IsFirstTimeAdventureMode())
		{
			if (mLevel == 1)
			{
				mPlantRow[0] = PlantRowType::PLANTROW_DIRT;
				mPlantRow[1] = PlantRowType::PLANTROW_DIRT;
				mPlantRow[3] = PlantRowType::PLANTROW_DIRT;
				mPlantRow[4] = PlantRowType::PLANTROW_DIRT;
			}
			else if (mLevel == 2 || mLevel == 3)
			{
				mPlantRow[0] = PlantRowType::PLANTROW_DIRT;
				mPlantRow[4] = PlantRowType::PLANTROW_DIRT;
			}
		}
		else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED)
		{
			mPlantRow[0] = PlantRowType::PLANTROW_DIRT;
			mPlantRow[4] = PlantRowType::PLANTROW_DIRT;
		}
	}
	else if (mBackground == BackgroundType::BACKGROUND_2_NIGHT)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
	}
	else if (mBackground == BackgroundType::BACKGROUND_3_POOL || mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM || mBackground == BackgroundType::BACKGROUND_4_FOG)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_POOL;
		mPlantRow[3] = PlantRowType::PLANTROW_POOL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
	}
	else if (mBackground == BackgroundType::BACKGROUND_3_UNSODDED)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[1] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[2] = PlantRowType::PLANTROW_POOL;
		mPlantRow[3] = PlantRowType::PLANTROW_POOL;
		mPlantRow[4] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
	}
	else if (mBackground == BackgroundType::BACKGROUND_3_POOLWIDE)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_POOL;
		mPlantRow[1] = PlantRowType::PLANTROW_POOL;
		mPlantRow[2] = PlantRowType::PLANTROW_POOL;
		mPlantRow[3] = PlantRowType::PLANTROW_POOL;
		mPlantRow[4] = PlantRowType::PLANTROW_POOL;
		mPlantRow[5] = PlantRowType::PLANTROW_POOL;
	}
	else if (mBackground == BackgroundType::BACKGROUND_3_GRASS)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
	}
	else if (mBackground == BackgroundType::BACKGROUND_HIGHGROUND)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
	}
	else if (mBackground == BackgroundType::BACKGROUND_5_ROOF || mBackground == BackgroundType::BACKGROUND_6_BOSS)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
	}
	else if (mBackground == BackgroundType::BACKGROUND_GRAVEYARD)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
	}
	else if (mBackground == BackgroundType::BACKGROUND_6_AIRRAID)
	{
		mPlantRow[0] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[1] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[2] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[3] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[4] = PlantRowType::PLANTROW_DIRT;
		mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
	}
	else
	{
		TOD_ASSERT();
	}

	for (int x = 0; x < MAX_GRID_SIZE_X; x++)
	{
		for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
		{
			if (mPlantRow[y] == PlantRowType::PLANTROW_DIRT)
			{
				mGridSquareType[x][y] = GridSquareType::GRIDSQUARE_DIRT;
			}
			else if (mPlantRow[y] == PlantRowType::PLANTROW_POOL && x >= 0 && x <= 8)
			{
				mGridSquareType[x][y] = GridSquareType::GRIDSQUARE_POOL;
			}
			else if (mPlantRow[y] == PlantRowType::PLANTROW_HIGH_GROUND && x >= 4 && x <= 8)
			{
				mGridSquareType[x][y] = GridSquareType::GRIDSQUARE_HIGH_GROUND;
			}
		}
	}

	if (mBackground == BackgroundType::BACKGROUND_GRAVEYARD)
	{
		for (int x = 5; x < MAX_GRID_SIZE_X; x++)
		{
			for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
			{
				mGridSquareType[x][y] = GridSquareType::GRIDSQUARE_DIRT;
			}
		}
	}
	else if (mBackground == BackgroundType::BACKGROUND_HIGHGROUND)
	{
		for (int x = 4; x < MAX_GRID_SIZE_X; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				mGridSquareType[x][y] = GridSquareType::GRIDSQUARE_HIGH_GROUND;
			}
		}
	}

	MTRand aLevelRNG(GetLevelRandSeed());
	if (StageHasGraveStones() || mApp->IsWhackAZombieLevel())
	{
		if (mBackground == BACKGROUND_GRAVEYARD)
		{
			AddGraveStones(6, RandRangeInt(1, 2), aLevelRNG);
			AddGraveStones(7, RandRangeInt(1, 3), aLevelRNG);
			AddGraveStones(8, RandRangeInt(2, 3), aLevelRNG);
		}
		else if (mApp->IsWhackAZombieLevel())
		{
			mChallenge->WhackAZombiePlaceGraves(9);
		}
		else if (mBackground == BackgroundType::BACKGROUND_2_NIGHT)
		{
			if (mApp->IsSurvivalNormal(mApp->mGameMode))
			{
				AddGraveStones(5, 1, aLevelRNG);
				AddGraveStones(6, 1, aLevelRNG);
				AddGraveStones(7, 1, aLevelRNG);
				AddGraveStones(8, 2, aLevelRNG);
			}
			else if (!mApp->IsAdventureMode())
			{
				AddGraveStones(4, 1, aLevelRNG);
				AddGraveStones(5, 1, aLevelRNG);
				AddGraveStones(6, 2, aLevelRNG);
				AddGraveStones(7, 2, aLevelRNG);
				AddGraveStones(8, 3, aLevelRNG);
			}
			else if (mLevel == 11 || mLevel == 12 || mLevel == 13)
			{
				AddGraveStones(6, 1, aLevelRNG);
				AddGraveStones(7, 1, aLevelRNG);
				AddGraveStones(8, 2, aLevelRNG);
			}
			else if (mLevel == 14 || mLevel == 16)
			{
				AddGraveStones(5, 1, aLevelRNG);
				AddGraveStones(6, 1, aLevelRNG);
				AddGraveStones(7, 2, aLevelRNG);
				AddGraveStones(8, 3, aLevelRNG);
			}
			else if (mLevel == 17 || mLevel == 18 || mLevel == 19)
			{
				AddGraveStones(4, 1, aLevelRNG);
				AddGraveStones(5, 2, aLevelRNG);
				AddGraveStones(6, 2, aLevelRNG);
				AddGraveStones(7, 3, aLevelRNG);
				AddGraveStones(8, 3, aLevelRNG);
			}
			else if (mLevel >= 20)
			{
				AddGraveStones(3, 1, aLevelRNG);
				AddGraveStones(4, 2, aLevelRNG);
				AddGraveStones(5, 2, aLevelRNG);
				AddGraveStones(6, 2, aLevelRNG);
				AddGraveStones(7, 3, aLevelRNG);
				AddGraveStones(8, 3, aLevelRNG);
			}
			else
			{
				//TOD_ASSERT();
			}
		}
	}

	PickSpecialGraveStone();

	switch (mBackground) {
	case BACKGROUND_1_DAY:
	case BACKGROUND_2_NIGHT:
	case BACKGROUND_3_POOL:
	case BACKGROUND_3_POOLWIDE:
	case BACKGROUND_3_UNSODDED:
	case BACKGROUND_3_GRASS:
	case BACKGROUND_4_FOG:
	case BACKGROUND_GRAVEYARD:
	case BACKGROUND_HIGHGROUND:
	{
		InitBushes();
		break;
	}
	}
}

//0x40AB10
void Board::InitZombieWavesForLevel(int theForLevel)
{
	if (mApp->IsWhackAZombieLevel() || (mApp->IsWallnutBowlingLevel() && !mApp->IsFirstTimeAdventureMode()))
	{
		mChallenge->InitZombieWaves();
		return;
	}

	for (int aZombieType = ZombieType::ZOMBIE_NORMAL; aZombieType < ZombieType::NUM_ZOMBIE_TYPES; aZombieType++)
	{
		mZombieAllowed[aZombieType] = CanZombieSpawnOnLevel((ZombieType)aZombieType, theForLevel);
	}
}

bool Board::IsZombieWaveDistributionOk()
{
	if (!mApp->IsAdventureMode())
		return true;

	int aZombieTypeCount[(int)ZombieType::NUM_ZOMBIE_TYPES] = { 0 };
	for (int aWave = 0; aWave < mNumWaves; aWave++)
	{
		for (int aIndex = 0; aIndex < MAX_ZOMBIES_IN_WAVE; aIndex++)
		{
			ZombieType aZombieType = mZombiesInWave[aWave][aIndex];
			if (aZombieType == ZombieType::ZOMBIE_INVALID)
			{
				break;
			}

			TOD_ASSERT(aZombieType >= 0 && aZombieType < ZombieType::NUM_ZOMBIE_TYPES);
			aZombieTypeCount[(int)aZombieType]++;
		}
	}

	for (ZombieType aZombieType = ZombieType::ZOMBIE_NORMAL; aZombieType < ZombieType::NUM_ZOMBIE_TYPES; aZombieType = (ZombieType)((int)aZombieType + 1))
	{
		if (aZombieType != ZombieType::ZOMBIE_YETI && CanZombieSpawnOnLevel(aZombieType, mLevel) && aZombieTypeCount[(int)aZombieType] == 0)
		{
			TodTraceAndLog("Didn't spawn required zombie %s, level %d", SexyStringToStringFast(GetZombieDefinition(aZombieType).mZombieName), mLevel);
			return false;
		}
	}
	return true;
}

//0x40ABB0
void Board::InitZombieWaves()
{
	mIsVeteran = mApp->mVeteranMode;

	memset(mZombieAllowed, false, sizeof(mZombieAllowed));
	if (mApp->IsAdventureMode())
	{
		InitZombieWavesForLevel(mLevel);
	}
	else
	{
		mChallenge->InitZombieWaves();
	}
	PickZombieWaves();
	//TOD_ASSERT(IsZombieWaveDistributionOk());

	mCurrentWave = 0;
	mVeteranCounter = 0;
	mTotalSpawnedWaves = 0;
	mApp->mSawYeti = false;
	if (mApp->IsFirstTimeAdventureMode() && mLevel == 2)
	{
		mZombieCountDown = ZOMBIE_COUNTDOWN * 2;
	}
	else if (mApp->IsSurvivalMode() && mChallenge->mSurvivalStage > 0)
	{
		mZombieCountDown = ZOMBIE_COUNTDOWN_RANGE;
	}
	else
	{
		mZombieCountDown = ZOMBIE_COUNTDOWN_FIRST_WAVE;

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL) {
			mZombieCountDown += 500;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_WE_SMASH) {
			mZombieCountDown *= 4;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_METAL_FESTIVAL) {
			mZombieCountDown *= 2;
		}
	}

	/*if (mIsVeteran)
	{
		mZombieCountDown /= GetVeteranMultiplier();
	}*/

	mZombieHealthWaveStart = 0;
	mLastBungeeWave = 0;
	mProgressMeterWidth = 0;
	mHugeWaveCountDown = 0;
	mLevelAwardSpawned = false;
	mZombieCountDownStart = mZombieCountDown;
	mZombieHealthToNextWave = -1;
}

//0x40ACB0
void Board::FreezeEffectsForCutscene(bool theFreeze)
{
	TodParticleSystem* aParticle = nullptr;
	while (IterateParticles(aParticle))
	{
		if (aParticle->mEffectType == ParticleEffect::PARTICLE_GRAVE_BUSTER)
		{
			if (aParticle->mEffectType == ParticleEffect::PARTICLE_POOL_SPARKLY && mIceTrapCounter == 0)
			{
				aParticle->mDontUpdate = theFreeze;
			}
		}
	}

	Reanimation* aReanim = nullptr;
	while (IterateReanimations(aReanim))
	{
		if (aReanim->mReanimationType == ReanimationType::REANIM_SLEEPING)
		{
			aReanim->mAnimRate = theFreeze ? 0.0f : RandRangeFloat(6, 8);
		}
	}
}

//0x40AD60
void Board::InitSurvivalStage()
{
	RefreshSeedPacketFromCursor();
	mApp->mSoundSystem->GamePause(true);
	FreezeEffectsForCutscene(true);
	mLevelComplete = false;
	InitZombieWaves();
	mApp->mGameScene = GameScenes::SCENE_LEVEL_INTRO;
	mApp->ShowSeedChooserScreen();
	mCutScene->StartLevelIntro();
	mSeedBank->UpdateWidth();
	
	for (int i = 0; i < SEEDBANK_MAX; i++)
	{
		SeedPacket* aPacket = &mSeedBank->mSeedPackets[i];
		aPacket->mX = GetSeedPacketPositionX(i);
		aPacket->mPacketType = SeedType::SEED_NONE;
	}

	mSeedBankP2->UpdateWidth();

	for (int i = 0; i < SEEDBANK_MAX; i++)
	{
		SeedPacket* aPacket = &mSeedBankP2->mSeedPackets[i];
		aPacket->mX = GetSeedPacketPositionX(i);
		aPacket->mPacketType = SeedType::SEED_NONE;
		aPacket->misP2 = true;
	}

	if (StageHasFog())
	{
		mFogBlownCountDown = FOG_BLOW_RETURN_TIME;
	}
	for (int j = 0; j < MAX_GRID_SIZE_Y; j++)
	{
		mWaveRowGotLawnMowered[j] = -100;
	}
}

//0x40AE70
Rect Board::GetShovelButtonRect()
{
	float bankWidth = mSeedBank->mWidth;
	/*if (mIsCOOP && HasConveyorBeltSeedBank())
		bankWidth = 334.0f - 10;*/

	Rect aRect(mSeedBank->mX + mSeedBank->mWidth + 12 + 79, -BOARD_OFFSET_Y, 70, Sexy::IMAGE_SEEDBANK->GetHeight());
	if (mApp->IsSlotMachineLevel() || mApp->IsSquirrelLevel())
	{
		aRect.mX = 600 - BOARD_ADDITIONAL_WIDTH;
	}

	if (mApp->IsScaryPotterLevel() || mApp->IsWhackAZombieLevel())
	{
		if (mSeedBank->mY < 0)
			aRect.mX = TodAnimateCurveFloatTime(-IMAGE_SEEDBANK->GetHeight() - BOARD_OFFSET_Y, 0, mSeedBank->mY + IMAGE_SEEDBANK->GetHeight(), mSeedBank->mX + 79, mSeedBank->mX + 79 + 91 + mSeedBank->mWidth, CURVE_LINEAR);
		else
			aRect.mX = mSeedBank->mX + 79 + 91 + mSeedBank->mWidth;

		aRect.mX -= BOARD_ADDITIONAL_WIDTH;
	}
	else if (mApp->IsChallengeWithoutSeedBank())
	{
		aRect.mX = -BOARD_ADDITIONAL_WIDTH;
	}

	return aRect;
}

//0x40AF00
void Board::GetZenButtonRect(GameObjectType theObjectType, Rect& theRect)
{
	// 此函数与内测版的差异在于，内测版在此函数中通过下列语句先取得了铲子按钮矩形：
	// Rect aRect = GetShovelButtonRect();
	// 而原版需要在函数调用前先自行取得铲子按钮矩形，并将该矩形作为参数传递给此函数，
	// 原版中此函数有将 theRect 的引用作为返回值，但并无直接使用返回值的情况。
	// 此处为了防止误用返回值而出现问题，故删除其返回值，如需调用可按照如下方式：
	// Rect aButtonRect = GetShovelButtonRect();
	// GetZenButtonRect(xxx, aButtonRect);

	theRect.mX = -BOARD_ADDITIONAL_WIDTH;
	bool usable = true;
	for (int anObject = GameObjectType::OBJECT_TYPE_WATERING_CAN; anObject <= GameObjectType::OBJECT_TYPE_NEXT_GARDEN; anObject++)
	{
		// 只要有一个按钮不可用，则所有可用按钮排列的起始横坐标为 30
		if (!CanUseGameObject((GameObjectType)anObject))
		{
			usable = false;
		}
	}
	if (usable)
	{
		theRect.mX = 0;
	}

	for (int anObject = GameObjectType::OBJECT_TYPE_WATERING_CAN; anObject < theObjectType; anObject++)
	{
		// 每存在一个序号小于目标的可用按钮，则目标按钮的横坐标增加 70
		if (CanUseGameObject((GameObjectType)anObject))
		{
			theRect.mX += 70;//Sexy::IMAGE_SHOVELBANK->GetWidth();
		}
	}
	//return theRect;
}

//0x40AF90
void Board::InitLevel()
{
	mMainCounter = 0;
	mEnableGraveStones = false;
	mSodPosition = 0;
	mPrevBoardResult = mApp->mBoardResult;
	
	GameMode aGameMode = mApp->mGameMode;
	if (aGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM && aGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		mApp->mMusic->StopAllMusic();
	}

	// 赋值当前关卡
	mLevel = mApp->IsAdventureMode() ? mApp->mPlayerInfo->mLevel : 0;
	// 设定关卡背景
	PickBackground();
	// 设定关卡出怪
	InitZombieWaves();
	// 设定关卡初始阳光数量
	if (aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST ||
		mApp->IsScaryPotterLevel() || mApp->IsWhackAZombieLevel())
	{
		mSunMoney = 0;
	}
	else if (aGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
	{
		mSunMoney = 2000;
	}
	else if (aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_DAY || aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_NIGHT)
	{
		mSunMoney = 3000;
	}
	else if (aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_POOL || aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_FOG)
	{
		mSunMoney = 4000;
	}
	else if (aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_ROOF || aGameMode == GAMEMODE_CHALLENGE_LAST_STAND_GLOOM)
	{
		mSunMoney = 5000;
	}
	else if (mApp->IsIZombieLevel())
	{
		mSunMoney = 150;
	}
	else if (mApp->IsFirstTimeAdventureMode() && mLevel == 1)
	{
		mSunMoney = 150;
	}
	else if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
		mSunMoneyP2 = mSunMoney = 150;
	}
	else
	{
		mSunMoney = 50;
		////if (StageHasRoof() && StageIsNight())
		////	mSunMoney += 75;
	}

	/*if (mIsCOOP)
	{
		mSunMoney /= 2;
		mSunMoneyP2 = mSunMoney;
	}*/

	// 初始化行选择数组
	memset(mRowPickingArray, 0, sizeof(mRowPickingArray));
	// 初始化每行的基础数据
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		mWaveRowGotLawnMowered[aRow] = -100;
		mIceMinX[aRow] = BOARD_ICE_START;
		mIceTimer[aRow] = 0;
		mIceParticleID[aRow] = ParticleSystemID::PARTICLESYSTEMID_NULL;
		mRowPickingArray[aRow].mItem = aRow;
	}
	// 初始化阳光掉落
	mNumSunsFallen = 0;
	if (!StageIsNight())
	{
		mSunCountDown = RandRangeInt(425, 700);
	}
	// 初始化字幕播放记录
	memset(mHelpDisplayed, 0, sizeof(mHelpDisplayed));
	// 初始化卡槽及卡牌
	mSeedBank->mNumPackets = GetNumSeedsInBank();
	mSeedBank->UpdateWidth();

	mSeedBankP2->mNumPackets = GetNumSeedsInBank();
	mSeedBankP2->UpdateWidth();

	for (int i = 0; i < SEEDBANK_MAX; i++)
	{
		SeedPacket* aPacket = &mSeedBank->mSeedPackets[i];
		aPacket->mIndex = i;
		aPacket->mX = GetSeedPacketPositionX(i);
		aPacket->mY = 8;
		aPacket->mPacketType = SeedType::SEED_NONE;
	}

	for (int i = 0; i < SEEDBANK_MAX; i++)
	{
		SeedPacket* aPacket = &mSeedBankP2->mSeedPackets[i];
		aPacket->mIndex = i;
		aPacket->mX = GetSeedPacketPositionX(i);
		aPacket->mY = 8;
		aPacket->mPacketType = SeedType::SEED_NONE;
		aPacket->misP2 = true;
	}
	
	// 设定固定卡牌
	if (mApp->IsSlotMachineLevel())
	{
		//TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_PEASHOOTER);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_SNOWPEA);
		
		if (mApp->mGameMode == GAMEMODE_CHALLENGE_SLOT_MACHINE_2)
		{
			mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_CABBAGEPULT);
			mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_KERNELPULT);
			mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_FLOWERPOT);
		}
	}
	else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 6);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_PEASHOOTER);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_CHERRYBOMB);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_WALLNUT);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_REPEATER);
		mSeedBank->mSeedPackets[4].SetPacketType(SeedType::SEED_SNOWPEA);
		mSeedBank->mSeedPackets[5].SetPacketType(SeedType::SEED_CHOMPER);
	}
	else if (aGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 2);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIQUARIUM_SNORKLE);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIQUARIUM_TROPHY);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_SCREEN_DOOR);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_DIGGER);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_LADDER);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 4);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_BUNGEE);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_BALLOON);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 4);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_POLEVAULTER);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_GARGANTUAR);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 4);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_NORMAL);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_POLEVAULTER);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_DANCER);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 6);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_IMP);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_TRAFFIC_CONE);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_BUNGEE);
		mSeedBank->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_DIGGER);
		mSeedBank->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_LADDER);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 8);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_IMP);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_TRAFFIC_CONE);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_POLEVAULTER);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_BUNGEE);
		mSeedBank->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_DIGGER);
		mSeedBank->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_LADDER);
		mSeedBank->mSeedPackets[7].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL);
	}
	else if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS)
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 9);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_IMP);
		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_ZOMBIE_TRAFFIC_CONE);
		mSeedBank->mSeedPackets[2].SetPacketType(SeedType::SEED_ZOMBIE_POLEVAULTER);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_ZOMBIE_PAIL);
		mSeedBank->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_BUNGEE);
		mSeedBank->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_DIGGER);
		mSeedBank->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_LADDER);
		mSeedBank->mSeedPackets[7].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL);
		mSeedBank->mSeedPackets[8].SetPacketType(SeedType::SEED_ZOMBIE_DANCER);
	}
	else if (mApp->IsScaryPotterLevel())
	{
		TOD_ASSERT(mSeedBank->mNumPackets == 1);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_CHERRYBOMB);
	}
	else if (mApp->IsWhackAZombieLevel())
	{
		//TOD_ASSERT(mSeedBank->mNumPackets == 3);
		mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_POTATOMINE);

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2)
			mSeedBank->mSeedPackets[0].SetPacketType(SeedType::SEED_FLOWERPOT);

		mSeedBank->mSeedPackets[1].SetPacketType(SeedType::SEED_GRAVEBUSTER);
		mSeedBank->mSeedPackets[2].SetPacketType(mApp->IsAdventureMode() ? SeedType::SEED_CHERRYBOMB : SeedType::SEED_ICESHROOM);
		mSeedBank->mSeedPackets[3].SetPacketType(SeedType::SEED_UMBRELLA);
	}
	else if (!ChooseSeedsOnCurrentLevel() && !HasConveyorBeltSeedBank())
	{
		mSeedBank->mNumPackets = GetNumSeedsInBank();
		mSeedBankP2->mNumPackets = GetNumSeedsInBank();

		/*if (mIsCOOP)
		{
			mSeedBank->mNumPackets = floor(mSeedBank->mNumPackets / 2);
			mSeedBank->mNumPackets = min(4, mSeedBank->mNumPackets);
			mSeedBankP2->mNumPackets = mSeedBank->mNumPackets;
		}*/

		// 卡槽错误的关卡，依次填充所有卡牌
		for (int i = 0; i < mSeedBank->mNumPackets; i++)
		{
			mSeedBank->mSeedPackets[i].SetPacketType((SeedType)i);
			mSeedBankP2->mSeedPackets[i].SetPacketType((SeedType)i);
		}
	}
	// 将所有子控件标记为已变动
	MarkAllDirty();
	
	mPaused = false;
	mOutOfMoneyCounter = 0;
	if (StageHasFog())
	{
		mFogBlownCountDown = 200;
		mFogOffset = 1065 - LeftFogColumn() * 80;
		if (StageHasRoof())
			mFogOffset -= BOARD_ADDITIONAL_WIDTH;
	}
	// 关卡玩法相关的初始化
	mChallenge->InitLevel();
}

Reanimation* Board::CreateRakeReanim(float theRakeX, float theRakeY, int theRenderOrder)
{
	Reanimation* aReanim = mApp->AddReanimation(GridToPixelX(theRakeX, theRakeY) + 20, GridToPixelY(theRakeX, theRakeY), theRenderOrder, REANIM_RAKE);
	aReanim->mAnimRate = 0;
	aReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
	aReanim->mIsAttachment = true;
	return aReanim;
}

//0x40B9C0
void Board::PlaceRake()
{
	if (!mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_RAKE])
		return;

	int aGridX = 7;
	if (mApp->IsScaryPotterLevel())
	{
		GridItem* aGridItem = nullptr;
		while (IterateGridItems(aGridItem))
		{
			if (aGridItem->mGridItemType == GridItemType::GRIDITEM_SCARY_POT && aGridItem->mGridX <= aGridX && aGridItem->mGridX > 0)
			{
				aGridX = aGridItem->mGridX - 1;
			}
		}
	}
	else
	{
		if (!StageHasZombieWalkInFromRight() || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED ||
			mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA)
			return;
	}

	int aPickCount = 0;
	TodWeightedArray aPickArray[MAX_GRID_SIZE_Y];
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		if (aRow != 5 && mPlantRow[aRow] == PlantRowType::PLANTROW_NORMAL)
		{
			aPickArray[aPickCount].mWeight = 1;
			aPickArray[aPickCount].mItem = aRow;
			aPickCount++;
		}
	}
	if (aPickCount == 0)
		return;

	int aGridY = TodPickFromWeightedArray(aPickArray, aPickCount);
	mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_RAKE]--;
	GridItem* aRake = mGridItems.DataArrayAlloc();
	aRake->mGridItemType = GridItemType::GRIDITEM_RAKE;
	aRake->mGridX = aGridX;
	aRake->mGridY = aGridY;
	aRake->mPosX = GridToPixelX(aGridX, aGridY);
	aRake->mPosY = GridToPixelY(aGridX, aGridY);
	aRake->mRenderOrder = MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, aGridY, 9);
	aRake->mGridItemReanimID = mApp->ReanimationGetID(CreateRakeReanim(aGridX, aGridY, 0));
	aRake->mGridItemState = GridItemState::GRIDITEM_STATE_RAKE_ATTRACTING;
}

//0x40BC70
void Board::InitLawnMowers()
{
	GameMode aGameMode = mApp->mGameMode;
	// 这里优化一下原版的代码，事先列举一些不创建小推车的关卡
	if (aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST ||
		aGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || aGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || aGameMode == GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE ||
		aGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN || LawnApp::IsLastStandLevel(aGameMode) || aGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM ||
		aGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID || mApp->IsSquirrelLevel() || mApp->IsIZombieLevel() || (StageHasRoof() && !mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_ROOF_CLEANER])
		|| aGameMode == GAMEMODE_CHALLENGE_BOMB_ALL || aGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS || mIsVeteran)
		return;

	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		if ((aGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED && aRow <= 4) || 
			(aGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED_2 && aRow <= 5) || 
			(mApp->IsAdventureMode() && mLevel == 35 && aRow <= 5) ||   // 这里原版没有对于行的判断，故冒险模式 4-5 关卡有 6 行小推车
			(!mApp->IsScaryPotterLevel() && mPlantRow[aRow] != PlantRowType::PLANTROW_DIRT))  // 除冒险模式 4-5 关卡外的破罐者模式关卡无小推车
		{
			LawnMower* aLawnMower = mLawnMowers.DataArrayAlloc();
			aLawnMower->LawnMowerInitialize(aRow);
			aLawnMower->mVisible = false;
		}
	}
}

void Board::InitBushes() {
	bool nighty = StageIsNight();
	for (int i = 0; i < 6; i++) {
		Bush* aBush = mBushList[i];
		BushCheap* aBushCheap = mBushCheapList[i];
		float posX = 0, posY = 0;

		switch (i) {
		case 0:
			posX = 710;
			posY = -20;
			break;
		case 1:
			posX = 720;
			posY = 110;
			break;
		case 2:
			posX = 730;
			posY = 200;
			break;
		case 3:
			posX = 735;
			posY = 320;
			break;
		case 4:
			posX = 730;
			posY = 400;
			break;
		case 5:
			posX = 740;
			posY = 450;
			break;
		}
		
		if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		{
			posX += 85;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
		{
			posX += BOARD_ADDITIONAL_WIDTH;
		}

		if (mBackground == BACKGROUND_HIGHGROUND)
		{
			if (i < 5)
				posY -= HIGH_GROUND_HEIGHT;
		}

		aBush->BushInitialize(posX,posY , i, nighty, i);
		aBushCheap->BushCheapInitialize(posX, posY, i, nighty, i);
	}
}

//0x40BD30
bool Board::ChooseSeedsOnCurrentLevel()
{
	if (mApp->IsChallengeWithoutSeedBank() || HasConveyorBeltSeedBank())
		return false;

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID)
		return false;

	if (mApp->IsIZombieLevel() || mApp->IsSlotMachineLevel())
		return false;

	return (!mApp->IsFirstTimeAdventureMode() || mLevel > 7);
}

//0x40BE00
void Board::StartLevel()
{
	mCoinBankFadeCount = 0;
	mApp->mLastLevelStats->Reset();
	mChallenge->StartLevel();

	if (mApp->IsSurvivalMode() && mChallenge->mSurvivalStage > 0)
	{
		mApp->EraseFile(GetSavedGameName(mApp->mGameMode, mApp->mPlayerInfo->mId));
		FreezeEffectsForCutscene(false);
		mApp->mSoundSystem->GamePause(false);
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || 
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM ||
		mApp->mGameMode == GameMode::GAMEMODE_UPSELL || 
		mApp->mGameMode == GameMode::GAMEMODE_INTRO || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID ||
		mApp->IsFinalBossLevel())
		return;

	mApp->mMusic->StartGameMusic();

	if (mApp->mGameScene == SCENE_PLAYING)
	{
		if (mApp->mGameMode != GameMode::GAMEMODE_ADVENTURE) {
			mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName), "Playing");
		}
		else
		{
			int aStage = ClampInt((mLevel - 1) / 10 + 1, 1, 6);
			int aSub = mLevel - (aStage - 1) * 10;
			std::string Details = "Adventure " + std::to_string(aStage) + "-" + std::to_string(aSub);
			const char* DetailsChar = Details.c_str();
			mApp->UpdateDiscordRPC(DetailsChar, "Playing");
		}
	}
}

//0x40BF10
LawnMower* Board::GetBottomLawnMower()
{
	LawnMower* aLawnMower = nullptr;
	LawnMower* aBottomMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		if (aLawnMower->mMowerState == LawnMowerState::MOWER_TRIGGERED || aLawnMower->mMowerState == LawnMowerState::MOWER_SQUISHED)
			continue;

		if (aBottomMower == nullptr || aBottomMower->mRow < aLawnMower->mRow)
		{
			aBottomMower = aLawnMower;
		}
	}
	return aBottomMower;
}

//0x40BF60
void Board::UpdateLevelEndSequence()
{
	if (mNextSurvivalStageCounter > 0)
	{
		if (!IsScaryPotterDaveTalking())
		{
			mNextSurvivalStageCounter--;
			if (mApp->IsAdventureMode() && mApp->IsScaryPotterLevel() && mNextSurvivalStageCounter == 300)
			{
				mApp->CrazyDaveEnter();
				mApp->CrazyDaveTalkIndex(mChallenge->mSurvivalStage == 0 ? 2700 : 2800);
				mChallenge->PuzzleNextStageClear();
				mNextSurvivalStageCounter = 100;
			}
		}

		if (mNextSurvivalStageCounter == 1 && mApp->IsSurvivalMode())
		{
			TryToSaveGame();
		}

		if (!mNextSurvivalStageCounter)
		{
			if (mApp->IsScaryPotterLevel())
			{
				if (mApp->IsAdventureMode())
					return;

				if (!IsFinalScaryPotterStage())
				{
					mChallenge->PuzzleNextStageClear();
					mChallenge->ScaryPotterPopulate();
				}
			}
			else if (LawnApp::IsEndlessIZombie(mApp->mGameMode))
			{
				mChallenge->PuzzleNextStageClear();
				mChallenge->IZombieInitLevel();
			}
			else if (LawnApp::IsLastStandLevel(mApp->mGameMode))
			{
				ClearAdvice(AdviceType::ADVICE_NONE);
			}
			else
			{
				mLevelComplete = true;
				RemoveZombiesForRepick();
			}
			return;
		}
	}

	if (mBoardFadeOutCounter < 0)
		return;

	mBoardFadeOutCounter--;

	if (mBoardFadeOutCounter == 0)
	{
		mLevelComplete = true;
		return;
	}
	if (mBoardFadeOutCounter == 300)
	{
		if (!IsSurvivalStageWithRepick() && !(mLevel == 9 || mLevel == 19 || mLevel == 29 || mLevel == 39 || mLevel == 49))
		{
			mApp->PlaySample(Sexy::SOUND_LIGHTFILL);
		}
	}

	if (mScoreNextMowerCounter > 0)
	{
		mScoreNextMowerCounter--;
		if (mScoreNextMowerCounter)
		{
			return;
		}
	}

	if (CanDropLoot() && !IsSurvivalStageWithRepick())
	{
		mScoreNextMowerCounter = 40;

		LawnMower* aLawnMower = GetBottomLawnMower();
		if (aLawnMower)
		{
			AddCoin(aLawnMower->mPosX + 40, aLawnMower->mPosY + 40, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_LAWNMOWER_COIN);
			
			mApp->PlayFoley(FoleyType::FOLEY_COIN);
			SoundInstance* aSoundInstance = mApp->mSoundManager->GetSoundInstance(Sexy::SOUND_POINTS);
			if (aSoundInstance)
			{
				aSoundInstance->Play(false, true);
				float aPitch = ClampFloat(6 - CountUntriggerLawnMowers(), 0.0f, 6.0f);
				aSoundInstance->AdjustPitch(aPitch);
			}
			aLawnMower->Die();
		}
	}
}

//0x40C2C0
void Board::CompleteEndLevelSequenceForSaving()
{
	if (CanDropLoot())
	{
		LawnMower* aLawnMower = nullptr;
		while (IterateLawnMowers(aLawnMower))
		{
			if (aLawnMower->mMowerState != LawnMowerState::MOWER_TRIGGERED && aLawnMower->mMowerState != LawnMowerState::MOWER_SQUISHED)
			{
				int aCoinValue = Coin::GetCoinValue(CoinType::COIN_GOLD);
				mApp->mPlayerInfo->AddCoins(aCoinValue);
				mCoinsCollected += aCoinValue;
			}
		}
	}

	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		if (aCoin->mIsBeingCollected)
		{
			aCoin->ScoreCoin();
		}
		else
		{
			aCoin->Die();
		}
	}
	
	mApp->UpdatePlayerProfileForFinishingLevel();
}

//0x40C3E0
void Board::FadeOutLevel()
{
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING)
	{
		RefreshSeedPacketFromCursor();
		mApp->mLastLevelStats->Reset();
		mLevelComplete = true;
	}

	bool aNeedSoundEffect = true;
	if (mApp->IsScaryPotterLevel() && !IsFinalScaryPotterStage())
	{
		aNeedSoundEffect = false;
	}
	else if (IsSurvivalStageWithRepick() || IsLastStandStageWithRepick() || mApp->IsEndlessIZombie(mApp->mGameMode))
	{
		aNeedSoundEffect = false;
	}
	if (aNeedSoundEffect)
	{
		mApp->mMusic->StopAllMusic();
		if (mIsVeteran && !mApp->HasBeatenChallengeVeteran(mApp->mGameMode))
		{
			mApp->PlayFoley(FoleyType::FOLEY_FINAL_FANFARE);
		}
		else if (mApp->IsAdventureMode() && mLevel == 60)
		{
			mApp->PlayFoley(FoleyType::FOLEY_FINAL_FANFARE);
		}
		else if (mApp->TrophiesNeedForDiamondSunflower() == 1)
		{
			mApp->PlayFoley(FoleyType::FOLEY_FINAL_FANFARE);
		}
		else if (mApp->TrophiesNeedForGoldSunflower() == 1)
		{
			mApp->PlayFoley(FoleyType::FOLEY_FINAL_FANFARE);
		}
		else
		{
			mApp->PlayFoley(FoleyType::FOLEY_WINMUSIC);
		}
	}

	if (mApp->IsScaryPotterLevel() && !IsFinalScaryPotterStage())
	{
		mNextSurvivalStageCounter = 500;
		if (mApp->IsAdventureMode())
		{
			ClearAdvice(AdviceType::ADVICE_NONE);
		}
		else
		{
			mLevelAwardSpawned = true;
			SexyString aStreakStr = mApp->IsEndlessScaryPotter(mApp->mGameMode) ? _S("[ADVICE_MORE_SCARY_POTS]") : _S("[ADVICE_3_IN_A_ROW]");
			SexyString aMessage = TodReplaceNumberString(aStreakStr, _S("{STREAK}"), mChallenge->mSurvivalStage + 1);
			PuzzleSaveStreak();
			ClearAdvice(AdviceType::ADVICE_NONE);
			DisplayAdvice(aMessage, MessageStyle::MESSAGE_STYLE_BIG_MIDDLE, AdviceType::ADVICE_NONE);

			mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName), aMessage);
		}
		return;
	}

	if (mApp->IsEndlessIZombie(mApp->mGameMode))
	{
		mNextSurvivalStageCounter = 500;
		SexyString aMessage = TodReplaceNumberString(_S("[ADVICE_MORE_IZOMBIE]"), _S("{STREAK}"), mChallenge->mSurvivalStage + 1);
		PuzzleSaveStreak();
		ClearAdvice(AdviceType::ADVICE_NONE);
		DisplayAdvice(aMessage, MessageStyle::MESSAGE_STYLE_BIG_MIDDLE, AdviceType::ADVICE_NONE);
		return;
	}

	if (IsLastStandStageWithRepick())
	{
		mNextSurvivalStageCounter = 500;
		mChallenge->LastStandCompletedStage();
		return;
	}

	if (!IsSurvivalStageWithRepick())
	{
		RefreshSeedPacketFromCursor();
		mApp->mLastLevelStats->mUnusedLawnMowers = CountUntriggerLawnMowers();

		mBoardFadeOutCounter = 600;
		if (mLevel == 9 || mLevel == 19 || mLevel == 29 || mLevel == 39 || mLevel == 49)
		{
			mBoardFadeOutCounter = 500;
		}

		if (CanDropLoot())
		{
			mScoreNextMowerCounter = 200;
		}

		Coin* aCoin = nullptr;
		while (IterateCoins(aCoin))
		{
			aCoin->TryAutoCollectAfterLevelAward();
		}
	}
	else
	{
		TOD_ASSERT(mApp->IsSurvivalMode());
		mNextSurvivalStageCounter = 500;
		DisplayAdvice(_S("[ADVICE_MORE_ZOMBIES]"), MessageStyle::MESSAGE_STYLE_BIG_MIDDLE, AdviceType::ADVICE_NONE);
		mApp->mMusic->FadeOut(500);
		mApp->PlaySample(Sexy::SOUND_HUGE_WAVE);
		for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
		{
			mIceTimer[aRow] = mNextSurvivalStageCounter;
		}
	}

	mApp->SetCursor(CURSOR_POINTER);
}

void Board::DisplayAdvice(const SexyString& theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex)
{
	if (theHelpIndex != AdviceType::ADVICE_NONE)
	{
		if (mHelpDisplayed[theHelpIndex])
			return;

		mHelpDisplayed[theHelpIndex] = true;
	}

	mAdvice->SetLabel(theAdvice, theMessageStyle);
	mHelpIndex = theHelpIndex;
}

//0x40CA10
void Board::DisplayAdviceAgain(const SexyString& theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex)
{
	if (theHelpIndex != AdviceType::ADVICE_NONE)
	{
		mHelpDisplayed[theHelpIndex] = false;
	}
	DisplayAdvice(theAdvice, theMessageStyle, theHelpIndex);
}

//0x40CA50
void Board::ClearAdviceImmediately()
{
	ClearAdvice(AdviceType::ADVICE_NONE);
	mAdvice->mDuration = 0;
}

//0x40CAB0
void Board::ClearAdvice(AdviceType theHelpIndex)
{
	if (theHelpIndex == AdviceType::ADVICE_NONE || theHelpIndex == mHelpIndex)
	{
		mAdvice->ClearLabel();
		mHelpIndex = AdviceType::ADVICE_NONE;
	}
}

//0x40CB10
Coin* Board::AddCoin(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion)
{
	Coin* aCoin = mCoins.DataArrayAlloc();
	aCoin->CoinInitialize(theX, theY, theCoinType, theCoinMotion);
	if (mApp->IsFirstTimeAdventureMode() && mLevel == 1)
	{
		DisplayAdvice(_S("[ADVICE_CLICK_ON_SUN]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_CLICK_ON_SUN);
	}
	return aCoin;
}

//0x40CCE0
bool Board::IsPlantInCursor()
{
	return 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW;
}

//0x40CD10
SeedType Board::GetSeedTypeInCursor()
{
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW)
	{
		PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
		if (aPottedPlant)
		{
			return aPottedPlant->mSeedType;
		}
	}

	if (!IsPlantInCursor())
	{
		return SeedType::SEED_NONE;
	}
	return mCursorObject->mType == SeedType::SEED_IMITATER ? mCursorObject->mImitaterType : mCursorObject->mType;
}

//0x40CD80
void Board::RefreshSeedPacketFromCursor()
{
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN)
	{
		mCoins.DataArrayTryToGet(mCursorObject->mCoinID)->DroppedUsableSeed();
	}
	else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK)
	{
		TOD_ASSERT(mCursorObject->mSeedBankIndex >= 0 && mCursorObject->mSeedBankIndex < mSeedBank->mNumPackets);
		mSeedBank->mSeedPackets[mCursorObject->mSeedBankIndex].Activate();
	}
	if (mPlayers[0]->mControlType == CONTROL_MOUSE) {
		mPlayers[0]->mSeedBankIndex = -1;
	}
	ClearCursor();
}

//0x40CE00
bool Board::IsPoolSquare(int theGridX, int theGridY)
{
	if (theGridX >= 0 && theGridY >= 0 && mGridSquareType && mGridSquareType[theGridX][theGridY])
	{
		TOD_ASSERT(theGridX < MAX_GRID_SIZE_X && theGridY < MAX_GRID_SIZE_Y);
		return mGridSquareType[theGridX][theGridY] == GridSquareType::GRIDSQUARE_POOL;
	}
	return false;
}

//0x40CE20
Plant* Board::NewPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
	Plant* aPlant = mPlants.DataArrayAlloc();
	aPlant->mIsOnBoard = true;
	aPlant->PlantInitialize(theGridX, theGridY, theSeedType, theImitaterType);
	return aPlant;
}

Plant* Board::NewPlantEnemy(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
	Plant* aPlant = mPlants.DataArrayAlloc();
	aPlant->mEnemy = true;
	aPlant->mIsOnBoard = true;
	aPlant->PlantInitialize(theGridX, theGridY, theSeedType, theImitaterType);
	return aPlant;
}

//0x40CE60
void Board::DoPlantingEffects(int theGridX, int theGridY, Plant* thePlant)
{
	int aXPos = GridToPixelX(theGridX, theGridY) + 41;
	int aYPos = GridToPixelY(theGridX, theGridY) + 74;
	if (thePlant)
	{
		if (thePlant->mSeedType == SeedType::SEED_LILYPAD)
		{
			aYPos += 15;
		}
		else if (thePlant->mSeedType == SeedType::SEED_FLOWERPOT || thePlant->mSeedType == SeedType::SEED_FLYINGPOT || thePlant->mSeedType == SeedType::SEED_WATERPOT)
		{
			aYPos += 30;
		}

		if (thePlant->mSeedType != SEED_FLOWERPOT && thePlant->mSeedType != SeedType::SEED_FLYINGPOT && thePlant->mSeedType != SeedType::SEED_WATERPOT)
		{
			aYPos += PlantDrawHeightOffset(this, thePlant, thePlant->mSeedType, theGridX, theGridY);
		}
	}
	
	if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE)
	{
		mApp->PlayFoley(FoleyType::FOLEY_CERAMIC);
		return;
	}
	if (mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
	{
		mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);
		return;
	}
	if (Plant::IsFlying(thePlant->mSeedType))
	{
		mApp->PlayFoley(FoleyType::FOLEY_PLANT);
		return;
	}

	if (IsPoolSquare(theGridX, theGridY) && !GetFlyingPotAt(theGridX, theGridY))
	{
		mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);
		mApp->AddTodParticle(aXPos, aYPos, RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_PLANTING_POOL);
	}
	else
	{
		mApp->PlayFoley(FoleyType::FOLEY_PLANT);
		mApp->AddTodParticle(aXPos, aYPos, RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_PLANTING);
	}
}

//0x40D120
Plant* Board::AddPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
	Plant* aPlant = NewPlant(theGridX, theGridY, theSeedType, theImitaterType);
	DoPlantingEffects(theGridX, theGridY, aPlant);
	mChallenge->PlantAdded(aPlant);

	int aSunPlantsCount = CountPlantByType(SeedType::SEED_SUNSHROOM) + CountPlantByType(SeedType::SEED_SUNFLOWER);
	if (aSunPlantsCount > mMaxSunPlants)
	{
		mMaxSunPlants = aSunPlantsCount;  //mMaxSunPlants = max(aSunPlantsCount, mMaxSunPlants);
	}

	if ((theSeedType == SEED_PLANTERN || theSeedType == SEED_PLASMAWOOD || theSeedType == SEED_TORCHWOOD || theSeedType == SEED_FIRESHROOM) && StageHasDarkness())
	{
		mUpdateDarkness = true;
	}

	return aPlant;
}

Plant* Board::AddPlantEnemy(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
	Plant* aPlant = NewPlantEnemy(theGridX, theGridY, theSeedType, theImitaterType);
	DoPlantingEffects(theGridX, theGridY, aPlant);
	mChallenge->PlantAdded(aPlant);

	int aSunPlantsCount = CountPlantByType(SeedType::SEED_SUNSHROOM) + CountPlantByType(SeedType::SEED_SUNFLOWER);
	if (aSunPlantsCount > mMaxSunPlants)
	{
		mMaxSunPlants = aSunPlantsCount;  //mMaxSunPlants = max(aSunPlantsCount, mMaxSunPlants);
	}

	return aPlant;
}

//0x40D1A0
Plant* Board::GetPumpkinAt(int theGridX, int theGridY)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mPlantCol == theGridX && aPlant->mRow == theGridY && !aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
		{
			return aPlant;
		}
	}
	return nullptr;
}

//0x40D220
Plant* Board::GetFlowerPotAt(int theGridX, int theGridY)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mPlantCol == theGridX && aPlant->mRow == theGridY && !aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_FLOWERPOT)
		{
			return aPlant;
		}
	}
	return nullptr;
}

Plant* Board::GetFlyingPotAt(int theGridX, int theGridY)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mPlantCol == theGridX && aPlant->mRow == theGridY && !aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_FLYINGPOT)
		{
			return aPlant;
		}
	}
	return nullptr;
}

Plant* Board::GetWaterPotAt(int theGridX, int theGridY)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mPlantCol == theGridX && aPlant->mRow == theGridY && !aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_WATERPOT)
		{
			return aPlant;
		}
	}
	return nullptr;
}

//0x40D2A0
void Board::GetPlantsOnLawn(int theGridX, int theGridY, PlantsOnLawn* thePlantOnLawn)
{
	thePlantOnLawn->mPotPlant = nullptr;
	thePlantOnLawn->mPadPlant = nullptr;
	thePlantOnLawn->mPumpkinPlant = nullptr;
	thePlantOnLawn->mFlyingPlant = nullptr;
	thePlantOnLawn->mNormalPlant = nullptr;

	if (theGridX < 0 || theGridX >= MAX_GRID_SIZE_X || theGridY < 0 || theGridY >= MAX_GRID_SIZE_Y)
		return;

	if (mApp->IsWallnutBowlingLevel() && !mCutScene->IsInShovelTutorial())
		return;

	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SEED_CHERRYHOVERBOMB || aPlant->mState == STATE_BURNED)
			continue;

		SeedType aSeedType = aPlant->mSeedType;
		if (aSeedType == SeedType::SEED_IMITATER && aPlant->mImitaterType != SeedType::SEED_NONE)
		{
			aSeedType = aPlant->mImitaterType;
		}

		// 检测植物是否位于目标格子内
		if (aPlant->mRow != theGridY)
		{
			continue;
		}
		if (aSeedType == SeedType::SEED_COBCANNON)
		{
			if (aPlant->mPlantCol < theGridX - 1 || aPlant->mPlantCol > theGridX)
			{
				continue;
			}
		}
		else
		{
			if (aPlant->mPlantCol != theGridX)
			{
				continue;
			}
		}
		if (aPlant->NotOnGround())
		{
			continue;
		}

		// 将植物写入 thePlantOnLawn 的记录
		if (Plant::IsFlying(aPlant->mSeedType))
		{
			//TOD_ASSERT(!thePlantOnLawn->mFlyingPlant);
			thePlantOnLawn->mFlyingPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_FLOWERPOT || aSeedType == SeedType::SEED_FLYINGPOT || aSeedType == SeedType::SEED_WATERPOT)
		{
			thePlantOnLawn->mPotPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
		{
			thePlantOnLawn->mPadPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
		{
			TOD_ASSERT(!thePlantOnLawn->mPumpkinPlant);
			thePlantOnLawn->mPumpkinPlant = aPlant;
		}
		else
		{
			thePlantOnLawn->mNormalPlant = aPlant;
		}
	}
}

void Board::GetPlantsOnLawn2(int theGridX, int theGridY, PlantsOnLawn* thePlantOnLawn)
{
	thePlantOnLawn->mPotPlant = nullptr;
	thePlantOnLawn->mPadPlant = nullptr;
	thePlantOnLawn->mPumpkinPlant = nullptr;
	thePlantOnLawn->mFlyingPlant = nullptr;
	thePlantOnLawn->mNormalPlant = nullptr;

	if (theGridX < 0 || theGridX >= MAX_GRID_SIZE_X || theGridY < 0 || theGridY >= MAX_GRID_SIZE_Y)
		return;

	if (mApp->IsWallnutBowlingLevel() && !mCutScene->IsInShovelTutorial())
		return;

	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mIsDummy)
			continue;

		SeedType aSeedType = aPlant->mSeedType;
		if (aSeedType == SeedType::SEED_IMITATER && aPlant->mImitaterType != SeedType::SEED_NONE)
		{
			aSeedType = aPlant->mImitaterType;
		}

		// 检测植物是否位于目标格子内
		if (aPlant->mRow != theGridY)
		{
			continue;
		}
		if (aSeedType == SeedType::SEED_COBCANNON)
		{
			if (aPlant->mPlantCol < theGridX - 1 || aPlant->mPlantCol > theGridX)
			{
				continue;
			}
		}
		else
		{
			if (aPlant->mPlantCol != theGridX)
			{
				continue;
			}
		}
		if (aPlant->NotOnGround())
		{
			continue;
		}

		// 将植物写入 thePlantOnLawn 的记录
		if (Plant::IsFlying(aPlant->mSeedType))
		{
			//TOD_ASSERT(!thePlantOnLawn->mFlyingPlant);
			thePlantOnLawn->mFlyingPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_FLOWERPOT || aSeedType == SeedType::SEED_FLYINGPOT || aSeedType == SeedType::SEED_WATERPOT)
		{
			thePlantOnLawn->mPotPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
		{
			thePlantOnLawn->mPadPlant = aPlant;
		}
		else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
		{
			TOD_ASSERT(!thePlantOnLawn->mPumpkinPlant);
			thePlantOnLawn->mPumpkinPlant = aPlant;
		}
		else
		{
			thePlantOnLawn->mNormalPlant = aPlant;
		}
	}
}

Plant* Board::GetTopPlantAt(int theGridX, int theGridY, PlantPriority thePriority)
{
	if (theGridX < 0 || theGridX >= MAX_GRID_SIZE_X || theGridY < 0 || theGridY >= MAX_GRID_SIZE_Y)
		return nullptr;

	if (mApp->IsWallnutBowlingLevel() && !mCutScene->IsInShovelTutorial())
		return nullptr;

	PlantsOnLawn aPlantOnLawn;
	GetPlantsOnLawn(theGridX, theGridY, &aPlantOnLawn);

	switch (thePriority)
	{
	case PlantPriority::TOPPLANT_EATING_ORDER:
		if (aPlantOnLawn.mPumpkinPlant)							return aPlantOnLawn.mPumpkinPlant;
		else if (aPlantOnLawn.mNormalPlant)						return aPlantOnLawn.mNormalPlant;
		else if (aPlantOnLawn.mFlyingPlant)						return aPlantOnLawn.mFlyingPlant;
		else if (aPlantOnLawn.mPadPlant)						return aPlantOnLawn.mPadPlant;
		else													return aPlantOnLawn.mPotPlant;
	case PlantPriority::TOPPLANT_DIGGING_ORDER:
		if (aPlantOnLawn.mNormalPlant)							return aPlantOnLawn.mNormalPlant;
		else if (aPlantOnLawn.mFlyingPlant)						return aPlantOnLawn.mFlyingPlant;
		else if (aPlantOnLawn.mPadPlant)						return aPlantOnLawn.mPadPlant;
		else													return aPlantOnLawn.mPotPlant;
	case PlantPriority::TOPPLANT_BUNGEE_ORDER:
	case PlantPriority::TOPPLANT_CATAPULT_ORDER:
	case PlantPriority::TOPPLANT_ANY:
		if (aPlantOnLawn.mFlyingPlant)							return aPlantOnLawn.mFlyingPlant;
		else if (aPlantOnLawn.mNormalPlant)						return aPlantOnLawn.mNormalPlant;
		else if (aPlantOnLawn.mPumpkinPlant)					return aPlantOnLawn.mPumpkinPlant;
		else if (aPlantOnLawn.mPadPlant)						return aPlantOnLawn.mPadPlant;
		else													return aPlantOnLawn.mPotPlant;
	case PlantPriority::TOPPLANT_ZEN_TOOL_ORDER:
		if (aPlantOnLawn.mFlyingPlant)							return aPlantOnLawn.mFlyingPlant;
		else if (aPlantOnLawn.mPumpkinPlant)					return aPlantOnLawn.mPumpkinPlant;
		else if (aPlantOnLawn.mNormalPlant)						return aPlantOnLawn.mNormalPlant;
		else if (aPlantOnLawn.mPadPlant)						return aPlantOnLawn.mPadPlant;
		else													return aPlantOnLawn.mPotPlant;
	case PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION:			return aPlantOnLawn.mNormalPlant;
	case PlantPriority::TOPPLANT_ONLY_FLYING:					return aPlantOnLawn.mFlyingPlant;
	case PlantPriority::TOPPLANT_ONLY_PUMPKIN:					return aPlantOnLawn.mPumpkinPlant;
	case PlantPriority::TOPPLANT_ONLY_UNDER_PLANT:
		if (aPlantOnLawn.mPadPlant)								return aPlantOnLawn.mPadPlant;
		else													return aPlantOnLawn.mPotPlant;
	default:													TOD_ASSERT();
	}
}

//0x40D3A0
int Board::CountSunFlowers()
{
	int aCount = 0;
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->MakesSun())
		{
			aCount++;
		}
	}
	return aCount;
}

//0x40D3F0
int Board::CountPlantByType(SeedType theSeedType)
{
	int aCount = 0;
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == theSeedType)
		{
			aCount++;
		}
	}
	return aCount;
}

//0x40D430
int Board::CountEmptyPotsOrLilies(SeedType theSeedType)
{
	int aCount = 0;
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == theSeedType && !GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION))
		{
			aCount++;
		}
	}
	return aCount;
}

//0x40D4F0
bool Board::IsValidCobCannonSpotHelper(int theGridX, int theGridY)
{
	PlantsOnLawn aPlantOnLawn;
	GetPlantsOnLawn(theGridX, theGridY, &aPlantOnLawn);
	if (aPlantOnLawn.mPumpkinPlant)
		return false;

	if (aPlantOnLawn.mNormalPlant && aPlantOnLawn.mNormalPlant->mSeedType == SeedType::SEED_KERNELPULT)
		return true;

	return mApp->mEasyPlantingCheat && CanPlantAt(theGridX, theGridY, SeedType::SEED_KERNELPULT) == PlantingReason::PLANTING_OK;
}

//0x40D550
bool Board::IsValidCobCannonSpot(int theGridX, int theGridY)
{
	if (!IsValidCobCannonSpotHelper(theGridX, theGridY) || !IsValidCobCannonSpotHelper(theGridX + 1, theGridY))
		return false;

	return !GetFlowerPotAt(theGridX, theGridY) == !GetFlowerPotAt(theGridX + 1, theGridY);
}

//0x40D5C0
bool Board::HasValidCobCannonSpot()
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_KERNELPULT && IsValidCobCannonSpot(aPlant->mPlantCol, aPlant->mRow))
		{
			return true;
		}
	}
	return false;
}

//0x40D620
Projectile* Board::AddProjectile(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType)
{
	Projectile* aProjectile = mProjectiles.DataArrayAlloc();
	aProjectile->ProjectileInitialize(theX, theY, theRenderOrder, theRow, theProjectileType);
	return aProjectile;
}

//0x40D660
bool Board::CanZombieSpawnOnLevel(ZombieType theZombieType, int theLevel)
{
	if (theZombieType == ZOMBIE_INVALID)
		return false;

	const ZombieDefinition& aZombieDef = GetZombieDefinition(theZombieType);
	if (theZombieType == ZombieType::ZOMBIE_YETI)
	{
		return gLawnApp->CanSpawnYetis();
	}

	if (theLevel < aZombieDef.mStartingLevel || aZombieDef.mPickWeight == 0)
	{
		return false;
	}

	//TOD_ASSERT(gZombieAllowedLevels[theZombieType].mZombieType == theZombieType);
	return gZombieAllowedLevels[theZombieType].mAllowedOnLevel[ClampInt(theLevel - 1, 0, 59)]; //49
}

//0x40D6F0
ZombieType Board::GetIntroducedZombieType()
{
	if (!mApp->IsAdventureMode() || mLevel == 1)
	{
		return ZombieType::ZOMBIE_INVALID;
	}

	for (ZombieType aZombieType = ZombieType::ZOMBIE_NORMAL; aZombieType < ZombieType::NUM_ZOMBIE_TYPES; aZombieType = (ZombieType)((int)aZombieType + 1))
	{
		const ZombieDefinition& aZombieDef = GetZombieDefinition(aZombieType);
		if (aZombieType != ZombieType::ZOMBIE_YETI && mApp->CanSpawnYetis() && aZombieDef.mStartingLevel == mLevel)
		{
			return aZombieType;
		}
	}
	return ZombieType::ZOMBIE_INVALID;
}

//0x40D770
ZombieType Board::PickGraveRisingZombieType(int theZombiePoints)
{
	TodWeightedArray aZombieWeightArray[(int)ZombieType::NUM_ZOMBIE_TYPES];
	int aCount = 0;
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_METAL_FESTIVAL) {
		aCount = 1;
		aZombieWeightArray[0].mItem = ZombieType::ZOMBIE_PAIL;
		aZombieWeightArray[0].mWeight = GetZombieDefinition(ZombieType::ZOMBIE_PAIL).mPickWeight;
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_WE_SMASH) {
		aCount = 1;
		aZombieWeightArray[0].mItem = ZombieType::ZOMBIE_GARGANTUAR;
		aZombieWeightArray[0].mWeight = GetZombieDefinition(ZombieType::ZOMBIE_GARGANTUAR).mPickWeight;
	}
	else {
		if (mZombieAllowed[ZOMBIE_NORMAL])
		{
			aZombieWeightArray[aCount].mItem = ZombieType::ZOMBIE_NORMAL;
			aZombieWeightArray[aCount].mWeight = GetZombieDefinition(ZombieType::ZOMBIE_NORMAL).mPickWeight;
			aCount++;
		}
		if (mZombieAllowed[ZOMBIE_TRAFFIC_CONE])
		{
			aZombieWeightArray[aCount].mItem = ZombieType::ZOMBIE_TRAFFIC_CONE;
			aZombieWeightArray[aCount].mWeight = GetZombieDefinition(ZombieType::ZOMBIE_TRAFFIC_CONE).mPickWeight;
			aCount++;
		}
	}

	if (aCount == 0) return ZOMBIE_INVALID;

	return (ZombieType)TodPickFromWeightedArray(aZombieWeightArray, aCount);
}

//0x40D8A0
ZombieType Board::PickZombieType(int theZombiePoints, int theWaveIndex, ZombiePicker* theZombiePicker)
{
	int aPickCount = 0;
	TodWeightedArray aZombieWeightArray[ZombieType::NUM_ZOMBIE_TYPES];
	for (int aZombieType = ZombieType::ZOMBIE_NORMAL; aZombieType < ZombieType::NUM_ZOMBIE_TYPES; aZombieType++)
	{
		if (!mZombieAllowed[aZombieType])
			continue;

		const ZombieDefinition& aZombieDef = GetZombieDefinition((ZombieType)aZombieType);

		// ================================================================================================
		// ▲ 将不符合出怪限制或超出剩余点数的僵尸类型排除
		// ================================================================================================
		GameMode aGameMode = mApp->mGameMode;
		// 蹦极僵尸在无尽模式中仅在旗帜波出现
		if (aZombieType == ZombieType::ZOMBIE_BUNGEE && mApp->IsSurvivalEndless(aGameMode))
		{
			if (!IsFlagWave(theWaveIndex))
			{
				continue;
			}
		}
		// 僵尸最早出现的波数的限制（出怪限制）
		else if (aGameMode != GameMode::GAMEMODE_CHALLENGE_POGO_PARTY && aGameMode != GameMode::GAMEMODE_CHALLENGE_METAL_FESTIVAL && aGameMode != GameMode::GAMEMODE_CHALLENGE_WE_SMASH && aGameMode != GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA)
		{
			int aFirstAllowedWave = aZombieDef.mFirstAllowedWave;
			// 无尽模式中，僵尸最早可出现的波数逐渐前移
			if (mApp->IsSurvivalEndless(aGameMode))
			{
				int aFlags = GetSurvivalFlagsCompleted();
				int aAllowedWave = aFirstAllowedWave - TodAnimateCurve(18, 50, aFlags, 0, 15, TodCurves::CURVE_LINEAR);
				aFirstAllowedWave = max(aAllowedWave, 1);
			}
			if (theWaveIndex + 1 < aFirstAllowedWave || theZombiePoints < aZombieDef.mZombieValue)
			{
				continue;
			}
		}

		// ================================================================================================
		// ▲ 生存模式中，根据当前旗帜数等重新计算僵尸的权重
		// ================================================================================================
		int aPickWeight = aZombieDef.mPickWeight;
		if (mApp->IsSurvivalMode())
		{
			int aFlags = GetSurvivalFlagsCompleted();
			// 伽刚特尔和雪橇车僵尸的每波出怪上限
			if (aZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombieType == ZombieType::ZOMBIE_ZAMBONI)
			{
				if (theZombiePicker->mZombieTypeCount[aZombieType] >= TodAnimateCurve(10, 50, aFlags, 2, 50, TodCurves::CURVE_LINEAR))
				{
					continue;
				}
			}
			// 红眼的旗帜波出怪上限和非旗帜波出怪总和上限
			else if (aZombieType >= ZOMBIE_REDEYE_GARGANTUAR)
			{
				if (IsFlagWave(theWaveIndex))
				{
					if (theZombiePicker->mZombieTypeCount[aZombieType] >= TodAnimateCurve(14, 100, aFlags, 1, 50, TodCurves::CURVE_LINEAR))
					{
						continue;
					}
				}
				else
				{
					if (theZombiePicker->mAllWavesZombieTypeCount[aZombieType] >= TodAnimateCurve(10, 110, aFlags, 1, 50, TodCurves::CURVE_LINEAR))
					{
						continue;
					}
					aPickWeight = 1000;
				}
			}
			// 普通僵尸和路障僵尸的权重衰减
			else if (aZombieType == ZombieType::ZOMBIE_NORMAL)
			{
				aPickWeight = TodAnimateCurve(10, 50, aFlags, aPickWeight, aPickWeight / 10, TodCurves::CURVE_LINEAR);
			}
			else if (aZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE)
			{
				aPickWeight = TodAnimateCurve(10, 50, aFlags, aPickWeight, aPickWeight / 4, TodCurves::CURVE_LINEAR);
			}
		}

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_POGO_PARTY && aZombieType == ZombieType::ZOMBIE_POGO)
		{
			aPickWeight *= 10;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOBSLED_BONANZA && (aZombieType == ZombieType::ZOMBIE_ZAMBONI || aZombieType == ZombieType::ZOMBIE_BOBSLED))
		{
			aPickWeight *= 10;
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_WE_SMASH && (aZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR))
		{
			aPickWeight *= 10;
		}

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL)
		{
			if (aZombieType == ZombieType::ZOMBIE_NORMAL) {
				aPickWeight = TodAnimateCurve(0, 10, mCurrentWave, aPickWeight, aPickWeight / 10, TodCurves::CURVE_LINEAR);

			}else if (aZombieType == ZombieType::ZOMBIE_PAIL){
				aPickWeight = TodAnimateCurve(0, 10, mCurrentWave, aPickWeight, aPickWeight * 10, TodCurves::CURVE_LINEAR);

			}else if (aZombieType == ZombieType::ZOMBIE_GARGANTUAR){
				aPickWeight = 0;
			}
		}

		if (aPickWeight == 0)
			continue;

		aZombieWeightArray[aPickCount].mItem = aZombieType;
		aZombieWeightArray[aPickCount].mWeight = aPickWeight;
		aPickCount++;
	}

	// 加权随机地取得一种可能的僵尸类型并返回
	return (ZombieType)TodPickFromWeightedArray(aZombieWeightArray, aPickCount);
}

bool Board::IsZombieTypePoolOnly(ZombieType theZombieType)
{
	return (theZombieType == ZombieType::ZOMBIE_SNORKEL || theZombieType == ZombieType::ZOMBIE_DIVER || theZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER || theZombieType == ZombieType::ZOMBIE_SHARK_RIDER);
}

//0x40DB20
bool Board::RowCanHaveZombieType(int theRow, ZombieType theZombieType)
{
	if (!RowCanHaveZombies(theRow))
	{
		return false;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED_2 && mCurrentWave < 5
		&& mPlantRow[theRow] == PlantRowType::PLANTROW_POOL)
	{
		return true;
	}

	if (mBackground == BACKGROUND_GRAVEYARD && theRow == 2 && theZombieType == ZOMBIE_DIGGER)
	{
		return false;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED && mPlantRow[theRow] == PlantRowType::PLANTROW_DIRT && mCurrentWave < 5)
	{
		return false;  // 无草皮之地关卡，无草皮的行在前 5 波不刷出僵尸
	}
	if (mPlantRow[theRow] == PlantRowType::PLANTROW_POOL && !Zombie::ZombieTypeCanGoInPool(theZombieType) && mBackground != BACKGROUND_3_POOLWIDE)
	{
		return false;  // 水路不会刷出不能进入泳池的僵尸
	}
	if (mPlantRow[theRow] == PlantRowType::PLANTROW_HIGH_GROUND && !Zombie::ZombieTypeCanGoOnHighGround(theZombieType))
	{
		return false;  // 高地不会刷出不能走上高地的僵尸
	}

	int aCurrentWave = mCurrentWave;
	if (LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		aCurrentWave += mChallenge->mSurvivalStage * GetNumWavesPerSurvivalStage();
	}
	// 非水路不能刷出水路僵尸；前 5 小波，水面仅刷出潜水僵尸或海豚骑士僵尸
	if (mPlantRow[theRow] == PlantRowType::PLANTROW_POOL && mBackground != BACKGROUND_3_POOLWIDE && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
	{
		if (aCurrentWave < 5 && !IsZombieTypePoolOnly(theZombieType))
		{
			return false;
		}
	}
	else if (IsZombieTypePoolOnly(theZombieType) && mBackground != BACKGROUND_3_POOLWIDE && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
	{
		return false;
	}
	// 雪橇僵尸小队仅能在有冰道的行刷出
	if (theZombieType == ZOMBIE_BOBSLED && !mIceTimer[theRow])
	{
		return false;
	}
	// “自古一路无巨人”（生存模式除外）

	/*if (theRow == 0 && !mApp->IsSurvivalMode())
	{
		if (theZombieType == ZombieType::ZOMBIE_GARGANTUAR || theZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
		{
			return false;
		}
	}*/

	// 非舞王僵尸或当前为泳池关卡，则可以刷出该僵尸
	if (theZombieType != ZombieType::ZOMBIE_DANCER && theZombieType != ZombieType::ZOMBIE_DISCO || StageHasPool())
	{
		return true;
	}
	// 舞王僵尸在非泳池关卡中，为保证能召唤伴舞僵尸，仅在中间三行刷出
	return RowCanHaveZombies(theRow - 1) && RowCanHaveZombies(theRow + 1);
}

//0x40DC50
int Board::PickRowForNewZombie(ZombieType theZombieType)
{
	// ====================================================================================================
	// ▲ 当存在正在寻找目标僵尸的钉耙，且僵尸可以出现在钉耙所在行时，优先出现在钉耙所在行
	// ====================================================================================================
	GridItem* aRake = GetRake();
	if (aRake && aRake->mGridItemState == GridItemState::GRIDITEM_STATE_RAKE_ATTRACTING && RowCanHaveZombieType(aRake->mGridY, theZombieType))
	{
		aRake->mGridItemState = GridItemState::GRIDITEM_STATE_RAKE_WAITING;
		TodUpdateSmoothArrayPick(mRowPickingArray, MAX_GRID_SIZE_Y, aRake->mGridY);
		return aRake->mGridY;
	}

	// ====================================================================================================
	// ▲ 遍历每一行，将所有能允许该僵尸出现的行及其对应权重写入挑选数组中
	// ====================================================================================================
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		// 如果本行不能出现目标僵尸，则将本行权重置零，并继续下一行
		if (!RowCanHaveZombieType(aRow, theZombieType))
		{
			mRowPickingArray[aRow].mWeight = 0;
		}
		// 保护传送门关卡中，每行的出怪概率受传送门位置影响
		else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2 || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
		{
			mRowPickingArray[aRow].mWeight = mChallenge->PortalCombatRowSpawnWeight(aRow);
		}
		// 隐形食脑者关卡中，前 3 波第六路不出怪
		else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL && mCurrentWave <= 3 && aRow == 5)
		{
			mRowPickingArray[aRow].mWeight = 0;
		}
		// 丢车保护
		else
		{
			int aWavesMowered = mCurrentWave - mWaveRowGotLawnMowered[aRow];
			if (mApp->IsContinuousChallenge() && mCurrentWave == mNumWaves - 1)
			{
				aWavesMowered = 100;
			}

			if (aWavesMowered <= 1)
			{
				mRowPickingArray[aRow].mWeight = 0.01f;
			}
			else if (aWavesMowered <= 2)
			{
				mRowPickingArray[aRow].mWeight = 0.5f;
			}
			else
			{
				mRowPickingArray[aRow].mWeight = 1.0f;
			}
		}
	}
	return TodPickFromSmoothArray(mRowPickingArray, MAX_GRID_SIZE_Y);
}

//0x40DD90
bool Board::CanAddBobSled()
{
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		if (mIceTimer[aRow] > 0 && mIceMinX[aRow] < 700)
		{
			return true;
		}
	}
	return false;
}

//0x40DDC0
Zombie* Board::AddZombieInRow(ZombieType theZombieType, int theRow, int theFromWave)
{
	if (mZombies.mSize >= mZombies.mMaxSize - 1)
	{
		TodTrace("Too many zombies!!");
		return nullptr;
	}

	if (theZombieType == ZOMBIE_INVALID)	return nullptr;

	bool aVariant = !Rand(5);
	Zombie* aZombie = mZombies.DataArrayAlloc();
	aZombie->ZombieInitialize(theRow, theZombieType, aVariant, nullptr, theFromWave);

	if (theZombieType == ZombieType::ZOMBIE_BOBSLED && aZombie->IsOnBoard())
	{
		for (int _i = 0; _i < 3; _i++)
		{
			Zombie* mZombie = mZombies.DataArrayAlloc();
			mZombie->ZombieInitialize(theRow, ZombieType::ZOMBIE_BOBSLED, false, aZombie, theFromWave);
		}
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_DANCE_PARTY || mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
	{
		aZombie->mDanceSwitchRows = RandRangeInt(250, 750);
		aZombie->EnableDance(true);
		
		if (mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON && RandRangeInt(0,4) == 4)
		{
			int weirdID = RandRangeInt(0, 3);
			if (weirdID == 0)
			{
				//dont do anything
			}
			else if (weirdID == 1)
			{
				aZombie->mUpdateTwice = !aZombie->mUpdateTwice;
			}
			else if (weirdID == 2)
			{
				aZombie->mGiant = !aZombie->mGiant;

				if (aZombie->mGiant)
				{
					aZombie->mScaleZombie *= 2.0f;
					aZombie->mBodyHealth *= 4;
					aZombie->mHelmHealth *= 4;
					aZombie->mShieldHealth *= 4;
					aZombie->mFlyingHealth *= 4;
				}
				else {
					aZombie->mScaleZombie /= 2.0f;
					aZombie->mBodyHealth /= 4;
					aZombie->mHelmHealth /= 4;
					aZombie->mShieldHealth /= 4;
					aZombie->mFlyingHealth /= 4;
				}

				aZombie->PickRandomSpeed();
			}
			else if (weirdID == 3)
			{
				aZombie->mTiny = !aZombie->mTiny;

				if (aZombie->mTiny)
				{
					aZombie->mScaleZombie *= 0.5f;
					aZombie->mBodyHealth /= 4;
					aZombie->mHelmHealth /= 4;
					aZombie->mShieldHealth /= 4;
					aZombie->mFlyingHealth /= 4;
				}
				else {
					aZombie->mScaleZombie /= 0.5f;
					aZombie->mBodyHealth *= 4;
					aZombie->mHelmHealth *= 4;
					aZombie->mShieldHealth *= 4;
					aZombie->mFlyingHealth *= 4;
				}

				aZombie->PickRandomSpeed();
			}
		

			aZombie->UpdateReanim();
		}
	}

	return aZombie;
}

Zombie* Board::AddZombie(ZombieType theZombieType, int theFromWave)
{
	return AddZombie(theZombieType, theFromWave, true);
}

Zombie* Board::AddZombie(ZombieType theZombieType, int theFromWave, bool introBush) {
	int row = PickRowForNewZombie(theZombieType);

	if (mBackground == BackgroundType::BACKGROUND_GRAVEYARD && GetGraveStoneCount() > 0 && 
		(theZombieType == ZOMBIE_NORMAL || theZombieType == ZOMBIE_TRAFFIC_CONE) &&
		(Rand(4) == 0 || mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER))
	{
		Zombie* aZombie = mZombies.DataArrayAlloc();
		aZombie->ZombieInitialize(0, theZombieType, Rand(5), nullptr, theFromWave);

		int aGridArrayCount = 0;
		TodWeightedGridArray aGridArray[MAX_GRID_SIZE_X * 5];
	
		GridItem* aGridItem = nullptr;
		while (IterateGridItems(aGridItem))
		{
			if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE && aGridItem->mGridItemCounter >= 100)
			{
				aGridArray[aGridArrayCount].mX = aGridItem->mGridX;
				aGridArray[aGridArrayCount].mY = aGridItem->mGridY;
				aGridArray[aGridArrayCount].mWeight = 10000;
				aGridArrayCount++;
			}
		}

		TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(aGridArray, aGridArrayCount);
		aZombie->RiseFromGrave(aGrid->mX, aGrid->mY);

		if (mLevelAwardSpawned)
		{
			aZombie->DieNoLoot();
		}

		return aZombie;
	}

	if (!introBush && theZombieType != ZOMBIE_BACKUP_DANCER && theZombieType != ZOMBIE_DISCO_BACKUP && !theZombieType != ZOMBIE_BUNGEE && !theZombieType != ZOMBIE_DIGGER)
		AnimateBush(row);
	return AddZombieInRow(theZombieType, row, theFromWave);;
}

//0x40DEA0
void Board::RemoveAllZombies()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying())
		{
			aZombie->DieNoLoot();
		}
	}
}

//0x40DF00
void Board::RemoveZombiesForRepick()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying() && aZombie->mMindControlled && aZombie->mPosX > 720)
		{
			aZombie->DieNoLoot();
		}
	}
}

//0x40DF70
void Board::RemoveCutsceneZombies()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE)
		{
			aZombie->DieNoLoot();
		}
	}
}

//0x40DFC0
bool Board::IsIceAt(int theGridX, int theGridY)
{
	TOD_ASSERT(theGridY >= 0 && theGridY < MAX_GRID_SIZE_Y);
	if (mIceTimer[theGridY] == 0 || mIceMinX[theGridY] > 750)
		return false;

	return theGridX >= PixelToGridXKeepOnBoard(mIceMinX[theGridY] + 12, 0);
}

//0x40E020
PlantingReason Board::CanPlantAt(int theGridX, int theGridY, SeedType theSeedType)
{
	// 目标位置不在场地内，则返回“不能种在那里”
	if (theGridX < 0 || theGridX >= MAX_GRID_SIZE_X || theGridY < 0 || theGridY >= MAX_GRID_SIZE_Y)
	{
		return PlantingReason::PLANTING_NOT_HERE;
	}

	// 从关卡玩法上，判断能否种植
	PlantingReason aReason = mChallenge->CanPlantAt(theGridX, theGridY, theSeedType);
	if (aReason != PlantingReason::PLANTING_OK || Challenge::IsZombieSeedType(theSeedType))
	{
		return aReason;
	}

	if (theSeedType == SEED_CHERRYHOVERBOMB) {
		return PLANTING_OK;
	}

	PlantsOnLawn aPlantOnLawn;
	GetPlantsOnLawn2(theGridX, theGridY, &aPlantOnLawn);
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (aPlantOnLawn.mPotPlant || aPlantOnLawn.mPumpkinPlant || aPlantOnLawn.mFlyingPlant || aPlantOnLawn.mNormalPlant)
		{
			return PlantingReason::PLANTING_NOT_HERE;
		}
		if (mApp->mZenGarden->mGardenType == GARDEN_AQUARIUM && !Plant::IsAquatic(theSeedType))
		{
			return PlantingReason::PLANTING_NOT_ON_WATER;
		}

		return PlantingReason::PLANTING_OK;
	}

	// 墓碑吞噬者只能种植在墓碑上
	bool aHasGrave = GetGraveStoneAt(theGridX, theGridY);
	if (theSeedType == SeedType::SEED_GRAVEBUSTER)
	{
		if (aPlantOnLawn.mNormalPlant)
		{
			return PlantingReason::PLANTING_NOT_HERE;
		}

		return aHasGrave ? PlantingReason::PLANTING_OK : PlantingReason::PLANTING_ONLY_ON_GRAVES;
	}

	if (theSeedType == SeedType::SEED_INSTANTCOFFEE)
	{
		if (aPlantOnLawn.mFlyingPlant && aPlantOnLawn.mFlyingPlant->mSeedType == SeedType::SEED_INSTANTCOFFEE)
		{
			return PlantingReason::PLANTING_NOT_HERE;
		}

		if (!aPlantOnLawn.mNormalPlant || !aPlantOnLawn.mNormalPlant->mIsAsleep || aPlantOnLawn.mNormalPlant->mWakeUpCounter > 0 ||
			aPlantOnLawn.mNormalPlant->mOnBungeeState == PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
		{
			return PlantingReason::PLANTING_NEEDS_SLEEPING;
		}

		return PlantingReason::PLANTING_OK;
	}
	else if (Plant::IsFlying(theSeedType)) 
	{
		if (aPlantOnLawn.mFlyingPlant && aPlantOnLawn.mFlyingPlant->mSeedType != SeedType::SEED_INSTANTCOFFEE)
		{
			return PlantingReason::PLANTING_NOT_HERE;
		}

		return PlantingReason::PLANTING_OK;
	}
	// 非墓碑吞噬者且非飞行植物，则不能种在墓碑上
	if (aHasGrave)
	{
		return Plant::IsFlying(theSeedType) ? PlantingReason::PLANTING_OK : PlantingReason::PLANTING_NOT_ON_GRAVE;
	}
	
	Plant* aPotPlant = aPlantOnLawn.mPotPlant;
	Plant* aPadPlant = aPlantOnLawn.mPadPlant;
	bool aHasLilypad = false, aHasFlowerPot = false, aHasWaterPot = false, aHasFlyingPot = false;

	if (!aPotPlant|| aPotPlant->mOnBungeeState == PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
	{
		aHasFlowerPot = false;
		aHasWaterPot = false;
		aHasFlyingPot = false;
	}
	else
	{
		aHasFlowerPot = aPotPlant->mSeedType == SeedType::SEED_FLOWERPOT;
		aHasWaterPot = aPotPlant->mSeedType == SeedType::SEED_WATERPOT;
		aHasFlyingPot = aPotPlant->mSeedType == SeedType::SEED_FLYINGPOT;
	}

	if (!aPadPlant || aPadPlant->mOnBungeeState == PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
	{
		aHasLilypad = false;
	}
	else
	{
		aHasLilypad = aPadPlant->mSeedType == SeedType::SEED_LILYPAD;
	}

	// 部分情况下的格子中不能种植植物
	if (GetCraterAt(theGridX, theGridY) && !aHasFlyingPot && theSeedType != SEED_FLYINGPOT)
	{
		return PlantingReason::PLANTING_NOT_ON_CRATER;
	}
	if (GetScaryPotAt(theGridX, theGridY) || IsIceAt(theGridX, theGridY) && !aHasFlyingPot && theSeedType != SEED_FLYINGPOT)
	{
		return PlantingReason::PLANTING_NOT_HERE;
	}
	GridSquareType aGridSquare = mGridSquareType[theGridX][theGridY];
	if ((aGridSquare == GridSquareType::GRIDSQUARE_DIRT || aGridSquare == GridSquareType::GRIDSQUARE_NONE) && !aHasFlyingPot && theSeedType != SEED_FLYINGPOT /* && !aHasFlowerPot && !aHasWaterPot && theSeedType != SEED_WATERPOT && theSeedType != SEED_FLOWERPOT*/)
	{
		return PlantingReason::PLANTING_NOT_HERE;
	}
	// 水生植物只能种在水上
	Plant* aNormalPlant = aPlantOnLawn.mNormalPlant;
	if (theSeedType == SeedType::SEED_LILYPAD  || theSeedType == SeedType::SEED_SEASHROOM)
	{
		if (!IsPoolSquare(theGridX, theGridY) && !aHasWaterPot)
			return PlantingReason::PLANTING_ONLY_IN_POOL;

		return (aNormalPlant || aPadPlant) ? PlantingReason::PLANTING_NOT_HERE : PlantingReason::PLANTING_OK;
	}
	else if (theSeedType == SeedType::SEED_TANGLEKELP)
	{
		if (!IsPoolSquare(theGridX, theGridY) && !aHasWaterPot)
			return PlantingReason::PLANTING_ONLY_IN_POOL;

		return (aNormalPlant || aPadPlant) ? PlantingReason::PLANTING_NOT_HERE : PlantingReason::PLANTING_OK;
	}

	if (Plant::IsFlying(theSeedType))
		return aPlantOnLawn.mFlyingPlant ? PlantingReason::PLANTING_NOT_HERE : PlantingReason::PLANTING_OK;
	

	// 地刺/地刺王只能种在坚固的地面
	if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
	{
		if (aGridSquare == GridSquareType::GRIDSQUARE_POOL || StageHasRoof() || aPotPlant)
		{
			return PlantingReason::PLANTING_NEEDS_GROUND;
		}
	}
	// 非水生植物不能种在水面上（南瓜头可以种在香蒲上）
	Plant* aPumpkinPlant = aPlantOnLawn.mPumpkinPlant;
	if (aGridSquare == GridSquareType::GRIDSQUARE_POOL && !aHasLilypad && theSeedType != SeedType::SEED_CATTAIL && !aHasFlyingPot && theSeedType != SeedType::SEED_FLYINGPOT)
	{
		if (!aNormalPlant || aNormalPlant->mSeedType != SeedType::SEED_CATTAIL || theSeedType != SeedType::SEED_PUMPKINSHELL)
		{
			return PlantingReason::PLANTING_NOT_ON_WATER;
		}
	}
	// 花盆的种植条件
	if (theSeedType == SeedType::SEED_FLOWERPOT || theSeedType == SeedType::SEED_FLYINGPOT || theSeedType == SeedType::SEED_WATERPOT)
	{
		return (aNormalPlant || aPotPlant || aPumpkinPlant) ? PlantingReason::PLANTING_NOT_HERE : PlantingReason::PLANTING_OK;
	}
	// 屋顶种植需要花盆
	if (StageHasRoof() || mBackground == BackgroundType::BACKGROUND_GRAVEYARD && theGridY == 2)
	{
		if (aHasLilypad && !aNormalPlant || mApp->IsWallnutBowlingLevel())
			return PLANTING_OK;
		//if (!aHasFlowerPot && theSeedType != SEED_PUFFSHROOM && theSeedType != SEED_SCAREDYSHROOM)
		if (!aHasFlowerPot && !aHasFlyingPot)
			return PlantingReason::PLANTING_NEEDS_POT;
	}

	// 南瓜头的种植条件
	bool aAidPurchased = mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_FIRSTAID] > 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_HEAT_WAVE;
	if (theSeedType == SeedType::SEED_PUMPKINSHELL)
	{
		// 不可种植在玉米加农炮上
		if (aNormalPlant && aNormalPlant->mSeedType == SeedType::SEED_COBCANNON)
		{
			return PlantingReason::PLANTING_NOT_HERE;
		}
		// 无南瓜头时，可以种植南瓜头
		if (!aPumpkinPlant)
		{
			return PlantingReason::PLANTING_OK;
		}
		// 南瓜头的坚果包扎术
		if (aAidPurchased && aPumpkinPlant->mPlantHealth < aPumpkinPlant->mPlantMaxHealth * 2 / 3 &&
			aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL && aPumpkinPlant->mOnBungeeState != PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
		{
			return PlantingReason::PLANTING_OK;
		}

		return PlantingReason::PLANTING_NOT_HERE;
	}
	// 土豆地雷只能种在陆地上
	if (aHasLilypad && (theSeedType == SeedType::SEED_POTATOMINE || theSeedType == SeedType::SEED_SEEDROOT))
	{
		return PlantingReason::PLANTING_ONLY_ON_GROUND;
	}

	if (aPadPlant)
	{
		// 香蒲对底端植物的紫卡升级
		if (theSeedType == SeedType::SEED_CATTAIL)
		{
			if (aNormalPlant)
			{
				return PlantingReason::PLANTING_NOT_HERE;
			}
			if (aPadPlant->IsUpgradableTo(theSeedType) && aPadPlant->mOnBungeeState != PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
			{
				return PlantingReason::PLANTING_OK;
			}
			if (Plant::IsUpgrade(theSeedType))
			{
				return PlantingReason::PLANTING_NEEDS_UPGRADE;
			}
		}
		else
		{
			// 模仿中的模仿者不可作为花盆或睡莲
			if (aPadPlant->mSeedType == SeedType::SEED_IMITATER)
			{
				return PlantingReason::PLANTING_NOT_HERE;
			}
		}
	}

	// 一般紫卡植物的更迭判断
	if (aNormalPlant)
	{
		// 紫卡植物的升级
		if (aNormalPlant->IsUpgradableTo(theSeedType) && aNormalPlant->mOnBungeeState != PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
		{
			return PlantingReason::PLANTING_OK;
		}
		if (Plant::IsUpgrade(theSeedType))
		{
			return PlantingReason::PLANTING_NEEDS_UPGRADE;
		}

		// 坚果包扎术
		if ((theSeedType == SeedType::SEED_WALLNUT || theSeedType == SeedType::SEED_TALLNUT) && aAidPurchased)
		{
			if (aNormalPlant->mPlantHealth < aNormalPlant->mPlantMaxHealth * 2 / 3 &&
				aNormalPlant->mSeedType == theSeedType && aNormalPlant->mOnBungeeState != PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
			{
				return PlantingReason::PLANTING_OK;
			}
		}

		if (theSeedType == SeedType::SEED_UMBRELLA && aAidPurchased && mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL)
		{
			if (aNormalPlant->mPlantHealth < aNormalPlant->mPlantMaxHealth * 2 / 3 &&
				aNormalPlant->mSeedType == theSeedType && aNormalPlant->mOnBungeeState != PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE)
			{
				return PlantingReason::PLANTING_OK;
			}
		}

		return PlantingReason::PLANTING_NOT_HERE;
	}

	// 免费种植模式下紫卡的额外判断
	if (!mApp->mEasyPlantingCheat && Plant::IsUpgrade(theSeedType))
	{
		return PlantingReason::PLANTING_NEEDS_UPGRADE;
	}
	if (theSeedType == SeedType::SEED_COBCANNON && !IsValidCobCannonSpot(theGridX, theGridY))
	{
		return PlantingReason::PLANTING_NEEDS_UPGRADE;
	}
	else if (theSeedType == SeedType::SEED_CATTAIL && aGridSquare != GridSquareType::GRIDSQUARE_POOL)
	{
		return PlantingReason::PLANTING_NOT_HERE;
	}

	if (aHasWaterPot && !aHasLilypad && !Plant::IsAquatic(theSeedType))
		return PlantingReason::PLANTING_NOT_ON_WATER;

	return PlantingReason::PLANTING_OK;
}

//0x40E520
void Board::UpdateCursor()
{
	int aMouseX = mApp->mWidgetManager->mLastMouseX - mX;
	int aMouseY = mApp->mWidgetManager->mLastMouseY - mY;
	bool aShowFinger = false;
	bool aShowDrag = false;
	bool aHideCursor = false;

	if (mApp->mSeedChooserScreen && mApp->mSeedChooserScreen->Contains(aMouseX + mX, aMouseY + mY))
		return;

	if (mApp->GetDialogCount() > 0)
		return;

	if (mPaused || mBoardFadeOutCounter >= 0 || mTimeStopCounter > 0 || mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
	{
		mApp->SetCursor(Sexy::CURSOR_POINTER);
		return;
	}

	HitResult aHitResult;
	MouseHitTest(aMouseX, aMouseY, &aHitResult);
	switch (aHitResult.mObjectType)
	{
	case GameObjectType::OBJECT_TYPE_MENU_BUTTON:
	case GameObjectType::OBJECT_TYPE_SPEED_BUTTON:
	case GameObjectType::OBJECT_TYPE_STORE_BUTTON:
	case GameObjectType::OBJECT_TYPE_SHOVEL:
	case GameObjectType::OBJECT_TYPE_WATERING_CAN:
	case GameObjectType::OBJECT_TYPE_FERTILIZER:
	case GameObjectType::OBJECT_TYPE_BUG_SPRAY:
	case GameObjectType::OBJECT_TYPE_PHONOGRAPH:
	case GameObjectType::OBJECT_TYPE_CHOCOLATE:
	case GameObjectType::OBJECT_TYPE_GLOVE:
	case GameObjectType::OBJECT_TYPE_MONEY_SIGN:
	case GameObjectType::OBJECT_TYPE_NEXT_GARDEN:
	case GameObjectType::OBJECT_TYPE_WHEELBARROW:
	case GameObjectType::OBJECT_TYPE_SLOT_MACHINE_HANDLE:
	case GameObjectType::OBJECT_TYPE_TREE_FOOD:
	case GameObjectType::OBJECT_TYPE_STINKY:
	case GameObjectType::OBJECT_TYPE_TREE_OF_WISDOM:
	case GameObjectType::OBJECT_TYPE_COIN:
	case GameObjectType::OBJECT_TYPE_PROJECTILE:
		aShowFinger = true;
		break;

	case GameObjectType::OBJECT_TYPE_SEEDPACKET:
		aShowFinger = ((SeedPacket*)aHitResult.mObject)->CanPickUp();
		break;

	case GameObjectType::OBJECT_TYPE_SCARY_POT:
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL)
		{
			aShowFinger = true;
		}
		else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_HAMMER)
		{
			aHideCursor = true;
		}
		break;

	case GameObjectType::OBJECT_TYPE_PLANT:
		if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) && !HasLevelAwardDropped())
		{
			aShowFinger = true;
		}
		if (((Plant*)aHitResult.mObject)->mState == PlantState::STATE_COBCANNON_READY)
		{
			aShowFinger = true;
		}
		break;

	default:
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_HAMMER)
		{
			aHideCursor = true;
		}
		break;
	}

	if (mChallenge->mBeghouledMouseCapture || aShowDrag)
	{
		mApp->SetCursor(Sexy::CURSOR_DRAGGING);
	}
	else if (aShowFinger)
	{
		mApp->SetCursor(Sexy::CURSOR_HAND);
	}
	else if (aHideCursor)
	{
		mApp->SetCursor(Sexy::CURSOR_NONE);
	}
	else
	{
		mApp->SetCursor(Sexy::CURSOR_POINTER);
	}
}

//0x40E760
void Board::MouseMove(int x, int y)
{
	Widget::MouseMove(x, y);
	mChallenge->MouseMove(x, y);
}

//0x40E760
void Board::MouseDrag(int x, int y)
{
	Widget::MouseDrag(x, y);
	mChallenge->MouseMove(x, y);
}

//0x40E780
Zombie* Board::ZombieHitTest(int theMouseX, int theMouseY)
{
	Zombie* aZombie = nullptr;
	Zombie* aRecord = nullptr;
	while (IterateZombies(aZombie))
	{
		// 排除已死亡的僵尸
		if (aZombie->mDead || aZombie->IsDeadOrDying())
			continue;

		// 排除关卡引入阶段及选卡界面的植物僵尸
		if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && Zombie::IsZombotany(aZombie->mZombieType))
			continue;

		// 范围判定
		if (aZombie->GetZombieRect().Contains(theMouseX, theMouseY))
		{
			if (aRecord == nullptr || aZombie->mY > aRecord->mY)
			{
				aRecord = aZombie;
			}
		}
	}

	return aRecord;
}

//0x40E880
bool Board::IsPlantInGoldWateringCanRange(int theMouseX, int theMouseY, Plant* thePlant)
{
	/*
	Rect aRect = Rect(theMouseX - 70, theMouseY - 80, 160, 160);
	if (GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ZEN_TOOL_ORDER) == thePlant)
	{
		return aRect.Contains(thePlant->mX + 40, thePlant->mY + 40);
	}
	return false;
	*/

	int aMinX = theMouseX - 70;
	int aMaxX = theMouseX + 90;
	int aMinY = theMouseY - 80;
	int aMaxY = theMouseY + 80;

	if (GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ZEN_TOOL_ORDER) == thePlant)
	{
		return thePlant->mX + 40 >= aMinX && thePlant->mX + 40 < aMaxX && thePlant->mY + 40 >= aMinY && thePlant->mY + 40 < aMaxY;
	}
	return false;
}

//0x40E940
void Board::HighlightPlantsForMouse(int theMouseX, int theMouseY)
{
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GOLD_WATERINGCAN])
	{
		Plant* aPlant = nullptr;
		while (IteratePlants(aPlant))
		{
			if (IsPlantInGoldWateringCanRange(theMouseX, theMouseY, aPlant))
			{
				aPlant->mHighlighted = true;
				Plant* aFlowerPot = GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
				if (aFlowerPot)
				{
					aFlowerPot->mHighlighted = true;
				}
			}
		}
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mApp->mGameScene == SCENE_PLAYING) {
		Plant* aPlant = SpecialPlantHitTest(theMouseX, theMouseY);
		if (aPlant)
			aPlant->mHighlighted = true;
	}
	else
	{
		Plant* aPlant = ToolHitTest(theMouseX, theMouseY);
		if (aPlant)
		{
			aPlant->mHighlighted = true;
			if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
			{
				Plant* aFlowerPot = GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
				if (aFlowerPot)
				{
					aFlowerPot->mHighlighted = true;
				}
			}
		}
	}
}

//0x40EAB0
void Board::UpdateMousePosition()
{
	UpdateCursor();
	UpdateToolTip();
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		aPlant->mHighlighted = false;
	}

	SeedType aCursorSeedType = GetSeedTypeInCursor();
	int aMouseX = mApp->mWidgetManager->mLastMouseX - mX;
	int aMouseY = mApp->mWidgetManager->mLastMouseY - mY;

	// 破罐者关卡中，检测并高亮鼠标悬浮的罐子
	if (mApp->IsScaryPotterLevel())
	{
		GridItem* aGridItem = nullptr;
		while (IterateGridItems(aGridItem))
		{
			if (aGridItem->mGridItemType == GridItemType::GRIDITEM_SCARY_POT)
			{
				aGridItem->mHighlighted = false;
			}
		}

		HitResult aHitResult;
		MouseHitTest(aMouseX, aMouseY, &aHitResult);
		if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_SCARY_POT)
		{
			GridItem* aScaryPot = (GridItem*)aHitResult.mObject;
			aScaryPot->mHighlighted = true;
			return;
		}
	}

	// 禅境花园，设定蜗牛的高亮与否
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		GridItem* aStinky = mApp->mZenGarden->GetStinky();
		if (aStinky)
		{
			HitResult aHitResult;
			MouseHitTest(aMouseX, aMouseY, &aHitResult);
			aStinky->mHighlighted = aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_STINKY;
		}
	}

	/*if (mIsP1Keyboard && P1HasShovel) {
		float OffsetY = 0;
		if (StageHasRoof() && P1SelectorX < 440.0f) {
			OffsetY = int((440.0f - P1SelectorX) * 0.25f);
		}
		float theY = P1SelectorY + OffsetY + (StageHas6Rows() || StageHasRoof() ? 42.5f : 72);
		HighlightPlantsForMouse(P1SelectorX, theY);
	}*/

	// 手持铲子或花园工具时，令作用的植物高亮
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_SHOVEL || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_FERTILIZER ||
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PHONOGRAPH || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE ||
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_GLOVE || 
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_MONEY_SIGN ||
		(mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW && !mApp->mZenGarden->GetPottedPlantInWheelbarrow()))
	{
		HighlightPlantsForMouse(aMouseX, aMouseY);
		return;
	}

	// 咖啡豆及坚果包扎术
	if (aCursorSeedType == SeedType::SEED_INSTANTCOFFEE)
	{
		int aGridX = PlantingPixelToGridX(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);
		int aGridY = PlantingPixelToGridY(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);

		Plant* aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
		if (aPlant && aPlant->mIsAsleep && CanPlantAt(aGridX, aGridY, SeedType::SEED_INSTANTCOFFEE) == PlantingReason::PLANTING_OK)
		{
			aPlant->mHighlighted = true;
		}
	}
	else if (aCursorSeedType == SeedType::SEED_WALLNUT || aCursorSeedType == SeedType::SEED_TALLNUT || aCursorSeedType == SeedType::SEED_UMBRELLA)
	{
		int aGridX = PlantingPixelToGridX(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);
		int aGridY = PlantingPixelToGridY(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);

		Plant* aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ONLY_PUMPKIN);
		if (aPlant && aPlant->mSeedType == aCursorSeedType && CanPlantAt(aGridX, aGridY, aCursorSeedType) == PlantingReason::PLANTING_OK)
		{
			aPlant->mHighlighted = true;
		}
	}
	else if (aCursorSeedType == SeedType::SEED_PUMPKINSHELL)
	{
		int aGridX = PlantingPixelToGridX(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);
		int aGridY = PlantingPixelToGridY(mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH, mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y, aCursorSeedType);

		Plant* aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
		if (aPlant && aPlant->mSeedType == SeedType::SEED_PUMPKINSHELL && CanPlantAt(aGridX, aGridY, SeedType::SEED_PUMPKINSHELL) == PlantingReason::PLANTING_OK)
		{
			aPlant->mHighlighted = true;
		}
	}
}

//0x40EF00
void Board::UpdateToolTip()
{
	if (!mApp->mWidgetManager->mMouseIn || !mApp->mActive || mTimeStopCounter > 0 || mApp->GetDialogCount() > 0 || mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
	{
		mToolTip->mVisible = false;
		return;
	}

	if (mWidgetManager == nullptr)	return;

	int aMouseX = mWidgetManager->mLastMouseX - mX;
	int aMouseY = mWidgetManager->mLastMouseY - mY;

	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
	{
		if (!mCutScene->mSeedChoosing)
		{
			mToolTip->mVisible = false;
			return;
		}

		if (mSeedBank->ContainsPoint(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY) ||
			mApp->mSeedChooserScreen->mAlmanacButton->IsMouseOver() ||
			mApp->mSeedChooserScreen->mStoreButton->IsMouseOver() ||
			mApp->mSeedChooserScreen->mImitaterButton->IsMouseOver())
		{
			mToolTip->mVisible = false;
			return;
		}

		Zombie* aZombie = ZombieHitTest(aMouseX, aMouseY);
		if (aZombie == nullptr || aZombie->mFromWave != Zombie::ZOMBIE_WAVE_CUTSCENE)
		{
			mToolTip->mVisible = false;
			return;
		}

		SexyString aZombieName = StrFormat(_S("[%s]"), GetZombieDefinition(aZombie->mZombieType).mZombieName);
		mToolTip->SetTitle(aZombieName);
		if (mApp->CanShowAlmanac())
		{
			mToolTip->SetLabel(_S("[CLICK_TO_VIEW]"));
		}
		else
		{
			mToolTip->SetLabel(_S(""));
		}
		mToolTip->SetWarningText(_S(""));

		Rect aRect = aZombie->GetZombieRect();
		mToolTip->mX = aRect.mWidth / 2 + aRect.mX + 5;
		mToolTip->mY = aRect.mHeight + aRect.mY - 10;
		if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE)
		{
			mToolTip->mY = aZombie->mY;
		}

		mToolTip->mVisible = true;
		mToolTip->mCenter = true;

		mToolTip->mMinLeft = IMAGE_SEEDCHOOSER_BACKGROUND->GetWidth();
		if (mApp->mSeedChooserScreen->mAlmanacButton->mBtnNoDraw && mApp->mSeedChooserScreen->mStoreButton->mBtnNoDraw)
		{
			mToolTip->mMaxBottom = 600;
		}
		else
		{
			mToolTip->mMaxBottom = 570;
		}
		if (!mApp->mSeedChooserScreen->mImitaterButton->mBtnNoDraw)
		{
			mToolTip->CalculateSize();
			if (mX + mToolTip->mX - mToolTip->mWidth / 2 < 524)
			{
				mToolTip->mMaxBottom = 503;
			}
		}

		return;
	}

	if (!CanInteractWithBoardButtons())
	{
		mToolTip->mVisible = false;
		return;
	}

	mToolTip->mMinLeft = 0;
	mToolTip->mMaxBottom = BOARD_HEIGHT;
	mToolTip->SetTitle(_S(""));
	mToolTip->SetLabel(_S(""));
	mToolTip->SetWarningText(_S(""));
	mToolTip->mCenter = false;
	if (mChallenge->UpdateToolTip(aMouseX, aMouseY))
	{
		return;
	}

	HitResult aHitResult;
	MouseHitTest(aMouseX, aMouseY, &aHitResult);

	if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_SHOVEL)
	{
		mToolTip->SetLabel(_S("[SHOVEL_TOOLTIP]"));
		Rect aShovelButtonRect = GetShovelButtonRect();
		mToolTip->mX = aShovelButtonRect.mX + 35;
		mToolTip->mY = aShovelButtonRect.mY + 72;
		mToolTip->mCenter = true;
		mToolTip->mVisible = true;
		return;
	}

	if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_NEXT_GARDEN)
	{
		mToolTip->SetLabel(_S("[NEXT_GARDEN_TOOLTIP]"));
		Rect aButtonRect = GetShovelButtonRect();
		mToolTip->mX = 564 + 35 - BOARD_ADDITIONAL_WIDTH;
		mToolTip->mY = aButtonRect.mY + 52;
		mToolTip->mCenter = true;
		mToolTip->mVisible = true;
		return;
	}

	if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WATERING_CAN ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_FERTILIZER ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_BUG_SPRAY ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_PHONOGRAPH ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_CHOCOLATE ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_GLOVE ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_MONEY_SIGN ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WHEELBARROW ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_TREE_FOOD)
	{
		if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WATERING_CAN)
		{
			mToolTip->SetLabel(_S("[WATERING_CAN_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_FERTILIZER)
		{
			mToolTip->SetLabel(_S("[FERTILIZER_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_BUG_SPRAY)
		{
			mToolTip->SetLabel(_S("[BUG_SPRAY_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_PHONOGRAPH)
		{
			mToolTip->SetLabel(_S("[PHONOGRAPH_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_CHOCOLATE)
		{
			mToolTip->SetLabel(_S("[CHOCOLATE_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_GLOVE)
		{
			mToolTip->SetLabel(_S("[GLOVE_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_MONEY_SIGN)
		{
			mToolTip->SetLabel(_S("[MONEY_SIGN_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WHEELBARROW)
		{
			mToolTip->SetLabel(_S("[WHEELBARROW_TOOLTIP]"));
		}
		else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_TREE_FOOD)
		{
			mToolTip->SetLabel(_S("[TREE_FERTILIZER_TOOLTIP]"));
		}

		Rect aButtonRect = GetShovelButtonRect();
		GetZenButtonRect(aHitResult.mObjectType, aButtonRect);
		this->mToolTip->mX = aButtonRect.mX + 35;
		this->mToolTip->mY = aButtonRect.mY + 72;
		this->mToolTip->mCenter = true;
		this->mToolTip->mVisible = true;
		return;
	}

	if (aHitResult.mObjectType != GameObjectType::OBJECT_TYPE_SEEDPACKET)
	{
		mToolTip->mVisible = false;
		return;
	}

	SeedPacket* aSeedPacket = (SeedPacket*)aHitResult.mObject;
	SeedType aUseSeedType = aSeedPacket->mPacketType;
	if (aSeedPacket->mPacketType == SeedType::SEED_IMITATER && aSeedPacket->mImitaterType != SeedType::SEED_NONE)
	{
		aUseSeedType = aSeedPacket->mImitaterType;
	}

	if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
	{
		if (aUseSeedType == SeedType::SEED_REPEATER)
		{
			mToolTip->SetLabel(_S("[BEGHOULED_REPEATER_UPGRADE_TOOLTIP]"));
		}
		else if (aUseSeedType == SeedType::SEED_FUMESHROOM)
		{
			mToolTip->SetLabel(_S("[BEGHOULED_FUMESHROOM_UPGRADE_TOOLTIP]"));
		}
		else if (aUseSeedType == SeedType::SEED_TALLNUT)
		{
			mToolTip->SetLabel(_S("[BEGHOULED_TALLNUT_UPGRADE_TOOLTIP]"));
		}
		else if (aUseSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE)
		{
			mToolTip->SetLabel(_S("[BEGHOULED_SHUFFLE_TOOLTIP]"));
		}
		else if (aUseSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
		{
			mToolTip->SetLabel(_S("[BEGHOULED_CRATER_TOOLTIP]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_SLOT_MACHINE_SUN)
	{
		mToolTip->SetLabel(_S("[SLOT_MACHINE_SUN_TOOLTIP]"));
	}
	else if (aUseSeedType == SeedType::SEED_SLOT_MACHINE_DIAMOND)
	{
		mToolTip->SetLabel(_S("[SLOT_MACHINE_DIAMOND_TOOLTIP]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIQUARIUM_SNORKLE)
	{
		mToolTip->SetLabel(_S("[ZOMBIQUARIUM_SNORKEL_TOOLTIP]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIQUARIUM_TROPHY)
	{
		mToolTip->SetLabel(_S("[ZOMBIQUARIUM_TROPHY_TOOLTIP]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_NORMAL)
	{
		mToolTip->SetLabel(_S("[ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_TRAFFIC_CONE)
	{
		mToolTip->SetLabel(_S("[CONEHEAD_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_POLEVAULTER)
	{
		mToolTip->SetLabel(_S("[POLE_VAULTING_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_PAIL)
	{
		mToolTip->SetLabel(_S("[BUCKETHEAD_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_LADDER)
	{
		mToolTip->SetLabel(_S("[LADDER_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_DIGGER)
	{
		mToolTip->SetLabel(_S("[DIGGER_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_BUNGEE)
	{
		mToolTip->SetLabel(_S("[BUNGEE_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_FOOTBALL)
	{
		mToolTip->SetLabel(_S("[FOOTBALL_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_BALLOON)
	{
		mToolTip->SetLabel(_S("[BALLOON_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_SCREEN_DOOR)
	{
		mToolTip->SetLabel(_S("[SCREEN_DOOR_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBONI)
	{
		mToolTip->SetLabel(_S("[ZOMBONI]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_POGO)
	{
		mToolTip->SetLabel(_S("[POGO_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_DANCER)
	{
		mToolTip->SetLabel(_S("[DANCING_ZOMBIE]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_GARGANTUAR)
	{
		mToolTip->SetLabel(_S("[GARGANTUAR]"));
	}
	else if (aUseSeedType == SeedType::SEED_ZOMBIE_IMP)
	{
		mToolTip->SetLabel(_S("[IMP]"));
	}
	else
	{
		mToolTip->SetLabel(StrFormat(_S("[%s]"),GetPlantDefinition(aUseSeedType).mPlantName));
	}

	int aPlantCost = GetCurrentPlantCost(aSeedPacket->mPacketType, aSeedPacket->mImitaterType);
	if (mApp->mEasyPlantingCheat)
	{
		mToolTip->SetWarningText(_S("FREE_PLANTING_CHEAT"));
	}
	else if (!aSeedPacket->mActive && (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST))
	{
		if (aSeedPacket->mPacketType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
		{
			mToolTip->SetWarningText(_S("[BEGHOULED_NO_CRATERS]"));
		}
		else
		{
			mToolTip->SetWarningText(_S("[BEGHOULED_SEED_ALREADY_PURCHASED]"));
		}
	}
	else if (!aSeedPacket->mActive)
	{
		mToolTip->SetWarningText(_S("[WAITING_FOR_SEED]"));
	}
	else if (!CanTakeSunMoney(aPlantCost) && !HasConveyorBeltSeedBank() && !mApp->IsSlotMachineLevel())
	{
		mToolTip->SetWarningText(_S("[NOT_ENOUGH_SUN]"));
	}
	else if (aUseSeedType == SeedType::SEED_GATLINGPEA)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_REPEATER]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_PLASMAWOOD)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("requires repeater"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_WINTERMELON)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_MELONPULT]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_TWINSUNFLOWER)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_SUNFLOWER]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_SPIKEROCK)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_SPIKEWEED]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_COBCANNON)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_KERNELPULTS]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_GOLD_MAGNET)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_MAGNETSHROOM]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_GLOOMSHROOM)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_FUMESHROOM]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_CATTAIL)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_LILY_PAD]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_PLASMAWOOD)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_TORCHWOOD]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_SHOOTINGSTAR)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_STARFRUIT]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_BRAVEYSHROOM)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_SCAREDY_SHROOM]"));
		}
	}
	else if (aUseSeedType == SeedType::SEED_SUPERCHOMPER)
	{
		if (!PlantingRequirementsMet(aUseSeedType))
		{
			mToolTip->SetWarningText(_S("[REQUIRES_CHOMPERS]"));
		}
	}

	mToolTip->mX = (SEED_PACKET_WIDTH - mToolTip->mWidth) / 2 + mSeedBank->mX + aSeedPacket->mOffsetX + aSeedPacket->mX;
	mToolTip->mY = mSeedBank->mY + aSeedPacket->mY + 70;
	mToolTip->mVisible = true;
}

//0x40FC70
void Board::MouseDownCobcannonFire(int x, int y, int theClickCount)
{
	if (theClickCount >= 0 && y >= 80)
	{
		if (mCobCannonCursorDelayCounter > 0 && Distance2D(x, y, mCobCannonMouseX, mCobCannonMouseY) < 100.0f)
		{
			return;  // 误点检测：点击加农炮后的 30cs 内，点击的位置和准心位置之间的距离小于 100 时，将被判定为误点
		}

		if (mCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR)
		{
			Plant* aCobcannon = mPlants.DataArrayTryToGet(mCursorObject->mCobCannonPlantID);
			if (aCobcannon)
			{
				aCobcannon->CobCannonFire(x, y);
			}
		}
	}
	ClearCursor();
}

//0x40FD30
void Board::MouseDownWithPlant(int x, int y, int theClickCount)
{
	// 右击鼠标：放下卡牌
	if (theClickCount < 0)
	{
		RefreshSeedPacketFromCursor();
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		return;
	}

	// 我是僵尸模式中，交由 Challenge 处理
	if (mApp->IsIZombieLevel())
	{
		mChallenge->IZombieMouseDownWithZombie(x, y, theClickCount);
		mPlayers[0]->mSeedBankIndex = -1;
		return;
	}

	SeedType aPlantingSeedType = GetSeedTypeInCursor();
	int aGridX = PlantingPixelToGridX(x, y, aPlantingSeedType);
	int aGridY = PlantingPixelToGridY(x, y, aPlantingSeedType);

	// 不在场地内的点击：放下卡牌
	if (aGridX < 0 || aGridX >= MAX_GRID_SIZE_X || aGridY < 0 || aGridY > MAX_GRID_SIZE_Y)
	{
		RefreshSeedPacketFromCursor();
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		return;
	}

	PlantingReason aReason = CanPlantAt(aGridX, aGridY, aPlantingSeedType);

	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && aGridX > 4 && aPlantingSeedType != SEED_CHERRYHOVERBOMB) {
		aReason = PLANTING_NOT_PASSED_LINE;
	}

	if (aReason != PlantingReason::PLANTING_OK)
	{
		// 根据不同的种植原因播放相应的提示字幕
		if (aReason == PlantingReason::PLANTING_ONLY_ON_GRAVES)
		{
			DisplayAdvice(_S("[ADVICE_GRAVEBUSTERS_ON_GRAVES]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_GRAVEBUSTERS_ON_GRAVES);
		}
		else if (aPlantingSeedType == SeedType::SEED_LILYPAD)
		{
			if (aReason == PlantingReason::PLANTING_ONLY_IN_POOL)
			{
				DisplayAdvice(_S("[ADVICE_LILYPAD_ON_WATER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_LILYPAD_ON_WATER);
			}
		}
		else if (aPlantingSeedType == SeedType::SEED_TANGLEKELP)
		{
			if (aReason == PlantingReason::PLANTING_ONLY_IN_POOL)
			{
				DisplayAdvice(_S("[ADVICE_TANGLEKELP_ON_WATER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_TANGLEKELP_ON_WATER);
			}
		}
		else if (aPlantingSeedType == SeedType::SEED_SEASHROOM)
		{
			if (aReason == PlantingReason::PLANTING_ONLY_IN_POOL)
			{
				DisplayAdvice(_S("[ADVICE_SEASHROOM_ON_WATER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_SEASHROOM_ON_WATER);
			}
		}
		else if (aReason == PlantingReason::PLANTING_ONLY_ON_GROUND)
		{
			DisplayAdvice(_S("[ADVICE_POTATO_MINE_ON_LILY]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_POTATOE_MINE_ON_LILY);
		}
		else if (aReason == PlantingReason::PLANTING_NOT_PASSED_LINE)
		{
			if (mApp->IsWallnutBowlingLevel())
				DisplayAdvice(_S("[ADVICE_NOT_PASSED_LINE]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NOT_PASSED_LINE);
			else
				DisplayAdvice("Place your plant to the left of the line", MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NOT_PASSED_LINE);
		}
		else if (aReason == PlantingReason::PLANTING_NEEDS_UPGRADE)
		{
			switch (aPlantingSeedType)
			{
			case SeedType::SEED_GATLINGPEA:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_REPEATERS]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_REPEATERS);
				break;

			case SeedType::SEED_PLASMAWOOD:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_TORCHWOOD]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_REPEATERS);
				break;

			case SeedType::SEED_TWINSUNFLOWER:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_SUNFLOWER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_SUNFLOWER);
				break;

			case SeedType::SEED_GLOOMSHROOM:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_FUMESHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_FUMESHROOM);
				break;

			case SeedType::SEED_CATTAIL:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_LILYPAD]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_LILYPAD);
				break;

			case SeedType::SEED_WINTERMELON:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_MELONPULT]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_MELONPULT);
				break;

			case SeedType::SEED_GOLD_MAGNET:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_MAGNETSHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_MAGNETSHROOM);
				break;

			case SeedType::SEED_SPIKEROCK:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_SPIKEWEED]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_SPIKEWEED);
				break;

			case SeedType::SEED_COBCANNON:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_KERNELPULT]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT);
				break;

			case SeedType::SEED_SHOOTINGSTAR:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_STARFRUIT]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT);
				break;

			case SeedType::SEED_SUPERCHOMPER:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_CHOMPER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT);
				break;

			case SeedType::SEED_BRAVEYSHROOM:
				DisplayAdvice(_S("[ADVICE_ONLY_ON_SCAREDY_SHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT);
				break;
			}
		}
		else if (aReason == PlantingReason::PLANTING_NOT_ON_ART)
		{
			SexyString aSeedName = Plant::GetNameString(mChallenge->GetArtChallengeSeed(aGridX, aGridY), SeedType::SEED_NONE);
			SexyString aMessage = TodReplaceString(_S("[ADVICE_WRONG_ART_TYPE]"), _S("{SEED}"), aSeedName);
			DisplayAdvice(aMessage, MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_WRONG_ART_TYPE);
		}
		else if (aReason == PlantingReason::PLANTING_NEEDS_POT)
		{
			if (mApp->IsFirstTimeAdventureMode() && mLevel == 41)
			{
				DisplayAdvice(_S("[ADVICE_PLANT_NEED_POT1]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NEED_POT);
			}
			else
			{
				DisplayAdvice(_S("[ADVICE_PLANT_NEED_POT2]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NEED_POT);
			}
		}
		else if (aReason == PlantingReason::PLANTING_NOT_ON_GRAVE)
		{
			DisplayAdvice(_S("[ADVICE_PLANT_NOT_ON_GRAVE]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NOT_ON_GRAVE);
		}
		else if (aReason == PlantingReason::PLANTING_NOT_ON_CRATER)
		{
			if (IsPoolSquare(aGridX, aGridY))
			{
				DisplayAdvice(_S("[ADVICE_CANT_PLANT_THERE]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_CANT_PLANT_THERE);
			}
			else
			{
				DisplayAdvice(_S("[ADVICE_PLANT_NOT_ON_CRATER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NOT_ON_CRATER);
			}
		}
		else if (aReason == PlantingReason::PLANTING_NOT_ON_WATER)
		{
			if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mZenGarden->mGardenType == GardenType::GARDEN_AQUARIUM)
			{
				DisplayAdvice(_S("[ZEN_ONLY_AQUATIC_PLANTS]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_NONE);
			}
			else if (aPlantingSeedType == SeedType::SEED_POTATOMINE)
			{
				DisplayAdvice(_S("[ADVICE_POTATO_MINE_ON_LILY]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_POTATOE_MINE_ON_LILY);
			}
			else
			{
				DisplayAdvice(_S("[ADVICE_PLANT_NOT_ON_WATER]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANT_NOT_ON_WATER);
			}
		}
		else if (aReason == PlantingReason::PLANTING_NEEDS_GROUND)
		{
			DisplayAdvice(_S("[ADVICE_PLANTING_NEEDS_GROUND]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANTING_NEEDS_GROUND);
		}
		else if (aReason == PlantingReason::PLANTING_NEEDS_SLEEPING)
		{
			DisplayAdvice(_S("[ADVICE_PLANTING_NEED_SLEEPING]"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_PLANTING_NEED_SLEEPING);
		}

		// 特定情况下，放下原有手持的植物
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE || mApp->IsWhackAZombieLevel())
		{
			RefreshSeedPacketFromCursor();
			mApp->PlayFoley(FoleyType::FOLEY_DROP);
		}
		// 不可种植的情况至此结束，直接跳转至返回
		return;
	}
	
	/* 以下为植物类型可以种植的情况 */
	// 清除种植相关的提示字幕
	ClearAdvice(AdviceType::ADVICE_PLANTING_NEED_SLEEPING);
	ClearAdvice(AdviceType::ADVICE_CANT_PLANT_THERE);
	ClearAdvice(AdviceType::ADVICE_PLANTING_NEEDS_GROUND);
	ClearAdvice(AdviceType::ADVICE_PLANT_NOT_ON_WATER);
	ClearAdvice(AdviceType::ADVICE_PLANT_NOT_ON_CRATER);
	ClearAdvice(AdviceType::ADVICE_PLANT_NOT_ON_GRAVE);
	ClearAdvice(AdviceType::ADVICE_PLANT_NEED_POT);
	ClearAdvice(AdviceType::ADVICE_PLANT_WRONG_ART_TYPE);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_LILYPAD);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_MAGNETSHROOM);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_FUMESHROOM);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_SUNFLOWER);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_SPIKEWEED);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_MELONPULT);
	ClearAdvice(AdviceType::ADVICE_PLANT_ONLY_ON_REPEATERS);
	ClearAdvice(AdviceType::ADVICE_PLANT_NOT_PASSED_LINE);
	ClearAdvice(AdviceType::ADVICE_PLANT_GRAVEBUSTERS_ON_GRAVES);
	ClearAdvice(AdviceType::ADVICE_PLANT_LILYPAD_ON_WATER);
	ClearAdvice(AdviceType::ADVICE_PLANT_TANGLEKELP_ON_WATER);
	ClearAdvice(AdviceType::ADVICE_PLANT_SEASHROOM_ON_WATER);
	ClearAdvice(AdviceType::ADVICE_PLANT_POTATOE_MINE_ON_LILY);
	ClearAdvice(AdviceType::ADVICE_SURVIVE_FLAGS);

	// 无免费种植、非传送带关卡的卡槽植物，判断阳光是否充足：充足则扣除阳光，不足则退出
	if (!mApp->mEasyPlantingCheat && (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK || mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR) && !HasConveyorBeltSeedBank())
	{
		if (!TakeSunMoney(GetCurrentPlantCost(aPlantingSeedType, SeedType::SEED_NONE)))
		{
			return;
		}
	}
	
	// 升级种植或坚果包扎术等情况时，先将原植物销毁
	bool aIsAwake = false;
	int aWakeUpCounter = 0;
	PlantsOnLawn aPlantOnLawn;
	GetPlantsOnLawn(aGridX, aGridY, &aPlantOnLawn);
	Plant* aNormalPlant = aPlantOnLawn.mNormalPlant;
	Plant* aPumpkinPlant = aPlantOnLawn.mPumpkinPlant;

	if (aNormalPlant && aNormalPlant->IsUpgradableTo(aPlantingSeedType))
	{
		if (aPlantingSeedType == SeedType::SEED_GLOOMSHROOM || aPlantingSeedType == SeedType::SEED_BRAVEYSHROOM)
		{
			aIsAwake = !aNormalPlant->mIsAsleep;
			aWakeUpCounter = aNormalPlant->mWakeUpCounter;
		}
		aNormalPlant->Die();
	}
	if ((aPlantingSeedType == SeedType::SEED_WALLNUT || aPlantingSeedType == SeedType::SEED_TALLNUT || aPlantingSeedType == SeedType::SEED_UMBRELLA) && aNormalPlant)
	{
		if (aNormalPlant->mSeedType == aPlantingSeedType)
		{
			aNormalPlant->Die();
		}
	}
	if (aPlantingSeedType == SeedType::SEED_PUMPKINSHELL && aPumpkinPlant)
	{
		if (aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
		{
			aPumpkinPlant->Die();
		}
	}
	if (aPlantingSeedType == SeedType::SEED_COBCANNON)
	{
		Plant* aRightPlant = GetTopPlantAt(aGridX + 1, aGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
		if (aRightPlant)
		{
			aRightPlant->Die();
		}
	}
	if (aPlantingSeedType == SeedType::SEED_CATTAIL)
	{
		if (aPlantOnLawn.mPadPlant)
		{
			aPlantOnLawn.mPadPlant->Die();
		}
		if (aNormalPlant)
		{
			aNormalPlant->Die();
		}
	}

	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE){
		if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mApp->mGameScene == SCENE_PLAYING)
		{
			MovePlant(mPlants.DataArrayTryToGet(mCursorObject->mGlovePlantID), aGridX, aGridY);
		}
		else
		{
			mApp->mZenGarden->MovePlant(mPlants.DataArrayTryToGet(mCursorObject->mGlovePlantID), aGridX, aGridY);
		}
	}
	else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW){
		mApp->mZenGarden->MouseDownWithFullWheelBarrow(x, y);
	}
	else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN){
		AddPlant(aGridX, aGridY, mCursorObject->mType, mCursorObject->mImitaterType);
		
		Coin* aCoin = mCoins.DataArrayTryToGet(mCursorObject->mCoinID);
		mCursorObject->mCoinID = CoinID::COINID_NULL;
		aCoin->Die();
	}
	else if (IsPlantInCursor()){
		Plant* aPlant = AddPlant(aGridX, aGridY, mCursorObject->mType, mCursorObject->mImitaterType);

		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK)
			mPlayers[0]->mSeedBankIndex = -1;

		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR) {
			Plant* aPlant = mPlants.DataArrayTryToGet((unsigned int)mCursorObject->mRootPlantID);
			if (aPlant) {
				aPlant->mRootTime = 0;
				aPlant->mRootMaxTime = Plant::GetRefreshTime(mCursorObject->mType, mCursorObject->mImitaterType) + Rand(300) + 750;
				if (Rand(20) == 0)  // 早爆的概率
				{
					aPlant->mRootMaxTime *= 1.5f;
				}
				aPlant->mState = STATE_SEEDROOT_DIVING;
				aPlant->PlayBodyReanim("anim_dive", REANIM_PLAY_ONCE_AND_HOLD, 20, 20);
			}
		}
		
		if (aIsAwake)
		{
			aPlant->SetSleeping(false);
		}
		else
		{
			aPlant->mWakeUpCounter = aWakeUpCounter;
		}

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_FLOOR_IS_LAVA)
		{
			if (GetFlyingPotAt(aGridX, aGridY) == nullptr && aPlant->mSeedType != SEED_FLYINGPOT && aPlant->mSeedType != SEED_CHERRYHOVERBOMB && !Plant::IsFlying(aPlant->mSeedType))
			{
				float aPosX = aPlant->mX + 40.0f;
				float aPosY = GetPosYBasedOnRow(aPosX + 10.0f, aGridY) - 10.0f;
				Reanimation* aFwoosh = mApp->AddReanimation(aPosX, aPosY, MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, aGridY, 1), ReanimationType::REANIM_JALAPENO_FIRE);
				aFwoosh->mAnimRate = 24;
				float aScale = RandRangeFloat(0.9f, 1.1f);
				float aFlip = Rand(2) ? 1.0f : -1.0f;
				aFwoosh->OverrideScale(aScale * aFlip, 1);
				aPlant->Die();
			}
		}

		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK){
			mSeedBank->mSeedPacketsTime[mCursorObject->mSeedBankIndex] = 0;
			mSeedBank->mSeedPackets[mCursorObject->mSeedBankIndex].Deactivate();
			mSeedBank->mSeedPackets[mCursorObject->mSeedBankIndex].WasPlanted();
		}
		else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR) {
			Plant* mPlant = mPlants.DataArrayTryToGet((unsigned int)mCursorObject->mRootPlantID);
			if (mPlant) {
				mPlant->mRootTime = 0;
			}
		}
	}
	else{
		TOD_ASSERT();
	}
	
	// 柱子关卡中，一列种植
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
	{
		for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
		{
			if (aRow == aGridY || CanPlantAt(aGridX, aRow, aPlantingSeedType) != PlantingReason::PLANTING_OK)
				continue;

			if (aPlantingSeedType == SeedType::SEED_WALLNUT || aPlantingSeedType == SeedType::SEED_TALLNUT)
			{
				aNormalPlant = GetTopPlantAt(aGridX, aRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
				if (aNormalPlant && aNormalPlant->mSeedType == aPlantingSeedType)
				{
					aNormalPlant->Die();
				}
			}
			if (aPlantingSeedType == SeedType::SEED_PUMPKINSHELL)
			{
				aPumpkinPlant = GetTopPlantAt(aGridX, aRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN);
				if (aPumpkinPlant && aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
				{
					aPumpkinPlant->Die();
				}
			}
			AddPlant(aGridX, aRow, mCursorObject->mType, mCursorObject->mImitaterType);
		}
	}

	// 设置教程状态相关
	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER)
	{
		SetTutorialState(mPlants.mSize >= 2 ? TutorialState::TUTORIAL_LEVEL_1_COMPLETED : TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER);
	}
	else if (mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PLANT_SUNFLOWER)
	{
		int aSunFlowersCount = CountSunFlowers();
		if (aPlantingSeedType == SeedType::SEED_SUNFLOWER && aSunFlowersCount == 2)
		{
			DisplayAdvice(_S("[ADVICE_MORE_SUNFLOWERS]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2, AdviceType::ADVICE_NONE);
			if (!mSeedBank->mSeedPackets[1].CanPickUp())
			{
				SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER);
			}
			else
			{
				SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
			}
		}
		else if (aSunFlowersCount >= 3)
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_COMPLETED);
		}
		else if (!mSeedBank->mSeedPackets[1].CanPickUp())
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER);
		}
		else
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
		}
	}
	else if (mTutorialState == TutorialState::TUTORIAL_MORESUN_PLANT_SUNFLOWER)
	{
		if (CountSunFlowers() >= 3)
		{
			SetTutorialState(TutorialState::TUTORIAL_MORESUN_COMPLETED);
			DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER5]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER, AdviceType::ADVICE_PLANT_SUNFLOWER5);
			mTutorialTimer = -1;
		}
		else if (!mSeedBank->mSeedPackets[1].CanPickUp())
		{
			SetTutorialState(TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER);
		}
		else
		{
			SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
		}
	}

	// 保龄球关卡，播放保龄球滚动的音效
	if (mApp->IsWallnutBowlingLevel())
	{
		mApp->PlaySample(Sexy::SOUND_BOWLING);
	}

	// 重置鼠标
	ClearCursor();
}

//0x410F70
Plant* Board::ToolHitTestHelper(HitResult* theHitResult)
{
	theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_PLANT;
	Plant* aPlant = (Plant*)theHitResult->mObject;
	return (aPlant->mSeedType != SeedType::SEED_GRAVEBUSTER || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) ? aPlant : nullptr;
}

Plant* Board::ToolHitTest(int theX, int theY)
{
	HitResult aHitResult;
	MouseHitTest(theX, theY, &aHitResult);
	if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_PLANT)
	{
		return ToolHitTestHelper(&aHitResult);
	}
	return nullptr;
}

//0x410FA0
void Board::TutorialArrowShow(int theX, int theY)
{
	TutorialArrowRemove();
	TodParticleSystem* aParticle = mApp->AddTodParticle(theX, theY, MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 0), ParticleEffect::PARTICLE_SEED_PACKET_PICK);
	mTutorialParticleID = mApp->ParticleGetID(aParticle);
}

//0x411020
void Board::TutorialArrowRemove()
{
	mApp->RemoveParticle(mTutorialParticleID);
	mTutorialParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
}

//0x411060
void Board::MouseDownWithTool(int x, int y, int theClickCount, CursorType theCursorType)
{
	if (theClickCount < 0)
	{
		ClearCursor();
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		return;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		mApp->mZenGarden->MouseDownWithTool(x, y, theCursorType);
		return;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		mChallenge->TreeOfWisdomTool(x, y);
		return;
	}

	Plant* aPlant = ToolHitTest(x, y);
	if (aPlant == nullptr)
	{
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
	}
	else if (theCursorType == CursorType::CURSOR_TYPE_SHOVEL)
	{
		mApp->PlayFoley(FoleyType::FOLEY_USE_SHOVEL);
		mPlantsShoveled++;
		aPlant->Die();

		//if (mApp->mGameMode != GAMEMODE_CHALLENGE_SQUIRREL)
		//	mPlayers[0]->mAbilityTime = mPlayers[0]->mAbilityMaxTime;

		if (aPlant->mSeedType == SeedType::SEED_CATTAIL && GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN))
		{
			NewPlant(aPlant->mPlantCol, aPlant->mRow, SeedType::SEED_LILYPAD, SeedType::SEED_NONE);
		}
		if (mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG || mTutorialState == TutorialState::TUTORIAL_SHOVEL_KEEP_DIGGING)
		{
			SetTutorialState(CountPlantByType(SeedType::SEED_PEASHOOTER) == 0 ? TutorialState::TUTORIAL_SHOVEL_COMPLETED : TutorialState::TUTORIAL_SHOVEL_KEEP_DIGGING);
		}
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE && mApp->mGameScene == SCENE_PLAYING)
	{
		Plant* heatPlant = ToolHitTest(x, y);
		if (heatPlant){
			mCursorObject->mType = heatPlant->mSeedType;
			mCursorObject->mImitaterType = heatPlant->mImitaterType;
			mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE;
			mCursorObject->mGlovePlantID = (PlantID)mPlants.DataArrayGetID(heatPlant);
			mIgnoreMouseUp = true;
			mApp->PlaySample(SOUND_TAP);
		}
		else {
			mApp->PlayFoley(FoleyType::FOLEY_DROP);
			ClearCursor();
		}
	}
	else {
		ClearCursor();
	}

	
}

//0x411280
Plant* Board::SpecialPlantHitTest(int x, int y)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
		{
			float aMinDist = GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION) ? 25 : 0;
			float aDistance = Distance2D(x, y, aPlant->mX + 40, aPlant->mY + 40);
			if (aDistance >= aMinDist && aDistance <= 50 && y > aPlant->mY + 25)
			{
				return aPlant;
			}
		}
		else if (Plant::IsFlying(aPlant->mSeedType))
		{
			if (Distance2D(x, y, aPlant->mX + 40, aPlant->mY) < 15)
			{
				return aPlant;
			}
		}
	}
	return nullptr;
}

//0x411470
bool Board::MouseHitTestPlant(int x, int y, HitResult* theHitResult)
{
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_COBCANNON_TARGET || mCursorObject->mCursorType == CursorType::CURSOR_TYPE_HAMMER)
		return false;

	Plant* aPlant;
	aPlant = SpecialPlantHitTest(x, y);
	if (aPlant)
	{
		theHitResult->mObject = aPlant;
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_PLANT;
		return true;
	}

	int aGridX = PixelToGridX(x, y);
	int aGridY = PixelToGridY(x, y);
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ZEN_TOOL_ORDER);
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && (!aPlant || !mApp->mZenGarden->PlantCanBeWatered(aPlant)))
		{
			Plant* aTopPlant = GetTopPlantAt(PixelToGridX(x - 30, y - 20), PixelToGridY(x - 30, y - 20), PlantPriority::TOPPLANT_ZEN_TOOL_ORDER);
			if (aTopPlant && mApp->mZenGarden->PlantCanBeWatered(aTopPlant))
			{
				aPlant = aTopPlant;
			}
		}
	}
	else
	{
		aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_DIGGING_ORDER);
		if (aPlant && aPlant->mSeedType == SeedType::SEED_LILYPAD)
		{
			if (GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ONLY_PUMPKIN))
			{
				return false;
			}
		}
	}

	// 植物不存在，或者手持巧克力但植物不需要巧克力时，返回“否”
	if (aPlant == nullptr)
	{
		return false;
	}
	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE && !mApp->mZenGarden->PlantCanHaveChocolate(aPlant))
	{
		theHitResult->mObject = nullptr;
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
		return false;
	}

	theHitResult->mObject = aPlant;
	theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_PLANT;
	return true;
}

//0x411730
bool Board::MouseHitTest(int x, int y, HitResult* theHitResult)
{
	if (mBoardFadeOutCounter >= 0 || IsScaryPotterDaveTalking())
	{
		theHitResult->mObject = nullptr;
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
		return false;
	}

	if (mMenuButton->IsMouseOver() && CanInteractWithBoardButtons()) //&& !mIsP1Keyboard
	{
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_MENU_BUTTON;
		return true;
	}
	if (mSpeedButton->IsMouseOver() && CanInteractWithBoardButtons())  //&& !mIsP1Keyboard
	{
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SPEED_BUTTON;
		return true;
	}
	else if (mStoreButton && mStoreButton->IsMouseOver() && CanInteractWithBoardButtons())  //&& !mIsP1Keyboard
	{
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_STORE_BUTTON;
		return true;
	}

	Rect aShovelButtonRect = GetShovelButtonRect();
	if (mSeedBank->MouseHitTest(x, y, theHitResult))
	{
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL ||
			mCursorObject->mCursorType == CursorType::CURSOR_TYPE_COBCANNON_TARGET ||
			mCursorObject->mCursorType == CursorType::CURSOR_TYPE_HAMMER)
			return true;
	}
	if (mShowShovel && aShovelButtonRect.Contains(x, y) && CanInteractWithBoardButtons())
	{
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SHOVEL;
		return true;
	}

	if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL || mCursorObject->mCursorType == CursorType::CURSOR_TYPE_HAMMER)
	{
		Coin* aCoin = nullptr;
		Coin* aTopCoin = nullptr;
		while (IterateCoins(aCoin))
		{
			HitResult aHitResultCoin;
			if (aCoin->MouseHitTest(x, y, &aHitResultCoin))
			{
				aCoin = (Coin*)aHitResultCoin.mObject;
				if (aTopCoin == nullptr || aCoin->mRenderOrder >= aTopCoin->mRenderOrder)
				{
					theHitResult->mObjectType = aHitResultCoin.mObjectType;
					theHitResult->mObject = aCoin;
					aTopCoin = aCoin;
				}
			}
		}
		if (aTopCoin)
		{
			return true;
		}
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		bool canClick = false;
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE && !mApp->mZenGarden->IsStinkyHighOnChocolate())
		{
			canClick = true;
		}
		else if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL && mApp->mZenGarden->IsStinkySleeping())
		{
			canClick = true;
		}

		GridItem* aStinky = mApp->mZenGarden->GetStinky();
		if (canClick && aStinky)
		{
			Rect aStinkyRect(aStinky->mPosX - 6, aStinky->mPosY - 10, 84, 90);
			if (aStinkyRect.Contains(x, y))
			{
				theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_STINKY;
				return true;
			}
		}
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		if (mCursorObject->mCursorType == CursorType::CURSOR_TYPE_TREE_FOOD && mChallenge->TreeOfWisdomHitTest(x, y, theHitResult))
		{
			return true;
		}
	}
	if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) && CanInteractWithBoardButtons())
	{
		for (int i = (int)GameObjectType::OBJECT_TYPE_WATERING_CAN; i <= (int)GameObjectType::OBJECT_TYPE_NEXT_GARDEN; i++)
		{
			GameObjectType aTool = (GameObjectType)i;
			if (CanUseGameObject(aTool) && (aTool != GameObjectType::OBJECT_TYPE_TREE_FOOD || mChallenge->TreeOfWisdomCanFeed()))
			{
				Rect aButtonRect = GetShovelButtonRect();
				if (aTool == GameObjectType::OBJECT_TYPE_NEXT_GARDEN)
				{
					aButtonRect.mX = 564 - BOARD_ADDITIONAL_WIDTH;
					aButtonRect.mWidth -= 10;
				}
				else
				{
					GetZenButtonRect(aTool, aButtonRect);
				}

				if (aButtonRect.Contains(x, y))
				{
					theHitResult->mObjectType = (GameObjectType)aTool;
					return true;
				}
			}
		}
	}

	if (MouseHitTestPlant(x, y, theHitResult))
		return true;

	if (mApp->IsScaryPotterLevel() &&
		mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL &&
		mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_SCARY_POTTER_MALLETING &&
		mApp->mGameScene == GameScenes::SCENE_PLAYING &&
		mApp->GetDialog(Dialogs::DIALOG_GAME_OVER) == nullptr &&
		mApp->GetDialog(Dialogs::DIALOG_CONTINUE) == nullptr)
	{
		GridItem* aScaryPot = GetGridItemAt(GridItemType::GRIDITEM_SCARY_POT, PixelToGridX(x, y), PixelToGridY(x, y));
		if (aScaryPot)
		{
			theHitResult->mObject = aScaryPot;
			theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SCARY_POT;
			return true;
		}
	}

	if (mApp->IsSlotMachineLevel() && !mPaused)
	{
		Rect aSlotMachineHandleRect = mChallenge->SlotMachineGetHandleRect();
		if (aSlotMachineHandleRect.Contains(x, y) && mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && !HasLevelAwardDropped())
		{
			theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SLOT_MACHINE_HANDLE;
			return true;
		}
	}

	theHitResult->mObject = nullptr;
	theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
	return false;
}


//0x411C10
void Board::PickUpTool(GameObjectType theObjectType)
{
	if (mPaused || (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mCutScene->IsInShovelTutorial()))
		return;

	switch (theObjectType)
	{
	case GameObjectType::OBJECT_TYPE_SHOVEL:
		if (mPlayers[0]->mAbilityTime > 0)		return;
		if (mTutorialState == TutorialState::TUTORIAL_SHOVEL_PICKUP)
		{
			SetTutorialState(TutorialState::TUTORIAL_SHOVEL_DIG);
		}
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
		mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
		break;

	case GameObjectType::OBJECT_TYPE_WATERING_CAN:
		if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_PICKUP_WATER)
		{
			mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_WATER_PLANT;
			DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_WATER_PLANT]"), MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG, AdviceType::ADVICE_NONE);
			TutorialArrowRemove();
		}
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_WATERING_CAN;
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		break;

	case GameObjectType::OBJECT_TYPE_FERTILIZER:
		if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] > PURCHASE_COUNT_OFFSET)
		{
			mCursorObject->mCursorType = CursorType::CURSOR_TYPE_FERTILIZER;
		}
		else
		{
			mApp->PlaySample(Sexy::SOUND_BUZZER);
		}
		break;

	case GameObjectType::OBJECT_TYPE_BUG_SPRAY:
		if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] > PURCHASE_COUNT_OFFSET)
		{
			mCursorObject->mCursorType = CursorType::CURSOR_TYPE_BUG_SPRAY;
		}
		else
		{
			mApp->PlaySample(Sexy::SOUND_BUZZER);
		}
		break;

	case GameObjectType::OBJECT_TYPE_PHONOGRAPH:
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PHONOGRAPH;
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		break;

	case GameObjectType::OBJECT_TYPE_CHOCOLATE:
		if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] > PURCHASE_COUNT_OFFSET)
		{
			mCursorObject->mCursorType = CursorType::CURSOR_TYPE_CHOCOLATE;
		}
		else
		{
			mApp->PlaySample(Sexy::SOUND_BUZZER);
		}
		break;

	case GameObjectType::OBJECT_TYPE_GLOVE:
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_GLOVE;
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		break;

	case GameObjectType::OBJECT_TYPE_MONEY_SIGN:
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_MONEY_SIGN;
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		break;

	case GameObjectType::OBJECT_TYPE_WHEELBARROW:
		mCursorObject->mCursorType = CursorType::CURSOR_TYPE_WHEEELBARROW;
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		break;

	case GameObjectType::OBJECT_TYPE_TREE_FOOD:
		if (mChallenge->TreeOfWisdomCanFeed())
		{
			if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_FOOD] > PURCHASE_COUNT_OFFSET)
			{
				mCursorObject->mCursorType = CursorType::CURSOR_TYPE_TREE_FOOD;
			}
			else
			{
				mApp->PlaySample(Sexy::SOUND_BUZZER);
			}
		}
		break;

	default:
		TOD_ASSERT();
		break;
	}

	mCursorObject->mType = SeedType::SEED_NONE;
}

//0x411F20
void Board::MouseDown(int x, int y, int theClickCount)
{
	Widget::MouseDown(x, y, theClickCount);
	mIgnoreMouseUp = !CanInteractWithBoardButtons();

	if (mTimeStopCounter > 0)
		return;

	HitResult aHitResult;
	MouseHitTest(x, y, &aHitResult);

	if (mChallenge->MouseDown(x, y, theClickCount, &aHitResult))
		return;

	if (mMenuButton->IsMouseOver() && CanInteractWithBoardButtons() && theClickCount > 0){
		mApp->PlaySample(Sexy::SOUND_GRAVEBUTTON);
	}
	else if (mStoreButton && mStoreButton->IsMouseOver() && CanInteractWithBoardButtons() && theClickCount > 0){
		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM){
			mApp->PlaySample(Sexy::SOUND_TAP);
		}
		else if (LawnApp::IsLastStandLevel(mApp->mGameMode) || mApp->mGameMode == GameMode::GAMEMODE_UPSELL){
			mApp->PlaySample(Sexy::SOUND_GRAVEBUTTON);
		}
		else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
			mApp->PlaySample(Sexy::SOUND_CRAZYDAVECRAZY);
			int plantToBoost = RandRangeInt(1, TodAnimateCurve(0, 10, mCurrentWave, 1, 3, CURVE_LINEAR));
			vector<Plant*> aPlantVect;

			Plant* aPlant = nullptr;
			while (IteratePlants(aPlant)) {
				if (aPlant->mSeedType == SEED_PEASHOOTER)
					aPlantVect.insert(aPlantVect.begin(), aPlant);
			}

			for (int i = 0; i < plantToBoost && !aPlantVect.empty(); i++) {
				int index = RandRangeInt(0, aPlantVect.size() - 1);
				aPlantVect[index]->mBerserkCounter = RandRangeInt(900, 1100);
				mApp->PlaySample(Sexy::SOUND_COFFEE);
				aPlantVect.erase(aPlantVect.begin() + index);
			}
		}
	}

	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && mApp->mSeedChooserScreen){
		mApp->mSeedChooserScreen->CancelLawnView();
	}
	if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON){
		mCutScene->ZombieWonClick();
		return;
	}
	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO){
		mCutScene->MouseDown(x, y);
	}
	
	if (mApp->mTodCheatKeys && !mApp->IsScaryPotterLevel() && mNextSurvivalStageCounter > 0){
		mNextSurvivalStageCounter = 2;
		for (int i = 0; i < MAX_GRID_SIZE_Y; i++)
		{
			if (mIceTimer[i] > 2)
			{
				mIceTimer[i] = 2;
			}
		}
	}

	CursorType aCursor = mCursorObject->mCursorType;
	if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_NONE){
		if (aCursor == CURSOR_TYPE_COBCANNON_TARGET)
		{
			MouseDownCobcannonFire(x, y, theClickCount);
			UpdateCursor();
			return;
		}
	}
	else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_COIN && theClickCount >= 0){
		Coin* aCoin = (Coin*)aHitResult.mObject;
		if (aCoin->mBoard){
			aCoin->MouseDown(x, y, theClickCount);
		}
		UpdateCursor();
		return;
	}

	Plant* mPlant = (Plant*)aHitResult.mObject;

	if (mPlant && mPlant->mSeedType == SEED_SEEDROOT && mPlant->mState == STATE_READY){
		if (IsPlantInCursor() && TakeSunMoney(Plant::GetCost(mCursorObject->mType))) {
			mPlayers[0]->mSeedBankIndex = -1;
			
			mPlant->mRootSeedType = mCursorObject->mType;
			mPlant->mRootImitaterType = mCursorObject->mImitaterType;
			mPlant->mRootTime = 0;
			mPlant->mRootMaxTime = Plant::GetRefreshTime(mCursorObject->mType, mCursorObject->mImitaterType) + Rand(300) + 750;
			if (Rand(20) == 0)  // 早爆的概率
			{
				mPlant->mRootMaxTime *= 1.5f;
			}
			mPlant->mState = STATE_SEEDROOT_DIVING;
			mPlant->PlayBodyReanim("anim_dive", REANIM_PLAY_ONCE_AND_HOLD, 20, 20);

			mSeedBank->mSeedPacketsTime[mCursorObject->mSeedBankIndex] = 0;
			mSeedBank->mSeedPackets[mCursorObject->mSeedBankIndex].Deactivate();
			mSeedBank->mSeedPackets[mCursorObject->mSeedBankIndex].WasPlanted();
			ClearCursor();
			return;
		}
	}

	if (aCursor == CursorType::CURSOR_TYPE_SHOVEL ||
		aCursor == CursorType::CURSOR_TYPE_WATERING_CAN ||
		aCursor == CursorType::CURSOR_TYPE_FERTILIZER ||
		aCursor == CursorType::CURSOR_TYPE_BUG_SPRAY ||
		aCursor == CursorType::CURSOR_TYPE_PHONOGRAPH ||
		aCursor == CursorType::CURSOR_TYPE_CHOCOLATE ||
		aCursor == CursorType::CURSOR_TYPE_GLOVE ||
		aCursor == CursorType::CURSOR_TYPE_MONEY_SIGN ||
		aCursor == CursorType::CURSOR_TYPE_WHEEELBARROW ||
		aCursor == CursorType::CURSOR_TYPE_TREE_FOOD){
		MouseDownWithTool(x, y, theClickCount, aCursor);
	}
	else if (IsPlantInCursor()){
		MouseDownWithPlant(x, y, theClickCount);
	}
	else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_SEEDPACKET){
		if (!mPaused){
			((SeedPacket*)aHitResult.mObject)->MouseDown(x, y, theClickCount);
		}
	}
	else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_NEXT_GARDEN){
		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN){
			mApp->mZenGarden->GotoNextGarden();
		}
		else if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM){
			mChallenge->TreeOfWisdomNextGarden();
		}
		mApp->PlaySample(Sexy::SOUND_TAP);
	}
	else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_SHOVEL ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WATERING_CAN ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_FERTILIZER ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_BUG_SPRAY ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_PHONOGRAPH ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_CHOCOLATE ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_GLOVE ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_MONEY_SIGN ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_WHEELBARROW ||
		aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_TREE_FOOD){
		PickUpTool(aHitResult.mObjectType);
	}
	else if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_PLANT){
		((Plant*)aHitResult.mObject)->MouseDown(x, y, theClickCount);
	}
	
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BUTTERED_POPCORN && mApp->mGameScene == SCENE_PLAYING){
		Zombie* aZombie = nullptr;
		Zombie* aTopZombie = nullptr;
		while (IterateZombies(aZombie)){
			if (!aZombie->IsDeadOrDying() && !aZombie->mCannotBeHammered && aZombie->mButteredCounter == 0){
				Rect aZombieRect = aZombie->GetZombieRect();
				if (GetCircleRectOverlap(x, y, 45, aZombieRect)){
					if (aTopZombie == nullptr || aZombie->mRenderOrder >= aTopZombie->mRenderOrder){
						aTopZombie = aZombie;
					}
				}
			}
		}

		if (aTopZombie){
			mApp->PlayFoley(FOLEY_BUTTER);
			aTopZombie->ApplyButter();
		}
	}

	UpdateCursor();
}

//0x412330
void Board::ClearCursor()
{
	if (mAdvice->mDuration > 0)
	{
		if (mHelpIndex == AdviceType::ADVICE_PLANT_GRAVEBUSTERS_ON_GRAVES ||
			mHelpIndex == AdviceType::ADVICE_PLANT_LILYPAD_ON_WATER ||
			mHelpIndex == AdviceType::ADVICE_PLANT_TANGLEKELP_ON_WATER ||
			mHelpIndex == AdviceType::ADVICE_PLANT_SEASHROOM_ON_WATER ||
			mHelpIndex == AdviceType::ADVICE_PLANT_POTATOE_MINE_ON_LILY ||
			mHelpIndex == AdviceType::ADVICE_PLANT_WRONG_ART_TYPE ||
			mHelpIndex == AdviceType::ADVICE_PLANT_NEED_POT ||
			mHelpIndex == AdviceType::ADVICE_PLANT_NOT_PASSED_LINE ||
			mHelpIndex == AdviceType::ADVICE_PLANT_ONLY_ON_REPEATERS ||
			mHelpIndex == AdviceType::ADVICE_PLANT_ONLY_ON_MELONPULT ||
			mHelpIndex == AdviceType::ADVICE_PLANT_ONLY_ON_SUNFLOWER ||
			mHelpIndex == AdviceType::ADVICE_PLANT_ONLY_ON_SPIKEWEED ||
			mHelpIndex == AdviceType::ADVICE_PLANT_ONLY_ON_KERNELPULT)
		{
			ClearAdvice(mHelpIndex);
		}
	}

	mCursorObject->mType = SeedType::SEED_NONE;
	mCursorObject->mCursorType = CursorType::CURSOR_TYPE_NORMAL;
	mCursorObject->mSeedBankIndex = -1;
	mCursorObject->mCoinID = CoinID::COINID_NULL;
	mCursorObject->mDuplicatorPlantID = PlantID::PLANTID_NULL;
	mCursorObject->mCobCannonPlantID = PlantID::PLANTID_NULL;
	mCursorObject->mGlovePlantID = PlantID::PLANTID_NULL;
	mCursorObject->mRootPlantID = PlantID::PLANTID_NULL;
	mApp->SetCursor(CURSOR_POINTER);
	mChallenge->ClearCursor();

	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER)
	{
		SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER);
	}
	else if (mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PLANT_SUNFLOWER || mTutorialState == TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER)
	{
		if (!mSeedBank->mSeedPackets[1].CanPickUp())
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER);
		}
		else
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
		}
	}
	else if (mTutorialState == TutorialState::TUTORIAL_MORESUN_PLANT_SUNFLOWER || mTutorialState == TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER)
	{
		if (!mSeedBank->mSeedPackets[1].CanPickUp())
		{
			SetTutorialState(TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER);
		}
		else
		{
			SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
		}
	}
	else if (mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG)
	{
		SetTutorialState(TutorialState::TUTORIAL_SHOVEL_PICKUP);
	}
}

//0x412490
bool Board::CanInteractWithBoardButtons()
{
	if (mPaused || mApp->GetDialogCount() > 0)
		return false;

	if (mCursorObject->mCursorType != CursorType::CURSOR_TYPE_NORMAL && 
		mCursorObject->mCursorType != CursorType::CURSOR_TYPE_HAMMER &&
		mCursorObject->mCursorType != CursorType::CURSOR_TYPE_COBCANNON_TARGET)
		return false;

	if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_ZEN_FADING)
		return false;

	return mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mCrazyDaveState == CrazyDaveState::CRAZY_DAVE_OFF;
}

//0x412540
void Board::MouseUp(int x, int y, int theClickCount)
{
	Widget::MouseUp(x, y, theClickCount);
	if (mIgnoreMouseUp)
	{
		mIgnoreMouseUp = false;
		return;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED && mChallenge->MouseUp(x, y) && theClickCount > 0)
		return;

	if (CanInteractWithBoardButtons() && theClickCount > 0)
	{
		if (mMenuButton->IsMouseOver() && !mApp->GetDialog(Dialogs::DIALOG_GAME_OVER) && !mApp->GetDialog(Dialogs::DIALOG_LEVEL_COMPLETE))
		{
			mMenuButton->mIsOver = false;
			mMenuButton->mIsDown = false;
			UpdateCursor();
			ClearCursor();
			if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_COMPLETED)
			{
				mApp->FinishZenGardenToturial();
			}
			else if (mApp->mGameMode != GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GAMEMODE_TREE_OF_WISDOM &&  mApp->mGameMode != GAMEMODE_UPSELL)
			{
				mApp->PlaySample(Sexy::SOUND_PAUSE);
				mApp->DoNewOptions(false);
			}
			else
			{
				mIsSpeeding = false;
				mApp->mBoardResult = BoardResult::BOARDRESULT_QUIT;
				mApp->DoBackToMain();
			}
		}
		else if (mSpeedButton && mSpeedButton->IsMouseOver())
		{
			mIsSpeeding = !mIsSpeeding;
			if (mIsSpeeding)
				mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
			mSpeedButton->mButtonImage = mIsSpeeding ? IMAGE_SPEEDBUTTON_ACTIVATED : IMAGE_SPEEDBUTTON;
			UpdateCursor();
			ClearCursor();
		}
		else if(mStoreButton && mStoreButton->IsMouseOver())
		{
			if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
			{
				ClearAdviceImmediately();
				mApp->mZenGarden->OpenStore();
			}
			else if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
			{
				mChallenge->TreeOfWisdomOpenStore();
			}
			else if (LawnApp::IsLastStandLevel(mApp->mGameMode))
			{
				mChallenge->mChallengeState = ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT;
				mStoreButton->mBtnNoDraw = true;
				mStoreButton->mDisabled = true;
				mZombieCountDown = 10;
				mZombieCountDownStart = 10;
				mSpeedButton->mBtnNoDraw = false;
				mSpeedButton->mButtonImage = IMAGE_SPEEDBUTTON;
			}
			else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
				mChallenge->mChallengeStateCounter = RandRangeInt(-314, 314);
				mStoreButton->mBtnNoDraw = true;
				mStoreButton->mDisabled = true;
			}
			else if (mApp->mGameMode == GameMode::GAMEMODE_UPSELL)
			{
				if (mApp->mDRM)
				{
					mApp->mDRM->BuyGame();
				}
				mApp->DoBackToMain();
			}
		}
	}
}

void Board::ShowCoinBank(int theDuration)
{
	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		return;

	mCoinBankFadeCount = theDuration;
}

//0x4127A0
void Board::Pause(bool thePause)
{
	if (mPaused == thePause)
		return;

	mPaused = thePause;

	if (thePause && mApp->mPlayerInfo->mCoins > 0)
	{
		ShowCoinBank();
	}

	if (!thePause || mApp->mGameScene != GameScenes::SCENE_LEVEL_INTRO)
	{
		mApp->mSoundSystem->GamePause(thePause);
		mApp->mMusic->GameMusicPause(thePause);
	}

	if (mApp->mGameScene == SCENE_PLAYING && mApp->mGameMode != GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GAMEMODE_TREE_OF_WISDOM && mApp->mBoardResult != BOARDRESULT_WON && mApp->mGameScene != BOARDRESULT_LOST) {
		if (mApp->mGameMode != GameMode::GAMEMODE_ADVENTURE) {
			mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName), thePause ? "Paused" : "Playing");
		}
		else
		{
			int aStage = ClampInt((mLevel - 1) / 10 + 1, 1, 6);
			int aSub = mLevel - (aStage - 1) * 10;
			std::string Details = "Adventure " + std::to_string(aStage) + "-" + std::to_string(aSub);
			const char* DetailsChar = Details.c_str();
			mApp->UpdateDiscordRPC(DetailsChar, thePause ? "Paused" : "Playing");
		}
	}
}

//0x412850
int Board::GetGraveStonesCount()
{
	int aCount = 0;

	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE)
		{
			aCount++;
		}
	}

	return aCount;
}

//0x412890
void Board::PickSpecialGraveStone()
{
	GridItem* aGridItem = nullptr;
	GridItem* aPicks[MAX_GRAVE_STONES];
	int aPickCount = 0;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE)
		{
			TOD_ASSERT(aPickCount < MAX_GRAVE_STONES);
			aPicks[aPickCount] = aGridItem;
			aPickCount++;
		}
	}

	if (aPickCount > 0)
	{
		((GridItem*)TodPickFromArray((int*)aPicks, aPickCount))->mGridItemState = GridItemState::GRIDITEM_STATE_GRAVESTONE_SPECIAL;
	}
}

//0x4128F0
void Board::SpawnZombiesFromPool()
{
	if (mIceTrapCounter > 0)
		return;
	
	int aCount, aZombiePoints;
	if (mLevel == 21 || mLevel == 22 || mLevel == 31 || mLevel == 32)
	{
		aCount = 2;
		aZombiePoints = 3;
	}
	else if (mLevel == 23 || mLevel == 24 || mLevel == 25 || mLevel == 33 || mLevel == 34 || mLevel == 35)
	{
		aCount = 3;
		aZombiePoints = 5;
	}
	else
	{
		aCount = 3;
		aZombiePoints = 7;
	}
	
	int aGridArrayCount = 0;
	TodWeightedGridArray aGridArray[MAX_POOL_GRID_SIZE];
	for (int aGridX = 5; aGridX < MAX_GRID_SIZE_X; aGridX++)
	{
		for (int aGridY = 2; aGridY <= 3; aGridY++)
		{
			aGridArray[aGridArrayCount].mX = aGridX;
			aGridArray[aGridArrayCount].mY = aGridY;
			aGridArray[aGridArrayCount].mWeight = 10000;
			aGridArrayCount++;
			TOD_ASSERT(aGridArrayCount <= MAX_POOL_GRID_SIZE);
		}
	}

	if (aGridArrayCount < 0)
	{
		aGridArrayCount = 0;
	}
	for (int i = 0; i < aCount; i++)
	{
		TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(aGridArray, aGridArrayCount);
		aGrid->mWeight = 0;

		ZombieType aZombieType = PickGraveRisingZombieType(aZombiePoints);
		if (aZombieType == ZOMBIE_INVALID)
		{
			return;
		}

		Zombie* aZombie = AddZombieInRow(aZombieType, aGrid->mY, mCurrentWave);
		if (aZombie == nullptr)
		{
			return;
		}

		aZombie->RiseFromGrave(aGrid->mX, aGrid->mY);
		aZombiePoints -= GetZombieDefinition(aZombieType).mZombieValue;
		if (aZombiePoints < 1)
		{
			aZombiePoints = 1;
		}
	}
}

//0x412A90
void Board::SetupBungeeDrop(BungeeDropGrid* theBungeeDropGrid)
{
	theBungeeDropGrid->mGridArrayCount = 0;
	for (int aGridX = 4; aGridX < MAX_GRID_SIZE_X; aGridX++)
	{
		for (int aGridY = 0; aGridY <= 4; aGridY++)
		{
			int aCount = theBungeeDropGrid->mGridArrayCount;
			theBungeeDropGrid->mGridArray[aCount].mX = aGridX;
			theBungeeDropGrid->mGridArray[aCount].mY = aGridY;
			theBungeeDropGrid->mGridArray[aCount].mWeight = 10000;
			theBungeeDropGrid->mGridArrayCount++;
			TOD_ASSERT(theBungeeDropGrid->mGridArrayCount <= LENGTH(theBungeeDropGrid->mGridArray));
		}
	}
}

//0x412B60
void Board::BungeeDropZombie(BungeeDropGrid* theBungeeDropGrid, ZombieType theZombieType)
{
	TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(theBungeeDropGrid->mGridArray, theBungeeDropGrid->mGridArrayCount);
	aGrid->mWeight = 1;

	Zombie* aBungeeZombie = AddZombie(ZombieType::ZOMBIE_BUNGEE, mCurrentWave);
	Zombie* aZombie = AddZombie(theZombieType, mCurrentWave);

	if (aZombie)
	{
		aBungeeZombie->mCannotBeHammered = aZombie->mCannotBeHammered = true;
		TOD_ASSERT(aBungeeZombie && aZombie);

		aBungeeZombie->BungeeDropZombie(aZombie, aGrid->mX, aGrid->mY);
	}
}

Plant* Board::FindPlanternPlant(int theGridX, int theGridY, int range)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_PLANTERN && !aPlant->NotOnGround() && GridInRange(theGridX, theGridY, aPlant->mPlantCol, aPlant->mRow, range, 1))
		{
			return aPlant;
		}
	}
	return nullptr;
}

//0x412C30
void Board::SpawnZombiesFromSky()
{
	if (mIceTrapCounter > 0)
		return;

	int aCount, aZombiePoints;
	if (mLevel == 41 || mLevel == 42)
	{
		aCount = 2;
		aZombiePoints = 3;
	}
	else if (mLevel == 43 || mLevel == 44 || mLevel == 45)
	{
		aCount = 3;
		aZombiePoints = 5;
	}
	else
	{
		aCount = 3;
		aZombiePoints = 7;
	}
	
	BungeeDropGrid aBungeeDropGrid;
	SetupBungeeDrop(&aBungeeDropGrid);

	if (aCount > aBungeeDropGrid.mGridArrayCount)
	{
		aCount = aBungeeDropGrid.mGridArrayCount;
	}

	if (aBungeeDropGrid.mGridArrayCount == 0 || aCount <= 0)
		return;

	for (int i = 0; i < aCount; i++)
	{
		ZombieType aZombieType = PickGraveRisingZombieType(aZombiePoints);
		if (aZombieType == ZOMBIE_INVALID)
		{
			return;
		}

		BungeeDropZombie(&aBungeeDropGrid, aZombieType);
		aZombiePoints -= GetZombieDefinition(aZombieType).mZombieValue;
		if (aZombiePoints < 1)
		{
			aZombiePoints = 1;
		}
	}
}

//0x412CE0
void Board::SpawnZombiesFromGraves()
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2)
		return;

	if (StageHasRoof())
	{
		SpawnZombiesFromSky();
	}
	else if (StageHasPool())
	{
		SpawnZombiesFromPool();
		return;
	}
	
	int aZombiePoints = GetGraveStonesCount();
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType != GridItemType::GRIDITEM_GRAVESTONE || aGridItem->mGridItemCounter < 100)
		{
			continue;
		}

		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVE_DANGER && Rand(mNumWaves) > mCurrentWave)
		{
			continue;
		}
		
		ZombieType aZombieType = PickGraveRisingZombieType(aZombiePoints);
		Zombie* aZombie = AddZombie(aZombieType, mCurrentWave, true);
		if (aZombie == nullptr)
		{
			return;
		}

		aZombie->RiseFromGrave(aGridItem->mGridX, aGridItem->mGridY);
		aZombiePoints -= GetZombieDefinition(aZombieType).mZombieValue;
		if (aZombieType < 1)
		{
			aZombiePoints = 1;
		}
	}
}

//0x412E30
int Board::TotalZombiesHealthInWave(int theWaveIndex)
{
	int aTotalHealth = 0;
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mFromWave == theWaveIndex && !aZombie->mMindControlled && !aZombie->IsDeadOrDying() &&
			aZombie->mZombieType != ZombieType::ZOMBIE_BUNGEE && aZombie->mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
		{
			aTotalHealth += aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth * 0.2f + aZombie->mFlyingHealth;
		}
	}
	return aTotalHealth;
}

//0x412EE0
void Board::SpawnZombieWave()
{
	mChallenge->SpawnZombieWave();
	if (mApp->IsBungeeBlitzLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2)
	{
		BungeeDropGrid aBungeeDropGrid;
		SetupBungeeDrop(&aBungeeDropGrid);

		for (int i = 0; i < MAX_ZOMBIES_IN_WAVE; i++)
		{
			ZombieType aZombieType = mZombiesInWave[mCurrentWave][i];
			if (aZombieType == ZombieType::ZOMBIE_INVALID)
				break;
			
			if (aZombieType == ZombieType::ZOMBIE_BUNGEE || aZombieType == ZombieType::ZOMBIE_ZAMBONI){
				AddZombie(aZombieType, mCurrentWave);
			}
			else
			{
				if (mApp->mGameMode != GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2){
					BungeeDropZombie(&aBungeeDropGrid, aZombieType);
				}
				else {
					bool aIsFinalWave = mCurrentWave == mNumWaves;
					float aMaxSpeed = TodAnimateCurve(1, 12, mCurrentWave, 1, 3, CURVE_EASE_IN);

					TodWeightedGridArray* aGrid = TodPickFromWeightedGridArray(aBungeeDropGrid.mGridArray, aBungeeDropGrid.mGridArrayCount);
					aGrid->mWeight = 1;

					if (aIsFinalWave)
					{
						aZombieType = Rand(2) == 0 ? ZOMBIE_TRAFFIC_CONE : ZOMBIE_PAIL;
						aMaxSpeed = 2;
					}

					Zombie* aBungeeZombie = AddZombie(ZombieType::ZOMBIE_BUNGEE, mCurrentWave);
					Zombie* aZombie = AddZombie(aZombieType, mCurrentWave);

					if (aBungeeZombie == nullptr || aZombie == nullptr)
						break;

					aBungeeZombie->mCannotBeHammered = aZombie->mCannotBeHammered = true;
					aBungeeZombie->BungeeDropZombie(aZombie, aGrid->mX, aGrid->mY);
					aZombie->mPostDropVelocityX = RandRangeFloat(0.5f, aMaxSpeed);
					aZombie->mDropVelocityChange = true;
				}
			}
		}
	}
	else
	{
		TOD_ASSERT(mCurrentWave >= 0 && mCurrentWave < MAX_ZOMBIE_WAVES && mCurrentWave < mNumWaves);
		for (int i = 0; i < MAX_ZOMBIES_IN_WAVE; i++)
		{
			ZombieType aZombieType = mZombiesInWave[mCurrentWave][i];
			if (aZombieType == ZombieType::ZOMBIE_INVALID)
				break;

			if (aZombieType == ZombieType::ZOMBIE_BOBSLED && !CanAddBobSled() && mZombieAllowed[ZOMBIE_NORMAL])
			{
				for (int i = 0; i < MAX_ZOMBIE_FOLLOWERS; i++)
				{
					AddZombie(ZombieType::ZOMBIE_NORMAL, mCurrentWave, StageHasRoof());  // 生成 4 只普通僵尸以代替雪橇僵尸小队
				}
			}
			else
			{

				if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
				{
					Zombie* aBoss = GetBossZombie();
					if (aBoss && !aBoss->IsDeadOrDying())
					{
						AddZombie(aZombieType, mCurrentWave, StageHasRoof());
					}
				}
				else
				{
					AddZombie(aZombieType, mCurrentWave, StageHasRoof());
				}
			}
		}
	}

	if (mCurrentWave == mNumWaves - 1 && !mApp->IsContinuousChallenge())
	{
		mRiseFromGraveCounter = 210;
	}

	if (IsFlagWave(mCurrentWave))
	{
		mFlagRaiseCounter = FLAG_RAISE_TIME;
	}
	
	mCurrentWave++;
	mTotalSpawnedWaves++;
}

//0x4130D0
void Board::UpdateGameObjects()
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		aPlant->Update();
	}

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		aZombie->Update();
	}

	Bush* aBush = nullptr;
	while (IterateBushes(aBush) && mApp->Is3dAccel())
	{
		aBush->Update();
	}

	Projectile* aProjectile = nullptr;
	while (IterateProjectiles(aProjectile))
	{
		aProjectile->Update();
	}

	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		aCoin->Update();
	}

	LawnMower* aLawnMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		aLawnMower->Update();
	}

	mCursorPreview->Update();
	mCursorObject->Update();

	for (int i = 0; i < mSeedBank->mNumPackets; i++)
	{
		mSeedBank->mSeedPackets[i].Update();
	}

	for (int i = 0; i < mSeedBankP2->mNumPackets; i++)
	{
		mSeedBankP2->mSeedPackets[i].Update();
	}
}

//0x413220
void Board::StopAllZombieSounds()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		aZombie->StopZombieSound();
	}
}

//0x413260
int Board::GetSurvivalFlagsCompleted()
{
	int aWavesPerFlag = GetNumWavesPerFlag();
	int aFlagsCompleted = mChallenge->mSurvivalStage * GetNumWavesPerSurvivalStage() / aWavesPerFlag;
	int aCurrentWave = mCurrentWave;
	if (IsFlagWave(aCurrentWave - 1) && mBoardFadeOutCounter < 0 && !mNextSurvivalStageCounter)
	{
		aCurrentWave -= 1;
	}
	return aCurrentWave / aWavesPerFlag + aFlagsCompleted;
}

//0x413320
void Board::SurvivalSaveScore()
{
	if (!mApp->IsSurvivalMode())
		return;

	int aFlagsCompleted = GetSurvivalFlagsCompleted();
	int& aFlagsRecord = (mIsVeteran ?
		mApp->mPlayerInfo->mChallengeVeteranRecords[mApp->GetCurrentChallengeIndex()] :
		mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()]);

	if (aFlagsCompleted > aFlagsRecord)
	{
		aFlagsRecord = aFlagsCompleted;
		mApp->WriteCurrentUserConfig();
	}
}

//0x413390
void Board::PuzzleSaveStreak()
{
	if (!mApp->IsEndlessIZombie(mApp->mGameMode) && !mApp->IsEndlessScaryPotter(mApp->mGameMode))
		return;

	int aStreak = mChallenge->mSurvivalStage + 1;
	int& aRecord = mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()];
	if (aStreak > aRecord)
	{
		aRecord = aStreak;
		mApp->WriteCurrentUserConfig();
	}
}

void Board::HideSpeedButton() {
	mIsSpeeding = false;
	mSpeedButton->mBtnNoDraw = true;
	mSpeedButton->mButtonImage = IMAGE_SPEEDBUTTON;
}

//0x413400
void Board::ZombiesWon(Zombie* theZombie)
{
	if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
		return;

	ClearAdvice(AdviceType::ADVICE_NONE);
	mApp->mBoardResult = BoardResult::BOARDRESULT_LOST;

	HideSpeedButton();

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie == theZombie)
			continue;

		if (aZombie->GetZombieRect().mX < -50 ||
			aZombie->mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE ||
			aZombie->mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
		{
			if ((aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) &&
				aZombie->IsDeadOrDying() && aZombie->mPosX < 140)
			{
				aZombie->DieNoLoot();
			}
		}
	}

	SurvivalSaveScore();

	SexyString aGameOverMsg;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
	{
		aGameOverMsg = _S("[ZOMBIQUARIUM_DEATH_MESSAGE]");
	}
	else if (LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		SexyString aFlagStr = mApp->Pluralize(GetSurvivalFlagsCompleted(), _S("[ONE_FLAG]"), _S("[COUNT_FLAGS]"));
		aGameOverMsg = TodReplaceString(_S("[LAST_STAND_DEATH_MESSAGE]"), _S("{FLAGS}"), aFlagStr);
	}
	else if (mApp->IsEndlessIZombie(mApp->mGameMode) || mApp->IsEndlessScaryPotter(mApp->mGameMode))
	{
		aGameOverMsg = TodReplaceNumberString(_S("[ENDLESS_PUZZLE_DEATH_MESSAGE]"), _S("{STREAK}"), mChallenge->mSurvivalStage);
	}
	else if (mApp->IsIZombieLevel())
	{
		aGameOverMsg = _S("[I_ZOMBIE_DEATH_MESSAGE]");
	}
	else if ((mApp->mGameMode == GAMEMODE_CHALLENGE_OPEN_FIRE || mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID))
	{
		if (mMaxPlayers > 1){
			aGameOverMsg = "A Shooter died!";
		}
		else{
			aGameOverMsg = "Your Shooter died!";
		}
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_NONE_SHALL_PASS)
	{
		aGameOverMsg = "A zombie has passed the line!";
	}
	else
	{

		mApp->mGameScene = GameScenes::SCENE_ZOMBIES_WON;
		if (theZombie)  // 原版此处没有对 theZombie 进行空指针判断，但加上判断后便允许绕过僵尸而直接调用游戏失败
		{
			theZombie->WalkIntoHouse();
			theZombie->mGameOverStartX = theZombie->mX;
			theZombie->mGameOverStartY = theZombie->mY;
		}

		ClearAdvice(AdviceType::ADVICE_NONE);
		mCutScene->StartZombiesWon();
		FreezeEffectsForCutscene(true);
		TutorialArrowRemove();
		UpdateCursor();

		/*if (mApp->mGameMode != GAMEMODE_ADVENTURE) {
			mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName), "Game Over");
		}
		else {
			int aStage = ClampInt((mLevel - 1) / 10 + 1, 1, 6);
			int aSub = mLevel - (aStage - 1) * 10;
			std::string Details = "Adventure " + std::to_string(aStage) + "-" + std::to_string(aSub);
			const char* DetailsChar = Details.c_str();
			mApp->UpdateDiscordRPC(DetailsChar, "Game Over");
		}*/
		
		return;
	}

	GameOverDialog* aGameOverDialog = new GameOverDialog(aGameOverMsg, true);
	mApp->AddDialog(Dialogs::DIALOG_GAME_OVER, aGameOverDialog);

	mApp->mMusic->StopAllMusic();
	StopAllZombieSounds();
	mApp->PlaySample(Sexy::SOUND_LOSEMUSIC);

	ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIES_WON, true);
	Reanimation* aReanim = mApp->AddReanimation(-BOARD_OFFSET_X, 0, MakeRenderOrder(RenderLayer::RENDER_LAYER_SCREEN_FADE, 0, 0), ReanimationType::REANIM_ZOMBIES_WON);
	aReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
	aReanim->GetTrackInstanceByName("fullscreen")->mTrackColor = Color::Black;
	aReanim->SetFramesForLayer("anim_screen");

	//mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName) + (string)" - Game Over", aGameOverMsg);
}

void Board::UpdateGameRPC(SexyString name, SexyString info) {
	if (mApp->mGameMode != GameMode::GAMEMODE_ADVENTURE) {
		mApp->UpdateDiscordRPC(TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName), "Won");
	}
	else
	{
		int aStage = ClampInt((mLevel - 1) / 10 + 1, 1, 6);
		int aSub = mLevel - (aStage - 1) * 10;
		std::string Details = "Adventure " + std::to_string(aStage) + "-" + std::to_string(aSub) + name;
		const char* DetailsChar = Details.c_str();
		mApp->UpdateDiscordRPC(DetailsChar, info);
	}
}

//0x4138D0
bool Board::IsFinalScaryPotterStage()
{
	if (!mApp->IsScaryPotterLevel())
		return false;

	if (mApp->IsAdventureMode())
	{
		return mChallenge->mSurvivalStage == 2;
	}
	
	return !mApp->IsEndlessScaryPotter(mApp->mGameMode);
}

//0x413920
bool Board::IsFinalSurvivalStage()
{
	if (!mApp->IsSurvivalMode())
		return false;

	int aFlags = GetNumWavesPerSurvivalStage() * (mChallenge->mSurvivalStage + 1) / GetNumWavesPerFlag();
	if (mApp->IsSurvivalNormal(mApp->mGameMode))
	{
		return aFlags >= 5;
	}
	if (mApp->IsSurvivalHard(mApp->mGameMode))
	{
		return aFlags >= 10;
	}

	return false;
}

bool Board::IsLastStandFinalStage()
{
	return LawnApp::IsLastStandLevel(mApp->mGameMode) && mChallenge->mSurvivalStage == LAST_STAND_FLAGS - 1;
}

//0x4139E0
bool Board::IsSurvivalStageWithRepick()
{
	return mApp->IsSurvivalMode() && !IsFinalSurvivalStage();
}

//0x413A10
bool Board::IsLastStandStageWithRepick()
{
	return LawnApp::IsLastStandLevel(mApp->mGameMode) && !IsLastStandFinalStage();
}

//0x413A40
bool Board::HasLevelAwardDropped()
{
	return mLevelAwardSpawned || mNextSurvivalStageCounter > 0 || mBoardFadeOutCounter >= 0;
}

//0x413A70
void Board::UpdateSunSpawning()
{
	if (StageIsNight() || 
		HasLevelAwardDropped() || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAT_WAVE ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL || 
		mApp->mGameMode == GameMode::GAMEMODE_UPSELL ||
		mApp->mGameMode == GameMode::GAMEMODE_INTRO || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN ||
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || 
		LawnApp::IsLastStandLevel(mApp->mGameMode) || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE ||
		mApp->IsIZombieLevel() ||
		mApp->IsScaryPotterLevel() || 
		mApp->IsSquirrelLevel() || 
		HasConveyorBeltSeedBank() || 
		mTutorialState == TutorialState::TUTORIAL_SLOT_MACHINE_PULL)
		return;

	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER || mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER)
	{
		if (mPlants.mSize == 0)
		{
			return;
		}
	}

	mSunCountDown--;
	if (mSunCountDown != 0)
		return;

	mNumSunsFallen++;
	mSunCountDown = min(SUN_COUNTDOWN_MAX, SUN_COUNTDOWN + mNumSunsFallen * 10) + Rand(SUN_COUNTDOWN_RANGE);

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY) mSunCountDown += 8000;

	CoinType aSunType = mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY ? CoinType::COIN_LARGESUN : CoinType::COIN_SUN;
	AddCoin(RandRangeInt(100, 649 ), 60, aSunType, CoinMotion::COIN_MOTION_FROM_SKY);
}

//0x413C00
void Board::NextWaveComing()
{
	if (mCurrentWave + 1 == mNumWaves)
	{
		if (!IsSurvivalStageWithRepick() && !LawnApp::IsLastStandLevel(mApp->mGameMode) && !mApp->IsContinuousChallenge())
		{
			mApp->AddReanimation(0, 0, MakeRenderOrder(RenderLayer::RENDER_LAYER_ABOVE_UI, 0, 0), ReanimationType::REANIM_FINAL_WAVE);
			mFinalWaveSoundCounter = 60;
		}
	}

	if (mCurrentWave == 0)
	{
		mApp->PlaySample(Sexy::SOUND_AWOOGA);
	}
	else if ((mApp->IsWhackAZombieLevel() && mCurrentWave == mNumWaves - 1) || IsFlagWave(mCurrentWave))
	{
		mApp->PlaySample(Sexy::SOUND_SIREN);
	}
}

//0x413D00
void Board::UpdateZombieSpawning()
{
	if (mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO)
		return;

	if (mFinalWaveSoundCounter > 0)
	{
		mFinalWaveSoundCounter--;
		if (mFinalWaveSoundCounter == 0)
		{
			mApp->PlaySample(Sexy::SOUND_FINALWAVE);
		}
	}

	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER || 
		mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER ||
		mTutorialState == TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER || 
		mTutorialState == TutorialState::TUTORIAL_SLOT_MACHINE_PULL)
		return;

	if (HasLevelAwardDropped())
		return;

	if (mRiseFromGraveCounter > 0)
	{
		mRiseFromGraveCounter--;
		if (mRiseFromGraveCounter == 0)
		{
			SpawnZombiesFromGraves();
		}
	}

	if (mHugeWaveCountDown > 0)
	{
		mHugeWaveCountDown--;
		if (mHugeWaveCountDown == 0)
		{
			ClearAdvice(AdviceType::ADVICE_HUGE_WAVE);
			NextWaveComing();
			mZombieCountDown = 1;
		}
		else
		{
			if (mHugeWaveCountDown == 725)
			{
				mApp->PlaySample(Sexy::SOUND_HUGE_WAVE);
			}
			else
			{
				if (mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK || 
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES || 
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST ||
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF ||
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_GRASS_THE_MOON ||
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_WOODY_TOMBSTONE ||
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_LUNACY_CADABREATH || 
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_BRAIN_STORM ||
					mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_GLOOMBLOOM)
				{
					if (mHugeWaveCountDown == 400)
					{
						mApp->mMusic->StartBurst();
					}

					if (StageHasDarkness())
					{
						if (mHugeWaveCountDown <= 400)
						{

							int targetValue = (int)TodAnimateCurveFloat(400, 0, mHugeWaveCountDown, 0, 550, CURVE_LINEAR);
							if (mDarknessCounter < targetValue)
								mDarknessCounter = targetValue;
						}
					}
				}
				else if (mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS )
				{
					if (mHugeWaveCountDown == 700)
					{
						mApp->mMusic->StartBurst();
					}

					if (StageHasDarkness())
					{
						if (mHugeWaveCountDown <= 700)
						{

							int targetValue = (int)TodAnimateCurveFloat(700, 0, mHugeWaveCountDown, 0, 550, CURVE_LINEAR);
							if (mDarknessCounter < targetValue)
								mDarknessCounter = targetValue;
						}
					}
				}
				return;
			}
		}
	}

	if (mChallenge->UpdateZombieSpawning())
		return;

	if (mCurrentWave == mNumWaves)
	{
		if (IsFinalSurvivalStage())
		{
			return;
		}
		if (LawnApp::IsLastStandLevel(mApp->mGameMode))
		{
			return;
		}
		if (!mApp->IsSurvivalMode() && !mApp->IsContinuousChallenge())
		{
			return;
		}
	}

	mZombieCountDown--;

	if (mCurrentWave == mNumWaves && mApp->IsSurvivalMode())
	{
		if (mZombieCountDown == 0)
		{
			FadeOutLevel();
		}
		return;
	}

	if (mZombieCountDown > 200 && mZombieCountDownStart - mZombieCountDown > 400 && TotalZombiesHealthInWave(mCurrentWave - 1) <= mZombieHealthToNextWave)
	{
		mZombieCountDown = 200;
	}
	if (mZombieCountDown == 5)
	{
		if (IsFlagWave(mCurrentWave))
		{
			ClearAdviceImmediately();
			DisplayAdviceAgain(_S("[ADVICE_HUGE_WAVE]"), MessageStyle::MESSAGE_STYLE_HUGE_WAVE, AdviceType::ADVICE_HUGE_WAVE);
			mHugeWaveCountDown = 750;
			return;
		}
		NextWaveComing();
	}
	if (mZombieCountDown == 0)
	{
		SpawnZombieWave();
		mZombieHealthWaveStart = TotalZombiesHealthInWave(mCurrentWave - 1);

		if (mCurrentWave == mNumWaves && mApp->IsSurvivalMode())
		{
			mZombieHealthToNextWave = 0;
			mZombieCountDown = ZOMBIE_COUNTDOWN_BEFORE_REPICK + 1;
		}
		else if (IsFlagWave(mCurrentWave) && (mApp->IsWallnutBowlingLevel() || LawnApp::IsLastStandLevel(mApp->mGameMode)))
		{
			mZombieHealthToNextWave = 0;
			mZombieCountDown = ZOMBIE_COUNTDOWN_BEFORE_FLAG;
		}
		else
		{
			mZombieHealthToNextWave = RandRangeFloat(0.5f, 0.65f) * mZombieHealthWaveStart;
			if (mApp->IsLittleTroubleLevel() || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN || 
				LawnApp::IsLastStandLevel(mApp->mGameMode)){
				mZombieCountDown = 750;
			}
			else{
				mZombieCountDown = ZOMBIE_COUNTDOWN + Rand(ZOMBIE_COUNTDOWN_RANGE);

				if (mIsVeteran)
				{
					mZombieCountDown /= GetVeteranMultiplier();
				}
			}
		}

		mZombieCountDownStart = mZombieCountDown;
	}
}

float Board::GetVeteranMultiplier(){
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_COLUMN || mApp->IsWallnutBowlingLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_EVERYONE_IS_DEAD)
	{
		return 1;
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE)
	{
		return 1.25f;
	}
	else if (mApp->IsChallengeWithoutSeedBank())
	{
		return 1.5f;
	}
	return 2;
}

//0x414100
void Board::UpdateIce()
{
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
	{
		if (mIceTimer[aRow])
		{
			mIceTimer[aRow]--;
			TodParticleSystem* aParticleIce = mApp->ParticleTryToGet(mIceParticleID[aRow]);
			if (mIceTimer[aRow] == 0)
			{
				mIceMinX[aRow] = BOARD_ICE_START;
				if (aParticleIce)
				{
					aParticleIce->ParticleSystemDie();
				}
			}
			else
			{
				float aPosX = mIceMinX[aRow];
				float aPosY = GridToPixelY(8, aRow);
				if (aParticleIce)
				{
					aParticleIce->SystemMove(aPosX, aPosY);
				}
				else
				{
					int aRenderPosition = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, aRow, 3);
					aParticleIce = mApp->AddTodParticle(aPosX - BOARD_ADDITIONAL_WIDTH, aPosY, aRenderPosition, ParticleEffect::PARTICLE_ICE_SPARKLE);
					mIceParticleID[aRow] = mApp->ParticleGetID(aParticleIce);
				}
			}

			int anAlpha = ClampInt(mIceTimer[aRow] / 10, 0, 255);
			aParticleIce->OverrideColor(nullptr, Color(255, 255, 255, anAlpha));
		}
	}
}

//0x414270
void Board::UpdateProgressMeter()
{
	if (mApp->IsFinalBossLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID || mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
	{
		Zombie* aBoss = GetBossZombie();
		if (aBoss && !aBoss->IsDeadOrDying())
		{
			mProgressMeterWidth = 150 * (aBoss->mBodyMaxHealth - aBoss->mBodyHealth) / aBoss->mBodyMaxHealth;
		}
		else
		{
			mProgressMeterWidth = 150;
		}
	}
	else if (mCurrentWave != 0)
	{
		// 更新旗帜升起倒计时
		if (mFlagRaiseCounter > 0)
			mFlagRaiseCounter--;

		int aTotalWidth = 150;  // 可用于平均分配给每一小波的进度条总长度
		int aNumWavesPerFlag = GetNumWavesPerFlag();  // 本关卡中每相邻两个旗帜波之前的小波数量
		bool aHasFlags = ProgressMeterHasFlags();  // 进度条标注旗帜时，旗帜波占用更长的进度条
		if (aHasFlags)
		{
			aTotalWidth -= 12 * mNumWaves / aNumWavesPerFlag;  // 从每个旗帜波分割出 12 单位的长度
		}

		int aWaveLength = aTotalWidth / (mNumWaves - 1);  // 每一小波占用的进度条长度
		int aCurrentWaveLength = (mCurrentWave - 1) * aTotalWidth / (mNumWaves - 1);  // 当前波开始时的进度条长度
		int aNextWaveLength = mCurrentWave * aTotalWidth / (mNumWaves - 1);  // 下一波开始时的进度条长度
		if (aHasFlags)
		{
			int anExtraLength = mCurrentWave / aNumWavesPerFlag * 12;  // 归还已刷新的旗帜波分割的长度
			aCurrentWaveLength += anExtraLength;
			aNextWaveLength += anExtraLength;
		}

		// 根据倒计时初步计算当前波已经过的比例
		float aFraction = (mZombieCountDownStart - mZombieCountDown) / (float)mZombieCountDownStart;
		if (mZombieHealthToNextWave != -1)
		{
			// 取得本波僵尸的当前血量
			int aHealthCurrent = TotalZombiesHealthInWave(mCurrentWave - 1);
			// 取得（本波开始时的僵尸总血量 - 下一波刷新时的僵尸总血量），即：本波刷新需要对僵尸造成的伤害
			int aDamageTarget = mZombieHealthWaveStart - mZombieHealthToNextWave;  //开始时的血量 - 刷新时的血量
			if (aDamageTarget < 1)
			{
				aDamageTarget = 1;  // 需要的伤害至少为 1
			}
			// 再次以刷新血量计算一次当前波已经过的比例
			// 血量比例 = [目标伤害 - (当前血量 - 刷新血量)] / 目标伤害 = (目标伤害 - 仍需造成的伤害) / 目标伤害 = 当前伤害 / 目标伤害
			float aHealthFraction = (aDamageTarget - aHealthCurrent + mZombieHealthToNextWave) / (float)aDamageTarget;
			// 最终比例取上述二者的较大值
			aFraction = max(aHealthFraction, aFraction);
		}

		// 计算当前应当的进度条长度，并将长度的范围限定在 [1, 150] 之间
		int aLength = ClampInt(aCurrentWaveLength + FloatRoundToInt((aNextWaveLength - aCurrentWaveLength) * aFraction), 1, 150);
		// 取得当前实际与理论的进度条长度之差
		int aDelta = aLength - mProgressMeterWidth;
		// 当差值不超过一波的长度时，每 20cs 调整一次长度；否则，每 5cs 调整一次长度
		if ((aDelta > aWaveLength && (mMainCounter % 5 == 0)) || (aDelta > 0 && (mMainCounter % 20 == 0)))
		{
			mProgressMeterWidth++;
		}
	}
}

//0x4144E0
void Board::UpdateTutorial()
{
	if (mTutorialTimer > 0)
		mTutorialTimer--;

	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER && mTutorialTimer == 0)
	{
		DisplayAdvice(_S("[ADVICE_CLICK_PEASHOOTER]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_NONE);
		TutorialArrowShow(mSeedBank->mX + mSeedBank->mSeedPackets[0].mX, mSeedBank->mY + mSeedBank->mSeedPackets[0].mY);
		mTutorialTimer = -1;
	}
	else if (mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER || 
		mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PLANT_SUNFLOWER ||
		mTutorialState == TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER)
	{
		if (mTutorialTimer == 0)
		{
			DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER2]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2, AdviceType::ADVICE_NONE);
			mTutorialTimer = -1;
		}
		else if (mZombieCountDown == 750 && mCurrentWave == 0)
		{
			DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER3]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2, AdviceType::ADVICE_NONE);
		}
	}
	else if (mTutorialState == TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER || 
		mTutorialState == TutorialState::TUTORIAL_MORESUN_PLANT_SUNFLOWER ||
		mTutorialState == TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER)
	{
		if (mTutorialTimer == 0)
		{
			DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER5]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER, AdviceType::ADVICE_PLANT_SUNFLOWER5);
			mTutorialTimer = -1;
		}
	}

	// 冒险模式初期关卡，检测到向日葵数量小于 3 时，进入“更多向日葵”的教程
	if (mApp->IsFirstTimeAdventureMode() && mLevel >= 3 && mLevel != 5 && mLevel <= 7 && mTutorialState == TutorialState::TUTORIAL_OFF &&
		mCurrentWave >= 5 && !gShownMoreSunTutorial && mSeedBank->mSeedPackets[1].CanPickUp() && CountPlantByType(SeedType::SEED_SUNFLOWER) < 3)
	{
		TOD_ASSERT(!ChooseSeedsOnCurrentLevel());
		DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER4]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER_STAY, AdviceType::ADVICE_NONE);
		gShownMoreSunTutorial = true;
		SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
		mTutorialTimer = 500;
	}
}

//0x414CB0
void Board::SetTutorialState(TutorialState theTutorialState)
{
	switch (theTutorialState)
	{
	case TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER:
		if (mPlants.mSize == 0)
		{
			float aPosX = mSeedBank->mX + mSeedBank->mSeedPackets[0].mX;
			float aPosY = mSeedBank->mY + mSeedBank->mSeedPackets[0].mY;
			TutorialArrowShow(aPosX, aPosY);
			DisplayAdvice(_S("[ADVICE_CLICK_SEED_PACKET]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_NONE);
		}
		else
		{
			DisplayAdvice(_S("[ADVICE_ENOUGH_SUN]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_NONE);
			mTutorialTimer = 400;
		}
		break;

	case TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER:
		mTutorialTimer = -1;
		TutorialArrowRemove();
		if (mPlants.mSize == 0)
		{
			DisplayAdvice(_S("[ADVICE_CLICK_ON_GRASS]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_NONE);
		}
		else
		{
			ClearAdvice(AdviceType::ADVICE_NONE);
		}
		break;

	case TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER:
		DisplayAdvice(_S("[ADVICE_PLANTED_PEASHOOTER]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, AdviceType::ADVICE_NONE);
		mSunCountDown = 400;
		break;

	case TutorialState::TUTORIAL_LEVEL_1_COMPLETED:
		DisplayAdvice(_S("[ADVICE_ZOMBIE_ONSLAUGHT]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1, AdviceType::ADVICE_NONE);
		mZombieCountDown = 99;
		mZombieCountDownStart = mZombieCountDown;
		break;

	case TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER:
	case TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER:
		TutorialArrowShow(mSeedBank->mX + mSeedBank->mSeedPackets[1].mX, mSeedBank->mY + mSeedBank->mSeedPackets[1].mY);
		break;

	case TutorialState::TUTORIAL_LEVEL_2_PLANT_SUNFLOWER:
	case TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER:
	case TutorialState::TUTORIAL_MORESUN_PLANT_SUNFLOWER:
	case TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER:
		TutorialArrowRemove();
		break;

	case TutorialState::TUTORIAL_LEVEL_2_COMPLETED:
		if (mCurrentWave == 0)
		{
			mZombieCountDown = 999;
			mZombieCountDownStart = mZombieCountDown;
		}
		break;

	case TutorialState::TUTORIAL_SLOT_MACHINE_PULL:
		DisplayAdvice(_S("[ADVICE_SLOT_MACHINE_PULL]"), MessageStyle::MESSAGE_STYLE_SLOT_MACHINE, AdviceType::ADVICE_SLOT_MACHINE_PULL);
		break;

	case TutorialState::TUTORIAL_SLOT_MACHINE_COMPLETED:
		ClearAdvice(AdviceType::ADVICE_SLOT_MACHINE_PULL);
		break;

	case TutorialState::TUTORIAL_SHOVEL_PICKUP:
	{
		DisplayAdvice(_S("[ADVICE_CLICK_SHOVEL]"), MessageStyle::MESSAGE_STYLE_HINT_STAY, AdviceType::ADVICE_NONE);
		Rect aShovelButtonRect = GetShovelButtonRect();
		int aPosX = aShovelButtonRect.mX + aShovelButtonRect.mWidth / 2 - 25;
		int aPosY = aShovelButtonRect.mY + aShovelButtonRect.mHeight - 65;
		TutorialArrowShow(aPosX, aPosY);
		break;
	}

	case TutorialState::TUTORIAL_SHOVEL_DIG:
		DisplayAdvice(_S("[ADVICE_CLICK_PLANT]"), MessageStyle::MESSAGE_STYLE_HINT_STAY, AdviceType::ADVICE_NONE);
		TutorialArrowRemove();
		break;

	case TutorialState::TUTORIAL_SHOVEL_KEEP_DIGGING:
		DisplayAdvice(_S("[ADVICE_KEEP_DIGGING]"), MessageStyle::MESSAGE_STYLE_HINT_STAY, AdviceType::ADVICE_NONE);
		break;

	case TutorialState::TUTORIAL_SHOVEL_COMPLETED:
		ClearAdvice(AdviceType::ADVICE_NONE);
		mCutScene->mCutsceneTime = 1500;
		mCutScene->mCrazyDaveDialogStart = 2410;
		break;
	}

	mTutorialState = theTutorialState;
}

//0x415920
void Board::UpdateGame()
{
	UpdateGameObjects();
	if (StageHasFog() && mFogBlownCountDown > 0)
	{
		float aMaxFogOffset = 1065.0f - LeftFogColumn() * 80.0f ;
		if (StageHasRoof())
			aMaxFogOffset -= BOARD_ADDITIONAL_WIDTH;
		if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
		{
			mFogOffset = TodAnimateCurveFloat(200, 0, mFogBlownCountDown, aMaxFogOffset + BOARD_ADDITIONAL_WIDTH, 0, TodCurves::CURVE_EASE_OUT);
		}
		else if (mFogBlownCountDown < 2000)
		{
			mFogOffset = TodAnimateCurveFloat(2000, 0, mFogBlownCountDown, aMaxFogOffset, 0, TodCurves::CURVE_EASE_OUT);
		}
		else if (mFogOffset < aMaxFogOffset)
		{
			mFogOffset = TodAnimateCurveFloat(-5, aMaxFogOffset, mFogOffset * 1.1f, 0, aMaxFogOffset, TodCurves::CURVE_LINEAR);
		}
	}

	if (mApp->mGameScene != GameScenes::SCENE_PLAYING /* && !mCutScene->ShouldRunUpsellBoard()*/)
		return;

	mMainCounter++;
	UpdateSunSpawning();
	UpdateZombieSpawning();
	UpdateIce();
	if (mIceTrapCounter > 0)
	{
		mIceTrapCounter--;
		if (mIceTrapCounter == 0)
		{
			TodParticleSystem* aPoolSparklyParticle = mApp->ParticleTryToGet(mPoolSparklyParticleID);
			if (aPoolSparklyParticle)
			{
				aPoolSparklyParticle->mDontUpdate = false;
			}

			TodParticleSystem* aPoolSparklyParticle2 = mApp->ParticleTryToGet(mPoolSparklyParticleIDExtra2);
			if (aPoolSparklyParticle2)
			{
				aPoolSparklyParticle2->mDontUpdate = false;
			}

			TodParticleSystem* aPoolSparklyParticle3 = mApp->ParticleTryToGet(mPoolSparklyParticleIDExtra3);
			if (aPoolSparklyParticle3)
			{
				aPoolSparklyParticle3->mDontUpdate = false;
			}
		}
	}

	if (mFogBlownCountDown > 0)
	{
		mFogBlownCountDown--;
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID) {;
		mChallenge->mAirRaidCLOUDTime = (mChallenge->mAirRaidCLOUDTime + 3845) % 3840;
		mChallenge->mAirRaidSTARTime = (mChallenge->mAirRaidSTARTime + 3841) % 3840;
	}


	if ((CountZombiesOnScreen() >= 10 || CountPlantByType(SEED_PLANTERN) < 2) && mDarknessCounter < 550)
	{
		mDarknessCounter++;
	}
	else if (CountZombiesOnScreen() < 4 && CountPlantByType(SEED_PLANTERN) >= 2 && mDarknessCounter > 0)
	{
		mDarknessCounter--;
	}

	if (mMainCounter == 1 && mApp->IsFirstTimeAdventureMode())
	{
		if (mLevel == 1)
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER);
		}
		else if (mLevel == 2)
		{
			SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
			DisplayAdvice(_S("[ADVICE_PLANT_SUNFLOWER1]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2, AdviceType::ADVICE_NONE);
			mTutorialTimer = 500;
		}
	}

	UpdateProgressMeter();
}

//0x415D40
void Board::Update()
{
	TodHesitationBracket aHesitation("Board::Update");

	Widget::Update();
	MarkDirty();

	mCutScene->Update();
	UpdateMousePosition();

	for (int i = 0; i < 0xFF; i++) {
		mJustPressedKeys[i] = mApp->mWidgetManager->mKeyDown[i] && !mPrevKeys[i];
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		mApp->mZenGarden->ZenGardenUpdate();

		/*Plant* aPlant = nullptr;
		while (IteratePlants(aPlant))
		{
			if (GetZenToolAt(aPlant->mPlantCol, aPlant->mRow) == nullptr && aPlant->mPottedPlantIndex >= 0)
			{
				PottedPlant* aPottedPlant = mApp->mZenGarden->PottedPlantFromIndex(aPlant->mPottedPlantIndex);
				PottedPlantNeed aNeed = mApp->mZenGarden->GetPlantsNeed(aPottedPlant);
				if (aNeed == PottedPlantNeed::PLANTNEED_WATER)
				{
					aPlant->mHighlighted = true;
					mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_WATERING_CAN);
					return;
				}
				else if (aNeed == PottedPlantNeed::PLANTNEED_FERTILIZER)
				{
					aPlant->mHighlighted = true;
					if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] <= PURCHASE_COUNT_OFFSET)
					{
						mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] = PURCHASE_COUNT_OFFSET + 1;
					}
					mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_FERTILIZER);
					return;
				}
				else if (aNeed == PottedPlantNeed::PLANTNEED_BUGSPRAY)
				{
					aPlant->mHighlighted = true;
					if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] <= PURCHASE_COUNT_OFFSET)
					{
						mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] = PURCHASE_COUNT_OFFSET + 1;
					}
					mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_BUG_SPRAY);
					return;
				}
				else if (aNeed == PottedPlantNeed::PLANTNEED_PHONOGRAPH)
				{
					aPlant->mHighlighted = true;
					mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_PHONOGRAPH);
					return;
				}
			}
		}*/
	}
	if (IsScaryPotterDaveTalking())
	{
		mApp->UpdateCrazyDave();
	}

	if (mPaused)
	{
		mChallenge->Update();
		mCursorPreview->mVisible = false;
		mCursorObject->mVisible = false;
		return;
	}
	
	if (mApp->mGameScene == SCENE_PLAYING && mApp->mBoardResult != BOARDRESULT_WON && mApp->mBoardResult != BOARDRESULT_LOST)
	{
		if (mJustPressedKeys[KeyCode::KEYCODE_BACK] && mApp->mGameMode != GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GAMEMODE_TREE_OF_WISDOM && mApp->mGameMode != GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		{
			mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
			mHeldSpeeding = mIsSpeeding = true;
			mSpeedButton->mButtonImage = mIsSpeeding ? IMAGE_SPEEDBUTTON_ACTIVATED : IMAGE_SPEEDBUTTON;
		}
	}

	if (mHeldSpeeding && !mApp->mWidgetManager->mKeyDown[KeyCode::KEYCODE_BACK]){
		mHeldSpeeding = mIsSpeeding = false;
		mSpeedButton->mButtonImage = mIsSpeeding ? IMAGE_SPEEDBUTTON_ACTIVATED : IMAGE_SPEEDBUTTON;
	}

	for (int i = 0; i < mMaxPlayers; i++) {
		PlayerController* player = mPlayers[i];

		if (player->mGlovePlantID != PLANTID_NULL) {
			Plant* mPlant = mPlants.DataArrayTryToGet(player->mGlovePlantID);
			if (!mPlant)	
				player->mGlovePlantID = PLANTID_NULL;
		}

		if (player->mControlType == CONTROL_MOUSE) continue;
		
		float xInput = 0;
		float yInput = 0;

		string upKey = "";
		upKey = player->mKeyUp;
		string leftKey = "";
		leftKey = player->mKeyLeft;
		string downKey = "";
		downKey = player->mKeyDown;
		string rightKey = "";
		rightKey = player->mKeyRight;
		
		if (mApp->mWidgetManager->mKeyDown[GetKeyCodeFromName(upKey)])
			yInput -= player->mControllerSpeed;
		if (mApp->mWidgetManager->mKeyDown[GetKeyCodeFromName(leftKey)])
			xInput -= player->mControllerSpeed;
		if (mApp->mWidgetManager->mKeyDown[GetKeyCodeFromName(downKey)])
			yInput += player->mControllerSpeed;
		if (mApp->mWidgetManager->mKeyDown[GetKeyCodeFromName(rightKey)])
			xInput += player->mControllerSpeed;

		int	gridX = PixelToGridXKeepOnBoard(player->mX, player->mY);
		int gridY = PixelToGridYKeepOnBoard(player->mX, player->mY);

		float magnitude = sqrt(xInput * xInput + yInput * yInput);
		if (magnitude > 0) {
			xInput /= magnitude;
			yInput /= magnitude;
		}

		if (xInput == 0 && yInput == 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_AIR_RAID) {
			if (player->mMoveTime > 0)
				player->mMoveTime--;

			int roofY = (StageHasRoof() ? 15 : 0);

			float targetX = GridToPixelX(gridX, gridY) + 33;
			float targetY = GridToPixelY(gridX, gridY) + roofY;

			player->mX = TodAnimateCurve(100, 0, player->mMoveTime, player->mX, targetX, CURVE_LINEAR);
			player->mY = TodAnimateCurve(100, 0, player->mMoveTime, player->mY, targetY, CURVE_LINEAR);
		}
		else {
			player->mMoveTime = 100;
			player->mX += xInput * player->mControllerSpeed;
			player->mY += yInput * player->mControllerSpeed;
			player->mX = max(min(player->mX, 720 + LAWN_XMIN), 0);
			player->mY = max(min(player->mY, (StageHas6Rows() ? 510 : 500) + LAWN_YMIN), 0);
		}

		string aKey = "";
		aKey = player->mKeyA;

		if (mJustPressedKeys[GetKeyCodeFromName(aKey)] && player->mSeedBankIndex > -1) {
			if (player->mGlovePlantID != PLANTID_NULL){
				Plant* mPlant = mPlants.DataArrayTryToGet(player->mGlovePlantID);
				if (mPlant) {
					SeedType seedType = mPlant->mSeedType;
					bool is2x2 = false;
					if (Plant::IsUpgrade(seedType)){
						if (seedType == SEED_CATTAIL){
							seedType = SEED_LILYPAD;
						}
						else{
							is2x2 = seedType == SEED_COBCANNON;
							seedType = SEED_PEASHOOTER;
						}
					}
					bool reason = !CanPlantAt(gridX, gridY, seedType);
					if (is2x2)	reason = reason && !CanPlantAt(gridX + 1, gridY, seedType);

					if (seedType != SEED_NONE && seedType <= NUM_SEED_TYPES && !reason) {
						MovePlant(mPlant, gridX, gridY);
						mApp->PlayFoley(FOLEY_DROP);
						player->mGlovePlantID = PLANTID_NULL;
						//if (mChallenge->mChallengeState != STATECHALLENGE_NORMAL && mApp->mGameScene == SCENE_PLAYING)
						//	player->mAbilityTime = player->mAbilityMaxTime;
						
					}
				}
			}
			else {
				SeedType seedType = mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType;
				if (seedType == SEED_IMITATER)
					seedType = mSeedBank->mSeedPackets[player->mSeedBankIndex].mImitaterType;

				bool canPlant = CanPlantAt(gridX, gridY, seedType) == PLANTING_OK;

				if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && seedType != SEED_CHERRYHOVERBOMB) {
						canPlant = canPlant && gridX >= 4;
				}

				if (seedType != SEED_NONE && seedType <= NUM_SEED_TYPES && canPlant)
				{
					bool canTakeSuns = CanTakeSunMoney(GetCurrentPlantCost(mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType, SeedType::SEED_NONE));
					bool canUseSeed = mSeedBank->mSeedPackets[player->mSeedBankIndex].mActive;
					bool allowToPlace = canTakeSuns && canUseSeed || mApp->mEasyPlantingCheat || HasConveyorBeltSeedBank();

					if (allowToPlace){
						if (!HasConveyorBeltSeedBank())
							TakeSunMoney(GetCurrentPlantCost(mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType, SeedType::SEED_NONE));

						if (player->mSeedBankIndex == mCursorObject->mSeedBankIndex)
							ClearCursor();

						mSeedBank->mSeedPacketsTime[player->mSeedBankIndex] = 0;

						bool aIsAwake = false;
						int aWakeUpCounter = 0;
						PlantsOnLawn aPlantOnLawn;
						GetPlantsOnLawn(gridX, gridY, &aPlantOnLawn);
						Plant* aNormalPlant = aPlantOnLawn.mNormalPlant;
						Plant* aPumpkinPlant = aPlantOnLawn.mPumpkinPlant;
						if (aNormalPlant && aNormalPlant->IsUpgradableTo(mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType))
						{
							if (mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_GLOOMSHROOM)
							{
								aIsAwake = !aNormalPlant->mIsAsleep;
								aWakeUpCounter = aNormalPlant->mWakeUpCounter;
							}
							aNormalPlant->Die();
						}
						if ((mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_WALLNUT || mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_TALLNUT) && aNormalPlant)
						{
							if (aNormalPlant->mSeedType == mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType)
							{
								aNormalPlant->Die();
							}
						}
						if (mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_PUMPKINSHELL && aPumpkinPlant)
						{
							if (aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
							{
								aPumpkinPlant->Die();
							}
						}
						if (mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_COBCANNON)
						{
							Plant* aRightPlant = GetTopPlantAt(gridX + 1, gridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
							if (aRightPlant)
							{
								aRightPlant->Die();
							}
						}
						if (mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SeedType::SEED_CATTAIL)
						{
							if (aPlantOnLawn.mPotPlant)
							{
								aPlantOnLawn.mPotPlant->Die();
							}
							if (aNormalPlant)
							{
								aNormalPlant->Die();
							}
						}

						Plant* aPlant = nullptr;

						if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
						{
							aPlant = AddPlantEnemy(gridX, gridY, mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType, mSeedBank->mSeedPackets[player->mSeedBankIndex].mImitaterType);
						}
						else {
							aPlant = AddPlant(gridX, gridY, mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType, mSeedBank->mSeedPackets[player->mSeedBankIndex].mImitaterType);
						}

						if (aIsAwake)
							aPlant->SetSleeping(false);
						else
							aPlant->mWakeUpCounter = aWakeUpCounter;

						if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL)
						{
							if (mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SEED_POTATOMINE || mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SEED_CHERRYHOVERBOMB || mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SEED_CHERRYBOMB || mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SEED_JALAPENO || mSeedBank->mSeedPackets[player->mSeedBankIndex].mPacketType == SEED_DOOMSHROOM)
								mChallenge->mBombsLeft = max(0, mChallenge->mBombsLeft - 1);
						}

						mSeedBank->mSeedPackets[player->mSeedBankIndex].Deactivate();
						mSeedBank->mSeedPackets[player->mSeedBankIndex].WasPlanted();
					}
			
				}
			}
		}

		string lbKey = "";
		lbKey = player->mKeyLB;
		string rbKey = "";
		rbKey = player->mKeyRB;

		if (mJustPressedKeys[GetKeyCodeFromName(lbKey)])
			player->mSeedBankIndex--;

		if (mJustPressedKeys[GetKeyCodeFromName(rbKey)])
			player->mSeedBankIndex++;

		int maxSeedsInBank = 0;

		for (int i = 0; i < mSeedBank->mNumPackets; i++)
		{
			int rightEdge = mSeedBank->mX;
			if (HasConveyorBeltSeedBank()) {
				/*if (mIsCOOP)
					rightEdge += IMAGE_CONVEYORBELT_BACKDROP_COOP->GetWidth();
				else*/
				rightEdge += IMAGE_CONVEYORBELT_BACKDROP->GetWidth();
			}
			else {
				rightEdge += mSeedBank->mWidth;
			}

			if (mSeedBank->mSeedPackets[i].mPacketType != SEED_NONE && mSeedBank->mSeedPackets[i].mX + mSeedBank->mSeedPackets[i].mOffsetX + 25 < rightEdge + 79)
				maxSeedsInBank++;
		}

		if (maxSeedsInBank == 0)
			player->mSeedBankIndex = -1;
		else
			player->mSeedBankIndex = max(0, min(player->mSeedBankIndex, maxSeedsInBank - 1));

		string bKey = "";
		bKey = player->mKeyB;

		if (mJustPressedKeys[GetKeyCodeFromName(bKey)] && player->mAbilityTime == 0) {
			if (player->mAbilityType == ABILITY_BUTTER){
				Zombie* aZombie = nullptr;
				Zombie* aTopZombie = nullptr;
				while (IterateZombies(aZombie))
				{
					if (!aZombie->IsDeadOrDying() && !aZombie->mCannotBeHammered)
					{
						Rect aZombieRect = aZombie->GetZombieRect();
						if (GetCircleRectOverlap(player->mX, player->mY, 45, aZombieRect))
						{
							if (aTopZombie == nullptr || aZombie->mRenderOrder >= aTopZombie->mRenderOrder)
							{
								aTopZombie = aZombie;
							}
						}
					}
				}

				if (aTopZombie)
				{
					aTopZombie->ApplyButter();
					mApp->PlayFoley(FOLEY_BUTTER);
					//player->mAbilityTime = player->mAbilityMaxTime;
				}
			}
			else if (player->mAbilityType == ABILITY_HAMMER) {
				mApp->PlayFoley(FOLEY_SWING);

				Zombie* aZombie = nullptr;
				Zombie* aTopZombie = nullptr;
				while (IterateZombies(aZombie))
				{
					if (!aZombie->IsDeadOrDying() && !aZombie->mCannotBeHammered)
					{
						Rect aZombieRect = aZombie->GetZombieRect();
						if (GetCircleRectOverlap(player->mX, player->mY, 45, aZombieRect))
						{
							if (aTopZombie == nullptr || aZombie->mRenderOrder >= aTopZombie->mRenderOrder)
							{
								aTopZombie = aZombie;
							}
						}
					}
				}

				if (aTopZombie)
				{
					mApp->PlayFoley(FOLEY_BONK);
					mApp->AddTodParticle(player->mX - 3, player->mY + 9, RENDER_LAYER_ABOVE_UI, PARTICLE_POW);
					if (aTopZombie->mAltitude >= 0)
						aTopZombie->TakeBodyDamage(20, 0U);
					//player->mAbilityTime = player->mAbilityMaxTime;
				}
			}
			else if (player->mAbilityType == ABILITY_GLOVE) {
				Plant* hitPlant = ToolHitTest(player->mX, player->mY);
				if (hitPlant){
					player->mGlovePlantID = (PlantID)mPlants.DataArrayGetID(hitPlant);
					mApp->PlaySample(SOUND_TAP);
				}
				else if (player->mGlovePlantID != PLANTID_NULL){
					player->mGlovePlantID = PLANTID_NULL;
					mApp->PlayFoley(FOLEY_DROP);
				}
			}
		}
	}

	bool aDisabled = !CanInteractWithBoardButtons() || mIgnoreMouseUp;
	if (!mMenuButton->mBtnNoDraw)
	{
		mMenuButton->mDisabled = aDisabled;
	}
	mMenuButton->Update();
	if (mStoreButton)
	{
		mStoreButton->mDisabled = aDisabled;
		mStoreButton->Update();
	}

	if (!mSpeedButton->mBtnNoDraw)
	{
		mSpeedButton->mDisabled = aDisabled;
	}
	mSpeedButton->Update();

	mApp->mEffectSystem->Update();
	mAdvice->Update();
	UpdateTutorial();

	if (mCobCannonCursorDelayCounter > 0)
	{
		mCobCannonCursorDelayCounter--;
	}
	if (mOutOfMoneyCounter > 0)
	{
		mOutOfMoneyCounter--;
	}
	if (mShakeCounter > 0)
	{
		mShakeCounter--;
		if (mShakeCounter == 0)
		{
			mX = BOARD_ADDITIONAL_WIDTH;
			mY = BOARD_OFFSET_Y;
		}
		else
		{
			if (!Rand(3))
			{
				mShakeAmountX = -mShakeAmountX;
			}
			mX = TodAnimateCurve(12, 0, mShakeCounter, BOARD_ADDITIONAL_WIDTH, BOARD_ADDITIONAL_WIDTH + mShakeAmountX, TodCurves::CURVE_BOUNCE);
			mY = TodAnimateCurve(12, 0, mShakeCounter, BOARD_OFFSET_Y, BOARD_OFFSET_Y + mShakeAmountY, TodCurves::CURVE_BOUNCE);
		}
	}
	if (mCoinBankFadeCount > 0 && mApp->GetDialog(Dialogs::DIALOG_PURCHASE_PACKET_SLOT) == nullptr)
	{
		mCoinBankFadeCount--;
	}
	UpdateLayers();

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SLOWMO && mApp->mBoardResult != BOARDRESULT_WON && mApp->mGameScene != BOARDRESULT_LOST)
	{
		if (gLawnApp->mAppCounter % 2 == 1)
			return;
	}

	if (mTimeStopCounter > 0)
		return;

	mEffectCounter++;
	if (StageHasPool() && !mIceTrapCounter && mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON && !mCutScene->IsSurvivalRepick())
	{
		mApp->mPoolEffect->mPoolCounter++;
	}
	if ((mBackground == BackgroundType::BACKGROUND_3_POOL || mBackground == BackgroundType::BACKGROUND_3_POOLWIDE || mBackground == BackgroundType::BACKGROUND_3_UNSODDED) && mPoolSparklyParticleID == ParticleSystemID::PARTICLESYSTEMID_NULL && mDrawCount > 0)
	{
		int aRenderPosition = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
		TodParticleSystem* aPoolParticle = mApp->AddTodParticle(450 , 295 , aRenderPosition, ParticleEffect::PARTICLE_POOL_SPARKLY);
		mPoolSparklyParticleID = mApp->ParticleGetID(aPoolParticle);

		if (mBackground == BACKGROUND_3_POOLWIDE) {
			int aRenderPosition2 = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
			TodParticleSystem* aPoolParticle2 = mApp->AddTodParticle(450, 125, aRenderPosition2, ParticleEffect::PARTICLE_POOL_SPARKLY);
			mPoolSparklyParticleIDExtra2 = mApp->ParticleGetID(aPoolParticle2);

			int aRenderPosition3 = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
			TodParticleSystem* aPoolParticle3 = mApp->AddTodParticle(450, 465, aRenderPosition3, ParticleEffect::PARTICLE_POOL_SPARKLY);
			mPoolSparklyParticleIDExtra3 = mApp->ParticleGetID(aPoolParticle3);
		}
	}

	UpdateGridItems();
	UpdateFwoosh();
	UpdateGame();
	UpdateFog();
	mChallenge->Update();
	for (int i = 0; i < mMaxPlayers; i++) {
		if (mPlayers[i]->mAbilityTime > 0)
			mPlayers[i]->mAbilityTime--;
	}
	UpdateLevelEndSequence();
	mPrevMouseX = mApp->mWidgetManager->mLastMouseX;
	mPrevMouseY = mApp->mWidgetManager->mLastMouseY;

	for (int i = 0; i < mMaxPlayers; i++) {
		mPlayers[i]->UpdateAnimations();
	}

	for (int i = 0; i < 0xFF;i++) {
		mPrevKeys[i] = mApp->mWidgetManager->mKeyDown[i];
	}

	if (mIsSpeeding)
	{
		for (int i = 0; i < mMaxPlayers; i++) {
			mPlayers[i]->UpdateAnimations();
		}

		mApp->mEffectSystem->Update();
		mAdvice->Update();
		UpdateTutorial();

		if (mCobCannonCursorDelayCounter > 0)
		{
			mCobCannonCursorDelayCounter--;
		}
		if (mOutOfMoneyCounter > 0)
		{
			mOutOfMoneyCounter--;
		}
		if (mShakeCounter > 0)
		{
			mShakeCounter--;
			if (mShakeCounter == 0)
			{
				mX = BOARD_ADDITIONAL_WIDTH;
				mY = BOARD_OFFSET_Y;
			}
			else
			{
				if (!Rand(3))
				{
					mShakeAmountX = -mShakeAmountX;
				}
				mX = TodAnimateCurve(12, 0, mShakeCounter, BOARD_ADDITIONAL_WIDTH, BOARD_ADDITIONAL_WIDTH + mShakeAmountX, TodCurves::CURVE_BOUNCE);
				mY = TodAnimateCurve(12, 0, mShakeCounter, BOARD_OFFSET_Y, BOARD_OFFSET_Y + mShakeAmountY, TodCurves::CURVE_BOUNCE);
			}
		}
		if (mCoinBankFadeCount > 0 && mApp->GetDialog(Dialogs::DIALOG_PURCHASE_PACKET_SLOT) == nullptr)
		{
			mCoinBankFadeCount--;
		}
		UpdateLayers();

		if (mTimeStopCounter > 0)
			return;

		mEffectCounter++;
		if (StageHasPool() && !mIceTrapCounter && mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON && !mCutScene->IsSurvivalRepick())
		{
			mApp->mPoolEffect->mPoolCounter++;
		}
		if ((mBackground == BackgroundType::BACKGROUND_3_POOL || mBackground == BackgroundType::BACKGROUND_3_POOLWIDE || mBackground == BackgroundType::BACKGROUND_3_UNSODDED) && mPoolSparklyParticleID == ParticleSystemID::PARTICLESYSTEMID_NULL && mDrawCount > 0)
		{
			int aRenderPosition = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
			TodParticleSystem* aPoolParticle = mApp->AddTodParticle(450, 295, aRenderPosition, ParticleEffect::PARTICLE_POOL_SPARKLY);
			mPoolSparklyParticleID = mApp->ParticleGetID(aPoolParticle);

			if (mBackground == BACKGROUND_3_POOLWIDE) {
				int aRenderPosition2 = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
				TodParticleSystem* aPoolParticle2 = mApp->AddTodParticle(450, 125, aRenderPosition2, ParticleEffect::PARTICLE_POOL_SPARKLY);
				mPoolSparklyParticleIDExtra2 = mApp->ParticleGetID(aPoolParticle2);

				int aRenderPosition3 = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, 2, 0);
				TodParticleSystem* aPoolParticle3 = mApp->AddTodParticle(450, 465, aRenderPosition3, ParticleEffect::PARTICLE_POOL_SPARKLY);
				mPoolSparklyParticleIDExtra3 = mApp->ParticleGetID(aPoolParticle3);
			}
		}

		UpdateTutorial();
		UpdateGridItems();
		UpdateFwoosh();
		UpdateGame();
		UpdateFog();
		mChallenge->Update();

		for (int i = 0; i < mMaxPlayers; i++) {
			if (mPlayers[i]->mAbilityTime > 0)
				mPlayers[i]->mAbilityTime--;
		}
	}
}

//0x416080
void Board::UpdateLayers()
{
	if (mWidgetManager)
	{
		mWidgetManager->MarkAllDirty();

		for (DialogList::iterator anIter = mApp->mDialogList.begin(); anIter != mApp->mDialogList.end(); ++anIter)
		{
			Dialog* aDialog = *anIter;
			mWidgetManager->BringToFront(aDialog);
			aDialog->MarkDirty();
		}
	}
}

//0x416110
bool Board::RowCanHaveZombies(int theRow)
{
	if (theRow < 0 || theRow >= MAX_GRID_SIZE_Y)
		return false;

	if (mBackground == BACKGROUND_3_POOLWIDE)
		return true;

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED_2 && mCurrentWave < 5 && mPlantRow[theRow] == PlantRowType::PLANTROW_DIRT)
		return false;

	if (mBackground == BACKGROUND_HIGHGROUND && mCurrentWave < 5 && theRow < 4)
		return false;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID)
		return true;

	return (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED && theRow <= 4 || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED_2 && theRow <= 5) || mPlantRow[theRow] != PlantRowType::PLANTROW_DIRT;
}

int Board::GetIceZPos(int theRow)
{
	return MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, theRow, 2);
}

//0x416140
void Board::DrawIce(Graphics* g, int theGridY)
{
	int aPosY = GridToPixelY(8, theGridY) + 20;
	int aHeight = Sexy::IMAGE_ICE->GetHeight();
	int aWidth = Sexy::IMAGE_ICE->GetWidth();
	int anAlpha = ClampInt(255 * mIceTimer[theGridY] / 10, 0, 255);
	if (anAlpha < 255)
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, anAlpha));
	}

	int aBeginningX = mIceMinX[theGridY] + 13, aDeltaX;
	for (int aPosX = aBeginningX; aPosX < 800; aPosX += aDeltaX)
	{
		if (aPosX == aBeginningX)
		{
			aDeltaX = (800 - aBeginningX) % aWidth;
			if (!aDeltaX) aDeltaX = aWidth;
		}
		else aDeltaX = aWidth;
		Rect aRepeatSrcRect(aWidth - aDeltaX, 0, aDeltaX, aHeight);
		Rect aRepeatDstRect(aPosX, aPosY, aDeltaX, aHeight);
		g->DrawImage(Sexy::IMAGE_ICE, aRepeatDstRect, aRepeatSrcRect);
	}
	g->DrawImage(Sexy::IMAGE_ICE_CAP, mIceMinX[theGridY], aPosY);
	g->SetColorizeImages(false);
}

//0x416290
void Board::DrawBackdrop(Graphics* g)
{
	Image* aBgImage = nullptr;
	
	switch (mBackground)
	{
	case BackgroundType::BACKGROUND_1_DAY:				aBgImage = Sexy::IMAGE_BACKGROUND1;						break;
	case BackgroundType::BACKGROUND_2_NIGHT:			aBgImage = Sexy::IMAGE_BACKGROUND2;						break;
	case BackgroundType::BACKGROUND_3_POOL:				aBgImage = Sexy::IMAGE_BACKGROUND3;						break;
	case BackgroundType::BACKGROUND_3_POOLWIDE:			aBgImage = Sexy::IMAGE_BACKGROUND3WIDE;					break;
	case BackgroundType::BACKGROUND_3_UNSODDED:			aBgImage = Sexy::IMAGE_BACKGROUND3UNSODDED;				break;
	case BackgroundType::BACKGROUND_3_GRASS:			aBgImage = Sexy::IMAGE_BACKGROUND3GRASS;				break;
	case BackgroundType::BACKGROUND_4_FOG:				aBgImage = Sexy::IMAGE_BACKGROUND4;						break;
	case BackgroundType::BACKGROUND_5_ROOF:				aBgImage = Sexy::IMAGE_BACKGROUND5;						break;
	case BackgroundType::BACKGROUND_6_BOSS:				aBgImage = Sexy::IMAGE_BACKGROUND6BOSS;					break;
	case BackgroundType::BACKGROUND_6_AIRRAID:			aBgImage = Sexy::IMAGE_BACKGROUND6AIRRAIDBG;			break;
	case BackgroundType::BACKGROUND_GRAVEYARD:			aBgImage = Sexy::IMAGE_BACKGROUNDGRAVEYARD;				break;
	case BackgroundType::BACKGROUND_HIGHGROUND:			aBgImage = Sexy::IMAGE_BACKGROUNDHIGHGROUND;			break;
	case BackgroundType::BACKGROUND_MUSHROOM_GARDEN:	aBgImage = Sexy::IMAGE_BACKGROUND_MUSHROOMGARDEN;		break;
	case BackgroundType::BACKGROUND_GREENHOUSE:			aBgImage = Sexy::IMAGE_BACKGROUND_GREENHOUSE;			break;
	case BackgroundType::BACKGROUND_ZOMBIQUARIUM:		aBgImage = Sexy::IMAGE_AQUARIUM1;						break;
	case BackgroundType::BACKGROUND_TREEOFWISDOM:		aBgImage = nullptr;										break;
	default:											/*TOD_ASSERT();*/										break;
	}

	if (mLevel == 1 && mApp->IsFirstTimeAdventureMode())
	{
		g->DrawImage(Sexy::IMAGE_BACKGROUND1UNSODDED, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
		int aWidth = TodAnimateCurve(0, 1000, mSodPosition, 0, Sexy::IMAGE_SOD1ROW->GetWidth(), TodCurves::CURVE_LINEAR);
		Rect aSrcRect(0, 0, aWidth, Sexy::IMAGE_SOD1ROW->GetHeight());
		g->DrawImage(Sexy::IMAGE_SOD1ROW, 239 - BOARD_OFFSET_X, 265, aSrcRect);
	}
	else if (((mLevel == 2 || mLevel == 3) && mApp->IsFirstTimeAdventureMode()) || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_UNSODDED)
	{
		g->DrawImage(Sexy::IMAGE_BACKGROUND1UNSODDED, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
		g->DrawImage(Sexy::IMAGE_SOD1ROW, 239 - BOARD_OFFSET_X, 265);
		int aWidth = TodAnimateCurve(0, 1000, mSodPosition, 0, Sexy::IMAGE_SOD3ROW->GetWidth(), TodCurves::CURVE_LINEAR);
		Rect aSrcRect(0, 0, aWidth, Sexy::IMAGE_SOD3ROW->GetHeight());
		g->DrawImage(Sexy::IMAGE_SOD3ROW, 235 - BOARD_OFFSET_X, 149, aSrcRect);
	}
	else if (mLevel == 4 && mApp->IsFirstTimeAdventureMode())
	{
		g->DrawImage(Sexy::IMAGE_BACKGROUND1UNSODDED, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
		g->DrawImage(Sexy::IMAGE_SOD3ROW, 235 -BOARD_OFFSET_X, 149);
		int aWidth = TodAnimateCurve(0, 1000, mSodPosition, 0, 773, TodCurves::CURVE_LINEAR);
		Rect aSrcRect(232, 0, aWidth + BOARD_ADDITIONAL_WIDTH, Sexy::IMAGE_BACKGROUND1->GetHeight());
		g->DrawImage(Sexy::IMAGE_BACKGROUND1, 232  -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y, aSrcRect);
	}
	else if (aBgImage)
	{
		if (aBgImage == Sexy::IMAGE_BACKGROUND_MUSHROOMGARDEN || aBgImage == Sexy::IMAGE_BACKGROUND_GREENHOUSE || aBgImage == Sexy::IMAGE_AQUARIUM1)
		{
			g->DrawImage(aBgImage, -BOARD_ADDITIONAL_WIDTH, -BOARD_OFFSET_Y);
		}
		else if (mBackground == BACKGROUND_6_AIRRAID)
		{
			g->DrawImage(aBgImage, -BOARD_ADDITIONAL_WIDTH, -BOARD_OFFSET_Y);
			g->DrawImage(IMAGE_BACKGROUND6AIRRAIDSTARS, -BOARD_ADDITIONAL_WIDTH - mChallenge->mAirRaidSTARTime, -BOARD_OFFSET_Y);
			g->DrawImage(IMAGE_BACKGROUND6AIRRAIDSTARS, -BOARD_ADDITIONAL_WIDTH - mChallenge->mAirRaidSTARTime + IMAGE_BACKGROUND6AIRRAIDSTARS->GetWidth(), -BOARD_OFFSET_Y);
			g->DrawImage(IMAGE_BACKGROUND6AIRRAIDCLOUDS, -BOARD_ADDITIONAL_WIDTH - mChallenge->mAirRaidCLOUDTime, -BOARD_OFFSET_Y);
			g->DrawImage(IMAGE_BACKGROUND6AIRRAIDCLOUDS, -BOARD_ADDITIONAL_WIDTH - mChallenge->mAirRaidCLOUDTime + IMAGE_BACKGROUND6AIRRAIDCLOUDS->GetWidth(), -BOARD_OFFSET_Y);
		}
		else
		{			
			if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
			{
				g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
				if (mApp->mGameScene == SCENE_PLAYING && mChallenge->mChallengeStateCounter <= 500 && mChallenge->mChallengeStateCounter >= 0)
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(255, 255, 255, 255 * (500 - mChallenge->mChallengeStateCounter) / 500));
					if (StageIsNight())
						aBgImage = Sexy::IMAGE_BACKGROUND5;
					else
						aBgImage = Sexy::IMAGE_BACKGROUND6BOSS;

					g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
					g->SetColorizeImages(false);
				}
			}
			else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BLOOD_MOON)
			{
				if (mApp->mGameScene == SCENE_PLAYING)
				{
					int daColor = 0;
					if (mChallenge->mBloodMoon <= 500 && mChallenge->mBloodMoon >= 0 && !mChallenge->mBloodMooning)
					{
						daColor = 128 * (500 - mChallenge->mBloodMoon) / 500 ;
					}
					else if (mChallenge->mBloodMoon <= 500 && mChallenge->mBloodMoon >= 0 && mChallenge->mBloodMooning)
					{
						daColor = max(128 - 128 * (500 - mChallenge->mBloodMoon) / 500 , 0);
					}
					else if (mChallenge->mBloodMooning)
					{
						daColor = 128;
					}
				
					g->SetColorizeImages(true);

					
					g->SetColor(Color(255, 255 - daColor, 255 - daColor, 255));
					

					g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
					
					g->SetColor(Color(daColor, 0, 0, 255));
	
					g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
					g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
					g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
					g->SetColorizeImages(false);
				}
				else {
					g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
				}
			}
			else {
				g->DrawImage(aBgImage, -(BOARD_OFFSET_X + BOARD_ADDITIONAL_WIDTH), -BOARD_OFFSET_Y);
			}
		}
	}

	if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
	{
		DrawHouseDoorBottom(g);
	}
	if (StageHasPool())
	{
		if (mBackground == BACKGROUND_3_POOLWIDE)
			mApp->mPoolEffect->WidePoolEffectDraw(g, StageIsNight());
		else
			mApp->mPoolEffect->PoolEffectDraw(g, StageIsNight());
			
	}
	if (mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER)
	{
		Graphics aClipG(*g);
		aClipG.SetColorizeImages(true);
		aClipG.SetColor(GetFlashingColor(mMainCounter, 75));
		aClipG.DrawImage(Sexy::IMAGE_SOD1ROW, 239 - BOARD_OFFSET_X, 265);
		aClipG.SetColorizeImages(false);
	}
	mChallenge->DrawBackdrop(g);
	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && StageHasGraveStones())
	{
		g->DrawImage(Sexy::IMAGE_NIGHT_GRAVE_GRAPHIC, 1092 + BOARD_ADDITIONAL_WIDTH / 4, 40);
	}
}

//0x416690
bool RenderItemSortFunc(const RenderItem& theItem1, const RenderItem& theItem2)
{
	if (theItem1.mZPos == theItem2.mZPos)
	{
		return theItem1.mGameObject < theItem2.mGameObject;
	}

	return theItem1.mZPos < theItem2.mZPos;
}

//0x4166C0
void Board::AddBossRenderItem(RenderItem* theRenderList, int& theCurRenderItem, Zombie* theBossZombie)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	int aBackLegRow = 1;
	int aFrontLegRow = 3;
	int aBackArmRow = 4;
	if (theBossZombie->IsDeadOrDying())
	{
		aBackArmRow = 1;
	}
	else if (theBossZombie->mZombiePhase == ZombiePhase::PHASE_BOSS_STOMPING)
	{
		Reanimation* aBossReanim = mApp->ReanimationTryToGet(theBossZombie->mBodyReanimID);
		if (aBossReanim->mAnimTime > 0.25f && aBossReanim->mAnimTime < 0.75f)
		{
			if (theBossZombie->mTargetRow == 1)
			{
				aBackLegRow = 2;
			}
			else if (theBossZombie->mTargetRow == 3)
			{
				aFrontLegRow = 4;
			}
		}
	}

	RenderItem* aItem = &theRenderList[theCurRenderItem];
	aItem->mRenderObjectType = RenderObjectType::RENDER_ITEM_BOSS_PART;
	aItem->mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_BOSS, aBackLegRow, 2);
	aItem->mBossPart = BossPart::BOSS_PART_BACK_LEG;
	theCurRenderItem++;
	aItem = &theRenderList[theCurRenderItem];
	aItem->mRenderObjectType = RenderObjectType::RENDER_ITEM_BOSS_PART;
	aItem->mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_BOSS, aFrontLegRow, 2);
	aItem->mBossPart = BossPart::BOSS_PART_FRONT_LEG;
	theCurRenderItem++;
	aItem = &theRenderList[theCurRenderItem];
	aItem->mRenderObjectType = RenderObjectType::RENDER_ITEM_BOSS_PART;
	aItem->mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_BOSS, 4, 2);
	aItem->mBossPart = BossPart::BOSS_PART_MAIN;
	theCurRenderItem++;
	aItem = &theRenderList[theCurRenderItem];
	aItem->mRenderObjectType = RenderObjectType::RENDER_ITEM_BOSS_PART;
	aItem->mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_BOSS, aBackArmRow, 3);
	aItem->mBossPart = BossPart::BOSS_PART_BACK_ARM;
	theCurRenderItem++;

	Reanimation* aBallReanim = mApp->ReanimationTryToGet(theBossZombie->mBossFireBallReanimID);
	if (aBallReanim)
	{
		RenderItem* aItem = &theRenderList[theCurRenderItem];
		aItem->mRenderObjectType = RenderObjectType::RENDER_ITEM_BOSS_PART;
		aItem->mZPos = aBallReanim->mRenderOrder;
		aItem->mBossPart = BossPart::BOSS_PART_FIREBALL;
		theCurRenderItem++;
	}
}

static inline void AddGameObjectRenderItem(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	theCurRenderItem++;
}

static inline void AddGameObjectRenderItemCursorPreview(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	aRenderItem.mCursorPreview = (CursorPreview*)theGameObject;

	theCurRenderItem++;
}

static inline void AddGameObjectRenderItemPlant(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	aRenderItem.mPlant = (Plant*)theGameObject;

	theCurRenderItem++;
}

static inline void AddGameObjectRenderItemZombie(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	aRenderItem.mZombie = (Zombie*)theGameObject;
	theCurRenderItem++;
}

static inline void AddGameObjectRenderItemProjectile(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	aRenderItem.mProjectile = (Projectile*)theGameObject;
	theCurRenderItem++;
}

static inline void AddGameObjectRenderItemCoin(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, GameObject* theGameObject)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = theGameObject->mRenderOrder;
	aRenderItem.mGameObject = theGameObject;
	aRenderItem.mCoin = (Coin*)theGameObject;
	theCurRenderItem++;
}

static inline void AddUIRenderItem(RenderItem* theRenderList, int& theCurRenderItem, RenderObjectType theRenderObjectType, int thePosZ)
{
	TOD_ASSERT(theCurRenderItem < MAX_RENDER_ITEMS);
	RenderItem& aRenderItem = theRenderList[theCurRenderItem];
	aRenderItem.mRenderObjectType = theRenderObjectType;
	aRenderItem.mZPos = thePosZ;
	aRenderItem.mGameObject = nullptr;
	theCurRenderItem++;
}

//0x416880
void Board::DrawGameObjects(Graphics* g)
{
	TodHesitationTrace("creating render list");

	RenderItem aRenderList[MAX_RENDER_ITEMS];
	int aRenderItemCount = 0;

	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mOnBungeeState == PlantOnBungeeState::NOT_ON_BUNGEE)
		{
			AddGameObjectRenderItemPlant(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_PLANT, aPlant);

			if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && aPlant->mPottedPlantIndex != -1)
			{
				RenderItem& aRenderItem = aRenderList[aRenderItemCount];
				aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_PLANT_OVERLAY;
				aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, 0, mY);
				aRenderItem.mPlant = aPlant;
				aRenderItemCount++;
			}

			if ((aPlant->mSeedType == SeedType::SEED_MAGNETSHROOM || aPlant->mSeedType == SeedType::SEED_GOLD_MAGNET) && aPlant->DrawMagnetItemsOnTop())
			{
				RenderItem& aRenderItem = aRenderList[aRenderItemCount];
				aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_PLANT_MAGNET_ITEMS;
				aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, -1);
				aRenderItem.mPlant = aPlant;
				aRenderItemCount++;
			}
		}
	}

	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		AddGameObjectRenderItemCoin(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_COIN, aCoin);
	}
	
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
		{
			AddBossRenderItem(aRenderList, aRenderItemCount, aZombie);
		}
		else
		{
			AddGameObjectRenderItemZombie(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_ZOMBIE, aZombie);

			if (aZombie->HasShadow())
			{
				RenderItem& aRenderItem = aRenderList[aRenderItemCount];
				aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_ZOMBIE_SHADOW;
				aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, aZombie->mRow, 3);
				aRenderItem.mZombie = aZombie;
				aRenderItemCount++;
			}

			if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE)
			{
				RenderItem& aRenderItem = aRenderList[aRenderItemCount];
				aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_ZOMBIE_BUNGEE_TARGET;
				aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, aZombie->mRow, 1);
				aRenderItem.mZombie = aZombie;
				aRenderItemCount++;
			}
		}
	}

	if (mApp->Is3dAccel()){
		Bush* aBushItem = nullptr;
		while (IterateBushes(aBushItem))
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_OVERLAY;
			if (mApp->mGameScene == SCENE_PLAYING || mApp->mGameMode == GAMEMODE_INTRO || mApp->mGameMode == GAMEMODE_UPSELL)
				aRenderItem.mZPos = aBushItem->mRenderOrder;
			else
				aRenderItem.mZPos = MakeRenderOrder(RENDER_LAYER_GROUND, aBushItem->mRow, 0);
			aRenderItem.mBush = aBushItem;
			aRenderItemCount++;
		}
	}
	else {
		BushCheap* aBushCheapItem = nullptr;
		while (IterateBushesCheap(aBushCheapItem))
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_OVERLAYCHEAP;
			if (mApp->mGameScene == SCENE_PLAYING)
				aRenderItem.mZPos = aBushCheapItem->mRenderOrder;
			else
				aRenderItem.mZPos = MakeRenderOrder(RENDER_LAYER_GROUND, aBushCheapItem->mRow, 0);
			aRenderItem.mBushCheap = aBushCheapItem;
			aRenderItemCount++;
		}
	}

	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_BUSH_OVERLAY, MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, 7, 0));
	
	Projectile* aProjectile = nullptr;
	while (IterateProjectiles(aProjectile))
	{
		AddGameObjectRenderItemProjectile(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_PROJECTILE, aProjectile);

		RenderItem& aRenderItem = aRenderList[aRenderItemCount];
		aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_PROJECTILE_SHADOW;
		aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, aProjectile->mRow, 3);
		aRenderItem.mProjectile = aProjectile;
		aRenderItemCount++;
	}
	
	LawnMower* aLawnMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		RenderItem& aRenderItem = aRenderList[aRenderItemCount];
		aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_MOWER;
		aRenderItem.mZPos = aLawnMower->mRenderOrder;
		aRenderItem.mMower = aLawnMower;
		aRenderItemCount++;
	}
	
	TodParticleSystem* aParticle = nullptr;
	while (IterateParticles(aParticle))
	{
		if (!aParticle->mIsAttachment)
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_PARTICLE;
			aRenderItem.mZPos = aParticle->mRenderOrder;
			aRenderItem.mParticleSytem = aParticle;
			aRenderItemCount++;
		}
	}
	
	Reanimation* aReanimation = nullptr;
	while (IterateReanimations(aReanimation))
	{
		if (!aReanimation->mIsAttachment)
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_REANIMATION;
			aRenderItem.mZPos = aReanimation->mRenderOrder;
			aRenderItem.mReanimation = aReanimation;
			aRenderItemCount++;
		}
	}
	
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		RenderItem& aRenderItem = aRenderList[aRenderItemCount];
		aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_GRID_ITEM;
		aRenderItem.mZPos = aGridItem->mRenderOrder;
		aRenderItem.mGridItem = aGridItem;
		aRenderItemCount++;

		if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && aGridItem->mGridItemType == GridItemType::GRIDITEM_STINKY)
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_GRID_ITEM_OVERLAY;
			aRenderItem.mZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, 0, aGridItem->mPosY - 30.0f);
			aRenderItem.mGridItem = aGridItem;
			aRenderItemCount++;
		}
	}

	for (int i = 0; i < MAX_GRID_SIZE_Y; i++)
	{
		if (mIceTimer[i])
		{
			RenderItem& aRenderItem = aRenderList[aRenderItemCount];
			aRenderItem.mRenderObjectType = RenderObjectType::RENDER_ITEM_ICE;
			aRenderItem.mBoardGridY = i;
			aRenderItem.mZPos = GetIceZPos(i);
			aRenderItemCount++;
		}
	}
	
	int aZPos;
	if (mTimeStopCounter > 0)
	{
		aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_ABOVE_UI, 0, 0);
	}
	else if (mApp->mGameScene == GameScenes::SCENE_PLAYING || mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
	{
		aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_BOTTOM, 0, 1);
	}
	else if (mCutScene->IsAfterSeedChooser() || mCutScene->IsInShovelTutorial() || mHelpIndex == AdviceType::ADVICE_CLICK_TO_CONTINUE)
	{
		aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_BOTTOM, 0, 1);
	}
	else
	{
		aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_ABOVE_UI, 0, 0);
	}

	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_BACKDROP, MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_BOTTOM, 0, 0));
	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_BOTTOM_UI, aZPos);
	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_COIN_BANK, MakeRenderOrder(RenderLayer::RENDER_LAYER_COIN_BANK, 0, 0));
	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_TOP_UI, MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_TOP, 0, 0));
	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_SCREEN_FADE, MakeRenderOrder(RenderLayer::RENDER_LAYER_SCREEN_FADE, 0, 0));
	
	if (mApp->mGameScene == GameScenes::SCENE_ZOMBIES_WON)
	{
		int aZPos;
		if (StageHasRoof())
		{
			aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, 0, 4);
		}
		else
		{
			aZPos = MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, 3, 2);
		}
		AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_DOOR_MASK, aZPos);
	}

	if (StageHasFog())
	{
		AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_FOG, MakeRenderOrder(RenderLayer::RENDER_LAYER_FOG, 0, 0));
	}

	if (mApp->IsStormyNightLevel() || StageHasRain())
	{
		AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_STORM, MakeRenderOrder(RenderLayer::RENDER_LAYER_FOG, 0, 3));
	}	

	AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_TOP_OVERLAY, MakeRenderOrder(RenderLayer::RENDER_LAYER_FOG, 0, 4));

	if (StageHasDarkness())
	{
		AddUIRenderItem(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_DARKNESS, MakeRenderOrder(RenderLayer::RENDER_LAYER_FOG, 0, 4));
	}

	AddGameObjectRenderItemCursorPreview(aRenderList, aRenderItemCount, RenderObjectType::RENDER_ITEM_CURSOR_PREVIEW, mCursorPreview);

	TodHesitationTrace("start sort");
	std::sort(aRenderList, aRenderList + aRenderItemCount, RenderItemSortFunc);

	TodHesitationTrace("end sort, start draw");
	for (int i = 0; i < aRenderItemCount; i++)
	{
		RenderItem& aRenderItem = aRenderList[i];
		switch (aRenderItem.mRenderObjectType)
		{
		case RenderObjectType::RENDER_ITEM_PLANT:
		{
			Plant* aPlant = aRenderItem.mPlant;
			if (aPlant->BeginDraw(g)) {
				aPlant->Draw(g);
				aPlant->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_PLANT_OVERLAY:
		{
			Plant* aPlant = aRenderItem.mPlant;
			if (aPlant->BeginDraw(g)){
				mApp->mZenGarden->DrawPlantOverlay(g, aPlant);
				aPlant->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_PLANT_MAGNET_ITEMS:
		{
			Plant* aPlant = aRenderItem.mPlant;
			if (aPlant->BeginDraw(g))
			{
				aPlant->DrawMagnetItems(g);
				aPlant->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_MOWER:
		{
			LawnMower* aLawnMower = aRenderItem.mMower;
			aLawnMower->Draw(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_ZOMBIE:
		{
			Zombie* aZombie = aRenderItem.mZombie;
			if (aZombie->BeginDraw(g))
			{
				aZombie->Draw(g);
				aZombie->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_ZOMBIE_SHADOW:
		{
			Zombie* aZombie = aRenderItem.mZombie;
			if (aZombie->BeginDraw(g))
			{
				aZombie->DrawShadow(g);
				aZombie->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_ZOMBIE_BUNGEE_TARGET:
		{
			Zombie* aZombie = aRenderItem.mZombie;
			aZombie->DrawBungeeTarget(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_BOSS_PART:
		{
			Zombie* aBossZombie = GetBossZombie();
			if (aBossZombie && aBossZombie->BeginDraw(g))
			{
				aBossZombie->DrawBossPart(g, aRenderItem.mBossPart);
				aBossZombie->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_COIN:
		{
			Coin* aCoin = aRenderItem.mCoin;
			if (aCoin->BeginDraw(g))
			{
				aCoin->Draw(g);
				aCoin->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_PROJECTILE:
		{
			Projectile* aProjectile = aRenderItem.mProjectile;
			if (aProjectile->BeginDraw(g))
			{
				aProjectile->Draw(g);
				aProjectile->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_PROJECTILE_SHADOW:
		{
			Projectile* aProjectile = aRenderItem.mProjectile;
			if (aProjectile->BeginDraw(g))
			{
				aProjectile->DrawShadow(g);
				aProjectile->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_CURSOR_PREVIEW:
		{
			CursorPreview* aCursorPreview = aRenderItem.mCursorPreview;
			if (aCursorPreview->BeginDraw(g))
			{
				aCursorPreview->Draw(g);
				aCursorPreview->EndDraw(g);
			}
			break;
		}
		
		case RenderObjectType::RENDER_ITEM_GRID_ITEM:
		{
			GridItem* aGridItem = aRenderItem.mGridItem;
			aGridItem->DrawGridItem(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_GRID_ITEM_OVERLAY:
		{
			GridItem* aGridItem = aRenderItem.mGridItem;
			aGridItem->DrawGridItemOverlay(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_ICE:
			DrawIce(g, aRenderItem.mBoardGridY);
			break;

		case RenderObjectType::RENDER_ITEM_PARTICLE:
		{
			TodParticleSystem* aParticle = aRenderItem.mParticleSytem;
			aParticle->Draw(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_REANIMATION:
		{
			Reanimation* aReanimation = aRenderItem.mReanimation;
			aReanimation->Draw(g);
			break;
		}

		case RenderObjectType::RENDER_ITEM_COIN_BANK:
			DrawUICoinBank(g);
			break;

		case RenderObjectType::RENDER_ITEM_BACKDROP:
			DrawBackdrop(g);
			break;

		case RenderObjectType::RENDER_ITEM_DOOR_MASK:
			DrawHouseDoorTop(g);
			break;

		case RenderObjectType::RENDER_ITEM_BOTTOM_UI:
			DrawUIBottom(g);
			break;
		
		case RenderObjectType::RENDER_ITEM_TOP_UI:
			DrawUITop(g);
			break;
			
		case RenderObjectType::RENDER_ITEM_FOG:
			DrawFog(g);
			break;

		case RenderObjectType::RENDER_ITEM_OVERLAY:
		{
			Bush* bush = aRenderItem.mBush;
			if (bush->BeginDraw(g))
			{
				bush->Draw(g);
				bush->EndDraw(g);
			}
			break;
		}

		case RenderObjectType::RENDER_ITEM_OVERLAYCHEAP:
		{
			BushCheap* bushCheap = aRenderItem.mBushCheap;
			if (bushCheap->BeginDraw(g))
			{
				bushCheap->Draw(g);
				bushCheap->EndDraw(g);
			}
			break;
		}

		case RENDER_ITEM_BUSH_OVERLAY:
		{
			switch (mBackground)
			{
				case BACKGROUND_1_DAY:
					g->DrawImage(Sexy::IMAGE_BACKGROUND1_COVER, 685, 557);
					break;
				case BACKGROUND_2_NIGHT:
					g->DrawImage(Sexy::IMAGE_BACKGROUND2_COVER, 685, 557);
					break;
				case BACKGROUND_3_POOL:
				case BACKGROUND_3_POOLWIDE:
				case BACKGROUND_3_UNSODDED:
				case BACKGROUND_3_GRASS:
					g->DrawImage(Sexy::IMAGE_BACKGROUND3_COVER, 671, 613);
					break;
				case BACKGROUND_4_FOG:
				case BACKGROUND_HIGHGROUND:
					g->DrawImage(Sexy::IMAGE_BACKGROUND4_COVER, 671, 613);
					break;
			}

			break;
		}

		case RENDER_ITEM_TOP_OVERLAY:
		{
			int offsetX = (mApp->IsFinalBossLevel() ? BOARD_ADDITIONAL_WIDTH : 0);
			g->mTransX += offsetX;

			switch (mBackground)
			{
				case BACKGROUND_5_ROOF:
				{
					if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
					{
						g->DrawImage(Sexy::IMAGE_BACKGROUND5_TREES, mRoofTreeOffset, -60);
						if (mApp->mGameScene == SCENE_PLAYING && mChallenge->mChallengeStateCounter <= 500 && mChallenge->mChallengeStateCounter >= 0)
						{
							g->SetColorizeImages(true);
							g->SetColor(Color(255, 255, 255, 255 * (500 - mChallenge->mChallengeStateCounter) / 500));
							g->DrawImage(StageIsNight() ? IMAGE_BACKGROUND5_TREES : IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
							g->SetColorizeImages(false);
						}

						g->DrawImage(Sexy::IMAGE_BACKGROUND5_POLE, mRoofPoleOffset, -60);
						if (mApp->mGameScene == SCENE_PLAYING && mChallenge->mChallengeStateCounter <= 500 && mChallenge->mChallengeStateCounter >= 0)
						{
							g->SetColorizeImages(true);
							g->SetColor(Color(255, 255, 255, 255 * (500 - mChallenge->mChallengeStateCounter) / 500));
							g->DrawImage(StageIsNight() ? IMAGE_BACKGROUND5_POLE : IMAGE_BACKGROUND6_POLE, mRoofTreeOffset, -60);
							g->SetColorizeImages(false);
						}
					}
					else
					{
						g->DrawImage(Sexy::IMAGE_BACKGROUND5_TREES, mRoofTreeOffset, -60);
						g->DrawImage(Sexy::IMAGE_BACKGROUND5_POLE, mRoofPoleOffset, -60);
					}
					break;
				}
				case BACKGROUND_6_BOSS:
				{
					if (mApp->mGameMode == GAMEMODE_CHALLENGE_TOO_MANY_ECLIPSES)
					{
						g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
						if (mApp->mGameScene == SCENE_PLAYING && mChallenge->mChallengeStateCounter <= 500 && mChallenge->mChallengeStateCounter >= 0)
						{
							g->SetColorizeImages(true);
							g->SetColor(Color(255, 255, 255, 255 * (500 - mChallenge->mChallengeStateCounter) / 500));
							g->DrawImage(StageIsNight() ? IMAGE_BACKGROUND5_TREES : IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
							g->SetColorizeImages(false);
						}

						g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);
						if (mApp->mGameScene == SCENE_PLAYING && mChallenge->mChallengeStateCounter <= 500 && mChallenge->mChallengeStateCounter >= 0)
						{
							g->SetColorizeImages(true);
							g->SetColor(Color(255, 255, 255, 255 * (500 - mChallenge->mChallengeStateCounter) / 500));
							g->DrawImage(StageIsNight() ? IMAGE_BACKGROUND5_POLE : IMAGE_BACKGROUND6_POLE, mRoofTreeOffset, -60);
							g->SetColorizeImages(false);
						}
					}
					else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BLOOD_MOON)
					{
						if (mApp->mGameScene == SCENE_PLAYING)
						{
							int daColor = 0;
							if (mChallenge->mBloodMoon <= 500 && mChallenge->mBloodMoon >= 0 && !mChallenge->mBloodMooning)
							{
								daColor = 128 * (500 - mChallenge->mBloodMoon) / 500;
							}
							else if (mChallenge->mBloodMoon <= 500 && mChallenge->mBloodMoon >= 0 && mChallenge->mBloodMooning)
							{
								daColor = max(128 - 128 * (500 - mChallenge->mBloodMoon) / 500, 0);
							}
							else if (mChallenge->mBloodMooning)
							{
								daColor = 128;
							}


							g->SetColorizeImages(true);


							g->SetColor(Color(255, 255 - daColor, 255 - daColor, 255));
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);


							g->SetColor(Color(daColor, 0, 0, 255));
							g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);


							g->SetColor(Color(255, 255 - daColor, 255 - daColor, 255));

							g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);

							g->SetColor(Color(daColor, 0, 0, 255));

							g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);

							g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
							g->SetColorizeImages(false);
						}
						else {
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
							g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);
						}
					}
					else if (mApp->IsStormyNightLevel()) {
						int timePos = 0, maxPos = 0;
						bool jitter = false;
						if (mChallenge->mChallengeState == STATECHALLENGE_STORM_FLASH_1 && mChallenge->mChallengeStateCounter < 300)
						{
							jitter = true;
							if (mChallenge->mChallengeStateCounter > STORM_FLASH_TIME)
							{
								timePos = mChallenge->mChallengeStateCounter - STORM_FLASH_TIME;
								maxPos = 255;
							}
							else
							{
								timePos = mChallenge->mChallengeStateCounter;
								maxPos = 92;
							}
						}
						else if (mChallenge->mChallengeState == STATECHALLENGE_STORM_FLASH_2 && mChallenge->mChallengeStateCounter < 300)
						{
							jitter = true;
							timePos = mChallenge->mChallengeStateCounter / 2.0f;
							maxPos = 255;
						}
						else if (mChallenge->mChallengeState == STATECHALLENGE_STORM_FLASH_3 && mChallenge->mChallengeStateCounter < 150)
						{
							jitter = true;
							timePos = mChallenge->mChallengeStateCounter;
							maxPos = 255;
						}

						int aDarkness = 255;

						if (jitter)
						{
							MTRand aDrawRand = MTRand(mMainCounter / 6);
							aDarkness = TodAnimateCurve(150, 0, timePos, 255 - maxPos, 255, CURVE_LINEAR) + aDrawRand.NextNoAssert((unsigned long)64) - 32;
						}

						aDarkness = ClampInt(255 - aDarkness, 0, 255);
						g->SetColorizeImages(true);
						g->SetColor(Color(aDarkness, aDarkness, aDarkness, 255));
						g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
						g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);
						g->SetColorizeImages(false);
					}
					else {
						g->DrawImage(Sexy::IMAGE_BACKGROUND6_TREES, mRoofTreeOffset, -60);
						g->DrawImage(Sexy::IMAGE_BACKGROUND6_POLE, mRoofPoleOffset, -60);
					}

					break;
				}
			}

			g->mTransX -= offsetX;

			break;
		}
		
		case RenderObjectType::RENDER_ITEM_STORM:
			mChallenge->DrawWeather(g);
			break;

		case RenderObjectType::RENDER_ITEM_DARKNESS:
			DrawDarkness(g);
			break;
		
		case RenderObjectType::RENDER_ITEM_SCREEN_FADE:
			DrawFadeOut(g);
			break;
		

		default:
			TOD_ASSERT();
			break;
		}
	}

	TodHesitationTrace("end draw");
}

void Board::DrawDarkness(Graphics* g)
{
	if (mUpdateDarkness)
	{
		mUpdateDarkness = false;
		Graphics aMemoryGraphics(mDarknessImage);
		mDarknessImage->FillRect(Rect(0, 0, mDarknessImage->mWidth, mDarknessImage->mHeight), Color::Black, 0);
		aMemoryGraphics.SetDrawMode(2);

		if (mApp->mGameMode != GAMEMODE_CHALLENGE_LIGHTS_OUT)
		{
			Graphics aMemoryGraphicsDarker(mDarknessDarkerImage);
			mDarknessDarkerImage->FillRect(Rect(0, 0, mDarknessDarkerImage->mWidth, mDarknessDarkerImage->mHeight), Color::Black, 0);
			aMemoryGraphicsDarker.SetDrawMode(2);


			Image* img = IMAGE_FLASHLIGHT_LIGHT;

			Plant* aPlant = nullptr;
			while (IteratePlants(aPlant)) {
				if (aPlant->mSeedType == SEED_PLANTERN) {
					/*aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - img->mWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - img->mHeight / 2.0f + BOARD_OFFSET_Y);

					float scaledWidth = img->mWidth * 0.75f;
					float scaledHeight = img->mHeight * 0.5f;
					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);*/

					float scaledWidth = img->mWidth * 1.25;
					aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - img->mHeight / 2 + BOARD_OFFSET_Y, scaledWidth, img->mHeight);

					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - img->mWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - img->mHeight / 2.0f + BOARD_OFFSET_Y);
				}
				else if (aPlant->mSeedType == SEED_TORCHWOOD || aPlant->mSeedType == SEED_FIRESHROOM) {
					/*aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - img->mWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - img->mHeight / 2.0f + BOARD_OFFSET_Y);

					float scaledWidth = img->mWidth * 0.75f;
					float scaledHeight = img->mHeight * 0.5f;
					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);*/

					float scaledWidth = img->mWidth * 0.3125f;
					float scaledHeight = img->mHeight * 0.25f;
					aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - 25 - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);

				    scaledWidth = img->mWidth * 0.25f;
					scaledHeight = img->mHeight * 0.25f;
					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - 25 - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);
				}
				else if (aPlant->mSeedType == SEED_PLASMAWOOD) {
					/*aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - img->mWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - img->mHeight / 2.0f + BOARD_OFFSET_Y);

					float scaledWidth = img->mWidth * 0.75f;
					float scaledHeight = img->mHeight * 0.5f;
					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);*/

					float scaledWidth = img->mWidth * 0.625f;
					float scaledHeight = img->mHeight * 0.5f;
					aMemoryGraphics.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - 25 - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);

					scaledWidth = img->mWidth * 0.5f;
					scaledHeight = img->mHeight * 0.5f;
					aMemoryGraphicsDarker.DrawImage(img, aPlant->mX + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH,
						aPlant->mY + 42.5f - 25 - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);
				}
			}
		}

		if (mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT)
		{
			//float scaledWidth = img->mWidth * 0.75f;
			//float scaledHeight = img->mHeight * 0.75f;
			//aMemoryGraphics.DrawImage(img, 400 + 42.5f - scaledWidth / 2 + BOARD_ADDITIONAL_WIDTH * 2,
			//	300 + 42.5f - scaledHeight / 2 + BOARD_OFFSET_Y, scaledWidth, scaledHeight);
					
			//aMemoryGraphics.DrawImage(img, 400 + 42.5f - img->mWidth / 2 + BOARD_ADDITIONAL_WIDTH * 2,
			//	300 + 42.5f - img->mHeight / 2 + BOARD_OFFSET_Y);
		}
		
	}
	
	int targetMax = 223;

	if (mApp->IsAdventureMode() && mLevel == 59 || mApp->mGameMode == GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT)
	{
		targetMax = 191;
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT)
	{
		targetMax = 255;
	}

	if (mLevel == 51)
	{
		targetMax = 64;
	}
	else if (mLevel == 52)
	{
		targetMax = 96;
	}
	else if (mLevel == 53)
	{
		targetMax = 127;
	}
	else if (mLevel == 54)
	{
		targetMax = 191;
	}

	int targetAlpha = targetMax;

	if (mDarknessCounter > 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_LIGHTS_OUT)
		targetAlpha = TodAnimateCurveFloat(0, 550, mDarknessCounter, targetMax, 255, CURVE_LINEAR);

	g->SetColorizeImages(true);

	if (mApp->mGameScene == SCENE_LEVEL_INTRO)
	{
		targetAlpha = TodAnimateCurveFloat(6000, 7830, mCutScene->mCutsceneTime, targetMax * 0.875f, targetAlpha, CURVE_LINEAR);
	}
	else if (mApp->IsSurvivalMode() && mNextSurvivalStageCounter > 0)
	{
		targetAlpha = TodAnimateCurveFloat(500, 0, mNextSurvivalStageCounter, targetAlpha, targetMax * 0.875f, CURVE_LINEAR);
	}

	g->mColor.mAlpha = targetAlpha;

	float offsetX = -BOARD_ADDITIONAL_WIDTH;
	float offsetY = -BOARD_OFFSET_Y;

	/*if (mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT)
	{
		offsetX += -BOARD_ADDITIONAL_WIDTH * 2 + 60 + mCursorObject->mX;
		offsetY += -BOARD_OFFSET_Y * 5 + mCursorObject->mY;
	}*/

	g->mTransX += offsetX;
	g->mTransY += offsetY;

	if (mDarknessCounter > 0 && mApp->mGameMode != GAMEMODE_CHALLENGE_LIGHTS_OUT)
	{
		g->mColor.mAlpha = TodAnimateCurve(250, 550, mDarknessCounter, targetAlpha, 0, CURVE_LINEAR);
		g->DrawImage(mDarknessImage, 0, 0);
		g->mColor.mAlpha = TodAnimateCurve(0, 250, mDarknessCounter, 0, targetAlpha, CURVE_LINEAR);
		g->DrawImage(mDarknessDarkerImage, 0, 0);
	}
	else
	{
		g->DrawImage(mDarknessImage, 0, 0);
	}

	g->mTransX -= offsetX;
	g->mTransY -= offsetY;

	g->SetColorizeImages(false);

	DrawUIBottom(g);
	DrawTopRightUI(g);
}

//0x4173C0
bool Board::HasProgressMeter()
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID ||
		mApp->IsFinalBossLevel() || 
		mApp->IsSlotMachineLevel() || 
		mApp->mGameMode == GAMEMODE_CHALLENGE_RENEWABLE_ENERGY ||
		mApp->IsSquirrelLevel() || 
		mApp->IsIZombieLevel())
		return true;

	if (mProgressMeterWidth == 0 && mApp->mGameMode != GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		return false;

	if (mApp->IsContinuousChallenge() || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || 
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || 
		mApp->IsScaryPotterLevel())
		return false;

	return true;
}

//0x417450
bool Board::ProgressMeterHasFlags()
{
	if (mApp->IsFirstTimeAdventureMode() && mLevel == 1)
		return false;

	if (mApp->IsWhackAZombieLevel() ||
		mApp->IsFinalBossLevel() ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID ||
		mApp->IsSlotMachineLevel() ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_RENEWABLE_ENERGY ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL ||
		mApp->IsSquirrelLevel() ||
		mApp->IsIZombieLevel())
		return false;

	return true;
}

//0x417500
void Board::DrawProgressMeter(Graphics* g)
{
	if (!HasProgressMeter())
		return;

	bool vsMode = mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS;

	// ====================================================================================================
	// ▲ 绘制进度条进度部分的贴图
	// ====================================================================================================

	if (vsMode){
		g->DrawImage(Sexy::IMAGE_DUMMYMETER, 584 + BOARD_ADDITIONAL_WIDTH, 555 + BOARD_OFFSET_Y);
		g->DrawImage(Sexy::IMAGE_DUMMYMETER, 26 - BOARD_ADDITIONAL_WIDTH, 555 + BOARD_OFFSET_Y);
	}
	else{
		g->DrawImageCel(Sexy::IMAGE_FLAGMETER, 600 + BOARD_ADDITIONAL_WIDTH, 575 + BOARD_OFFSET_Y, 0);
	}
	int aCelWidth = Sexy::IMAGE_FLAGMETER->GetCelWidth();
	int aCelHeight = Sexy::IMAGE_FLAGMETER->GetCelHeight();
	int aClipWidth = TodAnimateCurve(0, PROGRESS_METER_COUNTER, mProgressMeterWidth, 0, 143, TodCurves::CURVE_LINEAR);
	Rect aSrcRect(aCelWidth - aClipWidth - 7 , aCelHeight , aClipWidth, aCelHeight);
	Rect aDstRect(aCelWidth - aClipWidth + 593 + BOARD_ADDITIONAL_WIDTH, 575 + BOARD_OFFSET_Y, aClipWidth, aCelHeight);

	if (mApp->mGameMode != GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		g->DrawImage(Sexy::IMAGE_FLAGMETER, aDstRect, aSrcRect);
	
	// ====================================================================================================
	// ▲ 根据不同关卡，绘制进度条上的文字或旗帜
	// ====================================================================================================
	int aPosX = aCelWidth / 2 + 600;
	Color aColor(224, 187, 98);
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
	{
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), mChallenge->mChallengeScore, 75, TodStringTranslate(_S("[MATCHES]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (mApp->IsSquirrelLevel())
	{
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), mChallenge->mChallengeScore, 7, TodStringTranslate(_S("[SQUIRRELS]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (mApp->IsSlotMachineLevel())
	{
		int maxSuns = 2000;
		if (mApp->mGameMode == GAMEMODE_CHALLENGE_SLOT_MACHINE_2) maxSuns = 2500;

		int aSunMoney = ClampInt(mSunMoney, 0, maxSuns);
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), aSunMoney, maxSuns, TodStringTranslate(_S("[SUN]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_RENEWABLE_ENERGY)
	{
		int aSunMoney = ClampInt(mSunProduced, 0, 12000);
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), aSunMoney, 12000, TodStringTranslate(_S("[SUN]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
	{
		int aSunMoney = ClampInt(mSunMoney, 0, 1000);
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), aSunMoney, 1000, TodStringTranslate(_S("[SUN]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (mApp->IsIZombieLevel())
	{
		SexyString aMatchStr = StrFormat(_S("%d / %d %s"), mChallenge->mChallengeScore, 5, TodStringTranslate(_S("[BRAINS]")).c_str());
		TodDrawString(g, aMatchStr, aPosX + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT12, aColor, DrawStringJustification::DS_ALIGN_CENTER);
	}
	else if (ProgressMeterHasFlags())
	{
		int aNumWavesPerFlag = GetNumWavesPerFlag();
		int aNumFlagWaves = mNumWaves / aNumWavesPerFlag;
		int aFlagsPosEnd = 590 + aCelWidth;  // 旗帜区域的右界横坐标
		for (int aFlagWave = 1; aFlagWave <= aNumFlagWaves; aFlagWave++)
		{
			// 取得旗帜升起时的高度偏移
			int aHeight = 0;
			int aTotalWavesAtFlag = aFlagWave * aNumWavesPerFlag;
			if (aTotalWavesAtFlag < mCurrentWave)
			{
				aHeight = 14;
			}
			else if (aTotalWavesAtFlag == mCurrentWave)
			{
				aHeight = TodAnimateCurve(100, 0, mFlagRaiseCounter, 0, 14, TodCurves::CURVE_LINEAR);
			}
			// 计算旗帜的横坐标
			int aPosX = TodAnimateCurve(0, mNumWaves, aTotalWavesAtFlag, aFlagsPosEnd, 606, TodCurves::CURVE_LINEAR);

			if (!vsMode)
			{
				// 绘制旗杆
				g->DrawImageCel(Sexy::IMAGE_FLAGMETERPARTS, aPosX + BOARD_ADDITIONAL_WIDTH, 571 + BOARD_OFFSET_Y, 1, 0);
				// 绘制旗帜
				g->DrawImageCel(Sexy::IMAGE_FLAGMETERPARTS, aPosX + BOARD_ADDITIONAL_WIDTH, 572 - aHeight + BOARD_OFFSET_Y, 2, 0);
			}
		}

		if (vsMode) {
			int xCountEnemy = (StageHas6Rows() ? 6 : 5);
			int xCount = xCountEnemy;

			Plant* aPlant = nullptr;
			while (IteratePlants(aPlant)) {
				if (aPlant->mIsDummy) {
					if (xCountEnemy >= 0 && aPlant->mEnemy)
						xCountEnemy--;
					if (xCount >= 0 && !aPlant->mEnemy)
						xCount--;
				}
			}

			for (int i = 0; i < min(xCountEnemy, 3); i++) {
				int aPosX = TodAnimateCurve(0, 3, i, aFlagsPosEnd, 606 + BOARD_ADDITIONAL_WIDTH, TodCurves::CURVE_LINEAR);
				g->DrawImage(Sexy::IMAGE_TARGET_X, aPosX - 43.75f, 571 - 12.5f + BOARD_OFFSET_Y);
			}

			for (int i = 0; i < min(xCount, 3); i++) {
				int aPosX = TodAnimateCurve(0, 3, i, 16 + aCelWidth, 32 - BOARD_ADDITIONAL_WIDTH, TodCurves::CURVE_LINEAR);
				g->DrawImage(Sexy::IMAGE_TARGET_X, aPosX - 43.75f + 16, 571 - 12.5f + BOARD_OFFSET_Y);
			}
		}
	}

	// ====================================================================================================
	// ▲ 绘制进度条的额外部分
	// ====================================================================================================
	// 绘制“关卡进程”的小牌子

	if (!vsMode)
		g->DrawImage(Sexy::IMAGE_FLAGMETERLEVELPROGRESS, 638 + BOARD_ADDITIONAL_WIDTH, 589 + BOARD_OFFSET_Y);

	// 判断是否需要绘制进度条当前位置处的小僵尸头，不需要则直接返回
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER ||
		mApp->IsSquirrelLevel() || 
		mApp->IsSlotMachineLevel() ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_RENEWABLE_ENERGY || 
		mApp->IsIZombieLevel() || 
		mApp->IsFinalBossLevel() || 
		mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		return;
	// 绘制僵尸头
	int aHeadProgress = TodAnimateCurve(0, 150, mProgressMeterWidth, 0, 135, CURVE_LINEAR);

	g->DrawImageCel(Sexy::IMAGE_FLAGMETERPARTS, aCelWidth - aHeadProgress + 580 + BOARD_ADDITIONAL_WIDTH, 572 + BOARD_OFFSET_Y, 0, 0);
}

//0x417C80
void Board::DrawHouseDoorBottom(Graphics* g)
{
	switch (mBackground)
	{
	case BackgroundType::BACKGROUND_1_DAY:		g->DrawImage(Sexy::IMAGE_BACKGROUND1_GAMEOVER_INTERIOR_OVERLAY, -126, 225);		break;
	case BackgroundType::BACKGROUND_2_NIGHT:	g->DrawImage(Sexy::IMAGE_BACKGROUND2_GAMEOVER_INTERIOR_OVERLAY, -125, 196);		break;
	case BackgroundType::BACKGROUND_3_POOL:
	case BackgroundType::BACKGROUND_3_POOLWIDE:
	case BackgroundType::BACKGROUND_3_UNSODDED:
	case BackgroundType::BACKGROUND_3_GRASS:
		g->DrawImage(Sexy::IMAGE_BACKGROUND3_GAMEOVER_INTERIOR_OVERLAY, -171, 241);		break;
	case BackgroundType::BACKGROUND_4_FOG:		g->DrawImage(Sexy::IMAGE_BACKGROUND4_GAMEOVER_INTERIOR_OVERLAY, -172, 246);		break;
	default:																													break;
	}
}

//0x417CF0
void Board::DrawHouseDoorTop(Graphics* g)
{
	switch (mBackground)
	{
	case BackgroundType::BACKGROUND_1_DAY:		g->DrawImage(Sexy::IMAGE_BACKGROUND1_GAMEOVER_MASK, -130, 202);		break;
	case BackgroundType::BACKGROUND_2_NIGHT:	g->DrawImage(Sexy::IMAGE_BACKGROUND2_GAMEOVER_MASK, -128, 207);		break;
	case BackgroundType::BACKGROUND_3_POOL:
	case BackgroundType::BACKGROUND_3_POOLWIDE:
	case BackgroundType::BACKGROUND_3_UNSODDED:
	case BackgroundType::BACKGROUND_3_GRASS:
		g->DrawImage(Sexy::IMAGE_BACKGROUND3_GAMEOVER_MASK, -172, 234);		break;
	case BackgroundType::BACKGROUND_4_FOG:		g->DrawImage(Sexy::IMAGE_BACKGROUND4_GAMEOVER_MASK, -173, 133);		break;
	case BackgroundType::BACKGROUND_5_ROOF:		g->DrawImage(Sexy::IMAGE_BACKGROUND5_GAMEOVER_MASK, -220, 81);		break;
	case BackgroundType::BACKGROUND_6_BOSS:		g->DrawImage(Sexy::IMAGE_BACKGROUND6_GAMEOVER_MASK, -220, 81);		break;
	default:																										break;
	}
}

void Board::DrawLevel(Graphics* g)
{
	// ====================================================================================================
	// ▲ 获取完整的关卡名称的字符串
	// ====================================================================================================
	SexyString aLevelStr;
	if (mApp->IsAdventureMode())
	{
		aLevelStr = TodStringTranslate(_S("[LEVEL]")) + _S("") + mApp->GetStageString(mLevel);
	}
	else
	{
		aLevelStr = mApp->GetCurrentChallengeDef().mChallengeName;

		if (mApp->IsSurvivalMode() || mApp->LawnApp::IsLastStandLevel(mApp->mGameMode))
		{
			int aFlags = GetSurvivalFlagsCompleted();
			if (aFlags > 0)
			{
				SexyString aFlagStr = mApp->Pluralize(aFlags, _S("[ONE_FLAG]"), _S("[COUNT_FLAGS]"));
				SexyString aCompletedStr = TodReplaceString(_S("[FLAGS_COMPLETED]"), _S("{FLAGS}"), aFlagStr);
				aLevelStr = StrFormat(_S("%s - %s"), TodStringTranslate(aLevelStr).c_str(), aCompletedStr.c_str());

			}
		}
		else if (mApp->IsEndlessIZombie(mApp->mGameMode) || mApp->IsEndlessScaryPotter(mApp->mGameMode))
		{
			int aStreak = mChallenge->mSurvivalStage;
			if (mNextSurvivalStageCounter > 0)
			{
				aStreak++;
			}
			if (aStreak > 0)
			{
				SexyString aStreakStr = TodReplaceNumberString(_S("[ENDLESS_STREAK]"), _S("{STREAK}"), aStreak);
				aLevelStr = StrFormat(_S("%s - %s"), TodStringTranslate(aLevelStr).c_str(), aStreakStr.c_str());
			}
		}
	}

	// ====================================================================================================
	// ▲ 正式开始绘制关卡名称字符串
	// ====================================================================================================
	int aPosX = 780;
	int aPosY = 595 + BOARD_OFFSET_Y;
	if (HasProgressMeter())
	{
		aPosX = 593;
	}
	if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_ZEN_FADING)
	{
		aPosY += TodAnimateCurve(50, 0, mChallenge->mChallengeStateCounter, 0, 50 + BOARD_OFFSET_Y, TodCurves::CURVE_EASE_IN_OUT);
	}

	aPosX += BOARD_ADDITIONAL_WIDTH;

	if (mIsVeteran)
	{
		SexyString aVeteranStr = TodStringTranslate(_S("[VETERAN]"));
		aLevelStr = StrFormat(_S("%s %s"), TodStringTranslate(aLevelStr).c_str(), aVeteranStr.c_str());
	}

	TodDrawString(g, aLevelStr, aPosX, aPosY, Sexy::FONT_HOUSEOFTERROR16, Color(224, 187, 98), DrawStringJustification::DS_ALIGN_RIGHT);
}
//0x4182D0
void Board::DrawZenWheelBarrowButton(Graphics* g, int theOffsetY)
{
	Rect aButtonRect = GetShovelButtonRect();
	GetZenButtonRect(GameObjectType::OBJECT_TYPE_WHEELBARROW, aButtonRect);
	PottedPlant* aPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
	if (aPlant && mCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW)
	{
		if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_ZEN_FADING)
		{
			g->DrawImage(Sexy::IMAGE_ZEN_WHEELBARROW, aButtonRect.mX - 7, aButtonRect.mY + theOffsetY - 3);
		}
		else
		{
			g->DrawImage(Sexy::IMAGE_ZEN_WHEELBARROW, aButtonRect.mX - 7, aButtonRect.mY + theOffsetY + 4);
		}

		if (aPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
		{
			mApp->mZenGarden->DrawPottedPlant(g, aButtonRect.mX + 23, aButtonRect.mY + theOffsetY - 8, aPlant, 0.6f, true);
		}
		else if (aPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
		{
			mApp->mZenGarden->DrawPottedPlant(g, aButtonRect.mX + 28, aButtonRect.mY + theOffsetY + 2, aPlant, 0.5f, true);
		}
		else
		{
			mApp->mZenGarden->DrawPottedPlant(g, aButtonRect.mX + 34, aButtonRect.mY + theOffsetY + 12, aPlant, 0.4f, true);
		}
	}
	else
	{
		g->DrawImage(Sexy::IMAGE_ZEN_WHEELBARROW, aButtonRect.mX - 7, aButtonRect.mY + theOffsetY - 3);
	}
}

//0x417DA0
void Board::DrawZenButtons(Graphics* g)
{
	int aOffsetY = 0;
	if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_ZEN_FADING)
	{
		aOffsetY = TodAnimateCurve(50, 0, mChallenge->mChallengeStateCounter, 0, -72 - BOARD_OFFSET_Y, TodCurves::CURVE_EASE_IN_OUT);
	}

	for (GameObjectType aTool = GameObjectType::OBJECT_TYPE_WATERING_CAN; aTool <= GameObjectType::OBJECT_TYPE_NEXT_GARDEN; aTool = (GameObjectType)(aTool + 1))
	{
		if (!CanUseGameObject(aTool))
			continue;

		Rect aButtonRect = GetShovelButtonRect();
		if (aTool == GameObjectType::OBJECT_TYPE_NEXT_GARDEN)
		{
			aButtonRect.mX = 564 - BOARD_ADDITIONAL_WIDTH;
			if (!mMenuButton->mBtnNoDraw)
			{
				g->DrawImage(Sexy::IMAGE_ZEN_NEXTGARDEN, aButtonRect.mX + 2, aButtonRect.mY + aOffsetY);
			}
		}
		else
		{
			GetZenButtonRect(aTool, aButtonRect);
			g->DrawImage(Sexy::IMAGE_SHOVELBANK, aButtonRect.mX, aButtonRect.mY + aOffsetY);
			if ((int)mCursorObject->mCursorType == (int)CursorType::CURSOR_TYPE_WATERING_CAN + (int)aTool - 6)
			{
				continue;  // 如果工具正在被手持，则跳过绘制
			}

			if (aTool == GameObjectType::OBJECT_TYPE_WATERING_CAN)
			{
				if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GOLD_WATERINGCAN])
				{
					g->DrawImage(Sexy::IMAGE_WATERINGCANGOLD, aButtonRect.mX - 2, aButtonRect.mY + aOffsetY - 6);
				}
				else
				{
					g->DrawImage(Sexy::IMAGE_WATERINGCAN, aButtonRect.mX - 2, aButtonRect.mY + aOffsetY - 6);
				}
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_FERTILIZER)
			{
				int aCharges = mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] - PURCHASE_COUNT_OFFSET;
				if (aCharges == 0)
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(96, 96, 96));
				}
				else if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_FERTILIZE_PLANTS)
				{
					g->SetColorizeImages(true);
					g->SetColor(GetFlashingColor(mMainCounter, 75));
				}
				g->DrawImage(Sexy::IMAGE_FERTILIZER, aButtonRect.mX - 6, aButtonRect.mY + aOffsetY - 7);
				g->SetColorizeImages(false);

				SexyString aChargeString = StrFormat(_S("x%d"), aCharges);
				TodDrawString(g, aChargeString, aButtonRect.mX + 64, aButtonRect.mY + aOffsetY + 65, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_BUG_SPRAY)
			{
				int aCharges = mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] - PURCHASE_COUNT_OFFSET;
				if (aCharges == 0)
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(128, 128, 128));
				}
				g->DrawImage(Sexy::IMAGE_BUG_SPRAY, aButtonRect.mX, aButtonRect.mY + aOffsetY - 1);
				g->SetColorizeImages(false);

				SexyString aChargeString = StrFormat(_S("x%d"), aCharges);
				TodDrawString(g, aChargeString, aButtonRect.mX + 64, aButtonRect.mY + aOffsetY + 65, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_PHONOGRAPH)
			{
				g->DrawImage(Sexy::IMAGE_PHONOGRAPH, aButtonRect.mX + 2, aButtonRect.mY + aOffsetY + 2);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_CHOCOLATE)
			{
				int aCharges = mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] - PURCHASE_COUNT_OFFSET;
				if (aCharges == 0)
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(128, 128, 128));
				}
				g->DrawImage(Sexy::IMAGE_CHOCOLATE, aButtonRect.mX + 6, aButtonRect.mY + aOffsetY + 4);
				g->SetColorizeImages(false);

				SexyString aChargeString = StrFormat(_S("x%d"), aCharges);
				TodDrawString(g, aChargeString, aButtonRect.mX + 64, aButtonRect.mY + aOffsetY + 65, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_GLOVE)
			{
				if (mCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE &&
					mCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW)
				{
					g->DrawImage(Sexy::IMAGE_ZEN_GARDENGLOVE, aButtonRect.mX - 6, aButtonRect.mY + aOffsetY - 4);
				}
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_MONEY_SIGN)
			{
				g->DrawImage(Sexy::IMAGE_ZEN_MONEYSIGN, aButtonRect.mX - 5, aButtonRect.mY + aOffsetY - 4);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_WHEELBARROW)
			{
				DrawZenWheelBarrowButton(g, aOffsetY);
			}
			else if (aTool == GameObjectType::OBJECT_TYPE_TREE_FOOD)
			{
				int aCharges = mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_FOOD] - PURCHASE_COUNT_OFFSET;
				if (aCharges <= 0)
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(128, 128, 128));
					aCharges = 0;
				}
				if (!mChallenge->TreeOfWisdomCanFeed())
				{
					g->SetColorizeImages(true);
					g->SetColor(Color(128, 128, 128));
				}
				g->DrawImage(Sexy::IMAGE_TREEFOOD, aButtonRect.mX - 6, aButtonRect.mY + aOffsetY - 7);
				g->SetColorizeImages(false);

				SexyString aChargeString = StrFormat(_S("x%d"), aCharges);
				TodDrawString(g, aChargeString, aButtonRect.mX + 64, aButtonRect.mY + aOffsetY + 65, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
			}
		}
	}
}


//0x418B70
void Board::DrawShovel(Graphics* g)
{
	if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM){
		if (mShowShovel){
			Rect aShovelRect = GetShovelButtonRect();

			for (int i = 0; i < mMaxPlayers; i++) {
				PlayerController* player = mPlayers[i];
				
				if (player->mAbilityType == ABILITY_SHOVEL)
				{
					g->DrawImage(Sexy::IMAGE_SHOVELBANK, aShovelRect.mX, aShovelRect.mY);

					if (mCursorObject->mCursorType != CursorType::CURSOR_TYPE_SHOVEL)
					{
						if (mChallenge->mChallengeState == (ChallengeState)15)
						{
							g->SetColorizeImages(true);
							g->SetColor(GetFlashingColor(mMainCounter, 75));
						}
						g->DrawImage(Sexy::IMAGE_SHOVEL, aShovelRect.mX - 7, aShovelRect.mY - 3);
						g->SetColorizeImages(false);
					}
				}
				
				/*if (player->mAbilityType == ABILITY_BUTTER) {
					aDarknessHeight = player->mAbilityTime / (float)player->mAbilityMaxTime * Sexy::IMAGE_BUTTER_ICON->GetHeight();

					g->DrawImage(Sexy::IMAGE_BUTTER_ICON, posX, posY - 3);

					g->SetColorizeImages(true);
					g->SetColor(Color(64, 64, 64, 255));
					g->SetClipRect(posX, posY - 3, Sexy::IMAGE_BUTTER_ICON->GetWidth(), aDarknessHeight);
					g->DrawImage(Sexy::IMAGE_BUTTER_ICON, posX, posY - 3);
					g->ClearClipRect();
					g->SetColorizeImages(false);
				}
				else if (player->mAbilityType == ABILITY_GLOVE) {
					aDarknessHeight = player->mAbilityTime / (float)player->mAbilityMaxTime * Sexy::IMAGE_ZEN_GARDENGLOVE->GetHeight();
					aDarknessHeight = (player->mGlovePlantID != PLANTID_NULL ? 1 : aDarknessHeight);

					g->DrawImage(Sexy::IMAGE_ZEN_GARDENGLOVE, posX - 6, posY - 3);

					g->SetColorizeImages(true);
					g->SetColor(Color(64, 64, 64, 255));
					g->SetClipRect(posX - 6, posY - 3, Sexy::IMAGE_BUTTER_ICON->GetWidth(), aDarknessHeight);
					g->DrawImage(Sexy::IMAGE_ZEN_GARDENGLOVE, posX- 6, posY - 3);
					g->ClearClipRect();
					g->SetColorizeImages(false);
				}
				else if (player->mAbilityType == ABILITY_HAMMER) {
					aDarknessHeight = player->mAbilityTime / (float)player->mAbilityMaxTime * Sexy::IMAGE_HAMMER_ICON->GetHeight();

					g->DrawImage(Sexy::IMAGE_HAMMER_ICON, posX - 6, posY - 3);

					g->SetColorizeImages(true);
					g->SetColor(Color(64, 64, 64, 255));
					g->SetClipRect(posX - 6, posY - 3, Sexy::IMAGE_BUTTER_ICON->GetWidth(), aDarknessHeight);
					g->DrawImage(Sexy::IMAGE_HAMMER_ICON, posX - 6, posY - 3);
					g->ClearClipRect();
					g->SetColorizeImages(false);
				}
				*/
			}
		}
		
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM )
	{
		DrawZenButtons(g);
	}
}

//0x418C70
void Board::DrawDebugText(Graphics* g)
{
	SexyString aText;

	switch (mDebugTextMode)
	{
	case DebugTextMode::DEBUG_TEXT_NONE:
		break;

	case DebugTextMode::DEBUG_TEXT_ZOMBIE_SPAWN:
	{
		int aTime = mZombieCountDownStart - mZombieCountDown;
		float aCountDownFraction = (float)aTime / (float)mZombieCountDownStart;

		aText += StrFormat(_S("ZOMBIE SPAWNING DEBUG\n"));
		aText += StrFormat(_S("CurrentWave: %d of %d\n"), mCurrentWave, mNumWaves);
		aText += StrFormat(_S("TimeSinseLastSpawn: %d %s\n"), aTime, aTime > 400 ? _S("") : _S("(too soon)"));
		aText += StrFormat(_S("ZombieCountDown: %d/%d (%.0f%%)\n"), mZombieCountDown, mZombieCountDownStart, aCountDownFraction);

		if (mZombieHealthToNextWave != -1)
		{
			int aTotalHealth = TotalZombiesHealthInWave(mCurrentWave - 1);
			int aHealthRange = max(mZombieHealthWaveStart - mZombieHealthToNextWave, 1);
			float aHealthFraction = (float)(mZombieHealthToNextWave - aTotalHealth + aHealthRange) / (float)aHealthRange;
			aText += StrFormat(_S("ZombieHealth: CurZombieHealth %d trigger %d (%.0f%%)\n"), aTotalHealth, mZombieHealthToNextWave, aHealthFraction * 100);
		}
		else
		{
			aText += StrFormat(_S("ZombieHealth: before first wave\n"));
		}

		if (mHugeWaveCountDown > 0)
		{
			aText += StrFormat(_S("HugeWaveCountDown: %d\n"), mHugeWaveCountDown);
		}

		Zombie* aBossZombie = GetBossZombie();
		if (aBossZombie)
		{
			aText += StrFormat(_S("\nSpawn: %d\n"), aBossZombie->mSummonCounter);
			aText += StrFormat(_S("Stomp: %d\n"), aBossZombie->mBossStompCounter);
			aText += StrFormat(_S("Bungee: %d\n"), aBossZombie->mBossBungeeCounter);
			aText += StrFormat(_S("Head: %d\n"), aBossZombie->mBossHeadCounter);
			aText += StrFormat(_S("Health: %d of %d\n"), aBossZombie->mBodyHealth, aBossZombie->mBodyMaxHealth);
		}

		break;
	}

	case DebugTextMode::DEBUG_TEXT_MUSIC:
	{
		aText += StrFormat(_S("MUSIC DEBUG\n"));
		aText += StrFormat(_S("CurrentWave: %d of %d\n"), mCurrentWave, mNumWaves);

		if (mApp->mMusic->mCurMusicFileMain == MusicFile::MUSIC_FILE_NONE)
		{
			aText += StrFormat(_S("No music"));
		}
		else
		{
			aText += StrFormat(_S("Music Burst: "));

			if (mApp->mMusic->mMusicBurstState == MusicBurstState::MUSIC_BURST_OFF)
			{
				aText += StrFormat(_S("Off"));
			}
			else if (mApp->mMusic->mMusicBurstState == MusicBurstState::MUSIC_BURST_STARTING)
			{
				aText += StrFormat(_S("Starting %d/%d"), mApp->mMusic->mBurstStateCounter, 400);
			}
			else if (mApp->mMusic->mMusicBurstState == MusicBurstState::MUSIC_BURST_ON)
			{
				aText += StrFormat(_S("On at least until %d/%d"), mApp->mMusic->mBurstStateCounter, 800);
			}
			else if (mApp->mMusic->mMusicBurstState == MusicBurstState::MUSIC_BURST_FINISHING)
			{
				aText += StrFormat(_S("Finishing %d/%d"), mApp->mMusic->mBurstStateCounter, 400);
			}

			if (mApp->mMusic->mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
			{
				aText += StrFormat(_S(", Drums off"));
			}
			else if (mApp->mMusic->mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_ON_QUEUED)
			{
				aText += StrFormat(_S(", Drums queued on"));
			}
			else if (mApp->mMusic->mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_ON)
			{
				aText += StrFormat(_S(", Drums on"));
			}
			else if (mApp->mMusic->mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED)
			{
				aText += StrFormat(_S(", Drums queued off"));
			}
			else if (mApp->mMusic->mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_FADING)
			{
				aText += StrFormat(_S(", Drums fading off %d/%d"), mApp->mMusic->mDrumsStateCounter, 50);
			}
			aText += StrFormat(_S("\n"));

			int aPackedOrderMain = mApp->mMusic->GetMusicOrder(mApp->mMusic->mCurMusicFileMain);
			int aCurrentOrder = LOWORD(aPackedOrderMain);
			aText += StrFormat(_S("Music order %02d row %02d\n"), LOWORD(aPackedOrderMain), HIWORD(aPackedOrderMain) / 4);
			if (mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK ||
				mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES ||
				mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST ||
				mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF)
			{
				int aPackedOrderHihats = mApp->mMusic->GetMusicOrder(mApp->mMusic->mCurMusicFileHihats);
				int aPackedOrderDrums = mApp->mMusic->GetMusicOrder(mApp->mMusic->mCurMusicFileDrums);
				if (aCurrentOrder == LOWORD(aPackedOrderHihats) && aCurrentOrder == LOWORD(aPackedOrderDrums))
				{
					int aDiffHihats = HIWORD(aPackedOrderHihats) - HIWORD(aPackedOrderMain);
					int aDiffDrums = HIWORD(aPackedOrderDrums) - HIWORD(aPackedOrderMain);
					if (abs(aDiffHihats) > 1 || abs(aDiffDrums) > 1)
					{
						aText += StrFormat(_S("Music unsynced hihats %d drums %d\n"), aDiffHihats, aDiffDrums);
					}
				}

				HMUSIC aMusicHandle1 = mApp->mMusic->GetBassMusicHandle(mApp->mMusic->mCurMusicFileMain);
				HMUSIC aMusicHandle2 = mApp->mMusic->GetBassMusicHandle(mApp->mMusic->mCurMusicFileHihats);
				HMUSIC aMusicHandle3 = mApp->mMusic->GetBassMusicHandle(mApp->mMusic->mCurMusicFileDrums);
				//int bpm1 = gBass->BASS_MusicGetAttribute(aMusicHandle1, BASS_MUSIC_ATTRIB_BPM);
				//int bpm2 = gBass->BASS_MusicGetAttribute(aMusicHandle2, BASS_MUSIC_ATTRIB_BPM);
				//int bpm3 = gBass->BASS_MusicGetAttribute(aMusicHandle3, BASS_MUSIC_ATTRIB_BPM);
				//aText += StrFormat(_S("bpm1 %d bmp2 %d bpm3 %d\n"), bpm1, bpm2, bpm3);
			}
			else if (mApp->mMusic->mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS)
			{
				int aPackedOrderDrums = mApp->mMusic->GetMusicOrder(mApp->mMusic->mCurMusicFileDrums);
				aText += StrFormat(_S("Drum order %02d row %02d\n"), LOWORD(aPackedOrderDrums), HIWORD(aPackedOrderDrums) / 4);
				int aDiffDrums = HIWORD(aPackedOrderDrums) - HIWORD(aPackedOrderMain);
				if (abs(aDiffDrums) > 0 && abs(aDiffDrums) <= 128)
				{
					aText += StrFormat(_S("Drums unsynced %d"), aDiffDrums);
				}
			}
		}

		break;
	}

	case DebugTextMode::DEBUG_TEXT_MEMORY:
		aText += StrFormat(_S("MEMORY DEBUG\n"));
		aText += StrFormat(_S("attachments %d\n"), mApp->mEffectSystem->mAttachmentHolder->mAttachments.mSize);
		aText += StrFormat(_S("emitters %d\n"), mApp->mEffectSystem->mParticleHolder->mEmitters.mSize);
		aText += StrFormat(_S("particles %d\n"), mApp->mEffectSystem->mParticleHolder->mParticles.mSize);
		aText += StrFormat(_S("particle systems %d\n"), mApp->mEffectSystem->mParticleHolder->mParticleSystems.mSize);
		aText += StrFormat(_S("trails %d\n"), mApp->mEffectSystem->mTrailHolder->mTrails.mSize);
		aText += StrFormat(_S("reanimation %d\n"), mApp->mEffectSystem->mReanimationHolder->mReanimations.mSize);
		aText += StrFormat(_S("zombies %d\n"), mZombies.mSize);
		aText += StrFormat(_S("plants %d\n"), mPlants.mSize);
		aText += StrFormat(_S("projectiles %d\n"), mProjectiles.mSize);
		aText += StrFormat(_S("coins %d\n"), mCoins.mSize);
		aText += StrFormat(_S("lawn mowers %d\n"), mLawnMowers.mSize);
		aText += StrFormat(_S("grid items %d\n"), mGridItems.mSize);
		break;

	case DebugTextMode::DEBUG_TEXT_COLLISION:
		aText += StrFormat(_S("COLLISION DEBUG\n"));
		break;

	default:
		TOD_ASSERT();
		break;
	}

	g->SetFont(mDebugFont);
	g->SetColor(Color::Black);
	g->DrawStringWordWrapped(aText, 10, 89);
	g->DrawStringWordWrapped(aText, 11, 91);
	g->DrawStringWordWrapped(aText, 9, 90);
	g->DrawStringWordWrapped(aText, 11, 90);
	g->SetColor(Color(255, 255, 255));
	g->DrawStringWordWrapped(aText, 10, 90);
}

//0x419AE0
void Board::DrawDebugObjectRects(Graphics* g)
{
	if (mDebugTextMode != DebugTextMode::DEBUG_TEXT_COLLISION)
		return;

	{
		Plant* aPlant = nullptr;
		while (IteratePlants(aPlant))
		{
			Rect aRect = aPlant->GetPlantRect();
			g->SetColor(Color(0, 255, 0));
			g->DrawRect(aRect);

			Rect aAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			if (aAttackRect.mWidth < BOARD_WIDTH)
			{
				g->SetColor(Color(255, 0, 0));
				g->DrawRect(aAttackRect);
			}

			Rect aSecondaryRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_SECONDARY);
			if (aSecondaryRect.mWidth < BOARD_WIDTH)
			{
				g->SetColor(Color(255, 0, 128));
				g->DrawRect(aSecondaryRect);
			}
		}
	}
	{
		Zombie* aZombie = nullptr;
		while (IterateZombies(aZombie))
		{
			if (!aZombie->IsDeadOrDying())
			{
				Rect aRect = aZombie->GetZombieRect();
				g->SetColor(Color(0, 255, 0));
				g->DrawRect(aRect);

				Rect aAttackRect = aZombie->GetZombieAttackRect();
				g->SetColor(Color(255, 0, 0));
				g->DrawRect(aAttackRect);
			}
		}
	}
	{
		LawnMower* aLawnMower = nullptr;
		while (IterateLawnMowers(aLawnMower))
		{
			Rect aAttackRect = aLawnMower->GetLawnMowerAttackRect();
			g->SetColor(Color(255, 0, 0));
			g->DrawRect(aAttackRect);
		}
	}
	{
		Projectile* aProjectile = nullptr;
		while (IterateProjectiles(aProjectile))
		{
			g->SetColor(Color(255, 0, 0));
			Rect aDamageRect = aProjectile->GetProjectileRect();
			g->DrawRect(aDamageRect);
		}
	}
}

//0x419EB0
void Board::DrawFadeOut(Graphics* g)
{
	if (mBoardFadeOutCounter < 0 || IsSurvivalStageWithRepick())
		return;

	int anAlpha = TodAnimateCurve(200, 0, mBoardFadeOutCounter, 0, 255, TodCurves::CURVE_LINEAR);
	if (mLevel == 9 || mLevel == 19 || mLevel == 29 || mLevel == 39 || mLevel == 49)
	{
		g->SetColor(Color(0, 0, 0, anAlpha));
	}
	else
	{
		g->SetColor(Color(255, 255, 255, anAlpha));
	}
	g->FillRect(-BOARD_ADDITIONAL_WIDTH, -BOARD_OFFSET_Y, mWidth + BOARD_ADDITIONAL_WIDTH, mHeight + BOARD_OFFSET_Y);
}

//0x419F60
void Board::DrawTopRightUI(Graphics* g)
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (mChallenge->mChallengeState == STATECHALLENGE_ZEN_FADING)
		{
			mSpeedButton->mY = mMenuButton->mY = TodAnimateCurve(50, 0, mChallenge->mChallengeStateCounter, -10 - BOARD_OFFSET_Y, -50 - BOARD_OFFSET_Y, TodCurves::CURVE_EASE_IN_OUT);
			mStoreButton->mX = TodAnimateCurve(50, 0, mChallenge->mChallengeStateCounter, 678 + BOARD_ADDITIONAL_WIDTH, BOARD_WIDTH + BOARD_ADDITIONAL_WIDTH, TodCurves::CURVE_EASE_IN_OUT);
		}
		else
		{
			mSpeedButton->mY = mMenuButton->mY = -10 - BOARD_OFFSET_Y;
			mStoreButton->mX = 678 + BOARD_ADDITIONAL_WIDTH;
		}
	}

	if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_COMPLETED)
	{
		g->SetColorizeImages(true);
		g->SetColor(GetFlashingColor(mMainCounter, 75));
	}

	//mMenuButton->Draw(g);
	g->SetColorizeImages(false);

	if (mStoreButton && !LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_VISIT_STORE)
		{
			g->SetColorizeImages(true);
			g->SetColor(GetFlashingColor(mMainCounter, 75));
		}
		mStoreButton->Draw(g);
		g->SetColorizeImages(false);
	}
}

//0x41A0A0
void Board::DrawUIBottom(Graphics* g)
{
	if (mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
	{
		int aWaveTime = abs(mMainCounter / 8 % 22 - 11);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->DrawImageCel(Sexy::IMAGE_WAVESIDE, -240, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 160 - 240, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 320 - 240, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 320 - 240 - 80, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 320 - 80, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 320 + 80, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 320 + 240, 40, aWaveTime);
		g->DrawImageCel(Sexy::IMAGE_WAVECENTER, 480 + 240, 40, aWaveTime);
		TodDrawImageCelScaled(g, Sexy::IMAGE_WAVESIDE, 800 + 240, 40, 0, aWaveTime, -1.0f, 1.0f);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}

	if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE || mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->DrawImage(
			IMAGE_BACKGROUND_GREENHOUSE_OVERLAY, 
			Rect(-BOARD_ADDITIONAL_WIDTH, -BOARD_OFFSET_Y, BOARD_WIDTH, BOARD_HEIGHT), 
			Rect(0, 0, IMAGE_BACKGROUND_GREENHOUSE_OVERLAY->mWidth, IMAGE_BACKGROUND_GREENHOUSE_OVERLAY->mHeight)
		);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}

	if (mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON)
	{
		if (mSeedBank->BeginDraw(g))
		{
			mSeedBank->Draw(g);
			mSeedBank->EndDraw(g);
		}

		if (mApp->HasSecondarySeedBank()) {
			if (mSeedBankP2->BeginDraw(g))
			{
				mSeedBankP2->Draw(g);
				mSeedBankP2->EndDraw(g);
			}
		}

		if (mAdvice->mMessageStyle == MessageStyle::MESSAGE_STYLE_SLOT_MACHINE)
		{
			mAdvice->Draw(g);
		}
	}

	DrawShovel(g);
	
	if (!StageHasFog())
	{
		DrawTopRightUI(g);
	}
}

//0x41A2A0
void Board::DrawUICoinBank(Graphics* g)
{
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING && mApp->mCrazyDaveState == CrazyDaveState::CRAZY_DAVE_OFF)
		return;

	if (mCoinBankFadeCount <= 0)
		return;

	int aPosX = 57;
	int aPosY = 599 - Sexy::IMAGE_COINBANK->GetHeight() + BOARD_OFFSET_Y;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mCrazyDaveState != CrazyDaveState::CRAZY_DAVE_OFF)
	{
		aPosX = 450 - mX;
	}

	aPosX -= BOARD_ADDITIONAL_WIDTH;

	g->SetColorizeImages(true);
	int anAlpha = ClampInt(255 * mCoinBankFadeCount / 15, 0, 255);
	g->SetColor(Color(255, 255, 255, anAlpha));
	g->DrawImage(Sexy::IMAGE_COINBANK, aPosX, aPosY);

	g->SetColor(Color(180, 255, 90, anAlpha));
	g->SetFont(Sexy::FONT_CONTINUUMBOLD14);
	SexyString aCoinLabel = mApp->GetMoneyString(mApp->mPlayerInfo->mCoins);
	g->DrawString(aCoinLabel, aPosX + 116 - Sexy::FONT_CONTINUUMBOLD14->StringWidth(aCoinLabel), aPosY + 24);
	g->SetColorizeImages(false);
}

//0x41A430
void Board::ClearFogAroundPlant(Plant* thePlant, int theSize)
{
	int aFogFadeOutSpeed = mFogBlownCountDown >= 2000 ? 40 : mFogBlownCountDown > 0 ? 2 : 6;

	int aLeft = LeftFogColumn();
	int aFogOffsetX = (mFogOffset + 50) / 100;
	int aStartX = thePlant->mPlantCol - theSize - aFogOffsetX;
	int aEndX = thePlant->mPlantCol + theSize - aFogOffsetX;
	aStartX = max(aStartX, aLeft);
	aEndX = min(aEndX, MAX_GRID_SIZE_X + 4);

	int row = thePlant->mRow + 2;

	int aStartY = row - theSize;
	int aEndY = row + theSize;
	aStartY = max(aStartY, 0);
	aEndY = min(aEndY, MAX_GRID_SIZE_Y + 4);

	for (int x = aStartX; x <= aEndX; x++)
	{
		for (int y = aStartY; y <= aEndY; y++)
		{
			int aDistX = abs(x + aFogOffsetX - thePlant->mPlantCol);
			int aDistY = abs(y - row);
			/*
			if (theSize == 4)
			{
				if (aDistX > 3 || aDistY > 2) {
					continue;
				}
			}
			else if (aDistX + aDistY > theSize)
			{
				continue;
			}*/

			if (aDistX > max(theSize - 1, 0) || aDistY > max(theSize - 2, 0)) {
				continue;
			}
			


			mFogGridCelFog[x][y] = max(mFogGridCelFog[x][y] - aFogFadeOutSpeed, 0);
		}
	}
}


//0x41A5D0
void Board::UpdateFog()
{
	if (!StageHasFog())
		return;

	int aFogFadeInSpeed = mFogBlownCountDown >= 2000 ? 20 : mFogBlownCountDown > 0 ? 1 : 3;
	//int aFogFadeInSpeed = 3;
	if (mFogBlownCountDown > 0 && mFogBlownCountDown < 2000)
	{
		aFogFadeInSpeed = 1;
	}
	else if (mFogBlownCountDown > 0)
	{
		aFogFadeInSpeed = 20;
	}

	int aLeft = LeftFogColumn();
	for (int x = aLeft; x < MAX_GRID_SIZE_X + 4; x++)
	{
		for (int y = 0; y < MAX_GRID_SIZE_Y + 4; y++)
		{
			int aFogMax = x == aLeft ? 200 : 255;
			mFogGridCelFog[x][y] = min(mFogGridCelFog[x][y] + aFogFadeInSpeed, aFogMax);
		}
	}

	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->NotOnGround())
			continue;

		if (aPlant->mSeedType == SeedType::SEED_PLANTERN)
			ClearFogAroundPlant(aPlant, 4);
		else if (aPlant->mSeedType == SEED_PLASMAWOOD)
			ClearFogAroundPlant(aPlant, 3);
		else if (aPlant->mSeedType == SEED_TORCHWOOD || aPlant->mSeedType == SEED_FIRESHROOM)
			ClearFogAroundPlant(aPlant, 2);
		
	}
}

//0x41A730
void Board::DrawFog(Graphics* g)
{
	Image* aImageFog = mApp->Is3dAccel() ? Sexy::IMAGE_FOG : Sexy::IMAGE_FOG_SOFTWARE;

	for (int x = 0; x < MAX_GRID_SIZE_X + 4; x++)
	{
		for (int y = 0; y < MAX_GRID_SIZE_Y + 4; y++)
		{
			int aFadeAmount = mFogGridCelFog[x][y];
			if (aFadeAmount == 0)
				continue;

			// 取得格子内的雾的形状（第 6 行的雾的形状采用与第 0 行相同）
			// { sub eax,edx } 向前 [y / 6] 列，但 y 超出上限 y - 5 行，故相当于列不变，行 = y % 6；
			int aCelLook = mFogGridCelLook[x][y % MAX_GRID_SIZE_Y];
			int aCelCol =  aCelLook % 8;
			// 本格浓雾横坐标 = 列 * 80 + 浓雾偏移 - 15，纵坐标 = 行 * 85 + 20
			float aPosX = x * 80 + mFogOffset - 15;
			float aPosY = y * 80 + 20;

			float aSlopeOffset = 0.0f;
			if (aPosX < 440.0f && StageHasRoof()){
				aSlopeOffset = (440.0f - aPosX) * 0.25f;
				aPosY += aSlopeOffset;
			}
			
			// 开始计算周期变化的颜色，aTime 为根据主计时计算的时间
			float aTime = mMainCounter * PI * 2;
			// 与行、列有关的初始相位
			float aPhaseX = 6 * PI * x / (MAX_GRID_SIZE_X + 4);
			float aPhaseY = 6 * PI * y / (MAX_GRID_SIZE_Y + 1);
			// 根据初相和时间计算当前相位
			float aMotion = 13 + 4 * sin(aTime / 900 + aPhaseY) + 8 * sin(aTime / 500 + aPhaseX);

			int aColorVariant = 255 - aCelLook * 1.5 - aMotion * 1.5;
			int aLightnessVariant = 255 - aCelLook - aMotion;

			if (!mApp->Is3dAccel())
			{
				aPosX += 10;
				aPosY += 3;
				aCelCol = aCelLook % Sexy::IMAGE_FOG_SOFTWARE->mNumCols;
				aColorVariant = 255;
				aLightnessVariant = 255;
			}

			aPosY -= 160;

			int aCounter = mMainCounter;

			if (mApp->mGameScene != SCENE_PLAYING)
			{
				int mCutsceneTime = mCutScene->mCutsceneTime;
				int mEndCutscene = 6000 + mCutScene->mLawnMowerTime + mCutScene->mSodTime + mCutScene->mGraveStoneTime + mCutScene->mCrazyDaveTime + mCutScene->mFogTime + mCutScene->mBossTime + mCutScene->mReadySetPlantTime;

				aCounter = TodAnimateCurveFloat(mEndCutscene - 1000, mEndCutscene, mCutsceneTime, gLawnApp->mAppCounter % 400, mMainCounter % 200, CURVE_LINEAR);
			}
			else
			{
				aCounter = mMainCounter;
			}


			float aTime2 = aCounter * 2.0f * PI / 200.0f;

			float aPos = x * PI + y * 0.25f * PI;
			float aOffset = sin(aPos + aTime2) * 2.0f;
			aPosY += aOffset;

			aPos = y * PI + x * 0.25f * PI;
			aOffset = sin(aPos + aTime2) * 2.0f;
			aPosX -= aOffset;

			if (mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
			{
				int daColor = 128;
				g->SetColorizeImages(true);

				g->SetColor(Color(aColorVariant, aColorVariant, aLightnessVariant - daColor, aFadeAmount));
				g->DrawImageCelF(aImageFog, aPosX, aPosY, aCelCol, 0);

				g->SetColor(Color(daColor, daColor, 0, aFadeAmount));
				g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
				g->DrawImageCelF(aImageFog, aPosX, aPosY, aCelCol, 0);

				g->SetColorizeImages(false);
				g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
			}
			else
			{
				g->SetColorizeImages(true);
				g->SetColor(Color(aColorVariant, aColorVariant, aLightnessVariant, aFadeAmount));
				g->DrawImageCelF(aImageFog, aPosX, aPosY, aCelCol, 0);
				g->SetColorizeImages(false);
			}
		}
	}

	DrawUIBottom(g);
}

//0x41AA00
bool Board::IsScaryPotterDaveTalking()
{
	return mApp->IsScaryPotterLevel() && mNextSurvivalStageCounter > 0 && mApp->mCrazyDaveState != CrazyDaveState::CRAZY_DAVE_OFF;
}

//0x41AA40
void Board::DrawUITop(Graphics* g)
{
	if (StageHasFog())
	{
		DrawTopRightUI(g);
	}

	if (mTimeStopCounter > 0)
	{
		g->SetColor(Color(200, 200, 200, 210));
		g->FillRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
	}

	if (mApp->mGameScene == GameScenes::SCENE_PLAYING || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		DrawProgressMeter(g);
		DrawLevel(g);
	}

	if (mStoreButton && LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		Graphics storeG(*g);
		storeG.mTransX -= BOARD_ADDITIONAL_WIDTH;
		storeG.mTransY -= BOARD_OFFSET_Y;
		mStoreButton->Draw(&storeG);
	}

	if ((mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO) && mCutScene->mUpsellHideBoard)
	{
		g->SetColor(Color(0, 0, 0));
		g->FillRect(-BOARD_ADDITIONAL_WIDTH, -BOARD_OFFSET_Y, BOARD_WIDTH + BOARD_ADDITIONAL_WIDTH, BOARD_HEIGHT + BOARD_OFFSET_Y);
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_UPSELL)
	{
		mCutScene->DrawUpsell(g);
	}
	else if (mApp->mGameMode == GameMode::GAMEMODE_INTRO)
	{
		mCutScene->DrawIntro(g);
	}

	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN ||
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || 
		IsScaryPotterDaveTalking())
	{
		Graphics aScreenSpace(*g);
		aScreenSpace.mTransX -= mX;
		aScreenSpace.mTransY -= mY;
		mApp->DrawCrazyDave(&aScreenSpace);
	}

	if (mAdvice->mMessageStyle != MessageStyle::MESSAGE_STYLE_SLOT_MACHINE)
	{
		mAdvice->Draw(g);
	}

	if (mTimeStopCounter == 0 && mCursorObject->BeginDraw(g))
	{
		mCursorObject->Draw(g);
		mCursorObject->EndDraw(g);
	}

	mToolTip->Draw(g);
	DrawDebugText(g);
	DrawDebugObjectRects(g);

	if (mApp->mGameScene == SCENE_PLAYING) {
		for (int i = 0; i < mMaxPlayers; i++) {
			PlayerController* player = mPlayers[i];
			if (player->mControlType == CONTROL_MOUSE) continue;
			player->Draw(g);
		}
	}

	/*
	if (mIsP1Keyboard && mApp->mGameScene == SCENE_PLAYING)
	{
		if (P1HasShovel)
		{
			float xPos = 0.0f, yPos = 0.0f, selectorY = 0.0f;

			if (StageHasPool())
				selectorY = P1SelectorGridY / 5.0f * 15;

			if (StageHasRoof())
				selectorY += 15;

			if (P1SelectorTime < 25) {
				selectorY -= 1.5f;
				selectorY += P1SelectorTime / 25.0f * 1.5f;
			}
			else if (P1SelectorTime >= 25 && P1SelectorTime < 50) {
				selectorY += (P1SelectorTime - 25.0f) / 25.0f * 1.5f;
			}
			else if (P1SelectorTime >= 50 && P1SelectorTime < 75) {
				selectorY += 1.5f;
				selectorY -= (P1SelectorTime - 50.0f) / 25.0f * 1.5f;
			}
			else if (P1SelectorTime >= 75 && P1SelectorTime < 100) {
				selectorY -= (P1SelectorTime - 75.0f) / 25.0f * 1.5f;
			}

			xPos = GridToPixelX(P1SelectorGridX, P1SelectorGridY);
			yPos = GridToPixelY(P1SelectorGridX, P1SelectorGridY) + selectorY;

			g->DrawImageF(IMAGE_SHOVEL, xPos + 10, yPos);
		}

		if (P1HasPlant && P1SeedBankID > -1)
		{
			SeedType seedType = mSeedBank->mSeedPackets[P1SeedBankID].mPacketType;
			SeedType imitaterType = mSeedBank->mSeedPackets[P1SeedBankID].mImitaterType;

			if (seedType == SEED_IMITATER) {
				seedType = imitaterType;
				imitaterType = mSeedBank->mSeedPackets[P1SeedBankID].mPacketType;
			}
	
			if ((P1CurPlant->mSeedType != seedType || P1CurPlant->mImitaterType != imitaterType) && P1CobCanonPlantID == PLANTID_NULL) {
				P1CurPlant->PlantInitialize(0, 0, seedType, imitaterType);
				if (imitaterType == SEED_IMITATER){
					FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
					if (imitaterType == SeedType::SEED_HYPNOSHROOM || imitaterType == SeedType::SEED_SQUASH || imitaterType == SeedType::SEED_POTATOMINE ||
						imitaterType == SeedType::SEED_GARLIC || imitaterType == SeedType::SEED_LILYPAD)
						aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

					Reanimation* aBodyReanim = mApp->ReanimationTryToGet(P1CurPlant->mBodyReanimID);
					if (aBodyReanim)
					{
						aBodyReanim->mFilterEffect = aFilter;
					}
					Reanimation* aHeadReanim = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID);
					if (aHeadReanim)
					{
						aHeadReanim->mFilterEffect = aFilter;
					}Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID2);
					if (aHeadReanim2)
					{
						aHeadReanim2->mFilterEffect = aFilter;
					}Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID3);
					if (aHeadReanim3)
					{
						aHeadReanim3->mFilterEffect = aFilter;
					}
				}
				else {
					Reanimation* aBodyReanim = mApp->ReanimationTryToGet(P1CurPlant->mBodyReanimID);
					if (aBodyReanim)
					{
						aBodyReanim->mFilterEffect = FILTER_EFFECT_NONE;
					}
					Reanimation* aHeadReanim = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID);
					if (aHeadReanim)
					{
						aHeadReanim->mFilterEffect = FILTER_EFFECT_NONE;
					}Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID2);
					if (aHeadReanim2)
					{
						aHeadReanim2->mFilterEffect = FILTER_EFFECT_NONE;
					}Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(P1CurPlant->mHeadReanimID3);
					if (aHeadReanim3)
					{
						aHeadReanim3->mFilterEffect = FILTER_EFFECT_NONE;
					}
				}
			}

			P1CurPlant->mX = GridToPixelX(P1SelectorGridX, P1SelectorGridY);
			P1CurPlant->mY = GridToPixelY(P1SelectorGridX, P1SelectorGridY);

			P1CurPlant->mPlantCol = P1SelectorGridX;
			P1CurPlant->mRow = P1SelectorGridY;

			if (P1CurPlant->mSeedType != SEED_NONE && P1CurPlant->mSeedType < NUM_SEED_TYPES && P1CobCanonPlantID == PLANTID_NULL){
				bool mDarken = CanPlantAt(P1SelectorGridX, P1SelectorGridY, seedType) != PLANTING_OK || !mSeedBank->mSeedPackets[P1SeedBankID].mActive || ! CanTakeSunMoney(GetCurrentPlantCost(seedType, SeedType::SEED_NONE));

				if (!mDarken && !mPaused){
					P1CurPlant->Update();

					if (mIsSpeeding)
						P1CurPlant->Update();
				}

				Graphics plantG(*g);
				plantG.SetColorizeImages(true);
				plantG.mColor = mDarken ? Color::Black : Color::White;
				plantG.mColor.mAlpha = 64;

				if (P1CurPlant->BeginDraw(&plantG)) {
					P1CurPlant->Draw(&plantG);
					P1CurPlant->EndDraw(&plantG);
				}
			}
		}
	}*/
	
	mMenuButton->Draw(g);
	mSpeedButton->Draw(g);
}

//0x41ACF0
void Board::Draw(Graphics* g)
{
	if (mApp->GetDialog(Dialogs::DIALOG_STORE) || mApp->GetDialog(Dialogs::DIALOG_ALMANAC))
		return;

	g->SetLinearBlend(true);

	if (mDrawCount && mCutScene->mPreloaded)
	{
		int aTickCount = GetTickCount();
		int aIntervalDraws = mDrawCount - mIntervalDrawCountStart;
		int aInterval = aTickCount - mIntervalDrawTime;
		if (aInterval > 10000)
		{
			float aIntervalFPS = (aIntervalDraws * 1000 + 500) / aInterval;
			if (mMinFPS > aIntervalFPS)
			{
				mMinFPS = aIntervalFPS;
			}
			mIntervalDrawCountStart = mDrawCount;
			mIntervalDrawTime = aTickCount;
		}
	}
	else
	{
		ResetFPSStats();
	}

	mDrawCount++;
	DrawGameObjects(g);
}

//0x41AE60
void Board::SetMustacheMode(bool theEnableMustache)
{
	mApp->PlayFoley(FoleyType::FOLEY_POLEVAULT);
	mMustacheMode = theEnableMustache;
	mApp->mMustacheMode = theEnableMustache;

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		aZombie->EnableMustache(theEnableMustache);
	}
}

//0x41AF00
void Board::SetFutureMode(bool theEnableFuture)
{
	mApp->PlaySample(Sexy::SOUND_BOING);
	mFutureMode = theEnableFuture;
	mApp->mFutureMode = theEnableFuture;

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		aZombie->EnableFuture(theEnableFuture);
	}
}

//0x41AF60
void Board::SetPinataMode(bool theEnablePinata)
{
	mApp->PlayFoley(FoleyType::FOLEY_JUICY);
	mPinataMode = theEnablePinata;
	mApp->mPinataMode = theEnablePinata;
}

//0x41AFD0
void Board::SetDanceMode(bool theEnableDance)
{
	mApp->PlayFoley(FoleyType::FOLEY_DANCER);
	mDanceMode = theEnableDance;
	mApp->mDanceMode = theEnableDance;

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (!aZombie->mDead && mApp->mGameMode != GAMEMODE_CHALLENGE_DANCE_PARTY && mApp->mGameMode != GAMEMODE_CHALLENGE_WEIRDMAGEDDON)
		{
			aZombie->EnableDance(theEnableDance);
		}
	}
}

//0x41B0D0
void Board::SetSuperMowerMode(bool theEnableSuperMower)
{
	mApp->PlayFoley(FoleyType::FOLEY_ZAMBONI);
	mSuperMowerMode = theEnableSuperMower;
	mApp->mSuperMowerMode = theEnableSuperMower;

	LawnMower* aLawnMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		aLawnMower->EnableSuperMower(theEnableSuperMower);
	}
}

void Board::SetDaisyMode(bool theEnableDaisy)
{
	mApp->PlaySample(SOUND_LOADINGBAR_FLOWER);
	mDaisyMode = theEnableDaisy;
	mApp->mDaisyMode = theEnableDaisy;
}

void Board::SetSukhbirMode(bool theEnableSukhbir)
{
	mApp->PlaySample(Sexy::SOUND_SUKHBIR);
	mSukhbirMode = theEnableSukhbir;
	mApp->mSukhbirMode = theEnableSukhbir;
}

//0x41B1D0
void Board::DoTypingCheck(KeyCode theKey)
{
	if (mApp->mKonamiCheck->Check(theKey))
	{
		mApp->PlayFoley(FoleyType::FOLEY_DROP);
		return;
	}
	if (mApp->mMustacheCheck->Check(theKey) || mApp->mMoustacheCheck->Check(theKey))
	{
		SetMustacheMode(!mMustacheMode);
		return;
	}
	if (mApp->mSuperMowerCheck->Check(theKey) || mApp->mSuperMowerCheck2->Check(theKey))
	{
		SetSuperMowerMode(!mSuperMowerMode);
		return;
	}
	if (mApp->mFutureCheck->Check(theKey))
	{
		SetFutureMode(!mFutureMode);
		return;
	}
	if (mApp->mPinataCheck->Check(theKey))
	{
		if (mApp->CanDoPinataMode())
		{
			SetPinataMode(!mPinataMode);
			return;
		}
		else
		{
			if (mApp->mGameScene == GameScenes::SCENE_PLAYING)
			{
				DisplayAdvice(_S("[CANT_USE_CODE]"), MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST, AdviceType::ADVICE_NONE);
			}
			mApp->PlaySample(Sexy::SOUND_BUZZER);
			return;
		}
	}
	if (mApp->mDanceCheck->Check(theKey))
	{
		if (mApp->CanDoDanceMode())
		{
			SetDanceMode(!mDanceMode);
			return;
		}
		else
		{
			if (mApp->mGameScene == GameScenes::SCENE_PLAYING)
			{
				DisplayAdvice(_S("[CANT_USE_CODE]"), MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST, AdviceType::ADVICE_NONE);
			}
			mApp->PlaySample(Sexy::SOUND_BUZZER);
			return;
		}
	}
	if (mApp->mDaisyCheck->Check(theKey))
	{
		if (mApp->CanDoDaisyMode())
		{
			SetDaisyMode(!mDaisyMode);
			return;
		}
		else
		{
			if (mApp->mGameScene == GameScenes::SCENE_PLAYING)
			{
				DisplayAdvice(_S("[CANT_USE_CODE]"), MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST, AdviceType::ADVICE_NONE);
			}
			mApp->PlaySample(Sexy::SOUND_BUZZER);
			return;
		}
	}
	if (mApp->mSukhbirCheck->Check(theKey))
	{
		SetSukhbirMode(!mSukhbirMode);
		return;
	}
}

//0x41B820
void Board::KeyDown(KeyCode theKey)
{
	DoTypingCheck(theKey);

	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && 
		mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && 
		mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		mCutScene->KeyDown(theKey);
	}
	else if (theKey == KeyCode::KEYCODE_RETURN || theKey == KeyCode::KEYCODE_SPACE)
	{
		if (IsScaryPotterDaveTalking() && mApp->mCrazyDaveMessageIndex != -1)
		{
			mChallenge->AdvanceCrazyDaveDialog();
		}
		else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
		{
			mApp->mZenGarden->AdvanceCrazyDaveDialog();
		}
		else if (theKey == KeyCode::KEYCODE_SPACE && mApp->CanPauseNow())
		{
			mApp->PlaySample(Sexy::SOUND_PAUSE);
			mApp->DoPauseDialog();
		}
	}
	else if (theKey == KeyCode::KEYCODE_ESCAPE && mApp->mGameMode != GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GAMEMODE_TREE_OF_WISDOM)
	{
		if (mCursorObject->mCursorType != CursorType::CURSOR_TYPE_NORMAL)
		{
			RefreshSeedPacketFromCursor();
		}
		else if (CanInteractWithBoardButtons() && mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON)
		{
			mApp->DoNewOptions(false);
		}
	}
}

static void TodCrash()
{
	TOD_ASSERT(false, "Crash%s", "!!!!");
}

void Board::KeyUp(KeyCode theChar)
{
	Widget::KeyUp(theChar);
}

//0x41B950（原版中废弃）
void Board::KeyChar(SexyChar theChar)
{
	/*if (mIsP1Keyboard && mApp->mGameScene == SCENE_PLAYING)
	{
		if (theChar == 'q')
			P1SeedBankID--;
		if (theChar == 'e')
			P1SeedBankID++;

		int seed1Count = 0;
		for (int i = 0; i < mSeedBank->mNumPackets; i++)
		{
			int rightEdge = mSeedBank->mX;
			if (HasConveyorBeltSeedBank()){
				if (mIsCOOP)
					rightEdge += IMAGE_CONVEYORBELT_BACKDROP_COOP->GetWidth();
				else
					rightEdge += IMAGE_CONVEYORBELT_BACKDROP->GetWidth();
			}
			else {
				rightEdge += mSeedBank->mWidth;
			}

			if (mSeedBank->mSeedPackets[i].mPacketType != SEED_NONE && mSeedBank->mSeedPackets[i].mX + mSeedBank->mSeedPackets[i].mOffsetX + 25 < rightEdge + 79)
				seed1Count++;
		}

		P1SeedBankID = max(0, min(P1SeedBankID, seed1Count - 1));
		if (seed1Count == 0)
			P1SeedBankID = -1;

		if (theChar == _S('x'))
		{
			SeedType seedType = mSeedBank->mSeedPackets[P1SeedBankID].mPacketType;
			if (seedType == SEED_IMITATER)
				seedType = mSeedBank->mSeedPackets[P1SeedBankID].mImitaterType;

			float OffsetY = 0;
			if (StageHasRoof() && P1SelectorX < 440.0f) {
				OffsetY = int((440.0f - P1SelectorX) * 0.25f);
			}
			Plant* aPlant = ToolHitTest(P1SelectorX, P1SelectorY + OffsetY + (StageHas6Rows() || StageHasRoof() ? 42.5f : 72));

			if (P1HasShovel)
			{
				P1HasShovel = false;

				if (aPlant == nullptr)
				{
					mApp->PlayFoley(FoleyType::FOLEY_DROP);
				}
				else
				{
					mApp->PlayFoley(FoleyType::FOLEY_USE_SHOVEL);
					mPlantsShoveled++;
					aPlant->Die();
				}
			}
			else if (aPlant && aPlant->mState == PlantState::STATE_COBCANNON_READY) {
				P1CobCanonPlantID = (PlantID)mPlants.DataArrayGetID(aPlant);
			}
			else if (P1CobCanonPlantID != PLANTID_NULL) {
				Plant* aCobcannon = mPlants.DataArrayTryToGet(P1CobCanonPlantID);
				if (aCobcannon)
				{
					aCobcannon->CobCannonFire(P1SelectorX, P1SelectorY + OffsetY + (StageHas6Rows() || StageHasRoof() ? 42.5f : 72));
				}
				P1CobCanonPlantID = PLANTID_NULL;
			}
			else if (P1SeedBankID > -1 && P1HasPlant && seedType != SEED_NONE && seedType <= NUM_SEED_TYPES && !CanPlantAt(P1SelectorGridX, P1SelectorGridY, seedType))
			{
				if (!mApp->mEasyPlantingCheat && !HasConveyorBeltSeedBank())
				{
					if (!mSeedBank->mSeedPackets[P1SeedBankID].mActive) {
						mApp->PlaySample(Sexy::SOUND_BUZZER);
						return;
					}
					if (!TakeSunMoney(GetCurrentPlantCost(mSeedBank->mSeedPackets[P1SeedBankID].mPacketType, SeedType::SEED_NONE))){
						return;
					}
				}

				if (P1SeedBankID == mCursorObject->mSeedBankIndex)
					ClearCursor();

				mSeedBank->mSeedPacketsTime[P1SeedBankID] = 0;

				bool aIsAwake = false;
				int aWakeUpCounter = 0;
				PlantsOnLawn aPlantOnLawn;
				GetPlantsOnLawn(P1SelectorGridX, P1SelectorGridY, &aPlantOnLawn);
				Plant* aNormalPlant = aPlantOnLawn.mNormalPlant;
				Plant* aPumpkinPlant = aPlantOnLawn.mPumpkinPlant;
				if (aNormalPlant && aNormalPlant->IsUpgradableTo(mSeedBank->mSeedPackets[P1SeedBankID].mPacketType))
				{
					if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_GLOOMSHROOM)
					{
						aIsAwake = !aNormalPlant->mIsAsleep;
						aWakeUpCounter = aNormalPlant->mWakeUpCounter;
					}
					aNormalPlant->Die();
				}
				if ((mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_WALLNUT || mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_TALLNUT) && aNormalPlant)
				{
					if (aNormalPlant->mSeedType == mSeedBank->mSeedPackets[P1SeedBankID].mPacketType)
					{
						aNormalPlant->Die();
					}
				}
				if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_PUMPKINSHELL && aPumpkinPlant)
				{
					if (aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
					{
						aPumpkinPlant->Die();
					}
				}
				if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_COBCANNON)
				{
					Plant* aRightPlant = GetTopPlantAt(P1SelectorGridX + 1, P1SelectorGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
					if (aRightPlant)
					{
						aRightPlant->Die();
					}
				}
				if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_CATTAIL)
				{
					if (aPlantOnLawn.mPotPlant)
					{
						aPlantOnLawn.mPotPlant->Die();
					}
					if (aNormalPlant)
					{
						aNormalPlant->Die();
					}
				}
				
				Plant* aPlant = AddPlant(P1SelectorGridX, P1SelectorGridY, mSeedBank->mSeedPackets[P1SeedBankID].mPacketType, mSeedBank->mSeedPackets[P1SeedBankID].mImitaterType);
				if (aIsAwake)
					aPlant->SetSleeping(false);
				else
					aPlant->mWakeUpCounter = aWakeUpCounter;

				if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL)
				{
					if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SEED_POTATOMINE || mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SEED_CHERRYBOMB || mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SEED_JALAPENO || mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SEED_DOOMSHROOM)
						mChallenge->mBombsLeft = max(0, mChallenge->mBombsLeft - 1);
				}


				mSeedBank->mSeedPackets[P1SeedBankID].Deactivate();
				mSeedBank->mSeedPackets[P1SeedBankID].WasPlanted();

				if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
				{
					for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++)
					{
						if (aRow == P1SelectorGridX || CanPlantAt(P1SelectorGridX, aRow, mSeedBank->mSeedPackets[P1SeedBankID].mPacketType) != PlantingReason::PLANTING_OK)
							continue;

						if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_WALLNUT || mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_TALLNUT)
						{
							aNormalPlant = GetTopPlantAt(P1SelectorGridX, aRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
							if (aNormalPlant && aNormalPlant->mSeedType == mSeedBank->mSeedPackets[P1SeedBankID].mPacketType)
							{
								aNormalPlant->Die();
							}
						}
						if (mSeedBank->mSeedPackets[P1SeedBankID].mPacketType == SeedType::SEED_PUMPKINSHELL)
						{
							aPumpkinPlant = GetTopPlantAt(P1SelectorGridX, aRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN);
							if (aPumpkinPlant && aPumpkinPlant->mSeedType == SeedType::SEED_PUMPKINSHELL)
							{
								aPumpkinPlant->Die();
							}
						}
						AddPlant(P1SelectorGridX, aRow, mCursorObject->mType, mCursorObject->mImitaterType);
					}
					
				}
			}

			P1HasPlant = !P1HasShovel;
		}

		if (theChar == _S('z') && mShowShovel && mShovelCounter == 0)
		{
			if (!P1HasShovel)
			{
				P1HasShovel = true;
				mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
			}
			else {
				P1HasShovel = false;
				mApp->PlaySample(SOUND_TAP);
			}
			P1HasPlant = !P1HasShovel;
		}
		
		if (theChar == _S('z') && mShowShovel && mButterCounter == 0){
			Zombie* aZombie = nullptr;
			bool playSound = false;

			float OffsetY = 0;
			if (StageHasRoof() && P1SelectorX < 440.0f) {
				OffsetY = int((440.0f - P1SelectorX) * 0.25f);
			}
			float theY = P1SelectorY + OffsetY + (StageHas6Rows() || StageHasRoof() ? 42.5f : 72);

			while (IterateZombies(aZombie))
			{
				if (aZombie->GetZombieRect().Contains(P1SelectorX, P1SelectorY) && aZombie->mButteredCounter == 0)
				{
					playSound = true;
					aZombie->ApplyButter();
				}
			}

			if (playSound){
				if (mApp->mGameMode != GAMEMODE_CHALLENGE_BUTTERED_POPCORN)
					mButterCounter = 900;
				mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
			}
		}
	}*/

	if (!mApp->mDebugKeysEnabled)
		return;

	TodTraceAndLog("Board cheat key '%c'", theChar);

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (theChar == _S('m'))
		{
			if (!mApp->mZenGarden->IsZenGardenFull(true))
			{
				PottedPlant aPottedPlant;
				aPottedPlant.InitializePottedPlant(SeedType::SEED_MARIGOLD);
				aPottedPlant.mDrawVariation = (DrawVariation)RandRangeInt((int)DrawVariation::VARIATION_MARIGOLD_WHITE, (int)DrawVariation::VARIATION_MARIGOLD_LIGHT_GREEN);
				mApp->mZenGarden->AddPottedPlant(&aPottedPlant);
			}
			return;
		}
		
		if (theChar == _S('+'))
		{
			if (!mApp->mZenGarden->IsZenGardenFull(true))
			{
				PottedPlant aPottedPlant;
				aPottedPlant.InitializePottedPlant(mApp->mZenGarden->PickRandomSeedType());
				mApp->mZenGarden->AddPottedPlant(&aPottedPlant);
			}
			return;
		}
		
		if (theChar == _S('a'))
		{
			if (!mApp->mZenGarden->IsZenGardenFull(true))
			{
				PottedPlant aPottedPlant;
				aPottedPlant.InitializePottedPlant(mApp->mZenGarden->PickRandomSeedType());
				aPottedPlant.mPlantAge = PottedPlantAge::PLANTAGE_FULL;
				mApp->mZenGarden->AddPottedPlant(&aPottedPlant);
			}
			return;
		}
		
		if (theChar == _S('f'))
		{
			Plant* aPlant = nullptr;
			while (IteratePlants(aPlant))
			{
				if (GetZenToolAt(aPlant->mPlantCol, aPlant->mRow) == nullptr && aPlant->mPottedPlantIndex >= 0)
				{
					PottedPlant* aPottedPlant = mApp->mZenGarden->PottedPlantFromIndex(aPlant->mPottedPlantIndex);
					PottedPlantNeed aNeed = mApp->mZenGarden->GetPlantsNeed(aPottedPlant);
					if (aNeed == PottedPlantNeed::PLANTNEED_WATER)
					{
						aPlant->mHighlighted = true;
						mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_WATERING_CAN);
						return;
					}
					else if (aNeed == PottedPlantNeed::PLANTNEED_FERTILIZER)
					{
						aPlant->mHighlighted = true;
						if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] <= PURCHASE_COUNT_OFFSET)
						{
							mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] = PURCHASE_COUNT_OFFSET + 1;
						}
						mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_FERTILIZER);
						return;
					}
					else if (aNeed == PottedPlantNeed::PLANTNEED_BUGSPRAY)
					{
						aPlant->mHighlighted = true;
						if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] <= PURCHASE_COUNT_OFFSET)
						{
							mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] = PURCHASE_COUNT_OFFSET + 1;
						}
						mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_BUG_SPRAY);
						return;
					}
					else if (aNeed == PottedPlantNeed::PLANTNEED_PHONOGRAPH)
					{
						aPlant->mHighlighted = true;
						mApp->mZenGarden->MouseDownWithFeedingTool(aPlant->mX, aPlant->mY, CursorType::CURSOR_TYPE_PHONOGRAPH);
						return;
					}
				}
			}
			return;
		}

		if (theChar == _S('r'))
		{
			Plant* aPlant = nullptr;
			while (IteratePlants(aPlant))
			{
				if (aPlant->mPottedPlantIndex >= 0)
				{
					TOD_ASSERT(aPlant->mPottedPlantIndex < mApp->mPlayerInfo->mNumPottedPlants);
					PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[aPlant->mPottedPlantIndex];
					mApp->mZenGarden->ResetPlantTimers(aPottedPlant);
				}
			}
			return;
		}

		if (theChar == _S('s'))
		{
			if (mApp->mZenGarden->IsStinkySleeping())
			{
				mApp->mZenGarden->WakeStinky();
			}
			else
			{
				mApp->mZenGarden->ResetStinkyTimers();
			}
			return;
		}

		if (theChar == _S('c'))
		{
			if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] < PURCHASE_COUNT_OFFSET)
			{
				mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] = PURCHASE_COUNT_OFFSET + 1;
			}
			else
			{
				mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE]++;
			}
			return;
		}

		if (theChar == _S(']'))
		{
			PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
			if (aPottedPlant)
			{
				aPottedPlant->mSeedType = (SeedType)((int)aPottedPlant->mSeedType + 1);
				if (aPottedPlant->mSeedType == SeedType::SEED_GATLINGPEA)
				{
					aPottedPlant->mSeedType = SeedType::SEED_PEASHOOTER;
				}
				if (aPottedPlant->mSeedType == SeedType::SEED_FLOWERPOT)
				{
					aPottedPlant->mSeedType = SeedType::SEED_KERNELPULT;
				}
			}
			return;
		}
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		if (theChar == _S('f'))
		{
			if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_FOOD] <= PURCHASE_COUNT_OFFSET)
			{
				mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_FOOD] = PURCHASE_COUNT_OFFSET + 1;
			}
			mChallenge->TreeOfWisdomFertilize();
		}
		else if (theChar == _S('g'))
		{
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('b'))
		{
			mChallenge->mChallengeStateCounter = 1;
		}
		else if (theChar == _S('0'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 0;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('1'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 9;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('2'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 19;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('3'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 29;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('4'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 39;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('5'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 49;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('6'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 98;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('7'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 498;
			mChallenge->TreeOfWisdomGrow();
		}
		else if (theChar == _S('8'))
		{
			mApp->mPlayerInfo->mChallengeRecords[mApp->GetCurrentChallengeIndex()] = 998;
			mChallenge->TreeOfWisdomGrow();
		}

		return;
	}

	if (theChar == _S('<'))
	{
		mApp->DoNewOptions(false);
	}
	else if (theChar == _S('l'))
	{
		mApp->DoCheatDialog();
	}
	else if (theChar == _S('#'))
	{
		if (mApp->IsSurvivalMode())
		{
			if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
			{
				return;
			}

			mCurrentWave = mNumWaves;
			mChallenge->mSurvivalStage += 5;
			RemoveAllZombies();
			FadeOutLevel();
		}
	}
	else if (theChar == _S('!'))
	{
		mIsSpeeding = false;
		mApp->mBoardResult = BoardResult::BOARDRESULT_CHEAT;
		if (IsLastStandStageWithRepick())
		{
			if (mNextSurvivalStageCounter == 0)
			{
				mCurrentWave = mNumWaves;
				RemoveAllZombies();
				FadeOutLevel();
			}
		}
		else if ((mApp->IsScaryPotterLevel() && !IsFinalScaryPotterStage()) || mApp->IsEndlessIZombie(mApp->mGameMode))
		{
			if (mNextSurvivalStageCounter == 0)
			{
				RemoveAllZombies();
				FadeOutLevel();
			}
		}
		else if (mApp->IsSurvivalMode())
		{
			if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
			{
				return;
			}

			mCurrentWave = mNumWaves;
			if (!IsSurvivalStageWithRepick())
			{
				//RemoveAllZombies();
			}
			FadeOutLevel();
		}
		else
		{
			RemoveAllZombies();
			FadeOutLevel();
			mBoardFadeOutCounter = 200;
		}
	}
	else if (theChar == _S('+'))
	{
		mApp->mBoardResult = BoardResult::BOARDRESULT_CHEAT;
		if (IsLastStandStageWithRepick())
		{
			if (mNextSurvivalStageCounter == 0)
			{
				mCurrentWave = mNumWaves;
				RemoveAllZombies();
				FadeOutLevel();
			}
		}
		else if ((mApp->IsScaryPotterLevel() && !IsFinalScaryPotterStage()) || mApp->IsEndlessIZombie(mApp->mGameMode))
		{
			if (mNextSurvivalStageCounter == 0)
			{
				RemoveAllZombies();
				FadeOutLevel();
			}
		}
		else if (mApp->IsSurvivalEndless(mApp->mGameMode))
		{
			if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO)
			{
				return;
			}

			mCurrentWave = mNumWaves;
			RemoveAllZombies();
			FadeOutLevel();
		}
		else if (mApp->IsSurvivalMode())
		{
			mChallenge->mSurvivalStage = 5;
			RemoveAllZombies();
			FadeOutLevel();
			mBoardFadeOutCounter = 200;
		}
		else
		{
			RemoveAllZombies();
			FadeOutLevel();
			mBoardFadeOutCounter = 200;
		}
	}
	else if (theChar == _S('8'))
	{
		mApp->mEasyPlantingCheat = !mApp->mEasyPlantingCheat;
	}
	else if (theChar == _S('7'))
	{
		mApp->ToggleSlowMo();
	}
	else if (theChar == _S('6'))
	{
		mApp->ToggleFastMo();
	}
	else if (theChar == _S('z'))
	{
		mDebugTextMode = (DebugTextMode)((int)mDebugTextMode + 1);
		if (mDebugTextMode > DebugTextMode::DEBUG_TEXT_COLLISION)
		{
			mDebugTextMode = DebugTextMode::DEBUG_TEXT_NONE;
		}
	}

	if (mApp->mGameScene != GameScenes::SCENE_PLAYING)
	{
		return;
	}

	Zombie* aBossZombie = GetBossZombie();
	if (aBossZombie && !aBossZombie->IsDeadOrDying())
	{
		if (theChar == _S('b'))
		{
			aBossZombie->mBossBungeeCounter = 0;
			return;
		}
		if (theChar == _S('u'))
		{
			aBossZombie->mSummonCounter = 0;
			return;
		}
		if (theChar == _S('s'))
		{
			aBossZombie->mBossStompCounter = 0;
			return;
		}
		if (theChar == _S('r'))
		{
			aBossZombie->BossRVAttack();
			return;
		}
		if (theChar == _S('h'))
		{
			aBossZombie->mBossHeadCounter = 0;
			return;
		}
		if (theChar == _S('d'))
		{
			aBossZombie->TakeDamage(10000, 0U);
			return;
		}
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBOTANY_2)
	{
		if (theChar == _S('w'))
		{
			AddZombie(ZombieType::ZOMBIE_WALLNUT_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('t'))
		{
			AddZombie(ZombieType::ZOMBIE_TALLNUT_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('j'))
		{
			AddZombie(ZombieType::ZOMBIE_JALAPENO_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('g'))
		{
			AddZombie(ZombieType::ZOMBIE_GATLING_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('s'))
		{
			AddZombie(ZombieType::ZOMBIE_SQUASH_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('d'))
		{
			AddZombie(ZombieType::ZOMBIE_FLAG, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
		if (theChar == _S('p'))
		{
			AddZombie(ZombieType::ZOMBIE_HYPNOSHROOM_HEAD, Zombie::ZOMBIE_WAVE_DEBUG);
			return;
		}
	}

	if (theChar == _S('q'))
	{
		if (mApp->IsSurvivalEndless(mApp->mGameMode))
		{
			mApp->mEasyPlantingCheat = true;
			for (int y = 0; y < MAX_GRID_SIZE_X; y++)
			{
				for (int x = 0; x < MAX_GRID_SIZE_Y; x++)
				{
					if (CanPlantAt(x, y, SeedType::SEED_LILYPAD) == PlantingReason::PLANTING_OK)
					{
						AddPlant(x, y, SeedType::SEED_LILYPAD, SeedType::SEED_NONE);
					}
					if (CanPlantAt(x, y, SeedType::SEED_PUMPKINSHELL) == PlantingReason::PLANTING_OK)
					{
						if (x <= 6 || IsPoolSquare(x, y))
						{
							AddPlant(x, y, SeedType::SEED_PUMPKINSHELL, SeedType::SEED_NONE);
						}
					}
					if (CanPlantAt(x, y, SeedType::SEED_GATLINGPEA) == PlantingReason::PLANTING_OK)
					{
						if (x < 5)
						{
							AddPlant(x, y, SeedType::SEED_GATLINGPEA, SeedType::SEED_NONE);
						}
						else if (x == 5)
						{
							AddPlant(x, y, SeedType::SEED_TORCHWOOD, SeedType::SEED_NONE);
						}
						else if (x == 6)
						{
							AddPlant(x, y, SeedType::SEED_SPLITPEA, SeedType::SEED_NONE);
						}
						else if (y == 2 || y == 3)
						{
							AddPlant(x, y, SeedType::SEED_GLOOMSHROOM, SeedType::SEED_NONE);
							if (CanPlantAt(x, y, SeedType::SEED_INSTANTCOFFEE) == PlantingReason::PLANTING_OK)
							{
								AddPlant(x, y, SeedType::SEED_INSTANTCOFFEE, SeedType::SEED_NONE);
							}
						}
					}
				}
			}
		}
		else if (mApp->IsIZombieLevel())
		{
			mApp->mEasyPlantingCheat = true;
			for (int i = 0; i < 5; i++)
			{
				mChallenge->IZombiePlaceZombie(ZombieType::ZOMBIE_FOOTBALL, 6, i);
			}
		}
		else
		{
			mApp->mEasyPlantingCheat = true;
			for (int y = 0; y < MAX_GRID_SIZE_Y; ++y)
			{
				for (int x = 0; x < MAX_GRID_SIZE_X; ++x)
				{
					if (StageHasRoof() && CanPlantAt(x, y, SeedType::SEED_FLOWERPOT) == PlantingReason::PLANTING_OK)
					{
						AddPlant(x, y, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE);
					}
					if (CanPlantAt(x, y, SeedType::SEED_LILYPAD) == PlantingReason::PLANTING_OK)
					{
						AddPlant(x, y, SeedType::SEED_LILYPAD, SeedType::SEED_NONE);
					}
					if (CanPlantAt(x, y, SeedType::SEED_THREEPEATER) == PlantingReason::PLANTING_OK)
					{
						AddPlant(x, y, SeedType::SEED_THREEPEATER, SeedType::SEED_NONE);
					}
				}
			}

			if (!mChallenge->UpdateZombieSpawning())
			{
				int aWavesRemaining = min(mNumWaves - mCurrentWave, 20);
				while (aWavesRemaining)
				{
					SpawnZombieWave();
					aWavesRemaining--;
				}
			}

			if (mApp->IsScaryPotterLevel())
			{
				GridItem* aGridItem = nullptr;
				while (IterateGridItems(aGridItem))
				{
					if (aGridItem->mGridItemType == GridItemType::GRIDITEM_SCARY_POT)
					{
						mChallenge->ScaryPotterOpenPot(aGridItem);
					}
				}
			}
		}

		return;
	}

	if (theChar == _S('O'))
	{
		mApp->mEasyPlantingCheat = true;
		for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (CanPlantAt(x, y, SeedType::SEED_FLOWERPOT) == PlantingReason::PLANTING_OK)

					AddPlant(x, y, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE);
				
			}
		}
		return;
	}

	if (theChar == _S('?') || theChar == _S('/'))
	{
		if (mHugeWaveCountDown > 0)
		{
			mHugeWaveCountDown = 1;
		}
		else
		{
			mZombieCountDown = 6;
		}
		return;
	}

	if (theChar == _S('b'))
	{
		AddZombie(ZombieType::ZOMBIE_BUNGEE, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('o'))
	{
		AddZombie(ZombieType::ZOMBIE_DOOR, Zombie::ZOMBIE_WAVE_DEBUG); // FOOTBALL
		return;
	}
	if (theChar == _S('s'))
	{
		AddZombie(ZombieType::ZOMBIE_NINJAIMP, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('L'))
	{
		AddZombie(ZombieType::ZOMBIE_LADDER, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('y'))
	{
		AddZombie(ZombieType::ZOMBIE_YETI, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('a'))
	{
		AddZombie(ZombieType::ZOMBIE_FLAG, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('w'))
	{
		AddZombie(ZombieType::ZOMBIE_NEWSPAPER, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('F'))
	{
		AddZombie(ZombieType::ZOMBIE_BALLOON, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('n'))
	{
		if (StageHasPool())
		{
			AddZombie(ZombieType::ZOMBIE_SNORKEL, Zombie::ZOMBIE_WAVE_DEBUG);
		}
	}
	if (theChar == _S('c'))
	{
		AddZombie(ZombieType::ZOMBIE_TRAFFIC_CONE, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('m'))
	{
		AddZombie(ZombieType::ZOMBIE_DISCO, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('h'))
	{
		AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	//if (theChar == _S('H')
	//{
	//	AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
	//	AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
	//	AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
	//	AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
	//	AddZombie(ZombieType::ZOMBIE_PAIL, Zombie::ZOMBIE_WAVE_DEBUG);
	//	return;
	//}
	if (theChar == _S('D'))
	{
		AddZombie(ZombieType::ZOMBIE_DIGGER, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('p'))
	{
		AddZombie(ZombieType::ZOMBIE_POLEVAULTER, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('P'))
	{
		AddZombie(ZombieType::ZOMBIE_POGO, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('R'))
	{
		if (StageHasPool())
		{
			AddZombie(ZombieType::ZOMBIE_DOLPHIN_RIDER, Zombie::ZOMBIE_WAVE_DEBUG);
		}
		return;
	}
	else if(theChar == _S('j'))
	{
		AddZombie(ZombieType::ZOMBIE_JACK_IN_THE_BOX, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('g'))
	{
		AddZombie(ZombieType::ZOMBIE_GARGANTUAR, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('G'))
	{
		AddZombie(ZombieType::ZOMBIE_REDEYE_GARGANTUAR, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('i'))
	{
		AddZombie(ZombieType::ZOMBIE_ZAMBONI, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('C'))
	{
		AddZombie(ZombieType::ZOMBIE_CATAPULT, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('1'))
	{
		Plant* aPlant = GetTopPlantAt(0, 0, PlantPriority::TOPPLANT_ANY);
		if (aPlant)
		{
			aPlant->Die();
			mChallenge->ZombieAtePlant(nullptr, aPlant);
			return;
		}
	}
	if (theChar == _S('B'))
	{
		mFogBlownCountDown = 2200;
		return;
	}
	if (theChar == _S('t'))
	{
		if (!CanAddBobSled())
		{
			int aRow = Rand(5);
			int aPos = 400;
			if (StageHasPool())
			{
				aRow = Rand(2);
			}
			else if (StageHasRoof())
			{
				aPos = 500;
			}
			mIceTimer[aRow] = 3000;
			mIceMinX[aRow] = aPos;
		}

		AddZombie(ZombieType::ZOMBIE_BOBSLED, Zombie::ZOMBIE_WAVE_DEBUG);
		return;
	}
	if (theChar == _S('r'))
	{
		SpawnZombiesFromGraves();
		return;
	}
	if (theChar == _S('0'))
	{
		AddSunMoney(100);
		mApp->PlaySample(SOUND_BUTTONCLICK);
		return;
	}
	if (theChar == _S('9'))
	{
		AddSunMoney(999999);
		mApp->PlaySample(SOUND_BUTTONCLICK);
		return;
	}
	if (theChar == _S('$'))
	{
		mApp->mPlayerInfo->AddCoins(100);
		mApp->PlaySample(SOUND_BUTTONCLICK);
		ShowCoinBank();
		return;
	}
	if (theChar == _S('-'))
	{
		mSunMoney -= 100;
		if (mSunMoney < 0)
		{
			mSunMoney = 0;
		}
		return;
	}
	if (theChar == _S('%'))
	{
		mApp->SwitchScreenMode(mApp->mIsWindowed, true, false);
	}
	if (theChar == _S('M'))
	{
		mApp->mMusic->mBurstOverride = 2 - (mApp->mMusic->mBurstOverride != 1);
		return;
	}

	if (theChar == _S('\3') && mApp->mCtrlDown && mApp->mTodCheatKeys)
	{
		TodCrash();

		if (mHugeWaveCountDown > 0)
		{
			mHugeWaveCountDown = 1;
		}
		else
		{
			mZombieCountDown = 6;
		}
	}
}

//0x41B960
void Board::AddSunMoney(int theAmount)
{
	mSunMoney += theAmount;
	if (mSunMoney > 9990)
	{
		mSunMoney = 9990;
	}
}

//0x41B980
int Board::CountSunBeingCollected()
{
	int aCount = 0;
	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		if (aCoin->mIsBeingCollected && aCoin->IsSun())
		{
			aCount += aCoin->GetSunValue();
		}
	}
	return aCount;
}

//0x41B9F0
int Board::CountCoinsBeingCollected()
{
	int aCount = 0;
	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		if (aCoin->mIsBeingCollected && aCoin->IsMoney())
		{
			aCount += aCoin->GetCoinValue(aCoin->mType);
		}
	}
	return aCount;
}

//0x41BA60
bool Board::TakeSunMoney(int theAmount)
{
	if (CanTakeSunMoney(theAmount))
	{
		mSunMoney -= theAmount;
		return true;
	}

	mApp->PlaySample(Sexy::SOUND_BUZZER);
	mOutOfMoneyCounter = 70;
	return false;
}

//0x41BAB0
bool Board::CanTakeSunMoney(int theAmount)
{
	return theAmount <= mSunMoney + CountSunBeingCollected();
}

//0x41BAD0
void Board::ProcessDeleteQueue()
{
	PlayerController* aPlayer = nullptr;
	while (mPlayerController.IterateNext(aPlayer))
	{
		if (aPlayer->mDead)
		{
			mPlayerController.DataArrayFree(aPlayer);
		}
	}

	Plant* aPlant = nullptr;
	while (mPlants.IterateNext(aPlant))
	{
		if (aPlant->mDead)
		{
			mPlants.DataArrayFree(aPlant);
		}
	}

	Plant* aExtraPlants = nullptr;
	while (mExtraPlants.IterateNext(aExtraPlants))
	{
		if (aExtraPlants->mDead)
		{
			mPlants.DataArrayFree(aExtraPlants);
		}
	}
	
	Zombie* aZombie = nullptr;
	while (mZombies.IterateNext(aZombie))
	{
		if (aZombie->mDead)
		{
			mZombies.DataArrayFree(aZombie);
		}
	}

	Projectile* aProjectile = nullptr;
	while (mProjectiles.IterateNext(aProjectile))
	{
		if (aProjectile->mDead)
		{
			mProjectiles.DataArrayFree(aProjectile);
		}
	}
	
	Coin* aCoin = nullptr;
	while (mCoins.IterateNext(aCoin))
	{
		if (aCoin->mDead)
		{
			mCoins.DataArrayFree(aCoin);
		}
	}
	
	LawnMower* aLawnMower = nullptr;
	while (mLawnMowers.IterateNext(aLawnMower))
	{
		if (aLawnMower->mDead)
		{
			mLawnMowers.DataArrayFree(aLawnMower);
		}
	}
	
	GridItem* aGridItem = nullptr;
	while (mGridItems.IterateNext(aGridItem))
	{
		if (aGridItem->mDead)
		{
			mGridItems.DataArrayFree(aGridItem);
		}
	}
}

//0x41BE50
bool Board::HasConveyorBeltSeedBank()
{
	return 
		mApp->IsFinalBossLevel() || 
		mApp->IsMiniBossLevel() || 
		mApp->IsShovelLevel() || 
		mApp->IsWallnutBowlingLevel() ||
		mApp->IsLittleTroubleLevel() || 
		mApp->IsBigTroubleLevel() ||
		mApp->IsStormyNightLevel() || 
		mApp->IsBungeeBlitzLevel() || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT_2 ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_OPEN_FIRE ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BOMB_ALL;
}

//0x41BEE0
int Board::GetNumSeedsInBank()
{
	if (mApp->IsScaryPotterLevel())
	{
		return 1;
	}
	if (mApp->IsWhackAZombieLevel())
	{
		if (mApp->mGameMode == GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE_2)		return 4;
		return 3;
	}
	if (mApp->IsChallengeWithoutSeedBank())
	{
		return 0;
	}
	if (HasConveyorBeltSeedBank())
	{
		return 10;
	}
	if (mApp->IsSlotMachineLevel())
	{
		return mApp->mGameMode == GAMEMODE_CHALLENGE_SLOT_MACHINE_2 ? 4 : 3;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL)
	{
		return 6;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
	{
		return 0;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
	{
		return 2;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1 || mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2 ||
		mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3 || mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4)
	{
		return 3;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5 || mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6 ||
		mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7)
	{
		return 4;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8)
	{
		return 6;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9)
	{
		return 8;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS)
	{
		return 9;
	}

	if (mApp->HasSecondarySeedBank())
	{
		if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
		{
			return 6;
		}
		else
		{
			return 4;
		}
	}


	int aNumSeeds = min(mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_PACKET_UPGRADE] + 6, 10);
	int aSeedsAvailable = mApp->GetSeedsAvailable();
	return min(aNumSeeds, aSeedsAvailable);
}

//0x41C010
bool Board::StageIsNight()
{
	return 
		mBackground == BackgroundType::BACKGROUND_2_NIGHT || 
		mBackground == BackgroundType::BACKGROUND_4_FOG || 
		mBackground == BackgroundType::BACKGROUND_6_BOSS ||
		mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || 
		mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM ||
		mBackground == BackgroundType::BACKGROUND_6_AIRRAID ||
		mBackground == BackgroundType::BACKGROUND_GRAVEYARD ||
		mBackground == BackgroundType::BACKGROUND_HIGHGROUND;
}

//0x41C040
bool Board::StageHasGraveStones()
{
	if (mApp->IsWallnutBowlingLevel() ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POGO_PARTY ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT ||
		LawnApp::IsLastStandLevel(mApp->mGameMode) || 
		mApp->IsIZombieLevel() ||
		mApp->IsScaryPotterLevel())
		return false;

	return mBackground == BackgroundType::BACKGROUND_2_NIGHT || mBackground == BackgroundType::BACKGROUND_GRAVEYARD;
}

//0x41C0B0
bool Board::StageHasRoof()
{
	return (mBackground == BackgroundType::BACKGROUND_5_ROOF || mBackground == BackgroundType::BACKGROUND_6_BOSS);
}

//0x41C0D0
bool Board::StageHasPool()
{
	return mBackground == BackgroundType::BACKGROUND_3_POOL || 
		   mBackground == BackgroundType::BACKGROUND_3_UNSODDED || 
		   mBackground == BackgroundType::BACKGROUND_3_POOLWIDE || 
		   mBackground == BackgroundType::BACKGROUND_4_FOG;
}

bool Board::StageHasHighGrounds()
{
	return mBackground == BackgroundType::BACKGROUND_HIGHGROUND;
}

bool Board::StageHas6Rows()
{
	return mBackground == BackgroundType::BACKGROUND_3_POOL || 
		   mBackground == BackgroundType::BACKGROUND_3_UNSODDED || 
		   mBackground == BackgroundType::BACKGROUND_3_POOLWIDE|| 
		   mBackground == BackgroundType::BACKGROUND_3_GRASS ||
		   mBackground == BackgroundType::BACKGROUND_4_FOG ||
		   mBackground == BackgroundType::BACKGROUND_HIGHGROUND ||
		   mBackground == BackgroundType::BACKGROUND_3_GRASS;
}

//0x41C0F0
bool Board::StageHasZombieWalkInFromRight()
{
	if (mApp->IsWhackAZombieLevel() ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE_LEVEL ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN ||
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_AIR_RAID ||
		mApp->IsFinalBossLevel() ||
		mApp->IsIZombieLevel() ||
		mApp->IsSquirrelLevel() ||
		mApp->IsScaryPotterLevel())
		return false;

	return true;
}

//0x41C170
bool Board::StageHasFog()
{
	if (mApp->IsStormyNightLevel() ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_INVISIGHOUL ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_PORTAL_COMBAT_2 ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_HIGHGROUND ||
		mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT)
		return false;

	return mBackground == BackgroundType::BACKGROUND_4_FOG ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT;
}

bool Board::StageHasDarkness()
{
	return mApp->IsAdventureMode() && mLevel >= 51 && mLevel <= 59 ||
		   mApp->mGameMode == GAMEMODE_SURVIVAL_NORMAL_STAGE_6 || 
		   mApp->mGameMode == GAMEMODE_SURVIVAL_HARD_STAGE_6 || 
		   mApp->mGameMode == GAMEMODE_SURVIVAL_ENDLESS_STAGE_6 || 
		   mApp->mGameMode == GAMEMODE_CHALLENGE_LAST_STAND_GLOOM ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_DARK_FOGGY_NIGHT ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT;
}

bool Board::StageHasRain()
{
	return mApp->IsAdventureMode() && mLevel >= 51 && mLevel <= 58 ||
		   mApp->mGameMode == GAMEMODE_SURVIVAL_NORMAL_STAGE_6 ||
		   mApp->mGameMode == GAMEMODE_SURVIVAL_HARD_STAGE_6 ||
		   mApp->mGameMode == GAMEMODE_SURVIVAL_ENDLESS_STAGE_6 ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_LAST_STAND_GLOOM ||
		   mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS ||
		   mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS_2 ||
		   mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_WEIRDMAGEDDON ||
		   mApp->mGameMode == GAMEMODE_CHALLENGE_HAIL;
}

//0x41C1C0
int Board::LeftFogColumn()
{
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BLIND_FAITH)				return -1;
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_WEIRDMAGEDDON)			return 7;
	if (!mApp->IsAdventureMode())										return 5;
	if (mLevel == 31)													return 6;
	if (mLevel >= 32 && mLevel <= 36)									return 5;
	if (mLevel >= 37 && mLevel <= 40)									return 4;
	return 9;
}

//0x41C210
int Board::GetSeedPacketPositionX(int theIndex)
{
	if (mApp->IsSlotMachineLevel())
	{
		if (theIndex < 3)
			return theIndex * 59 + 247;
		else
			return (theIndex - 3) * 51 + 79;
	}

	if (HasConveyorBeltSeedBank())			return theIndex * 50 + 91;

	/*if (mSeedBank->mNumPackets <= 7)		return theIndex * 59 + 85;
	else if (mSeedBank->mNumPackets == 8)	return theIndex * 54 + 81;
	else if (mSeedBank->mNumPackets == 9)	return theIndex * 52 + 80;
	else*/			

	return theIndex * 51 + 79;
}

//0x41C290
int Board::GetSeedBankExtraWidth()
{
	int aNumPackets = mSeedBank->mNumPackets;
	return aNumPackets <= 6 ? 0 : aNumPackets == 7 ? 60 : aNumPackets == 8 ? 76 : aNumPackets == 9 ? 112 : 153;
}

void Board::OffsetYForPlanting(int& theY, SeedType theSeedType)
{
	if (Plant::IsFlying(theSeedType) || theSeedType == SeedType::SEED_GRAVEBUSTER)
	{
		theY += 15;
	}
	if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
	{
		theY -= 15;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mBackground == BackgroundType::BACKGROUND_GREENHOUSE)
	{
		theY -= 25;
	}
}

//0x41C2D0
int Board::PlantingPixelToGridX(int theX, int theY, SeedType theSeedType)
{
	OffsetYForPlanting(theY, theSeedType);
	return PixelToGridX(theX, theY);
}

//0x41C320
int Board::PlantingPixelToGridY(int theX, int theY, SeedType theSeedType)
{
	OffsetYForPlanting(theY, theSeedType);

	int aGridY = PixelToGridY(theX, theY);
	if (theSeedType == SeedType::SEED_INSTANTCOFFEE)
	{
		int aGridX = PixelToGridX(theX, theY);

		Plant* aPlant = GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
		if (aPlant && aPlant->mIsAsleep)
		{
			return aGridY;
		}

		int aGridYDown = PixelToGridY(theX, theY + 30);
		if (aGridYDown != aGridY)
		{
			Plant* aPlantDown = GetTopPlantAt(aGridX, aGridYDown, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
			if (aPlantDown && aPlantDown->mIsAsleep)
			{
				return aGridYDown;
			}
		}

		int aGridYUp = PixelToGridY(theX, theY - 50);
		if (aGridYUp != aGridY)
		{
			Plant* aPlantUp = GetTopPlantAt(aGridX, aGridYUp, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
			if (aPlantUp && aPlantUp->mIsAsleep)
			{
				return aGridYUp;
			}
		}
	}
	return aGridY;
}

//0x41C4C0
int Board::PixelToGridX(int theX, int theY)
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE ||
			mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN ||
			mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
		{
			return mApp->mZenGarden->PixelToGridX(theX, theY);
		}
	}

	if (theX < LAWN_XMIN)
		return -1;

	return ClampInt((theX - LAWN_XMIN) / 80, 0, MAX_GRID_SIZE_X - 1);
}

//0x41C530
int Board::PixelToGridXKeepOnBoard(int theX, int theY)
{
	int aGridX = PixelToGridX(theX, theY);
	return max(aGridX, 0);
}

//0x41C550
int Board::PixelToGridY(int theX, int theY)
{
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE ||
			mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN ||
			mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
		{
			return mApp->mZenGarden->PixelToGridY(theX, theY);
		}
	}

	int aGridX = PixelToGridX(theX, theY);
	if (aGridX == -1 || theY < LAWN_YMIN)
		return -1;

	if (StageHasRoof())
	{
		if (aGridX < 5)
		{
			theY -= (4 - aGridX) * 20;
		}
		return ClampInt((theY - LAWN_YMIN) / 85, 0, MAX_GRID_SIZE_Y - 2);
	}
	else if (StageHasPool() || StageHasHighGrounds() || mBackground == BACKGROUND_3_GRASS)
	{
		return ClampInt((theY - LAWN_YMIN) / 85, 0, MAX_GRID_SIZE_Y - 1);
	}
	else
	{
		return ClampInt((theY - LAWN_YMIN) / 100, 0, MAX_GRID_SIZE_Y - 2);
	}
}

//0x41C650
int Board::PixelToGridYKeepOnBoard(int theX, int theY)
{
	int aGridY = PixelToGridY(max(theX, 80), theY);
	return max(aGridY, 0);
}

//0x41C680
int Board::GridToPixelX(int theGridX, int theGridY)
{
	TOD_ASSERT(theGridX >= 0 && theGridX < MAX_GRID_SIZE_X);
	TOD_ASSERT(theGridY >= 0 && theGridY < MAX_GRID_SIZE_Y);
	if (mApp->mZenGarden && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE ||
			mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN ||
			mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
		{
			return mApp->mZenGarden->GridToPixelX(theGridX, theGridY);
		}
	}

	return theGridX * 80 + LAWN_XMIN;
}

//0x41C6C0
float Board::GetPosYBasedOnRow(float thePosX, int theRow)
{
	if (StageHasRoof())
	{
		float aSlopeOffset = 0.0f;
		if (thePosX < 440.0f)
		{
			aSlopeOffset = (440.0f - thePosX) * 0.25f;
		}

		return GridToPixelY(8, theRow) + aSlopeOffset;
	}

	return GridToPixelY(0, theRow);
}

//0x41C740
int Board::GridToPixelY(int theGridX, int theGridY)
{
	//TOD_ASSERT(theGridX >= 0 && theGridX < MAX_GRID_SIZE_X);
	//TOD_ASSERT(theGridY >= 0 && theGridY < MAX_GRID_SIZE_Y);
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE ||
			mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN ||
			mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
		{
			return mApp->mZenGarden->GridToPixelY(theGridX, theGridY);
		}
	}

	int aY;
	if (StageHasRoof())
	{
		int aSlopeOffset;
		if (theGridX < 5)
		{
			aSlopeOffset = (5 - theGridX) * 20;
		}
		else
		{
			aSlopeOffset = 0;
		}
		aY = theGridY * 85 + aSlopeOffset + LAWN_YMIN - 10;
	}
	else if (StageHasPool() || StageHasHighGrounds() || mBackground == BACKGROUND_3_GRASS)
	{
		aY = theGridY * 85 + LAWN_YMIN;
	}
	else
	{
		aY = theGridY * 100 + LAWN_YMIN;
	}

	if (theGridX != -1 && mGridSquareType[theGridX][theGridY] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		aY -= HIGH_GROUND_HEIGHT;
	}

	return aY;
}

ZombieID Board::ZombieGetID(Zombie* theZombie)
{
	return (ZombieID)mZombies.DataArrayGetID(theZombie);
}

Zombie* Board::ZombieGet(ZombieID theZombieID)
{
	return mZombies.DataArrayGet((unsigned int)theZombieID);
}

//0x41C7F0
Zombie* Board::ZombieTryToGet(ZombieID theZombieID)
{
	return mZombies.DataArrayTryToGet((unsigned int)theZombieID);
}

//0x41C820
int GetRectOverlap(const Rect& rect1, const Rect& rect2)
{
	int xmax, rmin, rmax;

	if (rect1.mX < rect2.mX)
	{
		rmin = rect1.mX + rect1.mWidth;
		rmax = rect2.mX + rect2.mWidth;
		xmax = rect2.mX;
	}
	else
	{
		rmin = rect2.mX + rect2.mWidth;
		rmax = rect1.mX + rect1.mWidth;
		xmax = rect1.mX;
	}

	if (rmin > xmax && rmin > rmax)
	{
		rmin = rmax;
	}

	return rmin - xmax;
}

//0x41C850
bool GetCircleRectOverlap(int theCircleX, int theCircleY, int theRadius, const Rect& theRect)
{
	int dx = 0;  // 圆心与矩形较近一条纵边的横向距离
	int dy = 0;  // 圆心与矩形较近一条横边的纵向距离
	bool xOut = false;  // 圆心横坐标是否不在矩形范围内
	bool yOut = false;  // 圆心纵坐标是否不在矩形范围内

	if (theCircleX < theRect.mX)
	{
		xOut = true;
		dx = theRect.mX - theCircleX;
	}
	else if (theCircleX > theRect.mX + theRect.mWidth)
	{
		xOut = true;
		dx = theCircleX - theRect.mX - theRect.mWidth;
	}
	if (theCircleY < theRect.mY)
	{
		yOut = true;
		dy = theRect.mY - theCircleY;
	}
	else if (theCircleY > theRect.mY + theRect.mHeight)
	{
		yOut = true;
		dy = theCircleY - theRect.mY - theRect.mHeight;
	}

	if (!xOut && !yOut)  // 如果圆心在矩形内
	{
		return true;
	}
	else if (xOut && yOut)
	{
		return dx * dx + dy * dy <= theRadius * theRadius;
	}
	else if (xOut)
	{
		return dx <= theRadius;
	}
	else
	{
		return dy <= theRadius;
	}
}

//0x41C8F0
bool Board::IterateZombies(Zombie*& theZombie)
{
	while (mZombies.IterateNext(theZombie))
	{
		if (!theZombie->mDead)
		{
			return true;
		}
	}

	theZombie = (Zombie*)-1;
	return false;
}

bool Board::IterateBushes(Bush*& theBush)
{
	while (mBushes.IterateNext(theBush))
	{
		return true;
	}

	theBush = (Bush*)-1;
	return false;
}

bool Board::IterateBushesCheap(BushCheap*& theBushCheap)
{
	while (mBushesCheap.IterateNext(theBushCheap))
	{
		return true;
	}

	theBushCheap = (BushCheap*)-1;
	return false;
}

//0x41C950
bool Board::IteratePlants(Plant*& thePlant)
{
	while (mPlants.IterateNext(thePlant))
	{
		if (!thePlant->mDead)
		{
			return true;
		}
	}

	thePlant = (Plant*)-1;
	return false;
}

//0x41C9B0
bool Board::IterateProjectiles(Projectile*& theProjectile)
{
	while (mProjectiles.IterateNext(theProjectile))
	{
		if (!theProjectile->mDead)
		{
			return true;
		}
	}

	theProjectile = (Projectile*)-1;
	return false;
}

//0x41CA10
bool Board::IterateCoins(Coin*& theCoin) 
{
	while (mCoins.IterateNext(theCoin))
	{
		if (!theCoin->mDead)
		{
			return true;
		}
	}

	theCoin = (Coin*)-1;
	return false;
}

//0x41CA70
bool Board::IterateLawnMowers(LawnMower*& theLawnMower)
{
	while (mLawnMowers.IterateNext(theLawnMower))
	{
		if (!theLawnMower->mDead)
		{
			return true;
		}
	}

	theLawnMower = (LawnMower*)-1;
	return false;
}

//0x41CAD0
bool Board::IterateGridItems(GridItem*& theGridItem)
{
	while (mGridItems.IterateNext(theGridItem))
	{
		if (!theGridItem->mDead)
		{
			return true;
		}
	}

	theGridItem = (GridItem*)-1;
	return false;
}


//0x41CB30
bool Board::IterateParticles(TodParticleSystem*& theParticle)
{
	while (mApp->mEffectSystem->mParticleHolder->mParticleSystems.IterateNext(theParticle))
	{
		if (!theParticle->mDead)
		{
			return true;
		}
	}

	theParticle = (TodParticleSystem*)-1;
	return false;
}

bool Board::IterateReanimations(Reanimation*& theReanimation)
{
	while (mApp->mEffectSystem->mReanimationHolder->mReanimations.IterateNext(theReanimation))
	{
		if (!theReanimation->mDead)
		{
			return true;
		}
	}

	theReanimation = (Reanimation*)-1;
	return false;
}


//0x41CBF0
void Board::KillAllPlantsInRadius(int theX, int theY, int theRadius)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (GetCircleRectOverlap(theX, theY, theRadius, aPlant->GetPlantRect()))
		{
			mPlantsEaten++;
			aPlant->Die();
		}
	}

	for (int i = 0; i < mMaxPlayers; i++) {
		Plant* mPlayerPlant = mExtraPlants.DataArrayTryToGet((unsigned int)mPlayers[i]->mPlayerPlantID);
		if (mPlayerPlant && mPlayerPlant->mSeedType != SEED_NONE && GetCircleRectOverlap(theX, theY, theRadius, mPlayerPlant->GetPlantRect())){
			mPlantsEaten++;
			mPlayerPlant->Die();
		}
	}
}

//0x41CC60
unsigned int Board::SeedNotRecommendedForLevel(SeedType theSeedType)
{
	unsigned int aNotRec = 0;
	if (Plant::IsNocturnal(theSeedType) && !StageIsNight())
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_NOCTURNAL, true);
	}
	if (theSeedType == SeedType::SEED_INSTANTCOFFEE && StageIsNight())
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_AT_NIGHT, true);
	}
	if (theSeedType == SeedType::SEED_GRAVEBUSTER && !StageHasGraveStones())
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_NEEDS_GRAVES, true);
	}
	if (theSeedType == SeedType::SEED_PLANTERN && !StageHasFog() && !StageHasDarkness())
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_NEEDS_FOG, true);
	}
	if (theSeedType == SeedType::SEED_FLOWERPOT && !StageHasRoof() && mApp->mGameMode != GAMEMODE_CHALLENGE_FLOOR_IS_LAVA && mBackground != BackgroundType::BACKGROUND_GRAVEYARD)
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_NEEDS_ROOF, true);
	}
	if (StageHasRoof() && (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK))
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_ON_ROOF, true);
	}
	if (!StageHasPool() && Plant::IsAquatic(theSeedType))
	{
		SetBit(aNotRec, NotRecommend::NOT_RECOMMENDED_NEEDS_POOL, true);
	}
	return aNotRec;
}

//0x41CD70
int Board::CountCoinByType(CoinType theCoinType)
{
	int aCount = 0;

	Coin* aCoin = nullptr;
	while (IterateCoins(aCoin))
	{
		if (aCoin->mType == theCoinType)
		{
			aCount++;
		}
	}

	return aCount;
}

int Board::GetGraveStoneCount()
{
	int aCount = 0;

	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE)
		{
			aCount++;
		}
	}

	return aCount;
}

//0x41CDB0
void Board::DropLootPiece(int thePosX, int thePosY, int theDropFactor)
{
	if (mApp->IsFirstTimeAdventureMode())
	{
		if (mLevel == 22 && mCurrentWave > 5 && !mApp->mPlayerInfo->mHasUnlockedMinigames && CountCoinByType(CoinType::COIN_PRESENT_MINIGAMES) == 0)
		{
			mApp->PlayFoley(FoleyType::FOLEY_ART_CHALLENGE);
			AddCoin(thePosX, thePosY, CoinType::COIN_PRESENT_MINIGAMES, CoinMotion::COIN_MOTION_COIN);
			return;
		}
		if (mLevel == 36 && mCurrentWave > 5 && !mApp->mPlayerInfo->mHasUnlockedPuzzleMode && CountCoinByType(CoinType::COIN_PRESENT_PUZZLE_MODE) == 0)
		{
			mApp->PlayFoley(FoleyType::FOLEY_ART_CHALLENGE);
			AddCoin(thePosX, thePosY, CoinType::COIN_PRESENT_PUZZLE_MODE, CoinMotion::COIN_MOTION_COIN);
			return;
		}
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
		Coin* aCoin = nullptr;
		int seedPeaShooters = 0;
		int seedWallnuts = 0;
		int seedLilyPads = 0;

		while (IterateCoins(aCoin)) {
			if (aCoin->mType == COIN_USABLE_SEED_PACKET) {
				if (aCoin->mUsableSeedType == SEED_PEASHOOTER) seedPeaShooters++;
				else if (aCoin->mUsableSeedType == SEED_WALLNUT) seedWallnuts++;
				else if (aCoin->mUsableSeedType == SEED_LILYPAD) seedLilyPads++;
			}
		}

		int requiredPeaShooterSeeds = 4 - CountPlantByType(SEED_PEASHOOTER);
		int requiredWallnutSeeds = 2 - CountPlantByType(SEED_WALLNUT);
		int requiredLilyPadSeeds = 4 - CountPlantByType(SEED_LILYPAD);

		requiredPeaShooterSeeds -= seedPeaShooters;
		requiredWallnutSeeds -= seedWallnuts;
		requiredLilyPadSeeds -= seedLilyPads;

		requiredPeaShooterSeeds = max(0, requiredPeaShooterSeeds);
		requiredWallnutSeeds = max(0, requiredWallnutSeeds);
		requiredLilyPadSeeds = max(0, requiredLilyPadSeeds);

		if (requiredPeaShooterSeeds > 0) {
			mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
			AddCoin(thePosX - 40, thePosY, COIN_USABLE_SEED_PACKET, COIN_MOTION_COIN)->mUsableSeedType = SEED_PEASHOOTER;
			return;
		}
		else if (requiredWallnutSeeds > 0) {
			mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
			AddCoin(thePosX - 40, thePosY, COIN_USABLE_SEED_PACKET, COIN_MOTION_COIN)->mUsableSeedType = SEED_WALLNUT;
			return;
		}
		else if (requiredLilyPadSeeds > 0) {
			mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
			AddCoin(thePosX - 40, thePosY, COIN_USABLE_SEED_PACKET, COIN_MOTION_COIN)->mUsableSeedType = SEED_LILYPAD;
			return;
		}
	}

	int aDropHit = Rand(30000);
	if (mApp->IsFirstTimeAdventureMode() && mLevel == 11 && !mDroppedFirstCoin && mCurrentWave > 5)
	{
		aDropHit = 1000;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
	{
		aDropHit *= 5;
	}

	if (mApp->IsWhackAZombieLevel())
	{
		int aSunChanceMin = 2500;
		int aSunChanceMax = mSunMoney > 500 ? 2800 : mSunMoney > 350 ? 3100 : mSunMoney > 200 ? 3700 : 5000;
		if (aDropHit >= aSunChanceMin * theDropFactor && aDropHit <= aSunChanceMax * theDropFactor)
		{
			mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
			AddCoin(thePosX - 20, thePosY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_COIN);
			AddCoin(thePosX - 40, thePosY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_COIN);
			AddCoin(thePosX - 60, thePosY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_COIN);
			return;
		}
	}

	if (mTotalSpawnedWaves > 70)
		return;

	int aPottedPlantChance;
	if (!mApp->mZenGarden->CanDropPottedPlantLoot())
	{
		aPottedPlantChance = 0;
	}
	else if (mApp->IsAdventureMode() && !mApp->IsFirstTimeAdventureMode())
	{
		aPottedPlantChance = 24;
	}
	else
	{
		aPottedPlantChance = mApp->IsSurvivalEndless(mApp->mGameMode) ? 3 : 12;
	}

	int aChocolateChance = aPottedPlantChance;
	if (mApp->mZenGarden->CanDropChocolate())
	{
		if (mApp->IsAdventureMode() && !mApp->IsFirstTimeAdventureMode())
		{
			aChocolateChance = aPottedPlantChance + 72;
		}
		else
		{
			aChocolateChance = aPottedPlantChance + (mApp->IsSurvivalEndless(mApp->mGameMode) ? 9 : 36);
		}
	}

	int aDiamondChance = aChocolateChance + 14;
	int aGoldChance = aChocolateChance + 250;
	int aSilverChance = aChocolateChance + 2500;

	if (mIsVeteran) {
		aDiamondChance = aChocolateChance + 14;
		aGoldChance = aChocolateChance + 2500;
		aSilverChance = aChocolateChance + 250;
	}

	CoinType aCoinType;
	if (aDropHit < aPottedPlantChance * theDropFactor)
	{
		aCoinType = CoinType::COIN_PRESENT_PLANT;
	}
	else if (aDropHit < aChocolateChance * theDropFactor)
	{
		aCoinType = CoinType::COIN_CHOCOLATE;
	}
	else if (aDropHit < aDiamondChance * theDropFactor)
	{
		aCoinType = mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_PACKET_UPGRADE] < 1 ? CoinType::COIN_GOLD : CoinType::COIN_DIAMOND;
	}
	else if (aDropHit < aGoldChance * theDropFactor)
	{
		aCoinType = CoinType::COIN_GOLD;
	}
	else if (aDropHit < aSilverChance * theDropFactor)
	{
		aCoinType = CoinType::COIN_SILVER;
	}
	else return;

	if (mApp->IsWallnutBowlingLevel() && Coin::IsMoney(aCoinType))
		return;

	if (mApp->IsFirstTimeAdventureMode() && mLevel == 11)
	{
		int aMoney = Coin::GetCoinValue(CoinType::COIN_GOLD) * mLawnMowers.mSize;
		int aCost = StoreScreen::GetItemCost(StoreItem::STORE_ITEM_PACKET_UPGRADE);
		aMoney += mApp->mPlayerInfo->mCoins + CountCoinsBeingCollected();
		if (Coin::GetCoinValue(aCoinType) + aMoney >= aCost)
		{
			return;
		}
	}

	mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
	AddCoin(thePosX - 40, thePosY, aCoinType, CoinMotion::COIN_MOTION_COIN);
	mDroppedFirstCoin = true;
}

//0x41D2C0
bool Board::CanDropLoot()
{
	return !mCutScene->ShouldRunUpsellBoard() && (!mApp->IsFirstTimeAdventureMode() || mLevel >= 11);
}

//0x41D320
bool Board::BungeeIsTargetingCell(int theGridX, int theGridY)
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying() && aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mRow == theGridY && aZombie->mTargetCol == theGridX)
		{
			return true;
		}
	}
	return false;
}

//0x41D390
Zombie* Board::GetBossZombie()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
		{
			return aZombie;
		}
	}
	return nullptr;
}

//0x41D3D0
Plant* Board::FindUmbrellaPlant(int theGridX, int theGridY)
{
	Plant* aPlant = nullptr;
	while (IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_UMBRELLA && !aPlant->NotOnGround() && GridInRange(theGridX, theGridY, aPlant->mPlantCol, aPlant->mRow, 1, 1))
		{
			return aPlant;
		}
	}
	return nullptr;
}

//0x41D450
void Board::DoFwoosh(int theRow)
{
	int aRenderOrder = MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, theRow, 1);
	for (int i = 0; i < 12; i++)
	{
		Reanimation* aOriReanim = mApp->ReanimationTryToGet(mFwooshID[theRow][i]);
		if (aOriReanim)
		{
			aOriReanim->ReanimationDie();
		}

		float aPosX = 750.0f  * i / 11.0f + 10.0f;
		float aPosY = GetPosYBasedOnRow(aPosX + 10.0f, theRow) - 10.0f - (mGridSquareType[min(i - 2, 8)][theRow] == GRIDSQUARE_HIGH_GROUND ? HIGH_GROUND_HEIGHT : 0);
		Reanimation* aFwoosh = mApp->AddReanimation(aPosX, aPosY, aRenderOrder, ReanimationType::REANIM_JALAPENO_FIRE);
		aFwoosh->SetFramesForLayer("anim_flame");
		aFwoosh->mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
		aFwoosh->mAnimRate *= RandRangeFloat(0.7f, 1.3f);

		float aScale = RandRangeFloat(0.9f, 1.1f);
		float aFlip = Rand(2) ? 1.0f : -1.0f;
		aFwoosh->OverrideScale(aScale * aFlip, 1);

		mFwooshID[theRow][i] = mApp->ReanimationGetID(aFwoosh);
		mFwooshGrid[i][theRow] = 100;
	}
	//mFwooshCountDown = 100;
}

//0x41D630
void Board::UpdateFwoosh()
{
	for (int aRow = 0; aRow < MAX_GRID_SIZE_Y; aRow++) {
		for (int aCol = 0; aCol < 12; aCol++)
		{
			int aFwooshRemaining = TodAnimateCurve(50, 0, --mFwooshGrid[aCol][aRow], 12, 0, TodCurves::CURVE_LINEAR);

			if (mFwooshGrid[aCol][aRow] == 0)
				continue;

			if (aCol >= 12 - aFwooshRemaining)
				continue;

			Reanimation* aFwoosh = mApp->ReanimationTryToGet(mFwooshID[aRow][aCol]);
			if (aFwoosh)
			{
				aFwoosh->SetFramesForLayer("anim_done");
				aFwoosh->mAnimRate = 15;
				aFwoosh->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME;
			}
			mFwooshID[aRow][aCol] = ReanimationID::REANIMATIONID_NULL;
		}
	}
}

//0x41D730
void Board::UpdateGridItems()
{
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (mEnableGraveStones && aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE && aGridItem->mGridItemCounter < 100)
		{
			aGridItem->mGridItemCounter++;
		}

		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_CRATER && mApp->mGameScene == GameScenes::SCENE_PLAYING)
		{
			if (aGridItem->mGridItemCounter > 0)
			{
				aGridItem->mGridItemCounter--;
			}
			if (aGridItem->mGridItemCounter == 0)
			{
				aGridItem->GridItemDie();
			}
		}
		aGridItem->Update();
	}
}

//0x41D7D0
bool Board::PlantingRequirementsMet(SeedType theSeedType)
{
	switch (theSeedType)
	{
	case SeedType::SEED_GATLINGPEA:			return CountPlantByType(SeedType::SEED_REPEATER);
	case SeedType::SEED_TWINSUNFLOWER:		return CountPlantByType(SeedType::SEED_SUNFLOWER);
	case SeedType::SEED_GLOOMSHROOM:		return CountPlantByType(SeedType::SEED_FUMESHROOM);
	case SeedType::SEED_CATTAIL:			return CountEmptyPotsOrLilies(SeedType::SEED_LILYPAD);
	case SeedType::SEED_WINTERMELON:		return CountPlantByType(SeedType::SEED_MELONPULT);
	case SeedType::SEED_GOLD_MAGNET:		return CountPlantByType(SeedType::SEED_MAGNETSHROOM);
	case SeedType::SEED_SPIKEROCK:			return CountPlantByType(SeedType::SEED_SPIKEWEED);
	case SeedType::SEED_PLASMAWOOD:			return CountPlantByType(SeedType::SEED_TORCHWOOD);
	case SeedType::SEED_SHOOTINGSTAR:		return CountPlantByType(SeedType::SEED_STARFRUIT);
	case SeedType::SEED_BRAVEYSHROOM:		return CountPlantByType(SeedType::SEED_SCAREDYSHROOM);
	case SeedType::SEED_SUPERCHOMPER:		return CountPlantByType(SeedType::SEED_CHOMPER);
	case SeedType::SEED_COBCANNON:			return HasValidCobCannonSpot();
	default:								return true;
	}
}

void Board::KillAllPlantsInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags, bool mEnemy)
{
	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS) {
		Plant* aPlant = nullptr;
		while (IteratePlants(aPlant))
		{
			if (aPlant->mEnemy == mEnemy)	continue;

			Rect aPlantRect = aPlant->GetPlantRect();
			int aRowDist = aPlant->mRow - theRow;

			if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aPlantRect)) {
				if (theBurn) {
					aPlant->ApplyBurn();
				}
				else {
					aPlant->mPlantHealth -= 1800;
					aPlant->mEatenFlashCountdown = 25;
				}
			}
		}
	}
}
//0x41D8A0
void Board::KillAllZombiesInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags)
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->EffectedByDamage(theDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			int aRowDist = aZombie->mRow - theRow;

			if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
				aRowDist = 0;
			
			if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aZombieRect))
			{
				if (theBurn && aZombie->mZombieType != ZOMBIE_GLASSDOOR)
				{
					aZombie->ApplyBurn();
				}
				else
				{
					aZombie->TakeDamage(1800, aZombie->mZombieType == ZOMBIE_GLASSDOOR ? 0 : 18U);
				}
				
			}
		}
	}
	
	int aGridX = PixelToGridXKeepOnBoard(theX, theY);
	int aGridY = PixelToGridYKeepOnBoard(theX, theY);
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
		{
			if (GridInRange(aGridItem->mGridX, aGridItem->mGridY, aGridX, aGridY, theRowRange, theRowRange))
			{
				aGridItem->GridItemDie();
			}
		}
	}
}

void Board::DamageAllZombieInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags, int daDamage)
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->EffectedByDamage(theDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			int aRowDist = aZombie->mRow - theRow;
			if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
				aRowDist = 0;
			

			if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aZombieRect))
				aZombie->TakeBurnDamage(daDamage, 18U);
		}
	}

	int aGridX = PixelToGridXKeepOnBoard(theX, theY);
	int aGridY = PixelToGridYKeepOnBoard(theX, theY);
	GridItem* aGridItem = nullptr;
	while (IterateGridItems(aGridItem))
	{
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
		{
			if (GridInRange(aGridItem->mGridX, aGridItem->mGridY, aGridX, aGridY, theRowRange, theRowRange))
			{
				aGridItem->GridItemDie();
			}
		}
	}
}

//0x41DA10
int Board::GetNumWavesPerSurvivalStage()
{
	if (LawnApp::IsLastStandLevel(mApp->mGameMode) || mApp->IsSurvivalNormal(mApp->mGameMode))
	{
		return 10;
	}
	else if (mApp->IsSurvivalHard(mApp->mGameMode) || mApp->IsSurvivalEndless(mApp->mGameMode))
	{
		return 20;
	}

	TOD_ASSERT();
}

//0x41DA50
void Board::RemoveParticleByType(ParticleEffect theEffectType)
{
	TodParticleSystem* aParticle = nullptr;
	while (IterateParticles(aParticle))
	{
		if (aParticle->mEffectType == theEffectType)
		{
			aParticle->ParticleSystemDie();
		}
	}
}

//0x41DA90
bool Board::PlantUsesAcceleratedPricing(SeedType theSeedType)
{
	return Plant::IsUpgrade(theSeedType) && mApp->IsSurvivalEndless(mApp->mGameMode) || mApp->mGameMode == GAMEMODE_CHALLENGE_INFLATION && Plant::GetCost(theSeedType) != 0;
}

//0x41DAE0
int Board::GetCurrentPlantCost(SeedType theSeedType, SeedType theImitaterType)
{
	if (theSeedType == SEED_IMITATER)
	{
		theSeedType = theImitaterType;
		theImitaterType = SEED_NONE;
	}

	int aCost = Plant::GetCost(theSeedType, theImitaterType);
	if (PlantUsesAcceleratedPricing(theSeedType))
	{
		aCost += CountPlantByType(theSeedType) * 50;
	}
	return aCost;
}

//0x41DB40
bool Board::CanUseGameObject(GameObjectType theGameObject)
{
	if (mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		return theGameObject == GameObjectType::OBJECT_TYPE_TREE_FOOD || theGameObject == GameObjectType::OBJECT_TYPE_NEXT_GARDEN;
	}
	if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		return false;
	}

	if (theGameObject == GameObjectType::OBJECT_TYPE_WATERING_CAN)
	{
		return true;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_NEXT_GARDEN)
	{
		return 
			mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_MUSHROOM_GARDEN] || 
			mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_AQUARIUM_GARDEN] ||
			mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_OF_WISDOM];
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_FERTILIZER)
	{
		return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_BUG_SPRAY)
	{
		return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_PHONOGRAPH)
	{
		return  mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_PHONOGRAPH] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_CHOCOLATE)
	{
		return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_WHEELBARROW)
	{
		return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_WHEEL_BARROW] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_GLOVE)
	{
		return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GARDENING_GLOVE] > 0;
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_MONEY_SIGN)
	{
		return mApp->HasFinishedAdventure();
	}
	if (theGameObject == GameObjectType::OBJECT_TYPE_TREE_FOOD)
	{
		return false;
	}
	
	TOD_ASSERT();
}

void Board::ShakeBoard(int theShakeAmountX, int theShakeAmountY)
{
	mShakeCounter = 12;
	mShakeAmountX = theShakeAmountX;
	mShakeAmountY = theShakeAmountY;
}

LawnMower* Board::FindLawnMowerInRow(int theRow)
{
	LawnMower* aLawnMower = nullptr;
	while (IterateLawnMowers(aLawnMower))
	{
		if (aLawnMower->mRow == theRow)
		{
			return aLawnMower;
		}
	}
	return nullptr;
}

Zombie* Board::GetWinningZombie()
{
	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mFromWave == Zombie::ZOMBIE_WAVE_WINNER)
		{
			return aZombie;
		}
	}
	return nullptr;
}

int Board::CountZombieByType(ZombieType theZombieType)
{
	int aCount = 0;

	Zombie* aZombie = nullptr;
	while (IterateZombies(aZombie))
	{
		if (aZombie->mZombieType == theZombieType)
		{
			aCount++;
		}
	}

	return aCount;
}

int Board::NumberZombiesInWave(int theWaveIndex)
{
	TOD_ASSERT(theWaveIndex >= 0 && theWaveIndex < MAX_ZOMBIE_WAVES && theWaveIndex < mNumWaves);

	for (int i = 0; i < MAX_ZOMBIES_IN_WAVE; i++)
	{
		if (mZombiesInWave[theWaveIndex][i] == ZombieType::ZOMBIE_INVALID)
		{
			return i;
		}
	}

	TOD_ASSERT();
	return 0;
}

bool Board::IsZombieTypeSpawnedOnly(ZombieType theZombieType)
{
	return (theZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || theZombieType == ZombieType::ZOMBIE_DISCO_BACKUP || theZombieType == ZombieType::ZOMBIE_BOBSLED || theZombieType == ZombieType::ZOMBIE_IMP);
}

void Board::MovePlant(Plant* thePlant, int theGridX, int theGridY)
{
	Plant* aTopPlantAtGrid = GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
	if (aTopPlantAtGrid)
	{
		PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
		RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

		if ((aTopPlantAtGrid->mSeedType == SeedType::SEED_FLOWERPOT || aTopPlantAtGrid->mSeedType == SeedType::SEED_FLYINGPOT || aTopPlantAtGrid->mSeedType == SeedType::SEED_WATERPOT))
			anOrder = PLANT_ORDER::PLANT_ORDER_UNDERLILYPAD;
		else if (aTopPlantAtGrid->mSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
			anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;

		aTopPlantAtGrid->mRenderOrder = Board::MakeRenderOrder(aLayer, aTopPlantAtGrid->mRow, anOrder * 5 - aTopPlantAtGrid->mX + 800);
	}

	int aPosX = GridToPixelX(theGridX, theGridY);
	int aPosY = GridToPixelY(theGridX, theGridY);

	float aDeltaX = aPosX - thePlant->mX;
	float aDeltaY = aPosY - thePlant->mY;
	thePlant->mX = aPosX;
	thePlant->mY = aPosY;
	thePlant->mPlantCol = theGridX;
	thePlant->mRow = theGridY;

	PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
	RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

	if ((thePlant->mSeedType == SeedType::SEED_FLOWERPOT || aTopPlantAtGrid && aTopPlantAtGrid->mSeedType == SeedType::SEED_FLYINGPOT || thePlant->mSeedType == SeedType::SEED_WATERPOT))
	{
		anOrder = PLANT_ORDER::PLANT_ORDER_UNDERLILYPAD;
	}
	else if (thePlant->mSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
	{
		anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;
	}

	thePlant->mRenderOrder = Board::MakeRenderOrder(aLayer, theGridY, anOrder * 5 - thePlant->mX + 800);

	TodParticleSystem* aParticle = mApp->ParticleTryToGet(thePlant->mParticleID);
	if (aParticle && aParticle->mEmitterList.mSize)
	{
		TodParticleEmitter* aEmitter = aParticle->mParticleHolder->mEmitters.DataArrayGet((unsigned int)aParticle->mEmitterList.GetHead()->mValue);
		aParticle->SystemMove(aEmitter->mSystemCenter.x + aDeltaX, aEmitter->mSystemCenter.y + aDeltaY);
	}

	DoPlantingEffects(theGridX, theGridY, thePlant);
}












