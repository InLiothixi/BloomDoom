#include "Board.h"
#include "Cutscene.h"
#include "Challenge.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../Resources.h"
#include "MessageWidget.h"
#include "../SexyAppFramework/Font.h"
#include "../Sexy.TodLib/FilterEffect.h"
#include "../SexyAppFramework/SexyMatrix.h"

SeedPacket::SeedPacket()
{
	mSlotMachiningPosition = 0.0f;
	mPacketType = SeedType::SEED_NONE;
	mImitaterType = SeedType::SEED_NONE;
	mIndex = -1;
	mSlotMachiningNextSeed = SeedType::SEED_NONE;
	mWidth = 50;
	mHeight = 70;
	mRefreshCounter = 0;
	mRefreshTime = 0;
	mRefreshing = false;
	mActive = true;
	mOffsetX = 0;
	mSlotMachineCountDown = 0;
	mSlotMachiningNextSeed = SeedType::SEED_NONE;
	mTimesUsed = 0;
	misP2 = false;
	mID = 0;
}

//0x487070
void SeedPacket::PickNextSlotMachineSeed()
{
	int aPeasCount = mBoard->CountPlantByType(SeedType::SEED_PEASHOOTER);

	SeedType SLOT_SEED_TYPES[6] = {
		SeedType::SEED_SUNFLOWER,
		SeedType::SEED_PEASHOOTER,
		SeedType::SEED_SNOWPEA,
		SeedType::SEED_WALLNUT,
		SeedType::SEED_SLOT_MACHINE_SUN,
		SeedType::SEED_SLOT_MACHINE_DIAMOND
	};

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SLOT_MACHINE_2)
	{
		SLOT_SEED_TYPES[1] = SEED_CABBAGEPULT;
		SLOT_SEED_TYPES[2] = SEED_KERNELPULT;
	}

	int aSeedsCount = 0;
	TodWeightedArray aSeedWeightArray[(int)SeedType::NUM_SEED_TYPES];
	for (int i = 0; i < LENGTH(SLOT_SEED_TYPES); i++)
	{
		SeedType aSeedType = SLOT_SEED_TYPES[i];

		int aWeight = 100;
		if (aSeedType == SeedType::SEED_PEASHOOTER)
		{
			aWeight = TodAnimateCurve(0, 5, aPeasCount, 200, 100, TodCurves::CURVE_LINEAR);
		}
		else if (aSeedType == SeedType::SEED_SLOT_MACHINE_DIAMOND)
		{
			aWeight = 30;
		}

		if (mIndex == 2 && aSeedType != SeedType::SEED_SLOT_MACHINE_DIAMOND)
		{
			if (aSeedType == mBoard->mSeedBank->mSeedPackets[0].mSlotMachiningNextSeed || aSeedType == mBoard->mSeedBank->mSeedPackets[1].mSlotMachiningNextSeed)
			{
				aWeight += aWeight / 2;
			}
		}

		aSeedWeightArray[aSeedsCount].mItem = (int)aSeedType;
		aSeedWeightArray[aSeedsCount].mWeight = aWeight;
		aSeedsCount++;
	}

	mSlotMachiningNextSeed = (SeedType)TodPickFromWeightedArray(aSeedWeightArray, aSeedsCount);
}

void SeedPacket::SlotMachineStart()
{
	mSlotMachineCountDown = 300;
	mSlotMachiningPosition = 0.0f;
	PickNextSlotMachineSeed();
}

//0x487190
void SeedPacket::FlashIfReady()
{
	if (!CanPickUp() || mApp->mEasyPlantingCheat)
		return;

	if (!mBoard->HasConveyorBeltSeedBank())
	{
		int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_BOTTOM, 0, 2);
		SeedBank* theSeedBank = mBoard->mSeedBank;
		mApp->AddTodParticle(mX + theSeedBank->mX, mY + theSeedBank->mY, aRenderPosition, ParticleEffect::PARTICLE_SEED_PACKET_FLASH);
	}
	
	if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER)
	{
		mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER);
	}
	else if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER)
	{
		mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
	}
	else if (mBoard->mTutorialState == TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER)
	{
		mBoard->SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
	}
}

void SeedPacket::Activate()
{
	mActive = true;
}

void SeedPacket::Deactivate()
{
	mActive = false;
	mRefreshCounter = 0;
	mRefreshTime = 0;
	mRefreshing = false;
}

void SeedPacket::SetActivate(bool theActive)
{
	if (theActive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

//0x487250
void SeedPacket::Update()
{
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING || mPacketType == SeedType::SEED_NONE)
	{
		return;
	}

	if (mBoard->mMainCounter == 0)
	{
		FlashIfReady();
	}

	if (!mActive && mRefreshing)
	{
		mRefreshCounter++;
		if (mRefreshCounter > mRefreshTime)
		{
			mRefreshCounter = 0;
			mRefreshing = false;
			Activate();
			FlashIfReady();
		}
	}

	if (mSlotMachineCountDown > 0 && mID < 3)
	{
		mSlotMachineCountDown--;
		float aFlipsPerSecont = TodAnimateCurveFloat(SLOT_MACHINE_TIME, 0, mSlotMachineCountDown, 6.0f, 2.0f, TodCurves::CURVE_LINEAR);
		mSlotMachiningPosition += aFlipsPerSecont * 0.01f;

		if (mSlotMachiningPosition >= 1.0f)
		{
			mPacketType = mSlotMachiningNextSeed;
			if (mSlotMachineCountDown == 0)
			{
				Activate();
				mSlotMachiningPosition = 0.0f;
			}
			else
			{
				mSlotMachiningPosition -= 1.0f;
				PickNextSlotMachineSeed();
			}
		}
		else if (mSlotMachineCountDown == 0)
		{
			mSlotMachineCountDown = 1;
		}
	}
}

//0x487380
void SeedPacketDrawSeed(Graphics* g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float theOffsetX, float theOffsetY, float theScale)
{
	Image* aImage = IMAGE_PACKET_PLANTS;
	SeedType aSeedType = theSeedType;
	if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
	{
		aSeedType = theImitaterType;
		FilterEffect aFilterEffect = FilterEffect::FILTER_EFFECT_WASHED_OUT;
		if (theImitaterType == SeedType::SEED_HYPNOSHROOM || theImitaterType == SeedType::SEED_SQUASH || theImitaterType == SeedType::SEED_POTATOMINE ||
			theImitaterType == SeedType::SEED_GARLIC || theImitaterType == SeedType::SEED_LILYPAD)
			aFilterEffect = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;
		aImage = FilterEffectGetImage(aImage, aFilterEffect);
	}

	if (aSeedType == SeedType::SEED_POTATOMINE && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 0, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_CHOMPER && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 1, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_HYPNOSHROOM && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 2, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_TALLNUT && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 3, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_BLOVER && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 4, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_PUMPKINSHELL && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 5, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_TWINSUNFLOWER && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 6, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_COBCANNON && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 7, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_CABBAGEPULT && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 8, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_KERNELPULT && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 9, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_MELONPULT && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 10, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_WINTERMELON && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 11, 0, g->mScaleX, g->mScaleY);
	}
	else if (aSeedType == SeedType::SEED_SPIKEROCK && g->mScaleX <= 1.0f)
	{
		TodDrawImageCelScaledF(g, aImage, x, y, 12, 0, g->mScaleX, g->mScaleY);
	}
	else
	{
		Graphics aSeedG(*g);
		aSeedG.mScaleX = theScale * g->mScaleX;
		aSeedG.mScaleY = theScale * g->mScaleY;

		Plant::DrawSeedType(&aSeedG, theSeedType, theImitaterType, DrawVariation::VARIATION_NORMAL, x + theOffsetX, y + theOffsetY);
	}
}

//0x4876F0
void DrawSeedPacket(Graphics* g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float thePercentDark, int theGrayness, bool theDrawCost, bool theUseCurrentCost, bool fromAward, float offX)
{
	if (theSeedType == SEED_NONE)
		return;

	SeedType aSeedType = theSeedType;
	if (aSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
	{
		aSeedType = theImitaterType;
	}

	if (theGrayness != 255)
	{
		g->SetColor(Color(theGrayness, theGrayness, theGrayness));
		g->SetColorizeImages(true);
	}
	else if (thePercentDark > 0)
	{
		g->SetColor(Color(128, 128, 128, 255));
		g->SetColorizeImages(true);
	}

	int aPacketBackground =
		theSeedType == SeedType::SEED_IMITATER ? 0 :
		Plant::IsUpgrade(aSeedType) ? 1 :
		theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER ? 3 :
		theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE ? 4 :
		theSeedType == SeedType::SEED_SLOT_MACHINE_SUN ? 5 :
		theSeedType == SeedType::SEED_SLOT_MACHINE_DIAMOND ? 6 :
		theSeedType == SeedType::SEED_ZOMBIQUARIUM_SNORKLE ? 7 :
		theSeedType == SeedType::SEED_ZOMBIQUARIUM_TROPHY ? 8 : 2;

	if (g->mScaleX > 1)
	{
		if (aPacketBackground == 0)
		{
			TodDrawImageCelScaledF(g, Sexy::IMAGE_SEEDPACKETIMITATER_LARGER, x, y, 0, 0, g->mScaleX * 0.5f, g->mScaleY * 0.5f);
		}
		else if (aPacketBackground == 1)
		{
			TodDrawImageCelScaledF(g, Sexy::IMAGE_SEEDPACKETUPGRADE_LARGER, x, y, 0, 0, g->mScaleX * 0.5f, g->mScaleY * 0.5f);
		}
		else
		{
			TodDrawImageCelScaledF(g, Sexy::IMAGE_SEEDPACKET_LARGER, x, y, 0, 0, g->mScaleX * 0.5f, g->mScaleY * 0.5f);
		}
	}
	else
	{
		TodDrawImageCelScaledF(g, Sexy::IMAGE_SEEDS, x, y, aPacketBackground, 0, g->mScaleX, g->mScaleY);
	}

	float aScale = 0.5f;
	bool aDrawSeedInMiddle = true;
	float aOffsetX = 5.0f;
	float aOffsetY = 8.0f;
	switch (aSeedType)
	{
	case SeedType::SEED_TALLNUT:
		aScale = 0.3f;
		aOffsetX = 12.0f;
		aOffsetY = 22.0f;
		break;

	case SeedType::SEED_INSTANTCOFFEE:
		aScale = 0.55f;
		aOffsetX = 0;
		aOffsetY = 9.0f;
		break;

	case SeedType::SEED_COBCANNON:
		aScale = 0.26f;
		aOffsetX = 6.0f;
		aOffsetY = 22.0f;
		break;

	case SeedType::SEED_CACTUS:
		aOffsetX = 9.0f;
		aOffsetY = 13.0f;
		break;

	case SeedType::SEED_POTATOMINE:
		aScale = 0.4f;
		aOffsetX = 8.0f;
		aOffsetY = 12.0f;
		break;

	case SeedType::SEED_MAGNETSHROOM:
		aOffsetY = 12.0f;
		break;

	case SeedType::SEED_ICYFUMESHROOM:
	case SeedType::SEED_FUMESHROOM:
	case SeedType::SEED_PUMPKINSHELL:
	case SeedType::SEED_CHOMPER:
	case SeedType::SEED_DOOMSHROOM:
	case SeedType::SEED_SQUASH:
	case SeedType::SEED_HYPNOSHROOM:
	case SeedType::SEED_SPIKEWEED:
	case SeedType::SEED_SPIKEROCK:
	case SeedType::SEED_PLANTERN:
	case SeedType::SEED_PLASMAWOOD:
	case SeedType::SEED_TORCHWOOD:
	case SeedType::SEED_TANGLEKELP:
		aScale = 0.4f;
		aOffsetX = 8.0f;
		aOffsetY = 12.0f;
		break;

	case SeedType::SEED_TWINSUNFLOWER:
	case SeedType::SEED_GLOOMSHROOM:
		aScale = 0.45f;
		aOffsetX = 7.0f;
		aOffsetY = 14.0f;
		break;

	case SeedType::SEED_CATTAIL:
		aScale = 0.45f;
		aOffsetX = 8.0f;
		aOffsetY = 13.0f;
		break;

	case SeedType::SEED_UMBRELLA:
		aScale = 0.5f;
		aOffsetX = 5.0f;
		aOffsetY = 10.0f;
		break;

	case SeedType::SEED_KERNELPULT:
		aScale = 0.4f;
		aOffsetX = 13.0f;
		aOffsetY = 14.0f;
		break;

	case SeedType::SEED_SLINGPEA:
	case SeedType::SEED_FROST_CABBAGE:
	case SeedType::SEED_CABBAGEPULT:
		aScale = 0.4f;
		aOffsetX = 15.0f;
		aOffsetY = 14.0f;
		break;

	case SeedType::SEED_MELONPULT:
	case SeedType::SEED_WINTERMELON:
		aScale = 0.35f;
		aOffsetX = 18.0f;
		aOffsetY = 19.0f;
		break;

	case SeedType::SEED_GRAVEBUSTER:
		aScale = 0.4f;
		aOffsetX = 10.0f;
		aOffsetY = 15.0f;
		break;

	case SeedType::SEED_SPLITPEA:
		aScale = 0.45f;
		aOffsetX = 12.0f;
		aOffsetY = 12.0f;
		break;

	case SeedType::SEED_BLOVER:
		aScale = 0.4f;
		aOffsetX = 8.0f;
		aOffsetY = 17.0f;
		break;

	case SeedType::SEED_STARFRUIT:
		aScale = 0.5f;
		aOffsetX = 6.0f;
		aOffsetY = 8.0f;
		break;

	case SeedType::SEED_THREEPEATER:
		aScale = 0.5f;
		aOffsetX = 5.0f;
		aOffsetY = 10.0f;
		break;

	case SeedType::SEED_GATLINGPEA:
		aScale = 0.5f;
		aOffsetX = 2.0f;
		aOffsetY = 8.0f;
		break;

	case SeedType::SEED_ZOMBIE_NORMAL:
	case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
	case SeedType::SEED_ZOMBIE_PAIL:
	case SeedType::SEED_ZOMBIE_DANCER:
		aScale = 0.35f;
		aOffsetX = -3.0f;
		aOffsetY = -7.0f;
		break;

	case SeedType::SEED_ZOMBIE_POLEVAULTER:
		aScale = 0.35f;
		aOffsetX = -8.0f;
		aOffsetY = -12.0f;
		break;

	case SeedType::SEED_ZOMBIE_LADDER:
	case SeedType::SEED_ZOMBIE_DIGGER:
	case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
	case SeedType::SEED_ZOMBIE_POGO:
		aScale = 0.35f;
		aOffsetX = -3.0f;
		aOffsetY = -10.0f;
		break;

	case SeedType::SEED_ZOMBIE_BUNGEE:
		aScale = 0.3f;
		aOffsetX = 1.0f;
		aOffsetY = -1.0f;
		break;

	case SeedType::SEED_ZOMBIE_FOOTBALL:
		aScale = 0.33f;
		aOffsetX = -7.0f;
		aOffsetY = -9.0f;
		break;

	case SeedType::SEED_ZOMBIE_BALLOON:
		aScale = 0.35f;
		aOffsetX = -3.0f;
		aOffsetY = -5.0f;
		break;

	case SeedType::SEED_ZOMBIE_IMP:
		aScale = 0.4f;
		aOffsetX = -12.0f;
		aOffsetY = -17.0f;
		break;

	case SeedType::SEED_ZOMBONI:
		aScale = 0.23f;
		aOffsetX = 12.0f;
		aOffsetY = 3.0f;
		break;

	case SeedType::SEED_ZOMBIE_GARGANTUAR:
		aScale = 0.23f;
		aOffsetX = 4.0f;
		aOffsetY = 3.0f;
		break;

	case SeedType::SEED_CHERRYHOVERBOMB:
		aScale = 0.6f;
		aOffsetX = 12.5f;
		aOffsetY = 6.25f;
		break;

	case SeedType::SEED_STINGER:
		aOffsetY = 25;
		break;

	case SeedType::SEED_SPORE_SHROOM:
		aOffsetY = 12.5f;
		break;

	case SeedType::SEED_GIANT_WALLNUT:
		aScale = 0.75f;
		aOffsetX = -6;
		aOffsetY = -4;
		break;

	case SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE:
	case SeedType::SEED_BEGHOULED_BUTTON_CRATER:
	case SeedType::SEED_SLOT_MACHINE_SUN:
	case SeedType::SEED_SLOT_MACHINE_DIAMOND:
	case SeedType::SEED_ZOMBIQUARIUM_SNORKLE:
	case SeedType::SEED_ZOMBIQUARIUM_TROPHY:
		aDrawSeedInMiddle = false;
		break;
	}

	if (((LawnApp*)gSexyAppBase)->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME)
	{
		if (aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_SUNFLOWER || aSeedType == SeedType::SEED_MARIGOLD)
		{
			aOffsetX = 16.0f;
			aOffsetY = 34.0f;
		}
	}


	aOffsetX = g->mScaleX * aOffsetX;
	aOffsetY = g->mScaleY * (aOffsetY + 1.0f);
	if (aDrawSeedInMiddle)
	{
		SeedPacketDrawSeed(g, x, y, theSeedType, theImitaterType, aOffsetX, aOffsetY, aScale);
	}

	if (thePercentDark > 0.0f)
	{
		int aDarknessHeight = FloatRoundToInt(68.0f * thePercentDark) + 2;
		Graphics aPlantG(*g);
		aPlantG.mScaleX = g->mScaleX;
		aPlantG.mScaleY = g->mScaleY;
		aPlantG.SetColor(Color(64, 64, 64, 255));
		aPlantG.SetColorizeImages(true);
		aPlantG.ClipRect(x, y, SEED_PACKET_WIDTH * aPlantG.mScaleX, aDarknessHeight * aPlantG.mScaleY);
		TodDrawImageCelScaledF(&aPlantG, Sexy::IMAGE_SEEDS, x, y, aPacketBackground, 0, aPlantG.mScaleX, aPlantG.mScaleY);
		if (aDrawSeedInMiddle)
		{
			SeedPacketDrawSeed(&aPlantG, x, y, theSeedType, theImitaterType, aOffsetX, aOffsetY, aScale);
		}
	}

	if (theDrawCost)
	{
		int offsetX = 0;
		SexyString aCostStr;
		if (gLawnApp->mBoard && gLawnApp->mBoard->PlantUsesAcceleratedPricing(aSeedType))
		{
			if (theUseCurrentCost)
			{
				int cost = gLawnApp->mBoard->GetCurrentPlantCost(theSeedType, theImitaterType);
				aCostStr = StrFormat(_S("%d"), cost);

				int numDigits = (cost == 0 ? 0 : static_cast<int>(floor(log10(abs(cost)))));
				offsetX = 6 * max(numDigits - 2, 0);
			}
			else
			{
				int cost = Plant::GetCost(theSeedType, theImitaterType);
				aCostStr = StrFormat(_S("%d+"), cost);

				int numDigits = (cost == 0 ? 0 : static_cast<int>(floor(log10(abs(cost)))));
				offsetX = 6 * max(numDigits - 2, 0) + 6;
			}
		}
		else
		{
			int cost = Plant::GetCost(theSeedType, theImitaterType);
			aCostStr = StrFormat(_S("%d"), Plant::GetCost(theSeedType, theImitaterType));
			int numDigits = (cost == 0 ? 0 : static_cast<int>(floor(log10(abs(cost)))));
			offsetX = 6 * max(numDigits - 2, 0);
		}

		Font* aTextFont = Sexy::FONT_BRIANNETOD12;
		int aTextOffsetX = 32 - aTextFont->StringWidth(aCostStr) + offsetX;
		int aTextOffsetY = aTextFont->GetAscent() + 52;

		if (g->mScaleX == 1.0f && g->mScaleY == 1.0f)
		{
			TodDrawString(g, aCostStr, x + aTextOffsetX, y + aTextOffsetY, aTextFont, Color::Black, DS_ALIGN_LEFT);
			g->SetLinearBlend(true);
		}
		else
		{
			SexyMatrix3 aMatrix;
			if (offX != 0)
			{
				TodScaleTransformMatrix(aMatrix, aTextOffsetX + (32 - aTextFont->StringWidth(aCostStr) * g->mScaleX / 2.0f) + x + offX, aTextOffsetY + (aTextFont->GetAscent() * g->mScaleY / 2.0f) + y + 8, g->mScaleX, g->mScaleY);
			}
			else if (!fromAward)
			{
				TodScaleTransformMatrix(aMatrix, aTextOffsetX* g->mScaleX + x + BOARD_ADDITIONAL_WIDTH, aTextOffsetY* g->mScaleY + y + BOARD_OFFSET_Y, g->mScaleX, g->mScaleY);
			}
			else
			{
				TodScaleTransformMatrix(aMatrix, aTextOffsetX * g->mScaleX + x, aTextOffsetY * g->mScaleY + y, g->mScaleX, g->mScaleY);
			}

			TodDrawStringMatrix(g, aTextFont, aMatrix, aCostStr, Color::Black);
			g->SetLinearBlend(true);
		}
	}

	g->SetColorizeImages(false);
}

void SeedPacket::Draw(Graphics* g)
{
	float aPercentDark = 0.0f;
	if (!mActive)
	{
		if (mRefreshTime == 0)
		{
			aPercentDark = 1.0f;
		}
		else
		{
			aPercentDark = (float)(mRefreshTime - mRefreshCounter) / (float)mRefreshTime;
		}
	}

	if (mSlotMachineCountDown > 0 && mID < 3)
	{
		int aOffsetY = FloatRoundToInt(-mHeight * mSlotMachiningPosition);

		Graphics aClipG(*g);
		aClipG.ClipRect(0, 0, mWidth, mHeight);

		DrawSeedPacket(&aClipG, 0.0f, aOffsetY, mPacketType, SeedType::SEED_NONE, 0.0f, 128, false, false);
		DrawSeedPacket(&aClipG, 0.0f, mHeight + aOffsetY, mSlotMachiningNextSeed, SeedType::SEED_NONE, 0.0f, 128, false, false);
	}
	else
	{
		SeedType aUseSeedType = mPacketType;
		if (mPacketType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
		{
			aUseSeedType = mImitaterType;
		}

		bool aDrawCost = true;
		if (mBoard->HasConveyorBeltSeedBank() || mApp->IsSlotMachineLevel() && mID < 3) 
		{
			aDrawCost = false;
		}
		int aCost = mBoard->GetCurrentPlantCost(mPacketType, mImitaterType);

		int aGrayness = 255;
		if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED && !mActive) ||
			(mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST && !mActive))
		{
			aGrayness = 64;
		}
		else if (mApp->mGameScene != GameScenes::SCENE_PLAYING)
		{
			aGrayness = mBoard->mSeedBank->mCutSceneDarken;
			aPercentDark = 0.0f;
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER && mBoard->mTutorialTimer == -1 && mPacketType == SeedType::SEED_PEASHOOTER)
		{
			aGrayness = GetFlashingColor(mBoard->mMainCounter, 75).mRed;
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER)
		{
			aGrayness = GetFlashingColor(mBoard->mMainCounter, 75).mRed;
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER)
		{
			aGrayness = GetFlashingColor(mBoard->mMainCounter, 75).mRed;
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_WHACK_A_ZOMBIE_PICK_SEED)
		{
			aGrayness = GetFlashingColor(mBoard->mMainCounter, 75).mRed;
		}
		else if (mApp->mEasyPlantingCheat)
		{
			aPercentDark = 0.0f;
		}
		else if ((!mBoard->CanTakeSunMoney(aCost) && aDrawCost) || aPercentDark > 1.0f || !mBoard->PlantingRequirementsMet(aUseSeedType))
		{
			aGrayness = 128;
		}
		
		DrawSeedPacket(g, mOffsetX, 0, mPacketType, mImitaterType, aPercentDark, aGrayness, aDrawCost, true, false, mX - 25);
	}
}

//0x488500
bool SeedPacket::CanPickUp()
{
	if (mBoard->mPaused || mApp->mGameScene != GameScenes::SCENE_PLAYING || mPacketType == SeedType::SEED_NONE)
	{
		return false;
	}

	SeedType aUseSeedType = mPacketType;
	if (mPacketType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
	{
		aUseSeedType = mImitaterType;
	}

	if (mApp->IsSlotMachineLevel())
	{
		return false;
	}

	if (!mApp->mEasyPlantingCheat)
	{
		if (!mActive)
		{
			return false;
		}

		int aCost = mBoard->GetCurrentPlantCost(mPacketType, mImitaterType);
		if (!mBoard->CanTakeSunMoney(aCost) && !mBoard->HasConveyorBeltSeedBank())
		{
			return false;
		}

		if (!mBoard->PlantingRequirementsMet(aUseSeedType))
		{
			return false;
		}
	}

	return true;
}

//0x488590
void SeedPacket::MouseDown(int x, int y, int theClickCount)
{
	if (mBoard->mPaused || mApp->mGameScene != GameScenes::SCENE_PLAYING || mPacketType == SeedType::SEED_NONE)
	{
		return;
	}

	if (mApp->IsSlotMachineLevel() && mID < 3)
	{
		if (!mBoard->mAdvice->IsBeingDisplayed())
		{
			mBoard->DisplayAdvice(_S("[ADVICE_SLOT_MACHINE_PULL]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_NONE);
		}
		mBoard->mChallenge->mSlotMachineRollCount = min(mBoard->mChallenge->mSlotMachineRollCount, 2);
		return;
	}

	SeedType aUseSeedType = mPacketType;
	if (mPacketType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
	{
		aUseSeedType = mImitaterType;
	}

	if (!mApp->mEasyPlantingCheat)
	{
		if (!mActive)
		{
			mApp->PlaySample(SOUND_BUZZER);
			if (mApp->IsFirstTimeAdventureMode() && mBoard->mLevel == 1 && mBoard->mHelpDisplayed[(int)AdviceType::ADVICE_CLICK_ON_SUN])
			{
				mBoard->DisplayAdvice(_S("[ADVICE_SEED_REFRESH]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1, AdviceType::ADVICE_SEED_REFRESH);
			}
			return;
		}

		int aCost = mBoard->GetCurrentPlantCost(mPacketType, mImitaterType);
		if (!mBoard->CanTakeSunMoney(aCost) && !mBoard->HasConveyorBeltSeedBank())
		{
			mApp->PlaySample(SOUND_BUZZER);
			mBoard->mOutOfMoneyCounter = 70;
			if (mApp->IsFirstTimeAdventureMode() && mBoard->mLevel == 1 && mBoard->mHelpDisplayed[(int)AdviceType::ADVICE_CLICK_ON_SUN])
			{
				mBoard->DisplayAdvice(_S("[ADVICE_CANT_AFFORD_PLANT]"), MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1, AdviceType::ADVICE_CANT_AFFORD_PLANT);
			}
			return;
		}

		if (!mBoard->PlantingRequirementsMet(aUseSeedType))
		{
			mApp->PlaySample(SOUND_BUZZER);
			if (aUseSeedType == SeedType::SEED_GATLINGPEA)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_REPEATER]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_REPEATER);
			}
			else if (aUseSeedType == SeedType::SEED_PLASMAWOOD)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_TORCHWOOD]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_REPEATER);
			}
			else if (aUseSeedType == SeedType::SEED_WINTERMELON)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_MELONPULT]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_MELONPULT);
			}
			else if (aUseSeedType == SeedType::SEED_TWINSUNFLOWER)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_SUNFLOWER]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_SUNFLOWER);
			}
			else if (aUseSeedType == SeedType::SEED_SPIKEROCK)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_SPIKEWEED]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_SPIKEWEED);
			}
			else if (aUseSeedType == SeedType::SEED_COBCANNON)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_KERNELPULT]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_KERNELPULT);
			}
			else if (aUseSeedType == SeedType::SEED_GOLD_MAGNET)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_MAGNETSHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_MAGNETSHROOM);
			}
			else if (aUseSeedType == SeedType::SEED_GLOOMSHROOM)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_FUMESHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_FUMESHROOM);
			}
			else if (aUseSeedType == SeedType::SEED_CATTAIL)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_LILYPAD]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_LILYPAD);
			}
			else if (aUseSeedType == SeedType::SEED_SHOOTINGSTAR)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_STARFRUIT]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_LILYPAD);
			}
			else if (aUseSeedType == SeedType::SEED_SUPERCHOMPER)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_CHOMPER]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_LILYPAD);
			}
			else if (aUseSeedType == SeedType::SEED_BRAVEYSHROOM)
			{
				mBoard->DisplayAdvice(_S("[ADVICE_PLANT_NEEDS_SCAREDY_SHROOM]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_PLANT_NEEDS_LILYPAD);
			}
			else
			{
				TOD_ASSERT();
			}

			return;
		}
	}

	mBoard->ClearAdvice(AdviceType::ADVICE_CANT_AFFORD_PLANT);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_REPEATER);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_MELONPULT);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_SUNFLOWER);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_KERNELPULT);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_SPIKEWEED);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_MAGNETSHROOM);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_FUMESHROOM);
	mBoard->ClearAdvice(AdviceType::ADVICE_PLANT_NEEDS_LILYPAD);

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
	{
		mBoard->mChallenge->BeghouledPacketClicked(this);
	}
	else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM)
	{
		mBoard->mChallenge->ZombiquariumPacketClicked(this);
	}
	else
	{
		mBoard->mCursorObject->mType = mPacketType;
		mBoard->mCursorObject->mImitaterType = mImitaterType;
		mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_BANK;
		mBoard->mCursorObject->mSeedBankIndex = mIndex;
		mApp->PlaySample(SOUND_SEEDLIFT);

		if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER)
		{
			mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PLANT_PEASHOOTER);
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER)
		{
			if (mPacketType == SeedType::SEED_SUNFLOWER)
			{
				mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PLANT_SUNFLOWER);
			}
			else
			{
				mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER);
			}
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER)
		{
			if (mPacketType == SeedType::SEED_SUNFLOWER)
			{
				mBoard->SetTutorialState(TutorialState::TUTORIAL_MORESUN_PLANT_SUNFLOWER);
			}
			else
			{
				mBoard->SetTutorialState(TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER);
			}
		}
		else if (mBoard->mTutorialState == TutorialState::TUTORIAL_WHACK_A_ZOMBIE_PICK_SEED || mBoard->mTutorialState == TutorialState::TUTORIAL_WHACK_A_ZOMBIE_BEFORE_PICK_SEED)
		{
			mBoard->SetTutorialState(TutorialState::TUTORIAL_WHACK_A_ZOMBIE_COMPLETED);
		}

		if (mBoard->mPlayers[0]->mControlType == CONTROL_MOUSE) {
			mBoard->mPlayers[0]->mSeedBankIndex = mIndex;
		}

		//Deactivate();
	}
}

//0x488EC0
void SeedPacket::WasPlanted()
{
	//TOD_ASSERT(mPacketType != SeedType::SEED_NONE);

	if (mBoard->HasConveyorBeltSeedBank())
	{
		mBoard->mSeedBank->RemoveSeed(mIndex);
	}
	else if (mApp->IsSlotMachineLevel() && mID < 3)
	{
		Deactivate();
	}
	else if (LawnApp::IsLastStandLevel(mApp->mGameMode) && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT)
	{
		mTimesUsed++;
		mActive = true;
		FlashIfReady();
	}
	else
	{
		mTimesUsed++;
		mRefreshing = true;
		mRefreshTime = Plant::GetRefreshTime(mPacketType, mImitaterType);
	}
}

//0x488F90
bool SeedPacket::MouseHitTest(int theX, int theY, HitResult* theHitResult)
{
	if (mSlotMachineCountDown > 0 && mID < 3 || mPacketType == SeedType::SEED_NONE)
	{
		theHitResult->mObject = nullptr;
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
		return false;
	}

	if (theX >= mX + mOffsetX && theX < mX + mOffsetX + mWidth && theY >= mY && theY < mY + mHeight)
	{
		theHitResult->mObject = this;
		theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SEEDPACKET;
		return true;
	}

	theHitResult->mObject = nullptr;
	theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
	return false;
}

//0x489000
SeedBank::SeedBank()
{
	mWidth = IMAGE_SEEDBANK->GetWidth();
	mHeight = IMAGE_SEEDBANK->GetHeight();
	for (int i = 0; i < SEEDBANK_MAX; i++){
		mSeedPacketsTime[i] = 0;
	}
	mNumPackets = 0;
	misP2 = false;
	mConveyorBeltCounter = 0;
	mCutSceneDarken = 255;
}

//0x489630
void SeedBank::Draw(Graphics* g)
{
	if (mBoard->mCutScene->IsBeforePreloading())
	{
		return;
	}

	if (mApp->mGameScene != GameScenes::SCENE_PLAYING)
	{
		g->mTransX -= mBoard->mX - BOARD_ADDITIONAL_WIDTH;
		g->mTransY -= mBoard->mY - BOARD_OFFSET_Y;
	}

	if (mNumPackets == 0 || mApp->IsSlotMachineLevel() && mNumPackets == 3)
	{
		g->DrawImage(IMAGE_SUNBANK, 0, 0);
	}
	else if (mBoard->HasConveyorBeltSeedBank())
	{
		//g->mTransX -= 12;
		/*if (mBoard->mIsCOOP)
		{
			g->DrawImage(IMAGE_CONVEYORBELT_BACKDROP_COOP, 83, 0);
			g->SetClipRect(90, 63, 317, 96);
			g->DrawImageCel(IMAGE_CONVEYORBELT, 90, 63, mConveyorBeltCounter / 4 % 6);
			g->SetClipRect(90, 0, 317, BOARD_HEIGHT);
		}
		else*/ {
			g->DrawImage(IMAGE_CONVEYORBELT_BACKDROP, 83, 0);
			g->DrawImageCel(IMAGE_CONVEYORBELT, 90, 63, mConveyorBeltCounter / 4 % 6);
			g->SetClipRect(90, 0, 501, BOARD_HEIGHT);
		}
		//g->mTransX += 12;
	}
	else if (mNumPackets > 0)
	{
		g->DrawImage(IMAGE_SEEDSUNBANK, 0, 0);
		g->DrawImage(IMAGE_SEEDBANK, 79, 0, mWidth, mHeight);
		g->DrawImage(IMAGE_SEEDBANKEND, 79 + mWidth, 0);
	}

	if (mApp->mGameScene == SCENE_PLAYING) {
		for (int i = 0; i < mNumPackets; i++)
		{
			bool willSkip = false;
			for (int playerID = 0; playerID < mBoard->mMaxPlayers; playerID++) {
				if (mBoard->mPlayers[playerID]->mSeedBankIndex == i){
					willSkip = true;
					break;
				}
			}

			if (willSkip)
				continue;

			int& time = mSeedPacketsTime[i];
			time = max(time - 10, 0);

			SeedPacket* aSeedPacket = &mSeedPackets[i];

			Graphics seedG(*g);
			seedG.mScaleX = 1 + time / 100.0f * 0.25f;
			seedG.mScaleY = 1 + time / 100.0f * 0.25f;
			seedG.mTransX -= 6.25f * time / 100.0f;
			seedG.mTransY -= 8.75f * time / 100.0f;
			if (aSeedPacket->mPacketType != SeedType::SEED_NONE && aSeedPacket->BeginDraw(&seedG))
			{
				aSeedPacket->Draw(&seedG);
				aSeedPacket->EndDraw(&seedG);
			}
		}

		Image* imgSeedSelectors[4] = {
			IMAGE_SEED_SELECTOR,
			IMAGE_SEED_SELECTOR_BLUE,
			IMAGE_SEED_SELECTOR_GREEN,
			IMAGE_SEED_SELECTOR_RED
		};

		Image* imgArrows[4] = {
		IMAGE_P1_ARROW,
		IMAGE_P2_ARROW,
		IMAGE_P3_ARROW,
		IMAGE_P4_ARROW
		};

		Image* imgText[4] = {
			IMAGE_P1_TEXT,
			IMAGE_P2_TEXT,
			IMAGE_P3_TEXT,
			IMAGE_P4_TEXT
		};

		for (int i = 0; i < mBoard->mMaxPlayers; i++)
		{
			PlayerController* player = mBoard->mPlayers[i];

			SeedPacket* aSeedPacket = &mSeedPackets[player->mSeedBankIndex];
			if (aSeedPacket && aSeedPacket->mPacketType != SeedType::SEED_NONE)
			{
				int& time = mSeedPacketsTime[player->mSeedBankIndex];
				time = min(100, time + 20);

				float scale = 1 + time / 100.0f * 0.25f;
				float translationX = 6.25f * time / 100.0f;
				float translationY = 8.75f * time / 100.0f;

				Graphics seedG(*g);
				seedG.mScaleX = scale;
				seedG.mScaleY = scale;
				seedG.mTransX -= translationX;
				seedG.mTransY -= translationY;
				seedG.ClearClipRect();

				SexyMatrix3 matr;
				matr.ZeroMatrix();
				matr.m00 = scale;
				matr.m11 = scale;

				float seedX = aSeedPacket->mX + aSeedPacket->mOffsetX + 25 + 6.25f * time / 100.0f;
				float seedY = aSeedPacket->mY + 35 + 8.75f * time / 100.0f;
				seedG.DrawImageMatrix(imgSeedSelectors[player->mPlayerID], matr, seedX, seedY);

				if (aSeedPacket->BeginDraw(&seedG)) {
					aSeedPacket->Draw(&seedG);
					aSeedPacket->EndDraw(&seedG);
				}

				int& arrowTime = player->mArrowTime;
				float P1ArrowY = 0;
				if (arrowTime < 25) {
					P1ArrowY -= 1.5f;
					P1ArrowY += arrowTime / 25.0f * 1.5f;
				}
				else if (arrowTime >= 25 && arrowTime < 50) {
					P1ArrowY += (arrowTime - 25) / 25.0f * 1.5f;
				}
				else if (arrowTime >= 50 && arrowTime < 75) {
					P1ArrowY += 1.5f;
					P1ArrowY -= (arrowTime - 50) / 25.0f * 1.5f;
				}
				else if (arrowTime >= 75 && arrowTime < 100) {
					P1ArrowY -= (arrowTime - 75) / 25.0f * 1.5f;
				}

				matr.m11 = -scale;
				seedG.DrawImageMatrix(imgArrows[player->mPlayerID], matr, aSeedPacket->mX + aSeedPacket->mOffsetX + 25 * 1.25f, aSeedPacket->mY + 88 - P1ArrowY);
				matr.m11 = -scale;

				seedG.mScaleX = 1;
				seedG.mScaleY = 1;
				seedG.mTransX += translationX;
				seedG.mTransY += translationY;
				if (mBoard->mMaxPlayers > 1)
					seedG.DrawImageF(imgText[player->mPlayerID], aSeedPacket->mX + aSeedPacket->mOffsetX + 25 * 1.25f - 23.75f, aSeedPacket->mY + 88);
			}
		}
	}
	else {
		for (int i = 0; i < mNumPackets; i++)
		{
			SeedPacket* aSeedPacket = &mSeedPackets[i];
			if (aSeedPacket->mPacketType != SeedType::SEED_NONE && aSeedPacket->BeginDraw(g))
			{
				aSeedPacket->Draw(g);
				aSeedPacket->EndDraw(g);
			}
		}
	}

	g->ClearClipRect();

	if (mApp->IsSlotMachineLevel() && mY > -IMAGE_SEEDBANK->GetHeight())
	{
		g->DrawImage(IMAGE_SLOTMACHINE_OVERLAY, 189, -2);
	}

	if (!mBoard->HasConveyorBeltSeedBank())
	{
		SexyString aMoneyLabel = StrFormat(_S("%d"), max(misP2 ? mBoard->mSunMoneyP2 : mBoard->mSunMoney, 0));
		Color aMoneyColor(0, 0, 0);
		if (mBoard->mOutOfMoneyCounter > 0 && mBoard->mOutOfMoneyCounter % 20 < 10)
		{
			aMoneyColor = Color(255, 0, 0);
		}

		TodDrawString(g, aMoneyLabel, 34, 78, FONT_CONTINUUMBOLD14, aMoneyColor, DrawStringJustification::DS_ALIGN_CENTER);
	}

	if (mApp->mGameScene != GameScenes::SCENE_PLAYING)
	{
		g->mTransX += mBoard->mX - BOARD_ADDITIONAL_WIDTH;
		g->mTransY += mBoard->mY - BOARD_OFFSET_Y;
	}
}

//0x489970
bool SeedBank::MouseHitTest(int x, int y, HitResult* theHitResult)
{
	//if (x - mX <= mWidth - 5 + 79 + (mBoard->mIsCOOP ? 130 : 0) && mNumPackets > 0)
	if (x - mX <= mWidth - 5 + 79 && mNumPackets > 0)
	{
		for (SeedPacket& aSeedPacket : mSeedPackets)
		{
			if (aSeedPacket.MouseHitTest(x - mX, y - mY, theHitResult))
			{
				return true;
			}
		}
	}

	theHitResult->mObject = nullptr;
	theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
	return false;
}

//0x489A20
bool SeedBank::ContainsPoint(int theX, int theY)
{
	return theX >= mX && theX < mX + mWidth && theY >= mY && theY < mY + mHeight;
}

//0x489A50
void SeedBank::AddSeed(SeedType theSeedType, bool thePlaceOnLeft)
{
	TOD_ASSERT(mBoard->HasConveyorBeltSeedBank());
	TOD_ASSERT(theSeedType != SeedType::SEED_NONE);

	int aNumSeeds = GetNumSeedsOnConveyorBelt();
	if (aNumSeeds == mNumPackets)
	{
		return;
	}

	SeedPacket* aSeedPacket = &mSeedPackets[aNumSeeds];
	aSeedPacket->mPacketType = theSeedType;
	aSeedPacket->mRefreshCounter = 0;
	aSeedPacket->mRefreshTime = 0;
	aSeedPacket->mRefreshing = false;
	aSeedPacket->mActive = true;
	aSeedPacket->mOffsetX = 515 - (SEED_PACKET_WIDTH + 1) * aNumSeeds;
	//aSeedPacket->mOffsetX = (mBoard->mIsCOOP ? 333 : 515) - (SEED_PACKET_WIDTH + 1) * aNumSeeds;
	if (thePlaceOnLeft)
	{
		aSeedPacket->mOffsetX = 0;
	}
	if (aNumSeeds > 0)
	{
		SeedPacket* aPrevPacket = &mSeedPackets[aNumSeeds - 1];
		if (aSeedPacket->mOffsetX < aPrevPacket->mOffsetX)
		{
			aSeedPacket->mOffsetX = aPrevPacket->mOffsetX + 40;
		}
	}
}

//0x489AC0
void SeedBank::RemoveSeed(int theIndex)
{
	TOD_ASSERT(mBoard->HasConveyorBeltSeedBank());
	TOD_ASSERT(theIndex >= 0 && theIndex < GetNumSeedsOnConveyorBelt());

	for (int i = theIndex; i < mNumPackets; i++)
	{
		SeedPacket* aSeedPacket = &mSeedPackets[i];
		if (aSeedPacket->mPacketType == SeedType::SEED_NONE)
		{
			break;
		}

		if (i == mNumPackets - 1)
		{
			aSeedPacket->mPacketType = SeedType::SEED_NONE;
			aSeedPacket->mOffsetX = 0;
		}
		else
		{
			SeedPacket* aNextPacket = &mSeedPackets[i + 1];
			aSeedPacket->mPacketType = aNextPacket->mPacketType;
			aSeedPacket->mOffsetX = aNextPacket->mOffsetX + SEED_PACKET_WIDTH + 1;
		}

		aSeedPacket->mRefreshCounter = 0;
		aSeedPacket->mRefreshTime = 0;
		aSeedPacket->mRefreshing = false;
		aSeedPacket->mActive = true;
	}
}

//0x489B20
int SeedBank::GetNumSeedsOnConveyorBelt()
{
	for (int i = 0; i < mNumPackets; i++)
	{
		if (mSeedPackets[i].mPacketType == SeedType::SEED_NONE)
		{
			return i;
		}
	}
	return mNumPackets;
}

int SeedBank::CountOfTypeOnConveyorBelt(SeedType theSeedType)
{
	int aCount = 0;
	for (int i = 0; i < mNumPackets; i++)
	{
		if (mSeedPackets[i].mPacketType == theSeedType)
		{
			aCount++;
		}
	}
	return aCount;
}

//0x489B50
void SeedPacket::SetPacketType(SeedType theSeedType, SeedType theImitaterType)
{
	mPacketType = theSeedType;
	mImitaterType = theImitaterType;
	mRefreshCounter = 0;
	mRefreshTime = 0;
	mRefreshing = false;
	mActive = true;

	SeedType aUseSeedType = theSeedType;
	if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
	{
		aUseSeedType = theImitaterType;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST || 
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM || LawnApp::IsLastStandLevel(mApp->mGameMode) ||
		mApp->IsIZombieLevel() || mApp->IsScaryPotterLevel() || mApp->IsWhackAZombieLevel() || (mApp->IsSurvivalMode() && mBoard->mChallenge->mSurvivalStage > 0))
		return;

	if ((Plant::IsUpgrade(aUseSeedType) && !gLawnApp->IsSurvivalMode()) || Plant::GetRefreshTime(mPacketType, mImitaterType) == 5000 && mPacketType != SEED_SUNBLOOM)
	{
		mRefreshTime = 3500;
		mRefreshing = true;
		mActive = false;
	}
	else if (Plant::IsUpgrade(aUseSeedType) && gLawnApp->IsSurvivalMode())
	{
		mRefreshTime = 8000;
		mRefreshing = true;
		mActive = false;
	}
	else if (Plant::GetRefreshTime(mPacketType, mImitaterType) == 3000)
	{
		mRefreshTime = 2000;
		mRefreshing = true;
		mActive = false;
	}
}

//0x489C70
void SeedBank::UpdateConveyorBelt()
{
	mConveyorBeltCounter++;

	int speed = CONVEYOR_SPEED;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BOMB_ALL)
		speed = 0;

	if (speed != 0 && mConveyorBeltCounter % speed == 0)
	{
		for (int i = 0; i < mNumPackets; i++){
			SeedPacket* aSeedPacket = &mSeedPackets[i];
			if (aSeedPacket->mOffsetX > 0)
				aSeedPacket->mOffsetX = max(aSeedPacket->mOffsetX - 1, 0);
		}
		mBoard->UpdateToolTip();
	}
	else if (speed == 0) {
		for (int i = 0; i < mNumPackets; i++){
			SeedPacket* aSeedPacket = &mSeedPackets[i];
			aSeedPacket->mOffsetX = 0;
		}
		mBoard->UpdateToolTip();
	}
}

//0x489CD0
void SeedBank::UpdateWidth(bool fromBank)
{
	if (fromBank)
		mNumPackets = mBoard->GetNumSeedsInBank();
	if (mApp->mGameMode == GAMEMODE_CHALLENGE_BEGHOULED || mApp->mGameMode == GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
		mNumPackets = 4;

	/*if (mBoard->mIsCOOP)
	{
		mNumPackets = floor(mNumPackets / 2);
		mNumPackets = min(4, mNumPackets);
	}*/

	if (mApp->IsSlotMachineLevel())
	{
		for (int i = 0; i < mNumPackets; i++)
		{
			mSeedPackets[i].mID = i;
		}
	}
	mWidth = 51 * mNumPackets;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_SLOT_MACHINE_2)	mWidth = 51;
	
	for (int i = 0; i < mNumPackets; i++)
	{
		mSeedPackets[i].mX = mBoard->GetSeedPacketPositionX(i);
	}
}

//0x489D50
void SeedBank::RefreshAllPackets()
{
	for (int i = 0; i < mNumPackets; i++)
	{
		SeedPacket* aSeedPacket = &mSeedPackets[i];
		if (aSeedPacket->mPacketType == SeedType::SEED_NONE)
		{
			break;
		}

		if (aSeedPacket->mRefreshing)
		{
			aSeedPacket->mRefreshCounter = 0;
			aSeedPacket->mRefreshing = false;
			aSeedPacket->mActive = true;
			aSeedPacket->FlashIfReady();
		}
	}
}
