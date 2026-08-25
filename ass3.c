#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50

#define c_str_to_long strtol
#define c_str_to_double strtod
#define c_is_space isspace

typedef int32_t i32;
typedef double f64;

i32 get_int(void);
f64 get_double(void);
char* get_string(void);
i32 get_int_in_range(i32 min_val, i32 max_val);

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

void populate_student_db(Student db[], int *count) {
	printf("Enter total number of students (1-%d): ", MAX_STUDENTS);
	*count = get_int_in_range(1, MAX_STUDENTS);

	for (int i = 0; i < *count; i++) {
		printf("\nStudent %d Roll Number: ", i + 1);
		db[i].roll_number = get_int_in_range(1, 99999);

		printf("Student %d Name: ", i + 1);
		char* temp_name = get_string();
		strncpy(db[i].name, temp_name, MAX_NAME_LENGTH - 1);
		db[i].name[MAX_NAME_LENGTH - 1] = '\0';
		free(temp_name);
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































// INPUT SANITIZATION
// IGNORE
i32 get_int(void) {
	i32 result = 0;
	int valid = 0;
	char buf[1024];

	while (!valid) {
		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			printf("\nError: No more input available (end of input reached). Exiting.\n");
			exit(1);
		}

		size_t len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}

		if (buf[0] == '\0') {
			printf("Error: Input is completely empty. Please enter a number.\nRetry: ");
		} else {
			char* endptr = NULL;
			errno = 0;
			long parsed = c_str_to_long(buf, &endptr, 10);

			if (endptr == buf) {
				const char* p = buf;
				while (*p != '\0' && c_is_space((unsigned char)*p)) p++;

				if (*p == '\0') {
					printf("Error: Input contains only whitespace. Please enter a number.\nRetry: ");
				} else {
					printf("Error: Found invalid character '%c'. Expected a number.\nRetry: ", *p);
				}
			} else if (errno == ERANGE || parsed < INT32_MIN || parsed > INT32_MAX) {
				printf("Error: Value is out of range for a 32-bit integer (Overflow/Underflow).\nRetry: ");
			} else {
				while (*endptr != '\0' && c_is_space((unsigned char)*endptr)) {
					endptr++;
				}

				if (*endptr != '\0') {
					printf("Error: Found invalid trailing character '%c'.\nRetry: ", *endptr);
				} else {
					result = (i32)parsed;
					valid = 1;
				}
			}
		}
	}

	return result;
}

f64 get_double(void) {
	f64 result = 0.0;
	int valid = 0;
	char buf[1024];

	while (!valid) {
		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			printf("\nError: No more input available (end of input reached). Exiting.\n");
			exit(1);
		}

		size_t len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}

		if (buf[0] == '\0') {
			printf("Error: Input is completely empty. Please enter a number.\nRetry: ");
		} else {
			char* endptr = NULL;
			errno = 0;
			f64 parsed = c_str_to_double(buf, &endptr);

			if (endptr == buf) {
				const char* p = buf;
				while (*p != '\0' && c_is_space((unsigned char)*p)) p++;

				if (*p == '\0') {
					printf("Error: Input contains only whitespace. Please enter a decimal.\nRetry: ");
				} else {
					printf("Error: Found invalid character '%c'. Expected a decimal.\nRetry: ", *p);
				}
			} else if (errno == ERANGE) {
				printf("Error: Value is out of range for a 64-bit float (Overflow/Underflow).\nRetry: ");
			} else {
				while (*endptr != '\0' && c_is_space((unsigned char)*endptr)) {
					endptr++;
				}

				if (*endptr != '\0') {
					printf("Error: Found invalid trailing character '%c'.\nRetry: ", *endptr);
				} else {
					result = parsed;
					valid = 1;
				}
			}
		}
	}

	return result;
}

char* get_string(void) {
	char* result = NULL;
	int valid = 0;
	char buf[1024];

	while (!valid) {
		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			printf("\nError: No more input available (end of input reached). Exiting.\n");
			exit(1);
		}

		size_t len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}

		const char* p = buf;
		while (*p != '\0' && c_is_space((unsigned char)*p)) p++;

		if (*p == '\0') {
			printf("Error: Input cannot be empty or consist only of whitespace.\nRetry: ");
		} else {
			result = strdup(buf);
			valid = 1;
		}
	}

	return result;
}

i32 get_int_in_range(i32 min_val, i32 max_val) {
	i32 val = 0;
	int valid = 0;

	while (!valid) {
		val = get_int();
		if (val >= min_val && val <= max_val) {
			valid = 1;
		} else {
			printf("Error: Value %d is out of bounds (%d to %d).\nRetry: ", val, min_val, max_val);
		}
	}

	return val;
}
