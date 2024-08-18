#include "../Board.h"
#include "../Zombie.h"
#include "GameButton.h"
#include "StoreScreen.h"
#include "../Cutscene.h"
#include "../SeedPacket.h"
#include "../../LawnApp.h"
#include "AlmanacDialog.h"
#include "ImitaterDialog.h"
#include "../System/Music.h"
#include "../../Resources.h"
#include "../../Lawn/Plant.h"
#include "../ToolTipWidget.h"
#include "SeedChooserScreen.h"
#include "../../GameConstants.h"
#include "../System/PlayerInfo.h"
#include "../System/PopDRMComm.h"
#include "../../SexyAppFramework/Debug.h"
#include "../../SexyAppFramework/Dialog.h"
#include "../../SexyAppFramework/MTRand.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/WidgetManager.h"

const Rect cSeedClipRect = Rect(-BOARD_ADDITIONAL_WIDTH, 122 - BOARD_OFFSET_Y, 465, 420 + 70);
//0x483380
SeedChooserScreen::SeedChooserScreen()
{
	mApp = (LawnApp*)gSexyAppBase;
	mBoard = mApp->mBoard;

	mHitOffsetX = BOARD_ADDITIONAL_WIDTH;
	mHitOffsetY = BOARD_OFFSET_Y;

	mClip = false;
	// mSeedChooserAge = 0;  原版并没有初始化 mSeedChooserAge
	mSeedsInFlight = 0;
	mSeedsInBank = 0;
	mLastMouseX = -1;
	mLastMouseY = -1;
	mChooseState = CHOOSE_NORMAL;
	mViewLawnTime = 0;
	mToolTip = new ToolTipWidget();
	mToolTipSeed = -1;
	mScrollAmount = 0;
	mScrollPosition = 0;
	mMaxScrollPosition = 70 + 70 * max(0, floor((SeedType::NUM_SEEDS_IN_CHOOSER - 48) / 8));

	mStartButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Start);
	mStartButton->SetLabel(_S("[LETS_ROCK_BUTTON]"));
	mStartButton->mButtonImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON;
	mStartButton->mOverImage = nullptr;
	mStartButton->mDownImage = nullptr;
	mStartButton->mDisabledImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON_DISABLED;
	mStartButton->mOverOverlayImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW;
	mStartButton->SetFont(Sexy::FONT_DWARVENTODCRAFT18YELLOW);
	mStartButton->Resize(154 - BOARD_ADDITIONAL_WIDTH, 545 + BOARD_OFFSET_Y, 156, 42);
	mStartButton->mTextOffsetY = -1;
	mStartButton->mParentWidget = this;
	EnableStartButton(false);

	mMenuButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Menu);
	mMenuButton->SetLabel(_S("[MENU_BUTTON]"));
	mMenuButton->Resize(681 + BOARD_ADDITIONAL_WIDTH, -10 - BOARD_OFFSET_Y, 117, 46);
	mMenuButton->mDrawStoneButton = true;
	mMenuButton->mParentWidget = this;

	Color aBtnColor = Color(42, 42, 90);
	Image* aBtnImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON2;
	Image* aOverImage = Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW;
	int aImageWidth = aBtnImage->GetWidth();
	int aImageHeight = aOverImage->GetHeight();

	mRandomButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Random);
	mRandomButton->SetLabel(_S("[RANDOM_SET_BUTTON]"));
	mRandomButton->mButtonImage = aBtnImage;
	mRandomButton->mOverImage = aOverImage;
	mRandomButton->mDownImage = nullptr;
	mRandomButton->SetFont(Sexy::FONT_BRIANNETOD12);
	mRandomButton->mColors[0] = aBtnColor;
	mRandomButton->mColors[1] = aBtnColor;
	mRandomButton->Resize(332 - BOARD_ADDITIONAL_WIDTH, 561 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mRandomButton->mParentWidget = this;
	mRandomButton->mTextOffsetY = 1;

	/*if (!mApp->mTodCheatKeys)
	{
		mRandomButton->mBtnNoDraw = true;
		mRandomButton->mDisabled = true;
	}*/

	mViewLawnButton = new GameButton(SeedChooserScreen::SeedChooserScreen_ViewLawn);
	mViewLawnButton->SetLabel(_S("[VIEW_LAWN]"));
	mViewLawnButton->mButtonImage = aBtnImage;
	mViewLawnButton->mOverImage = aOverImage;
	mViewLawnButton->mDownImage = nullptr;
	mViewLawnButton->SetFont(Sexy::FONT_BRIANNETOD12);
	mViewLawnButton->mColors[0] = aBtnColor;
	mViewLawnButton->mColors[1] = aBtnColor;
	mViewLawnButton->Resize(22 - BOARD_ADDITIONAL_WIDTH, 561 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mViewLawnButton->mParentWidget = this;
	mViewLawnButton->mTextOffsetY = 1;
	if (!mBoard->mCutScene->IsSurvivalRepick())
	{
		mViewLawnButton->mBtnNoDraw = true;
		mViewLawnButton->mDisabled = true;
	}

	mAlmanacButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Almanac);
	mAlmanacButton->SetLabel(_S("[ALMANAC_BUTTON]"));
	mAlmanacButton->mButtonImage = aBtnImage;
	mAlmanacButton->mOverImage = aOverImage;
	mAlmanacButton->mDownImage = nullptr;
	mAlmanacButton->SetFont(Sexy::FONT_BRIANNETOD12);
	mAlmanacButton->mColors[0] = aBtnColor;
	mAlmanacButton->mColors[1] = aBtnColor;
	mAlmanacButton->Resize(560 + BOARD_ADDITIONAL_WIDTH, 572 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mAlmanacButton->mParentWidget = this;
	mAlmanacButton->mTextOffsetY = 1;

	mStoreButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Store);
	mStoreButton->SetLabel(_S("[SHOP_BUTTON]"));
	mStoreButton->mButtonImage = aBtnImage;
	mStoreButton->mOverImage = aOverImage;
	mStoreButton->mDownImage = nullptr;
	mStoreButton->SetFont(Sexy::FONT_BRIANNETOD12);
	mStoreButton->mColors[0] = aBtnColor;
	mStoreButton->mColors[1] = aBtnColor;
	mStoreButton->Resize(680 + BOARD_ADDITIONAL_WIDTH, 572 + BOARD_OFFSET_Y, aImageWidth, aImageHeight);
	mStoreButton->mParentWidget = this;
	mStoreButton->mTextOffsetY = 1;

	mImitaterButton = new GameButton(SeedChooserScreen::SeedChooserScreen_Imitater);
	mImitaterButton->mButtonImage = Sexy::IMAGE_IMITATERSEED;
	mImitaterButton->mOverImage = Sexy::IMAGE_IMITATERSEED;
	mImitaterButton->mDownImage = Sexy::IMAGE_IMITATERSEED;
	mImitaterButton->mDisabledImage = Sexy::IMAGE_IMITATERSEEDDISABLED;
	mImitaterButton->Resize(464 - BOARD_ADDITIONAL_WIDTH, 515 + BOARD_OFFSET_Y, Sexy::IMAGE_IMITATERSEED->mWidth, Sexy::IMAGE_IMITATERSEED->mHeight);
	mImitaterButton->mParentWidget = this;

	if (!mApp->CanShowAlmanac() )//|| mApp->mGameMode == GAMEMODE_CHALLENGE_PLANTS_VS_PLANTS)
	{
		mAlmanacButton->mBtnNoDraw = true;
		mAlmanacButton->mDisabled = true;
	}
	if (!mApp->CanShowStore() )//|| mApp->mGameMode == GAMEMODE_CHALLENGE_PLANTS_VS_PLANTS)
	{
		mStoreButton->mBtnNoDraw = true;
		mStoreButton->mDisabled = true;
	}

	mLastPickedButton = new GameButton(SeedChooserScreen::SeedChooserScreen_LastPicked);
	mLastPickedButton->SetLabel(_S("[LAST_PICKED_BUTTON]"));
	mLastPickedButton->mButtonImage = aBtnImage;
	mLastPickedButton->mOverImage = aOverImage;
	mLastPickedButton->mDownImage = nullptr;
	mLastPickedButton->SetFont(Sexy::FONT_BRIANNETOD12);
	mLastPickedButton->mColors[0] = aBtnColor;
	mLastPickedButton->mColors[1] = aBtnColor;
	mLastPickedButton->Resize(332 - BOARD_ADDITIONAL_WIDTH, 561 + BOARD_OFFSET_Y - 26, aImageWidth, aImageHeight);
	mLastPickedButton->mParentWidget = this;
	mLastPickedButton->mTextOffsetY = 1;

	mSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT4, IMAGE_OPTIONS_SLIDERKNOB3, 0, this);
	mSlider->SetValue(max(0.0, min(mMaxScrollPosition, mScrollPosition)));
	mSlider->mHorizontal = false;
	mSlider->Resize(470 - BOARD_ADDITIONAL_WIDTH, 92 - BOARD_OFFSET_Y, 40, 405 + 120);
	mSlider->mThumbOffsetX -= 14;
	mSlider->mVisible = mMaxScrollPosition > 0;

	DBG_ASSERT(mApp->GetSeedsAvailable() <= NUM_SEED_TYPES);
	memset(mChosenSeeds, 0, sizeof(mChosenSeeds));
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		aChosenSeed.mSeedType = aSeedType;
		GetSeedPositionInChooser(aSeedType, aChosenSeed.mX, aChosenSeed.mY);
		aChosenSeed.mTimeStartMotion = 0;
		aChosenSeed.mTimeEndMotion = 0;
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		aChosenSeed.mEndX = aChosenSeed.mX;
		aChosenSeed.mEndY = aChosenSeed.mY;
		aChosenSeed.mSeedState = aSeedType == SEED_IMITATER ? SEED_PACKET_HIDDEN : SEED_IN_CHOOSER;
		aChosenSeed.mSeedIndexInBank = 0;
		aChosenSeed.mRefreshCounter = 0;
		aChosenSeed.mRefreshing = false;
		aChosenSeed.mImitaterType = SEED_NONE;
		aChosenSeed.mCrazyDavePicked = false;
	}
	if (mBoard->mCutScene->IsSurvivalRepick())
	{
		for (int anIdx = 0; anIdx < mBoard->mSeedBank->mNumPackets; anIdx++)
		{
			SeedPacket* aSeedPacket = &mBoard->mSeedBank->mSeedPackets[anIdx];
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedPacket->mPacketType];
			aChosenSeed.mRefreshing = aSeedPacket->mRefreshing;
			aChosenSeed.mRefreshCounter = aSeedPacket->mRefreshCounter;
		}
		mBoard->mSeedBank->mNumPackets = 0;

		if (mBoard->mMaxPlayers > 1)
		{
			for (int anIdx = 0; anIdx < mBoard->mSeedBankP2->mNumPackets; anIdx++)
			{
				SeedPacket* aSeedPacket = &mBoard->mSeedBankP2->mSeedPackets[anIdx];
				ChosenSeed& aChosenSeed = mChosenSeeds[aSeedPacket->mPacketType];
				aChosenSeed.mRefreshing = aSeedPacket->mRefreshing;
				aChosenSeed.mRefreshCounter = aSeedPacket->mRefreshCounter;
			}
			mBoard->mSeedBankP2->mNumPackets = 0;
		}
	}
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SEEING_STARS)
	{
		ChosenSeed& aStarFruit = mChosenSeeds[SEED_STARFRUIT];
		int aX = mBoard->GetSeedPacketPositionX(0) + 10 - BOARD_ADDITIONAL_WIDTH;
		aStarFruit.mX = aX, aStarFruit.mY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mStartX = aX, aStarFruit.mStartY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mEndX = aX, aStarFruit.mEndY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mSeedState = SEED_IN_BANK;
		aStarFruit.mSeedIndexInBank = 0;
		mSeedsInBank++;
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_ART_SEEING_NUTS)
	{
		ChosenSeed& aStarFruit = mChosenSeeds[SEED_WALLNUT];
		int aX = mBoard->GetSeedPacketPositionX(0) + 10 - BOARD_ADDITIONAL_WIDTH;
		aStarFruit.mX = aX, aStarFruit.mY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mStartX = aX, aStarFruit.mStartY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mEndX = aX, aStarFruit.mEndY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mSeedState = SEED_IN_BANK;
		aStarFruit.mSeedIndexInBank = 0;
		mSeedsInBank++;
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SEEING_FLOWERS)
	{
		ChosenSeed& aStarFruit = mChosenSeeds[SEED_SUNFLOWER];
		int aX = mBoard->GetSeedPacketPositionX(0) + 10 - BOARD_ADDITIONAL_WIDTH;
		aStarFruit.mX = aX, aStarFruit.mY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mStartX = aX, aStarFruit.mStartY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mEndX = aX, aStarFruit.mEndY = 8 - BOARD_OFFSET_Y;
		aStarFruit.mSeedState = SEED_IN_BANK;
		aStarFruit.mSeedIndexInBank = 0;
		mSeedsInBank++;

		ChosenSeed& aStarFruit2 = mChosenSeeds[SEED_TWINSUNFLOWER];
		int aX2 = mBoard->GetSeedPacketPositionX(1) + 10 - BOARD_ADDITIONAL_WIDTH;
		aStarFruit2.mX = aX2, aStarFruit2.mY = 8 - BOARD_OFFSET_Y;
		aStarFruit2.mStartX = aX2, aStarFruit2.mStartY = 8 - BOARD_OFFSET_Y;
		aStarFruit2.mEndX = aX2, aStarFruit2.mEndY = 8 - BOARD_OFFSET_Y;
		aStarFruit2.mSeedState = SEED_IN_BANK;
		aStarFruit2.mSeedIndexInBank = 1;
		mSeedsInBank++;
	}

	if (mApp->IsAdventureMode() && !mApp->IsFirstTimeAdventureMode())
		CrazyDavePickSeeds();
	UpdateImitaterButton();
}

int SeedChooserScreen::PickFromWeightedArrayUsingSpecialRandSeed(TodWeightedArray* theArray, int theCount, MTRand& theLevelRNG)
{
	int aTotalWeight = 0;
	for (int i = 0; i < theCount; i++)
		aTotalWeight += theArray[i].mWeight;
	DBG_ASSERT(aTotalWeight > 0);

	int aRndResult = theLevelRNG.Next((unsigned long)aTotalWeight);
	int aWeight = 0;
	for (int j = 0; j < theCount; j++)
	{
		aWeight += theArray[j].mWeight;
		if (aWeight > aRndResult) return theArray[j].mItem;
	}
	DBG_ASSERT(false);
}

//0x483F70
void SeedChooserScreen::CrazyDavePickSeeds()
{
	TodWeightedArray aSeedArray[NUM_SEED_TYPES];
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		aSeedArray[aSeedType].mItem = aSeedType;
		uint aRecFlags = SeedNotRecommendedToPick(aSeedType);
		if ((aSeedType == SEED_GATLINGPEA && !mApp->mPlayerInfo->mPurchases[STORE_ITEM_PLANT_GATLINGPEA]) || !mApp->SeedTypeAvailable(aSeedType) ||
			SeedNotAllowedToPick(aSeedType) || Plant::IsUpgrade(aSeedType) || aSeedType == SEED_IMITATER || aSeedType == SEED_UMBRELLA || aSeedType == SEED_BLOVER)
		{
			aSeedArray[aSeedType].mWeight = 0;
		}
		else
		{
			aSeedArray[aSeedType].mWeight = 1;
		}
	}
	if (mBoard->mZombieAllowed[ZOMBIE_BUNGEE] || mBoard->mZombieAllowed[ZOMBIE_CATAPULT])
	{
		aSeedArray[SEED_UMBRELLA].mWeight = 1;
	}
	if (mBoard->mZombieAllowed[ZOMBIE_BALLOON] || mBoard->StageHasFog())
	{
		aSeedArray[SEED_BLOVER].mWeight = 1;
	}
	if (mBoard->StageHasRoof())
	{
		aSeedArray[SEED_TORCHWOOD].mWeight = 0;
	}

	MTRand aLevelRNG = MTRand(mBoard->GetLevelRandSeed());
	for (int i = 0; i < 3; i++)
	{
		SeedType aPickedSeed = (SeedType)PickFromWeightedArrayUsingSpecialRandSeed(aSeedArray, NUM_SEEDS_IN_CHOOSER, aLevelRNG);
		aSeedArray[aPickedSeed].mWeight = 0;
		ChosenSeed& aChosenSeed = mChosenSeeds[aPickedSeed];

		int aPosX = mBoard->GetSeedPacketPositionX(i) + 10;
		aChosenSeed.mX = aPosX - BOARD_ADDITIONAL_WIDTH;
		aChosenSeed.mY = 8 - BOARD_OFFSET_Y;
		aChosenSeed.mStartX = aPosX - BOARD_ADDITIONAL_WIDTH;
		aChosenSeed.mStartY = 8 - BOARD_OFFSET_Y;
		aChosenSeed.mEndX = aPosX - BOARD_ADDITIONAL_WIDTH;
		aChosenSeed.mEndY = 8 - BOARD_OFFSET_Y;
		aChosenSeed.mSeedState = SEED_IN_BANK;
		aChosenSeed.mSeedIndexInBank = i;
		aChosenSeed.mCrazyDavePicked = true;
		mSeedsInBank++;
	}
}

//0x484220
bool SeedChooserScreen::Has7Rows()
{
	PlayerInfo* aPlayer = mApp->mPlayerInfo;
	if (mApp->HasFinishedAdventure() || mApp->mPlayerInfo->mPurchases[STORE_ITEM_PLANT_GATLINGPEA]) return true;
	for (SeedType aSeedType = SEED_TWINSUNFLOWER; aSeedType < SEED_COBCANNON; aSeedType = (SeedType)(aSeedType + 1))
		if (aSeedType != SEED_SPIKEROCK && mApp->SeedTypeAvailable(aSeedType)) return true;
	return false;
}

//0x484400
void SeedChooserScreen::GetSeedPositionInChooser(int theIndex, int& x, int& y)
{
	if (theIndex == SEED_IMITATER)
	{
		x = mImitaterButton->mX;
		y = mImitaterButton->mY;
	}
	else
	{
		if (theIndex > SEED_IMITATER)
			--theIndex;

		int aRow = theIndex / 8;
		int aCol = theIndex % 8;

		x = aCol * 53 + 22 - BOARD_ADDITIONAL_WIDTH;
		y = aRow * 70 + 123 - BOARD_OFFSET_Y;
		y -= (int)mScrollPosition;
	}
}

//0x484480
void SeedChooserScreen::GetSeedPositionInBank(int theIndex, int& x, int& y)
{
	x = mBoard->mSeedBank->mX - mX + mBoard->GetSeedPacketPositionX(theIndex) + BOARD_ADDITIONAL_WIDTH;
	y = mBoard->mSeedBank->mY - mY + 8 + BOARD_OFFSET_Y;
}

//0x4844D0
SeedChooserScreen::~SeedChooserScreen()
{
	if (mStartButton) delete mStartButton;
	if (mRandomButton) delete mRandomButton;
	if (mViewLawnButton) delete mViewLawnButton;
	if (mAlmanacButton) delete mAlmanacButton;
	if (mImitaterButton) delete mImitaterButton;
	if (mStoreButton) delete mStoreButton;
	if (mToolTip) delete mToolTip;
	if (mMenuButton) delete mMenuButton;
	if (mLastPickedButton) delete mLastPickedButton;
	if (mSlider) delete mSlider;
}

//0x4845E0
unsigned int SeedChooserScreen::SeedNotRecommendedToPick(SeedType theSeedType)
{
	uint aRecFlags = mBoard->SeedNotRecommendedForLevel(theSeedType);
	if (TestBit(aRecFlags, NOT_RECOMMENDED_NOCTURNAL) && PickedPlantType(SEED_INSTANTCOFFEE))
		SetBit(aRecFlags, NOT_RECOMMENDED_NOCTURNAL, false);
	if (TestBit(aRecFlags, NOT_RECOMMENDED_NEEDS_POOL) && PickedPlantType(SEED_WATERPOT))
		SetBit(aRecFlags, NOT_RECOMMENDED_NEEDS_POOL, false);
	return aRecFlags;
}

//0x484620
bool SeedChooserScreen::SeedNotAllowedToPick(SeedType theSeedType)
{
	if (LawnApp::IsLastStandLevel(mApp->mGameMode) && (theSeedType == SEED_SUNFLOWER || theSeedType == SEED_SUNSHROOM ||
		theSeedType == SEED_TWINSUNFLOWER || theSeedType == SEED_SEASHROOM || theSeedType == SEED_PUFFSHROOM || theSeedType == SEED_SUNBLOOM || theSeedType == SEED_MOONFLOWER ||
		theSeedType == SEED_PRIMAL_SUNFLOWER || theSeedType == SEED_SUNBLOOM))
		return true;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BLIND_FAITH && (theSeedType == SEED_BLOVER || theSeedType == SEED_HURRIKALE || theSeedType == SEED_PLANTERN || theSeedType == SEED_TORCHWOOD))
		return true;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_LIGHTS_OUT && theSeedType == SEED_PLANTERN)
		return true;

	return false;
}

//0x484640
bool SeedChooserScreen::SeedNotAllowedDuringTrial(SeedType theSeedType)
{
	return mApp->IsTrialStageLocked() && (theSeedType == SEED_SQUASH || theSeedType == SEED_THREEPEATER);
}

//0x484690
void SeedChooserScreen::Draw(Graphics* g)
{
	if (mApp->GetDialog(DIALOG_STORE) || mApp->GetDialog(DIALOG_ALMANAC))
		return;

	g->SetLinearBlend(true);
	if (!mBoard->ChooseSeedsOnCurrentLevel() || (mBoard->mCutScene && mBoard->mCutScene->IsBeforePreloading()))
		return;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_ZOMBIEZOMBIE) {
		g->DrawImage(Sexy::IMAGE_SEEDCHOOSER_BACKGROUND2, 0, 87);
		TodDrawString(g, _S("Pik Ur Zombeez"), 154, 110, Sexy::FONT_DWARVENTODCRAFT18YELLOW, Color(41, 41, 98), DS_ALIGN_CENTER);
	}else{
	g->DrawImage(Sexy::IMAGE_SEEDCHOOSER_BACKGROUND, -BOARD_ADDITIONAL_WIDTH, 87 - BOARD_OFFSET_Y);
	if (mApp->SeedTypeAvailable(SEED_IMITATER))
	{
		g->DrawImage(Sexy::IMAGE_SEEDCHOOSER_IMITATERADDON, 459 - BOARD_ADDITIONAL_WIDTH, 503 + BOARD_OFFSET_Y);
	}
	TodDrawString(g, _S("[CHOOSE_YOUR_PLANTS]"), 229 - BOARD_ADDITIONAL_WIDTH, 110 - BOARD_OFFSET_Y, Sexy::FONT_DWARVENTODCRAFT18YELLOW, Color::White, DS_ALIGN_CENTER);
	}

	for (SeedType aSeedShadow = SEED_PEASHOOTER; aSeedShadow < NUM_SEEDS_IN_CHOOSER; aSeedShadow = (SeedType)(aSeedShadow + 1))
	{
		int x, y;
		GetSeedPositionInChooser((int)aSeedShadow, x, y);

		if (aSeedShadow == SEED_IMITATER)
			continue;

		g->SetClipRect(cSeedClipRect);
		if (mApp->SeedTypeAvailable(aSeedShadow)){
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedShadow];
			if (aChosenSeed.mSeedState != SEED_IN_CHOOSER){
				DrawSeedPacket(g, x, y, aSeedShadow, SEED_NONE, 0, 55, true, false);
			}
		}
		else{
			g->DrawImage(Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
		}
		g->ClearClipRect();
		
	}

	int aNumSeedsInBank = mBoard->mSeedBank->mNumPackets;
	for (int anIndex = 0; anIndex < aNumSeedsInBank; anIndex++)
	{
		if (FindSeedInBank(anIndex) == SEED_NONE)
		{
			int x, y;
			GetSeedPositionInBank(anIndex, x, y);
			g->DrawImage(Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
		}
	}

	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		ChosenSeedState aSeedState = aChosenSeed.mSeedState;
		if (mApp->SeedTypeAvailable(aSeedType) && aSeedState != SEED_FLYING_TO_BANK && aSeedState != SEED_FLYING_TO_CHOOSER &&
			aSeedState != SEED_PACKET_HIDDEN && (aSeedState == SEED_IN_CHOOSER || mBoard->mCutScene->mSeedChoosing))
		{
			bool aGrayed = false;
			if (((SeedNotRecommendedToPick(aSeedType) || SeedNotAllowedToPick(aSeedType)) && aSeedState == SEED_IN_CHOOSER) ||
				SeedNotAllowedDuringTrial(aSeedType))
				aGrayed = true;

			int aPosX = aChosenSeed.mX;
			int aPosY = aChosenSeed.mY;

			if (aSeedState == SEED_IN_CHOOSER)
			{
				GetSeedPositionInChooser((int)aSeedType, aPosX, aPosY);
			}
			else if (aSeedState == SEED_IN_BANK)
			{
				aPosX -= mX - BOARD_ADDITIONAL_WIDTH;
				aPosY -= mY - BOARD_OFFSET_Y;
			}

			if (aSeedState != SEED_IN_BANK){
				g->SetClipRect(cSeedClipRect);
			}
			
			DrawSeedPacket(g, aPosX, aPosY, aChosenSeed.mSeedType, aChosenSeed.mImitaterType, 0, aGrayed ? 115 : 255, true, false);

			if (aSeedState != SEED_IN_BANK){
				g->ClearClipRect();
			}
		}
	}

	mImitaterButton->Draw(g);
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		ChosenSeedState aSeedState = aChosenSeed.mSeedState;
		if (mApp->SeedTypeAvailable(aSeedType) && (aSeedState == SEED_FLYING_TO_BANK || aSeedState == SEED_FLYING_TO_CHOOSER))
		{
			DrawSeedPacket(g, aChosenSeed.mX, aChosenSeed.mY, aChosenSeed.mSeedType, aChosenSeed.mImitaterType, 0, 255, true, false);
		}
	}

	mStartButton->Draw(g);
	mRandomButton->Draw(g);
	mViewLawnButton->Draw(g);
	mLastPickedButton->Draw(g);
	mAlmanacButton->Draw(g);
	mStoreButton->Draw(g);
	Graphics aBoardFrameG = Graphics(*g);
	aBoardFrameG.mTransX -= mX - BOARD_ADDITIONAL_WIDTH;
	aBoardFrameG.mTransY -= mY - BOARD_OFFSET_Y;
	mMenuButton->Draw(&aBoardFrameG);
	mToolTip->Draw(g);
}

void SeedChooserScreen::SliderVal(int theId, double theVal)
{
	switch (theId)
	{
	case 0:
		mScrollPosition = theVal * mMaxScrollPosition;
		break;
	}
}

//0x484C30
void SeedChooserScreen::UpdateViewLawn()
{
	if (mChooseState != CHOOSE_VIEW_LAWN) return;
	mViewLawnTime++;
	if (mViewLawnTime == 100) mBoard->DisplayAdviceAgain("[CLICK_TO_CONTINUE]", MESSAGE_STYLE_HINT_STAY, ADVICE_CLICK_TO_CONTINUE);
	else if (mViewLawnTime == 251) mViewLawnTime = 250;

	int aBoardX = BOARD_IMAGE_WIDTH_OFFSET - mApp->mWidth + BOARD_ADDITIONAL_WIDTH;
	int aSeedChooserY = SEED_CHOOSER_OFFSET_Y - Sexy::IMAGE_SEEDCHOOSER_BACKGROUND->GetHeight() - 204 + BOARD_OFFSET_Y;
	if (mViewLawnTime <= 100)
	{
		//mBoard->mRoofTreeOffset = TodAnimateCurve(0, 100, mViewLawnTime, WIDE_BOARD_WIDTH - BOARD_ADDITIONAL_WIDTH + 70, -BOARD_ADDITIONAL_WIDTH, TodCurves::CURVE_EASE_IN_OUT);;
		//mBoard->mRoofPoleOffset = CalcPosition(aTimePanRightStart, aTimePanRightEnd, WIDE_BOARD_WIDTH - BOARD_ADDITIONAL_WIDTH + 70, -BOARD_WIDTH);
		mBoard->Move(-TodAnimateCurve(0, 100, mViewLawnTime, aBoardX, -BOARD_ADDITIONAL_WIDTH, CURVE_EASE_IN_OUT), BOARD_OFFSET_Y);
		Move(BOARD_ADDITIONAL_WIDTH, TodAnimateCurve(0, 40, mViewLawnTime, aSeedChooserY, SEED_CHOOSER_OFFSET_Y, CURVE_EASE_IN_OUT));
	}
	else if (mViewLawnTime <= 250)
	{
		mBoard->Move(BOARD_ADDITIONAL_WIDTH, BOARD_OFFSET_Y);
		Move(BOARD_ADDITIONAL_WIDTH, SEED_CHOOSER_OFFSET_Y);
	}
	else if (mViewLawnTime <= 350)
	{
		mBoard->ClearAdvice(ADVICE_CLICK_TO_CONTINUE);
		mBoard->Move(-TodAnimateCurve(250, 350, mViewLawnTime, -BOARD_ADDITIONAL_WIDTH, aBoardX, CURVE_EASE_IN_OUT), BOARD_OFFSET_Y);
		Move(BOARD_ADDITIONAL_WIDTH, TodAnimateCurve(310, 350, mViewLawnTime, SEED_CHOOSER_OFFSET_Y, aSeedChooserY, CURVE_EASE_IN_OUT));
	}
	else
	{
		mChooseState = CHOOSE_NORMAL;
		mViewLawnTime = 0;
		mMenuButton->mDisabled = false;
	}
}

//0x484FD0
void SeedChooserScreen::LandFlyingSeed(ChosenSeed& theChosenSeed)
{
	if (theChosenSeed.mSeedState == SEED_FLYING_TO_BANK)
	{
		theChosenSeed.mX = theChosenSeed.mEndX;
		theChosenSeed.mY = theChosenSeed.mEndY;
		theChosenSeed.mTimeStartMotion = 0;
		theChosenSeed.mTimeEndMotion = 0;
		theChosenSeed.mSeedState = SEED_IN_BANK;
		mSeedsInFlight--;
	}
	else if (theChosenSeed.mSeedState == SEED_FLYING_TO_CHOOSER)
	{
		GetSeedPositionInChooser((int)theChosenSeed.mSeedState, theChosenSeed.mEndX, theChosenSeed.mEndY);
		theChosenSeed.mX = theChosenSeed.mEndX;
		theChosenSeed.mY = theChosenSeed.mEndY;
		theChosenSeed.mTimeStartMotion = 0;
		theChosenSeed.mTimeEndMotion = 0;
		theChosenSeed.mSeedState = SEED_IN_CHOOSER;
		mSeedsInFlight--;
		if (theChosenSeed.mSeedType == SEED_IMITATER)
		{
			theChosenSeed.mSeedState = SEED_PACKET_HIDDEN;
			theChosenSeed.mImitaterType = SEED_NONE;
			UpdateImitaterButton();
		}
	}
}

void SeedChooserScreen::MouseWheel(int theDelta)
{
	Widget::MouseWheel(theDelta);

	mScrollAmount -= theDelta;
	mScrollAmount -= mScrollAmount * 0.1f;
	RemoveToolTip();
}
//0x485040
void SeedChooserScreen::UpdateCursor()
{
	if (mApp->GetDialogCount() || mBoard->mCutScene->IsInShovelTutorial() || mApp->mGameMode == GAMEMODE_UPSELL) return;
	
	mApp->mWidgetManager->SetFocus(this);
	
	if (mSlider->mIsOver)
	{
		return;
	}

	SeedType aMouseSeedType = SeedHitTest(mLastMouseX, mLastMouseY);
	if (aMouseSeedType != SEED_NONE)
	{
		ChosenSeed& aMouseChosenSeed = mChosenSeeds[aMouseSeedType];
		if (aMouseChosenSeed.mSeedState == SEED_IN_BANK && aMouseChosenSeed.mCrazyDavePicked)
			aMouseSeedType = SEED_NONE;
	}

	if (mMouseVisible && mChooseState != CHOOSE_VIEW_LAWN && ((aMouseSeedType != SEED_NONE && !SeedNotAllowedToPick(aMouseSeedType)) ||
		mRandomButton->IsMouseOver() || mViewLawnButton->IsMouseOver() || mMenuButton->IsMouseOver()|| mAlmanacButton->IsMouseOver() || mImitaterButton->IsMouseOver() ||
		mStoreButton->IsMouseOver() || mStartButton->IsMouseOver() || mLastPickedButton->IsMouseOver()))
		mApp->SetCursor(CURSOR_HAND);
	else
		mApp->SetCursor(CURSOR_POINTER);
}

//0x4851A0
void SeedChooserScreen::Update()
{
	Widget::Update();

	float aScrollSpeed = 1 + abs(mScrollAmount) * 0.1f;
	mScrollPosition = ClampFloat(mScrollPosition += mScrollAmount * aScrollSpeed, 0, mMaxScrollPosition);
	mScrollAmount *= 0.9f;

	mSlider->SetValue(max(0.0, min(mMaxScrollPosition, mScrollPosition)) / mMaxScrollPosition);

	mLastMouseX = mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH;
	mLastMouseY = mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y;

	mSeedChooserAge++;
	mToolTip->Update();

	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		if (mApp->SeedTypeAvailable(aSeedType))
		{
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
			if (aChosenSeed.mSeedState == SEED_FLYING_TO_BANK || aChosenSeed.mSeedState == SEED_FLYING_TO_CHOOSER)
			{
				int aTimeStart = aChosenSeed.mTimeStartMotion;
				int aTimeEnd = aChosenSeed.mTimeEndMotion;
				if (aChosenSeed.mSeedState == SEED_FLYING_TO_CHOOSER)
				{
					GetSeedPositionInChooser((int) aSeedType, aChosenSeed.mEndX, aChosenSeed.mEndY);
				}
				aChosenSeed.mX = TodAnimateCurve(aTimeStart, aTimeEnd, mSeedChooserAge, aChosenSeed.mStartX, aChosenSeed.mEndX, CURVE_EASE_IN_OUT);
				aChosenSeed.mY = TodAnimateCurve(aTimeStart, aTimeEnd, mSeedChooserAge, aChosenSeed.mStartY, aChosenSeed.mEndY, CURVE_EASE_IN_OUT);
				if (mSeedChooserAge >= aChosenSeed.mTimeEndMotion)
				{
					LandFlyingSeed(aChosenSeed);
				}
			}
		}
	}

	ShowToolTip();
	mStartButton->Update();
	mRandomButton->Update();
	mViewLawnButton->Update();
	mAlmanacButton->Update();
	mImitaterButton->Update();
	mStoreButton->Update();
	mMenuButton->Update();
	mLastPickedButton->Update();
	UpdateViewLawn();
	UpdateCursor();
	MarkDirty();
}

void SeedChooserScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	AddWidget(mSlider);
}

//0x42F6B0
void SeedChooserScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	RemoveWidget(mSlider);
}

//0x485340
bool SeedChooserScreen::DisplayRepickWarningDialog(const SexyChar* theMessage)
{
	return mApp->LawnMessageBox(
		Dialogs::DIALOG_CHOOSER_WARNING,
		_S("[DIALOG_WARNING]"),
		theMessage,
		_S("[DIALOG_BUTTON_YES]"),
		_S("[REPICK_BUTTON]"),
		Dialog::BUTTONS_YES_NO
	) == Dialog::ID_YES;
}

//0x485370
bool SeedChooserScreen::FlyersAreComming()
{
	for (int aWave = 0; aWave < mBoard->mNumWaves; aWave++)
	{
		for (int anIndex = 0; anIndex < MAX_ZOMBIES_IN_WAVE; anIndex++)
		{
			ZombieType aZombieType = mBoard->mZombiesInWave[aWave][anIndex];
			if (aZombieType == ZOMBIE_INVALID)
				break;

			if (aZombieType == ZOMBIE_BALLOON)
				return true;
		}
	}
	return false;
}

//0x4853D0
bool SeedChooserScreen::FlyProtectionCurrentlyPlanted()
{
	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SEED_CATTAIL || aPlant->mSeedType == SEED_CACTUS)
		{
			return true;
		}
	}
	return false;
}

//0x485420
bool SeedChooserScreen::CheckSeedUpgrade(SeedType theSeedTypeTo, SeedType theSeedTypeFrom)
{
	if (mApp->IsSurvivalMode() || !PickedPlantType(theSeedTypeTo) || PickedPlantType(theSeedTypeFrom))
		return true;

	SexyString aWarning = TodStringTranslate(_S("[SEED_CHOOSER_UPGRADE_WARNING]"));
	aWarning = TodReplaceString(aWarning, _S("{UPGRADE_TO}"), Plant::GetNameString(theSeedTypeTo));
	aWarning = TodReplaceString(aWarning, _S("{UPGRADE_FROM}"), Plant::GetNameString(theSeedTypeFrom));
	return DisplayRepickWarningDialog(aWarning.c_str());
}

//0x485600
void SeedChooserScreen::OnStartButton()
{
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SEEING_STARS && !PickedPlantType(SEED_STARFRUIT))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_SEEING_STARS_WARNING]")))
		{
			return;
		}
	}

	if (mApp->IsFirstTimeAdventureMode() && mBoard->mLevel == 11 && !PickedPlantType(SEED_PUFFSHROOM))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_PUFFSHROOM_WARNING]")))
		{
			return;
		}
	}
	if (!PickedPlantType(SEED_SUNFLOWER) && !PickedPlantType(SEED_TWINSUNFLOWER) && !PickedPlantType(SEED_SUNSHROOM) && !PickedPlantType(SEED_SUNBLOOM) &&
		!PickedPlantType(SEED_PRIMAL_SUNFLOWER) &&
		!mBoard->mCutScene->IsSurvivalRepick() && !LawnApp::IsLastStandLevel(mApp->mGameMode))
	{
		if (mApp->IsFirstTimeAdventureMode() && mBoard->mLevel == 11)
		{
			if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_NIGHT_SUN_WARNING]")))
			{
				return;
			}
		}
		else if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_SUN_WARNING]")))
		{
			return;
		}
	}
	if (mBoard->StageHasPool() && !PickedPlantType(SEED_LILYPAD) && !PickedPlantType(SEED_SEASHROOM) && !PickedPlantType(SEED_TANGLEKELP) && !mBoard->mCutScene->IsSurvivalRepick())
	{
		if (mApp->IsFirstTimeAdventureMode() && mBoard->mLevel == 21)
		{
			if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_LILY_WARNING]")))
			{
				return;
			}
		}
		else if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_POOL_WARNING]")))
		{
			return;
		}
	}
	if ((mBoard->StageHasRoof() || mApp->mGameMode == GAMEMODE_CHALLENGE_FLOOR_IS_LAVA) && !PickedPlantType(SEED_FLOWERPOT) && mApp->SeedTypeAvailable(SEED_FLOWERPOT))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_ROOF_WARNING]")))
		{
			return;
		}
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_ART_SEEING_NUTS && !PickedPlantType(SEED_WALLNUT) && !PickedPlantType(SEED_GARLIC))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_ART_WALLNUT_WARNING]")))
		{
			return;
		}
	}
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SEEING_FLOWERS &&
		(!PickedPlantType(SEED_SUNFLOWER) || !PickedPlantType(SEED_TWINSUNFLOWER)))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_ART_2_WARNING]")))
		{
			return;
		}
	}

	if (FlyersAreComming() && !FlyProtectionCurrentlyPlanted() && !PickedPlantType(SEED_CATTAIL) && !PickedPlantType(SEED_CACTUS) && !PickedPlantType(SEED_BLOVER) && !PickedPlantType(SEED_HURRIKALE))
	{
		if (!DisplayRepickWarningDialog(_S("[SEED_CHOOSER_FLYER_WARNING]")))
		{
			return;
		}
	}

	if (!CheckSeedUpgrade(SEED_GATLINGPEA, SEED_REPEATER) ||
		!CheckSeedUpgrade(SEED_WINTERMELON, SEED_MELONPULT) ||
		!CheckSeedUpgrade(SEED_TWINSUNFLOWER, SEED_SUNFLOWER) ||
		!CheckSeedUpgrade(SEED_SPIKEROCK, SEED_SPIKEWEED) ||
		!CheckSeedUpgrade(SEED_COBCANNON, SEED_KERNELPULT) ||
		!CheckSeedUpgrade(SEED_GOLD_MAGNET, SEED_MAGNETSHROOM) ||
		!CheckSeedUpgrade(SEED_GLOOMSHROOM, SEED_FUMESHROOM) ||
		!CheckSeedUpgrade(SEED_CATTAIL, SEED_LILYPAD))
		return;

	CloseSeedChooser();
}

//0x4859B0
void SeedChooserScreen::PickRandomSeeds()
{
	for (int anIndex = mSeedsInBank; anIndex < mBoard->mSeedBank->mNumPackets; anIndex++)
	{
		SeedType aSeedType;
		do aSeedType = (SeedType)Rand(mApp->GetSeedsAvailable());
		while (!mApp->SeedTypeAvailable(aSeedType) || aSeedType == SEED_IMITATER || mChosenSeeds[aSeedType].mSeedState != SEED_IN_CHOOSER);
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		aChosenSeed.mTimeStartMotion = 0;
		aChosenSeed.mTimeEndMotion = 0;
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		GetSeedPositionInBank(anIndex, aChosenSeed.mEndX, aChosenSeed.mEndY);
		aChosenSeed.mSeedState = SEED_IN_BANK;
		aChosenSeed.mSeedIndexInBank = anIndex;
		mSeedsInBank++;
	}
	for (SeedType aSeedFlying = SEED_PEASHOOTER; aSeedFlying < NUM_SEEDS_IN_CHOOSER; aSeedFlying = (SeedType)(aSeedFlying + 1))
		LandFlyingSeed(mChosenSeeds[aSeedFlying]);
	CloseSeedChooser();
}

void SeedChooserScreen::PickCPUSeeds()
{
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		if (aChosenSeed.mSeedState == SEED_IN_CHOOSER || aChosenSeed.mSeedState == SEED_PACKET_HIDDEN || aChosenSeed.mCrazyDavePicked)
		{
			continue;
		}

		aChosenSeed.mTimeStartMotion = mSeedChooserAge;
		aChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		GetSeedPositionInChooser(aChosenSeed.mSeedType, aChosenSeed.mEndX, aChosenSeed.mEndY);
		aChosenSeed.mSeedIndexInBank = 0;
		if (aChosenSeed.mSeedState != SEED_FLYING_TO_CHOOSER && aChosenSeed.mSeedState != SEED_FLYING_TO_BANK)
		{
			mSeedsInFlight++;
		}
		aChosenSeed.mSeedState = SEED_FLYING_TO_CHOOSER;
		mSeedsInBank--;
	}

	vector<SeedType> mPickedSeeds;
	MTRand aLevelRNG = MTRand(mBoard->GetLevelRandSeed());
	TodWeightedArray aSeedArray[NUM_SEEDS_IN_CHOOSER];

	bool mHasCoffeeBean = Rand(3) == 0 && !mBoard->StageIsNight() && mApp->SeedTypeAvailable(SEED_INSTANTCOFFEE);

	// Producer
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{

		aSeedArray[aSeedType].mItem = aSeedType;

		bool isNocturnal = Plant::IsNocturnal(aSeedType);
		int score = 0;

		if (Plant::MakesSun(aSeedType) && !Plant::IsUpgrade(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end()
			&& mApp->SeedTypeAvailable(aSeedType))
		{
			if (isNocturnal && mBoard->StageIsNight())
				score = 10;
			else
				score = 1;

		}


		aSeedArray[aSeedType].mWeight = score;
	}

	mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));

	// Shooter
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		aSeedArray[aSeedType].mItem = aSeedType;
		aSeedArray[aSeedType].mWeight = 0;
		if (!Plant::IsUpgrade(aSeedType) && Plant::IsShooter(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end()
			&& mApp->SeedTypeAvailable(aSeedType))
		{
			int score = 2;

			if (!mHasCoffeeBean && Plant::IsNocturnal(aSeedType) || SeedNotRecommendedToPick(aSeedType))
				score = 0;
			else if ((mHasCoffeeBean || mBoard->StageIsNight()) && Plant::IsNocturnal(aSeedType))
				score += 3;
			if (mBoard->StageHasHighGrounds() || (mBoard->StageHasRoof() && Plant::IsRoofShooter(aSeedType)))
				score += 5;

			if (mBoard->mZombieAllowed[ZOMBIE_BALLOON] && aSeedType == SEED_CACTUS)
				score += 1;
			else if (!mBoard->mZombieAllowed[ZOMBIE_BALLOON] && aSeedType == SEED_CACTUS)
				score -= 1;

			aSeedArray[aSeedType].mWeight = score;
		}
	}

	mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));

	// Instas
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		aSeedArray[aSeedType].mItem = aSeedType;
		aSeedArray[aSeedType].mWeight = 0;
		if (!Plant::IsUpgrade(aSeedType) && Plant::IsInsta(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end()
			&& mApp->SeedTypeAvailable(aSeedType))
		{
			int score = 1;

			if (!mHasCoffeeBean && Plant::IsNocturnal(aSeedType))
				score = 0;
			else if ((mHasCoffeeBean || mBoard->StageIsNight()) && Plant::IsNocturnal(aSeedType))
				score += 2;

			aSeedArray[aSeedType].mWeight = score;
		}
	}

	mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));

	// Tanks
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		aSeedArray[aSeedType].mItem = aSeedType;
		aSeedArray[aSeedType].mWeight = 0;
		if (!Plant::IsUpgrade(aSeedType) && Plant::IsTank(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end()
			&& mApp->SeedTypeAvailable(aSeedType))
		{
			int score = 1;

			if (!mHasCoffeeBean && Plant::IsNocturnal(aSeedType))
				score = 0;
			else if ((mHasCoffeeBean || mBoard->StageIsNight()) && Plant::IsNocturnal(aSeedType))
				score += 2;

			if (mBoard->StageHasRoof() && aSeedType == SEED_TORCHWOOD)
				score -= 1;

			aSeedArray[aSeedType].mWeight = score;
		}
	}

	mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));

	// Special Seeds
	if (mBoard->StageHasGraveStones() && mApp->SeedTypeAvailable(SEED_GRAVEBUSTER))
	{
		mPickedSeeds.push_back(SEED_GRAVEBUSTER);
	}

	if (mBoard->StageHasPool() && mApp->SeedTypeAvailable(SEED_LILYPAD))
	{
		mPickedSeeds.push_back(SEED_LILYPAD);
	}

	if (mBoard->StageHasRoof() && mApp->SeedTypeAvailable(SEED_FLOWERPOT))
	{
		mPickedSeeds.push_back(SEED_FLOWERPOT);
	}

	if (find(mPickedSeeds.begin(), mPickedSeeds.end(), SEED_CACTUS) == mPickedSeeds.end() &&
		find(mPickedSeeds.begin(), mPickedSeeds.end(), SEED_STINGER) == mPickedSeeds.end() &&
		mBoard->mZombieAllowed[ZOMBIE_BALLOON] && mApp->SeedTypeAvailable(SEED_BLOVER))
	{
		mPickedSeeds.push_back(SEED_BLOVER);
	}

	if ((mBoard->StageHasFog() || mBoard->mZombieAllowed[ZOMBIE_WRAITH]) && mApp->SeedTypeAvailable(SEED_PLANTERN))
	{
		mPickedSeeds.push_back(SEED_PLANTERN);
	}

	if ((mBoard->mZombieAllowed[ZOMBIE_BUNGEE] || mBoard->mZombieAllowed[ZOMBIE_CATAPULT]) && mApp->SeedTypeAvailable(SEED_UMBRELLA))
	{
		mPickedSeeds.push_back(SEED_UMBRELLA);
	}

	// Add Coffee Bean if needed
	if (mHasCoffeeBean)
	{
		for (int i = 0; i < mPickedSeeds.size(); i++)
		{
			mHasCoffeeBean = Plant::IsNocturnal(mPickedSeeds[i]);
			if (mHasCoffeeBean)
			{
				break;
			}
		}
	}

	if (mHasCoffeeBean )
	{
		mPickedSeeds.push_back(SEED_INSTANTCOFFEE);
	}

	/*int mode = Rand(2);

	if (mode == 0 && mPickedSeeds.size() < mBoard->mSeedBank->mNumPackets)
	{
		for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
		{
			aSeedArray[aSeedType].mItem = (int)aSeedType;

			bool isNocturnal = Plant::IsNocturnal(aSeedType);
			int score = 0;

			if (Plant::MakesSun(aSeedType) && !Plant::IsUpgrade(aSeedType) &&
				(isNocturnal && (mHasCoffeeBean || mBoard->StageIsNight()) || !isNocturnal) &&
				find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end())
			{
				score = isNocturnal && !mBoard->StageIsNight() ? 2 : 10;
			}

			if (Plant::MakesSun(aSeedType) && isNocturnal && mBoard->StageIsNight())
				score += 3;

			if (mBoard->StageIsNight() && !isNocturnal)
				score -= 1;

			aSeedArray[aSeedType].mWeight = score;
		}
		mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));
	}
	else if (mode == 1 && mPickedSeeds.size() < mBoard->mSeedBank->mNumPackets)
	{
		for (int i = 0; i < 3; i++) 
		{
			if (mPickedSeeds.size() + i < mBoard->mSeedBank->mNumPackets)
				break;

			for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
			{
				aSeedArray[aSeedType].mItem = aSeedType;
				aSeedArray[aSeedType].mWeight = 0;
				if (!Plant::IsUpgrade(aSeedType) && Plant::IsShooter(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end())
				{
					int score = 2;

					if (!mHasCoffeeBean && Plant::IsNocturnal(aSeedType) || SeedNotRecommendedToPick(aSeedType))
						score = 0;
					else if ((mHasCoffeeBean || mBoard->StageIsNight()) && Plant::IsNocturnal(aSeedType))
						score += 3;
					if (mBoard->StageHasHighGrounds() || (mBoard->StageHasRoof() && Plant::IsRoofShooter(aSeedType)))
						score += 5;

					if (mBoard->mZombieAllowed[ZOMBIE_BALLOON] && aSeedType == SEED_CACTUS)
						score += 1;
					else if (!mBoard->mZombieAllowed[ZOMBIE_BALLOON] && aSeedType == SEED_CACTUS)
						score -= 1;

					aSeedArray[aSeedType].mWeight = score;
				}
			}
			mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));
		}
	}*/

	int numSeeds = mPickedSeeds.size();
	while (numSeeds < mBoard->mSeedBank->mNumPackets)
	{
		numSeeds++;
		for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
		{
			aSeedArray[aSeedType].mItem = aSeedType;
			aSeedArray[aSeedType].mWeight = 0;
			if (!Plant::IsUpgrade(aSeedType) && find(mPickedSeeds.begin(), mPickedSeeds.end(), aSeedType) == mPickedSeeds.end()
				&& aSeedType != SEED_IMITATER)
			{
				int score = 1;

				if (!mHasCoffeeBean && Plant::IsNocturnal(aSeedType) || SeedNotRecommendedToPick(aSeedType))
				{
					score = 0;
				}
				else if ((mHasCoffeeBean || mBoard->StageIsNight()) && Plant::IsNocturnal(aSeedType))
				{
					score += 1;
				}
				if (mBoard->StageHasHighGrounds() || (mBoard->StageHasRoof() && Plant::IsRoofShooter(aSeedType)))
				{
					score += 1;
				}

				aSeedArray[aSeedType].mWeight = score;
			}
		}
		mPickedSeeds.push_back((SeedType)TodPickFromWeightedArray(aSeedArray, NUM_SEEDS_IN_CHOOSER));
	}

	
	// Set Chosen Seeds
	for (SeedType seedType : mPickedSeeds)
	{
		numSeeds++;
		ChosenSeed& aChosenSeed = mChosenSeeds[seedType];
		aChosenSeed.mTimeStartMotion = mSeedChooserAge;
		aChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
		GetSeedPositionInChooser((int)aChosenSeed.mSeedType, aChosenSeed.mX, aChosenSeed.mY);
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mY += mScrollPosition;
		}
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mStartY -= mScrollPosition;
		}
		GetSeedPositionInBank(mSeedsInBank, aChosenSeed.mEndX, aChosenSeed.mEndY);
		aChosenSeed.mSeedIndexInBank = mSeedsInBank;
		if (aChosenSeed.mSeedState != SEED_FLYING_TO_BANK && aChosenSeed.mSeedState != SEED_FLYING_TO_CHOOSER)
		{
			mSeedsInFlight++;
		}
		aChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
		mSeedsInBank++;
	}

	/*for (int anIndex = 0; anIndex < mSeedBankMap.size(); anIndex++)
	{
		ChosenSeed& aChosenSeed = mSeedBankMap[anIndex];

		aChosenSeed.mTimeStartMotion = mSeedChooserAge;
		aChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
		GetSeedPositionInChooser((int)aChosenSeed.mSeedType, aChosenSeed.mX, aChosenSeed.mY);
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mY += mScrollPosition;
		}
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mStartY -= mScrollPosition;
		}
		GetSeedPositionInBank(mSeedsInBank, aChosenSeed.mEndX, aChosenSeed.mEndY);
		aChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
		aChosenSeed.mSeedIndexInBank = mSeedsInBank;
		mSeedsInFlight++;
		mSeedsInBank++;

		if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
			EnableStartButton(true);
	}*/


	/*for (int anIndex = mSeedsInBank; anIndex < mBoard->mSeedBank->mNumPackets; anIndex++)
	{
		SeedType aSeedType;
		do aSeedType = (SeedType)Rand(mApp->GetSeedsAvailable());
		while (!mApp->SeedTypeAvailable(aSeedType) || aSeedType == SEED_IMITATER || mChosenSeeds[aSeedType].mSeedState != SEED_IN_CHOOSER);
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		aChosenSeed.mTimeStartMotion = mSeedChooserAge;
		aChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
		GetSeedPositionInChooser((int)aChosenSeed.mSeedType, aChosenSeed.mX, aChosenSeed.mY);
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mY += mScrollPosition;
		}
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		if (aChosenSeed.mSeedType != SEED_IMITATER)
		{
			aChosenSeed.mStartY -= mScrollPosition;
		}
		GetSeedPositionInBank(mSeedsInBank, aChosenSeed.mEndX, aChosenSeed.mEndY);
		aChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
		aChosenSeed.mSeedIndexInBank = mSeedsInBank;
		mSeedsInFlight++;
		mSeedsInBank++;

		if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
			EnableStartButton(true);
	}*/

	mApp->PlaySample(Sexy::SOUND_TAP);
	/*for (SeedType aSeedFlying = SEED_PEASHOOTER; aSeedFlying < NUM_SEEDS_IN_CHOOSER; aSeedFlying = (SeedType)(aSeedFlying + 1))
	{
		LandFlyingSeed(mChosenSeeds[aSeedFlying]);
	}
	CloseSeedChooser();*/

	if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
		EnableStartButton(true);
}
//0x485BB0
void SeedChooserScreen::ButtonDepress(int theId)
{
	if (mSeedsInFlight > 0 || mChooseState == CHOOSE_VIEW_LAWN || !mMouseVisible)
		return;

	if (theId == SeedChooserScreen::SeedChooserScreen_ViewLawn)
	{
		mChooseState = CHOOSE_VIEW_LAWN;
		mMenuButton->mDisabled = true;
		mViewLawnTime = 0;
	}
	else if (theId == SeedChooserScreen::SeedChooserScreen_Almanac)
	{
		mApp->DoAlmanacDialog()->WaitForResult(true);
		if (mApp->mGameMode != GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
			mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_CHOOSE_YOUR_SEEDS);
	}
	else if (theId == SeedChooserScreen::SeedChooserScreen_Store)
	{
		StoreScreen* aStore = mApp->ShowStoreScreen();
		aStore->mBackButton->SetLabel("[STORE_BACK_TO_GAME]");
		aStore->WaitForResult();
		if (aStore->mGoToTreeNow)
		{
			mApp->KillBoard();
			mApp->PreNewGame(GAMEMODE_TREE_OF_WISDOM, false);
		}
		else 
		{
			if (mApp->mGameMode != GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
				mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_CHOOSE_YOUR_SEEDS);
		}
	}
	else if (theId == SeedChooserScreen::SeedChooserScreen_Menu)
	{
		mMenuButton->mIsOver = false;
		mMenuButton->mIsDown = false;
		UpdateCursor();
		mApp->DoNewOptions(false);

	}
	else if (mApp->GetSeedsAvailable() >= mBoard->mSeedBank->mNumPackets)
	{
		if (theId == SeedChooserScreen::SeedChooserScreen_Start)
		{
			OnStartButton();
		}
		else if (theId == SeedChooserScreen::SeedChooserScreen_Random)
		{
			PickCPUSeeds();
		}
	}

	//else if (theId == SeedChooserScreen::SeedChooserScreen_Repick) {
		/*for (int i = 0; i < SEEDBANK_MAX; i++) {
			if (!mApp->mPlayerInfo->mLastSeedsInBank[i])	continue;

			for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1)) {
				ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
				if (aChosenSeed.mSeedState == SEED_IN_BANK && aChosenSeed.mSeedType != (SeedType)mApp->mPlayerInfo->mLastSeedsInBank[i]) {
					aChosenSeed.mTimeStartMotion = mSeedChooserAge;
					aChosenSeed.mTimeEndMotion = mSeedChooserAge + 15;
					aChosenSeed.mStartX = aChosenSeed.mX;
					aChosenSeed.mStartY = aChosenSeed.mY;
					GetSeedPositionInChooser(aSeedType, aChosenSeed.mEndX, aChosenSeed.mEndY);
					aChosenSeed.mSeedState = SEED_FLYING_TO_CHOOSER;
					aChosenSeed.mSeedIndexInBank = 0;
					mSeedsInFlight++;
					mSeedsInBank--;
				}
			}
		}
		*/
		/*int addCount = 0;
		for (int i = 0; i < SEEDBANK_MAX; i++) {
			if (!mApp->mPlayerInfo->mLastSeedsInBank[i])	continue;

			SeedType aSeedType = (SeedType)mApp->mPlayerInfo->mLastSeedsInBank[i];
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];

			if (mChosenSeeds->mCrazyDavePicked || aChosenSeed.mSeedState == SEED_IN_BANK)	continue;

			aChosenSeed.mTimeStartMotion = mSeedChooserAge;
			aChosenSeed.mTimeEndMotion = mSeedChooserAge + 15;
			aChosenSeed.mStartX = aChosenSeed.mX;
			aChosenSeed.mStartY = aChosenSeed.mY;
			GetSeedPositionInBank(i, aChosenSeed.mEndX, aChosenSeed.mEndY);
			aChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
			aChosenSeed.mSeedIndexInBank = i;
			mSeedsInFlight++;
			addCount++;
		}
		mSeedsInBank += addCount;

		if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
			EnableStartButton(true);*/

			//}
}

//0x485D80
SeedType SeedChooserScreen::SeedHitTest(int x, int y)
{
	if (mMouseVisible)
	{
		for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
		{
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
			if (!mApp->SeedTypeAvailable(aSeedType) || aChosenSeed.mSeedState == SEED_PACKET_HIDDEN) continue;

			int seedX = aChosenSeed.mX;
			int seedY = aChosenSeed.mY;

			if (aChosenSeed.mSeedState == SEED_IN_CHOOSER)
			{
				GetSeedPositionInChooser((int)aChosenSeed.mSeedType, seedX, seedY);
			}

			Rect seedRect = Rect(seedX, seedY, SEED_PACKET_WIDTH, SEED_PACKET_HEIGHT);
			bool isValid = true;
			if (aChosenSeed.mSeedState != SEED_IN_BANK)	{
				isValid = y + 1 > cSeedClipRect.mY && y - 1 < cSeedClipRect.mY + cSeedClipRect.mHeight;
				
			}
			if (seedRect.Contains(x, y) && isValid) return aSeedType;
		}
	}
	return SEED_NONE;
}

//0x485E20
SeedType SeedChooserScreen::FindSeedInBank(int theIndexInBank)
{
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		if (mApp->SeedTypeAvailable(aSeedType))
		{
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
			if (aChosenSeed.mSeedState == SEED_IN_BANK && aChosenSeed.mSeedIndexInBank == theIndexInBank) return aSeedType;
		}
	}
	return SEED_NONE;
}

void SeedChooserScreen::EnableStartButton(bool theEnabled)
{
	mStartButton->SetDisabled(!theEnabled);
	if (theEnabled) mStartButton->mColors[GameButton::COLOR_LABEL] = Color::White;
	else mStartButton->mColors[GameButton::COLOR_LABEL] = Color(64, 64, 64);
}

//0x485E90
void SeedChooserScreen::ClickedSeedInBank(ChosenSeed& theChosenSeed)
{
	for (int anIndex = theChosenSeed.mSeedIndexInBank + 1; anIndex < mBoard->mSeedBank->mNumPackets; anIndex++)
	{
		SeedType aSeedType = FindSeedInBank(anIndex);
		if (aSeedType != SEED_NONE)
		{
			ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
			aChosenSeed.mTimeStartMotion = mSeedChooserAge;
			aChosenSeed.mTimeEndMotion = mSeedChooserAge + 15;
			aChosenSeed.mStartX = aChosenSeed.mX;
			aChosenSeed.mStartY = aChosenSeed.mY;
			GetSeedPositionInBank(anIndex - 1, aChosenSeed.mEndX, aChosenSeed.mEndY);
			aChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
			aChosenSeed.mSeedIndexInBank = anIndex - 1;
			mSeedsInFlight++;
		}
	}
	theChosenSeed.mTimeStartMotion = mSeedChooserAge;
	theChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
	theChosenSeed.mStartX = theChosenSeed.mX;
	theChosenSeed.mStartY = theChosenSeed.mY;
	GetSeedPositionInChooser(theChosenSeed.mSeedType, theChosenSeed.mEndX, theChosenSeed.mEndY);
	//if (theChosenSeed.mSeedType != SEED_IMITATER)
	//	theChosenSeed.mEndY -= mScrollPosition;
	theChosenSeed.mSeedState = SEED_FLYING_TO_CHOOSER;
	theChosenSeed.mSeedIndexInBank = 0;
	mSeedsInBank--;
	mSeedsInFlight++;
	RemoveToolTip();
	EnableStartButton(false);
	mApp->PlaySample(Sexy::SOUND_TAP);
}

//0x486030
void SeedChooserScreen::ClickedSeedInChooser(ChosenSeed& theChosenSeed)
{
	if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
		return;

	theChosenSeed.mTimeStartMotion = mSeedChooserAge;
	theChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;
	GetSeedPositionInChooser((int)theChosenSeed.mSeedType, theChosenSeed.mX, theChosenSeed.mY);
	if (theChosenSeed.mSeedType != SEED_IMITATER)
		theChosenSeed.mY += mScrollPosition;
	theChosenSeed.mStartX = theChosenSeed.mX;
	theChosenSeed.mStartY = theChosenSeed.mY;
	if (theChosenSeed.mSeedType != SEED_IMITATER)
		theChosenSeed.mStartY -= mScrollPosition;
	GetSeedPositionInBank(mSeedsInBank, theChosenSeed.mEndX, theChosenSeed.mEndY);
	theChosenSeed.mSeedState = SEED_FLYING_TO_BANK;
	theChosenSeed.mSeedIndexInBank = mSeedsInBank;
	mSeedsInFlight++;
	mSeedsInBank++;

	RemoveToolTip();
	mApp->PlaySample(Sexy::SOUND_TAP);
	if (mSeedsInBank == mBoard->mSeedBank->mNumPackets)
		EnableStartButton(true);
}

//0x486150
void SeedChooserScreen::ShowToolTip()
{
	if (!mApp->mWidgetManager->mMouseIn || !mApp->mActive || mApp->GetDialogCount() > 0 || mChooseState == CHOOSE_VIEW_LAWN)
	{
		RemoveToolTip();
	}
	else if (mSeedsInFlight <= 0)
	{
		if (mImitaterButton->IsMouseOver() && mMouseVisible)
		{
			mToolTip->SetLabel(Plant::GetToolTip(SEED_IMITATER));
			mToolTip->SetTitle(Plant::GetNameString(SEED_IMITATER));
			mToolTip->SetWarningText(_S(""));
			mToolTip->mX = (SEED_PACKET_WIDTH - mToolTip->mWidth) / 2 + mImitaterButton->mX;
			mToolTip->mY = mImitaterButton->mY - mToolTip->mHeight;
			mToolTip->mVisible = true;
		}
		else
		{
			SeedType aSeedType = SeedHitTest(mLastMouseX, mLastMouseY);
			if (aSeedType == SEED_NONE)
			{
				RemoveToolTip();
			}
			else if (aSeedType != mToolTipSeed)
			{
				RemoveToolTip();
				ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
				uint aRecFlags = SeedNotRecommendedToPick(aSeedType);
				if (SeedNotAllowedToPick(aSeedType))
				{
					mToolTip->SetWarningText(_S("[NOT_ALLOWED_ON_THIS_LEVEL]"));
				}
				else if (SeedNotAllowedDuringTrial(aSeedType))
				{
					mToolTip->SetWarningText(_S("[FULL_VERSION_ONLY]"));
				}
				else if (aChosenSeed.mSeedState == SEED_IN_BANK && aChosenSeed.mCrazyDavePicked)
				{
					mToolTip->SetWarningText(_S("[CRAZY_DAVE_WANTS]"));
				}
				else if (aRecFlags != 0U)
				{
					if (TestBit(aRecFlags, NOT_RECOMMENDED_NOCTURNAL))
					{
						mToolTip->SetWarningText(_S("[NOCTURNAL_WARNING]"));
					}
					else
					{
						mToolTip->SetWarningText(_S("[NOT_RECOMMENDED_FOR_LEVEL]"));
					}
				}
				else
				{
					mToolTip->SetWarningText(_S(""));
				}

				if (aSeedType == SEED_IMITATER)
				{
					mToolTip->SetTitle(Plant::GetNameString(aSeedType, aChosenSeed.mImitaterType));
					mToolTip->SetLabel(Plant::GetToolTip(aChosenSeed.mImitaterType));
				}
				else
				{
					mToolTip->SetTitle(Plant::GetNameString(aSeedType, SEED_NONE));
					mToolTip->SetLabel(Plant::GetToolTip(aSeedType));
				}

				int aSeedX, aSeedY;
				if (aChosenSeed.mSeedState == SEED_IN_BANK)
				{
					GetSeedPositionInBank(aChosenSeed.mSeedIndexInBank, aSeedX, aSeedY);
				}
				else
				{
					GetSeedPositionInChooser(aSeedType, aSeedX, aSeedY);
					//aSeedY -= mScrollPosition;
				}

				mToolTip->mX = (int) ((SEED_PACKET_WIDTH - mToolTip->mWidth) / 2.0f + aSeedX);
				mToolTip->mY = aSeedY + 70 ;
				mToolTip->mVisible = true;
				mToolTipSeed = aSeedType;
			}
		}
	}
}

void SeedChooserScreen::RemoveToolTip()
{
	mToolTip->mVisible = false;
	mToolTipSeed = SEED_NONE;
}

//0x486600
void SeedChooserScreen::CancelLawnView()
{
	if (mChooseState == CHOOSE_VIEW_LAWN && mViewLawnTime > 100 && mViewLawnTime <= 250) mViewLawnTime = 251;
}

//0x486630
void SeedChooserScreen::MouseUp(int x, int y, int theClickCount)
{
	if (theClickCount == 1)
	{
		if (mMenuButton->IsMouseOver()) ButtonDepress(SeedChooserScreen::SeedChooserScreen_Menu);
		else if (mStartButton->IsMouseOver()) ButtonDepress(SeedChooserScreen::SeedChooserScreen_Start);
		else if (mAlmanacButton->IsMouseOver()) ButtonDepress(SeedChooserScreen::SeedChooserScreen_Almanac);
		else if (mStoreButton->IsMouseOver()) ButtonDepress(SeedChooserScreen::SeedChooserScreen_Store);
	}
}

//0x4866E0
void SeedChooserScreen::UpdateImitaterButton()
{
	if (!mApp->SeedTypeAvailable(SEED_IMITATER))
	{
		mImitaterButton->mBtnNoDraw = true;
		mImitaterButton->mDisabled = true;
	}
	else
	{
		mImitaterButton->mBtnNoDraw = false;
		mImitaterButton->mDisabled = mChosenSeeds[SEED_IMITATER].mSeedState != SEED_PACKET_HIDDEN;
	}
}

//0x486770
void SeedChooserScreen::MouseDown(int x, int y, int theClickCount)
{
	Widget::MouseDown(x, y, theClickCount);

	if (mSeedsInFlight > 0)
	{
		for (int i = 0; i < NUM_SEEDS_IN_CHOOSER; i++)
		{
			LandFlyingSeed(mChosenSeeds[i]);
		}
	}

	if (mChooseState == CHOOSE_VIEW_LAWN)
	{
		CancelLawnView();
	}
	else if (mRandomButton->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
		ButtonDepress(SeedChooserScreen::SeedChooserScreen_Random);
	}
	else if (mViewLawnButton->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
		ButtonDepress(SeedChooserScreen::SeedChooserScreen_ViewLawn);
	}
	else if (mMenuButton->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_GRAVEBUTTON);
	}
	else if (mStartButton->IsMouseOver() || mAlmanacButton->IsMouseOver() || mStoreButton->IsMouseOver())
	{
		mApp->PlaySample(Sexy::SOUND_TAP);
	}
	else if (mImitaterButton->IsMouseOver())
	{
		if (mSeedsInBank != mBoard->mSeedBank->mNumPackets)
		{
			mApp->PlaySample(Sexy::SOUND_TAP);
			ImitaterDialog* aDialog = new ImitaterDialog();
			mApp->AddDialog(aDialog->mId, aDialog);
			aDialog->Resize((mWidth - aDialog->mWidth) / 2, (mHeight - aDialog->mHeight) / 2, aDialog->mWidth, aDialog->mHeight);
			mApp->mWidgetManager->SetFocus(aDialog);
		}
	}
	else
	{
		if (!mBoard->mSeedBank->ContainsPoint(x, y) && !mAlmanacButton->IsMouseOver() && !mStoreButton->IsMouseOver() && mApp->CanShowAlmanac())
		{
			Zombie* aZombie = mBoard->ZombieHitTest(x - mBoard->mX + BOARD_ADDITIONAL_WIDTH, y - mBoard->mY + BOARD_OFFSET_Y);
			if (aZombie && aZombie->mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE && aZombie->mZombieType != ZOMBIE_REDEYE_GARGANTUAR)
			{
				mApp->DoAlmanacDialog(SEED_NONE, aZombie->mZombieType)->WaitForResult(true);
				if (mApp->mGameMode != GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
					mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_CHOOSE_YOUR_SEEDS);
				return;
			}
		}

		SeedType aSeedType = SeedHitTest(x, y);
		if (aSeedType != SEED_NONE && !SeedNotAllowedToPick(aSeedType))
		{
			if (SeedNotAllowedDuringTrial(aSeedType))
			{
				mApp->PlaySample(Sexy::SOUND_TAP);
				if (mApp->LawnMessageBox(
					DIALOG_MESSAGE,
					_S("[GET_FULL_VERSION_TITLE]"),
					_S("[GET_FULL_VERSION_BODY]"),
					_S("[GET_FULL_VERSION_YES_BUTTON]"),
					_S("[GET_FULL_VERSION_NO_BUTTON]"),
					Dialog::BUTTONS_YES_NO
				) == Dialog::ID_YES)
				{
					if (mApp->mDRM)
					{
						mApp->mDRM->BuyGame();
					}
					mApp->DoBackToMain();
				}
			}
			else
			{
				ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
				if (aChosenSeed.mSeedState == SEED_IN_BANK)
				{
					if (aChosenSeed.mCrazyDavePicked)
					{
						mApp->PlaySample(Sexy::SOUND_BUZZER);
						mToolTip->FlashWarning();
					}
					else ClickedSeedInBank(aChosenSeed);
				}
				else if (aChosenSeed.mSeedState == SEED_IN_CHOOSER)
					ClickedSeedInChooser(aChosenSeed);
			}
		}
	}
}

//0x486CE0
bool SeedChooserScreen::PickedPlantType(SeedType theSeedType)
{
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		if (aChosenSeed.mSeedState == SEED_IN_BANK)
		{
			if (aChosenSeed.mSeedType == theSeedType || (aChosenSeed.mSeedType == SEED_IMITATER && aChosenSeed.mImitaterType == theSeedType))
			{
				return true;
			}
		}
	}
	return false;
}

//0x486D20
void SeedChooserScreen::CloseSeedChooser()
{
	DBG_ASSERT(mBoard->mSeedBank->mNumPackets == mBoard->GetNumSeedsInBank());
	for (int anIndex = 0; anIndex < mBoard->mSeedBank->mNumPackets; anIndex++)
	{
		SeedType aSeedType = FindSeedInBank(anIndex);
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		SeedPacket& aSeedPacket = mBoard->mSeedBank->mSeedPackets[anIndex];
		aSeedPacket.SetPacketType(aSeedType, aChosenSeed.mImitaterType);
		if (aChosenSeed.mRefreshing)
		{
			aSeedPacket.mRefreshCounter = aChosenSeed.mRefreshCounter;
			aSeedPacket.mRefreshTime = Plant::GetRefreshTime(aSeedPacket.mPacketType, aSeedPacket.mImitaterType);
			aSeedPacket.mRefreshing = true;
			aSeedPacket.mActive = false;
		}

		mApp->mPlayerInfo->mLastSeedsInBank[anIndex] = (int)aSeedType;
	}
	mBoard->mCutScene->EndSeedChooser();
}

//0x486E80
void SeedChooserScreen::KeyDown(KeyCode theKey)
{
	mBoard->DoTypingCheck(theKey);
}

//0x486EA0
void SeedChooserScreen::KeyChar(SexyChar theChar)
{
	if (mChooseState == CHOOSE_VIEW_LAWN && (theChar == ' ' || theChar == '\r' || theChar == '\u001B'))
	{
		CancelLawnView();
	}
	else if (mApp->mTodCheatKeys && theChar == '\u001B')
	{
		PickRandomSeeds();
	}
	else 
	{
		mBoard->KeyChar(theChar);
	}
}

//0x486F10
void SeedChooserScreen::UpdateAfterPurchase()
{
	for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1))
	{
		ChosenSeed& aChosenSeed = mChosenSeeds[aSeedType];
		if (aChosenSeed.mSeedState == SEED_IN_BANK)
			GetSeedPositionInBank(aChosenSeed.mSeedIndexInBank, aChosenSeed.mX, aChosenSeed.mY);
		else if (aChosenSeed.mSeedState == SEED_IN_CHOOSER)
			GetSeedPositionInChooser(aSeedType, aChosenSeed.mX, aChosenSeed.mY);
		else continue;
		aChosenSeed.mStartX = aChosenSeed.mX;
		aChosenSeed.mStartY = aChosenSeed.mY;
		aChosenSeed.mEndX = aChosenSeed.mX;
		aChosenSeed.mEndY = aChosenSeed.mY;
	}
	EnableStartButton(mSeedsInBank == mBoard->mSeedBank->mNumPackets);
	UpdateImitaterButton();
}
