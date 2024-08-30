#include "PlayerController.h"
#include "Board.h"
#include "Challenge.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../SexyAppFramework/MemoryImage.h"

PlayerController::PlayerController() {
}

void PlayerController::Initialize() {
	Plant* mPreviewPlant = mBoard->mExtraPlants.DataArrayAlloc();
	mPreviewPlant->mIsOnBoard = true;
	mPreviewPlant->mSeedType = SEED_NONE;
	mPreviewPlant->mIsPreview = true;
	mPreviewPlant->mEnemy = mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS;
	mPreviewPlantID = (PlantID)mBoard->mExtraPlants.DataArrayGetID(mPreviewPlant);

	Plant* mPlayerPlant = mBoard->mExtraPlants.DataArrayAlloc();
	mPlayerPlant->mIsOnBoard = false;
	mPlayerPlant->mSeedType = SEED_NONE;
	mPlayerPlant->mIsPlayer = true;
	mPlayerPlantID = (PlantID)mBoard->mExtraPlants.DataArrayGetID(mPlayerPlant);

	mSelectorTime = Rand(100);
	mArrowTime = Rand(100);
	mSeedBankIndex = -1;
	mControlType = CONTROL_KEYBOARD;
	mAbilityType = ABILITY_SHOVEL;
	mGlovePlantID = PLANTID_NULL;
}

void PlayerController::Draw(Graphics* g) {
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

	Image* imgSelectors[4] = {
		IMAGE_BOARD_SELECTOR,
		IMAGE_BOARD_SELECTOR_BLUE,
		IMAGE_BOARD_SELECTOR_GREEN,
		IMAGE_BOARD_SELECTOR_RED
	};

	int	gridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
	int gridY = mBoard->PixelToGridYKeepOnBoard(mX, mY);

	float arrowFloat = mBoard->StageHasRoof() ? 12 : 0;

	if (mControllerType == CONTROLLER_PLANT || mControllerType == CONTROLLER_ZOMBIE || mControllerType == CONTROLLER_SHOVEL)
	{
		SexyMatrix3 matrix = SexyMatrix3();
		matrix.ZeroMatrix();
		matrix.m00 = 0.8173f;
		matrix.m11 = mBoard->StageHas6Rows() || mBoard->StageHasRoof() ? 0.59f : 0.69f;

		if (mBoard->StageHasRoof() && mX < 440.0f)
			matrix.m10 = -0.25f;

		float selectorFloat = mBoard->StageHasRoof() ? 12 : 0;
		float selectorCycle = mSelectorTime % 100;
		if (selectorCycle < 25) {
			selectorFloat -= 1.5f * (1 - selectorCycle / 25.0f);
		}
		else if (selectorCycle < 50) {
			selectorFloat += 1.5f * ((selectorCycle - 25) / 25.0f);
		}
		else if (selectorCycle < 75) {
			selectorFloat += 1.5f * (1 - (selectorCycle - 50) / 25.0f);
		}
		else {
			selectorFloat -= 1.5f * ((selectorCycle - 75) / 25.0f);
		}

		g->DrawImageMatrix(imgSelectors[mPlayerID], matrix, mBoard->GridToPixelX(gridX, gridY) + 32.5f, mBoard->GridToPixelY(gridX, gridY) + 42.5f + selectorFloat);

		float scale = 1;
		float arrowCycle = mArrowTime % 100;
		if (arrowCycle < 25) {
			scale += 0.1f * (arrowCycle / 25.0f - 1);
			arrowFloat -= 1.5f * (1 - arrowCycle / 25.0f);
		}
		else if (arrowCycle < 50) {
			scale += 0.1f * ((arrowCycle - 25.0f) / 25.0f);
			arrowFloat += 1.5f * ((arrowCycle - 25) / 25.0f);
		}
		else if (arrowCycle < 75) {
			scale += 0.1f * (1 - (arrowCycle - 50.0f) / 25.0f);
			arrowFloat += 1.5f * (1 - (arrowCycle - 50) / 25.0f);
		}
		else {
			scale -= 0.1f * ((arrowCycle - 75.0f) / 25.0f);
			arrowFloat -= 1.5f * ((arrowCycle - 75) / 25.0f);
		}

		matrix.m00 = scale;
		matrix.m11 = scale;
		g->DrawImageMatrix(IMAGE_BEJEWEL_SELECTORSHADOW, matrix, mX + 7.5f, mY + (mBoard->StageHasRoof() || mBoard->StageHas6Rows() ? 42.5f : 50));

		if (mControllerType == CONTROLLER_PLANT && mSeedBankIndex != -1) {
			SeedType seedType = mBoard->mSeedBank->mSeedPackets[mSeedBankIndex].mPacketType;
			SeedType imitaterType = mBoard->mSeedBank->mSeedPackets[mSeedBankIndex].mImitaterType;

			if (mGlovePlantID != PLANTID_NULL){
				Plant* aPlant = mBoard->mExtraPlants.DataArrayTryToGet(mGlovePlantID);
				if (aPlant){
					seedType = aPlant->mSeedType;
					imitaterType = aPlant->mImitaterType;
				}
			}

			if (seedType == SEED_IMITATER) {
				SeedType prevSeedType = seedType;
				seedType = imitaterType;
				imitaterType = prevSeedType;
			}

			Plant* mPreviewPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int)mPreviewPlantID);
			if (mPreviewPlant)
			{
				if (mPreviewPlant->mSeedType != seedType || mPreviewPlant->mImitaterType != imitaterType) {
					
					if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
						mPreviewPlant->mEnemy = true;

					mPreviewPlant->PlantInitialize(0, 0, seedType, imitaterType);
					mPreviewPlant->UpdateReanim();

					if (imitaterType == SEED_IMITATER) {
						FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
						if (imitaterType == SeedType::SEED_HYPNOSHROOM || imitaterType == SeedType::SEED_SQUASH || imitaterType == SeedType::SEED_POTATOMINE ||
							imitaterType == SeedType::SEED_GARLIC || imitaterType == SeedType::SEED_LILYPAD)
							aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

						Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mPreviewPlant->mBodyReanimID);
						if (aBodyReanim)
						{
							aBodyReanim->mFilterEffect = aFilter;
						}
						Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID);
						if (aHeadReanim)
						{
							aHeadReanim->mFilterEffect = aFilter;
						}Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID2);
						if (aHeadReanim2)
						{
							aHeadReanim2->mFilterEffect = aFilter;
						}Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID3);
						if (aHeadReanim3)
						{
							aHeadReanim3->mFilterEffect = aFilter;
						}
					}
					else {
						Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mPreviewPlant->mBodyReanimID);
						if (aBodyReanim)
						{
							aBodyReanim->mFilterEffect = FILTER_EFFECT_NONE;
						}
						Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID);
						if (aHeadReanim)
						{
							aHeadReanim->mFilterEffect = FILTER_EFFECT_NONE;
						}Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID2);
						if (aHeadReanim2)
						{
							aHeadReanim2->mFilterEffect = FILTER_EFFECT_NONE;
						}Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(mPreviewPlant->mHeadReanimID3);
						if (aHeadReanim3)
						{
							aHeadReanim3->mFilterEffect = FILTER_EFFECT_NONE;
						}
					}
				}

				mPreviewPlant->mX = mBoard->GridToPixelX(gridX, gridY);
				mPreviewPlant->mY = mBoard->GridToPixelY(gridX, gridY);
				mPreviewPlant->mPlantCol = gridX;
				mPreviewPlant->mRow = gridY;
				mPreviewPlant->CalcRenderOrder();

				if (mPreviewPlant->mSeedType != SEED_NONE && mPreviewPlant->mSeedType < NUM_SEED_TYPES) {

					SeedType seedType = mPreviewPlant->mSeedType;
					bool canTakeSuns = mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(mBoard->mSeedBank->mSeedPackets[mSeedBankIndex].mPacketType, SeedType::SEED_NONE));
					bool canUseSeed = mBoard->mSeedBank->mSeedPackets[mSeedBankIndex].mActive;
					bool canPlant = mBoard->CanPlantAt(gridX, gridY, seedType) == PLANTING_OK;
					if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && seedType != SEED_CHERRYHOVERBOMB) {
						canPlant = canPlant && gridX >= 4;
					}
					bool allowToPlace = (canTakeSuns || mBoard->HasConveyorBeltSeedBank()) && canUseSeed && canPlant || mApp->mEasyPlantingCheat;
					bool mDarken = !allowToPlace;

					if (!mDarken && !mBoard->mPaused) {
						mPreviewPlant->Update();

						if (mBoard->mIsSpeeding)
							mPreviewPlant->Update();
					}

					Graphics plantG(*g);
					plantG.SetColorizeImages(true);
					plantG.mColor = mDarken ? Color::Black : Color::White;
					plantG.mColor.mAlpha = 64;

					Graphics aMemoryGraphics(mBoard->mPreviewImage);
					aMemoryGraphics.ClearRect(0, 0, mBoard->mPreviewImage->mWidth, mBoard->mPreviewImage->mHeight);
					aMemoryGraphics.SetLinearBlend(true);

					aMemoryGraphics.mTransX = 0;
					aMemoryGraphics.mTransY = 0;

					if (mPreviewPlant->BeginDraw(&aMemoryGraphics)) {
						mPreviewPlant->Draw(&aMemoryGraphics);
						mPreviewPlant->EndDraw(&aMemoryGraphics);
					}

					plantG.DrawImage(mBoard->mPreviewImage, 0, 0);
				}
			}
		}
	}

	SexyMatrix3 matrix = SexyMatrix3();
	matrix.ZeroMatrix();
	matrix.m00 = 1.25f;
	matrix.m11 = 1.25f;

	if (mBoard->mMaxPlayers > 1)
	{
		g->DrawImageF(imgText[mPlayerID], mX - 15, mY - 22.5f);
	}
	g->DrawImageMatrix(imgArrows[mPlayerID], matrix, mX, mY + arrowFloat + 7.5f - (mBoard->StageHasRoof() ? 15 : 0));
}

void PlayerController::UpdateAnimations() {
	mSelectorTime++;
	mSelectorTime = fmod(mSelectorTime, 100);
	mArrowTime++;
	mArrowTime = fmod(mArrowTime, 100);

	if (mArrowTime % 2 == 1) {
		mArrowTime++;
		mArrowTime = fmod(mArrowTime, 100);
	}
}