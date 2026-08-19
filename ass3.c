#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50

typedef struct {
	int roll_number;
	char name[MAX_NAME_LENGTH];
} Student;

typedef enum {
	MENU_INPUT = 1,
	MENU_RANDOMIZE,
	MENU_DISPLAY,
	MENU_LINEAR_SEARCH,
	MENU_BINARY_SEARCH,
	MENU_INSERTION_SORT,
	MENU_SELECTION_SORT,
	MENU_SHELL_SORT,
	MENU_EXIT
} MenuOption;

bool is_valid_input(const char *str) {
	for (int i = 0; str[i] != '\0'; i++) {
		bool is_alpha = (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z');
		bool is_sym = isdigit((unsigned char)str[i]) || str[i] == '-' || str[i] == '+' || isspace((unsigned char)str[i]);

		if (is_alpha || !is_sym) {
			return false;
		}
	}
	return true;
}

bool is_in_range(int val, int min, int max) {
	return val >= min && val <= max;
}

bool read_line(char *buf, size_t size) {
	if (fgets(buf, size, stdin) == NULL) {
		clearerr(stdin);
		return false;
	}
	return true;
}

bool parse_int(const char *buf, int *res) {
	if (!is_valid_input(buf)) {
		return false;
	}
	char trail;
	return sscanf(buf, "%d %c", res, &trail) == 1;
}

int get_int(void) {
	char buf[256];
	int val;
	while (true) {
		if (read_line(buf, sizeof(buf)) && parse_int(buf, &val)) {
			return val;
		}
		printf("Invalid integer input.\nRetry: ");
	}
}

int get_int_in_range(int min, int max) {
	while (true) {
		int val = get_int();
		if (is_in_range(val, min, max)) {
			return val;
		}
		printf("Value out of bounds.\nRetry: ");
	}
}

void get_string(char *buf, size_t size) {
	while (true) {
		if (read_line(buf, size)) {
			buf[strcspn(buf, "\r\n")] = '\0';
			if (strlen(buf) > 0) {
				return;
			}
		}
		printf("Invalid string input.\nRetry: ");
	}
}

void populate_student_db(Student db[], int *count) {
	printf("Enter total number of students (1-%d): ", MAX_STUDENTS);
	*count = get_int_in_range(1, MAX_STUDENTS);

	for (int i = 0; i < *count; i++) {
		printf("\nStudent %d Roll Number: ", i + 1);
		db[i].roll_number = get_int_in_range(1, 99999);

		printf("Student %d Name: ", i + 1);
		get_string(db[i].name, MAX_NAME_LENGTH);
	}
}

void generate_random_students(Student db[], int *count) {
	printf("Enter total number of random records to generate (1-%d): ", MAX_STUDENTS);
	*count = get_int_in_range(1, MAX_STUDENTS);

	const char *first[] = {"Alex", "Sam", "Jordan", "Taylor", "Morgan", "Chris", "Pat", "Riley", "Dakota", "Avery", "Liam", "Emma", "Noah", "Olivia"};
	const char *last[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez"};

	int t_first = sizeof(first) / sizeof(first[0]);
	int t_last = sizeof(last) / sizeof(last[0]);

	bool used[1000] = {false};

	for (int i = 0; i < *count; i++) {
		int roll;
		do {
			roll = (rand() % 900) + 100;
		} while (used[roll]);

		used[roll] = true;
		db[i].roll_number = roll;

		snprintf(db[i].name, MAX_NAME_LENGTH, "%s %s", first[rand() % t_first], last[rand() % t_last]);
	}

	printf("\nSuccessfully generated %d random student records!\n", *count);
}

void print_student_db(const Student db[], int count) {
	printf("\n%-15s %-30s\n", "Roll Number", "Name");
	printf("----------------------------------------------\n");
	for (int i = 0; i < count; i++) {
		printf("%-15d %-30s\n", db[i].roll_number, db[i].name);
	}
}

void swap_students(Student *a, Student *b) {
	Student temp = *a;
	*a = *b;
	*b = temp;
}

int linear_search(const Student db[], int count, int target) {
	for (int i = 0; i < count; i++) {
		if (db[i].roll_number == target) {
			return i;
		}
	}
	return -1;
}

int binary_search(const Student db[], int count, int target) {
	int left = 0;
	int right = count - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;

		if (db[mid].roll_number == target) {
			return mid;
		}
		if (db[mid].roll_number < target) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return -1;
}

void insertion_sort(Student db[], int count) {
	for (int i = 1; i < count; i++) {
		Student key = db[i];
		int j = i - 1;

		while (j >= 0 && db[j].roll_number > key.roll_number) {
			db[j + 1] = db[j];
			j = j - 1;
		}
		db[j + 1] = key;
	}
}

void selection_sort(Student db[], int count) {
	for (int i = 0; i < count - 1; i++) {
		int min_idx = i;
		for (int j = i + 1; j < count; j++) {
			if (db[j].roll_number < db[min_idx].roll_number) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			swap_students(&db[min_idx], &db[i]);
		}
	}
}

void shell_sort(Student db[], int count) {
	for (int gap = count / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < count; i += 1) {
			Student temp = db[i];
			int j;
			for (j = i; j >= gap && db[j - gap].roll_number > temp.roll_number; j -= gap) {
				db[j] = db[j - gap];
			}
			db[j] = temp;
		}
	}
}

int main(void) {
	srand((unsigned int)time(NULL));

	Student db[MAX_STUDENTS];
	int count = 0;
	bool loaded = false;
	bool sorted = false;
	bool running = true;

	while (running) {
		printf("\n1. Input Records\n2. Randomize Records\n3. Display Records\n4. Linear Search\n5. Binary Search\n6. Insertion Sort\n7. Selection Sort\n8. Shell Sort\n9. Exit\nSelection: ");
		MenuOption selection = (MenuOption)get_int_in_range(MENU_INPUT, MENU_EXIT);

		switch (selection) {
			case MENU_INPUT:
				populate_student_db(db, &count);
				loaded = true;
				sorted = false;
				break;

			case MENU_RANDOMIZE:
				generate_random_students(db, &count);
				loaded = true;
				sorted = false;
				break;

			case MENU_DISPLAY:
				if (!loaded) {
					printf("Database is empty.\n");
				} else {
					print_student_db(db, count);
				}
				break;

			case MENU_LINEAR_SEARCH:
				if (!loaded) {
					printf("Database is empty.\n");
				} else {
					printf("Enter Roll Number to search: ");
					int target = get_int();
					int found = linear_search(db, count, target);

					if (found != -1) {
						printf("Record Found: %s (Roll No: %d) at index %d\n", db[found].name, db[found].roll_number, found);
					} else {
						printf("Record with Roll Number %d not found.\n", target);
					}
				}
				break;

			case MENU_BINARY_SEARCH:
				if (!loaded) {
					printf("Database is empty.\n");
				} else if (!sorted) {
					printf("Error: Binary search requires sorted data. Please sort the database first.\n");
				} else {
					printf("Enter Roll Number to search: ");
					int target = get_int();
					int found = binary_search(db, count, target);

					if (found != -1) {
						printf("Record Found: %s (Roll No: %d) at index %d\n", db[found].name, db[found].roll_number, found);
					} else {
						printf("Record with Roll Number %d not found.\n", target);
					}
				}
				break;

			case MENU_INSERTION_SORT:
				if (!loaded) {
					printf("Database is empty.\n");
				} else {
					insertion_sort(db, count);
					sorted = true;
					printf("Database sorted using Insertion Sort.\n");
					print_student_db(db, count);
				}
				break;

			case MENU_SELECTION_SORT:
				if (!loaded) {
					printf("Database is empty.\n");
				} else {
					selection_sort(db, count);
					sorted = true;
					printf("Database sorted using Selection Sort.\n");
					print_student_db(db, count);
				}
				break;

			case MENU_SHELL_SORT:
				if (!loaded) {
					printf("Database is empty.\n");
				} else {
					shell_sort(db, count);
					sorted = true;
					printf("Database sorted using Shell Sort.\n");
					print_student_db(db, count);
				}
				break;

			case MENU_EXIT:
				running = false;
				break;
		}
	}
	return 0;
}
