#include "DataSync.h"
#include "PlayerInfo.h"
#include "../LawnCommon.h"
#include "../Widget/ChallengeScreen.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Buffer.h"
#include "../../SexyAppFramework/SexyAppBase.h"

static int gUserVersion = 12;

PlayerInfo::PlayerInfo()
{
	Reset();
}

//0x468310
void PlayerInfo::SyncSummary(DataSync& theSync)
{
	theSync.SyncString(mName);
	theSync.SyncLong(mUseSeq);
	theSync.SyncLong(mId);
}

#define safeExec(statement) try{statement;}catch(...){}

//0x468390
void PlayerInfo::SyncDetails(DataSync& theSync)
{
	if (theSync.GetReader())
	{
		Reset();
	}

	int aVersion = gUserVersion;
	safeExec(theSync.SyncLong(aVersion));

	// We want it to be compatible with dynamic save files
	/*theSync.SetVersion(aVersion);
	if (aVersion != gUserVersion)
	{
		return;
	}*/

	safeExec(theSync.SyncLong(mLevel))
	safeExec(theSync.SyncLong(mCoins));
	safeExec(theSync.SyncLong(mFinishedAdventure));


	for (int i = 0; i < NUM_GAME_MODES; i++)
	{
		safeExec(theSync.SyncLong(mChallengeRecords[i]));
	}


	for (int i = 0; i < NUM_STORE_ITEMS; i++)
	{
		safeExec(theSync.SyncLong(mPurchases[i]));
	}
	
	safeExec(theSync.SyncLong(mPlayTimeActivePlayer));
	safeExec(theSync.SyncLong(mPlayTimeInactivePlayer));
	safeExec(theSync.SyncLong(mHasUsedCheatKeys));
	safeExec(theSync.SyncLong(mHasWokenStinky));
	safeExec(theSync.SyncLong(mDidntPurchasePacketUpgrade));
	safeExec(theSync.SyncLong(mLastStinkyChocolateTime));
	safeExec(theSync.SyncLong(mStinkyPosX));
	safeExec(theSync.SyncLong(mStinkyPosY));
	safeExec(theSync.SyncLong(mHasUnlockedMinigames));
	safeExec(theSync.SyncLong(mHasUnlockedPuzzleMode));
	safeExec(theSync.SyncLong(mHasNewMiniGame));
	safeExec(theSync.SyncLong(mHasNewScaryPotter));
	safeExec(theSync.SyncLong(mHasNewIZombie));
	safeExec(theSync.SyncLong(mHasNewSurvival));
	safeExec(theSync.SyncLong(mHasUnlockedSurvivalMode));
	safeExec(theSync.SyncLong(mNeedsMessageOnGameSelector));
	safeExec(theSync.SyncLong(mNeedsMagicTacoReward));
	safeExec(theSync.SyncLong(mHasSeenStinky));
	safeExec(theSync.SyncLong(mHasSeenUpsell));
	safeExec(theSync.SyncLong(mPlaceHolderPlayerStats));
	safeExec(theSync.SyncLong(mNumPottedPlants));

	for (int i = 0; i < SEEDBANK_MAX; i++)
	{
		safeExec(theSync.SyncLong(mLastSeedsInBank[i]));
	}

	safeExec(theSync.SyncBytes(&mLastTimeVisitedZen, sizeof(__time32_t)));
	safeExec(theSync.SyncBytes(&mLastTimeStinkyAwake, sizeof(__time32_t)));

	for (int i = 0; i < MAX_POTTED_PLANTS; i++)
	{
		safeExec(theSync.SyncBytes(&mPottedPlant[i], sizeof(PottedPlant)));
	}
	

	for (int i = 0; i < NUM_GAME_MODES; i++)
	{
		safeExec(theSync.SyncLong(mChallengeVeteranRecords[i]));
	}
	
}

//0x469400
void PlayerInfo::LoadDetails()
{
	try
	{
		Buffer aBuffer;
		std::string aFileName =GetAppDataFolder() + StrFormat("userdata/user%d.dat", mId);
		if (!gSexyAppBase->ReadBufferFromFile(aFileName, &aBuffer, false))
		{
			return;
		}

		DataReader aReader;
		aReader.OpenMemory(aBuffer.GetDataPtr(), aBuffer.GetDataLen(), false);
		DataSync aSync(aReader);
		SyncDetails(aSync);
	}
	catch (DataReaderException&)
	{
		TodTrace("Failed to player data, resetting it\n");
		Reset();
	}
}

//0x4695F0
void PlayerInfo::SaveDetails()
{
	DataWriter aWriter;
	aWriter.OpenMemory();
	DataSync aSync(aWriter);
	SyncDetails(aSync);

	MkDir(GetAppDataFolder() + "userdata");
	std::string aFileName = GetAppDataFolder() + StrFormat("userdata/user%d.dat", mId);
	gSexyAppBase->WriteBytesToFile(aFileName, aWriter.GetDataPtr(), aWriter.GetDataLen());
}

//0x469810
void PlayerInfo::DeleteUserFiles()
{
	std::string aFilename = GetAppDataFolder() + StrFormat("userdata/user%d.dat", mId);
	gSexyAppBase->EraseFile(aFilename);

	for (int i = 0; i < (int)GameMode::NUM_GAME_MODES; i++)
	{
		std::string aFileName = GetSavedGameName((GameMode)i, mId);
		gSexyAppBase->EraseFile(aFileName);
	}
}

//0x469940
void PlayerInfo::Reset()
{
	mLevel = 1;
	mCoins = 0;
	mFinishedAdventure = 0;
	memset(mChallengeRecords, 0, sizeof(mChallengeRecords));
	memset(mPurchases, 0, sizeof(mPurchases));
	/*for (int i = 0; i < sizeof(mPurchases) / sizeof(mPurchases[0]); i++) {
		mPurchases[i] = 0;
	}*/
	mPlayTimeActivePlayer = 0;
	mPlayTimeInactivePlayer = 0;
	mHasUsedCheatKeys = 0;
	mHasWokenStinky = 0;
	mDidntPurchasePacketUpgrade = 0;
	mLastStinkyChocolateTime = 0;
	mStinkyPosX = 0;
	mStinkyPosY = 0;
	mHasUnlockedMinigames = 0;
	mHasUnlockedPuzzleMode = 0;
	mHasNewMiniGame = 0;
	mHasNewScaryPotter = 0;
	mHasNewIZombie = 0;
	mHasNewSurvival = 0;
	mHasUnlockedSurvivalMode = 0;
	mNeedsMessageOnGameSelector = 0;
	mNeedsMagicTacoReward = 0;
	mHasSeenStinky = 0;
	mHasSeenUpsell = 0;
	mPlaceHolderPlayerStats = 0;
	memset(mLastSeedsInBank, 0, sizeof(mLastSeedsInBank));
	memset(&mLastTimeStinkyAwake, 0, sizeof(__time64_t));
	memset(&mLastTimeVisitedZen, 0, sizeof(__time64_t));
	memset(mPottedPlant, 0, sizeof(mPottedPlant));
	mNumPottedPlants = 0;
	memset(mChallengeVeteranRecords, 0, sizeof(mChallengeVeteranRecords));
}

void PlayerInfo::AddCoins(int theAmount)
{
	mCoins += theAmount;

	if (mCoins > 999999999)
	{
		mCoins = 999999999;
	}
	else if (mCoins < 0)
	{
		mCoins = 0;
	}
}

void PlayerInfo::ResetChallengeRecord(GameMode theGameMode)
{
	int aGameMode = (int)theGameMode - (int)GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_1;
	TOD_ASSERT(aGameMode >= 0 && aGameMode <= NUM_CHALLENGE_MODES);
	mChallengeRecords[aGameMode] = 0;
}

//0x469A00
void PottedPlant::InitializePottedPlant(SeedType theSeedType)
{
	memset(this, 0, sizeof(PottedPlant));
	mSeedType = theSeedType;
	mDrawVariation = DrawVariation::VARIATION_NORMAL;
	mLastWateredTime = 0i64;
	mFacing = (FacingDirection)RandRangeInt((int)FacingDirection::FACING_RIGHT, (int)FacingDirection::FACING_LEFT);
	mPlantAge = PottedPlantAge::PLANTAGE_SPROUT;
	mTimesFed = 0;
	mWhichZenGarden = GardenType::GARDEN_MAIN;
	mFeedingsPerGrow = RandRangeInt(3, 5);
	mPlantNeed = PottedPlantNeed::PLANTNEED_NONE;
	mLastNeedFulfilledTime = 0i64;
	mLastFertilizedTime = 0i64;
	mLastChocolateTime = 0i64;
}
