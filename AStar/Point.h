#pragma once
#include "stdafx.h"

typedef struct _Point{
    int X;
    int Y;
    int F;
    int G;
    int H;

    struct _Point *parentPoint;

    void calcF()
    {
        F = G + H;
    }

}Point,*pPoint;

