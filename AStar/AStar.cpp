// AStar.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Point.h"
#include "AStat.h"
#include "list"

int _tmain(int argc, _TCHAR* argv[])
{
    int map[][6] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,-1,0,1,0,2,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0};
    Point start = {2,2,0,0,0,0};
    Point end = {6,2,0,0,0,0};

    AStar myAStar(8,6,10,14);
    //myAStar.insertPoint(start);
    myAStar.findPath(start,end,*map);

	return 0;
}
