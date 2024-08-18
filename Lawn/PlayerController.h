#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "GameObject.h"
#include "Plant.h"
#include "Zombie.h"
#include "SeedPacket.h"

#define MAX_GRID_SIZE_Y 6

enum ControllerType {
	CONTROLLER_NONE,
	CONTROLLER_PLANT,		//Preview Plant
	CONTROLLER_ZOMBIE,		//Preview Zombie
	CONTROLLER_COBCANON,	//CobCanon Crosshair
	CONTROLLER_SHOVEL,		//Player 1 Shovel
	CONTROLLER_HAMMER,		//For Whack a Zombie
	CONTROLLER_GLOVE		//For Heat Wave
};

enum AbilityType {
	ABILITY_SHOVEL,
	ABILITY_BUTTER,
	ABILITY_HAMMER,
	ABILITY_GLOVE
};

enum ControlType {
	CONTROL_MOUSE,
	CONTROL_KEYBOARD,
	CONTROL_CONTROLLER
};
namespace Sexy
{
	class Graphics;
};
using namespace Sexy;

class PlayerController : public GameObject {
public:
	float					mX;
	float					mY;
	int						mID;
	int					    mPlayerID;
	bool                    mDead;

	ControllerType			mControllerType;
	AbilityType				mAbilityType;
	PlantID					mPreviewPlantID;
	PlantingReason			mPlantingReason;
	float					mControllerSpeed;
	int						mSeedBankIndex;

	char					mKeyUp;
	char					mKeyLeft;
	char					mKeyDown;
	char					mKeyRight;
	char					mKeyLB;
	char					mKeyRB;
	char					mKeyA;
	char					mKeyB;
	char					mKeyX;
	int						mMoveTime;
	int						mArrowTime;
	int						mSelectorTime;
	int						mAbilityTime;
	int						mAbilityMaxTime;

	PlantID					mPlayerPlantID;
	bool					mPlayerMovesVertical;
	bool					mPlayerMovesHorizontal;

	PlantID					mGlovePlantID;
	ControlType             mControlType;

public:
	PlayerController();

	void					Initialize();
	void					UpdateAnimations();
	void                    Draw(Graphics* g);
};

#endif