#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    // (x, y) 表示小球的坐标，坐标原点为左上角，x表示宽，y表示高
    int x = 4, y = 2;

    int height = 10;
    int width = 15;
    int velocity_y = 1; // 小球的运动速度
    int velocity_x = 1;

    while(1){
        y = y + velocity_y;
        x = x + velocity_x;
        system("clear"); // 清空终端桌面
        for(int i = 0; i < y; i++){
            printf("\n");
        }
        for(int j = 0; j < x; j++){
            printf(" ");
        }
        printf("o\n");
        sleep(1); // 不加 sleep() 函数导致小球下落太快，视觉无法观察

        if((y == 0) || (y == height)) // 到达底部，速度方向反转
            velocity_y = -velocity_y;
        if ((x == 0) || (x == width)) // 到达顶部，速度反向再次反转
            velocity_x = -velocity_x;
    }

    return 0;
}