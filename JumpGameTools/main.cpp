/* 微信的跳一跳辅助
 *
 * 使用方法：
 * 1. mac上安装adb的方法：brew install --cask android-platform-tools
 * 2. 手机上打开设置-关于本机，多次点击开启开发者模式，连接手机
 * 3. 使用adb devices连接手机
 *
 * 1. win下载地址：https://developer.android.google.cn/studio/releases/platform-tools?authuser=0&hl=tr
 * 2. 将目录添加到环境变量
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <unistd.h>
#include "easyx.h"
#include "graphics.h"


// 计算颜色差异
int calculateColorDifference(COLORREF color1, COLORREF color2) {
    int redDiff = abs(GetRValue(color1) - GetRValue(color2));
    int greenDiff = abs(GetGValue(color1) - GetGValue(color2));
    int blueDiff = abs(GetBValue(color1) - GetBValue(color2));
    return redDiff + greenDiff + blueDiff;
}


int main() {

    IMAGE image, img;
    int px, py; // 游戏主角的坐标
    int tx = -1, ty = -1; // 目标点的坐标：初始化为无效值，表示未找到

    while(true)
    {

        // 截图命令
        system("adb shell screencap -p /sdcard/screenshot.png");
        system("adb pull /sdcard/screenshot.png");

        //initgraph(900, 900, EW_SHOWCONSOLE);


        loadimage(&image, "screenshot.png");

       //putimage(0,0, &image);

        // 从image获取一部分图像
        SetWorkingImage(&image); // 工作区
        getimage(&img, 100, 700, 900,900);
        //SetWorkingImage(NULL); // 回到默认工作区
        //putimage(0,0, &img);

        SetWorkingImage(&img);


        int x, y;
        for(y = 899; y>0; y--)
        {
            for(x = 0; x < 900; x++)
            {
                if(getpixel(x, y) == RGB(57, 57, 99))
                {
                    px = x;
                    py = y;
                    break;
                }
            }
            if(getpixel(x, y) == RGB(65, 60, 102))
            {
                break;
            }
        }

        printf("游戏主角的坐标：px = %d\t py = %d\n", px, py);



        COLORREF bgcolor = getpixel(0, 0); // 背景颜色
        for(y = 0; y < 900; y++)
        {
            for(x = 0; x < 900; x++)
            {
                COLORREF tColor = getpixel(x, y); // 获取当前点的值
                int colorDifference = calculateColorDifference(tColor, bgcolor);
                if(colorDifference > 50)  // 不相似
                {
                    tx = x;
                    ty = y;
                    break;
                }

            }
            if(tx != -1 && ty != -1)
            {
                break; // 结束外层循环
            }
        }

        printf("目标点的坐标：tx = %d\t ty = %d\n", tx, ty);

        ty += 50;  // 目标点向下移动一点

        double d = sqrt(double((px - tx)*(px-tx) + (py-ty)*(py-ty)));  // 游戏主角与目标点距离

        double dtime = 1.35 * d;  // 毫秒
        int time = (int)(dtime + 0.5) > (int)(dtime) ? (int)dtime + 1 : (int)dtime; // 四舍五入

        char shellString[100];
        sprintf(shellString, "adb shell input swipe 200 300 205 305 %d", time); // 拼接命令字符串，触摸屏幕
        system(shellString);

        sleep(2);

    }


    _getch(); // 等待用户按键
    return 0;
}
