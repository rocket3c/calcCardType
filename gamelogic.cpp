#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
	//BYTE gamelogic::m_cbCardData[FULL_COUNT] = {
	//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //方块 A - K
	//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //梅花 A - K
	//	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //红桃 A - K
	//	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //黑桃 A - K
	 // };

//构造函数
gamelogic::gamelogic() {
}

//析构函数
gamelogic::~gamelogic() {
}

//洗牌
void gamelogic::shuffle_poker(BYTE(&poker)[FULL_COUNT], int game_list) {
	std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
	//std::uniform_int_distribution<> distr(0, 52);	
	BYTE m_cbCardData[FULL_COUNT] ={0};
	
	if(game_list == 1) //德州的牌 52张 
	{
		BYTE CardData[FULL_COUNT] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //方块 A - K
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //梅花 A - K
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //红桃 A - K
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //黑桃 A - K
	  };
	  	CopyMemory(m_cbCardData, CardData, sizeof(CardData));
	}
    if(game_list == 2) //港式五张的牌 28张 
	{
		BYTE CardData[FULL_COUNT] = {
		0x01, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //方块 A - K
		0x11, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //梅花 A - K
		0x21, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //红桃 A - K
		0x31, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //黑桃 A - K	
    	};
    	CopyMemory(m_cbCardData, CardData, sizeof(CardData));
	}
	
	for(int i = 0; i < 200; i++) {
		int rand_v1 =  0;
		int rand_v2 =  0;
		if(game_list == 1)
	    {
	    	std::uniform_int_distribution<> distr(0, 52);
	    	rand_v1 =  distr(gen);
		    rand_v2 =  distr(gen);
		}
	else if(game_list == 2)
	{
		std::uniform_int_distribution<> distr(0, 28);	
		rand_v1 =  distr(gen);
		rand_v2 =  distr(gen);
	}
		if(rand_v1 != rand_v2) {
			int c = m_cbCardData[rand_v1];
			m_cbCardData[rand_v1] = m_cbCardData[rand_v2];
			m_cbCardData[rand_v2] = c;
		}
	}
	CopyMemory(poker, m_cbCardData, sizeof(poker));
}

//获取类型
BYTE gamelogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount) {
	//数据校验
	if (cbCardCount != MAX_CENTERCOUNT) return 0;
	//变量定义
	bool cbSameColor = true, bLineCard = true;
	BYTE cbFirstColor = GetCardColor(cbCardData[0]);
	BYTE cbFirstValue = GetCardLogicValue(cbCardData[0]);

	//牌形分析
	for (BYTE i = 1; i < cbCardCount; i++) {
		if (GetCardColor(cbCardData[i]) != cbFirstColor)
			cbSameColor = false;
		if (cbFirstValue != (GetCardLogicValue(cbCardData[i]) + i))
			bLineCard = false;
		if (!cbSameColor && !bLineCard) break;
	}

	//皇家同花顺
	if (cbSameColor && bLineCard && cbFirstValue == 14) return CT_KING_TONG_HUA_SHUN;

	//顺子类型
	if (!cbSameColor && bLineCard) return CT_SHUN_ZI;

	//同花类型
	if (cbSameColor && !bLineCard) return CT_TONG_HUA;

	//同花顺类型
	if (cbSameColor && bLineCard)  return CT_TONG_HUA_SHUN;

	//扑克分析
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

	//类型判断
	if (AnalyseResult.cbFourCount == 1)  return CT_TIE_ZHI;

	if (AnalyseResult.cbLONGCount == 2)  return CT_TWO_LONG;

	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbThreeCount == 1)) return CT_HU_LU;

	if ((AnalyseResult.cbThreeCount == 1) && (AnalyseResult.cbLONGCount == 0)) return CT_THREE_TIAO;

	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbSignedCount == 3)) return CT_ONE_LONG;

	return CT_SINGLE;
}

BYTE  gamelogic::FiveFromSix(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData, BYTE cbLastCardData[]) {
	BYTE cbTempCardData[MAX_CENTERCOUNT] = { 0 };

	//拷贝数据
	CopyMemory(cbTempCardData, cbHandCardData, MAX_CENTERCOUNT);
	SortCardList(cbTempCardData, MAX_CENTERCOUNT);

	CopyMemory(cbLastCardData, cbTempCardData, MAX_CENTERCOUNT);
	BYTE cbCardKind = GetCardType(cbLastCardData, MAX_CENTERCOUNT);
	for (int i = 0; i < MAX_CENTERCOUNT; i++) {
		CopyMemory(cbTempCardData, cbHandCardData, MAX_CENTERCOUNT);
		cbTempCardData[i] = cbCenterCardData;
		SortCardList(cbTempCardData, MAX_CENTERCOUNT);
		BYTE cbTempCardKind = GetCardType(cbTempCardData, MAX_CENTERCOUNT);
		//牌型大小
		if (CompareCard(cbTempCardData, cbLastCardData, MAX_CENTERCOUNT) == 2) {
			cbCardKind = cbTempCardKind;
			CopyMemory(cbLastCardData, cbTempCardData, MAX_CENTERCOUNT);
		}
	}
	return cbCardKind;
}

//最大牌型
BYTE gamelogic::FiveFromSeven(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData[], BYTE cbCenterCardCount, BYTE cbLastCardData[], BYTE cbLastCardCount)
{
	//临时变量
	BYTE cbTempCardData[MAX_COUNT + MAX_CENTERCOUNT], cbTempLastCardData[5];
	ZeroMemory(cbTempCardData, sizeof(cbTempCardData));
	ZeroMemory(cbTempLastCardData, sizeof(cbTempLastCardData));

	//拷贝数据
	CopyMemory(cbTempCardData, cbHandCardData, sizeof(BYTE)*MAX_COUNT);
	CopyMemory(&cbTempCardData[2], cbCenterCardData, sizeof(BYTE)*MAX_CENTERCOUNT);

	//排列扑克
	SortCardList(cbTempCardData, CountArray(cbTempCardData));

	CopyMemory(cbLastCardData, cbTempCardData, sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbCardKind = GetCardType(cbLastCardData, sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbTempCardKind = 0;

	//组合算法
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			for (int k = j + 1; k < 5; k++)
			{
				for (int l = k + 1; l < 6; l++)
				{
					for (int m = l + 1; m < 7; m++)
					{
						//获取数据
						cbTempLastCardData[0] = cbTempCardData[i];
						cbTempLastCardData[1] = cbTempCardData[j];
						cbTempLastCardData[2] = cbTempCardData[k];
						cbTempLastCardData[3] = cbTempCardData[l];
						cbTempLastCardData[4] = cbTempCardData[m];

						//获取牌型
						cbTempCardKind = GetCardType(cbTempLastCardData, CountArray(cbTempLastCardData));

						//牌型大小
						if (CompareCard(cbTempLastCardData, cbLastCardData, CountArray(cbTempLastCardData)) == 2)
						{
							cbCardKind = cbTempCardKind;
							CopyMemory(cbLastCardData, cbTempLastCardData, sizeof(BYTE)*CountArray(cbTempLastCardData));
						}
					}
				}
			}
		}
	}

	return cbCardKind;
}

//查找最大
bool gamelogic::SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT], UserWinList &EndResult, const int lAddScore[]) {
	//清理数据
	INT i = 0;
	ZeroMemory(&EndResult, sizeof(EndResult));

	WORD wWinnerID = 0;
	for (i = 0; i < GAME_PLAYER; i++) {
		if (bCardData[i][0] != 0) {
			wWinnerID = i;
			break;
		}
	}

	if (i == GAME_PLAYER) return false;

	WORD wTemp = wWinnerID;
	for (i = 1; i < GAME_PLAYER; i++) {
		WORD j = (i + wTemp) % GAME_PLAYER;
		if (bCardData[j][0] == 0) continue;
		if (CompareCard(bCardData[j], bCardData[wWinnerID], MAX_CENTERCOUNT) > 1) {
			wWinnerID = j;
		}
	}

	//查找相同数据
	EndResult.wWinerList[EndResult.bSameCount++] = wWinnerID;
	for (i = 0; i < GAME_PLAYER; i++) {
		if (i == wWinnerID || bCardData[i][0] == 0) continue;
		if (CompareCard(bCardData[i], bCardData[wWinnerID], MAX_CENTERCOUNT) == FALSE) {
			EndResult.wWinerList[EndResult.bSameCount++] = i;
		}
	}

	//从小到大
	if (EndResult.bSameCount > 1 && lAddScore != NULL) {
		int iSameCount = (int)EndResult.bSameCount;
		while ((iSameCount--) > 0) {
			int lTempSocre = lAddScore[EndResult.wWinerList[iSameCount]];
			for (i = iSameCount - 1; i >= 0; i--) {
				if (lTempSocre < lAddScore[EndResult.wWinerList[i]]) {
					lTempSocre = lAddScore[EndResult.wWinerList[i]];
					WORD tmp = EndResult.wWinerList[iSameCount];
					EndResult.wWinerList[iSameCount] = EndResult.wWinerList[i];
					EndResult.wWinerList[i] = tmp;
				}
			}
		}
	}
	return true;
}

//排列扑克
void gamelogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount) {
	//转换数值
	BYTE cbLogicValue[FULL_COUNT] = { 0 };
	for (BYTE i = 0; i < cbCardCount; i++) cbLogicValue[i] = GetCardLogicValue(cbCardData[i]);

	//排序操作
	bool bSorted = true;
	BYTE cbTempData ;
	BYTE bLast = cbCardCount - 1;
	do {
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++) {
			if ((cbLogicValue[i] < cbLogicValue[i + 1]) || ((cbLogicValue[i] == cbLogicValue[i + 1]) && (cbCardData[i] < cbCardData[i + 1]))) {
				cbTempData = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbTempData;
				cbTempData = cbLogicValue[i];
				cbLogicValue[i] = cbLogicValue[i + 1];
				cbLogicValue[i + 1] = cbTempData;
				bSorted = false;
			}
		}
		bLast--;
	} while (!bSorted);
}

//逻辑数值
BYTE gamelogic::GetCardLogicValue(BYTE cbCardData) {
	//扑克属性
	//BYTE bCardColor = GetCardColor(cbCardData);
	BYTE bCardValue = GetCardValue(cbCardData);
	return (bCardValue == 1) ? (bCardValue + 13) : bCardValue;
}

//对比扑克
BYTE gamelogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount) {
	//获取类型
	BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
	BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

	if (cbFirstType > cbNextType) return 2;

	if (cbFirstType < cbNextType) return 1;

	//简单类型
	switch (cbFirstType) {
		case CT_SINGLE: {   //单牌
			BYTE i = 0;
			for (i = 0; i < cbCardCount; i++) {
				BYTE cbNextValue = GetCardLogicValue(cbNextData[i]);
				BYTE cbFirstValue = GetCardLogicValue(cbFirstData[i]);
				if (cbFirstValue > cbNextValue)
					return 2;
				else if (cbFirstValue < cbNextValue)
					return 1;
				else
					continue;
			}
			if (i == cbCardCount) return 0;
			break;
		}
		case CT_ONE_LONG:   //对子
		case CT_TWO_LONG:   //两对
		case CT_THREE_TIAO:   //三条
		case CT_TIE_ZHI:    //铁支
		case CT_HU_LU: {    //葫芦
			//分析扑克
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextData, cbCardCount, AnalyseResultNext);
			AnalysebCardData(cbFirstData, cbCardCount, AnalyseResultFirst);

			//四条数值
			if (AnalyseResultFirst.cbFourCount > 0) {
				BYTE cbNextValue = AnalyseResultNext.cbFourLogicVolue[0];
				BYTE cbFirstValue = AnalyseResultFirst.cbFourLogicVolue[0];

				if (cbFirstValue != cbNextValue) return (cbFirstValue > cbNextValue) ? 2 : 1;

				cbFirstValue = AnalyseResultFirst.cbSignedLogicVolue[0];
				cbNextValue = AnalyseResultNext.cbSignedLogicVolue[0];

				if (cbFirstValue != cbNextValue)
					return (cbFirstValue > cbNextValue) ? 2 : 1;
				else
					return 0;
			}

			//三条数值
			if (AnalyseResultFirst.cbThreeCount > 0) {
				BYTE tmpNextValue = AnalyseResultNext.cbThreeLogicVolue[0];
				BYTE tmpFirstValue = AnalyseResultFirst.cbThreeLogicVolue[0];

				if (tmpFirstValue != tmpNextValue)  return (tmpFirstValue > tmpNextValue) ? 2 : 1;

				if (CT_HU_LU == cbFirstType) {
					tmpFirstValue = AnalyseResultFirst.cbLONGLogicVolue[0];
					tmpNextValue = AnalyseResultNext.cbLONGLogicVolue[0];
					if (tmpFirstValue != tmpNextValue) {
						return (tmpFirstValue > tmpNextValue) ? 2 : 1;
					} else {
						return 0;
					}
				} else {
					BYTE i = 0;
					for (i = 0; i < AnalyseResultFirst.cbSignedCount; i++) {
						BYTE tmpNextValue1 = AnalyseResultNext.cbSignedLogicVolue[i];
						BYTE tmpFirstValue1 = AnalyseResultFirst.cbSignedLogicVolue[i];
						if (tmpFirstValue1 > tmpNextValue1)
							return 2;
						else if (tmpFirstValue1 < tmpNextValue1)
							return 1;
						else
							continue;

					}
					if (i == AnalyseResultFirst.cbSignedCount) return 0;
				}
			}

			//对子数值
			BYTE i2 = 0;
			for (i2 = 0; i2 < AnalyseResultFirst.cbLONGCount; i2++) {
				BYTE cbNextValue = AnalyseResultNext.cbLONGLogicVolue[i2];
				BYTE cbFirstValue = AnalyseResultFirst.cbLONGLogicVolue[i2];
				if (cbFirstValue > cbNextValue)
					return 2;
				else if (cbFirstValue < cbNextValue)
					return 1;
				else
					continue;
			}

			//散牌数值
			for (i2 = 0; i2 < AnalyseResultFirst.cbSignedCount; i2++) {
				BYTE cbNextValue = AnalyseResultNext.cbSignedLogicVolue[i2];
				BYTE cbFirstValue = AnalyseResultFirst.cbSignedLogicVolue[i2];
				if (cbFirstValue > cbNextValue)
					return 2;
				else if (cbFirstValue < cbNextValue)
					return 1;
				else
					continue;
			}
			if (i2 == AnalyseResultFirst.cbSignedCount) return 0;
			break;
		}
		case CT_SHUN_ZI:    //顺子
		case CT_TONG_HUA_SHUN: { //同花顺
			BYTE cbNextValue = GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstValue = GetCardLogicValue(cbFirstData[0]);
			if (cbFirstValue == cbNextValue) return 0;
			return (cbFirstValue > cbNextValue) ? 2 : 1;
		}
		break;
		case CT_TONG_HUA: { //同花
			BYTE i = 0;
			for (i = 0; i < cbCardCount; i++) {
				BYTE cbNextValue = GetCardLogicValue(cbNextData[i]);
				BYTE cbFirstValue = GetCardLogicValue(cbFirstData[i]);
				if (cbFirstValue == cbNextValue) continue;
				return (cbFirstValue > cbNextValue) ? 2 : 1;
			}
			if (i == cbCardCount) return 0;
		}
		break;
	}

	return  0;
}

//分析扑克
void gamelogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult& AnalyseResult) {
	//设置结果
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i = 0; i < cbCardCount; i++) {
		//变量定义
		BYTE cbSameCount = 1;
		BYTE cbSameCardData[4] = { cbCardData[i], 0, 0, 0 };
		BYTE cbLogicValue = GetCardLogicValue(cbCardData[i]);

		//获取同牌
		for (int j = i + 1; j < cbCardCount; j++) {
			//逻辑对比
			if (GetCardLogicValue(cbCardData[j]) != cbLogicValue) break;

			//设置扑克
			cbSameCardData[cbSameCount++] = cbCardData[j];
		}

		//保存结果
		switch (cbSameCount) {
			case 1:   //单张
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 2:   //两张
				AnalyseResult.cbLONGLogicVolue[AnalyseResult.cbLONGCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbLONGCardData[(AnalyseResult.cbLONGCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 3:   //三张
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 4:   //四张
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
		}

		//设置递增
		i += cbSameCount - 1;
	}
}

int gamelogic::AnalyCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) {
	int nTmp = 0, iMax = 0;
	BYTE  bCard[MAX_CENTERCOUNT] = { 0 };
	BYTE  nColor[4][MAX_CENTERCOUNT] = { 0 };
	BYTE  nColorCount[4] = { 0 };

	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
	CopyMemory(bCard, cbCardData, cbCardCount);

	SortCardList(bCard, cbCardCount);
	AnalysebCardData(bCard, cbCardCount, AnalyseResult);

	for (BYTE i = 0; i < cbCardCount; i++) {
		BYTE cbColor = GetCardColor(bCard[i]);
		cbColor >>= 4;
		nColor[cbColor][nColorCount[cbColor]++] = bCard[i];
		if (nColorCount[cbColor] > nTmp) {
			nTmp = nColorCount[cbColor];
			iMax = cbColor;
		}
	}
	AnalyseResult.cbColorCount = nTmp;
	CopyMemory(AnalyseResult.cbColorCardData, nColor[iMax], nTmp);
	if (nTmp == 4) AnalyseResult.cbBuColor = 9;

	nTmp = iMax = 0;
	for (BYTE i = 0; i < cbCardCount - 1; i++) {
		BYTE cbVal = GetCardLogicValue(bCard[i]);
		nColor[i][nColorCount[i]++] = bCard[i];

		for (BYTE j = i + 1; j < cbCardCount; j++) {
			BYTE cbNext = GetCardLogicValue(bCard[j]);
			if (cbNext + nColorCount[i] == cbVal) {
				nColor[i][nColorCount[i]++] = bCard[j];
				if (nColorCount[i] > nTmp) {
					nTmp = nColorCount[i];
					iMax = i;
				}
			} else {
				break;
			}
		}
	}
	AnalyseResult.cbShunCount = nTmp;
	CopyMemory(AnalyseResult.cbShunData, nColor[iMax], nTmp);
	if (nTmp == 4) {
		BYTE bFirst = GetCardLogicValue(AnalyseResult.cbShunData[0]);
		BYTE bLast = GetCardLogicValue(AnalyseResult.cbShunData[nTmp - 1]);
		if (bFirst == 14 || bLast == 2)
			AnalyseResult.cbBuShun = 4;
		else
			AnalyseResult.cbBuShun = 8;
	}
	AnalyseResult.MaxVal = GetCardLogicValue(bCard[0]);
	AnalyseResult.MinVal = GetCardLogicValue(bCard[cbCardCount - 1]);

	if (cbCardCount != MAX_CENTERCOUNT) return 0;
	if (AnalyseResult.cbColorCount == MAX_CENTERCOUNT && AnalyseResult.cbShunCount == MAX_CENTERCOUNT) return CT_TONG_HUA_SHUN;
	if (AnalyseResult.cbColorCount == MAX_CENTERCOUNT) return CT_TONG_HUA;
	if (AnalyseResult.cbShunCount == MAX_CENTERCOUNT)  return CT_SHUN_ZI;
	if (AnalyseResult.cbFourCount == 1)  return CT_TIE_ZHI;
	if (AnalyseResult.cbLONGCount == 2)  return CT_TWO_LONG;
	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbThreeCount == 1)) return CT_HU_LU;
	if ((AnalyseResult.cbThreeCount == 1) && (AnalyseResult.cbLONGCount == 0)) return CT_THREE_TIAO;
	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbSignedCount == 3)) return CT_ONE_LONG;
	return CT_SINGLE;
}

int  gamelogic::AnalyCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbOwnCard[], tagAnalyseResult* pAnaly, tagAnalyseResult& AnalyseResult) {
	BYTE bOneMin = 0, bOneMax = 0;
	BYTE  bCard[MAX_CENTERCOUNT] = { 0 };
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	if (cbOwnCard[0] != 0) bOneMin = GetCardLogicValue(cbCardData[0]);
	if (cbOwnCard[1] != 0) bOneMax = GetCardLogicValue(cbCardData[1]);

	CopyMemory(bCard, cbCardData, cbCardCount);
	SortCardList(bCard, cbCardCount);
	AnalyseResult.bType = (BYTE)AnalyCardData(bCard, cbCardCount, AnalyseResult);
	if (AnalyseResult.bType > CT_THREE_TIAO) return BIG_CARD;
	if (AnalyseResult.bType == CT_THREE_TIAO) {
		if (pAnaly->cbThreeCount == 0)
			return BIG_CARD;
		else
			return NONE_CARD;
	}
	if (AnalyseResult.bType == CT_TWO_LONG) {
		if (pAnaly->cbLONGCount < 2)
			return DTWO_CARD;
		else
			return NONE_CARD;
	}

	if (AnalyseResult.bType == CT_ONE_LONG) {
		if (pAnaly->cbLONGCount == 0 && pAnaly->cbColorCount <= 2) AnalyseResult.cbBuBu += 2;
	} else {
		if (bOneMax >= 10 && pAnaly->MaxVal < bOneMax) AnalyseResult.cbBuBu += 3;
		if (bOneMin >= 10 && pAnaly->MaxVal < bOneMin) AnalyseResult.cbBuBu += 3;
	}
	AnalyseResult.cbBuBu += AnalyseResult.cbBuColor;
	AnalyseResult.cbBuBu += AnalyseResult.cbBuShun;
	if (AnalyseResult.cbBuBu >= 8) return BUBU_CARD;

	if (AnalyseResult.bType == CT_ONE_LONG) {
		if (pAnaly->cbLONGCount > 0) return NONE_CARD;
		if (GetCardLogicValue(AnalyseResult.cbLONGCardData[0]) <= 6) return NONE_CARD;
		return TWO_CARD;
	}
	return NONE_CARD;
}

//HK 
//分析扑克
void gamelogic::AnalysebCardDataHK(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResultHK & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//变量定义
		BYTE cbSameCount=1;
		BYTE cbSameCardData[4]={cbCardData[i],0,0,0};
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//获取同牌
		for (int j=i+1;j<cbCardCount;j++)
		{
			//逻辑对比
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//设置扑克
			cbSameCardData[cbSameCount++]=cbCardData[j];
		}

		//保存结果
		switch (cbSameCount)
		{
		case 1:		//单张
			{
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 2:		//两张
			{
				AnalyseResult.cbDoubleLogicVolue[AnalyseResult.cbDoubleCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbDoubleCardData[(AnalyseResult.cbDoubleCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 3:		//三张
			{
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 4:		//四张
			{
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		}

		//设置递增
		i+=cbSameCount-1;
	}

	return;
}

//获取类型
BYTE gamelogic::GetCardGenre(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE cbCardTemp[MAX_COUNT];
	ZeroMemory(cbCardTemp, sizeof(cbCardTemp));

	CopyMemory(cbCardTemp,cbCardData, cbCardCount * sizeof(BYTE));
	SortCardList(cbCardTemp,cbCardCount);

	//简单牌形
	switch (cbCardCount)
	{
	case 1: //单牌
		{
			return CT_SINGLE;
		}
	case 2:	//对牌
		{
			return (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1]))?CT_ONE_LONG:CT_SINGLE;
		}
	}

	//五条类型
	if (cbCardCount>=MAX_COUNT)
	{
		//变量定义
		bool cbSameColor=true,bLineCard=true;
		BYTE cbFirstColor=GetCardColor(cbCardTemp[0]);
		BYTE cbFirstValue=GetCardLogicValue(cbCardTemp[0]);

		//牌形分析
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//数据分析
			if (GetCardColor(cbCardTemp[i])!=cbFirstColor) cbSameColor=false;
			if (cbFirstValue!=(GetCardLogicValue(cbCardTemp[i])+i)) bLineCard=false;

			//结束判断
			if ((cbSameColor==false)&&(bLineCard==false)) break;
		}

		//顺子类型
		if ((cbSameColor==false)&&(bLineCard==true)) return CT_SHUN_ZI;

		//同花类型
		if ((cbSameColor==true)&&(bLineCard==false)) return CT_TONG_HUA;

		//同花顺类型
		if ((cbSameColor==true)&&(bLineCard==true)) return CT_TONG_HUA_SHUN;
	}

	//扑克分析
	tagAnalyseResultHK AnalyseResult;
	AnalysebCardDataHK(cbCardTemp,cbCardCount,AnalyseResult);

	//四条类型
	if (AnalyseResult.cbFourCount==1) return CT_TIE_ZHI;

	//两对类型
	if (AnalyseResult.cbDoubleCount==2) return CT_TWO_LONG;

	//对牌类型
	if ((AnalyseResult.cbDoubleCount==1)&&(AnalyseResult.cbThreeCount==0)) return CT_ONE_LONG;

	//葫芦类型
	if (AnalyseResult.cbThreeCount==1) return (AnalyseResult.cbDoubleCount==1)?CT_HU_LU:CT_THREE_TIAO;

	return CT_SINGLE;
}


