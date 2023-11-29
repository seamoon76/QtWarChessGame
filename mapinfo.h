#ifndef MAPINFO_H
#define MAPINFO_H
#include<array>
#include<QString>
#include<vector>
using std::vector;
using std::array;

// 定义soldierMap结构、monsterMap结构和mapInfo类
// 记录每一关的地图信息和人物信息
// 具体的地图信息将在selectlevelwindow.cpp文件中定义

struct soldierMap
{
    QString soldierType;
    int x;
    int y;
};

struct monsterMap
{
    QString monsterType;
    int x;
    int y;
};

class mapInfo
{
public:
    // 地形信息
    int map[8][14];
    // 战士名单
    vector<soldierMap> soldierListInMap;
    // 怪物名单
    vector<monsterMap> monsterListInMap;

public:
    mapInfo();
};

#endif // MAPINFO_H
