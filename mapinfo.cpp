#include "mapinfo.h"


mapInfo::mapInfo()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<14;j++)
        {
            map[i][j]=2;//默认是floor地形
        }
    }
}
