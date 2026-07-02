#include <stdio.h>
#include <string.h>

#define MAX_N 12
int total_count = 0;

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

void print_board(int n, int queen[], int num)
{
    int i, j;
    printf("\n第%d个解：\n", num);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (queen[i] == j)
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
}

void backtrack(int row, int n, int queen[])
{
    int col;
    if (row == n)
    {
        total_count++;
        print_board(n, queen, total_count);
        return;
    }
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

void file_backtrack(int row, int n, int queen[], int *cnt, FILE *fp)
{
    int col, i, j;
    if (row == n)
    {
        (*cnt)++;
        fprintf(fp, "第%d组解：\n", *cnt);
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                fprintf(fp, queen[i] == j ? "Q " : ". ");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
        return;
    }
    for (col = 0; col < n; col++)
    {
        if (is_valid(row, col, queen))
        {
            queen[row] = col;
            file_backtrack(row + 1, n, queen, cnt, fp);
            queen[row] = 0;
        }
    }
}

void save_result(int n)
{
    FILE *fp = fopen("八皇后解结果.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败，无法保存！\n");
        return;
    }
    fprintf(fp, "%d皇后问题，总解数：%d\n\n", n, total_count);
    int temp_queen[MAX_N] = {0};
    int temp_cnt = 0;
    file_backtrack(0, n, temp_queen, &temp_cnt, fp);
    fclose(fp);
    printf("全部解已保存至：八皇后解结果.txt\n");
}

int get_queen_num()
{
    int n;
    printf("请输入皇后数量（范围1~12）：");
    while (scanf("%d", &n) != 1 || n < 1 || n > MAX_N)
    {
        while (getchar() != '\n');
        printf("输入无效！请输入1~12之间整数：");
    }
    return n;
}

int main()
{
    int n = get_queen_num();
    int queen[MAX_N] = {0};
    char op;
    total_count = 0;

    backtrack(0, n, queen);
    printf("\n%d皇后问题，全部合法解总数：%d\n", n, total_count);

    printf("是否将结果保存到文件？(Y/N)：");
    getchar();
    scanf("%c", &op);
    if (op == 'Y' || op == 'y')
    {
        save_result(n);
    }
    printf("程序运行结束\n");
    return 0;
}
