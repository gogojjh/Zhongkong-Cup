#include "stdafx.h"
#include "voice.h"
#include "manager.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

void voice(const char &c)
{
	switch (c)
	{
		case YELLOWBLOCK:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\yellowblock.wav"), NULL, SND_FILENAME); 
			break;
		}
		case REDBLOCK:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\redblock.wav"), NULL, SND_FILENAME); 
			break;
		}
		case GREENBLOCK:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\greenblock.wav"), NULL, SND_FILENAME); 
			break;
		}
		case BLUEBLOCK:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\blueblock.wav"), NULL, SND_FILENAME); 
			break;
		}
		case FENDA:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\jiaduobao.wav"), NULL, SND_FILENAME); 
			break;
		}
		case JIANYIKELE:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\jianyikele.wav"), NULL, SND_FILENAME); 
			break;
		}
		case XUEBI:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\xuebi.wav"), NULL, SND_FILENAME); 
			break;
		}
		case YANGLEDUO:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\yangleduo.wav"), NULL, SND_FILENAME); 
			break;
		}
		case QQXING:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\qqxing.wav"), NULL, SND_FILENAME); 
			break;
		}
		case MENGNIU:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\mengniu.wav"), NULL, SND_FILENAME); 
			break;
		}
		case NOTHING:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\none.wav"), NULL, SND_FILENAME); 
			break;
		}
		case BEGIN:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\begin.wav"), NULL, SND_FILENAME); 
			break;
		}
		case AREAA:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\areaa.wav"), NULL, SND_FILENAME); 
			break;
		}
		case AREAB:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\areab.wav"), NULL, SND_FILENAME); 
			break;
		}
		case AREAC:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\areac.wav"), NULL, SND_FILENAME); 
			break;
		}
		case AREAD:
		{
			PlaySound(TEXT("D:\\user\\Documents\\Visual Studio 2010\\robotfan\\sound\\aread.wav"), NULL, SND_FILENAME); 
			break;
		}
		default: break;
	}
}

