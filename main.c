#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define max 50

int print_food_list() {
	struct Node {
		char food[max], etc[max];
		int dday;
		struct Node* next;
	}* first, * pre, * cur, * newrec;
	first = NULL;
	pre = NULL;
	newrec = NULL;

	FILE* fp1;
	fp1 = fopen("data.txt", "r");
	if (fp1 == NULL) return -1;

	char c;
	char food[max];
	int arr[max], i = 0, j = 0, flag = 1;
	int t = 0;
	while ((c = fgetc(fp1)) != EOF) {
		food[i++] = c;
		
		if (c == '&' && flag == 2) {
			food[--i] = '\0';
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

	cur = first;
	int a = 1;
	while (cur != NULL) {
		if (cur->dday > 0) printf("%d. %s %d %s\n", a++, cur->food, cur->dday, cur->etc);
		else printf("%d. %s !%d %s\n", a++, cur->food, cur->dday * -1, cur->etc);
		cur = cur->next;
	}
	
	return 0;
}

int calculate_D_day(int y, int m, int d) {
	y = y - 1900;

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

	for (int i = 1; i < m; i++) {
		switch (i) {
		case 1:d = d + 31; break;
		case 2: {
			d = d + 28;
			if (flag == 0) d = d + 29;
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
	return d - a->tm_yday;
}