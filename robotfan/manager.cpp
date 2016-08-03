#include "stdafx.h"
#include "manager.h"

extern int number;

char manager(char input, int &number)
{
	char state = STOP , area;
	Mat originalImage;
	time_t start, end, time;
	int k;
	area = input;
	char filename[256];
	if (state==STOP)
	{
		originalImage = video(number);
		if ((number%2 != 0) || (originalImage.empty())) return 'z';
		char yb, rb, gb, bb;
		char drink;
		if (area == AREAA) 
		{
			yb = detectYellowBlock(originalImage);
			voice(yb);
			return yb;
		}
		else if (area == AREAB)
		{
			rb = '1';
			gb = '1';
			bb = '1';
			rb = detectRedBlock(originalImage);
			gb = detectGreenBlock(originalImage);
			bb = detectBlueBlock(originalImage);
			if (rb == REDBLOCK) 
			{
				voice(rb);
				return rb;
			}
			if (gb == GREENBLOCK) 
			{
				voice(gb);
				return gb;
			}
			else if (bb == BLUEBLOCK)
			{
				voice(bb);
				return bb;
			}
			else
			{
				voice(NOTHING);
				return NOTHING;
			}
		}
		else if (area == AREAC)
		{
			char drink = detect_areac(originalImage);
			voice(drink);
			cout << drink << endl;
			return drink;
		}
		else if (area == AREAD)
		{
			char drink = detect_aread(originalImage);
			voice(drink);
			cout << drink << endl;
			return drink;
		}
		else if (area == AREAE) //exception
		{
			char drink = detect_areae(originalImage);
			voice(drink);
			cout << drink << endl;
			return drink;
		}
	}
	//system("pause");
	return NOTHING;
}

