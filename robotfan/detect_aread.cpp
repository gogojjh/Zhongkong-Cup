#include "stdafx.h"
#include "detect_aread.h"

char detect_aread(Mat testImage)
{
	bool u = false;
	u = surfFeaturedetect(testImage, "../logo/yangleduo_logo.jpg");
	if (u) return YANGLEDUO;
	u = surfFeaturedetect(testImage, "../logo/mengniu_chun_logo.jpg");
	if (u) return MENGNIU;
	u = surfFeaturedetect(testImage, "../logo/qqxing_logo.jpg");
	if (u) return QQXING;
	return NOTHING;
}