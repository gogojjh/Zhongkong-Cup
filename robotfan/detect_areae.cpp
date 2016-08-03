#include "stdafx.h"
#include "detect_areae.h"

char detect_areae(Mat testImage)
{
	bool u = false;
	u = surfFeaturedetect(testImage, "../logo/fenda_logo.jpg");
	if (u) return FENDA;
	u = surfFeaturedetect(testImage, "../logo/jianyiakele_logo.jpg");
	if (u) return JIANYIKELE;
	u = surfFeaturedetect(testImage, "../logo/xuebi_logo.jpg");
	if (u) return XUEBI;
	u = surfFeaturedetect(testImage, "../logo/yangleduo_logo.jpg");
	if (u) return YANGLEDUO;
	u = surfFeaturedetect(testImage, "../logo/mengniu_chun_logo.jpg");
	if (u) return MENGNIU;
	u = surfFeaturedetect(testImage, "../logo/qqxing_logo.jpg");
	if (u) return QQXING;
	return NOTHING;
}