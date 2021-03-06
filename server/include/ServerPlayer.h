#pragma once

#ifndef SERVERPLAYER_H
#define SERVERPLAYER_H

#include "CString.h"

enum
{
	PLPROP_NICKNAME = 0,
	PLPROP_MAXPOWER = 1,
	PLPROP_CURPOWER = 2,
	PLPROP_RUPEESCOUNT = 3,
	PLPROP_ARROWSCOUNT = 4,
	PLPROP_BOMBSCOUNT = 5,
	PLPROP_GLOVEPOWER = 6,
	PLPROP_BOMBPOWER = 7,
	PLPROP_SWORDPOWER = 8,
	PLPROP_SHIELDPOWER = 9,
	PLPROP_GANI = 10,	// PLPROP_BOWGIF in pre-2.x
	PLPROP_HEADGIF = 11,
	PLPROP_CURCHAT = 12,
	PLPROP_COLORS = 13,
	PLPROP_ID = 14,
	PLPROP_X = 15,
	PLPROP_Y = 16,
	PLPROP_SPRITE = 17,
	PLPROP_STATUS = 18,
	PLPROP_CARRYSPRITE = 19,
	PLPROP_CURLEVEL = 20,
	PLPROP_HORSEGIF = 21,
	PLPROP_HORSEBUSHES = 22,
	PLPROP_EFFECTCOLORS = 23,
	PLPROP_CARRYNPC = 24,
	PLPROP_APCOUNTER = 25,
	PLPROP_MAGICPOINTS = 26,
	PLPROP_KILLSCOUNT = 27,
	PLPROP_DEATHSCOUNT = 28,
	PLPROP_ONLINESECS = 29,
	PLPROP_IPADDR = 30,
	PLPROP_UDPPORT = 31,
	PLPROP_ALIGNMENT = 32,
	PLPROP_ADDITFLAGS = 33,
	PLPROP_ACCOUNTNAME = 34,
	PLPROP_BODYIMG = 35,
	PLPROP_RATING = 36,
	PLPROP_GATTRIB1 = 37,
	PLPROP_GATTRIB2 = 38,
	PLPROP_GATTRIB3 = 39,
	PLPROP_GATTRIB4 = 40,
	PLPROP_GATTRIB5 = 41,
	PLPROP_ATTACHNPC = 42,
	PLPROP_GMAPLEVELX = 43,
	PLPROP_GMAPLEVELY = 44,
	PLPROP_Z = 45,
	PLPROP_GATTRIB6 = 46,
	PLPROP_GATTRIB7 = 47,
	PLPROP_GATTRIB8 = 48,
	PLPROP_GATTRIB9 = 49,
	PLPROP_JOINLEAVELVL = 50,
	PLPROP_PCONNECTED = 51,
	PLPROP_PLANGUAGE = 52,
	PLPROP_PSTATUSMSG = 53,
	PLPROP_GATTRIB10 = 54,
	PLPROP_GATTRIB11 = 55,
	PLPROP_GATTRIB12 = 56,
	PLPROP_GATTRIB13 = 57,
	PLPROP_GATTRIB14 = 58,
	PLPROP_GATTRIB15 = 59,
	PLPROP_GATTRIB16 = 60,
	PLPROP_GATTRIB17 = 61,
	PLPROP_GATTRIB18 = 62,
	PLPROP_GATTRIB19 = 63,
	PLPROP_GATTRIB20 = 64,
	PLPROP_GATTRIB21 = 65,
	PLPROP_GATTRIB22 = 66,
	PLPROP_GATTRIB23 = 67,
	PLPROP_GATTRIB24 = 68,
	PLPROP_GATTRIB25 = 69,
	PLPROP_GATTRIB26 = 70,
	PLPROP_GATTRIB27 = 71,
	PLPROP_GATTRIB28 = 72,
	PLPROP_GATTRIB29 = 73,
	PLPROP_GATTRIB30 = 74,
	PLPROP_OSTYPE = 75,	// 2.19+
	PLPROP_TEXTCODEPAGE = 76,	// 2.19+
	PLPROP_UNKNOWN77 = 77,
	PLPROP_X2 = 78,
	PLPROP_Y2 = 79,
	PLPROP_Z2 = 80,
	PLPROP_UNKNOWN81 = 81,

	// In Graal v5, where players have the Graal######## accounts, this is their chosen account alias (community name.)
	PLPROP_COMMUNITYNAME = 82,
};
#define propscount	83

class ServerPlayer
{
public:
	ServerPlayer();
	~ServerPlayer();

	unsigned short getId() const				{ return _id; }
	unsigned char getClientType() const			{ return _clientType; }
	double getPlayerX() const					{ return _x; }
	double getPlayerY() const					{ return _y; }
	int getAlignmentPoints() const				{ return _alignment; }
	const std::string& getAccountName() const	{ return _accountName; }
	const std::string& getLevelName() const		{ return _levelName; }
	const std::string& getNickName() const		{ return _nickName; }

	void setClientType(unsigned char type) { _clientType = type; }
	void setNickName(const std::string& nickname) { _nickName = nickname; }

	// Prop manipulation
	void setProps(CString& pPacket);

protected:
	unsigned short _id;
	unsigned char _clientType;
	double _x, _y;
	int _alignment;
	std::string _accountName, _nickName;
	std::string _levelName;
};

#endif