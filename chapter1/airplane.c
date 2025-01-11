#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define AIRPLANE "*"
#define ENEMY "@"
#define BULLET "|"

/*
1. 初始化飞机和起始位置的界面
2. 显示初始化界面
3. 响应按键的动作
    awsd 表示上下左右移动飞机
    q 表示发射飞机炮弹
*/

int position_x, position_y; // 飞机的位置坐标
const int high = 20, width = 20; // 屏幕的范围
int enemy_x, enemy_y; // 敌机的坐标
int bullet_x, bullet_y; // 子弹的坐标
int score; // 得分统计
const int count = 10; // 敌机多少循环下降一个空间
struct termios default_mode;
int exitFlag = 1;

void startup();
void show();
void updateWithoutInput();
void updateWithInput();
void set_nonblocking_noecho(int , int);

int main(){
    startup();
    // set_nonblocking_noecho(STDIN_FILENO, 0);
    while(exitFlag){
        show();
        updateWithoutInput();
        updateWithInput();
    }
    printf("Finishing ...\n");
    // set_nonblocking_noecho(STDIN_FILENO, 1);
    return 0;
}

void startup(){
    position_x = width / 2;
    position_y = high / 2;
    enemy_x = rand() % width;
    enemy_y = 0;
    score = 0;
    bullet_x = -1;
    bullet_y = -1;
}

void show(){
    system("clear"); // 清屏操作
    int i, j;
    for(i = 0; i < high; i++){
        for(j = 0; j < width; j++){
            if((i == position_y) && (j == position_x)){
                printf(AIRPLANE);
            }else if((i == enemy_y) && (j == enemy_x)){
                printf(ENEMY);
            }else if((i == bullet_y) && (j == bullet_x)){
                printf(BULLET);
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("Position x: %d, y: %d\n", position_x, position_y);
    printf("Bullet x: %d, y: %d\n", bullet_x, bullet_y);
    printf("Enemy x: %d, y: %d\n", enemy_x, enemy_y);
}

void updateWithInput(){
    char input = getchar();
    // sleep(1);
    switch(input){
        case 'a':
            position_x--;
            break;
        case 'd':
            position_x++;
            break;
        case 'w':
            position_y--;
            break;
        case 's':
            position_y++;
            break;
        case 'q':
            bullet_x = position_x;
            bullet_y = position_y - 1;
            break;
        case ' ':
            exitFlag = 0;
            break;
        default:
            break;
    }
}

void updateWithoutInput(){
    // 子弹默认上升运动
    if(bullet_y > -1){
        bullet_y--;
    }
    if((bullet_x == enemy_x) && (bullet_y == enemy_y)){
        score++;
        enemy_x = rand() % width;
        enemy_y = 0;
        bullet_x = -1;
        bullet_y = -1;
    }
    if(enemy_y > high){
        // 飞出屏幕之外
        enemy_x = rand() % width;
        enemy_y = 0;
    }
    // 敌机默认向下运动
    static int speed = 0;
    speed++;
    if(speed == count){
        enemy_y++;
        speed = 0;
    }

}


void set_nonblocking_noecho(int fd, int set) {
    struct termios termattr;
    int res;

    // 获取当前终端属性
    res = tcgetattr(fd, &termattr);
    if(set == 0){
        default_mode = termattr;
        // 修改属性，设置为非规范模式，关闭回显和信号字符处理
        termattr.c_lflag &= ~(ICANON | ECHO );  // 关闭回显、行处理、信号字符

        // 设置非阻塞模式
        termattr.c_cc[VMIN] = 0;  // 最小字符数为 0
        termattr.c_cc[VTIME] = 0;  // 超时时间为 0

        // 应用修改后的终端属性
        res = tcsetattr(fd, TCSANOW, &termattr);
    }else{
        tcsetattr(fd, TCSANOW, &default_mode);
    }
    return ;
}



