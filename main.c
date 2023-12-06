//비주얼 스튜디오 오류 차단
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


int get_remaining_days(char* expiration_date);

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

    foods[*num_food] = (Food){ category, "", "", "" }; // 오류발생 위치 수정 필요
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
//코드 통합 및 ui제작을 위한 함수들

//콘솔창 초기 설정 함수
void set_console() {
    system("title FoodWarehouse"); //콘솔창 이름 설정
    system("mode con:cols=50 lines=40"); //콘솔창 크기 설정

    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 10; //콘솔창에서 커서의 크기
    ConsoleCursor.dwSize = 1;   //커서의 가시성 (true: 보임, false: 안보임)

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

//텍스트 컬러 설정 함수
void textcolor(int color_number) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
    // 0: 검정 1: 파랑 2: 초록 3: 옥색 4: 빨강 5: 자주색
    // 6: 노랑 7: 하양 8: 회색 9: 연파랑 10: 연초록
    // 11: 연옥색 12: 연빨강 13: 연자주 14: 연노랑 15: 진한 회색
}

//커서를 해당 좌표로 이동
void gotoxy(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Cur;
    Cur.X = x;
    Cur.Y = y;
    SetConsoleCursorPosition(consoleHandle, Cur);
}

//consoleData에 들어있는 화면을 초기화할때 사용하는 함수
void FillConsole(char str[], char str_s, int max_value) {
    for (int i = 0; i < max_value; i++) {
        str[i] = str_s;
    }
}

//해당 좌표의 문자(문자열X)를 str로 편집하는 함수
void EditConsole(int x, int y, char str) {
    if ((x > 0 && y > 0) && (x - 1 < Console_X_MAX - 1 && y - 1 < Console_Y_MAX - 1)) {
        consoleData[(y - 1) * Console_X_MAX + x - 1] = str;
    }
}

//텍스트를 출력하는 함수
//x좌표, y좌표, x길이, y길이, 문자열
void DrawTXT(int x, int y, int size_x, int size_y, char spr[]) {
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            EditConsole(x + j, y + i, spr[i * size_x + j]);
        }
    }
}
//음식창고 로고를 consoleData에 출력
void logo() {
    DrawTXT(1, 1, 6, 2, "음  식창  고");
}
//선택 입력칸 '>>' 을 consoleData에 출력
void selectMenu() {
    DrawTXT(1, 39, 3, 1, ">> ");
}

void startMenu() {//메인메뉴 출력함수
    FillConsole(consoleData, ' ', Max_value);

    DrawTXT(16, 8, 11, 1, "1. 음식목록");
    DrawTXT(16, 10, 11, 1, "2. 음식추가");
    DrawTXT(16, 12, 7, 1, "3. 설정");
    DrawTXT(16, 14, 3, 1, "4. ");
    DrawTXT(16, 17, 7, 1, "0. 종료");

    logo();
    selectMenu();
    //DrawTXT() 함수는 consoleData 배열에만 출력해서 따로 출력해줘야함
    printf("%s", consoleData);
    //>> 앞으로 커서를 옮기기 위해서 gotoxy()함수 사용
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

void foodDisplay(Food* foods, int num_food) { //음식목록 화면 출력 함수 // 테스트 중
    FillConsole(consoleData, ' ', Max_value);
    char str_days[37];
    char str_num[20];
    if (num_food >= 1) {
        for (int line = 8, i = 0; line < 35, i < 27; line++, i++) {
            int remaining_days = get_remaining_days(foods[i].expiration_date);

            if (remaining_days >= 0) {
                sprintf(str_days, "%d. %s(%d일)", i + 1, foods[i].name, remaining_days);
                DrawTXT(12, line, 37, 1, str_days);
            }
            else {
                sprintf(str_days, "%d. %s(%d일)", i + 1, foods[i].name, remaining_days);
                DrawTXT(12, line, 37, 1, str_days);
            }
        }
    }
    else {
        DrawTXT(12, 8, 15, 1, "항목이 없습니다.");
    }
    sprintf(str_num, "음식 개수: %d", num_food);
    DrawTXT(12, 7, strlen(str_num), 1, str_num);

    DrawTXT(1, 38, 11, 1, "(0)돌아가기");
    logo();
    selectMenu();

    printf("%s", consoleData);
    gotoxy(3, 38);

    int num; scanf("%d", &num);
    if (num == 0) {
        pageStatus = 0;
    }
}

void foodAdd(Food* foods, int* num_food) {//음식추가 화면 출력 함수 // 미완성
    FillConsole(consoleData, ' ', Max_value);
    
    DrawTXT(16, 8, 7, 1, "1. 육류");
    DrawTXT(16, 10, 9, 1, "2. 유제품");
    DrawTXT(16, 12, 7, 1, "3. 김치");
    DrawTXT(16, 14, 7, 1, "4. 음료");
    DrawTXT(1, 38, 11, 1, "(0)돌아가기");
    
    logo();
    selectMenu();
    printf("%s", consoleData);
    gotoxy(3, 38);

    char name[MAX_NAME_LENGTH];
    char expiration_date[MAX_DATE_LENGTH];
    char note[MAX_NOTE_LENGTH];

    int category; scanf("%d", &category);
    if (category == 0) {
        pageStatus = 0;
    }
    else if (category == 1 || category == 2 || category == 3 || category == 4) {
        FillConsole(consoleData, ' ', Max_value);

        DrawTXT(16, 8, 12, 1, "1. 음식 이름");
        DrawTXT(16, 10, 12, 1, "2. 소비 기한");
        DrawTXT(16, 12, 12, 1, "3. 추가 정보");
        logo();
        selectMenu();


        DrawTXT(1, 38, 15, 1, "1. 음식 이름");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 38);
        scanf("%s", name);


        system("cls");
        DrawTXT(1, 38, 28, 1, "2. 소비 기한 (ex.2020-12-12)");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 38);
        scanf("%s", expiration_date);


        system("cls");
        DrawTXT(1, 38, 15, 1, "3. 추가 정보");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 38);
        scanf("%s", note);

        foods[*num_food] = (Food){ category, "", "", "" };  //에러 발생 위치 수정 요먕
        strcpy(foods[*num_food].name, name);
        strcpy(foods[*num_food].expiration_date, expiration_date);
        strcpy(foods[*num_food].note, note);
        (*num_food)++;
    }
}

void displaySetting() {//설정 화면 출력 함수
    FillConsole(consoleData, ' ', Max_value);

    DrawTXT(16, 8, 9, 1, "1. 사용자");
    DrawTXT(16, 10, 7, 1, "2. 온도");
    DrawTXT(16, 12, 12, 1, "3. 김치 종류");
    DrawTXT(1, 38, 11, 1, "(0)돌아가기");

    logo();
    selectMenu();
    printf("%s", consoleData);
    gotoxy(3, 38);

    int category; scanf("%d", &category);
    if (category == 0) {
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

    while (1) {
        while (pageStatus == 0) {//메인 메뉴
            //콘솔창 초기화 함수
            system("cls");
            startMenu();
        }
        while (pageStatus == 1) {//음식 목록
            system("cls");
            foodDisplay(foods, num_food);
        }
        while (pageStatus == 2) {//음식 추가
            system("cls");
            foodAdd(foods, num_food);
        }
        while (pageStatus == 3) {//설정
            system("cls");
            displaySetting();
        }
    }
    return(0);
}