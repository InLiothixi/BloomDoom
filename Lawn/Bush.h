#ifndef __BUSH_H__
#define __BUSH_H__

#include "GameObject.h"
#include "../Sexy.TodLib/Reanimator.h"

namespace Sexy
{
    class Graphics;
};
using namespace Sexy;

class Bush : public GameObject
{
public:
    ReanimationID           mReanimID;
    AttachmentID            mAttachmentID;
    bool                    mDead;
    int                     mId;

public:
    Bush();
    void                    BushInitialize(int x, int y, int id, bool isNight, int mRow);
    void                    Draw(Graphics* g);
    void                    AnimateBush();
    void                    Update();
};

class BushCheap : public GameObject
{
public:
    ReanimationID           mReanimID;
    bool                    mDead;
    int                     mId;
    bool                    mIsValid = false;

public:
    BushCheap();
    void                    BushCheapInitialize(int x, int y, int id, bool isNight, int mRow);
    void                    Draw(Graphics* g);
};
#endif