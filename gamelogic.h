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
//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����

//�˿�����
#define CT_SINGLE               1                 //��������
#define CT_ONE_LONG             2                 //��������
#define CT_TWO_LONG             3                 //��������
#define CT_THREE_TIAO           4                 //��������
#define CT_SHUN_ZI              5                 //˳������
#define CT_TONG_HUA             6                 //ͬ������
#define CT_HU_LU                7                 //��«����
#define CT_TIE_ZHI              8                 //��֧����
#define CT_TONG_HUA_SHUN        9                 //ͬ��˳��
#define CT_KING_TONG_HUA_SHUN   10                //�ʼ�ͬ��˳

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
//��Ŀ����
#define FULL_COUNT          52                  //ȫ����Ŀ
#define MAX_COUNT           2                   //�����Ŀ
#define MAX_CENTERCOUNT     5                   //�����Ŀ


//����ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

//��ֵ����
#define CARD_MASK_COLOR       0xF0              //��ɫ����
#define CARD_MASK_VALUE       0x0F              //��ֵ����

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

//ʤ����Ϣ�ṹ
struct UserWinList
{
  BYTE bSameCount;
  WORD wWinerList[GAME_PLAYER];
};

//�����ṹ DZ 
struct tagAnalyseResult
{
  BYTE              bType;
  BYTE              cbBuBu;
  BYTE              cbFourCount;                  //������Ŀ
  BYTE              cbThreeCount;                 //������Ŀ
  BYTE              cbLONGCount;                  //������Ŀ
  BYTE              cbSignedCount;                //������Ŀ
  BYTE              cbColorCount;
  BYTE              cbBuColor;
  BYTE              cbShunCount;
  BYTE              cbBuShun;
  BYTE              MaxVal;
  BYTE              MinVal;
  BYTE              cbFourLogicVolue[1];          //�����б�
  BYTE              cbThreeLogicVolue[1];         //�����б�
  BYTE              cbLONGLogicVolue[2];          //�����б�
  BYTE              cbSignedLogicVolue[5];        //�����б�
  BYTE              cbFourCardData[MAX_CENTERCOUNT];      //�����б�
  BYTE              cbThreeCardData[MAX_CENTERCOUNT];     //�����б�
  BYTE              cbLONGCardData[MAX_CENTERCOUNT];      //�����б�
  BYTE              cbSignedCardData[MAX_CENTERCOUNT];    //������Ŀ
  BYTE              cbColorCardData[MAX_CENTERCOUNT];    
  BYTE              cbShunData[MAX_CENTERCOUNT];
};

// HK
//�����ṹ
struct tagAnalyseResultHK
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE 							cbFourLogicVolue[1];				//�����б�
	BYTE 							cbThreeLogicVolue[1];				//�����б�
	BYTE 							cbDoubleLogicVolue[2];				//�����б�
	BYTE 							cbSignedLogicVolue[5];				//�����б�
	BYTE							cbFourCardData[MAX_COUNT];			//�����б�
	BYTE							cbThreeCardData[MAX_COUNT];			//�����б�
	BYTE							cbDoubleCardData[MAX_COUNT];		//�����б�
	BYTE							cbSignedCardData[MAX_COUNT];		//������Ŀ
};

typedef struct ChipCount {
	int nSet;
	int nAdd;
	int nFollow;
	int nPass;
} ChipCount;
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class gamelogic
{
  //��������
public:
  //���캯��
  gamelogic();
  //��������
  virtual ~gamelogic();
	
  //��������
private:
 // static BYTE           m_cbCardData[FULL_COUNT];       //�˿˶���

  //���ƺ���
public:
  //�����˿�
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //ϴ�� 
  void shuffle_poker(BYTE(&poker)[FULL_COUNT],int game_list);
  
  //���ͺ���
public:
  //��ȡ����
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ��ֵ
  BYTE GetCardValue(BYTE cbCardData) {return cbCardData & LOGIC_MASK_VALUE; }
  //��ȡ��ɫ
  BYTE GetCardColor(BYTE cbCardData) {return cbCardData & LOGIC_MASK_COLOR; }

  //���ܺ���
public:
  //�߼���ֵ
  BYTE GetCardLogicValue(BYTE cbCardData);
  //�Ա��˿�
  BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
  //�����˿�
  void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
  //7��5
  BYTE FiveFromSeven(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData[], BYTE cbCenterCardCount, BYTE cbLastCardData[], BYTE cbLastCardCount); 
  //6��5
  BYTE FiveFromSix(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData, BYTE cbLastCardData[]);
  //�������
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



