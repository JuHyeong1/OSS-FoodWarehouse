#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#define MAX 50

struct Node {
	char food[MAX], etc[MAX], experiment[MAX];
	int dday;
	struct Node* next;
}*first, * pre, * cur, * newrec;
char food_list_str1[MAX] = "삭제 및 상세정보 확인은 번호 입력";
char food_list_str2[MAX] = "삭제 - d, 상세정보 확인 - e";

int sort_food_list();
int calculate_D_day(int y, int m, int d);
void display_food_list(int str_change, int n, int max);
int delete(int num);
void print_etc(int num);
void free_malloc();
void SetConsole();
void gotoxy(int x, int y);

int main() {
	SetConsole();
	int x = 2, y = 20, min = 1;
	int max = sort_food_list();
	int str = 1;
	display_food_list(str, min, max);
	printf(">>");

	int n;
	char c;

	while (1) {
		display_food_list(str, min, max);
		gotoxy(x, y);

		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (min > 1 && min <= max - 8) min--;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			if (min >= 1 && min < max - 8) min++;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			scanf("%d", &n);

			if (n == 0) break;

			else if (n >= 1 && n <= max) {
				str = 2;
				display_food_list(str, min, max);
				scanf("%c", &c);

				if (c == 'd') {
					max = delete(n);
					str = 1;
				}

				else if (c == 'e') {
					print_etc(n);
					str = 1;
				}

				else;
			}

			else;
		}
		Sleep(100);
	}
}

int sort_food_list() {
	first = NULL;
	pre = NULL;
	newrec = NULL;

	FILE* fp1;
	fp1 = fopen("data.txt", "r");
	if (fp1 == NULL) return -1;

	char c;
	char food[MAX];
	int arr[MAX], i = 0, j = 0, flag = 1;
	int a = 0;
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
			a++;
		}
	}
	printf("%s", food_list_str1);
	fclose(fp1);

	return a;
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

void display_food_list(int str_change, int n, int max) {
	system("cls");
	printf("-----------------------------------\n");
	printf(" (로고)\n");
	printf("음식창고\n");
	printf("-----------------------------------\n");
	cur = first;
	int a = 1, x = 6, y = 7;
	if (max < 10) n = 1;
	while (cur != NULL) {
		if (a < n) {
			a++;
			cur = cur->next;
			continue;
		}
		if (a > n + 8) break;

		gotoxy(x, y);
		if (cur->dday > 0) printf("%d. %s(%d)\n", a, cur->food, cur->dday);
		else printf("%d. %s(!%d)\n", a, cur->food, cur->dday * -1);
		cur = cur->next;
		y++;
		a++;
	}
	x = 6;
	y = 17;
	gotoxy(x, y);
	if (str_change == 1) printf("0. 종료\n%s\n", food_list_str1);
	if (str_change == 2) printf("0.종료\n%s\n", food_list_str2);
	printf("-----------------------------------\n");
	printf(">>");
}

int delete(int n) {
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
		fprintf(fp, "%s&%s&%s\n", cur->food, cur->experiment, cur->etc);
		cur = cur->next;
		a++;
	}

	return a;
}

void print_etc(int n) {
	cur = first;
	for (int a = 1; a < n; a++) {
		cur = cur->next;
	}
	printf("%s\n>>", cur->etc);
}

void free_malloc() {
	cur = first;
	while (cur != NULL) {
		pre = cur;
		cur = cur->next;
		free(pre);
	}
}

void SetConsole() {
	system("title AAA");
	system("mode con:cols=100 lines=25");

	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 1;
	ConsoleCursor.dwSize = 1;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(consoleHandle, Cur);
}