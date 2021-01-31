#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <cstring>
#include<ctime>
#include<string>
#include<ctime>
#include <chrono>
#include <random>

#pragma once

#define FALSE  0
#define TRUE   1

// DZ
//数值掩码
#define LOGIC_MASK_COLOR      0xF0                //花色掩码
#define LOGIC_MASK_VALUE      0x0F                //数值掩码

//扑克类型
#define CT_SINGLE               1                 //单牌类型
#define CT_ONE_LONG             2                 //对子类型
#define CT_TWO_LONG             3                 //两对类型
#define CT_THREE_TIAO           4                 //三条类型
#define CT_SHUN_ZI              5                 //顺子类型
#define CT_TONG_HUA             6                 //同花类型
#define CT_HU_LU                7                 //葫芦类型
#define CT_TIE_ZHI              8                 //铁支类型
#define CT_TONG_HUA_SHUN        9                 //同花顺型
#define CT_KING_TONG_HUA_SHUN   10                //皇家同花顺

#define PREVPOS 1
#define MIDPOS 2
#define AFTERPOS 3

#define NONE_CARD 0
#define ONE_CARD  1
#define TWO_CARD  2
#define BUBU_CARD 3
#define DTWO_CARD 4
#define BIG_CARD  5

#define GAME_PLAYER 9
//数目定义
#define FULL_COUNT          52                  //全牌数目
#define MAX_COUNT           2                   //最大数目
#define MAX_CENTERCOUNT     5                   //最大数目


//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

//数值掩码
#define CARD_MASK_COLOR       0xF0              //花色掩码
#define CARD_MASK_VALUE       0x0F              //数值掩码

//////////////////////////////////////////////////////////////////////////

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

//胜利信息结构
struct UserWinList
{
  BYTE bSameCount;
  WORD wWinerList[GAME_PLAYER];
};

//分析结构 DZ 
struct tagAnalyseResult
{
  BYTE              bType;
  BYTE              cbBuBu;
  BYTE              cbFourCount;                  //四张数目
  BYTE              cbThreeCount;                 //三张数目
  BYTE              cbLONGCount;                  //两张数目
  BYTE              cbSignedCount;                //单张数目
  BYTE              cbColorCount;
  BYTE              cbBuColor;
  BYTE              cbShunCount;
  BYTE              cbBuShun;
  BYTE              MaxVal;
  BYTE              MinVal;
  BYTE              cbFourLogicVolue[1];          //四张列表
  BYTE              cbThreeLogicVolue[1];         //三张列表
  BYTE              cbLONGLogicVolue[2];          //两张列表
  BYTE              cbSignedLogicVolue[5];        //单张列表
  BYTE              cbFourCardData[MAX_CENTERCOUNT];      //四张列表
  BYTE              cbThreeCardData[MAX_CENTERCOUNT];     //三张列表
  BYTE              cbLONGCardData[MAX_CENTERCOUNT];      //两张列表
  BYTE              cbSignedCardData[MAX_CENTERCOUNT];    //单张数目
  BYTE              cbColorCardData[MAX_CENTERCOUNT];    
  BYTE              cbShunData[MAX_CENTERCOUNT];
};

// HK
//分析结构
struct tagAnalyseResultHK
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbDoubleLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[MAX_COUNT];			//四张列表
	BYTE							cbThreeCardData[MAX_COUNT];			//三张列表
	BYTE							cbDoubleCardData[MAX_COUNT];		//两张列表
	BYTE							cbSignedCardData[MAX_COUNT];		//单张数目
};

typedef struct ChipCount {
	int nSet;
	int nAdd;
	int nFollow;
	int nPass;
} ChipCount;
//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class gamelogic
{
  //函数定义
public:
  //构造函数
  gamelogic();
  //析构函数
  virtual ~gamelogic();
	
  //变量定义
private:
 // static BYTE           m_cbCardData[FULL_COUNT];       //扑克定义

  //控制函数
public:
  //排列扑克
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //洗牌 
  void shuffle_poker(BYTE(&poker)[FULL_COUNT],int game_list);
  
  //类型函数
public:
  //获取类型
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //获取数值
  BYTE GetCardValue(BYTE cbCardData) {return cbCardData & LOGIC_MASK_VALUE; }
  //获取花色
  BYTE GetCardColor(BYTE cbCardData) {return cbCardData & LOGIC_MASK_COLOR; }

  //功能函数
public:
  //逻辑数值
  BYTE GetCardLogicValue(BYTE cbCardData);
  //对比扑克
  BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
  //分析扑克
  void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
  //7返5
  BYTE FiveFromSeven(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData[], BYTE cbCenterCardCount, BYTE cbLastCardData[], BYTE cbLastCardCount); 
  //6返5
  BYTE FiveFromSix(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData, BYTE cbLastCardData[]);
  //查找最大
  bool SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const int lAddScore[]);

private:
	int  AnalyCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	int  AnalyCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbOwnCard[], tagAnalyseResult* pAnaly, tagAnalyseResult& AnalyseResult);
	
  //HK
public:
	BYTE GetCardGenre(BYTE cbCardData[], BYTE cbCardCount);
	void AnalysebCardDataHK(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResultHK & AnalyseResult);
};

//////////////////////////////////////////////////////////////////////////

#endif



