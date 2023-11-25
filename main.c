#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#define max 50

struct Node {
	char food[max], etc[max], experiment[max];
	int dday;
	struct Node* next;
}*first, * pre, * cur, * newrec;
char food_list_str1[max] = "삭제 및 상세정보 확인은 번호 입력: ";
char food_list_str2[max] = "삭제 - d, 상세정보 확인 - e";

void sort_food_list() {
	first = NULL;
	pre = NULL;
	newrec = NULL;

	FILE* fp1;
	fp1 = fopen("data.txt", "r");
	if (fp1 == NULL) return -1;

	char c;
	char food[max];
	int arr[max], i = 0, j = 0, flag = 1;
	while ((c = fgetc(fp1)) != EOF) {
		food[i++] = c;
		
		if (c == '&' && flag == 2) {
			food[--i] = '\0';
			strcpy(newrec->experiment, food);
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

			newrec->dday = calculate_D_day(arr[0], arr[1], arr[2]);
			if (first == NULL) first = newrec;

			else {
				cur = first;
				while (cur != NULL) {
					if (newrec->dday > cur->dday) {
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
		}
	}

	int a = display_food_list();
	printf("%s", food_list_str1);

	fclose(fp1);
}

int calculate_D_day(int y, int m, int d) {
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

int display_food_list() {
	system("cls");
	printf("-----------------------------------\n");
	printf(" (로고)\n");
	printf("음식창고\n");
	printf("-----------------------------------\n");
	cur = first;
	int a = 1;
	while (cur != NULL) {
		if (cur->dday > 0) printf("%d. %s(%d)\n", a++, cur->food, cur->dday);
		else printf("%d. %s(!%d)\n", a++, cur->food, cur->dday * -1);
		cur = cur->next;
	}
	printf("-----------------------------------\n");

	return a;
}

void delete(int num) {
	FILE* fp;
	cur = first;
	for (int a = 1; a < num; a++) {
		pre = cur;
		cur = cur->next;
	}
	if (cur == first) first = cur->next;
	else pre->next = cur->next;

	fp = fopen("data.txt", "w");
	cur = first;
	while (cur != NULL) {
		fprintf(fp, "%s&%s&%s\n", cur->food, cur->experiment, cur->etc);
		cur = cur->next;
	}
}

void print_etc(int num) {
	cur = first;
	for (int a = 1; a < num; a++) {
		cur = cur->next;
	}
	printf("%s\n", cur->etc);
}

void free_malloc() {
	cur = first;
	while (cur != NULL) {
		pre = cur;
		cur = cur->next;
		free(pre);
	}
}