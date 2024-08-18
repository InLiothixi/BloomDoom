#ifndef __ALMANACDIALOG_H__
#define __ALMANACDIALOG_H__

#include "LawnDialog.h"
#include "../../SexyAppFramework/Widget.h"
#include "../../SexyAppFramework/Slider.h"
#include "../../SexyAppFramework/SliderListener.h"
#include "../../ConstEnums.h"

using namespace Sexy;

#define NUM_ALMANAC_SEEDS NUM_SEEDS_IN_CHOOSER
#define NUM_ALMANAC_ZOMBIES NUM_ZOMBIE_TYPES

constexpr const float			ALMANAC_PLANT_POSITION_X		= 578.0f + BOARD_ADDITIONAL_WIDTH;
constexpr const float			ALMANAC_PLANT_POSITION_Y		= 140.0f + BOARD_OFFSET_Y;
constexpr const float			ALMANAC_ZOMBIE_POSITION_X		= 559.0f + BOARD_ADDITIONAL_WIDTH;
constexpr const float			ALMANAC_ZOMBIE_POSITION_Y		= 175.0f + BOARD_OFFSET_Y;
constexpr const int				ALMANAC_INDEXPLANT_POSITION_X	= 167 + BOARD_ADDITIONAL_WIDTH;
constexpr const int				ALMANAC_INDEXPLANT_POSITION_Y	= 255 + BOARD_OFFSET_Y;
constexpr const float			ALMANAC_INDEXZOMBIE_POSITION_X	= 535.0f + BOARD_ADDITIONAL_WIDTH;
constexpr const float			ALMANAC_INDEXZOMBIE_POSITION_Y	= 215.0f + BOARD_OFFSET_Y;

class Plant;
class Zombie;
class LawnApp;
class GameButton;
class Reanimation;
class AlmanacDialog : public LawnDialog, public Sexy::SliderListener
{
private:
	enum
	{
		ALMANAC_BUTTON_CLOSE = 0,
		ALMANAC_BUTTON_PLANT = 1,
		ALMANAC_BUTTON_ZOMBIE = 2,
		ALMANAC_BUTTON_INDEX = 3
	};

public:
	LawnApp*					mApp;					//+0x16C
	GameButton*					mCloseButton;			//+0x170
	GameButton*					mIndexButton;			//+0x174
	GameButton*					mPlantButton;			//+0x178
	GameButton*					mZombieButton;			//+0x17C
	AlmanacPage					mOpenPage;				//+0x180
	Reanimation*				mReanim[4];				//+0x184
	SeedType					mSelectedSeed;			//+0x194
	ZombieType					mSelectedZombie;		//+0x198
	Plant*						mPlant;					//+0x19C
	Zombie*						mZombie;				//+0x1A0
	Zombie*						mZombiePerfTest[400];	//+0x1A4
	float						mScrollPosition;
	float						mScrollAmount;
	const float					mBaseScrollSpeed = 1.0f;
	const float					mScrollAccel = 0.1f;
	float						mMaxScrollPosition;
	float                       mTextScrollPosition;
	Slider*						mSlider;
	Slider*						mTextSlider;

public:
	AlmanacDialog(LawnApp* theApp);
	virtual ~AlmanacDialog();

	void						ClearPlantsAndZombies();
	virtual void                AddedToManager(WidgetManager* theWidgetManager);
	virtual void				RemovedFromManager(WidgetManager* theWidgetManager);
	void						SetupPlant();
	void						SetupZombie();
	void						SetPage(AlmanacPage thePage);
	virtual void				Update();
	void						DrawIndex(Graphics* g);
	void						DrawPlants(Graphics* g);
	void						DrawZombies(Graphics* g);
	virtual void				Draw(Graphics* g);
	void						GetSeedPosition(SeedType theSeedType, int& x, int& y);
	SeedType					SeedHitTest(int x, int y);
	/*inline*/ bool				ZombieHasSilhouette(ZombieType theZombieType);
	bool						ZombieIsShown(ZombieType theZombieType);
	bool						ZombieHasDescription(ZombieType theZombieType);
	void						GetZombiePosition(ZombieType theZombieType, int& x, int& y);
	ZombieType					ZombieHitTest(int x, int y);
	virtual void				MouseUp(int x, int y, int theClickCount);
	virtual void				MouseDown(int x, int y, int theClickCount);
	virtual void				MouseWheel(int theDelta);
	virtual void				KeyDown(Sexy::KeyCode keyCode);
	virtual void				KeyChar(char theChar) {};
	void					    SliderVal(int theId, double theVal);

	static ZombieType			GetZombieType(int theIndex);
	/*inline*/ void				ShowPlant(SeedType theSeedType);
	/*inline*/ void				ShowZombie(ZombieType theZombieType);
};
extern bool gZombieDefeated[NUM_ZOMBIE_TYPES];

/*inline*/ void					AlmanacInitForPlayer();
/*inline*/ void					AlmanacPlayerDefeatedZombie(ZombieType theZombieType);

#endif
