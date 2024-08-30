#include "Bush.h"
#include "Board.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"

#include "../SexyAppFramework/DDInterface.h"
#include "../SexyAppFramework/D3DInterface.h"
#include "../SexyAppFramework/DDImage.h"

Bush::Bush() {}

const ReanimationType grassBushes[] = {
	ReanimationType::REANIM_BUSHES3,
	ReanimationType::REANIM_BUSHES5,
	ReanimationType::REANIM_BUSHES4,
	ReanimationType::REANIM_NIGHT_BUSHES3,
	ReanimationType::REANIM_NIGHT_BUSHES5,
	ReanimationType::REANIM_NIGHT_BUSHES4,
};

void Bush::BushInitialize(int x, int y, int daID, bool isNight, int mRow)
{
	int id = (daID + 3) % 3;	if (isNight)	id += 3;
	mX = x;	mY = y;	mId = id;

	mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, mRow, 9);
	Reanimation* mReanim = mApp->AddReanimation(x, y, mRenderOrder, grassBushes[id]);
	mReanimID = mApp->ReanimationGetID(mReanim);
	mReanim->mAnimRate = 0.0f;
	mAttachmentID = ATTACHMENTID_NULL;
	AttachReanim(mAttachmentID, mReanim, 0.0f, 0.0f);
}

void Bush::AnimateBush() {
	Reanimation* mReanim = mApp->ReanimationTryToGet(mReanimID);
	if (mReanim)	mReanim->PlayReanim("anim_rustle", REANIM_PLAY_ONCE_AND_HOLD, 0, 12);
}

void Bush::Update() {
	Reanimation* mReanim = mApp->ReanimationTryToGet(mReanimID);
	if (mReanim)	mReanim->Update();
}
void Bush::Draw(Graphics* g){
	if (mAttachmentID == ATTACHMENTID_NULL)		return;
	Graphics theBushGraphics(*g);
	MakeParentGraphicsFrame(&theBushGraphics);
	AttachmentDraw(mAttachmentID, &theBushGraphics, false);

}

BushCheap::BushCheap() {}

void BushCheap::BushCheapInitialize(int x, int y, int daID, bool isNight, int mRow)
{
	int id = (daID + 3) % 3;	if (isNight)	id += 3;
	mX = x;	mY = y;	mId = id;

	mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, mRow, 9);
}

void BushCheap::Draw(Graphics* g) {
	Image* grassBushesIMAGE[] = {
		IMAGE_BUSH3,
		IMAGE_BUSH5,
		IMAGE_BUSH4,
		IMAGE_NIGHTBUSH3,
		IMAGE_NIGHTBUSH5,
		IMAGE_NIGHTBUSH4,
		
	};

	float offsetX = 0;
	float offsetY = 0;

	if (mId == 0 || mId == 1) {
		offsetX = 7.3f;
		offsetY = -26.6f;
	}
	else if (mId == 3 || mId == 4) {
		offsetY = -26.6f;
	}
	else if (mId == 5) {
		offsetY = -39.8f;
	}
	else if (mId == 2) {
		offsetX = -1.2f;
		offsetY = -39.8f;
	}

	offsetX -= 0.5f;
	offsetY -= 0.5f;

	g->DrawImageF(grassBushesIMAGE[mId], offsetX, offsetY);
}
