#include<stdio.h>
#include <iostream>
#include "gamelogic.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace std;

//计算德州 
int calc_dz(long loop)
{
     BYTE poker[FULL_COUNT] ={0};
        gamelogic game;

    	cout << "\nTexas Hold'em poker start calc, Total Loop:" << loop << endl;
    	int kindcount[10] = {0};
	    
        for(long j = 0; j < loop; j++)
        {
		 game.shuffle_poker(poker,1); //德州的牌     
	    BYTE hand_card[2] = {poker[0],poker[1]};
		BYTE cent_card[5] = {poker[2],poker[3],poker[4],poker[5],poker[6]};	
    	BYTE last_card[5] = {0};
    	
    	BYTE kind = game.FiveFromSeven(hand_card, MAX_COUNT,
				cent_card, MAX_CENTERCOUNT, last_card, MAX_CENTERCOUNT);
    	
		switch(kind)
		{
			case 1: 
			kindcount[0] +=1;
			break;
			case 2:
			kindcount[1] +=1;
			break;
			case 3:
			kindcount[2] +=1;
			break; 
			case 4:
			kindcount[3] +=1;
			break; 
			case 5:
			kindcount[4] +=1;
			break;
			case 6:
			kindcount[5] +=1;
			break;
			case 7:
			kindcount[6] +=1;
			break; 
			case 8:
			kindcount[7] +=1;
			break;
			case 9:
			kindcount[8] +=1;
			break;
			case 10:
			kindcount[9] +=1;
			break;
				deault: break;
		} 
		}	
		
		double single = ((double)kindcount[0] / (double) loop)*100;
        double pair = ((double)kindcount[1] / (double)loop)*100;
    	double two_pair = ((double)kindcount[2] / (double) loop)*100;
        double three = ((double)kindcount[3] / (double) loop)*100;
        double shun =((double)kindcount[4] / (double) loop)*100;
        double tonghua =((double)kindcount[5] / (double) loop)*100;   
        double hulu = ((double)kindcount[6] / (double) loop)*100;
        double tiezhi =((double)kindcount[7] / (double) loop)*100; 
        double tonghuashun = ((double)kindcount[8] / (double) loop)*100;
        double royalshun =((double)kindcount[9] / (double) loop)*100;
		
		cout << "         single: " << kindcount[0] << "         single frequency: " << single <<"%" << endl;
		cout << "       one pair: " << kindcount[1] << "       one pair frequency: " << pair <<"%" << endl;
		cout << "       two pair: " << kindcount[2] << "       two pair frequency: " << two_pair <<"%" << endl;
		cout << "          trips: " << kindcount[3] << "          trips frequency: " << three <<"%" << endl;
		cout << "       straight: " << kindcount[4] << "       straight frequency: " << shun <<"%" << endl;
		cout << "          flush: " << kindcount[5] << "          flush frequency: " << tonghua <<"%" << endl;
		cout << "     full house: " << kindcount[6] << "     full house frequency: " << hulu <<"%" << endl;
		cout << "          quads: " << kindcount[7] << "          quads frequency: " << tiezhi <<"%" << endl;
		cout << " straight flush: " << kindcount[8] << " straight flush frequency: " << tonghuashun <<"%" << endl;
		cout << "          royal: " << kindcount[9] << "          royal frequency: " << royalshun <<"%" << endl;

		return 0;	
}

//计算港式五张 
int calc_hk(long loop)
{
     BYTE poker[FULL_COUNT] ={0};
        gamelogic game;
    	cout << "\nHK Five poker start calc, Total Loop:" << loop << endl;
    	int kindcount[9] = {0};
	    
        for(long j = 0; j < loop; j++)
        {
		 game.shuffle_poker(poker,2);   //2:港式五张的牌  
		BYTE hand_card[5] = {poker[0],poker[1],poker[2],poker[3],poker[4]};	
    	
    	BYTE kind = game.GetCardGenre(hand_card, sizeof(hand_card));
    	
		switch(kind)
		{
			case 1: 
			kindcount[0] +=1;
			break;
			case 2:
			kindcount[1] +=1;
			break;
			case 3:
			kindcount[2] +=1;
			break; 
			case 4:
			kindcount[3] +=1;
			break; 
			case 5:
			kindcount[4] +=1;
			break;
			case 6:
			kindcount[5] +=1;
			break;
			case 7:
			kindcount[6] +=1;
			break; 
			case 8:
			kindcount[7] +=1;
			break;
			case 9:
			case 10:
			kindcount[8] +=1;
			break;
				deault: break;
		} 
		}	
		
		double single = ((double)kindcount[0] / (double) loop)*100;
        double pair = ((double)kindcount[1] / (double)loop)*100;
    	double two_pair = ((double)kindcount[2] / (double) loop)*100;
        double three = ((double)kindcount[3] / (double) loop)*100;
        double shun =((double)kindcount[4] / (double) loop)*100;
        double tonghua =((double)kindcount[5] / (double) loop)*100;
        double hulu = ((double)kindcount[6] / (double) loop)*100;
        double tiezhi =((double)kindcount[7] / (double) loop)*100; 
        double tonghuashun = ((double)kindcount[8] / (double) loop)*100;

		cout << "         single: " << kindcount[0] << "         single frequency: " << single <<"%" << endl;
		cout << "       one pair: " << kindcount[1] << "       one pair frequency: " << pair <<"%" << endl;
		cout << "       two pair: " << kindcount[2] << "       two pair frequency: " << two_pair <<"%" << endl;
		cout << "          trips: " << kindcount[3] << "          trips frequency: " << three <<"%" << endl;
		cout << "       straight: " << kindcount[4] << "       straight frequency: " << shun <<"%" << endl;
		cout << "          flush: " << kindcount[5] << "          flush frequency: " << tonghua <<"%" << endl;
		cout << "     full house: " << kindcount[6] << "     full house frequency: " << hulu <<"%" << endl;
		cout << "          quads: " << kindcount[7] << "          quads frequency: " << tiezhi <<"%" << endl;
		cout << " straight flush: " << kindcount[8] << " straight flush frequency: " << tonghuashun <<"%" << endl;

		return 0;	
}


int main(int argc, char** argv)
 {
 	if(argc < 3)
 	{
 		cout << " Start parameter error, 3 parameters are required!" << endl;
 		cout << " argv[1] is project name" << endl;
 		cout << " argv[2] is loop count" << endl;
 		cout << " for example start dz exe file (Windows:dz 1 10000)or(Linux: dz 1 10000)" << endl;
 		cout << " argv[1]: 1:dz 2:hk" << endl;
 		 return 1;
	}
   long count = atol(argv[1]);
   long loop = atol(argv[2]);
    
   switch(count)
   {
   	case 1:	
   	{
	  calc_dz(loop); 
      break;
	}
     case 2:
	 {
	  calc_hk(loop); 
      break;
	 }	
    default: break;
   }
  
	 
	return 0;
}




