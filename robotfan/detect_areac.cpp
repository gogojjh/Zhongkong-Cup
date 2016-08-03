#include "stdafx.h"
#include "detect_areac.h"

char detect_areac(Mat testImage)
{
	bool u = false;
	u = surfFeaturedetect(testImage, "../logo/fenda_logo.jpg");
	if (u) return FENDA;
	u = surfFeaturedetect(testImage, "../logo/jianyikele_logo.jpg");
	if (u) return JIANYIKELE;
	u = surfFeaturedetect(testImage, "../logo/jianyikele_logo2.jpg");
	if (u) return JIANYIKELE;
	u = surfFeaturedetect(testImage, "../logo/jianyikele_logo3.jpg");
	if (u) return JIANYIKELE;
	u = surfFeaturedetect(testImage, "../logo/xuebi_logo.jpg");
	if (u) return XUEBI;
	return NOTHING;
}