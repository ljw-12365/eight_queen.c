#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>   // EasyX图形库头文件
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string.h>

#define MAX_N 12
#define MAX_SOLUTION 15000  // 可容纳12皇后全部14200组解

int total_count = 0;
int solutions[MAX_SOLUTION][MAX_N]; // 存储所有解的皇后位置

// 函数声明（放在main之前，解决未定义问题）
int is_valid(int row, int col, int queen[]);
void print_board(int n, int queen[], int num);
void draw_board(int n, int index);
void backtrack(int row, int n, int queen[]);
void save_result(int n);
int get_queen_num();

// 检测皇后位置是否合法
int is_valid(int row, int col, int queen[])
{
    for (int i = 0; i < row; i++)
    {
        if (queen[i] == col || (row - col) == (i - queen[i]) || (row + col) == (i + queen[i]))
        {
            return 0;
        }
    }
    return 1;
}

// 控制台打印文字棋盘
void print_board(int n, int queen[], int num)
{
    printf("\n第%d个解：\n", num);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf(queen[i] == j ? "Q " : ". ");
        }
        printf("\n");
    }
}

// 绘制单组解的图形棋盘
void draw_board(int n, int index)
{
    cleardevice();
    int cell_size = 600 / n;

    // 绘制黑白棋盘格（严丝合缝，无黑线）
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            setfillcolor((i + j) % 2 == 0 ? WHITE : LIGHTGRAY);
            fillrectangle(j * cell_size, i * cell_size,
                (j + 1) * cell_size,
                (i + 1) * cell_size);

            // 绘制红色皇后Q
            if (solutions[index][i] == j)
            {
                settextcolor(RED);
                settextstyle(cell_size / 2, 0, _T("宋体"));
                outtextxy(j * cell_size + cell_size / 4, i * cell_size, _T("Q"));
            }
        }
    }

    // 顶部进度提示
    TCHAR tip[64];
    _stprintf_s(tip, _T("第%d组 / 共%d组 | 按任意键/点鼠标切换下一个"), index + 1, total_count);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("宋体"));
    outtextxy(10, 10, tip);
}

// 回溯求解，同时保存解+控制台打印
void backtrack(int row, int n, int queen[])
{
    if (row == n)
    {
        memcpy(solutions[total_count], queen, sizeof(int) * n);
        total_count++;
        print_board(n, queen, total_count);
        return;
    }
    for (int col = 0; col < n; col++)
    {
        if (is_valid(row, col, queen))
        {
            queen[row] = col;
            backtrack(row + 1, n, queen);
            queen[row] = 0;
        }
    }
}

// 保存所有解到文件
void save_result(int n)
{
    FILE* fp = fopen("八皇后解结果.txt", "w");
    if (!fp)
    {
        printf("文件打开失败！\n");
        return;
    }

    fprintf(fp, "%d皇后问题 总解数：%d\n\n", n, total_count);
    for (int i = 0; i < total_count; i++)
    {
        fprintf(fp, "第%d组解：\n", i + 1);
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                fprintf(fp, solutions[i][row] == col ? "Q " : ". ");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("结果已保存到：八皇后解结果.txt\n");
}

// 获取合法输入
int get_queen_num()
{
    int n;
    printf("请输入皇后数量（1~12）：");
    while (scanf("%d", &n) != 1 || n < 1 || n > MAX_N)
    {
        while (getchar() != '\n');
        printf("输入无效，请重新输入：");
    }
    return n;
}

int main()
{
    int n = get_queen_num();
    int queen[MAX_N] = { 0 };
    char op;

    // 第一步：计算所有解，控制台同步输出全部结果
    backtrack(0, n, queen);
    printf("\n==============================\n");
    printf("%d皇后问题 总解数：%d\n", n, total_count);
    printf("控制台已显示全部解，图形窗口按任意键/点鼠标切换\n");
    printf("==============================\n");

    // 第二步：打开图形窗口，初始化去黑线设置
    initgraph(600, 600);
    setbkcolor(WHITE);       // 背景设为白色，彻底消除黑色底色
    setbkmode(TRANSPARENT);  // 文字背景透明，消除顶部黑条
    cleardevice();

    // 第三步：按键/点鼠标逐个切换解
    for (int i = 0; i < total_count; i++)
    {
        draw_board(n, i);
        // 等待图形窗口的按键或鼠标左键点击
        ExMessage msg;
        while (true)
        {
            msg = getmessage(EX_KEY | EX_MOUSE);
            if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN)
            {
                break;
            }
        }
    }
    closegraph(); // 全部看完自动关闭窗口

    // 第四步：询问是否保存文件
    printf("是否保存结果到文件？(Y/N)：");
    (void)getchar();
    (void)scanf("%c", &op);
    if (op == 'Y' || op == 'y')
    {
        save_result(n);
    }

    printf("程序运行结束\n");
    return 0;
}