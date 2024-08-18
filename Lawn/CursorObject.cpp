#include "Board.h"
#include "Cutscene.h"
#include "ZenGarden.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../Resources.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../SexyAppFramework/WidgetManager.h"

//0x438640
CursorObject::CursorObject()
{
    mType = SeedType::SEED_NONE;
    mImitaterType = SeedType::SEED_NONE;
    mSeedBankIndex = -1;
    mX = 0;
    mY = 0;
    mCursorType = CursorType::CURSOR_TYPE_NORMAL;
    mCoinID = CoinID::COINID_NULL;
    mDuplicatorPlantID = PlantID::PLANTID_NULL;
    mCobCannonPlantID = PlantID::PLANTID_NULL;
    mGlovePlantID = PlantID::PLANTID_NULL;
    mRootPlantID = PlantID::PLANTID_NULL;
    mReanimCursorID = ReanimationID::REANIMATIONID_NULL;
    
    if (mApp->IsWhackAZombieLevel())
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_HAMMER, true);
        Reanimation* aHammerReanim = mApp->AddReanimation(-25.0f, 16.0f, 0, ReanimationType::REANIM_HAMMER);
        aHammerReanim->mIsAttachment = true;
        aHammerReanim->PlayReanim("anim_whack_zombie", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
        aHammerReanim->mAnimTime = 1.0f;
        mReanimCursorID = mApp->ReanimationGetID(aHammerReanim);
    }

    mWidth = 80;
    mHeight = 80;
}

//0x438780
void CursorObject::Update()
{
    if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->IsInShovelTutorial())
    {
        mVisible = false;
        return;
    }

    if (!mApp->mWidgetManager->mMouseIn)
    {
        mVisible = false;
        return;
    }

    Reanimation* aCursorReanim = mApp->ReanimationTryToGet(mReanimCursorID);
    if (aCursorReanim)
    {
        aCursorReanim->Update();
    }

    mVisible = true;
    mX = mApp->mWidgetManager->mLastMouseX - 25 - BOARD_ADDITIONAL_WIDTH;
    mY = mApp->mWidgetManager->mLastMouseY - 35 - BOARD_OFFSET_Y;

    /*if (mBoard && !mBoard->mIsDown) {
        if (mBoard->IsPlantInCursor()) {
            mBoard->MouseDownWithPlant(mX, mY, 1);
        }
        else if (mCursorType != CURSOR_TYPE_NORMAL) {
            mBoard->MouseDownWithTool(mX, mY, 1, mCursorType);
        }
        else if (mCobCannonPlantID != PLANTID_NULL) {
            mBoard->MouseDownCobcannonFire(mX, mY, 1);
        }
    }*/
}

void CursorObject::Die()
{
    mApp->RemoveReanimation(mReanimCursorID);
}

//0x438820
void CursorObject::Draw(Graphics* g)
{
    switch (mCursorType)
    {
    case CursorType::CURSOR_TYPE_SHOVEL:
        g->DrawImage(IMAGE_SHOVEL, 10, -30);
        break;

    case CursorType::CURSOR_TYPE_WATERING_CAN:
        if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GOLD_WATERINGCAN])
        {
            g->DrawImage(IMAGE_ZEN_GOLDTOOLRETICLE, -62, -37);
            g->DrawImage(IMAGE_WATERINGCANGOLD, -3, 12);
        }
        else
        {
            g->DrawImage(IMAGE_WATERINGCAN, -3, 12);
        }
        break;

    case CursorType::CURSOR_TYPE_FERTILIZER:
        g->DrawImage(IMAGE_FERTILIZER, -15, 0);
        break;

    case CursorType::CURSOR_TYPE_BUG_SPRAY:
        g->DrawImage(IMAGE_BUG_SPRAY, -9, -1);
        break;

    case CursorType::CURSOR_TYPE_PHONOGRAPH:
        g->DrawImage(IMAGE_PHONOGRAPH, -17, 10);
        break;

    case CursorType::CURSOR_TYPE_CHOCOLATE:
        g->DrawImage(IMAGE_CHOCOLATE, -2, -8);
        break;

    case CursorType::CURSOR_TYPE_GLOVE:
        g->DrawImage(IMAGE_ZEN_GARDENGLOVE, -17, 15);
        break;

    case CursorType::CURSOR_TYPE_MONEY_SIGN:
        g->DrawImage(IMAGE_ZEN_MONEYSIGN, -17, -10);
        break;

    case CursorType::CURSOR_TYPE_TREE_FOOD:
        g->DrawImage(IMAGE_TREEFOOD, -15, 0);
        break;

    case CursorType::CURSOR_TYPE_WHEEELBARROW:
    {
        PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
        if (aPottedPlant)
        {
            g->DrawImage(IMAGE_ZEN_WHEELBARROW, -20, -23);
            if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
            {
                mApp->mZenGarden->DrawPottedPlant(g, 10.0f, -35.0f, aPottedPlant, 0.6f, true);
            }
            else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
            {
                mApp->mZenGarden->DrawPottedPlant(g, 15.0f, -25.0f, aPottedPlant, 0.5f, true);
            }
            else
            {
                mApp->mZenGarden->DrawPottedPlant(g, 21.0f, -15.0f, aPottedPlant, 0.4f, true);
            }
        }
        else
        {
            g->DrawImage(IMAGE_ZEN_WHEELBARROW, -20, -30);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE:
    {
        if (mApp->mGameMode == GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            Plant* aPlant = mBoard->mPlants.DataArrayGet((unsigned int)mGlovePlantID);
            PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[aPlant->mPottedPlantIndex];
            if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM){
                mApp->mZenGarden->DrawPottedPlant(g, -10.0f, -10.0f, aPottedPlant, 1.0f, false);
            }
            else{
                mApp->mZenGarden->DrawPottedPlant(g, -22.0f, -38.0f, aPottedPlant, 1.0f, true);
            }
        }
        else if (mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE) {
            Plant* aPlant = mBoard->mPlants.DataArrayGet((unsigned int)mGlovePlantID);
            if (aPlant)
                Plant::DrawSeedType(g, aPlant->mSeedType, SeedType::SEED_NONE, DrawVariation::VARIATION_NORMAL, -22.0f, 0);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW:
    {
        PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
        if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
        {
            mApp->mZenGarden->DrawPottedPlant(g, -10.0f, -10.0f, aPottedPlant, 1.0f, false);
        }
        else
        {
            mApp->mZenGarden->DrawPottedPlant(g, -22.0f, -38.0f, aPottedPlant, 1.0f, true);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_BANK:
    case CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN:
    case CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR:
    {
        float aOffsetX = -10.0f;
        float aOffsetY = PlantDrawHeightOffset(mBoard, nullptr, mType, -1, -1) - 10.0f;
        if (Plant::IsFlying(mType) || mType == SeedType::SEED_GRAVEBUSTER)
        {
            aOffsetY += 30.0f;
        }
        aOffsetY -= 15.0f;
        if (mApp->IsIZombieLevel())
        {
            aOffsetX -= 55.0f;
            aOffsetY -= 70.0f;
        }

        Plant::DrawSeedType(g, mType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, aOffsetY);
        break;
    }
    
    case CursorType::CURSOR_TYPE_HAMMER:
        mApp->ReanimationGet(mReanimCursorID)->Draw(g);
        break;

    case CursorType::CURSOR_TYPE_COBCANNON_TARGET:
    {
        HitResult aHitResult;
        mBoard->MouseHitTest(mBoard->mPrevMouseX, mBoard->mPrevMouseY, &aHitResult);
        if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_NONE && mBoard->mPrevMouseY >= 80)
        {
            g->DrawImageCel(IMAGE_COBCANNON_TARGET, -11, 7, 0);
        }

        break;
    }
    }

    if (mBoard && mApp && mBoard->mMaxPlayers > 1)
        g->DrawImageF(IMAGE_P1_TEXT, 7.5f, 7.5f);
}

//0x438D50
CursorPreview::CursorPreview()
{
    mX = 0;
    mY = 0;
    mGridX = 0;
    mGridY = 0;
    mVisible = false;
    mWidth = 80;
    mHeight = 80;
}

//0x438DA0
void CursorPreview::Update()
{
    if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->IsInShovelTutorial())
    {
        mVisible = false;
        return;
    }

    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    int aMouseX = mApp->mWidgetManager->mLastMouseX - BOARD_ADDITIONAL_WIDTH;
    int aMouseY = mApp->mWidgetManager->mLastMouseY - BOARD_OFFSET_Y;
    mGridX = mBoard->PlantingPixelToGridX(aMouseX, aMouseY, aSeedType);
    mGridY = mBoard->PlantingPixelToGridY(aMouseX, aMouseY, aSeedType);
    if (mGridX >= 0 && mGridX < MAX_GRID_SIZE_X && mGridY >= 0 && mGridY <= MAX_GRID_SIZE_Y)
    {
        bool aShow = false;
        if (mBoard->IsPlantInCursor() && mBoard->CanPlantAt(mGridX, mGridY, aSeedType) == PlantingReason::PLANTING_OK)
        {
            aShow = true;
        }
        else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW)
        {
            if (mApp->mZenGarden->GetPottedPlantInWheelbarrow() && mBoard->CanPlantAt(mGridX, mGridY, aSeedType) == PlantingReason::PLANTING_OK)
            {
                aShow = true;
            }
        }

        if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && mGridX > 4 && aSeedType != SEED_CHERRYHOVERBOMB) {
            aShow = false;
        }

        if (aShow)
        {
            mX = mBoard->GridToPixelX(mGridX, mGridY);
            mY = mBoard->GridToPixelY(mGridX, mGridY);
            mVisible = true;
            return;
        }
    }

    mVisible = false;
}

//0x438EB0
void CursorPreview::Draw(Graphics* g)
{
    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    if (aSeedType == SeedType::SEED_NONE)
        return;

    g->SetColorizeImages(true);
    g->SetColor(Color(255, 255, 255, 100));

    PottedPlant* aPottedPlant = nullptr;

    if (mApp->mGameMode == GAMEMODE_CHALLENGE_ZEN_GARDEN) {

        if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW || mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW)
        {
            aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
        }
        else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
        {
            aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mBoard->mPlants.DataArrayGet((unsigned int)mBoard->mCursorObject->mGlovePlantID)->mPottedPlantIndex];
        }
    }

    if (aPottedPlant){
        bool aDrawPot = true;
        if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
        {
            aDrawPot = false;
        }
        mApp->mZenGarden->DrawPottedPlant(g, 0.0f, 0.0f, aPottedPlant, 1.0f, aDrawPot);
    }
    else
    {
        float aOffsetX, aOffsetY;
        if (mApp->IsIZombieLevel())
        {
            float aHeight = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, mGridY);
            if (aSeedType == SeedType::SEED_ZOMBIE_GARGANTUAR)
            {
                aHeight -= 30.0f;
            }

            aOffsetY = aHeight - 78.0f;
            aOffsetX = -49.0f;
        }
        else
        {
            aOffsetY = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, mGridY);
            aOffsetX = 0.0f;
        }

        Plant::DrawSeedType(g, mBoard->mCursorObject->mType, mBoard->mCursorObject->mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, aOffsetY);

        /*g->SetColorizeImages(true);
        g->mColor = Color(255, 255, 255, 128);
        int posX = mBoard->GridToPixelX(mGridX, mGridY);
        int posY = mBoard->GridToPixelY(mGridX, mGridY);
        int gridHeight = (mBoard->StageHasRoof() || mBoard->StageHas6Rows() ? 85 : 100);
        g->FillRect(-mX + mBoard->GridToPixelX(0, mGridY), -mY + posY, 765 - LAWN_XMIN, 100);
        g->FillRect(-mX + posX, -mY + mBoard->GridToPixelY(mGridX, 0), 85, gridHeight * (mBoard->StageHas6Rows() ? 6 : 5));*/

        /*Plant* mPreviewPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int)mBoard->mPlayers[0]->mPreviewPlantID);
        if (mPreviewPlant)
        {
            SeedType seedType = mBoard->mSeedBank->mSeedPackets[mBoard->mPlayers[0]->mSeedBankIndex].mPacketType;
            SeedType imitaterType = mBoard->mSeedBank->mSeedPackets[mBoard->mPlayers[0]->mSeedBankIndex].mImitaterType;

            if (mBoard->mPlayers[0]->mGlovePlantID != PLANTID_NULL) {
                Plant* aPlant = mBoard->mExtraPlants.DataArrayTryToGet(mBoard->mPlayers[0]->mGlovePlantID);
                if (aPlant) {
                    seedType = aPlant->mSeedType;
                    imitaterType = aPlant->mImitaterType;
                }
            }

            if (seedType == SEED_IMITATER) {
                SeedType prevSeedType = seedType;
                seedType = imitaterType;
                imitaterType = prevSeedType;
            }

            int	gridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
            int gridY = mBoard->PixelToGridYKeepOnBoard(mX, mY);

            if (mPreviewPlant->mSeedType != mBoard->mCursorObject->mType || mPreviewPlant->mImitaterType != mBoard->mCursorObject->mImitaterType) {

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
                bool canTakeSuns = mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(mBoard->mSeedBank->mSeedPackets[mBoard->mPlayers[0]->mSeedBankIndex].mPacketType, SeedType::SEED_NONE));
                bool canUseSeed = mBoard->mSeedBank->mSeedPackets[mBoard->mPlayers[0]->mSeedBankIndex].mActive;
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

                plantG.DrawImage(mBoard->mPreviewImage, -mX, -mY);
            }
        }
        */
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN){
        for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
        {
            if (y != mGridY && mBoard->CanPlantAt(mGridX, y, aSeedType) == PlantingReason::PLANTING_OK)
            {
                float aOffsetY = 85.0f * (y - mGridY) + PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, y);
                Plant::DrawSeedType(g, mBoard->mCursorObject->mType, mBoard->mCursorObject->mImitaterType, DrawVariation::VARIATION_NORMAL, 0.0f, aOffsetY);
            }
        }
    }

    g->SetColorizeImages(false);
}
