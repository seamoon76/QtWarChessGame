#ifndef MYCONSTANT_H
#define MYCONSTANT_H

// 自定义的常量类
// 在命名空间myconstant中
// 存储游戏配置

namespace myconstant {
    class Configuration{

    };
    const int WINDOW_WIDTH=1700;           // 主窗口宽度
    const int WINDOW_HEIGHT=900;           // 主窗口高度
    const int ITEM_HEIGHT=100;             // 图形单元的高度
    const int ITEM_LINE_MAX_NUMBER=14;     // 有多少行图形单元
    const int ITEM_WIDTH=100;              // 图形单元的高度
    const int ITEM_ROW_MAX_NUMBER=8;       // 有多少列图形单元
    const int MAX_HP_OF_LEVEL_1=50;        // 等级为1的角色的最大HP
    const int MAX_MP_OF_LEVEL_1=5;         // 等级为1的角色的最大MP


}
#endif // MYCONSTANT_H
