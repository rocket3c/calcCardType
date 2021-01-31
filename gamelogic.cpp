#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
	//BYTE gamelogic::m_cbCardData[FULL_COUNT] = {
	//	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� A - K
	//	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� A - K
	//	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� A - K
	//	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� A - K
	 // };

//���캯��
gamelogic::gamelogic() {
}

//��������
gamelogic::~gamelogic() {
}

//ϴ��
void gamelogic::shuffle_poker(BYTE(&poker)[FULL_COUNT], int game_list) {
	std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
	//std::uniform_int_distribution<> distr(0, 52);	
	BYTE m_cbCardData[FULL_COUNT] ={0};
	
	if(game_list == 1) //���ݵ��� 52�� 
	{
		BYTE CardData[FULL_COUNT] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� A - K
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� A - K
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� A - K
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� A - K
	  };
	  	CopyMemory(m_cbCardData, CardData, sizeof(CardData));
	}
    if(game_list == 2) //��ʽ���ŵ��� 28�� 
	{
		BYTE CardData[FULL_COUNT] = {
		0x01, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� A - K
		0x11, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� A - K
		0x21, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� A - K
		0x31, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� A - K	
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

//��ȡ����
BYTE gamelogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount) {
	//����У��
	if (cbCardCount != MAX_CENTERCOUNT) return 0;
	//��������
	bool cbSameColor = true, bLineCard = true;
	BYTE cbFirstColor = GetCardColor(cbCardData[0]);
	BYTE cbFirstValue = GetCardLogicValue(cbCardData[0]);

	//���η���
	for (BYTE i = 1; i < cbCardCount; i++) {
		if (GetCardColor(cbCardData[i]) != cbFirstColor)
			cbSameColor = false;
		if (cbFirstValue != (GetCardLogicValue(cbCardData[i]) + i))
			bLineCard = false;
		if (!cbSameColor && !bLineCard) break;
	}

	//�ʼ�ͬ��˳
	if (cbSameColor && bLineCard && cbFirstValue == 14) return CT_KING_TONG_HUA_SHUN;

	//˳������
	if (!cbSameColor && bLineCard) return CT_SHUN_ZI;

	//ͬ������
	if (cbSameColor && !bLineCard) return CT_TONG_HUA;

	//ͬ��˳����
	if (cbSameColor && bLineCard)  return CT_TONG_HUA_SHUN;

	//�˿˷���
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbFourCount == 1)  return CT_TIE_ZHI;

	if (AnalyseResult.cbLONGCount == 2)  return CT_TWO_LONG;

	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbThreeCount == 1)) return CT_HU_LU;

	if ((AnalyseResult.cbThreeCount == 1) && (AnalyseResult.cbLONGCount == 0)) return CT_THREE_TIAO;

	if ((AnalyseResult.cbLONGCount == 1) && (AnalyseResult.cbSignedCount == 3)) return CT_ONE_LONG;

	return CT_SINGLE;
}

BYTE  gamelogic::FiveFromSix(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData, BYTE cbLastCardData[]) {
	BYTE cbTempCardData[MAX_CENTERCOUNT] = { 0 };

	//��������
	CopyMemory(cbTempCardData, cbHandCardData, MAX_CENTERCOUNT);
	SortCardList(cbTempCardData, MAX_CENTERCOUNT);

	CopyMemory(cbLastCardData, cbTempCardData, MAX_CENTERCOUNT);
	BYTE cbCardKind = GetCardType(cbLastCardData, MAX_CENTERCOUNT);
	for (int i = 0; i < MAX_CENTERCOUNT; i++) {
		CopyMemory(cbTempCardData, cbHandCardData, MAX_CENTERCOUNT);
		cbTempCardData[i] = cbCenterCardData;
		SortCardList(cbTempCardData, MAX_CENTERCOUNT);
		BYTE cbTempCardKind = GetCardType(cbTempCardData, MAX_CENTERCOUNT);
		//���ʹ�С
		if (CompareCard(cbTempCardData, cbLastCardData, MAX_CENTERCOUNT) == 2) {
			cbCardKind = cbTempCardKind;
			CopyMemory(cbLastCardData, cbTempCardData, MAX_CENTERCOUNT);
		}
	}
	return cbCardKind;
}

//�������
BYTE gamelogic::FiveFromSeven(BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbCenterCardData[], BYTE cbCenterCardCount, BYTE cbLastCardData[], BYTE cbLastCardCount)
{
	//��ʱ����
	BYTE cbTempCardData[MAX_COUNT + MAX_CENTERCOUNT], cbTempLastCardData[5];
	ZeroMemory(cbTempCardData, sizeof(cbTempCardData));
	ZeroMemory(cbTempLastCardData, sizeof(cbTempLastCardData));

	//��������
	CopyMemory(cbTempCardData, cbHandCardData, sizeof(BYTE)*MAX_COUNT);
	CopyMemory(&cbTempCardData[2], cbCenterCardData, sizeof(BYTE)*MAX_CENTERCOUNT);

	//�����˿�
	SortCardList(cbTempCardData, CountArray(cbTempCardData));

	CopyMemory(cbLastCardData, cbTempCardData, sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbCardKind = GetCardType(cbLastCardData, sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbTempCardKind = 0;

	//����㷨
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
						//��ȡ����
						cbTempLastCardData[0] = cbTempCardData[i];
						cbTempLastCardData[1] = cbTempCardData[j];
						cbTempLastCardData[2] = cbTempCardData[k];
						cbTempLastCardData[3] = cbTempCardData[l];
						cbTempLastCardData[4] = cbTempCardData[m];

						//��ȡ����
						cbTempCardKind = GetCardType(cbTempLastCardData, CountArray(cbTempLastCardData));

						//���ʹ�С
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

//�������
bool gamelogic::SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT], UserWinList &EndResult, const int lAddScore[]) {
	//��������
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

	//������ͬ����
	EndResult.wWinerList[EndResult.bSameCount++] = wWinnerID;
	for (i = 0; i < GAME_PLAYER; i++) {
		if (i == wWinnerID || bCardData[i][0] == 0) continue;
		if (CompareCard(bCardData[i], bCardData[wWinnerID], MAX_CENTERCOUNT) == FALSE) {
			EndResult.wWinerList[EndResult.bSameCount++] = i;
		}
	}

	//��С����
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

//�����˿�
void gamelogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount) {
	//ת����ֵ
	BYTE cbLogicValue[FULL_COUNT] = { 0 };
	for (BYTE i = 0; i < cbCardCount; i++) cbLogicValue[i] = GetCardLogicValue(cbCardData[i]);

	//�������
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

//�߼���ֵ
BYTE gamelogic::GetCardLogicValue(BYTE cbCardData) {
	//�˿�����
	//BYTE bCardColor = GetCardColor(cbCardData);
	BYTE bCardValue = GetCardValue(cbCardData);
	return (bCardValue == 1) ? (bCardValue + 13) : bCardValue;
}

//�Ա��˿�
BYTE gamelogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount) {
	//��ȡ����
	BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
	BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

	if (cbFirstType > cbNextType) return 2;

	if (cbFirstType < cbNextType) return 1;

	//������
	switch (cbFirstType) {
		case CT_SINGLE: {   //����
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
		case CT_ONE_LONG:   //����
		case CT_TWO_LONG:   //����
		case CT_THREE_TIAO:   //����
		case CT_TIE_ZHI:    //��֧
		case CT_HU_LU: {    //��«
			//�����˿�
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextData, cbCardCount, AnalyseResultNext);
			AnalysebCardData(cbFirstData, cbCardCount, AnalyseResultFirst);

			//������ֵ
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

			//������ֵ
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

			//������ֵ
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

			//ɢ����ֵ
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
		case CT_SHUN_ZI:    //˳��
		case CT_TONG_HUA_SHUN: { //ͬ��˳
			BYTE cbNextValue = GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstValue = GetCardLogicValue(cbFirstData[0]);
			if (cbFirstValue == cbNextValue) return 0;
			return (cbFirstValue > cbNextValue) ? 2 : 1;
		}
		break;
		case CT_TONG_HUA: { //ͬ��
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

//�����˿�
void gamelogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult& AnalyseResult) {
	//���ý��
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i = 0; i < cbCardCount; i++) {
		//��������
		BYTE cbSameCount = 1;
		BYTE cbSameCardData[4] = { cbCardData[i], 0, 0, 0 };
		BYTE cbLogicValue = GetCardLogicValue(cbCardData[i]);

		//��ȡͬ��
		for (int j = i + 1; j < cbCardCount; j++) {
			//�߼��Ա�
			if (GetCardLogicValue(cbCardData[j]) != cbLogicValue) break;

			//�����˿�
			cbSameCardData[cbSameCount++] = cbCardData[j];
		}

		//������
		switch (cbSameCount) {
			case 1:   //����
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 2:   //����
				AnalyseResult.cbLONGLogicVolue[AnalyseResult.cbLONGCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbLONGCardData[(AnalyseResult.cbLONGCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 3:   //����
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
			case 4:   //����
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount] = cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount], cbSameCardData, cbSameCount);
				break;
		}

		//���õ���
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
//�����˿�
void gamelogic::AnalysebCardDataHK(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResultHK & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1;
		BYTE cbSameCardData[4]={cbCardData[i],0,0,0};
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//��ȡͬ��
		for (int j=i+1;j<cbCardCount;j++)
		{
			//�߼��Ա�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//�����˿�
			cbSameCardData[cbSameCount++]=cbCardData[j];
		}

		//������
		switch (cbSameCount)
		{
		case 1:		//����
			{
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 2:		//����
			{
				AnalyseResult.cbDoubleLogicVolue[AnalyseResult.cbDoubleCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbDoubleCardData[(AnalyseResult.cbDoubleCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 3:		//����
			{
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 4:		//����
			{
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		}

		//���õ���
		i+=cbSameCount-1;
	}

	return;
}

//��ȡ����
BYTE gamelogic::GetCardGenre(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE cbCardTemp[MAX_COUNT];
	ZeroMemory(cbCardTemp, sizeof(cbCardTemp));

	CopyMemory(cbCardTemp,cbCardData, cbCardCount * sizeof(BYTE));
	SortCardList(cbCardTemp,cbCardCount);

	//������
	switch (cbCardCount)
	{
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//����
		{
			return (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1]))?CT_ONE_LONG:CT_SINGLE;
		}
	}

	//��������
	if (cbCardCount>=MAX_COUNT)
	{
		//��������
		bool cbSameColor=true,bLineCard=true;
		BYTE cbFirstColor=GetCardColor(cbCardTemp[0]);
		BYTE cbFirstValue=GetCardLogicValue(cbCardTemp[0]);

		//���η���
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//���ݷ���
			if (GetCardColor(cbCardTemp[i])!=cbFirstColor) cbSameColor=false;
			if (cbFirstValue!=(GetCardLogicValue(cbCardTemp[i])+i)) bLineCard=false;

			//�����ж�
			if ((cbSameColor==false)&&(bLineCard==false)) break;
		}

		//˳������
		if ((cbSameColor==false)&&(bLineCard==true)) return CT_SHUN_ZI;

		//ͬ������
		if ((cbSameColor==true)&&(bLineCard==false)) return CT_TONG_HUA;

		//ͬ��˳����
		if ((cbSameColor==true)&&(bLineCard==true)) return CT_TONG_HUA_SHUN;
	}

	//�˿˷���
	tagAnalyseResultHK AnalyseResult;
	AnalysebCardDataHK(cbCardTemp,cbCardCount,AnalyseResult);

	//��������
	if (AnalyseResult.cbFourCount==1) return CT_TIE_ZHI;

	//��������
	if (AnalyseResult.cbDoubleCount==2) return CT_TWO_LONG;

	//��������
	if ((AnalyseResult.cbDoubleCount==1)&&(AnalyseResult.cbThreeCount==0)) return CT_ONE_LONG;

	//��«����
	if (AnalyseResult.cbThreeCount==1) return (AnalyseResult.cbDoubleCount==1)?CT_HU_LU:CT_THREE_TIAO;

	return CT_SINGLE;
}


