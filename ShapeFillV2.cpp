#include <stdio.h>
#include <stdlib.h>
 
#define N 100
#define M 100  //������������ ������ �������;
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 }; //�������� ��� �������� ��������� �����������
bool enclosed = true; //�����������, �� �������� � ����� �����������

static void enclosureCheck(char grid[M][N], int width, int height, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return; //����� �� �������
    if (grid[y][x] != ' ') return; //�������
    if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
        enclosed = false; //���� �� ������
        return;
    }
    grid[y][x] = '-'; //������������� ������������ �� ������� 

    for (int i = 0; i < 4; i++) {
        enclosureCheck(grid, width, height, x + dx[i], y + dy[i]);
    }
}

static void floodFill(char grid[M][N], int width, int height, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    if (grid[y][x] != ' ') return; //�������� �� ����� �� ������� � �� ������
    grid[y][x] = '@';
    for (int i = 0; i < 4; i++) {
        floodFill(grid, width, height, x + dx[i], y + dy[i]);
    }
}

int findStartPos(char grid[M][N], int width, int height, int* fx, int* fy) {
    for (int y = 1; y < height - 1; ++y) 
        for (int x = 1; x < width - 1; ++x) //��� �� �������
            if (grid[y][x] == ' ') { //���� ����� ������
                int count = 0;
                for (int i = 0; i < 4; ++i) { //��������� �������
                    int nx = x + dx[i], ny = y + dy[i];
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && grid[ny][nx] == '*')
                        count++; //����������� ������� ���� ���� *
                }
                if (count >= 2) {
                    *fx = x; *fy = y; 
                    return 1; //���� ���� 2+ * � �������� ��������, ��� ����� ��� ��������
                }
            }
    return 0;
}

void copyGrid(char copy[M][N], char source[M][N], int width, int height) {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            copy[y][x] = source[y][x];
}

int main() {
    
    FILE* file = fopen("f:\\test2.txt", "r");
    if (!file) {
        printf("Couldn't open the file!\n");
        return 1;
    }

    char grid[M][N]; 
    int width = 0, height = 0; 

    int c;
    int pos = 0;
    while ((c = fgetc(file)) != EOF && height < N) {
        if (c == '\n' || c == '\r') {
            if (pos > width) width = pos;
            if (c == '\r') {
                int next_c = fgetc(file);
                if (next_c != '\n' && next_c != EOF) ungetc(next_c, file);
            }

            for (int fill_x = pos; fill_x < N; ++fill_x)
                grid[height][fill_x] = ' ';
            pos = 0;
            height++;
        }
        else {
            if (pos < N)
                grid[height][pos++] = (char)c;
        } //��������� ������ �� �����, �������� �� ����� ����� ������ � �����
    }

    printf("Your shape:\n"); //����� ����������� ������
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) printf("%c", grid[y][x]);
        printf("\n");
    }

    int fx = -1, fy = -1;
    if (!findStartPos(grid, width, height, &fx, &fy)) { 
        printf("Couldn't find an inner point!\n");
        return 1;
    } //������� ��������� ������� - ��� ����� ��� �������� �����������

    char temp_grid[M][N];
    copyGrid(temp_grid, grid, width, height); //��������� ������� ��� ��������
    enclosureCheck(temp_grid, width, height, fx, fy); //��������� �����������

    if (!enclosed) {
        printf("\nThis shape isn't enclosed!\n");
    }
    else {
        floodFill(grid, width, height, fx, fy); //���� �������� ��������, ��������� ������
        printf("\nEnclosure check passed. Filled shape:\n");
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) printf("%c", grid[y][x]);
            printf("\n"); //������� ���������
        }
    }
}