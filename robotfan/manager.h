#include <string.h>
#include "video.h"
#include "voice.h"
#include "detectYellowBlock.h"
#include "detectRedBlock.h"
#include "detectGreenBlock.h"
#include "detectBlueBlock.h"
#include "detectObject.h"
#include "testState.h"
#include "defineName.h"
#include "detect_areac.h"
#include "detect_aread.h"
#include "detect_areae.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <ctime>
#include <time.h>

using namespace std;
using namespace cv;

char manager(char input, int &number);
