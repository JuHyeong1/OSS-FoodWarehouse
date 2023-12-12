//비주얼 스튜디오 오류 차단
//#pragma warning(disable:4996)

//#################################################################################
//헤더 선언

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

//#################################################################################


//#################################################################################
//전역 변수 선언

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_NOTE_LENGTH 100
#define MAX_FOODS 100
#define Console_X_MAX 52
#define Console_Y_MAX 40
#define Max_value Console_X_MAX * Console_Y_MAX

int pageStatus;//현재 화면 상태 1: 메인화면 2: 음식목록 3: 음식추가 9:종료
int num_food = 0;
int etc = 0;
char etc_str[MAX_NOTE_LENGTH];

//#################################################################################


//#################################################################################
//구조체 선언

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

//#################################################################################


//#################################################################################
//함수 원형

void UI();

int getRemainingDays(char* expiration_date);
int calculateDays(int y, int m, int d);

void settingConsole();
void settingTextColor(int color_number);
void gotoxy(int x, int y);

void displayMainMenu();

void displayFoodList(int str, int n);
void sortingFoodList();
int input(int min);

void addFood(Food* foods, int* num_food);

void displaySetting();

//#################################################################################


void main() {
    //Food foods[MAX_FOODS];
    Food* foods = NULL;
    foods = (Food*)malloc(sizeof(Food) * MAX_FOODS);

    int num_food = 0;

    srand(time(NULL));
    settingConsole();

    pageStatus = 0;
    while (1) {
        //pageStatus = 0;
        switch (pageStatus) {
            case 0://메인메뉴
            {//switch 문 안의 case 내에서 변수 선언을 위해서는 중괄호가 필요하다.
                int num;
                system("cls");
                displayMainMenu();
                gotoxy(5, 36);
                scanf("%d", &num);
                switch (num) {
                case 0:
                    pageStatus = 9;
                    break;
                case 1:
                case 2:
                case 3:
                    pageStatus = num;
                    break;
                default:
                    printf("잘못된 값이 입력되었습니다.");
                }
                Sleep(50);
                break;
            }
            case 1://음식목록
            {

                int min = 1;
                int str = 1;

                system("cls");
                sortingFoodList();
                displayFoodList(str, min);

                while (min != -1) {
                    system("cls");
                    displayFoodList(str, min);

                    if (GetAsyncKeyState(VK_UP) & 0x8000) {// 위, 아래 방향키로 스크롤 조작
                        if (min > 1 && min <= num_food - 9)
                            min--;
                    }
                    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                        if (min >= 1 && min < num_food - 9)
                            min++;
                    }
                    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {// 스페이스바를 누른 후 입력 받음.
                        etc = 0;
                        min = input(min);
                    }
                    Sleep(100);
                }
                if (min == -1)
                    pageStatus = 0;
            }
                break;
            case 2://음식추가
                system("cls");
                addFood(foods, &num_food);
                break;
            case 3://설정
                system("cls");
                displaySetting();
                break;
            case 9://종료
                system("cls");
                gotoxy(1,1);
                printf("프로그램을 종료합니다.");
                free(foods);
                Sleep(10000);
                exit(0);
                break;
            default:
                system("cls");
                UI();
                gotoxy(26, 20);
                printf("잘못된 입력입니다.");
                Sleep(10000);
        }
    }
    free(foods);
    exit(0);
}


//#################################################################################
//함수 정의

//UI
void UI() {
    puts("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
    for (int i = 0; i < 36; i++)
        puts("■                                                ■");
    puts("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

    gotoxy(3, 3); printf("음  식");
    gotoxy(3, 4); printf("창  고");

    gotoxy(2, 36); printf(">> ");
}

//유통기한 계산
int getRemainingDays(char* expiration_date) {
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

//콘솔
void settingConsole() {//콘솔창 초기 설정 함수
    system("title FoodWarehouse"); //콘솔창 이름 설정
    system("mode con:cols=52 lines=40"); //콘솔창 크기 설정

    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 10; //콘솔창에서 커서의 크기
    ConsoleCursor.dwSize = 1;   //커서의 가시성 (true: 보임, false: 안보임)

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}
void settingTextColor(int color_number) {//텍스트 컬러 설정 함수
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
    // 0: 검정 1: 파랑 2: 초록 3: 옥색 4: 빨강 5: 자주색
    // 6: 노랑 7: 하양 8: 회색 9: 연파랑 10: 연초록
    // 11: 연옥색 12: 연빨강 13: 연자주 14: 연노랑 15: 진한 회색
}
void gotoxy(int x, int y) {//커서를 해당 좌표로 이동
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Cur;
    Cur.X = x;
    Cur.Y = y;
    SetConsoleCursorPosition(consoleHandle, Cur);
}

//메인메뉴
void displayMainMenu() {
    UI();
    gotoxy(16, 8); printf("1. 음식목록");
    gotoxy(16, 10); printf("2. 음식추가");
    gotoxy(16, 12); printf("3. 설정");
    gotoxy(16, 17); printf("0. 종료");
}

//음식목록
void displayFoodList(int str, int n) {
    UI();

    cur = first;
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
            if (cur->remaining_days > 0)
                printf("%d. %s(%d)\n", a, cur->food, cur->remaining_days);
            else
                printf("%d. %s(!%d)\n", a, cur->food, cur->remaining_days * -1);
            cur = cur->next;
            a++;
            y++;
        }
        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35);
        if (str == 1)
            printf("삭제 및 상세정보 확인은 번호 입력");
        if (str == 2)
            printf("삭제 - d, 상세정보 확인 - e");
    }

    else {
        gotoxy(11, 7);
        printf("항목이 없습니다.");
    }

    gotoxy(11, 6);
    printf("음식 개수: %d", num_food);
    if (etc == 1) {
        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35);
        printf("%s", etc_str);
    }
}
void sortingFoodList() {
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
                displayFoodList(str, min);
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

//음식추가
void addFood(Food* foods, int* num_food) {
    UI();

    gotoxy(16, 8); printf("1. 육류");
    gotoxy(16, 10); printf("2. 유제품");
    gotoxy(16, 12); printf("3. 김치");
    gotoxy(16, 14); printf("4. 음료");
    gotoxy(2, 35); printf("                              ");
    gotoxy(2, 35); printf("(0)돌아가기");

    gotoxy(5, 36);

    char name[MAX_NAME_LENGTH];
    char expiration_date[MAX_DATE_LENGTH];
    char note[MAX_NOTE_LENGTH];
    Food newFood;

    int category = 9;
    scanf("%d", &category);

    if (category == 0) {
        pageStatus = 0;
    }
    else if (category == 1 || category == 2 || category == 4) {
        system("cls");
        UI();

        gotoxy(16, 8); printf("1. 음식 이름");
        gotoxy(16, 10); printf("2. 소비 기한 (yyyy-mm-dd)");
        gotoxy(16, 12); printf("3. 추가 정보");

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("1. 음식 이름");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf("%s", name);

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("2. 소비 기한 (yyyy-mm-dd)");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf("%s", expiration_date);

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("3. 추가 정보");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf("%s", note);


        newFood.category = category;
        strcpy(newFood.name, name);
        strcpy(newFood.expiration_date, expiration_date);
        strcpy(newFood.note, note);
        foods[*num_food] = newFood;
        (*num_food)++;
    }

    if (category == 3) {
        char manufacture_date[MAX_DATE_LENGTH];
        char note[MAX_NOTE_LENGTH];

        system("cls");
        UI();

        gotoxy(16, 8); printf("1. 김치 종류");
        gotoxy(10, 9); printf("(aa 배추김치, bb 깍두기, cc 파김치)");
        gotoxy(16, 11); printf("2. 제조 일자 (yyyy-mm-dd)");
        gotoxy(16, 13); printf("3. 추가 정보");

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("1. 김치 종류 (a, b, c)");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf(" %s", name);

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("2. 제조 일자 (yyyy-mm-dd)");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf("%s", manufacture_date);

        gotoxy(2, 35); printf("                              ");
        gotoxy(2, 35); printf("3. 추가 정보");
        gotoxy(5, 36); printf("                              ");
        gotoxy(5, 36);
        scanf("%s", note);

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

//설정
void displaySetting() {
    UI();
    gotoxy(16, 8); printf("1. 사용자");
    gotoxy(16, 10); printf("2. 온도");
    gotoxy(16, 12); printf("3. 김치 종류");
    gotoxy(2, 35); printf("                              ");
    gotoxy(2, 35); printf("(0)돌아가기");

    gotoxy(5, 36); printf("                              ");
    gotoxy(5, 36);

    int category = 999; scanf("%d", &category);
    switch (category) {
        case(1):
            gotoxy(16, 34);
            printf("사용자 이름");
            gotoxy(5, 36);
            Sleep(1000);
            break;
        case(2):
            gotoxy(16, 34);
            printf("온도");
            gotoxy(5, 36);
            Sleep(1000);
            break;
        case(3):
            gotoxy(16, 34);
            printf("김치 종류");
            gotoxy(5, 36);
            Sleep(1000);
            break;
        case(0):
            pageStatus = 0;
            break;
        default:
            gotoxy(5, 36);
            printf("잘못된 입력입니다.");
            gotoxy(5, 36);
            Sleep(1000);
    }
}

//#################################################################################