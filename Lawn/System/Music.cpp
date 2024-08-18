#include "Music.h"
#include "../Board.h"
#include "PlayerInfo.h"
#include "../../LawnApp.h"
#include "../../Paklib/PakInterface.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/BassLoader.h"
#include "../../SexyAppFramework/BassMusicInterface.h"

using namespace Sexy;

//0x45A260
Music::Music()
{
	mApp = (LawnApp*)gSexyAppBase;
	mMusicInterface = gSexyAppBase->mMusicInterface;
	mCurMusicTune = MusicTune::MUSIC_TUNE_NONE;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	mBurstOverride = -1;
	mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
	mQueuedDrumTrackPackedOrder = -1;
	mBaseBPM = 155;
	mBaseModSpeed = 3;
	mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
	mPauseOffset = 0;
	mPauseOffsetDrums = 0;
	mPaused = false;
	mMusicDisabled = false;
	mFadeOutCounter = 0;
	mFadeOutDuration = 0;
	mHQMusic = mApp->mHQMusic;
}

MusicFileData gMusicFileData[MusicFile::NUM_MUSIC_FILES];  //0x6A9ED0

//0x45A2C0
bool Music::TodLoadMusic(MusicFile theMusicFile, const std::string& theFileName)
{
	HMUSIC aHMusic = 0;
	HSTREAM aStream = 0;
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	std::string anExt;

	size_t aDot = theFileName.rfind('.');
	if (aDot != std::string::npos)  // 文件名中不含“.”（文件无扩展名）
		anExt = StringToLower(theFileName.substr(aDot + 1));  // 取得小写的文件扩展名

	if (anExt.compare("wav") && anExt.compare("ogg") && anExt.compare("mp3"))  // 如果不是这三种拓展名
	{
		PFILE* pFile = p_fopen(theFileName.c_str(), "rb");
		if (pFile == nullptr)
			return false;

		p_fseek(pFile, 0, SEEK_END);  // 指针调整至文件末尾
		int aSize = p_ftell(pFile);  // 当前位置即为文件长度
		p_fseek(pFile, 0, SEEK_SET);  // 指针调回文件开头
		void* aData = operator new[](aSize);
		p_fread(aData, sizeof(char), aSize, pFile);  // 按字节读取数据
		p_fclose(pFile);  // 关闭文件流

		if (gBass->mVersion2)
			aHMusic = gBass->BASS_MusicLoad2(true, aData, 0, 0, aBass->mMusicLoadFlags, 0);
		else
			aHMusic = gBass->BASS_MusicLoad(true, aData, 0, 0, aBass->mMusicLoadFlags);
		delete[] aData;

		if (aHMusic == 0)
			return false;
	}
	else
	{
		PFILE* pFile = p_fopen(theFileName.c_str(), "rb");
		if (pFile == nullptr)
			return false;

		p_fseek(pFile, 0, SEEK_END);  // 指针调整至文件末尾
		int aSize = p_ftell(pFile);  // 当前位置即为文件长度
		p_fseek(pFile, 0, SEEK_SET);  // 指针调回文件开头
		void* aData = operator new[](aSize);
		p_fread(aData, sizeof(char), aSize, pFile);  // 按字节读取数据
		p_fclose(pFile);  // 关闭文件流

		aStream = gBass->BASS_StreamCreateFile(true, aData, 0, aSize, 0);
		TOD_ASSERT(gMusicFileData[theMusicFile].mFileData == nullptr);
		gMusicFileData[theMusicFile].mFileData = (unsigned int*)aData;

		if (aStream == NULL)
			return false;
	}

	BassMusicInfo aMusicInfo;
	aMusicInfo.mHStream = aStream;
	aMusicInfo.mHMusic = aHMusic;
	aBass->mMusicMap.insert(BassMusicMap::value_type(theMusicFile, aMusicInfo));  // 将目标音乐文件编号和音乐信息的对组加入音乐数据容器
	return true;
}

//0x45A6C0
void Music::SetupMusicFileForTune(MusicFile theMusicFile, MusicTune theMusicTune)
{
	int aTrackCount = 0;
	int aTrackStart1 = -1, aTrackEnd1 = -1, aTrackStart2 = -1, aTrackEnd2 = -1;

	switch (theMusicTune)
	{
	case MusicTune::MUSIC_TUNE_DAY_GRASSWALK:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 23;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 27;	aTrackEnd1 = 27;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 24;	aTrackEnd1 = 26;											break;
		default: break;
		} break;
	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 17;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 18;	aTrackEnd1 = 24;  aTrackStart2 = 29;	aTrackEnd2 = 29;	break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 25;	aTrackEnd1 = 28;											break;
		default: break;
		} break;
	case MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 15;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 23;	aTrackEnd1 = 23;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 16;	aTrackEnd1 = 22;											break;
		default: break;
		} break;
	case MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF:
		switch (theMusicFile) {
		case MusicFile::MUSIC_FILE_MAIN_MUSIC:		aTrackCount = 30;	aTrackStart1 = 0;	aTrackEnd1 = 17;											break;
		case MusicFile::MUSIC_FILE_HIHATS:			aTrackCount = 30;	aTrackStart1 = 21;	aTrackEnd1 = 21;											break;
		case MusicFile::MUSIC_FILE_DRUMS:			aTrackCount = 30;	aTrackStart1 = 18;	aTrackEnd1 = 20;											break;
		default: break;
		} break;
	default:
		if (theMusicFile == MusicFile::MUSIC_FILE_MAIN_MUSIC || theMusicFile == MusicFile::MUSIC_FILE_DRUMS)
		{
			aTrackCount = 30;
			aTrackStart1 = 0;
			aTrackEnd1 = 29;
		}
		break;
	}

	HMUSIC aHMusic = GetBassMusicHandle(theMusicFile);

	for (int aTrack = 0; aTrack <= aTrackCount; aTrack++)
	{
		float aVolume = 0;
		if (aTrack >= aTrackStart1 && aTrack <= aTrackEnd1)
			aVolume = 1;
		else if (aTrack >= aTrackStart2 && aTrack <= aTrackEnd2)
			aVolume = 1;

		gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_VOL_CHAN + aTrack, aVolume * 100);
	}
}

void Music::LoadSong(MusicFile theMusicFile, const std::string& theFileName)
{
	TodHesitationTrace("preloadsong");
	if (!TodLoadMusic(theMusicFile, theFileName))
	{
		TodTrace("music failed to load\n");
		mMusicDisabled = true;
	}
	else
	{
		gBass->BASS_MusicSetAttribute(GetBassMusicHandle(theMusicFile), BASS_MUSIC_ATTRIB_PSCALER, 4);  // 设置音乐定位精确度属性
		TodHesitationTrace("song '%s'", theFileName.c_str());
	}
}

//0x45A8A0
void Music::MusicTitleScreenInit()
{
	LoadSong(MusicFile::MUSIC_FILE_MAIN_MUSIC, "sounds\\mainmusic.mo3");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_HELP, "sounds\\help.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	if (mApp->HasFinishedAdventure() && mApp->TrophiesNeedForGoldSunflower() <= 0)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FLORAL_HEAVEN);
	}
	else
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME);
	}
}

//0x45A980
void Music::MusicInit()
{
#ifdef _DEBUG
	int aNumLoadingTasks = mApp->mCompletedLoadingThreadTasks + GetNumLoadingTasks();
#endif

	LoadSong(MusicFile::MUSIC_FILE_GRASS_THE_MOON, "sounds\\grassthemoon.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_GRASS_THE_MOON_HIHATS, "sounds\\grassthemoon_hihats.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_GRASS_THE_MOON_DRUMS, "sounds\\grassthemoon_drums.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_BRAIN_STORM, "sounds\\brainstorm.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_FALLING_CRYSTALS, "sounds\\falling_crystals.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_RAINY_RIGOR_MORMIST, "sounds\\rainy_rigor_mormist.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_GRAVEYARD_BOSS, "sounds\\graveyardboss.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_DRUMS, "sounds\\mainmusic.mo3");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_HIHATS, "sounds\\mainmusic.mo3"); //"sounds\\mainmusic_hihats.mo3"
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_POOL_WATERYGRAVES_FAST, "sounds\\waterygravesfast.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_POOL_WATERYGRAVES_SLOW, "sounds\\waterygravesslow.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_GLOOMBLOOM, "sounds\\gloombloom.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_WOODY_TOMBSTONE, "sounds\\woodytombstone.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_WOODY_TOMBSTONE_HIHATS, "sounds\\woodytombstone_hihats.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	LoadSong(MusicFile::MUSIC_FILE_WOODY_TOMBSTONE_DRUMS, "sounds\\woodytombstone_drums.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	LoadSong(MusicFile::MUSIC_FILE_LUNACY_CADABREATH, "sounds\\lunacycadabreath.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

	if (mApp->mHQMusic)
	{
		LoadSong(MusicFile::MUSIC_FILE_FINAL_BOSS_BRAINIAC_MANIAC_HQ, "sounds\\brainiacmaniac_hq.ogg");
		mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

		LoadSong(MusicFile::MUSIC_FILE_CEREBRAWL_HQ, "sounds\\cerebrawl_hq.ogg");
		mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

		LoadSong(MusicFile::MUSIC_FILE_LOONBOON_HQ, "sounds\\loonboon_hq.ogg");
		mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;

		LoadSong(MusicFile::MUSIC_FILE_ULTIMATEBATTLE_HQ, "sounds\\ultimatebattle_hq.ogg");
		mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	}

	if (mApp->HasFinishedAdventure() && mApp->TrophiesNeedForGoldSunflower() <= 0)
	{
		BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
		if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_FLORAL_HEAVEN) == aBass->mMusicMap.end())
		{
			LoadSong(MusicFile::MUSIC_FILE_FLORAL_HEAVEN, "sounds\\floralheaven.ogg");
			mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
		}
	}

#ifdef _DEBUG
	LoadSong(MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN, "sounds\\ZombiesOnYourLawn.ogg");
	mApp->mCompletedLoadingThreadTasks += /*原版*/3500;///*内测版*/800;
	if (mApp->mCompletedLoadingThreadTasks != aNumLoadingTasks)
		TodTrace("Didn't calculate loading task count correctly!!!!");
#endif
}

//0x45AAC0
void Music::MusicCreditScreenInit()
{
#ifndef _DEBUG
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN) == aBass->mMusicMap.end())  // 如果尚未加载
		LoadSong(MusicFile::MUSIC_FILE_MAIN_MUSIC, "sounds\\ZombiesOnYourLawn.ogg");
#endif
}

//0x45ABB0
void Music::StopAllMusic()
{
	if (mMusicInterface != nullptr)
	{
		if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileMain);
		if (mCurMusicFileDrums != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileDrums);
		if (mCurMusicFileHihats != MusicFile::MUSIC_FILE_NONE)
			mMusicInterface->StopMusic(mCurMusicFileHihats);
	}

	mCurMusicTune = MusicTune::MUSIC_TUNE_NONE;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	mQueuedDrumTrackPackedOrder = -1;
	mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
	mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
	mPauseOffset = 0;
	mPauseOffsetDrums = 0;
	mPaused = false;
	mFadeOutCounter = 0;
}

//0x45AC20
HMUSIC Music::GetBassMusicHandle(MusicFile theMusicFile)
{
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	auto anItr = aBass->mMusicMap.find((int)theMusicFile);
	TOD_ASSERT(anItr != aBass->mMusicMap.end());
	return anItr->second.mHMusic;
}

HMUSIC Music::GetBassStreamHandle(MusicFile theMusicFile)
{
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	auto anItr = aBass->mMusicMap.find((int)theMusicFile);
	TOD_ASSERT(anItr != aBass->mMusicMap.end());
	return anItr->second.mHStream;
}

//0x45AC70
void Music::PlayFromOffset(MusicFile theMusicFile, int theOffset, double theVolume)
{
	BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
	auto anItr = aBass->mMusicMap.find((int)theMusicFile);
	TOD_ASSERT(anItr != aBass->mMusicMap.end());
	BassMusicInfo* aMusicInfo = &anItr->second;

	if (aMusicInfo->mHStream)
	{
		bool aNoLoop = theMusicFile == MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN;  // MV 音乐不循环
		mMusicInterface->PlayMusic(theMusicFile, theOffset, aNoLoop);
	}
	else
	{
		gBass->BASS_ChannelStop(aMusicInfo->mHMusic);  // 先停止正在播放的音乐
		SetupMusicFileForTune(theMusicFile, mCurMusicTune);  // 调整每条轨道的静音与否
		aMusicInfo->mStopOnFade = false;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap * theVolume;
		aMusicInfo->mVolumeAdd = 0.0;
		gBass->BASS_ChannelSetAttributes(aMusicInfo->mHMusic, -1, aMusicInfo->mVolume * 100.0, -101);  // 调整音乐音量
		gBass->BASS_ChannelSetFlags(aMusicInfo->mHMusic, BASS_MUSIC_POSRESET | BASS_MUSIC_RAMP | BASS_MUSIC_LOOP);
		gBass->BASS_ChannelSetPosition(aMusicInfo->mHMusic, theOffset | 0x80000000);  // 设置偏移位置
		gBass->BASS_ChannelPlay(aMusicInfo->mHMusic, false);  // 重新开始播放
	}
}

//0x45ADB0
void Music::PlayMusic(MusicTune theMusicTune, int theOffset, int theDrumsOffset)
{
	if (mMusicDisabled)
		return;

	if (mApp->mHQMusic != mHQMusic)
	{
		mHQMusic = mApp->mHQMusic;
		theOffset = -1;
	}

	mCurMusicTune = theMusicTune;
	mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
	mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
	bool aRestartingSong = theOffset != -1;

	switch (theMusicTune)
	{
	case MusicTune::MUSIC_TUNE_DAY_GRASSWALK:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		if (theOffset == -1)
		{
			theOffset = 0x80000030;
			theDrumsOffset = 0x8000005C;
		}
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theDrumsOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x8000005E;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x8000007D;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
		if (theOffset == -1)
			theOffset = 0x800000B8;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_CHOOSE_YOUR_SEEDS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0x8000007A;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0x80000098;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_ZEN_GARDEN:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
		if (theOffset == -1)
			theOffset = 0xDD;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_PUZZLE_CEREBRAWL:
		if (mApp->mHQMusic)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
			if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_CEREBRAWL_HQ) == aBass->mMusicMap.end())
			{
				LoadSong(MusicFile::MUSIC_FILE_CEREBRAWL_HQ, "sounds\\cerebrawl_hq.ogg");
			}

			mCurMusicFileMain = MusicFile::MUSIC_FILE_CEREBRAWL_HQ;
			if (theOffset == -1)
				theOffset = 0;
		}
		else
		{
			mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
			if (theOffset == -1)
				theOffset = 0xB1;
		}
		
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_MINIGAME_LOONBOON:
		if (mApp->mHQMusic)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
			if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_LOONBOON_HQ) == aBass->mMusicMap.end())
			{
				LoadSong(MusicFile::MUSIC_FILE_LOONBOON_HQ, "sounds\\loonboon_hq.ogg");
			}

			mCurMusicFileMain = MusicFile::MUSIC_FILE_LOONBOON_HQ;
			if (theOffset == -1)
				theOffset = 0;

		}
		else
		{
			mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
			if (theOffset == -1)
				theOffset = 0xA6;
		}
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_CONVEYER:
		if (mApp->mHQMusic)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
			if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_ULTIMATEBATTLE_HQ) == aBass->mMusicMap.end())
			{
				LoadSong(MusicFile::MUSIC_FILE_ULTIMATEBATTLE_HQ, "sounds\\ultimatebattle_hq.ogg");
			}

			mCurMusicFileMain = MusicFile::MUSIC_FILE_ULTIMATEBATTLE_HQ;
			if (theOffset == -1)
				theOffset = 0;
		}
		else
		{
			mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
			if (theOffset == -1)
				theOffset = 0xD4;
		}
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC:
		if (mApp->mHQMusic)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
			if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_FINAL_BOSS_BRAINIAC_MANIAC_HQ) == aBass->mMusicMap.end())
			{
				LoadSong(MusicFile::MUSIC_FILE_FINAL_BOSS_BRAINIAC_MANIAC_HQ, "sounds\\brainiacmaniac_hq.ogg");
			}

			mCurMusicFileMain = MusicFile::MUSIC_FILE_FINAL_BOSS_BRAINIAC_MANIAC_HQ;
			if (theOffset == -1)
				theOffset = 0;
		}
		else 
		{
			mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
			if (theOffset == -1)
				theOffset = 0x9E;
		}

		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;

	case MusicTune::MUSIC_TUNE_FLORAL_HEAVEN:
	{
		BassMusicInterface* aBass = (BassMusicInterface*)mApp->mMusicInterface;
		if (aBass->mMusicMap.find((int)MusicFile::MUSIC_FILE_FLORAL_HEAVEN) == aBass->mMusicMap.end())
		{
			LoadSong(MusicFile::MUSIC_FILE_FLORAL_HEAVEN, "sounds\\floralheaven.ogg");
		}

		mCurMusicFileMain = MusicFile::MUSIC_FILE_FLORAL_HEAVEN;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	}
	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES_FAST:
	{
		mCurMusicFileMain = MusicFile::MUSIC_FILE_POOL_WATERYGRAVES_FAST;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	}
	case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES_SLOW:
	{
		mCurMusicFileMain = MusicFile::MUSIC_FILE_POOL_WATERYGRAVES_SLOW;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	}
	case MusicTune::MUSIC_TUNE_GLOOMBLOOM:
	{
		mCurMusicFileMain = MusicFile::MUSIC_FILE_GLOOMBLOOM;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	}
	case MusicTune::MUSIC_TUNE_HELP:
	{
		mCurMusicFileMain = MusicFile::MUSIC_FILE_HELP;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	}
	case MusicTune::MUSIC_TUNE_CREDITS_ZOMBIES_ON_YOUR_LAWN:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	case MusicTune::MUSIC_TUNE_GRASS_THE_MOON:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_GRASS_THE_MOON;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_GRASS_THE_MOON_HIHATS;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_GRASS_THE_MOON_DRUMS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		break;
	case MusicTune::MUSIC_TUNE_WOODY_TOMBSTONE:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_WOODY_TOMBSTONE;
		mCurMusicFileHihats = MusicFile::MUSIC_FILE_WOODY_TOMBSTONE_HIHATS;
		mCurMusicFileDrums = MusicFile::MUSIC_FILE_WOODY_TOMBSTONE_DRUMS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
		PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
		break;

	case MusicTune::MUSIC_TUNE_BRAIN_STORM:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_BRAIN_STORM;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	case MusicTune::MUSIC_TUNE_FALLING_CRYSTALS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_FALLING_CRYSTALS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	case MusicTune::MUSIC_TUNE_RAINY_RIGOR_MORMIST:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_RAINY_RIGOR_MORMIST;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	case MusicTune::MUSIC_TUNE_GRAVEYARD_BOSS:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_GRAVEYARD_BOSS;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	case MusicTune::MUSIC_TUNE_LUNACY_CADABREATH:
		mCurMusicFileMain = MusicFile::MUSIC_FILE_LUNACY_CADABREATH;
		if (theOffset == -1)
			theOffset = 0;
		PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
		break;
	default:
		TOD_ASSERT(false);
		break;
	}


	if (aRestartingSong)
	{
		if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileMain);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_BPM, mBaseBPM);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_SPEED, mBaseModSpeed);
		}
		if (mCurMusicFileDrums != -1)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileDrums);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_BPM, mBaseBPM);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_SPEED, mBaseModSpeed);
		}
		if (mCurMusicFileHihats != -1)
		{
			HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileHihats);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_BPM, mBaseBPM);
			gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_SPEED, mBaseModSpeed);
		}
	}
	else
	{
		HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileMain);
		mBaseBPM = gBass->BASS_MusicGetAttribute(aHMusic, BASS_MUSIC_ATTRIB_BPM);
		mBaseModSpeed = gBass->BASS_MusicGetAttribute(aHMusic, BASS_MUSIC_ATTRIB_SPEED);
	}
}

unsigned long Music::GetMusicOrder(MusicFile theMusicFile)
{
	TOD_ASSERT(theMusicFile != MusicFile::MUSIC_FILE_NONE);
	return ((BassMusicInterface*)mApp->mMusicInterface)->GetMusicOrder((int)theMusicFile);
}

//0x45B1B0
void Music::MusicResyncChannel(MusicFile theMusicFileToMatch, MusicFile theMusicFileToSync)
{
	/*unsigned int aPosToMatch = GetMusicOrder(theMusicFileToMatch);
	unsigned int aPosToSync = GetMusicOrder(theMusicFileToSync);

	int aDiff = (aPosToSync >> 16) - (aPosToMatch >> 16);  // 待同步的音乐与目标音乐的乐曲序号之差
	*/
	//if (abs(aDiff) <= 128)  // 当前进行的乐曲序号之差超过 128 时，开摆（
	{
		HMUSIC aHMusic = GetBassMusicHandle(theMusicFileToSync);

		/*int aBPM = mBaseBPM;

		if (aDiff > 2)
			aBPM -= 2;
		else if (aDiff > 0)
			aBPM -= 1;
		else if (aDiff < -2)
			aBPM += 2;
		else if (aDiff < 0)
			aBPM -= 1;*/

		//gBass->BASS_MusicSetAttribute(aHMusic, BASS_MUSIC_ATTRIB_BPM, aBPM);  // 适当调整待同步音乐的速率以缩小差距
		gBass->BASS_ChannelSetPosition(aHMusic, gBass->BASS_ChannelGetPosition(GetBassStreamHandle(theMusicFileToMatch)));
	}
}

void Music::MusicResync()
{
	if (mCurMusicFileMain == MUSIC_FILE_GRASS_THE_MOON)
	{
		HMUSIC aHMusic = GetBassMusicHandle(mCurMusicFileHihats);
		gBass->BASS_ChannelSetPosition(aHMusic, gBass->BASS_ChannelGetPosition(GetBassStreamHandle(mCurMusicFileMain)));

		aHMusic = GetBassMusicHandle(mCurMusicFileDrums);
		gBass->BASS_ChannelSetPosition(aHMusic, gBass->BASS_ChannelGetPosition(GetBassStreamHandle(mCurMusicFileMain)));
		return;
	}

	if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE)
	{
		if (mCurMusicFileDrums != MusicFile::MUSIC_FILE_NONE)
			MusicResyncChannel(mCurMusicFileMain, mCurMusicFileDrums);
		if (mCurMusicFileHihats != MusicFile::MUSIC_FILE_NONE)
			MusicResyncChannel(mCurMusicFileMain, mCurMusicFileHihats);
	}
}

//0x45B240
void Music::StartBurst()
{
	if (mApp && mApp->mGameMode == GAMEMODE_INTRO)	return;

	if (mMusicBurstState == MusicBurstState::MUSIC_BURST_OFF)
	{
		mMusicBurstState = MusicBurstState::MUSIC_BURST_STARTING;
		mBurstStateCounter = 400;
	}
}

void Music::FadeOut(int theFadeOutDuration)
{
	if (mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
	{
		mFadeOutCounter = theFadeOutDuration;
		mFadeOutDuration = theFadeOutDuration;
	}
}

//0x45B260
void Music::UpdateMusicBurst()
{
	if (mApp->mBoard == nullptr)
		return;

	int aBurstScheme;
	if (mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK || mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES ||
		mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST || mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF || 
		mCurMusicTune == MusicTune::MUSIC_TUNE_GRASS_THE_MOON || mCurMusicTune == MusicTune::MUSIC_TUNE_WOODY_TOMBSTONE)
		aBurstScheme = 1;
	else if (mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS)
		aBurstScheme = 2;
	else
		return;

	int aPackedOrderMain = 0;
	
		
	//aPackedOrderMain = GetMusicOrder(mCurMusicFileMain);
	
	if (mCurMusicTune == MusicTune::MUSIC_TUNE_GRASS_THE_MOON || mCurMusicTune == MusicTune::MUSIC_TUNE_WOODY_TOMBSTONE)
	{
		aPackedOrderMain = gBass->BASS_ChannelGetPosition(GetBassStreamHandle(mCurMusicFileMain));
	}
	else
	{
		aPackedOrderMain = gBass->BASS_ChannelGetPosition(GetBassMusicHandle(mCurMusicFileMain));
	}

	if (mBurstStateCounter > 0)
		mBurstStateCounter--;
	if (mDrumsStateCounter > 0)
		mDrumsStateCounter--;

	float aFadeTrackVolume = 0.0f;
	float aDrumsVolume = 0.0f;
	float aMainTrackVolume = 1.0f;
	switch (mMusicBurstState)
	{
		case MusicBurstState::MUSIC_BURST_OFF:
		{
			if (mApp->mBoard->CountZombiesOnScreen() >= 10 || mBurstOverride == 1 || mApp->mGameMode == GAMEMODE_VERSUS_PLANTS_VS_PLANTS && mApp->mBoard->CountPlantsOnScreen() >= 10)
			{
				StartBurst();
			}
			break;
		}
		case MusicBurstState::MUSIC_BURST_STARTING:
		{
			if (aBurstScheme == 1)
			{
				aFadeTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0f, 1.0f, TodCurves::CURVE_LINEAR);
				if (mBurstStateCounter == 100)
				{
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
				}
				else if (mBurstStateCounter == 0)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
					mBurstStateCounter = 800;
				}
			}
			else if (aBurstScheme == 2)
			{
				if (mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
				{
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
					mBurstStateCounter = 400;
				}
				else if (mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_ON_QUEUED)
					mBurstStateCounter = 400;
				else
				{
					aMainTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
					if (mBurstStateCounter == 0)
					{
						mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
						mBurstStateCounter = 800;
					}
				}
			}
			break;
		}
		case MusicBurstState::MUSIC_BURST_ON:
		{
			aFadeTrackVolume = 1.0f;
			if (aBurstScheme == 2)
				aMainTrackVolume = 0.0f;
			if (mBurstStateCounter == 0 && ((mApp->mBoard->CountZombiesOnScreen() < 4 && mBurstOverride == -1) || mBurstOverride == 2))
			{
				if (aBurstScheme == 1)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
					mBurstStateCounter = 800;
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED;
					mQueuedDrumTrackPackedOrder = aPackedOrderMain;
				}
				else if (aBurstScheme == 2)
				{
					mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
					mBurstStateCounter = 1100;
					mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
					mDrumsStateCounter = 800;
				}
			}
			break;
		}
		case MusicBurstState::MUSIC_BURST_FINISHING:
		{
			if (aBurstScheme == 1)
			{
				aFadeTrackVolume = TodAnimateCurveFloat(800, 0, mBurstStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
			}
			else
			{
				aMainTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0f, 1.0f, TodCurves::CURVE_LINEAR);
			}

			if (mBurstStateCounter == 0 && mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
			{
				mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
			}
			break;
		}
	}

	int aDrumsJumpOrder = -1;
	int aOrderMain = 0, aOrderDrum = 0;
	if (aBurstScheme == 1)
	{
		//if (mCurMusicTune == MUSIC_TUNE_GRASS_THE_MOON)
		{
			aOrderMain = aPackedOrderMain;
			aOrderDrum = mQueuedDrumTrackPackedOrder;
		}
		/*else
		{
			aOrderMain = HIWORD(aPackedOrderMain) / 128;
			aOrderDrum = HIWORD(mQueuedDrumTrackPackedOrder) / 128;
		}*/
	}
	else if (aBurstScheme == 2)
	{
		{
			aOrderMain = aPackedOrderMain;
			aOrderDrum = mQueuedDrumTrackPackedOrder;
		}

		/*aOrderMain = LOWORD(aPackedOrderMain);
		aOrderDrum = LOWORD(mQueuedDrumTrackPackedOrder);
		if (HIWORD(aPackedOrderMain) > 252)
			aOrderMain++;
		if (HIWORD(mQueuedDrumTrackPackedOrder) > 252)
			aOrderDrum++;*/
	}

	switch (mMusicDrumsState)
	{
	case MusicDrumsState::MUSIC_DRUMS_ON_QUEUED:
		if (aOrderMain != aOrderDrum)
		{
			aDrumsVolume = 1.0f;
			mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON;

			if (aBurstScheme == 2)
			{
				aDrumsJumpOrder = (aOrderMain % 2 == 0 ? 76 : 77);
			}
		}
		break;
	case MusicDrumsState::MUSIC_DRUMS_ON:
		aDrumsVolume = 1.0f;
		break;
	case MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED:
		aDrumsVolume = 1.0f;
		if (aOrderMain != aOrderDrum && aBurstScheme == 1)
		{
			mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
			mDrumsStateCounter = 50;
		}
		break;
	case MusicDrumsState::MUSIC_DRUMS_FADING:
		if (aBurstScheme == 2)
		{
			aDrumsVolume = TodAnimateCurveFloat(800, 0, mDrumsStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
		}
		else
		{
			aDrumsVolume = TodAnimateCurveFloat(50, 0, mDrumsStateCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
		}

		if (mDrumsStateCounter == 0)
		{
			mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
		}

		break;
	case MusicDrumsState::MUSIC_DRUMS_OFF:
		break;
	}

	if (aBurstScheme == 1)
	{
		mMusicInterface->SetSongVolume(mCurMusicFileHihats, aFadeTrackVolume);
		mMusicInterface->SetSongVolume(mCurMusicFileDrums, aDrumsVolume);
	}
	else if (aBurstScheme == 2)
	{
		mMusicInterface->SetSongVolume(mCurMusicFileMain, aMainTrackVolume);
		mMusicInterface->SetSongVolume(mCurMusicFileDrums, aDrumsVolume);
		
		if (aDrumsJumpOrder != -1)
		{
			gBass->BASS_ChannelSetPosition(GetBassMusicHandle(mCurMusicFileDrums), LOWORD(aDrumsJumpOrder) | 0x80000000);
		}
		
	}
}

//0x45B670
void Music::MusicUpdate()
{
	if (mFadeOutCounter > 0)
	{
		mFadeOutCounter--;
		if (mFadeOutCounter == 0)
			StopAllMusic();
		else
		{
			float aFadeLevel = TodAnimateCurveFloat(mFadeOutDuration, 0, mFadeOutCounter, 1.0f, 0.0f, TodCurves::CURVE_LINEAR);
			mMusicInterface->SetSongVolume(mCurMusicFileMain, aFadeLevel);
		}
	}

	if (mApp->mBoard == nullptr || !mApp->mBoard->mPaused)
	{
		UpdateMusicBurst();
		MusicResync();
	}
}

//0x45B750
// GOTY @Patoke: 0x45EFA0
void Music::MakeSureMusicIsPlaying(MusicTune theMusicTune)
{
	if (mCurMusicTune != theMusicTune)
	{
		StopAllMusic();
		PlayMusic(theMusicTune, -1, -1);
	}
}

//0x45B770
void Music::StartGameMusic()
{
	TOD_ASSERT(mApp->mBoard);

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_ZEN_GARDEN);
	}
	else if (mApp->IsFinalBossLevel())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC);
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_GRAVEYARD_ENCOUNTER)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_GRAVEYARD_BOSS);
	}
	else if (mApp->IsWallnutBowlingLevel() || mApp->IsWhackAZombieLevel() || mApp->IsLittleTroubleLevel() || mApp->IsBungeeBlitzLevel() || 
		mApp->mGameMode == GAMEMODE_CHALLENGE_HEAT_WAVE || mApp->IsBigTroubleLevel())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_MINIGAME_LOONBOON);
	}
	else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SPEED)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES_FAST);
	}
	else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOWMO)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES_SLOW);
	}
	else if ((mApp->IsAdventureMode() && (mApp->mPlayerInfo->GetLevel() == 10 || mApp->mPlayerInfo->GetLevel() == 20 || mApp->mPlayerInfo->GetLevel() == 30)) ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN || mApp->mGameMode == GAMEMODE_CHALLENGE_OPEN_FIRE || mApp->mGameMode == GAMEMODE_CHALLENGE_AIR_RAID)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_CONVEYER);
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_BLOOD_MOON)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_GRASS_THE_MOON);
	}
	else if (mApp->mGameMode == GAMEMODE_CHALLENGE_FOREST)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_WOODY_TOMBSTONE);
	}
	else if (mApp->mBoard->mLevel >= 52 && mApp->mBoard->mLevel <= 54)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_LUNACY_CADABREATH);
	}
	else if (mApp->mBoard->mLevel == 55 || mApp->mBoard->StageHasRoof() && mApp->mBoard->StageIsNight() && mApp->mBoard->StageHasDarkness() && mApp->mBoard->StageHasRain())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_BRAIN_STORM);
	}
	else if (mApp->mBoard->StageHas6Rows() && mApp->mBoard->StageIsNight() && mApp->mBoard->StageHasRain())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_RAINY_RIGOR_MORMIST);
	}
	else if (mApp->mBoard->StageIsNight() && mApp->mBoard->StageHasDarkness())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_GLOOMBLOOM);
	}
	else if (mApp->mBoard->StageIsNight() && mApp->mBoard->StageHasRain())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FALLING_CRYSTALS);
	}
	else if (mApp->IsScaryPotterLevel() || mApp->IsIZombieLevel() || mApp->mGameMode == GAMEMODE_CHALLENGE_SURPRISE_ATTACK)
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_PUZZLE_CEREBRAWL);
	}
	else if (mApp->mBoard->StageHasFog())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST);
	}
	else if (mApp->mBoard->StageIsNight())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS);
	}
	else if (mApp->mBoard->StageHas6Rows())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES);
	}
	else if (mApp->mBoard->StageHasRoof())
	{
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF);
	}
	else
		MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_DAY_GRASSWALK);
}

//0x45B930
void Music::GameMusicPause(bool thePause)
{
	if (thePause)
	{
		if (!mPaused && mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
		{
			BassMusicInterface* aBass = (BassMusicInterface*)mMusicInterface;
			auto anItr = aBass->mMusicMap.find(mCurMusicFileMain);
			TOD_ASSERT(anItr != aBass->mMusicMap.end());
			BassMusicInfo* aMusicInfo = &anItr->second;

			if (aMusicInfo->mHStream)
			{
				mPauseOffset = gBass->BASS_ChannelGetPosition(aMusicInfo->mHStream);		
				mMusicInterface->StopMusic(mCurMusicFileMain);

				if (mCurMusicFileHihats != -1)
				{
					mMusicInterface->StopMusic(mCurMusicFileHihats);
				}
				if (mCurMusicFileDrums != -1)
				{
					mMusicInterface->StopMusic(mCurMusicFileDrums);
				}
			}
			else
			{
				int aOrderMain = GetMusicOrder(mCurMusicFileMain);
				mPauseOffset = MAKELONG(LOWORD(aOrderMain), HIWORD(aOrderMain) / 4);
				mMusicInterface->StopMusic(mCurMusicFileMain);

				if (mCurMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK || mCurMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES ||
					mCurMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST || mCurMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF)
				{
					mMusicInterface->StopMusic(mCurMusicFileDrums);
					mMusicInterface->StopMusic(mCurMusicFileHihats);
				}
				else if (mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS)
				{
					int aOrderDrum = GetMusicOrder(mCurMusicFileDrums);
					mPauseOffsetDrums = MAKELONG(LOWORD(aOrderDrum), HIWORD(aOrderDrum) / 4);
					mMusicInterface->StopMusic(mCurMusicFileDrums);
				}
			}
			mPaused = true;
		}
	}
	else if (mPaused)
	{
		if (mCurMusicTune != MusicTune::MUSIC_TUNE_NONE)
		{
			PlayMusic(mCurMusicTune, mPauseOffset, mPauseOffsetDrums);
		}
		
		mPaused = false;
	}
}

int Music::GetNumLoadingTasks()
{
	//return 800 * 3;  // 内测版
	return 3500 * 2;  // 原版
}
