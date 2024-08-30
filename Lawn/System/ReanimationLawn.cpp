#include "../Plant.h"
#include "../Zombie.h"
#include "../../LawnApp.h"
#include "ReanimationLawn.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../SexyAppFramework/Color.h"
#include "../../Sexy.TodLib/Reanimator.h"
#include "../../SexyAppFramework/MemoryImage.h"

#include "../../Resources.h"
#include "../../Sexy.TodLib/Attachment.h"

//0x46EF00
void ReanimatorCache::UpdateReanimationForVariation(Reanimation* theReanim, DrawVariation theDrawVariation)
{
	if (theDrawVariation >= DrawVariation::VARIATION_MARIGOLD_WHITE && theDrawVariation <= DrawVariation::VARIATION_MARIGOLD_LIGHT_GREEN)
	{
		int aVariationIndex = (int)theDrawVariation - (int)DrawVariation::VARIATION_MARIGOLD_WHITE;
		Color MARIGOLD_VARIATIONS[] = {
			Color(255, 255, 255),
			Color(230, 30, 195),
			Color(250, 125, 5),
			Color(255, 145, 215),
			Color(160, 255, 245),
			Color(230, 30, 30),
			Color(5, 130, 255),
			Color(195, 55, 235),
			Color(235, 210, 255),
			Color(255, 245, 55),
			Color(180, 255, 105)
		};

		TOD_ASSERT(aVariationIndex >= 0 && aVariationIndex < LENGTH(MARIGOLD_VARIATIONS));
		theReanim->GetTrackInstanceByName("Marigold_petals")->mTrackColor = MARIGOLD_VARIATIONS[aVariationIndex];
	}
	else
	{
		switch (theDrawVariation)
		{
		case DrawVariation::VARIATION_IMITATER:
			theReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_WASHED_OUT;
			break;
		case DrawVariation::VARIATION_IMITATER_LESS:
			theReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;
			break;
		case DrawVariation::VARIATION_ZEN_GARDEN:
			theReanim->SetFramesForLayer("anim_zengarden");
			break;
		case DrawVariation::VARIATION_ZEN_GARDEN_WATER:
			theReanim->SetFramesForLayer("anim_waterplants");
			break;
		case DrawVariation::VARIATION_AQUARIUM:
			theReanim->SetFramesForLayer("anim_idle_aquarium");
			break;
		case DrawVariation::VARIATION_SPROUT_NO_FLOWER:
			theReanim->SetFramesForLayer("anim_idle_noflower");
			break;
		default:
			TOD_ASSERT(false);
			break;
		}
	}
}

//0x46F100
void ReanimatorCache::DrawReanimatorFrame(Graphics* g, float thePosX, float thePosY, ReanimationType theReanimationType, const char* theTrackName, DrawVariation theDrawVariation)
{
	Reanimation aReanim;
	aReanim.ReanimationInitializeType(thePosX, thePosY, theReanimationType);

	if (theTrackName != nullptr && aReanim.TrackExists(theTrackName))
	{
		aReanim.SetFramesForLayer(theTrackName);
	}
	if (theReanimationType == ReanimationType::REANIM_KERNELPULT)
	{
		aReanim.AssignRenderGroupToTrack("Cornpult_butter", RENDER_GROUP_HIDDEN);
	}
	else if (theReanimationType == ReanimationType::REANIM_SUNFLOWER || theReanimationType == ReanimationType::REANIM_AMPLIFLOWER)
	{
		aReanim.mAnimTime = 0.15f;
	}
	aReanim.AssignRenderGroupToTrack("anim_waterline", RENDER_GROUP_HIDDEN);

	if (g->GetColorizeImages())
	{
		aReanim.mColorOverride = g->GetColor();
	}
	aReanim.OverrideScale(g->mScaleX, g->mScaleY);
	
	if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
	{
		UpdateReanimationForVariation(&aReanim, theDrawVariation);
	}

	aReanim.Draw(g);
}

//0x46F280
MemoryImage* ReanimatorCache::MakeBlankMemoryImage(int theWidth, int theHeight)
{
	MemoryImage* aImage = new MemoryImage();

	int aBitsCount = theWidth * theHeight;
	aImage->mBits = new unsigned long[aBitsCount + 1];
	aImage->mWidth = theWidth;
	aImage->mHeight = theHeight;
	aImage->mHasTrans = true;
	aImage->mHasAlpha = true;
	memset(aImage->mBits, 0, aBitsCount * 4);
	aImage->mBits[aBitsCount] = Sexy::MEMORYCHECK_ID;
	return aImage;
}

void ReanimatorCache::GetPlantImageSize(SeedType theSeedType, int& theOffsetX, int& theOffsetY, int& theWidth, int& theHeight)
{
	theOffsetX = -20;
	theOffsetY = -20;
	theWidth = 120;
	theHeight = 120;

	if (theSeedType == SeedType::SEED_TALLNUT)
	{
		theOffsetY = -40;
		theHeight += 40;
	}
	else if (theSeedType == SeedType::SEED_MELONPULT || theSeedType == SeedType::SEED_WINTERMELON)
	{
		theOffsetX = -40;
		theWidth += 40;
	}
	else if (theSeedType == SeedType::SEED_COBCANNON)
	{
		theWidth += 80;
	}
}

//0x46F330
MemoryImage* ReanimatorCache::MakeCachedMowerFrame(LawnMowerType theMowerType)
{
	MemoryImage* aImage;

	switch (theMowerType)
	{
	case LawnMowerType::LAWNMOWER_LAWN:
	{
		aImage = MakeBlankMemoryImage(90, 100);
		Graphics aMemoryGraphics(aImage);
		aMemoryGraphics.SetLinearBlend(true);
		aMemoryGraphics.mScaleX = 0.85f;
		aMemoryGraphics.mScaleY = 0.85f;
		DrawReanimatorFrame(&aMemoryGraphics, 10.0f, 0.0f, ReanimationType::REANIM_LAWNMOWER, "anim_normal", DrawVariation::VARIATION_NORMAL);
		break;
	}
	case LawnMowerType::LAWNMOWER_POOL:
	{
		aImage = MakeBlankMemoryImage(90, 100);
		Graphics aMemoryGraphics(aImage);
		aMemoryGraphics.SetLinearBlend(true);
		aMemoryGraphics.mScaleX = 0.8f;
		aMemoryGraphics.mScaleY = 0.8f;
		DrawReanimatorFrame(&aMemoryGraphics, 10.0f, 25.0f, ReanimationType::REANIM_POOL_CLEANER, nullptr, DrawVariation::VARIATION_NORMAL);
		break;
	}
	case LawnMowerType::LAWNMOWER_ROOF:
	{
		aImage = MakeBlankMemoryImage(90, 100);
		Graphics aMemoryGraphics(aImage);
		aMemoryGraphics.SetLinearBlend(true);
		aMemoryGraphics.mScaleX = 0.85f;
		aMemoryGraphics.mScaleY = 0.85f;
		DrawReanimatorFrame(&aMemoryGraphics, 10.0f, 0.0f, ReanimationType::REANIM_ROOF_CLEANER, nullptr, DrawVariation::VARIATION_NORMAL);
		break;
	}
	case LawnMowerType::LAWNMOWER_SUPER_MOWER:
	{
		aImage = MakeBlankMemoryImage(90, 100);
		Graphics aMemoryGraphics(aImage);
		aMemoryGraphics.SetLinearBlend(true);
		aMemoryGraphics.mScaleX = 0.85f;
		aMemoryGraphics.mScaleY = 0.85f;
		DrawReanimatorFrame(&aMemoryGraphics, 10.0f, 0.0f, ReanimationType::REANIM_LAWNMOWER, "anim_tricked", DrawVariation::VARIATION_NORMAL);
		break;
	}
	default:
		TOD_ASSERT(false);
		break;
	}

	return aImage;
}

//0x46F550
MemoryImage* ReanimatorCache::MakeCachedPlantFrame(SeedType theSeedType, DrawVariation theDrawVariation)
{
	int aOffsetX, aOffsetY, aWidth, aHeight;
	GetPlantImageSize(theSeedType, aOffsetX, aOffsetY, aWidth, aHeight);
	MemoryImage* aMemoryImage = MakeBlankMemoryImage(aWidth, aHeight);
	Graphics aMemoryGraphics(aMemoryImage);
	aMemoryGraphics.SetLinearBlend(true);

	PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
	//TOD_ASSERT(aPlantDef.mReanimationType != ReanimationType::REANIM_NONE);

	if (theSeedType == SeedType::SEED_POTATOMINE)
	{
		aMemoryGraphics.mScaleX *= 0.85f;
		aMemoryGraphics.mScaleY *= 0.85f;
		DrawReanimatorFrame(&aMemoryGraphics, -(int)(aOffsetX - 12.0f), -(int)(aOffsetY - 12.0f), aPlantDef.mReanimationType, "anim_armed", theDrawVariation);
	}
	else if (theSeedType == SeedType::SEED_INSTANTCOFFEE)
	{
		aMemoryGraphics.mScaleX *= 0.8f;
		aMemoryGraphics.mScaleY *= 0.8f;
		DrawReanimatorFrame(&aMemoryGraphics, -(int)(aOffsetX - 12.0f), -(int)(aOffsetY - 12.0f), aPlantDef.mReanimationType, "anim_idle", theDrawVariation);
	}
	else if (theSeedType == SeedType::SEED_EXPLODE_O_NUT)
	{
		//aMemoryGraphics.SetColorizeImages(true);
		//aMemoryGraphics.SetColor(Color(255, 64, 64));
		DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_idle", theDrawVariation);
	}
	else if (theSeedType == SeedType::SEED_WATERPOT)
	{
		DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_waterpot", theDrawVariation);
	}
	else if (theSeedType == SeedType::SEED_FLYINGPOT)
	{
		DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_airraid", theDrawVariation);
	}
	else if (theSeedType == SeedType::SEED_FIRESHROOM) {
		Reanimation aReanim;
		aReanim.ReanimationInitializeType(-aOffsetX, -aOffsetY, aPlantDef.mReanimationType);

		if (aReanim.TrackExists("anim_full_idle"))
		{
			aReanim.SetFramesForLayer("anim_full_idle");
		}

		aReanim.AssignRenderGroupToTrack("anim_waterline", RENDER_GROUP_HIDDEN);

		if (aMemoryGraphics.GetColorizeImages())
		{
			aReanim.mColorOverride = aMemoryGraphics.GetColor();
		}
		aReanim.OverrideScale(aMemoryGraphics.mScaleX, aMemoryGraphics.mScaleY);

		if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
		{
			UpdateReanimationForVariation(&aReanim, theDrawVariation);
		}

		aReanim.Draw(&aMemoryGraphics);
	}
	else if (theSeedType == SeedType::SEED_STINGER) {
		Reanimation aReanim;
		aReanim.ReanimationInitializeType(-aOffsetX, -aOffsetY + 5, aPlantDef.mReanimationType);

		if (aReanim.TrackExists("anim_full_idle"))
		{
			aReanim.SetFramesForLayer("anim_full_idle");
		}

		aReanim.AssignRenderGroupToTrack("anim_waterline", RENDER_GROUP_HIDDEN);

		if (aMemoryGraphics.GetColorizeImages())
		{
			aReanim.mColorOverride = aMemoryGraphics.GetColor();
		}
		aReanim.OverrideScale(aMemoryGraphics.mScaleX, aMemoryGraphics.mScaleY);

		if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
		{
			UpdateReanimationForVariation(&aReanim, theDrawVariation);
		}

		aReanim.Draw(&aMemoryGraphics);
	}
	else if (theSeedType == SeedType::SEED_SHOOTINGSTAR) {
		Reanimation aReanim;
		aReanim.ReanimationInitializeType(-aOffsetX, -aOffsetY, aPlantDef.mReanimationType);

		if (aReanim.TrackExists("anim_full_idle"))
		{
			aReanim.SetFramesForLayer("anim_full_idle");
		}

		aReanim.AssignRenderGroupToTrack("anim_waterline", RENDER_GROUP_HIDDEN);

		if (aMemoryGraphics.GetColorizeImages())
		{
			aReanim.mColorOverride = aMemoryGraphics.GetColor();
		}
		aReanim.OverrideScale(aMemoryGraphics.mScaleX, aMemoryGraphics.mScaleY);

		if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
		{
			UpdateReanimationForVariation(&aReanim, theDrawVariation);
		}

		aReanim.Draw(&aMemoryGraphics);
	}
	else if (theSeedType == SeedType::SEED_CHERRYHOVERBOMB) {
		Reanimation aReanim;
		aReanim.ReanimationInitializeType(-aOffsetX, -aOffsetY, aPlantDef.mReanimationType);
		aReanim.AssignRenderGroupToPrefix("Cherrybomb_right1", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToPrefix("CherryBomb_rightstem", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToPrefix("CherryBomb_rightmouth1", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToPrefix("CherryBomb_righteye21", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToPrefix("CherryBomb_righteye11", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToPrefix("CherryBomb_right3", RENDER_GROUP_HIDDEN);


		if (aReanim.TrackExists("anim_idle"))
		{
			aReanim.SetFramesForLayer("anim_idle");
		}

		aReanim.AssignRenderGroupToTrack("anim_waterline", RENDER_GROUP_HIDDEN);

		if (aMemoryGraphics.GetColorizeImages())
		{
			aReanim.mColorOverride = aMemoryGraphics.GetColor();
		}
		aReanim.OverrideScale(aMemoryGraphics.mScaleX, aMemoryGraphics.mScaleY);

		if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
		{
			UpdateReanimationForVariation(&aReanim, theDrawVariation);
		}

		aReanim.Draw(&aMemoryGraphics);
	}
	else
	{
		DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_idle", theDrawVariation);

		if (theSeedType == SeedType::SEED_PEASHOOTER || theSeedType == SeedType::SEED_SNOWPEA || theSeedType == SeedType::SEED_REPEATER ||
			theSeedType == SeedType::SEED_LEFTPEATER || theSeedType == SeedType::SEED_GATLINGPEA || theSeedType == SeedType::SEED_SLINGPEA)
		{
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_head_idle", theDrawVariation);
		}
		else if (theSeedType == SeedType::SEED_SPLITPEA)
		{
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_head_idle", theDrawVariation);
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_splitpea_idle", theDrawVariation);
		}
		else if (theSeedType == SeedType::SEED_THREEPEATER)
		{
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_head_idle1", theDrawVariation);
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_head_idle3", theDrawVariation);
			DrawReanimatorFrame(&aMemoryGraphics, -aOffsetX, -aOffsetY, aPlantDef.mReanimationType, "anim_head_idle2", theDrawVariation);
		}
	}

	return aMemoryImage;
}

//0x46F8A0
MemoryImage* ReanimatorCache::MakeCachedZombieFrame(ZombieType theZombieType) 
{
	MemoryImage* aMemoryImage = MakeBlankMemoryImage(200, 210);
	Graphics aMemoryGraphics(aMemoryImage);
	aMemoryGraphics.SetLinearBlend(true);

	ZombieType aUseZombieType = theZombieType;
	if (theZombieType == ZombieType::ZOMBIE_CACHED_POLEVAULTER_WITH_POLE)
	{
		aUseZombieType = ZombieType::ZOMBIE_POLEVAULTER;
	}
	ZombieDefinition& aZombieDef = GetZombieDefinition(aUseZombieType);
	TOD_ASSERT(aZombieDef.mReanimationType != ReanimationType::REANIM_NONE);

	float aPosX = 40.0f, aPosY = 40.0f;
	if (aZombieDef.mReanimationType == ReanimationType::REANIM_ZOMBIE)
	{
		if (theZombieType == ZombieType::ZOMBIE_TALLNUT_HEAD)
		{
			aPosX += 15.0f;
			aPosY += 15.0f;
		}

		Reanimation aReanim;
		aReanim.ReanimationInitializeType(aPosX, aPosY, aZombieDef.mReanimationType);
		if (theZombieType != ZombieType::ZOMBIE_PEA_HEAD && theZombieType != ZombieType::ZOMBIE_GATLING_HEAD || theZombieType != ZombieType::ZOMBIE_SQUASH_HEAD )
			aReanim.SetFramesForLayer("anim_idle");
		else
			aReanim.SetFramesForLayer("anim_walk2");
		
		Zombie::SetupReanimLayers(&aReanim, aUseZombieType);

		switch (theZombieType)
		{
			case ZombieType::ZOMBIE_DOOR:
			{
				aReanim.AssignRenderGroupToTrack("anim_screendoor", RENDER_GROUP_NORMAL);
				break;
			}
			case ZombieType::ZOMBIE_FLAG:
			{
				Reanimation aReanimFlag;
				aReanimFlag.ReanimationInitializeType(aPosX, aPosY, ReanimationType::REANIM_FLAG);
				aReanimFlag.SetFramesForLayer("Zombie_flag");
				aReanimFlag.Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_PEA_HEAD:
			{
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);

				ReanimatorTrackInstance* headTrackInst = aReanim.GetTrackInstanceByName("anim_head1");
				headTrackInst->mImageOverride = IMAGE_BLANK;
				ReanimatorTrackInstance* bodyTrackInst = aReanim.GetTrackInstanceByName("Zombie_body");

				Reanimation aPlantHeadReanim;
				aPlantHeadReanim.ReanimationInitializeType(aPosX + 65, aPosY - 5, ReanimationType::REANIM_PEASHOOTER);
				aPlantHeadReanim.PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
				AttachEffect* aAttachEffect = AttachReanim(bodyTrackInst->mAttachmentID, &aPlantHeadReanim, 0.0f, 0.0f);
				aPlantHeadReanim.mFrameBasePose = 0;
				TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);
				aPlantHeadReanim.Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_WALLNUT_HEAD:
			{
				aReanim.mFrameBasePose = 0;
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);
				aReanim.AssignRenderGroupToPrefix("Zombie_tie", -1);

				ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
				aTrackInstance->mImageOverride = IMAGE_BLANK;
				ReanimatorTrackInstance* aBodyInstance = aReanim.GetTrackInstanceByName("Zombie_body");

				Reanimation* aPlantHead = mApp->AddReanimation(aPosX + 60.0f, aPosY, 0, ReanimationType::REANIM_WALLNUT);
				AttachEffect* aAttachEffect = AttachReanim(aBodyInstance->mAttachmentID, aPlantHead, 0.0f, 0.0f);
				
				aPlantHead->OverrideScale(-0.8f, 0.8);
				aPlantHead->SetFramesForLayer("anim_idle");
				aPlantHead->Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_TALLNUT_HEAD:
			{
				aReanim.mFrameBasePose = 0;
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);
				aReanim.AssignRenderGroupToPrefix("Zombie_tie", -1);

				ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
				aTrackInstance->mImageOverride = IMAGE_BLANK;
				ReanimatorTrackInstance* aBodyInstance = aReanim.GetTrackInstanceByName("Zombie_body");

				Reanimation* aPlantHead = mApp->AddReanimation(aPosX + 61.0f, aPosY, 0, ReanimationType::REANIM_TALLNUT);
				AttachEffect* aAttachEffect = AttachReanim(aBodyInstance->mAttachmentID, aPlantHead, 0.0f, 0.0f);

				aPlantHead->OverrideScale(-0.8f, 0.8f);
				aPlantHead->SetFramesForLayer("anim_head_idle");
				aPlantHead->Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_JALAPENO_HEAD:
			{
				aReanim.mFrameBasePose = 0;
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);
				aReanim.AssignRenderGroupToPrefix("Zombie_tie", -1);

				ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
				aTrackInstance->mImageOverride = IMAGE_BLANK;
				ReanimatorTrackInstance* aBodyInstance = aReanim.GetTrackInstanceByName("Zombie_body");
				
				Reanimation* aPlantHead = mApp->AddReanimation(aPosX + 55.0f, aPosY, 0, ReanimationType::REANIM_JALAPENO);
				AttachEffect* aAttachEffect = AttachReanim(aBodyInstance->mAttachmentID, aPlantHead, 0.0f, 0.0f);

				aPlantHead->OverrideScale(-1.0f, 1.0f);
				aPlantHead->SetFramesForLayer("anim_idle");
				aPlantHead->Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_GATLING_HEAD:
			{
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);

				ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
				aTrackInstance->mImageOverride = IMAGE_BLANK;

				Reanimation* aPlantHead = mApp->AddReanimation(aPosX + 65.0f + 12.5f, aPosY - 5.0f, 0, ReanimationType::REANIM_GATLINGPEA);
				AttachEffect* aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aPlantHead, 0.0f, 0.0f);
				TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);

				aPlantHead->OverrideScale(-1.0f, 1.0f);
				aPlantHead->SetFramesForLayer("anim_head_idle");
				aPlantHead->mFrameBasePose = 0;
				aPlantHead->Draw(&aMemoryGraphics);
				break;
			}
			case ZombieType::ZOMBIE_SQUASH_HEAD:
			{
				aReanim.AssignRenderGroupToPrefix("anim_hair", -1);
				aReanim.AssignRenderGroupToPrefix("anim_head2", -1);

				ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
				aTrackInstance->mImageOverride = IMAGE_BLANK;

				ReanimatorTrackInstance* aBodyInstance = aReanim.GetTrackInstanceByName("Zombie_body");
				Reanimation* aPlantHead = mApp->AddReanimation(aPosX + 62.5, aPosY-15.0f, 0, ReanimationType::REANIM_SQUASH);
				AttachEffect* aAttachEffect = AttachReanim(aBodyInstance->mAttachmentID, aPlantHead, 0.0f, 0.0f);
				TodScaleRotateTransformMatrix(aAttachEffect->mOffset, 55.0f, -15.0f, 0.2f, -0.75f, 0.75f);

				aPlantHead->OverrideScale(-0.75f, 0.75f);
				aPlantHead->SetFramesForLayer("anim_idle");
				aPlantHead->mFrameBasePose = 0;
				aPlantHead->Draw(&aMemoryGraphics);
				break;
			}
		}
	
		aReanim.Draw(&aMemoryGraphics);
	}
	else if (aZombieDef.mReanimationType == ReanimationType::REANIM_BOSS)
	{
		Reanimation aReanim;
		aReanim.ReanimationInitializeType(-524.0f, -88.0f, aZombieDef.mReanimationType);
		aReanim.SetFramesForLayer("anim_head_idle");
		Reanimation aReanimDriver;
		aReanimDriver.ReanimationInitializeType(46.0f, 22.0f, ReanimationType::REANIM_BOSS_DRIVER);
		aReanimDriver.SetFramesForLayer("anim_idle");

		aReanim.Draw(&aMemoryGraphics);
		aReanimDriver.Draw(&aMemoryGraphics);
		aReanim.AssignRenderGroupToTrack("boss_body1", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToTrack("boss_neck", RENDER_GROUP_HIDDEN);
		aReanim.AssignRenderGroupToTrack("boss_head2", RENDER_GROUP_HIDDEN);
		aReanim.Draw(&aMemoryGraphics);
	}
	else
	{
		const char* aTrackName = "anim_idle";
		if (theZombieType == ZombieType::ZOMBIE_POGO)
		{
			aTrackName = "anim_pogo";
		}
		else if (theZombieType == ZombieType::ZOMBIE_CACHED_POLEVAULTER_WITH_POLE)
		{
			aTrackName = "anim_idle";
		}
		else if (theZombieType == ZombieType::ZOMBIE_POLEVAULTER)
		{
			aTrackName = "anim_walk";
		}
		else if (theZombieType == ZombieType::ZOMBIE_GARGANTUAR || theZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
		{
			aPosY = 60.0f;
		}
		

		if (theZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
		{
			Reanimation aReanim;
			aReanim.ReanimationInitializeType(aPosX, aPosY, aZombieDef.mReanimationType);
			aReanim.SetFramesForLayer("anim_idle");
			Zombie::SetupReanimLayers(&aReanim, aUseZombieType);
			ReanimatorTrackInstance* aTrackInstance = aReanim.GetTrackInstanceByName("anim_head1");
			aTrackInstance->mImageOverride = IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE;
			aReanim.Draw(&aMemoryGraphics);
		}
		else if (theZombieType == ZombieType::ZOMBIE_BALLOON)
		{
			Reanimation aReanim;
			aReanim.ReanimationInitializeType(aPosX, aPosY, aZombieDef.mReanimationType);
			Reanimation aPropellerReanim;
			aPropellerReanim.ReanimationInitializeType(aPosX, aPosY, aZombieDef.mReanimationType);
			aPropellerReanim.SetFramesForLayer("Propeller");
			aPropellerReanim.mLoopType = ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME;
			aPropellerReanim.AttachToAnotherReanimation(&aReanim, "hat");

			aReanim.Draw(&aMemoryGraphics);
			aPropellerReanim.Draw(&aMemoryGraphics);
		}
		else if (theZombieType == ZOMBIE_SNORKEL)
		{
			Reanimation aReanim;
			aReanim.ReanimationInitializeType(aPosX, aPosY, aZombieDef.mReanimationType);
			aReanim.AssignRenderGroupToPrefix("Zombie_diver", RENDER_GROUP_HIDDEN);
			aReanim.Draw(&aMemoryGraphics);
		}
		else {
			DrawReanimatorFrame(&aMemoryGraphics, aPosX, aPosY, aZombieDef.mReanimationType, aTrackName, DrawVariation::VARIATION_NORMAL);
		}

	}
	return aMemoryImage;
}

//0x46FDC0
void ReanimatorCache::ReanimatorCacheInitialize()
{
	mApp = (LawnApp*)gSexyAppBase;
	for (int i = 0; i < SeedType::NUM_SEED_TYPES; i++)
		mPlantImages[i] = nullptr;
	for (int i = 0; i < LawnMowerType::NUM_MOWER_TYPES; i++)
		mLawnMowers[i] = nullptr;
	for (int i = 0; i < ZombieType::NUM_ZOMBIE_TYPES; i++)
		mZombieImages[i] = nullptr;
}

//0x46FED0
void ReanimatorCache::ReanimatorCacheDispose()
{
	for (int i = 0; i < SeedType::NUM_SEED_TYPES; i++)
		delete mPlantImages[i];
	while (mImageVariationList.mSize != 0)
	{
		ReanimCacheImageVariation aImageVariation = mImageVariationList.RemoveHead();
		if (aImageVariation.mImage != nullptr)
			delete aImageVariation.mImage;
	}
	for (int i = 0; i < LawnMowerType::NUM_MOWER_TYPES; i++)
		delete mLawnMowers[i];
	for (int i = 0; i < ZombieType::NUM_ZOMBIE_TYPES; i++)
		delete mZombieImages[i];
}


//0x46FFB0
void ReanimatorCache::DrawCachedPlant(Graphics* g, float thePosX, float thePosY, SeedType theSeedType, DrawVariation theDrawVariation)
{
	//TOD_ASSERT(theSeedType >= 0 && theSeedType < SeedType::NUM_SEED_TYPES);

	MemoryImage* aImage = nullptr;
	if (theDrawVariation == DrawVariation::VARIATION_ZEN_GARDEN_WATER)
	{
		aImage = mPlantImages[(int)theSeedType];
		if (aImage == nullptr)
		{
			aImage = MakeCachedPlantFrame(theSeedType, DrawVariation::VARIATION_ZEN_GARDEN_WATER);
			mPlantImages[(int)theSeedType] = aImage;
		}
	}
	else if (theDrawVariation != DrawVariation::VARIATION_NORMAL)
	{
		for (TodListNode<ReanimCacheImageVariation>* aNode = mImageVariationList.mHead; aNode != nullptr; aNode = aNode->mNext)
		{
			ReanimCacheImageVariation& aImageVariation = aNode->mValue;
			if (aImageVariation.mSeedType == theSeedType && aImageVariation.mDrawVariation == theDrawVariation)
			{
				aImage = aImageVariation.mImage;
				break;
			}
		}

		if (aImage == nullptr)
		{
			aImage = MakeCachedPlantFrame(theSeedType, theDrawVariation);
			ReanimCacheImageVariation aNewImageVariation;
			aNewImageVariation.mSeedType = theSeedType;
			aNewImageVariation.mDrawVariation = theDrawVariation;
			aNewImageVariation.mImage = aImage;
			mImageVariationList.AddHead(aNewImageVariation);
		}
	}
	else
	{
		aImage = mPlantImages[(int)theSeedType];
		if (aImage == nullptr)
		{
			aImage = MakeCachedPlantFrame(theSeedType, DrawVariation::VARIATION_NORMAL);
			mPlantImages[(int)theSeedType] = aImage;
		}
	}

	int aOffsetX, aOffsetY, aWidth, aHeight;
	GetPlantImageSize(theSeedType, aOffsetX, aOffsetY, aWidth, aHeight);

	if (!mApp->Is3dAccel() && g->mScaleX == 1.0f && g->mScaleY == 1.0f)
		g->DrawImage(aImage, thePosX + aOffsetX, thePosY + aOffsetY,aWidth, aHeight );
	else
		TodDrawImageScaledF(g, aImage, thePosX + (aOffsetX * g->mScaleX), thePosY + (aOffsetY * g->mScaleY), g->mScaleX, g->mScaleY );
}

//0x470110
void ReanimatorCache::DrawCachedMower(Graphics* g, float thePosX, float thePosY, LawnMowerType theMowerType)
{
	TOD_ASSERT(theMowerType >= 0 && theMowerType < LawnMowerType::NUM_MOWER_TYPES);
	
	if (mLawnMowers[(int)theMowerType] == nullptr)
		mLawnMowers[(int)theMowerType] = MakeCachedMowerFrame(theMowerType);
	TodDrawImageScaledF(g, mLawnMowers[(int)theMowerType], thePosX - 20.0f, thePosY, g->mScaleX, g->mScaleY);
}

//0x470170
void ReanimatorCache::DrawCachedZombie(Graphics* g, float thePosX, float thePosY, ZombieType theZombieType)
{
	TOD_ASSERT(theZombieType >= 0 && theZombieType < ZombieType::NUM_CACHED_ZOMBIE_TYPES);
	
	if (mZombieImages[(int)theZombieType] == nullptr)
		mZombieImages[(int)theZombieType] = MakeCachedZombieFrame(theZombieType);
	TodDrawImageScaledF(g, mZombieImages[(int)theZombieType], thePosX, thePosY, g->mScaleX, g->mScaleY);
}
