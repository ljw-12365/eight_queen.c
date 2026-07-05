#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string.h>
#include <time.h>    // 计时头文件

#define MAX_N 12
#define MAX_SOLUTION 15000

// 全局变量仅定义一次（解决重定义C2086/C2374报错）
int total_count = 0;
int solutions[MAX_SOLUTION][MAX_N];

// 函数前置声明
int is_valid(int row, int col, int queen[]);
void print_board(int n, int queen[], int num);
void draw_board(int n, int index);
void backtrack(int row, int n, int queen[]);
void backtrack_save(int row, int n, int queen[], int* cnt, FILE* fp);
void save_result(int n);
int get_queen_num();

// 冲突检测
int is_valid(int row, int col, int queen[])
{
    int i;
    for (i = 0; i < row; i++)
    {
        if (queen[i] == col || (row - col) == (i - queen[i]) || (row + col) == (i + queen[i]))
        {
            return 0;
        }
    }
    return 1;
}

// 控制台打印字符棋盘
void print_board(int n, int queen[], int num)
{
    printf("\n第%d个解：\n", num);
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf(queen[i] == j ? "Q " : ". ");
        }
        printf("\n");
    }
}

// EasyX绘制棋盘（无黑线）
void draw_board(int n, int index)
{
    cleardevice();
    int cell_size = 600 / n;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            setfillcolor((i + j) % 2 == 0 ? WHITE : LIGHTGRAY);
            fillrectangle(j * cell_size, i * cell_size, (j + 1) * cell_size, (i + 1) * cell_size);
            if (solutions[index][i] == j)
            {
                settextcolor(RED);
                settextstyle(cell_size / 2, 0, _T("宋体"));
                outtextxy(j * cell_size + cell_size / 4, i * cell_size, _T("Q"));
            }
        }
    }
    TCHAR tip[64];
    _stprintf_s(tip, _T("第%d组 / 共%d组 | 按键/鼠标左键切换"), index + 1, total_count);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("宋体"));
    outtextxy(10, 10, tip);
}

// 回溯求解并保存全部解到数组
void backtrack(int row, int n, int queen[])
{
    if (row == n)
    {
        memcpy(solutions[total_count], queen, sizeof(int) * n);
        total_count++;
        print_board(n, queen, total_count);
        return;
    }
    int col;
    for (col = 0; col < n; col++)
    {
        if (is_valid(row, col, queen))
        {
            queen[row] = col;
            backtrack(row + 1, n, queen);
            queen[row] = 0;
        }
    }
}

// 文件保存专用回溯
void backtrack_save(int row, int n, int queen[], int* cnt, FILE* fp)
{
    if (row == n)
    {
        (*cnt)++;
        fprintf(fp, "第%d组解：\n", *cnt);
        int i, j;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                fprintf(fp, queen[i] == j ? "Q " : ". ");
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
        return;
    }
    int col;
    for (col = 0; col < n; col++)
    {
        if (is_valid(row, col, queen))
        {
            queen[row] = col;
            backtrack_save(row + 1, n, queen, cnt, fp);
            queen[row] = 0;
        }
    }
}

// 保存结果到txt
void save_result(int n)
{
    FILE* fp = fopen("八皇后解结果.txt", "w");
    if (!fp)
    {
        printf("文件打开失败！\n");
        return;
    }
    fprintf(fp, "%d皇后问题 总解数：%d\n\n", n, total_count);
    int queen[MAX_N] = { 0 }, cnt = 0;
    backtrack_save(0, n, queen, &cnt, fp);
    fclose(fp);
    printf("结果已保存至：八皇后解结果.txt\n");
}

// 获取合法输入
int get_queen_num()
{
    int n;
    printf("请输入皇后数量(1~12)：");
    while (scanf("%d", &n) != 1 || n < 1 || n > MAX_N)
    {
        while (getchar() != '\n');
        printf("输入无效，请重新输入：");
    }
    return n;
}

// 唯一main函数（解决C2084已有主体报错）
int main()
{
    int n = get_queen_num();
    int queen[MAX_N] = { 0 };
    char op;
    clock_t start, end;
    double run_ms;

    // 计时包裹求解核心
    start = clock();
    backtrack(0, n, queen);
    end = clock();
    run_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000;

    // 输出性能测试数据（答辩要用）
    printf("\n==============================\n");
    printf("%d皇后 总解数：%d\n", n, total_count);
    printf("纯求解耗时：%.2f 毫秒\n", run_ms);
    printf("==============================\n");

    // 图形窗口初始化
    initgraph(600, 600);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    // 鼠标+按键切换图片
    for (int i = 0; i < total_count; i++)
    {
        draw_board(n, i);
        ExMessage msg;
        while (1)
        {
            msg = getmessage(EX_KEY | EX_MOUSE);
            if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN)
                break;
        }
    }
    closegraph();

    // 文件保存选择
    printf("是否保存全部解到文本文件？(Y/N)：");
    (void)getchar();
    (void)scanf("%c", &op);
    if (op == 'Y' || op == 'y')
    {
        save_result(n);
    }

    printf("程序运行结束\n");
    return 0;
}