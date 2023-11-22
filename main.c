#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_NOTE_LENGTH 100
#define MAX_FOODS 100

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

void display_option(){
    printf("1. 사용자 이름\n");
    printf("2. 냉장고 온도\n");
    printf("3. 종료\n");
    
    option_open();
    
}



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

void display_food_list(Food *foods, int num_food) {
    printf("-------------------------------\n");
    printf(" (로고)\n");
    printf("음식창고\n");
    printf("-------------------------------\n");

    for (int i = 0; i < num_food; i++) {
        int remaining_days = get_remaining_days(foods[i].expiration_date);
        if (remaining_days >= 0) {
            printf("%d. %s(%d일)\n", i + 1, foods[i].name, remaining_days);
        } else {
            printf("%d. %s(!%d일)\n", i + 1, foods[i].name, -remaining_days);
        }
    }

    printf("-------------------------------\n");
    printf("선택: \n");
}
void food_open()
  {
    FILE *fp;
    char file_buff[100];
    fp = fopen("data.txt", "a");

    if (fp == NULL) printf("파일열기 실패\n");

    int i;

    for (i = 1; i <= 3; i++) {
      printf("파일에 적을 내용을 입력하세요 (%d번째 라인)\n", i);
      memset(file_buff, 0, sizeof(file_buff));
      scanf("%s", file_buff);
      if(i==3)
      {
      file_buff[strlen(file_buff)] = '\n';
      fputs(file_buff, fp);

        
      fclose(fp);
      }
      file_buff[strlen(file_buff)] = '&';
      fputs(file_buff, fp);

    }
      
    
  }

  void option_open()
  {
    FILE *fp;
    char file_buff[100];
    fp = fopen("option.txt", "a");

    if (fp == NULL) printf("파일열기 실패\n");

    int i;

    for (i = 1; i <= 2; i++) {
      printf("파일에 적을 내용을 입력하세요 (%d번째 라인)\n", i);
      memset(file_buff, 0, sizeof(file_buff));
      scanf("%s", file_buff);
      if(i==2)
      {
      file_buff[strlen(file_buff)] = '\n';
      fputs(file_buff, fp);

        
      fclose(fp);
      }
      file_buff[strlen(file_buff)] = '&';
      fputs(file_buff, fp);

    }
      
    
  }


void add_food(Food *foods, int *num_food) {
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
    
    food_open();
    (*num_food)++;
}

int get_remaining_days(char *expiration_date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Date expiration;
    sscanf(expiration_date, "%d-%d-%d", &expiration.year, &expiration.month, &expiration.day);

    struct tm expiration_tm = {0};
    expiration_tm.tm_year = expiration.year - 1900;
    expiration_tm.tm_mon = expiration.month - 1;
    expiration_tm.tm_mday = expiration.day;

    time_t expiration_time = mktime(&expiration_tm);
    double remaining_seconds = difftime(expiration_time, t);
    int remaining_days = (int)(remaining_seconds / (60 * 60 * 24));

    return remaining_days;
}

int main() {
    Food foods[MAX_FOODS];
    int num_food = 0;
    int choice;

    do {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_food_list(foods, num_food);

                break;
            case 2:
                add_food(foods, &num_food);
                break;
            case 3:
                // 설정 기능 추가
                break;
            case 4:
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다. 다시 선택하세요.\n");
        }
    } while (choice != 4);

    return 0;
}