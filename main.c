#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>


#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_NOTE_LENGTH 100
#define MAX_FOODS 100
#define Console_X_MAX 50
#define Console_Y_MAX 40
#define Max_value Console_X_MAX * Console_Y_MAX

int pageStatus = 0;//현재 화면 상태 1: 메인화면 2: 음식목록 3: 음식추가 9:종료
int num_food = 0;

char consoleData[Max_value];


typedef struct {
    int category;
    char name[MAX_NAME_LENGTH];
    char expiration_date[MAX_DATE_LENGTH];
    char note[MAX_NOTE_LENGTH];
} Food;

typedef struct {
    int year;
    int month;
    int day;
} Date;

void display_menu() {
    printf("-------------------------------\n");
    printf(" (로고)\n");
    printf("음식창고\n");
    printf("-------------------------------\n");
    printf("1. 음식 목록\n");
    printf("2. 음식 추가\n");
    printf("3. 설정\n");
    printf("4. 종료\n");
    printf("-------------------------------\n");
    printf("선택: ");
}

void display_food_list(Food* foods, int num_food) {
    printf("-------------------------------\n");
    printf(" (로고)\n");
    printf("음식창고\n");
    printf("-------------------------------\n");

    for (int i = 0; i < num_food; i++) {
        int remaining_days = get_remaining_days(foods[i].expiration_date);
        if (remaining_days >= 0) {
            printf("%d. %s(%d일)\n", i + 1, foods[i].name, remaining_days);
        }
        else {
            printf("%d. %s(!%d일)\n", i + 1, foods[i].name, -remaining_days);
        }
    }

    printf("-------------------------------\n");
    printf("선택: \n");
}

void add_food(Food* foods, int* num_food) {
    int category;
    char name[MAX_NAME_LENGTH];
    char expiration_date[MAX_DATE_LENGTH];
    char note[MAX_NOTE_LENGTH];

    printf("-------------------------------\n");
    printf(" (로고)\n");
    printf("음식창고\n");
    printf("-------------------------------\n");
    printf("1. 육류\n");
    printf("2. 유제품\n");
    printf("3. 김치\n");
    printf("4. 음료\n");
    printf("-------------------------------\n");
    printf("선택: ");
    scanf("%d", &category);

    printf("-------------------------------\n");
    printf(" (로고)\n");
    printf("음식창고\n");
    printf("-------------------------------\n");
    printf("1. 제품명\n");
    printf("2. 유통기한을 입력하시오.\n");
    printf("   ex)2023-10-01\n");
    printf("3. 별도 표기 사항\n");
    printf("-------------------------------\n");
    printf("1. ");
    scanf("%s", name);
    printf("2. ");
    scanf("%s", expiration_date);
    printf("3. ");
    scanf("%s", note);

    foods[*num_food] = (Food){ category, "", "", "" };
    strcpy(foods[*num_food].name, name);
    strcpy(foods[*num_food].expiration_date, expiration_date);
    strcpy(foods[*num_food].note, note);
    (*num_food)++;
}

int get_remaining_days(char* expiration_date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Date expiration;
    sscanf(expiration_date, "%d-%d-%d", &expiration.year, &expiration.month, &expiration.day);

    struct tm expiration_tm = { 0 };
    expiration_tm.tm_year = expiration.year - 1900;
    expiration_tm.tm_mon = expiration.month - 1;
    expiration_tm.tm_mday = expiration.day;

    time_t expiration_time = mktime(&expiration_tm);
    double remaining_seconds = difftime(expiration_time, t);
    int remaining_days = (int)(remaining_seconds / (60 * 60 * 24));

    return remaining_days;
}

//#######################################################################
void set_console() {//콘솔창 초기 설정 함수
    system("title FoodWarehouse"); //콘솔창 이름 설정
    system("mode con:cols=50 lines=40"); //콘솔창 크기 설정

    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 10; //콘솔창에서 커서의 크기
    ConsoleCursor.dwSize = 1;   //커서의 가시성

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void textcolor(int color_number) {//텍스트 컬러 설정 함수
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
    // 0: 검정 1: 파랑 2: 초록 3: 옥색 4: 빨강 5: 자주색
    // 6: 노랑 7: 하양 8: 회색 9: 연파랑 10: 연초록
    // 11: 연옥색 12: 연빨강 13: 연자주 14: 연노랑 15: 진한 회색
}

void gotoxy(int x, int y) {//커서의 위치이동
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Cur;
    Cur.X = x;
    Cur.Y = y;
    SetConsoleCursorPosition(consoleHandle, Cur);
}

void FillConsole(char str[], char str_s, int max_value) {//화면 초기화
    for (int i = 0; i < max_value; i++) {
        str[i] = str_s;
    }
}

void EditConsole(int x, int y, char str) {//화면 데이터를 수정하는 함수
    if ((x > 0 && y > 0) && (x - 1 < Console_X_MAX - 1 && y - 1 < Console_Y_MAX - 1)) {
        consoleData[(y - 1) * Console_X_MAX + x - 1] = str;
    }
}

void DrawSprite(int x, int y, int size_x, int size_y, char spr[]) {//텍스트를 출력하는 함수
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            EditConsole(x + j, y + i, spr[i * size_x + j]);
        }
    }
}

void logo() {
    DrawSprite(1, 1, 6, 2, "음  식창  고");
}
void selectMenu() {
    DrawSprite(1, 39, 3, 1, ">> ");
}

void startMenu() {//메인 메뉴
    FillConsole(consoleData, ' ', Max_value);

    DrawSprite(16, 8, 11, 1, "1. 음식목록");
    DrawSprite(16, 10, 11, 1, "2. 음식추가");
    DrawSprite(16, 12, 7, 1, "3. 설정");
    DrawSprite(16, 14, 3, 1, "4. ");
    DrawSprite(16, 17, 7, 1, "0. 종료");

    logo();
    selectMenu();
    printf("%s", consoleData);
    gotoxy(3, 38);

    char num; scanf("%c", &num);

    if (num == '1') {
        pageStatus = 1;
    }
    else if (num == '2') {
        pageStatus = 2;
    }
    else if (num == '3') {
        pageStatus = 3;
    }
    else if (num == '0') {
        pageStatus = 9;
        exit(1);
    }
    Sleep(50);
}

void foodDisplay(Food* foods, int num_food) {
    FillConsole(consoleData, ' ', Max_value);
    char str_days[37];
    char str_num[20];
    if (num_food >= 1) {
        for (int line = 8, i = 0; line < 35, i < 27; line++, i++) {
            int remaining_days = get_remaining_days(foods[i].expiration_date);

            if (remaining_days >= 0) {
                sprintf(str_days, "%d. %s(%d일)", i + 1, foods[i].name, remaining_days);
                DrawSprite(12, line, 37, 1, str_days);
            }
            else {
                sprintf(str_days, "%d. %s(%d일)", i + 1, foods[i].name, remaining_days);
                DrawSprite(12, line, 37, 1, str_days);
            }
        }
    }
    else {
        DrawSprite(12, 8, 15, 1, "항목이 없습니다.");
    }
    sprintf(str_num, "음식 개수: %d", num_food);
    DrawSprite(12, 7, strlen(str_num), 1, str_num);

    DrawSprite(1, 38, 11, 1, "(0)돌아가기");
    logo();
    selectMenu();

    printf("%s", consoleData);
    gotoxy(3, 38);

    char num; scanf("%c", &num);
    if (num == '0') {
        pageStatus = 0;
    }
}

//#######################################################################


int main(void) {
    Food foods[MAX_FOODS];
    int num_food = 0;
    int choice;

    srand(time(NULL));
    set_console();
    pageStatus = 0;

    while (pageStatus != 9) {
        while (pageStatus == 0) {
            system("cls");
            startMenu();
            printf("%s", consoleData);
        }
        while (pageStatus == 1) {
            system("cls");
            foodDisplay(foods, num_food);
        }
        while (pageStatus == 2) {
            system("cls");
            Sleep(50);
        }
        while (pageStatus == 3) {

        }
    }
    return(0);
}