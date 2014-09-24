#pragma once
#include "stdafx.h"
#include "Point.h"
#include "list"

#define __DEBUG__  
#ifdef __DEBUG__  
#define DEBUG(format,...) printf(""format"\n",##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif

using namespace std;

class AStar
{
    public:
        AStar(int maxx = 0, int maxy = 0, int straight = 0, int oblique = 0)
        {
            mapMaxX = maxx;
            mapMaxY = maxy;
            this->STRAIGHT = straight;
            this->OBLIQUE = oblique;
        }
        void findPath(Point start,Point end,int map[],bool bIgnoreCorner = false);
        bool bExist(Point point, list <Point> pointList);
        bool bExist(int x, int y, list <Point> pointList);
        //bool canReach(int x, int y);
        //bool canReach(Point start, int x, int y, bool bIgnoreCorner = false);
        //bool canReach(Point start, Point end,bool bIgnoreCorner = false);
        Point getMinF();
        int calcG(Point start, Point end);
        int calcH(Point start, Point end);
        void findAroundPoint(Point start,int map[]);
        void insertPoint(Point point,Point parentPoint,int nListType=0);
        void deletePoint(int nListType=0);
        bool bLegalPoint(Point point,int map[]);
        void updatePoint(Point point,Point parentPoint);
        void printPath(Point point);
        static bool op(Point x, Point y)
        {
            return x.F < y.F;
        }

    private:
        //int direct[8][2];
        list <Point> openPoint;
        list <Point> closePoint;
        Point m_start;
        Point m_end;
        int mapMaxX;
        int mapMaxY;
        int STRAIGHT;
        int OBLIQUE;
};


void AStar::findPath(Point start,Point end,int map[],bool bIgnoreCorner)
{
    //insertPoint(start);
    m_start = start;
    m_end = end;
    openPoint.push_front(start);
    while(openPoint.empty() == false)  //如果开启列表不为空
    {
        Point minF = getMinF();        //在开启列表中找到F值最小的点
        if(minF.X == end.X && minF.Y == end.Y)
        {
            DEBUG("find the path!\n");
            printPath(minF);
            break;
        }
        insertPoint(minF,minF,1);               //将这个点加入到关闭列表中
        deletePoint();                          //将这个点从开启列表中删除
        findAroundPoint(minF,map);              //将周围的点加入到开启列表中
    }
}


bool AStar::bExist(Point point, list<Point> pointList)
{
    list<Point>::iterator it;
    for(it = pointList.begin(); it != pointList.end(); it++)
    {
        if(point.X == it->X && point.Y == it->Y) return true;
    }
    return false;
}


bool AStar::bExist(int x, int y, list<Point> pointList)
{
    list<Point>::iterator it;
    for(it = pointList.begin(); it != pointList.end(); it++)
    {
        if(x == it->X && y == it->Y) return true;
    }
    return false;
}


Point AStar::getMinF()
{
    openPoint.sort(op);
    return openPoint.front();
}


int AStar::calcG(Point start, Point end)
{
    int G = abs(start.X - end.X) + abs(start.Y - end.Y) == 2 ? OBLIQUE : STRAIGHT;
    //int parentG = start.parentPoint != NULL ?
    return G + start.G;    //这里有待商榷
}


int AStar::calcH(Point start, Point end)
{
    int step = abs(start.X - end.X) + abs(start.Y - end.Y);
    return step * STRAIGHT;
}


void AStar::findAroundPoint(Point start,int map[])
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            Point aroundPoint = {start.X + i, start.Y + j};
            if(bLegalPoint(aroundPoint,map))
            {
                if(bExist(aroundPoint,openPoint))   //如果已经存在开放列表中,则重新计算G、H值
                {
                    int g = calcG(aroundPoint,start);
                    if(g < aroundPoint.G)
                    {
                        updatePoint(aroundPoint,start);
                        DEBUG("update,%d:%d",aroundPoint.X,aroundPoint.Y);
                    }
                }
                else
                {
                    insertPoint(aroundPoint,start);    //如果不存在,则将这个点插入到列表中,并且父节点为start
                    if(aroundPoint.X == 6 && aroundPoint.Y == 2)
                    {
                        DEBUG("...");
                    }
                    DEBUG("insert,%d:%d",aroundPoint.X,aroundPoint.Y);
                }
                
            }
        }
    }
}


void AStar::insertPoint(Point point,Point parentPoint,int nListType)
{
    if(nListType == 0)
    {
        point.G = calcG(parentPoint,point);
        point.H = calcH(point,m_end);
        point.calcF();
        point.parentPoint = (pPoint) malloc(sizeof(Point));
        memcpy(point.parentPoint,&parentPoint,sizeof(parentPoint));
        openPoint.push_front(point);
    }
    else
    {
        closePoint.push_front(point);
    }
}

void AStar::deletePoint(int nListType)
{
    openPoint.pop_front();
}


bool AStar::bLegalPoint(Point point,int map[])
{
    if(point.X > mapMaxX - 1 || point.X < 0 ||
        point.Y > mapMaxY - 1 || point.Y < 0 ||
        (map[point.Y*mapMaxX+point.X] == 1) ||
        (bExist(point,closePoint) == true)) return false;
    return true;
}


void AStar::updatePoint(Point point,Point parentPoint)
{
    list<Point>::iterator it;
    for(it = openPoint.begin(); it != openPoint.end(); it++)
    {
        if(point.X == it->X && point.Y == it->Y)
        {
            it->G = point.G;
            it->calcF();
            memcpy(it->parentPoint,&parentPoint,sizeof(parentPoint));
        }
    }
}

void AStar::printPath(Point point)
{
    printf("%d,%d\n",point.X,point.Y);
    pPoint parentPoint = point.parentPoint;
    while(parentPoint != NULL)
    {
        printf("%d,%d\n",parentPoint->X,parentPoint->Y);
        parentPoint = parentPoint->parentPoint;
    }
}