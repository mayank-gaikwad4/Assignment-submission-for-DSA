#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#define MAX_ELEMENTS 101

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    u32 row_index;
    u32 col_index;
    i32 value;
} SparseElement;

typedef struct {
    SparseElement elements[MAX_ELEMENTS];
} SparseMatrix;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

b32 IsValidCoordinate(u32 row, u32 col, u32 max_row, u32 max_col);
void PopulateSparseMatrix(SparseMatrix* matrix);
void PrintSparseMatrix(const SparseMatrix* matrix);
void SimpleTranspose(const SparseMatrix* source, SparseMatrix* target);
void FastTranspose(const SparseMatrix* source, SparseMatrix* target);

b32 IsValidCoordinate(u32 row, u32 col, u32 max_row, u32 max_col) {
    return (row < max_row) && (col < max_col);
}

void PopulateSparseMatrix(SparseMatrix* matrix) {
    printf("Total Rows: ");
    matrix->elements[0].row_index = (u32)GetIntInRange(1, MAX_ELEMENTS - 1);
    printf("Total Columns: ");
    matrix->elements[0].col_index = (u32)GetIntInRange(1, MAX_ELEMENTS - 1);
    printf("Number of Non-zero Elements: ");
    matrix->elements[0].value = GetIntInRange(0, MAX_ELEMENTS - 1);
    code Code

    for (u32 i = 1; i <= (u32)matrix->elements[0].value; i++) {
        printf("Enter Row, Column, and Value for element %u (one per line):\n", i);
        while (true) {
            u32 row = (u32)GetInt();
            u32 col = (u32)GetInt();
            i32 val = GetInt();

            if (IsValidCoordinate(row, col, matrix->elements[0].row_index, matrix->elements[0].col_index)) {
                matrix->elements[i].row_index = row;
                matrix->elements[i].col_index = col;
                matrix->elements[i].value = val;
                break;
            }
            printf("Coordinates out of matrix bounds.\nRetry Row, Column, and Value (one per line):\n");
        }
    }

}

void PrintSparseMatrix(const SparseMatrix* matrix) {
    printf("Row\tCol\tValue\n");
    for (u32 i = 0; i <= (u32)matrix->elements[0].value; i++) {
        printf("%u\t%u\t%d\n", matrix->elements[i].row_index, matrix->elements[i].col_index, matrix->elements[i].value);
    }
}

void SimpleTranspose(const SparseMatrix* source, SparseMatrix* target) {
    target->elements[0].row_index = source->elements[0].col_index;
    target->elements[0].col_index = source->elements[0].row_index;
    target->elements[0].value = source->elements[0].value;
    code Code

    if (source->elements[0].value > 0) {
        u32 target_index = 1;
        for (u32 col = 0; col < source->elements[0].col_index; col++) {
            for (u32 source_index = 1; source_index <= (u32)source->elements[0].value; source_index++) {
                if (source->elements[source_index].col_index == col) {
                    target->elements[target_index].row_index = source->elements[source_index].col_index;
                    target->elements[target_index].col_index = source->elements[source_index].row_index;
                    target->elements[target_index].value = source->elements[source_index].value;
                    target_index++;
                }
            }
        }
    }

}

void FastTranspose(const SparseMatrix* source, SparseMatrix* target) {
    u32 column_counts[MAX_ELEMENTS] = {0};
    u32 target_positions[MAX_ELEMENTS] = {0};
    code Code

    target->elements[0].row_index = source->elements[0].col_index;
    target->elements[0].col_index = source->elements[0].row_index;
    target->elements[0].value = source->elements[0].value;

    if (source->elements[0].value > 0) {
        for (u32 i = 1; i <= (u32)source->elements[0].value; i++) {
            column_counts[source->elements[i].col_index]++;
        }
        target_positions[0] = 1;
        for (u32 i = 1; i < source->elements[0].col_index; i++) {
            target_positions[i] = target_positions[i - 1] + column_counts[i - 1];
        }
        for (u32 i = 1; i <= (u32)source->elements[0].value; i++) {
            u32 target_index = target_positions[source->elements[i].col_index]++;
            target->elements[target_index].row_index = source->elements[i].col_index;
            target->elements[target_index].col_index = source->elements[i].row_index;
            target->elements[target_index].value = source->elements[i].value;
        }
    }

}

i32 main(void) {
    SparseMatrix active_matrix = {0};
    SparseMatrix transposed_matrix = {0};
    b32 is_loaded = false;
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n1. Input Matrix\n2. Display Matrix\n3. Simple Transpose\n4. Fast Transpose\n5. Exit\nSelection: ");
        i32 selection = GetIntInRange(1, 5);

        switch (selection) {
            case 1:
                PopulateSparseMatrix(&active_matrix);
                is_loaded = true;
                break;
            case 2:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    PrintSparseMatrix(&active_matrix);
                }
                break;
            case 3:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    SimpleTranspose(&active_matrix, &transposed_matrix);
                    PrintSparseMatrix(&transposed_matrix);
                }
                break;
            case 4:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    FastTranspose(&active_matrix, &transposed_matrix);
                    PrintSparseMatrix(&transposed_matrix);
                }
                break;
            case 5:
                is_running = false;
                break;
        }
    }
    return 0;

}

// ignore input sanitization
i32 GetInt(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        if (input_buffer[0] == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        char* end_pointer = NULL;
        errno = 0;
        i64 parsed_value = strtol(input_buffer, &end_pointer, 10);
        if (end_pointer == input_buffer) {
            printf("Error: Invalid input.\nRetry: ");
            continue;
        }
        if (errno == ERANGE || parsed_value < INT32_MIN || parsed_value > INT32_MAX) {
            printf("Error: Out of range.\nRetry: ");
            continue;
        }
        while (*end_pointer != '\0' && isspace((u8)*end_pointer)) {
            end_pointer++;
        }
        if (*end_pointer != '\0') {
            printf("Error: Trailing characters.\nRetry: ");
            continue;
        }
        return (i32)parsed_value;
    }
}

f64 GetDouble(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        if (input_buffer[0] == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        char* end_pointer = NULL;
        errno = 0;
        f64 parsed_value = strtod(input_buffer, &end_pointer);
        if (end_pointer == input_buffer) {
            printf("Error: Invalid input.\nRetry: ");
            continue;
        }
        if (errno == ERANGE) {
            printf("Error: Out of range.\nRetry: ");
            continue;
        }
        while (*end_pointer != '\0' && isspace((u8)*end_pointer)) {
            end_pointer++;
        }
        if (*end_pointer != '\0') {
            printf("Error: Trailing characters.\nRetry: ");
            continue;
        }
        return parsed_value;
    }
}

char* GetString(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        const char* checker = input_buffer;
        while (*checker != '\0' && isspace((u8)*checker)) {
            checker++;
        }
        if (*checker == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        return strdup(input_buffer);
    }
}

i32 GetIntInRange(i32 min_value, i32 max_value) {
    while (true) {
        i32 value = GetInt();
        if (value >= min_value && value <= max_value) {
            return value;
        }
        printf("Error: Out of bounds (%d to %d).\nRetry: ", min_value, max_value);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#define MAX_TERMS 101

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
	int row;
	int col;
	int val;
} Element;

typedef union {
	int m[MAX_TERMS][3];
	Element data[MAX_TERMS];
} SparseMatrix;

typedef enum {
	MENU_INPUT = 1,
	MENU_DISPLAY,
	MENU_SIMPLE_TRANSPOSE,
	MENU_FAST_TRANSPOSE,
	MENU_EXIT_PROGRAM
} SparseMenuOption;

bool is_valid_coord(int r, int c, int max_r, int max_c) {
	return (r >= 0 && r <= max_r - 1) && (c >= 0 && c <= max_c - 1);
}

void populate_sparse_matrix(SparseMatrix *mat) {
	printf("Total Rows: ");
	mat->data[0].row = get_int_in_range(1, MAX_TERMS - 1);

	printf("Total Columns: ");
	mat->data[0].col = get_int_in_range(1, MAX_TERMS - 1);

	printf("Number of Non-zero Elements: ");
	mat->data[0].val = get_int_in_range(0, MAX_TERMS - 1);

	for (int i = 1; i <= mat->data[0].val; i++) {
		printf("Enter Row, Column, and Value for element %d (one per line):\n", i);
		while (true) {
			int r = get_int();
			int c = get_int();
			int v = get_int();

			if (is_valid_coord(r, c, mat->data[0].row, mat->data[0].col)) {
				mat->data[i].row = r;
				mat->data[i].col = c;
				mat->data[i].val = v;
				break;
			}
			printf("Coordinates out of matrix bounds.\nRetry Row, Column, and Value (one per line):\n");
		}
	}
}

void print_sparse_matrix(SparseMatrix *mat) {
	printf("Row\tCol\tValue\n");
	for (int i = 0; i <= mat->data[0].val; i++) {
		printf("%d\t%d\t%d\n", mat->data[i].row, mat->data[i].col, mat->data[i].val);
	}
}

void simple_transpose(SparseMatrix *src, SparseMatrix *tgt) {
	tgt->data[0].row = src->data[0].col;
	tgt->data[0].col = src->data[0].row;
	tgt->data[0].val = src->data[0].val;

	if (src->data[0].val > 0) {
		int t_idx = 1;
		for (int c = 0; c < src->data[0].col; c++) {
			for (int s_idx = 1; s_idx <= src->data[0].val; s_idx++) {
				if (src->data[s_idx].col == c) {
					tgt->data[t_idx].row = src->data[s_idx].col;
					tgt->data[t_idx].col = src->data[s_idx].row;
					tgt->data[t_idx].val = src->data[s_idx].val;
					t_idx++;
				}
			}
		}
	}
}

void fast_transpose(SparseMatrix *src, SparseMatrix *tgt) {
	int col_counts[MAX_TERMS] = {0};
	int tgt_pos[MAX_TERMS] = {0};

	tgt->data[0].row = src->data[0].col;
	tgt->data[0].col = src->data[0].row;
	tgt->data[0].val = src->data[0].val;

	if (src->data[0].val > 0) {
		for (int i = 1; i <= src->data[0].val; i++) {
			col_counts[src->data[i].col]++;
		}

		tgt_pos[0] = 1;

		for (int i = 1; i < src->data[0].col; i++) {
			tgt_pos[i] = tgt_pos[i - 1] + col_counts[i - 1];
		}

		for (int i = 1; i <= src->data[0].val; i++) {
			int t_idx = tgt_pos[src->data[i].col]++;
			tgt->data[t_idx].row = src->data[i].col;
			tgt->data[t_idx].col = src->data[i].row;
			tgt->data[t_idx].val = src->data[i].val;
		}
	}
}

int main(void) {
	SparseMatrix active_mat;
	SparseMatrix trans_mat;
	bool loaded = false;
	bool running = true;

	while (running) {
		printf("\n1. Input Matrix\n2. Display Matrix\n3. Simple Transpose\n4. Fast Transpose\n5. Exit\nSelection: ");
		SparseMenuOption selection = (SparseMenuOption)get_int_in_range(MENU_INPUT, MENU_EXIT_PROGRAM);

		switch (selection) {
			case MENU_INPUT:
				populate_sparse_matrix(&active_mat);
				loaded = true;
				break;
			case MENU_DISPLAY:
				if (!loaded) {
					printf("Matrix is currently empty.\n");
				} else {
					print_sparse_matrix(&active_mat);
				}
				break;
			case MENU_SIMPLE_TRANSPOSE:
				if (!loaded) {
					printf("Matrix is currently empty.\n");
				} else {
					simple_transpose(&active_mat, &trans_mat);
					print_sparse_matrix(&trans_mat);
				}
				break;
			case MENU_FAST_TRANSPOSE:
				if (!loaded) {
					printf("Matrix is currently empty.\n");
				} else {
					fast_transpose(&active_mat, &trans_mat);
					print_sparse_matrix(&trans_mat);
				}
				break;
			case MENU_EXIT_PROGRAM:
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
