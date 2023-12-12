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
int etc = 0;
char etc_str[MAX_NOTE_LENGTH];

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

struct Node { // 삽입 정렬을 위한 연결리스트
    int category;
    char food[MAX_NAME_LENGTH], etc[MAX_NOTE_LENGTH], expiration[MAX_DATE_LENGTH];
    int remaining_days;
    struct Node* next;
}*first, * pre, * cur, * newrec;

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

int calculateDays(int y, int m, int d) {
    time_t cur;
    cur = time(NULL);
    struct tm* a;
    a = localtime(&cur);

    int flag = 1;
    if (y / 4 == 0) {
        if (y / 400 == 0) flag = 0;
        else if (y / 100 == 0);
        else flag = 0;
    }
    y = y - 1900;

    for (int i = 1; i < m; i++) {
        switch (i) {
        case 1:d = d + 31; break;
        case 2: {
            d = d + 28;
            if (flag == 0) d++;
            break;
        }
        case 3:d = d + 31; break;
        case 4:d = d + 30; break;
        case 5:d = d + 31; break;
        case 6:d = d + 30; break;
        case 7:d = d + 31; break;
        case 8:d = d + 31; break;
        case 9:d = d + 30; break;
        case 10:d = d + 31; break;
        case 11:d = d + 30; break;
        default:break;
        }
    }

    d = d + (y - a->tm_year) * 365;
    return d - a->tm_yday - 1;
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

void sort_food_list() {
    first = NULL;
    pre = NULL;
    newrec = NULL;

    FILE* fp1;
    fp1 = fopen("data.txt", "r");
    if (fp1 == NULL) return -1;

    char c;
    char food[100];
    int arr[100], i = 0, j = 0, flag = 1;
    int a = 0;
    while ((c = fgetc(fp1)) != EOF) {
        food[i++] = c;

        if (c == '&' && flag == 2) {
            food[--i] = '\0';
            strcpy(newrec->expiration, food);
            int result = 0;
            j = 0;
            for (int a = 0; food[a] != '\0'; a++) {
                if (food[a] == '-') {
                    arr[j++] = result;
                    result = 0;
                    continue;
                }
                result = result * 10 + (food[a] - '0');
            }
            arr[j] = result;

            newrec->remaining_days = calculateDays(arr[0], arr[1], arr[2]);
            if (first == NULL) first = newrec;

            else {
                cur = first;
                while (cur != NULL) {
                    if (newrec->remaining_days > cur->remaining_days) {
                        pre = cur;
                        cur = cur->next;
                    }

                    else break;
                }
                if (cur == first) first = newrec;
                else pre->next = newrec;

                newrec->next = cur;
            }
            i = 0;
        }

        if (c == '&' && flag == 1) {
            food[--i] = '\0';
            newrec = malloc(sizeof(struct Node));
            i = 0;

            if (newrec == NULL) {
                return -1;
            }

            strcpy(newrec->food, food);
            newrec->next = NULL;

            flag = 2;
        }

        if (c == '\n') {
            food[--i] = '\0';
            strcpy(newrec->etc, food);
            i = 0;
            flag = 1;
            a++;
        }
    }
    fclose(fp1);

    num_food = a;
}

void foodDisplay(int str, int n) {  // 음식목록 화면 출력 함수
    system("cls");                  // DrawTXT로 받아 consoleData에 쓰는 방법은 작동하지 않아서
    cur = first;                    // 쓰지 못했습니다..
    int a = 1, x = 11, y = 7;
    if (num_food != 0) {
        if (num_food < 11) n = 1;
        while (cur != NULL) {
            if (a < n) {
                a++;
                cur = cur->next;
                continue;
            }
            if (a > n + 9) break;

            gotoxy(x, y);
            if (cur->remaining_days > 0) printf("%d. %s(%d)\n", a, cur->food, cur->remaining_days);
            else printf("%d. %s(!%d)\n", a, cur->food, cur->remaining_days * -1);
            cur = cur->next;
            a++;
            y++;
        }
        gotoxy(0, 35);
        if (str == 1) printf("삭제 및 상세정보 확인은 번호 입력");
        if (str == 2) printf("삭제 - d, 상세정보 확인 - e");
    }

    else {
        gotoxy(11, 7);
        printf("항목이 없습니다.");
    }

    gotoxy(11, 6);
    printf("음식 개수: %d", num_food);
    if (etc == 1) {
        gotoxy(0, 36);
        printf("%s", etc_str);
    }
    gotoxy(0, 0);
    printf("음  식\n창  고");
    gotoxy(0, 37);
    printf("(0)돌아가기\n>>");

    gotoxy(3, 38);
}

int input(int min) { // 음식 목록에서 입력을 받는 함수 (정수 - 문자 순)
    int n;
    int str = 1;
    char c;
    while (1) {
        scanf("%d", &n);
        getchar();
        if (n == 0) {
            return -1;
        }

        else if (n >= 1 && n <= num_food) {
            while (1) {
                str = 2;
                foodDisplay(str, min);
                printf(" ");
                scanf("%c", &c);
                getchar();

                if (c == 'd') {
                    if (n == num_food && min <= num_food - 9) min--;
                    FILE* fp;
                    cur = first;
                    for (int a = 1; a < n; a++) {
                        pre = cur;
                        cur = cur->next;
                    }
                    if (cur == first) {
                        pre = first;
                        first = cur->next;
                        free(pre);
                    }
                    else {
                        pre->next = cur->next;
                        free(cur);
                    }

                    fp = fopen("data.txt", "w");
                    cur = first;
                    int a = 0;
                    while (cur != NULL) {
                        fprintf(fp, "%s&%s&%s\n", cur->food, cur->expiration, cur->etc);
                        cur = cur->next;
                        a++;
                    }
                    num_food = a;
                    break;
                }

                else if (c == 'e') {
                    cur = first;
                    for (int a = 1; a < n; a++) {
                        cur = cur->next;
                    }
                    etc = 1;
                    sprintf(etc_str, "%d. %s: %s", n, cur->food, cur->etc);
                    break;
                }

                else;
            }
            break;
        }

        else break;
    }


    return min;
}

void foodAdd(Food* foods, int* num_food) {
    FillConsole(consoleData, ' ', Max_value);

    DrawTXT(16, 8, 7, 1, "1. 육류");
    DrawTXT(16, 10, 9, 1, "2. 유제품");
    DrawTXT(16, 12, 7, 1, "3. 김치");
    DrawTXT(16, 14, 7, 1, "4. 음료");
    DrawTXT(1, 38, 11, 1, "(0)돌아가기");

    logo();
    selectMenu();
    printf("%s", consoleData);
    gotoxy(3, 40);

    char name[MAX_NAME_LENGTH];
    char expiration_date[MAX_DATE_LENGTH];
    char note[MAX_NOTE_LENGTH];
    Food newFood;

    int category; scanf("%d", &category);
    if (category == 0) {
        pageStatus = 0;
    }
    else if (category == 1 || category == 2 || category == 4) {
        FillConsole(consoleData, ' ', Max_value);

        DrawTXT(16, 8, 12, 1, "1. 음식 이름");
        DrawTXT(16, 10, 12, 1, "2. 소비 기한");
        DrawTXT(16, 12, 12, 1, "3. 추가 정보");
        logo();
        selectMenu();


        DrawTXT(1, 38, 15, 1, "1. 음식 이름");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 40);
        scanf_s("%s", name, 255);


        system("cls");
        DrawTXT(1, 38, 28, 1, "2. 소비 기한 (ex.2020-12-12)");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 40);
        scanf_s("%s", expiration_date, 255);


        system("cls");
        DrawTXT(1, 38, 15, 1, "3. 추가 정보");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 40);
        scanf_s("%s", note, 255);

        newFood.category = category;
        strcpy(newFood.name, name);
        strcpy(newFood.expiration_date, expiration_date);
        strcpy(newFood.note, note);

        foods[*num_food] = newFood;
        (*num_food)++;
    }

    if (category == 3) {
        FillConsole(consoleData, ' ', Max_value);

        DrawTXT(16, 8, 12, 1, "1. 김치 종류 (aa 배추김치, bb 깍두기, cc 파김치)");
        DrawTXT(16, 10, 12, 1, "2. 제조 일자 (yyyy-mm-dd)");
        DrawTXT(16, 12, 12, 1, "3. 특이 사항");
        logo();
        selectMenu();

        DrawTXT(1, 38, 28, 1, "1. 김치 종류 (a, b, c)");
        system("cls");
        printf("%s", consoleData);
        gotoxy(3, 40);

        scanf_s(" %s", name, 255);

        DrawTXT(1, 38, 28, 1, "2. 제조 일자 (yyyy-mm-dd)");
        printf("%s", consoleData);
        gotoxy(3, 40);

        char manufacture_date[MAX_DATE_LENGTH];
        scanf_s("%s", manufacture_date, 255);

        DrawTXT(1, 38, 15, 1, "3. 특이 사항");
        printf("%s", consoleData);
        gotoxy(3, 40);

        char note[MAX_NOTE_LENGTH];
        scanf_s("%s", note, 255);

        int remaining_days = -1; //다른 부분 완성후 연결필요
        if (name[*num_food] == 'a') {
            remaining_days = 28;
        }
        else if (name[*num_food] == 'b') {
            remaining_days = 10;
        }
        else if (name[*num_food] == 'c') {
            remaining_days = 7;
        }

        newFood.category = category;
        strncpy(newFood.name, name, 50);
        strncpy(newFood.note, note, 255);

        foods[*num_food] = newFood;
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
            sort_food_list();
            int min = 1;
            int str = 1;
            foodDisplay(str, min);
            while (min != -1) { 
                foodDisplay(str, min);

                if (GetAsyncKeyState(VK_UP) & 0x8000) {// 위, 아래 방향키로 스크롤 조작
                    if (min > 1 && min <= num_food - 9) min--;
                }
                if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    if (min >= 1 && min < num_food - 9) min++;
                }
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {// 스페이스바를 누른 후 입력 받음.
                    etc = 0;
                    min = input(min);
                }
                Sleep(100);
            }
            if (min == -1) pageStatus = 0;
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