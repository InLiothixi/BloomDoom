#include "Board.h"
#include "Plant.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "Projectile.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/TodParticle.h"

ProjectileDefinition gProjectileDefinition[] = {  //0x69F1C0
	{ ProjectileType::PROJECTILE_PEA,           0,  20  },
	{ ProjectileType::PROJECTILE_SNOWPEA,       0,  20  },
	{ ProjectileType::PROJECTILE_CABBAGE,       0,  40  },
	{ ProjectileType::PROJECTILE_MELON,         0,  80  },
	{ ProjectileType::PROJECTILE_PUFF,          0,  20  },
	{ ProjectileType::PROJECTILE_WINTERMELON,   0,  80  },
	{ ProjectileType::PROJECTILE_FIREBALL,      0,  40  },
	{ ProjectileType::PROJECTILE_STAR,          0,  20  },
	{ ProjectileType::PROJECTILE_SPIKE,         0,  20  },
	{ ProjectileType::PROJECTILE_BASKETBALL,    0,  75  },
	{ ProjectileType::PROJECTILE_KERNEL,        0,  20  },
	{ ProjectileType::PROJECTILE_COBBIG,        0,  300 },
	{ ProjectileType::PROJECTILE_BUTTER,        0,  40  },
	{ ProjectileType::PROJECTILE_FROSTCABBAGE,  0,  40  },
	{ ProjectileType::PROJECTILE_SLINGPEA,      0,  20  },
	{ ProjectileType::PROJECTILE_SPORE,         0,  40	},
	{ ProjectileType::PROJECTILE_PLASMABALL,    0,  80  },
	{ ProjectileType::PROJECTILE_BOMBBERRY,		0,  10  },
	{ ProjectileType::PROJECTILE_STARCICLE,		0,  20  },
	{ ProjectileType::PROJECTILE_MAGNIFYING_GRASS,0,  270	},
	{ ProjectileType::PROJECTILE_SHOOTING_STAR, 0,  20  },
	{ ProjectileType::PROJECTILE_CACTUS_SPIKE, 0,  10  },
	{ ProjectileType::PROJECTILE_SEAPUFF,          0,  20  },
};

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
	AttachmentDie(mAttachmentID);
}

//0x46C730
void Projectile::ProjectileInitialize(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType)
{
	int aGridX = mBoard->PixelToGridXKeepOnBoard(theX, theY);
	mProjectileType = theProjectileType;
	mPosX = theX;
	mPosY = theY;
	mPosZ = 0.0f;
	mVelX = 0.0f;
	mVelY = 0.0f;
	mVelZ = 0.0f;
	mAccZ = 0.0f;
	mShadowY = mBoard->GridToPixelY(aGridX, theRow) + 67.0f;
	mHitTorchwoodGridX = -1;
	mMotionType = ProjectileMotion::MOTION_STRAIGHT;
	mFrame = 0;
	mNumFrames = 1;
	mRow = theRow;
	mCobTargetX = 0.0f;
	mDamageRangeFlags = 0;
	mDead = false;
	mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
	mCobTargetRow = 0;
	mTargetZombieID = ZombieID::ZOMBIEID_NULL;
	mOnHighGround = mBoard->mGridSquareType[aGridX][theRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
	if (mBoard->StageHasRoof())
	{
		mShadowY -= 12.0f;
	}
	mRenderOrder = theRenderOrder;
	mRotation = 0.0f;
	mRotationSpeed = 0.0f;
	mWidth = 40;
	mHeight = 40;
	mProjectileAge = 0;
	mClickBackoffCounter = 0;
	mAnimTicksPerFrame = 0;
	mBackwards = mEnemy = false;
	mPlantParentID = PLANTID_NULL;
	mZombieParentID = ZOMBIEID_NULL;
	mHitCount = 0;

	if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_BUTTER || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE 
		|| mProjectileType == ProjectileType::PROJECTILE_SLINGPEA || mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		mRotation = -7 * PI / 25;  // DEG_TO_RAD(-50.4f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);

		if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE) {
			TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
			AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
		}else if (mProjectileType == ProjectileType::PROJECTILE_SPORE){
			TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
			AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mRotation = -2 * PI / 5;  // DEG_TO_RAD(-72.0f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);

		if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON){
			TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
			AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mRotation = 0.0f;
		mRotationSpeed = RandRangeFloat(-0.2f, -0.08f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
	}
	else if (mProjectileType == PROJECTILE_SHOOTING_STAR) 
	{
		float aOffsetX = 0;
		float aOffsetY = 0;
		Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PROJECTILE_SHOOTINGSTAR);
		if (mBackwards)
		{
			aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
			aOffsetX += 80.0f;
		}

		aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
		aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		TOD_ASSERT();
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mWidth = IMAGE_REANIM_COBCANNON_COB->GetWidth();
		mHeight = IMAGE_REANIM_COBCANNON_COB->GetHeight();
		mRotation = PI / 2;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		aParticle->OverrideColor(nullptr, Color(0, 255, 0));
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		mRotation = RandRangeFloat(0.0f, 2 * PI);
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
	{
		mShadowY += 15.0f;
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
		if (Rand(2) == 0)
		{
			mRotationSpeed = -mRotationSpeed;
		}
	}

	mAnimCounter = 0;
	mX = (int)mPosX;
	mY = (int)mPosY;
}

//0x46CAA0
Plant* Projectile::FindCollisionTargetPlant()
{
	Rect aProjectileRect = GetProjectileRect();

	for (int i = 0; i < mBoard->mMaxPlayers; i++) {
		PlantID playerPlantID = mBoard->mPlayers[i]->mPlayerPlantID;

		if (playerPlantID != PLANTID_NULL) {
			Plant* mPlayerPlant = mBoard->mExtraPlants.DataArrayTryToGet((unsigned int)playerPlantID);
			if (mPlayerPlant) {
				if (mEnemy == mPlayerPlant->mEnemy)
					continue;

				if (mPlayerPlant->mRow != mRow)
					continue;

				if (mProjectileType == PROJECTILE_PEA) {
					if (mPlayerPlant->mSeedType == SeedType::SEED_PUFFSHROOM ||
						mPlayerPlant->mSeedType == SeedType::SEED_ICEBERG_LETTUCE ||
						mPlayerPlant->mSeedType == SeedType::SEED_SUNSHROOM ||
						mPlayerPlant->mSeedType == SeedType::SEED_POTATOMINE ||
						mPlayerPlant->mSeedType == SeedType::SEED_SPIKEWEED ||
						mPlayerPlant->mSeedType == SeedType::SEED_SPIKEROCK ||
						mPlayerPlant->mSeedType == SeedType::SEED_LILYPAD ||
						mPlayerPlant->mSeedType == SeedType::SEED_FLOWERPOT ||
						mPlayerPlant->mSeedType == SeedType::SEED_WATERPOT ||
						mPlayerPlant->mSeedType == SeedType::SEED_FLYINGPOT ||
						mPlayerPlant->mSeedType == SeedType::SEED_SEEDROOT)  // 僵尸豌豆不能击中低矮植物
						continue;
				}

				Rect aPlantRect = mPlayerPlant->GetPlantRect();

				/*if (mPlayerPlant->mEnemy) {
					aPlantRect.mX += aPlantRect.mWidth;
				}

				if (mBackwards || mPlayerPlant->mEnemy) {
					aProjectileRect.mX -= (aProjectileRect.mX - mX) * 2 - aProjectileRect.mWidth;
				}*/

				if (GetRectOverlap(aProjectileRect, aPlantRect) > 8)
				{
					return mPlayerPlant;
				}
			}
		}
	}

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant)){
		if (mEnemy == aPlant->mEnemy)
			continue;

		if (aPlant->mState == STATE_BURNED)
			continue;

		if (aPlant->mRow != mRow)
			continue;

		if (mProjectileType == PROJECTILE_PEA){
			if (aPlant->mSeedType == SeedType::SEED_PUFFSHROOM ||
				aPlant->mSeedType == SeedType::SEED_SUNSHROOM ||
				aPlant->mSeedType == SeedType::SEED_POTATOMINE ||
				aPlant->mSeedType == SeedType::SEED_SPIKEWEED ||
				aPlant->mSeedType == SeedType::SEED_SPIKEROCK ||
				aPlant->mSeedType == SeedType::SEED_LILYPAD ||
				aPlant->mSeedType == SeedType::SEED_SEEDROOT)  // 僵尸豌豆不能击中低矮植物
				continue;
		}

		Rect aPlantRect = aPlant->GetPlantRect();

		aProjectileRect = GetProjectileRect();

		if (aPlant->mEnemy) {
			aProjectileRect.mX -= (aProjectileRect.mX - mX) * 2 + aProjectileRect.mWidth;
		}

		if (mMotionType == MOTION_LOBBED && aPlant->mEnemy){
			aPlantRect.mX -= aPlantRect.mWidth;
		}

		if (GetRectOverlap(aProjectileRect, aPlantRect) > 8)
		{
			if (mProjectileType == PROJECTILE_BASKETBALL){
				//if (mApp->mGameMode == GAMEMODE_CHALLENGE_OPEN_FIRE && mBoard->mPlayerPlant1->mRow == mRow)		return mBoard->mPlayerPlant1;
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
			}
			else {
				Plant* aTarget = mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_EATING_ORDER);

				if (mProjectileType == PROJECTILE_PEA && Plant::IsFlying(aPlant->mSeedType)) {
					if (aTarget->mSeedType == SeedType::SEED_PUFFSHROOM ||
						aTarget->mSeedType == SeedType::SEED_SUNSHROOM ||
						aTarget->mSeedType == SeedType::SEED_POTATOMINE ||
						aTarget->mSeedType == SeedType::SEED_SPIKEWEED ||
						aTarget->mSeedType == SeedType::SEED_SPIKEROCK ||
						aTarget->mSeedType == SeedType::SEED_LILYPAD ||
						aTarget->mSeedType == SeedType::SEED_SEEDROOT)
					{
						aTarget = aPlant;
					}
				}

				return aTarget;
			}
		}
	}

	return nullptr;
}

//0x46CC30
bool Projectile::PeaAboutToHitTorchwood()
{
	if (mMotionType != ProjectileMotion::MOTION_STRAIGHT)
		return false;

	if (mProjectileType != ProjectileType::PROJECTILE_PEA && mProjectileType != ProjectileType::PROJECTILE_SNOWPEA)
		return false;

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_TORCHWOOD && aPlant->mRow == mRow && !aPlant->NotOnGround() && mHitTorchwoodGridX != aPlant->mPlantCol)
		{
			Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			Rect aProjectileRect = GetProjectileRect();
			aProjectileRect.mX += 40;

			if (GetRectOverlap(aPlantAttackRect, aProjectileRect) > 10)
			{
				return true;
			}
		}
	}

	return false;
}

bool Projectile::PeaAboutToHitPlasmawood()
{
	if (mMotionType != ProjectileMotion::MOTION_STRAIGHT)
		return false;

	if (mProjectileType != ProjectileType::PROJECTILE_PEA && mProjectileType != ProjectileType::PROJECTILE_SNOWPEA)
		return false;

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_PLASMAWOOD && aPlant->mRow == mRow && !aPlant->NotOnGround() && mHitTorchwoodGridX != aPlant->mPlantCol)
		{
			Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			Rect aProjectileRect = GetProjectileRect();
			aProjectileRect.mX += 40;

			if (GetRectOverlap(aPlantAttackRect, aProjectileRect) > 10)
			{
				return true;
			}
		}
	}

	return false;
}

//0x46CD40
Zombie* Projectile::FindCollisionTarget()
{
	if (PeaAboutToHitTorchwood() || PeaAboutToHitPlasmawood())  // “卡火炬”的原理，这段代码在两版内测版中均不存在
		return nullptr;

	if (mProjectileType == PROJECTILE_BOMBBERRY)
		return nullptr;

	Rect aProjectileRect = GetProjectileRect();
	Zombie* aBestZombie = nullptr;
	int aMinX = 0;

	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == mRow) && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			if (aZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL && mPosZ >= 45.0f)
			{
				continue;
			}

			if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE) && mProjectileAge < 25 && mVelX >= 0.0f && aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
			{
				continue;
			}

			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) > 0)
			{
				if (aBestZombie == nullptr || aZombie->mX < aMinX)
				{
					aBestZombie = aZombie;
					aMinX = aZombie->mX;
				}
			}
		}
	}

	return aBestZombie;
}


//0x46CE80
void Projectile::CheckForCollision()
{
	if (mMotionType == ProjectileMotion::MOTION_PUFF && mProjectileAge >= 75)
	{
		Die();
		return;
	}

	if (mPosX > WIDE_BOARD_WIDTH || mPosX + mWidth < 0.0f)
	{
		Die();
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aProjectileRect = GetProjectileRect();
			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) >= 0 && mPosY > aZombieRect.mY&& mPosY < aZombieRect.mY + aZombieRect.mHeight)
			{
				DoImpact(aZombie);
			}
		}
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE) && (mPosY > 600.0f || mPosY < 0.0f))
	{
		Die();
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE) && mShadowY - mPosY > 90.0f)
	{
		return;
	}

	/*
	if ((mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_STAR) && mShadowY - mPosY > 90.0f)
	{
		return;
	}
	*/


	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		return;
	}
	
	//if (mApp->mGameMode == GAMEMODE_CHALLENGE_PLANTS_VS_PLANTS)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant) {
			const ProjectileDefinition& aProjectileDef = GetProjectileDef();
			aPlant->mPlantHealth -= aProjectileDef.mDamage;
			aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);

			if ((mProjectileType == PROJECTILE_SNOWPEA || mProjectileType == PROJECTILE_STARCICLE) && !aPlant->CanBeChilled()) {
				aPlant->ApplyChill(false);

			}

			/*if (aPlant->mSeedType == SEED_HYPNOSHROOM && !aPlant->mIsAsleep) {
				if (mPlantParentID != PLANTID_NULL) {
					aPlant->Die();
					Plant* mPlant = mBoard->mPlants.DataArrayGet((unsigned int)mPlantParentID);
					if (mPlant) {
						mApp->PlaySample(SOUND_MINDCONTROLLED);
						mPlant->mMindControlled = !mPlant->mMindControlled;
						mPlant->mEnemy = mPlant->mMindControlled;
					}
				}

				if (mZombieParentID != ZOMBIEID_NULL) {
					Zombie* mZombie = mBoard->mZombies.DataArrayGet((unsigned int)mZombieParentID);
					aPlant->Die();
					if (mZombie) {
						mApp->PlaySample(SOUND_MINDCONTROLLED);
						mZombie->mMindControlled = !mZombie->mMindControlled;
					}
				}
			}*/

			PlayImpactSound(nullptr);
			bool isSplashDMG = mProjectileType && aPlant && 
				mProjectileType == ProjectileType::PROJECTILE_MELON ||
				mProjectileType == ProjectileType::PROJECTILE_WINTERMELON ||
				mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
				mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
				mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE;

			if (isSplashDMG) {
				/*if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL) && theZombie) {
					theZombie->RemoveColdEffects();
				}*/

				DoSplashDamage(aPlant);
			}

			float aLastPosX = mPosX - mVelX;
			float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
			ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
			float aSplatPosX = mPosX + 12.0f;
			float aSplatPosY = mPosY + 12.0f;

			if (mProjectileType == ProjectileType::PROJECTILE_MELON)
			{
				mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
			{
				mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
			{
				int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
				mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
				mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);

				mApp->PlaySample(SOUND_DOOMSHROOM);
				mBoard->ShakeBoard(3, -4);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PEA)
			{
				aSplatPosX -= 15.0f;
				aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
			{
				if (isSplashDMG)
				{
					aSplatPosX -= 15.0f;
					aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX, mPosY - 23.0f + 337, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_PLASMA_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
			{
				aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
			{
				aSplatPosX -= 20.0f;
				aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SLINGPEA)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
			{
				if (isSplashDMG)
				{
					if (mBackwards)
						aSplatPosX = aLastPosX + 38.0f;
					else
						aSplatPosX = aLastPosX - 38.0f;
					aSplatPosY = aLastPosY + 23.0f;
					aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 20.0f;
				else
					aSplatPosX = aLastPosX - 20.0f;
				aSplatPosY = aLastPosY + 63.0f;
				aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

				/*if (theZombie)
				{
					theZombie->ApplyButter();
				}*/

				aPlant->mButteredCounter = 400;
			}

			if (aEffect != ParticleEffect::PARTICLE_NONE) {
				TodParticleSystem* aParticle = mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);

				if (mProjectileType == PROJECTILE_SEAPUFF)
				{
					aParticle->OverrideColor(nullptr, Color(0, 255, 0));
				}
			}

			Die();

			if (aPlant->mPlantHealth <= 0) {
				if (aPlant->mSeedType == SEED_EXPLODE_O_NUT)	aPlant->DoSpecial();
			}
		}
	}
	//else 
	{
		Zombie* aZombie = FindCollisionTarget();
		if (aZombie)
		{
			if (aZombie->mOnHighGround && CantHitHighGround())
			{
				return;
			}

			DoImpact(aZombie);
		}
	}
}

//0x46D090
bool Projectile::CantHitHighGround()
{
	if (mBackwards || mMotionType == ProjectileMotion::MOTION_HOMING || mProjectileType == PROJECTILE_SHOOTING_STAR || mIgnoreGround)
		return false;

	return (
		mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_STAR ||
		mProjectileType == ProjectileType::PROJECTILE_STARCICLE ||
		mProjectileType == ProjectileType::PROJECTILE_PUFF ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL || 
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS ||
		mProjectileType == ProjectileType::PROJECTILE_SEAPUFF ||
		mProjectileType == ProjectileType::PROJECTILE_CACTUS_SPIKE
		) && !mOnHighGround;
}

//0x46D0D0
void Projectile::CheckForHighGround()
{
	if (mBackwards || mIgnoreGround || mProjectileType == PROJECTILE_SHOOTING_STAR)
		return;

	float aShadowDelta = mShadowY - mPosY;
	
	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_COBBIG ||
		mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS ||
		mProjectileType == ProjectileType::PROJECTILE_CACTUS_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
	{
		if (aShadowDelta < 28.0f)
		{
			DoImpact(nullptr);
			return;
		}
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SEAPUFF) && aShadowDelta < 0.0f)
	{
		DoImpact(nullptr);
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_STAR|| mProjectileType == ProjectileType::PROJECTILE_STARCICLE) && aShadowDelta < 23.0f)
	{
		DoImpact(nullptr);
		return;
	}

	if (CantHitHighGround())
	{
		int aGridX = mBoard->PixelToGridXKeepOnBoard(mPosX + 30, mPosY);
		if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
		{
			DoImpact(nullptr);
		}
	}
}

//0x46D1F0
bool Projectile::IsSplashDamage(Zombie* theZombie)
{
	if (mProjectileType && theZombie && theZombie->IsFireResistant() && mProjectileType != ProjectileType::PROJECTILE_SHOOTING_STAR)
		return false;

	return 
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE;
}

//0x46D230
unsigned int Projectile::GetDamageFlags(Zombie* theZombie)
{
	unsigned int aDamageFlags = 0U;

	if (IsSplashDamage(theZombie))
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_LOBBED || mBackwards && !mEnemy)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR && mVelX < 0.0f)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}

	if (mProjectileType == ProjectileType::PROJECTILE_STARCICLE || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
	}

	return aDamageFlags;
}

//0x46D2B0
bool Projectile::IsZombieHitBySplash(Zombie* theZombie)
{
	Rect aProjectileRect = GetProjectileRect();
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
		|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		aProjectileRect.mWidth = 100;
	}

	int aRowDeviation = theZombie->mRow - mRow;
	Rect aZombieRect = theZombie->GetZombieRect();
	if (theZombie->IsFireResistant() && (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL))
	{
		return false;
	}

	if (theZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
	{
		aRowDeviation = 0;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
		|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		if (aRowDeviation != 0)
		{
			return false;
		}
	}
	else if (aRowDeviation > 1 || aRowDeviation < -1)
	{
		return false;
	}

	return theZombie->EffectedByDamage((unsigned int)mDamageRangeFlags) && GetRectOverlap(aProjectileRect, aZombieRect) >= 0;
}

//0x46D390
void Projectile::DoSplashDamage(void* theTarget)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	Zombie* theZombie = (Zombie*)theTarget;
	Plant* thePlant = (Plant*)theTarget;

	int aZombiesGetSplashed = 0;

	Plant* aPlant = nullptr;
	Zombie* aZombie = nullptr;

	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS)
	{
		while (mBoard->IteratePlants(aPlant)) {
			if (aPlant != thePlant){
				Rect aProjectileRect = GetProjectileRect();
				if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
					|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE) {
					aProjectileRect.mWidth = 100;
				}

				int aRowDeviation = aPlant->mRow - mRow;
				Rect aPlantRect = aPlant->GetPlantRect();
				/*if (aPlant->mEnemy) {
					aPlantRect.mX += aProjectileRect.mWidth;
				}*/

				bool hitBySplash = GetRectOverlap(aProjectileRect, aPlantRect) >= 0;

				if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
					|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
				{
					if (aRowDeviation != 0)
					{
						hitBySplash = false;
					}
				}
				else if (aRowDeviation > 1 || aRowDeviation < -1)
				{
					hitBySplash = false;
				}

				if (hitBySplash) aZombiesGetSplashed++;
			}
		}
	}
	else{
		while (mBoard->IterateZombies(aZombie)){
			if (aZombie != theZombie && IsZombieHitBySplash(aZombie)){
				aZombiesGetSplashed++;
			}
		}
	}

	int aOriginalDamage = aProjectileDef.mDamage;
	int aSplashDamage = aProjectileDef.mDamage / 3;
	int aMaxSplashDamageAmount = aSplashDamage * 7;
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
		 || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	int aSplashDamageAmount = aSplashDamage * aZombiesGetSplashed;
	if (aSplashDamageAmount > aMaxSplashDamageAmount)
	{
		//aSplashDamage *= aMaxSplashDamageAmount / aSplashDamage;
		aSplashDamage = aOriginalDamage * aMaxSplashDamageAmount / (aSplashDamageAmount * 3);
		aSplashDamage = max(aSplashDamage, 1);
	}

	aZombie = nullptr;
	aPlant = nullptr;

	if (mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS){
		while (mBoard->IteratePlants(aPlant)) {
			Rect aProjectileRect = GetProjectileRect();
			if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
				|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE) {
				aProjectileRect.mWidth = 100;
			}

			int aRowDeviation = aPlant->mRow - mRow;
			Rect aPlantRect = aPlant->GetPlantRect();
			/*if (aPlant->mEnemy) {
				aPlantRect.mX += aProjectileRect.mWidth;
			}*/

			bool hitBySplash = GetRectOverlap(aProjectileRect, aPlantRect) >= 0;

			if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR
				|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
			{
				if (aRowDeviation != 0)
				{
					hitBySplash = false;
				}
			}
			else if (aRowDeviation > 1 || aRowDeviation < -1)
			{
				hitBySplash = false;
			}

			if (hitBySplash) {
				aPlant->mPlantHealth -= (aPlant == thePlant ? aOriginalDamage : aSplashDamage);
				aPlant->mEatenFlashCountdown = 25;

				if (mProjectileType == PROJECTILE_WINTERMELON && !aPlant->CanBeChilled()) {
					aPlant->ApplyChill(false);
				}

				if (aPlant->mSeedType == SEED_HYPNOSHROOM && !aPlant->mIsAsleep && mPlantParentID != PLANTID_NULL && aPlant == thePlant) {
					aPlant->Die();
					Plant* mPlant = mBoard->mPlants.DataArrayGet((unsigned int)mPlantParentID);
					if (mPlant) {
						mApp->PlaySample(SOUND_MINDCONTROLLED);
						mPlant->mMindControlled = !mPlant->mMindControlled;
						mPlant->mEnemy = mPlant->mMindControlled;
					}
				}
			}
			
		}
	}
	else {
		while (mBoard->IterateZombies(aZombie))
		{
			if (IsZombieHitBySplash(aZombie))
			{
				unsigned int aDamageFlags = GetDamageFlags(aZombie);
				if (aZombie == theZombie)
				{
					aZombie->TakeDamage(aOriginalDamage, aDamageFlags);
				}
				else
				{
					unsigned int aSplashFlag = aDamageFlags;

					if (mProjectileType == PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
						aSplashDamage = 0;

					aZombie->TakeDamage(aSplashDamage, aSplashFlag);
				}
			}
		}
	}

}

//0x46D490
void Projectile::UpdateLobMotion()
{
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG && mPosZ < -700.0f)
	{
		mVelZ = 8.0f;
		mRow = mCobTargetRow;
		mPosX = mCobTargetX;
		int aCobTargetCol = mBoard->PixelToGridXKeepOnBoard(mCobTargetX, 0);
		mPosY = mBoard->GridToPixelY(aCobTargetCol, mCobTargetRow);
		mShadowY = mPosY + 67.0f;
		mRotation = -PI / 2;
	}

	mVelZ += mAccZ * 2;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
		mVelZ += mAccZ;

	mPosX += mVelX;

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_WINDNESDAY)
	{
		mPosX -= abs(mVelX) * 0.5f;
	}

	mPosY += mVelY;
	mPosZ += mVelZ;

	bool isRising = mVelZ < 0.0f;

	if (isRising && (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_COBBIG))
	{
		return;
	}

	if (mProjectileAge > 20)
	{
		if (isRising)
		{
			return;
		}

		float aMinCollisionZ = 0.0f;
		if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
		{
			aMinCollisionZ = -32.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL )
		{
			aMinCollisionZ = 60.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
		{
			aMinCollisionZ = -35.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_KERNEL 
			|| mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE || mProjectileType == ProjectileType::PROJECTILE_SLINGPEA 
			|| mProjectileType == PROJECTILE_SPORE)
		{
			aMinCollisionZ = -30.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
		{
			aMinCollisionZ = -60.0f;
		}
		if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
		{
			aMinCollisionZ += 40.0f;
		}

		if (mProjectileType == PROJECTILE_SHOOTING_STAR) {
			aMinCollisionZ += 307;
		}

		if (mPosZ <= aMinCollisionZ)
		{
			return;
		}
	}

	Plant* aPlant = nullptr;
	Zombie* aZombie = nullptr;
	if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS){
		aPlant = FindCollisionTargetPlant();
	}
	else{
		aZombie = FindCollisionTarget();
	}

	float aGroundZ = 80.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aGroundZ = -40.0f;
	}
	bool hitGround = mPosZ > aGroundZ;

	if (aZombie == nullptr && aPlant == nullptr && !hitGround)
	{
		return;
	}

	if (aPlant)
	{
		Plant* aUmbrellaPlant = nullptr;
		Plant* mPlant = nullptr;
		while (mBoard->IteratePlants(mPlant) && mProjectileType != PROJECTILE_BUTTER)
		{
			if (mPlant->mEnemy == mEnemy || mPlant->mState == STATE_BURNED || mPlant->mButteredCounter > 0 || mPlant->mIceTrapCounter > 0)	continue;

			if (mPlant->mSeedType == SeedType::SEED_UMBRELLA && !mPlant->NotOnGround() && GridInRange(aPlant->mPlantCol, aPlant->mRow, mPlant->mPlantCol, mPlant->mRow, 1, 1))
			{
				aUmbrellaPlant = mPlant;
				break;
			}
		}

		if (aUmbrellaPlant)
		{
			if (aUmbrellaPlant->mState == PlantState::STATE_UMBRELLA_REFLECTING)
			{
				mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
				int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
				TodParticleSystem* mParticle = mApp->AddTodParticle(mPosX + 20.0f, mPosY + 20.0f, aRenderPosition, ParticleEffect::PARTICLE_UMBRELLA_REFLECT);

				Image* aImage;
				float aScale = 1.0f;
				if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
				{
					aImage = IMAGE_REANIM_COBCANNON_COB;
					aScale = 0.9f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_SLINGPEA)
				{
					aImage = IMAGE_PROJECTILEPEA;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
				{
					aImage = IMAGE_PROJECTILESNOWPEA;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == PROJECTILE_SHOOTING_STAR)
				{
					aImage = nullptr;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE || mProjectileType == ProjectileType::PROJECTILE_CACTUS_SPIKE)
				{
					aImage = IMAGE_PROJECTILECACTUS;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
				{
					aImage = IMAGE_PROJECTILE_STAR;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
				{
					aImage = IMAGE_PUFFSHROOM_PUFF1;
					aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
				{
					aImage = IMAGE_SEASHROOM_PUFF1;
					aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
				{
					aImage = IMAGE_SPORESHROOM_SPORE;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
				{
					aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
					aScale = 1.0f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
				{
					aImage = IMAGE_REANIM_ICEBERGLETTUCE_LETTUCE;
					aScale = 1.0f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
				{
					aImage = IMAGE_REANIM_CORNPULT_KERNAL;
					aScale = 0.95f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
				{
					aImage = IMAGE_REANIM_CORNPULT_BUTTER;
					aScale = 0.8f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
				{
					aImage = IMAGE_REANIM_MELONPULT_MELON;
					aScale = 1.0f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
				{
					aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
					aScale = 1.0f;
				}
				else if (mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS)
				{
					aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
				}

				if (mProjectileType != PROJECTILE_BASKETBALL) {
					mParticle->OverrideImage(nullptr, aImage);
					mParticle->OverrideScale(nullptr, aScale);
				}

				Die();
			}
			else if (aUmbrellaPlant->mState != PlantState::STATE_UMBRELLA_TRIGGERED)
			{
				mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
				aUmbrellaPlant->DoSpecial();
			}
		}
		else
		{
			PlayImpactSound(nullptr);
			bool isSplashDMG = mProjectileType == ProjectileType::PROJECTILE_MELON ||
				mProjectileType == ProjectileType::PROJECTILE_WINTERMELON ||
				mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
				mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR ||
				mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
				mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE;

			if (isSplashDMG) {
				/*if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL) && theZombie) {
					theZombie->RemoveColdEffects();
				}*/

				DoSplashDamage(aPlant);
			}
			else {
				aPlant->mPlantHealth -= GetProjectileDef().mDamage;
				aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);
				if (mProjectileType == PROJECTILE_FROSTCABBAGE && !aPlant->CanBeChilled()) {
					aPlant->ApplyChill(false);
				}
			}

			float aLastPosX = mPosX - mVelX;
			float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
			ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
			float aSplatPosX = mPosX + 12.0f;
			float aSplatPosY = mPosY + 12.0f;

			if (mProjectileType == ProjectileType::PROJECTILE_MELON)
			{
				mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
			{
				mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
			{
				int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
				mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
				mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
				
				mApp->PlaySample(SOUND_DOOMSHROOM);
				mBoard->ShakeBoard(3, -4);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PEA)
			{
				aSplatPosX -= 15.0f;
				aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
			{
				aSplatPosX -= 15.0f;
				aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX, mPosY - 23.0f + 337, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
			{
				if (isSplashDMG)
				{
					Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_PLASMA_FIRE);
					aFireReanim->mAnimTime = 0.25f;
					aFireReanim->mAnimRate = 24.0f;
					aFireReanim->OverrideScale(0.7f, 0.4f);
				}
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
			{
				aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
			{
				aSplatPosX -= 20.0f;
				aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_SLINGPEA)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 38.0f ;
				else
					aSplatPosX = aLastPosX - 38.0f;
				aSplatPosY = aLastPosY + 23.0f;
				aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
			{
				if (mBackwards)
					aSplatPosX = aLastPosX + 20.0f;
				else
					aSplatPosX = aLastPosX - 20.0f;
				aSplatPosY = aLastPosY + 63.0f;
				aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

				/*if (theZombie)
				{
					theZombie->ApplyButter();
				}*/

				aPlant->mButteredCounter = 400;
			}

			if (aEffect != ParticleEffect::PARTICLE_NONE) {
				mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);
			}

			Die();

			if (aPlant->mPlantHealth <= 0) {
				if (aPlant->mSeedType == SEED_EXPLODE_O_NUT)	aPlant->DoSpecial();
			}
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mBoard->KillAllZombiesInRadius(mRow, mPosX + 80, mPosY + 40, 115, 1, true, mDamageRangeFlags);
		DoImpact(nullptr);
	}
	else
	{
		DoImpact(aZombie);
	}
}

//0x46D890
void Projectile::UpdateNormalMotion()
{
	
	 if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);

		/*if (mPlantParent && (!aZombie || aZombie->mDead || aZombie->mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || aZombie->mZombiePhase == PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == PHASE_BALLOON_POPPING))
		{
			int targetRow = mPlantParent->mRow;
			this->mRow = targetRow;
			if (mPlantParent->mApp && mBoard)
			{
				Zombie* zom = nullptr;
				zom = mPlantParent->FindTargetZombie(targetRow, WEAPON_PRIMARY);
				if (zom)
				{
					this->mTargetZombieID = mBoard->ZombieGetID(zom);
					aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
				}
			}
		}*/

	
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			SexyVector2 aTargetCenter(aZombie->ZombieTargetLeadX(0.0f), aZombieRect.mY + aZombieRect.mHeight / 2);
			SexyVector2 aProjectileCenter(mPosX + mWidth / 2, mPosY + mHeight / 2);
			SexyVector2 aToTarget = (aTargetCenter - aProjectileCenter).Normalize();
			SexyVector2 aMotion(mVelX, mVelY);

			aMotion += aToTarget * (0.001f * mProjectileAge);
			aMotion = aMotion.Normalize();
			aMotion *= 2.0f;

			mVelX = aMotion.x;
			mVelY = aMotion.y;
			mRotation = -atan2(mVelY, mVelX);
		}

		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR)
	{
		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;

		if (mVelY != 0.0f)
		{
			mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX += 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		if (mVelZ < 0.0f)
		{
			mVelZ += 0.002f;
			mVelZ = min(mVelZ, 0.0f);
			mPosY += mVelZ;
			mRotation = 0.3f - 0.7f * mVelZ * PI * 0.25f;
		}
		mPosX += 0.4f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX -= 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_THREEPEATER)
	{
		if (mBackwards)
			mPosX -= 3.33f;
		else
			mPosX += 3.33f;
		mPosY += mVelY;
		mVelY *= 0.97f;
		mShadowY += mVelY;
	}
	else
	{
		 if (mBackwards)
			 mPosX -= 3.33f;
		 else
			 mPosX += 3.33f;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
		{
			mVelZ += 0.004f;
		}
		else
		{
			mVelZ += 0.2f;
		}

		mPosY += mVelZ;
	}

	if (mApp->mGameMode == GAMEMODE_CHALLENGE_WINDNESDAY)
	{
		int vel = mVelX;

		if (mMotionType != MOTION_HOMING)
			vel = 3.33f;

		mPosX -= vel * 0.5f;
	}

	CheckForCollision();

	if (mMotionType != MOTION_HOMING && !mIgnoreGround)
		CheckForHighGround();
	
	if (mMotionType == MOTION_HOMING)
		mShadowY = mPosY + 67.0f;

	if (mIgnoreGround && mProjectileType == PROJECTILE_PEA)
		mShadowY = mPosY + 50.0f;
}

//0x46DC70
void Projectile::UpdateMotion()
{
	if (mAnimTicksPerFrame > 0)
	{
		mAnimCounter = (mAnimCounter + 1) % (mNumFrames * mAnimTicksPerFrame);
		mFrame = mAnimCounter / mAnimTicksPerFrame;
	}

	int aOldRow = mRow;
	float aOldY = mBoard->GetPosYBasedOnRow(mPosX, mRow);

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		UpdateLobMotion();
	}
	else
	{
		UpdateNormalMotion();
	}

	float aSlopeHeightChange = mBoard->GetPosYBasedOnRow(mPosX, aOldRow) - aOldY;

//#ifdef DO_FIX_BUGS
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aSlopeHeightChange = 0.0f;  // 修复“上界之风”
	}
//#endif
	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		mPosY += aSlopeHeightChange;
	}

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		mPosY += aSlopeHeightChange;
		mPosZ -= aSlopeHeightChange;
	}

	if (mProjectileType == PROJECTILE_SLINGPEA)
		mShadowY = mPosY + 67.0f;

	if (mProjectileType == PROJECTILE_SHOOTING_STAR)
		mShadowY = mPosY + 337;

	mShadowY += aSlopeHeightChange;
	mX = (int)mPosX;
	mY = (int)(mPosY + mPosZ);
	
}

//0x46DD30
void Projectile::PlayImpactSound(Zombie* theZombie)
{
	bool aPlayHelmSound = true;
	bool aPlaySplatSound = true;
	if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mApp->PlayFoley(FoleyType::FOLEY_KERNEL_SPLAT);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
		aPlaySplatSound = false;
	}
	else if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR) && IsSplashDamage(theZombie))
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->PlayFoley(FoleyType::FOLEY_MELONIMPACT);
		aPlaySplatSound = false;
	}

	if (aPlayHelmSound && theZombie)
	{
		if (theZombie->mHelmType == HELMTYPE_PAIL || theZombie->mHelmType == HELMTYPE_DIVER)
		{
			mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
			aPlaySplatSound = false;
		}
		else if (theZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || theZombie->mHelmType == HELMTYPE_DIGGER || theZombie->mHelmType == HELMTYPE_FOOTBALL)
		{
			mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
		}
	}

	if (aPlaySplatSound)
	{
		mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
	}
}

//0x46E000
void Projectile::DoImpact(Zombie* theZombie)
{
	bool dieNextImpact = true;

	if (mProjectileType == PROJECTILE_CACTUS_SPIKE && theZombie) 
	{
		bool wasHitBefore = false;
		for (int i = 0; i < sizeof(mHitZombies) / sizeof(mHitZombies[0]); i++) {
			if (mHitZombies[i] == theZombie) {
				wasHitBefore = true;
				break;
			}
		}

		if (wasHitBefore)
			return;

		mHitZombies[mHitCount++] = theZombie;
		dieNextImpact = mHitCount == 4;
	}

	PlayImpactSound(theZombie);

	if (IsSplashDamage(theZombie))
	{
		if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR) && theZombie)
		{
			theZombie->RemoveColdEffects();
		}

		DoSplashDamage(theZombie);
	}
	else if (theZombie)
	{
		unsigned int aDamageFlags = GetDamageFlags(theZombie);
		int daDamage = GetProjectileDef().mDamage;
		theZombie->TakeDamage(daDamage, aDamageFlags);
	}

	float aLastPosX = mPosX - mVelX;
	float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
	ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
	float aSplatPosX = mPosX + 12.0f;
	float aSplatPosY = mPosY + 12.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
		mApp->PlaySample(SOUND_DOOMSHROOM);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		if (IsSplashDamage(theZombie))
		{
			aSplatPosX -= 15.0f;
			aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX, mPosY - 23.0f + 337, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_PLASMA_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
	{
		aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
	{
		aSplatPosX -= 20.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SLINGPEA)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_KERNEL_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

		if (theZombie)
		{
			theZombie->ApplyButter();
		}
	}

	if (aEffect != ParticleEffect::PARTICLE_NONE)
	{
		if (theZombie)
		{
			float aPosX = aSplatPosX + 52.0f - theZombie->mX;
			float aPosY = aSplatPosY - theZombie->mY;
			if (theZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL || theZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
			{
				aPosY += 60.0f;
			}
			if (mBackwards)
			{
				aPosX -= 80.0f;
			}
			else if (mPosX > theZombie->mX + 40 && mMotionType != ProjectileMotion::MOTION_LOBBED)
			{
				aPosX -= 60.0f;
			}

			aPosY = ClampFloat(aPosY, 20.0f, 100.0f);
			theZombie->AddAttachedParticle(aPosX, aPosY, aEffect);
		}
		else
		{
			mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);
		}
	}

	if (mProjectileType == PROJECTILE_CACTUS_SPIKE && !dieNextImpact)
	{
		return;
	}

	if (mProjectileType == PROJECTILE_BOMBBERRY)
	{
		mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_POWIE);
		Zombie* aZombie = nullptr;
		while (mBoard->IterateZombies(aZombie))
		{
			if (aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
			{
				Rect aZombieRect = aZombie->GetZombieRect();

				if (aZombie->mRow - mRow <= 1 && GetCircleRectOverlap(aSplatPosX, aSplatPosY, 115, aZombieRect))
				{
					aZombie->TakeDamage(GetProjectileDef().mDamage, GetDamageFlags(aZombie));
				}
			}
		}
	}

	Die();
}

//0x46E460
void Projectile::Update()
{
	mProjectileAge++;
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->ShouldRunUpsellBoard())
		return;

	int aTime = 20;
	if (mProjectileType == ProjectileType::PROJECTILE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
		mProjectileType == ProjectileType::PROJECTILE_CABBAGE ||
		mProjectileType == ProjectileType::PROJECTILE_SPORE ||
		mProjectileType == ProjectileType::PROJECTILE_SLINGPEA ||
		mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE ||
		mProjectileType == ProjectileType::PROJECTILE_SHOOTING_STAR ||
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_KERNEL || 
		mProjectileType == ProjectileType::PROJECTILE_BUTTER || 
		mProjectileType == ProjectileType::PROJECTILE_COBBIG ||  
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS ||
		mProjectileType == ProjectileType::PROJECTILE_CACTUS_SPIKE)
	{
		aTime = 0;
	}

	if (mProjectileAge > aTime)
	{
		mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);
	}

	if (mClickBackoffCounter > 0)
	{
		mClickBackoffCounter--;
	}

	mRotation += mRotationSpeed;

	UpdateMotion();
	AttachmentUpdateAndMove(mAttachmentID, mPosX, mPosY + mPosZ);
	
}

//0x46E540
void Projectile::Draw(Graphics* g)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	Image* aImage;
	float aScale = 1.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aImage = IMAGE_REANIM_COBCANNON_COB;
		aScale = 0.9f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_SLINGPEA)
	{
		aImage = IMAGE_PROJECTILEPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_STARCICLE)
	{
		aImage = IMAGE_PROJECTILESNOWPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL || 
		mProjectileType == PROJECTILE_SHOOTING_STAR)
	{
		aImage = nullptr;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE || mProjectileType == ProjectileType::PROJECTILE_CACTUS_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		aImage = IMAGE_PROJECTILE_STAR;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SEAPUFF)
	{
		aImage = IMAGE_SEASHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		aImage = IMAGE_SPORESHROOM_SPORE;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		aImage = IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL;
		aScale = 1.1f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_BOMBBERRY)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FROSTCABBAGE)
	{
		aImage = IMAGE_REANIM_ICEBERGLETTUCE_LETTUCE;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		aImage = IMAGE_REANIM_CORNPULT_KERNAL;
		aScale = 0.95f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aImage = IMAGE_REANIM_CORNPULT_BUTTER;
		aScale = 0.8f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		aImage = IMAGE_REANIM_MELONPULT_MELON;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS)
	{
		aImage = IMAGE_REANIM_MELONPULT_MELON;
	}
	else
	{
		TOD_ASSERT();
	}

	bool aMirror = false;
	if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		aMirror = true;
	}

	if (aImage)
	{
		TOD_ASSERT(aProjectileDef.mImageRow < aImage->mNumRows);
		TOD_ASSERT(mFrame < aImage->mNumCols);

		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		Rect aSrcRect(aCelWidth * mFrame, aCelHeight * aProjectileDef.mImageRow, aCelWidth, aCelHeight);
		
		if (FloatApproxEqual(mRotation, 0.0f) && FloatApproxEqual(aScale, 1.0f))
		{
			Rect aDestRect(0, 0, aCelWidth, aCelHeight);
			if (mBackwards)
			{
				aDestRect.mX += aDestRect.mWidth;
				aDestRect.mWidth = -aDestRect.mWidth;
			}
			g->DrawImageMirror(aImage, aDestRect, aSrcRect, aMirror);
		}
		else
		{
			float aOffsetX = mPosX + aCelWidth * 0.5f;
			float aOffsetY = mPosY + mPosZ + aCelHeight * 0.5f;
			SexyTransform2D aTransform;
			float scaleX = aScale;
			if (mBackwards)
			{
				aOffsetX += aCelWidth;
				scaleX = -scaleX;
			}
			TodScaleRotateTransformMatrix(aTransform, aOffsetX + mBoard->mX, aOffsetY + mBoard->mY, mRotation, scaleX, aScale);
			TodBltMatrix(g, aImage, aTransform, g->mClipRect, Color::White, g->mDrawMode, aSrcRect);
		}
		
	}

	if (mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
	{
		Graphics theParticleGraphics(*g);
		MakeParentGraphicsFrame(&theParticleGraphics);
		AttachmentDraw(mAttachmentID, &theParticleGraphics, false);
	}
}

//0x46E8C0
void Projectile::DrawShadow(Graphics* g)
{
	int aCelCol = 0;
	float aScale = 1.0f;
	float aStretch = 1.0f;
	float aOffsetX = mPosX - mX;
	float aOffsetY = mPosY - mY;

	int aGridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
	bool isHighGround = false;
	if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		isHighGround = true;
	}
	if (mOnHighGround && !isHighGround)
	{
		aOffsetY += HIGH_GROUND_HEIGHT;
	}
	else if (!mOnHighGround && isHighGround)
	{
		aOffsetY -= HIGH_GROUND_HEIGHT;
	}

	if (mBoard->StageIsNight())
	{
		aCelCol = 1;
	}

	switch (mProjectileType)
	{
	case ProjectileType::PROJECTILE_SLINGPEA:
	case ProjectileType::PROJECTILE_PEA:
		aOffsetX += 3.0f;
		break;

	case ProjectileType::PROJECTILE_SNOWPEA:
		aOffsetX += -1.0f;
		aScale = 1.3f;
		break;

	case ProjectileType::PROJECTILE_STARCICLE:
	case ProjectileType::PROJECTILE_STAR:
		aOffsetX += 7.0f;
		break;

	case ProjectileType::PROJECTILE_FROSTCABBAGE:
	case ProjectileType::PROJECTILE_SPORE:
	case ProjectileType::PROJECTILE_SHOOTING_STAR:
	case ProjectileType::PROJECTILE_CABBAGE:
	case ProjectileType::PROJECTILE_KERNEL:
	case ProjectileType::PROJECTILE_BUTTER:
	case ProjectileType::PROJECTILE_MELON:
	case ProjectileType::PROJECTILE_WINTERMELON:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		aScale = 1.6f;
		break;

	case ProjectileType::PROJECTILE_SEAPUFF:
	case ProjectileType::PROJECTILE_PUFF:
		return;
		
	case ProjectileType::PROJECTILE_COBBIG:
		aScale = 1.0f;
		aStretch = 3.0f;
		aOffsetX += 57.0f;
		break;

	case ProjectileType::PROJECTILE_PLASMABALL:
	case ProjectileType::PROJECTILE_FIREBALL:
		aScale = 1.4f;
		break;
	}

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		float mZ = -mPosZ;
		float aHeight = ClampFloat(mZ, 0.0f, 200.0f);
		aScale *= 200.0f / (aHeight + 200.0f);

		if (mProjectileType == PROJECTILE_SHOOTING_STAR) {
			aScale = TodAnimateCurveFloatTime(0, 337, mPosZ, 0, aScale, CURVE_LINEAR);
		}
	}

	TodDrawImageCelScaledF(g, IMAGE_PEA_SHADOWS, aOffsetX, (mShadowY - mPosY + aOffsetY), aCelCol, 0, aScale * aStretch, aScale);
}

//0x46EB20
void Projectile::Die()
{
	mDead = true;

	if (mProjectileType == ProjectileType::PROJECTILE_SEAPUFF || mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		AttachmentCrossFade(mAttachmentID, "FadeOut");
		AttachmentDetach(mAttachmentID);
	}
	else
	{
		AttachmentDie(mAttachmentID);
	}

}

//0x46EBC0
Rect Projectile::GetProjectileRect()
{
	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		return Rect(mX - 15, mY, mWidth + 15, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		return Rect(mX + mWidth / 2 - 115, mY + mHeight / 2 - 115, 230, 230);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		return Rect(mX + 20, mY, 60, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE)
	{
		return Rect(mX - 25, mY, mWidth + 25, mHeight);
	}
	else
	{
		return Rect(mX, mY, mWidth, mHeight);
	}
}

//0x46ECB0
void Projectile::ConvertToFireball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_FIREBALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
	if (mBackwards)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}

void Projectile::ConvertToPlasmaball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;
	
	mProjectileType = ProjectileType::PROJECTILE_PLASMABALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PLASMA_PEA);
	if (mBackwards)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}

//0x46EE00
void Projectile::ConvertToPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

void Projectile::ConvertToSnowPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

ProjectileDefinition& Projectile::GetProjectileDef()
{
	ProjectileDefinition& aProjectileDef = gProjectileDefinition[(int)mProjectileType];
	TOD_ASSERT(aProjectileDef.mProjectileType == mProjectileType);

	return aProjectileDef;
}
