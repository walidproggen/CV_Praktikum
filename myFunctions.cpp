#include "myFunctions.h"


Rect computeROI (Point p1, Point p2, Size imgSize) { 
	// when x or y coordinate exceed picture/window size, readjust them
	p2.x = p2.x<0 ? 0:p2.x; p2.x = p2.x<imgSize.width ? p2.x:imgSize.width;
	p2.y = p2.y<0 ? 0:p2.y; p2.y = p2.y<imgSize.height ? p2.y:imgSize.height;
		
	// save mouse coordinates into a range: rangeWidth(x1, x2), rangeHeight(y1,y2)
	Range rangeWidth(p1.x > p2.x ? p2.x:p1.x, p1.x > p2.x ? p1.x:p2.x);
	Range rangeHeight(p1.y > p2.y ? p2.y:p1.y, p1.y > p2.y ? p1.y:p2.y);

	cout << "rangeWidth.start = " << rangeWidth.start << "; rangeWidth.end = " << rangeWidth.end << endl;
	cout << "rangeHeight.start = " << rangeHeight.start << "; rangeHeight.end = " << rangeHeight.end << endl;

	cout << "rangeWidth.size() = " << rangeWidth.size() << endl; 
	cout << "rangeHeight.size() = " << rangeHeight.size() << endl; 

	return Rect(rangeWidth.start, rangeHeight.start, rangeWidth.size(), rangeHeight.size());
}
