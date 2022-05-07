
#ifndef TMESSAGE_INCLUDE
#define TMESSAGE_INCLUDE

#define _CRT_SECURE_NO_WARNINGS

typedef unsigned char tbyte;
typedef unsigned short tword;

#define MASK_NONE			0x00
#define MASK_SERVER			0x01
#define MASK_CLIENT1		0x02
#define MASK_CLIENT2		0x04
#define MASK_CLIENT3		0x08
#define MASK_CLIENT4		0x10
#define MASK_CLIENT5		0x20
#define MASK_CLIENT6		0x40
#define MASK_CLIENT7		0x80
#define MASK_ALL_CLIENTS	0xfe
#define MASK_ALL			0xff

inline tbyte getMaskFromPlayerId(unsigned int playerId)
{
	tbyte masks[] = {MASK_SERVER, MASK_CLIENT1, MASK_CLIENT2, MASK_CLIENT3, MASK_CLIENT4, MASK_CLIENT5, MASK_CLIENT6, MASK_CLIENT7};

	if(playerId < 8)
		return masks[playerId];
	else
		return MASK_NONE;
}


enum EMessage { eMsgProfile = 0, eMsgStartMatch, eMsgPlUpdate, eMsgPlSound, eMsgOpenDoor, eMsgConsoleLog, eMsgRequestItem, eMsgRemoveItem, eMsgConcealItem,
				eMsgDamage, eMsgNewBullet, eMsgEndBullet, eMsgNewExplos, eMsgNewBarrier, eMsgNewItem, eMsgTurnSwitch, eMsgTalkToChar, eMsgIncreaseKills, eMsgEndMatch,
				eMsgKeepAlive, eMsgPlDisconnect };


struct TMessage
{
	tbyte mType;
	tbyte mSender;
	tbyte mReceiver;

	virtual unsigned int size() { return sizeof(TMessage); };
	virtual char* typeName() { return (char*)"Message"; };
};

struct TNetPlayerProfile
{
	char mName[8];
	char mCharacter[8];
	tbyte mClass;
	tbyte mColor1;
	tbyte mColor2;
	tbyte mRemoveStaff;
};

struct TMsgProfile : TMessage {

	TNetPlayerProfile mProfile;
	tbyte	mLobbyId;

	unsigned int size() { return sizeof(TMsgProfile); };
	char* typeName()	{ return (char*)"MsgProfile"; };
};

struct TMsgStartMatch : TMessage {

	tbyte mNumPlayers;
	tbyte mAssignedPlayerId;
	tbyte mEpisode;
	tbyte mLevel;
	tbyte mStartX, mStartY;
	TNetPlayerProfile mProfiles[8];
	char mArena[8];
	tword mTimeMax;

	unsigned int size() { return sizeof(TMsgStartMatch); };
	char* typeName()	{ return (char*)"MsgStartMatch"; };
};



struct TMsgPlUpdate : TMessage {

	tbyte mPlayerId;
	tbyte mCharacterType;
	tbyte mState;
	tbyte mCounter;
	tbyte mDir;
	tbyte mWeapon;
	tbyte mShield;
	tword mHealth;
	tbyte mSpecials[5];
	tbyte mElec;
	float mX;
	float mY;

	unsigned int size() { return sizeof(TMsgPlUpdate); };
	char* typeName()	{ return (char*)"MsgPlUpdate"; };
};

struct TMsgPlSound : TMessage {

	tbyte mPlayerId;
	tbyte mSound;

	unsigned int size() { return sizeof(TMsgPlSound); };
	char* typeName()	{ return (char*)"MsgPlSound"; };
};

struct TMsgOpenDoor : TMessage {

	tbyte mDoorId;

	unsigned int size() { return sizeof(TMsgOpenDoor); };
	char* typeName()	{ return (char*)"MsgOpenDoor"; };
};

struct TMsgConsoleLog : TMessage {

	char mText[1920/15];
	tbyte mColor;
	tbyte mPrio;

	unsigned int size() { return sizeof(TMsgConsoleLog); };
	char* typeName()	{ return (char*)"MsgConsoleLog"; };
};

struct TMsgRequestItem : TMessage {

	tbyte mItemId;
	tbyte mPlayerId;

	unsigned int size() { return sizeof(TMsgRequestItem); };
	char* typeName()	{ return (char*)"MsgRequestItem"; };
};

struct TMsgRemoveItem : TMessage {

	tbyte mItemId;

	unsigned int size() { return sizeof(TMsgRemoveItem); };
	char* typeName()	{ return (char*)"MsgRemoveItem"; };
};

struct TMsgConcealItem : TMessage {

	tbyte mItemId;
	tbyte mPlayerId;

	unsigned int size() { return sizeof(TMsgConcealItem); };
	char* typeName()	{ return (char*)"MsgConcealItem"; };
};

struct TMsgDamage : TMessage {

	tbyte mSourceType;
	tbyte mSourceId;
	tbyte mTargetType;
	tbyte mTargetId;
	tbyte mDamType;
	tword mAmount;

	unsigned int size() { return sizeof(TMsgDamage); };
	char* typeName()	{ return (char*)"MsgDamage"; };
};

struct TMsgNewBullet : TMessage {

	tbyte mCreatorType;
	tbyte mCreatorId;
	float mX;
	float mY;
	tbyte mDir;
	float mVel;
	float mAcc;
	tbyte mBulType;
	tword mDamage;
	tbyte mSample;

	unsigned int size() { return sizeof(TMsgNewBullet); };
	char* typeName()	{ return (char*)"MsgNewBullet"; };
};

struct TMsgEndBullet : TMessage {

	tbyte mId;

	unsigned int size() { return sizeof(TMsgEndBullet); };
	char* typeName()	{ return (char*)"MsgEndBullet"; };
};

struct TMsgNewExplos : TMessage {

	tbyte mCreatorType;
	tbyte mCreatorId;
	float mX;
	float mY;
	tbyte mExpType;
	tword mDamage;

	unsigned int size() { return sizeof(TMsgNewExplos); };
	char* typeName()	{ return (char*)"MsgNewExplos"; };
};

struct TMsgNewBarrier : TMessage {

	float mX;
	float mY;
	tbyte mBarrType;

	unsigned int size() { return sizeof(TMsgNewBarrier); };
	char* typeName()	{ return (char*)"MsgNewBarrier"; };
};

struct TMsgNewItem : TMessage {

	tbyte mX;
	tbyte mY;
	tbyte mItemType;

	unsigned int size() { return sizeof(TMsgNewItem); };
	char* typeName() { return (char*)"MsgNewItem"; };
};


struct TMsgTurnSwitch : TMessage {

	tbyte mSwitchId;

	unsigned int size() { return sizeof(TMsgTurnSwitch); };
	char* typeName()	{ return (char*)"MsgTurnSwitch"; };
};

struct TMsgTalkToChar : TMessage {

	tbyte mPlayerId;
	tbyte mEnemyId;

	unsigned int size() { return sizeof(TMsgTalkToChar); };
	char* typeName() { return (char*)"MsgTalkToChar"; };
};

struct TMsgIncreaseKills : TMessage {

	tbyte mPlayerId;

	unsigned int size() { return sizeof(TMsgIncreaseKills); };
	char* typeName()	{ return (char*)"MsgIncreaseKills"; };
};

struct TMsgEndMatch : TMessage {

	unsigned int size() { return sizeof(TMsgEndMatch); };
	char* typeName()	{ return (char*)"MsgEndMatch"; };
};

struct TMsgKeepAlive : TMessage {

	unsigned int size() { return sizeof(TMsgKeepAlive); };
	char* typeName()	{ return (char*)"MsgKeepAlive"; };
};

struct TMsgPlDisconnect : TMessage {

	tbyte mPlayerId;

	unsigned int size() { return sizeof(TMsgPlDisconnect); };
	char* typeName()	{ return (char*)"MsgPlDisconnect"; };
};


#endif
