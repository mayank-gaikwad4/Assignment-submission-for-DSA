#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#define MAX_ROWS 10
#define MAX_COLS 10

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
	int rows;
	int cols;
	int data[MAX_ROWS][MAX_COLS];
} DenseMatrix;

typedef enum {
	MENU_INPUT_A = 1,
	MENU_INPUT_B,
	MENU_PRINT,
	MENU_ADD,
	MENU_SUBTRACT,
	MENU_MULTIPLY,
	MENU_TRANSPOSE_A,
	MENU_TRANSPOSE_B,
	MENU_EXIT
} MenuOption;

DenseMatrix populate_dense_matrix(const char *name) {
	DenseMatrix mat = {0};

	printf("Matrix %s Total Rows: ", name);
	mat.rows = get_int_in_range(1, MAX_ROWS);

	printf("Matrix %s Total Columns: ", name);
	mat.cols = get_int_in_range(1, MAX_COLS);

	for (int r = 0; r < mat.rows; r++) {
		for (int c = 0; c < mat.cols; c++) {
			printf("Element [%d][%d]: ", r, c);
			mat.data[r][c] = get_int();
		}
	}
	return mat;
}

void print_dense_matrix(const DenseMatrix *mat, const char *name) {
	printf("\n%s (%dx%d):\n", name, mat->rows, mat->cols);
	for (int r = 0; r < mat->rows; r++) {
		printf("  | ");
		for (int c = 0; c < mat->cols; c++) {
			printf("%6d ", mat->data[r][c]);
		}
		printf("|\n");
	}
}

bool add_dense_matrices(const DenseMatrix *a, const DenseMatrix *b, DenseMatrix *res) {
	if (a->rows != b->rows || a->cols != b->cols) {
		return false;
	}

	res->rows = a->rows;
	res->cols = a->cols;

	for (int r = 0; r < a->rows; r++) {
		for (int c = 0; c < a->cols; c++) {
			res->data[r][c] = a->data[r][c] + b->data[r][c];
		}
	}
	return true;
}

bool subtract_dense_matrices(const DenseMatrix *a, const DenseMatrix *b, DenseMatrix *res) {
	if (a->rows != b->rows || a->cols != b->cols) {
		return false;
	}

	res->rows = a->rows;
	res->cols = a->cols;

	for (int r = 0; r < a->rows; r++) {
		for (int c = 0; c < a->cols; c++) {
			res->data[r][c] = a->data[r][c] - b->data[r][c];
		}
	}
	return true;
}

bool multiply_dense_matrices(const DenseMatrix *a, const DenseMatrix *b, DenseMatrix *res) {
	if (a->cols != b->rows) {
		return false;
	}

	res->rows = a->rows;
	res->cols = b->cols;

	for (int r = 0; r < a->rows; r++) {
		for (int c = 0; c < b->cols; c++) {
			res->data[r][c] = 0;
			for (int k = 0; k < a->cols; k++) {
				res->data[r][c] += a->data[r][k] * b->data[k][c];
			}
		}
	}
	return true;
}

DenseMatrix transpose_dense_matrix(const DenseMatrix *src) {
	DenseMatrix res = {0};
	res.rows = src->cols;
	res.cols = src->rows;

	for (int r = 0; r < src->rows; r++) {
		for (int c = 0; c < src->cols; c++) {
			res.data[c][r] = src->data[r][c];
		}
	}
	return res;
}

int main(void) {
	DenseMatrix mat_a = {0};
	DenseMatrix mat_b = {0};
	DenseMatrix mat_res = {0};
	bool a_loaded = false;
	bool b_loaded = false;
	bool running = true;

	while (running) {
		printf("\n1. Input A\n2. Input B\n3. Print Matrices\n4. Add\n5. Subtract\n6. Multiply\n7. Transpose A\n8. Transpose B\n9. Exit\nSelection: ");
		MenuOption selection = (MenuOption)get_int_in_range(MENU_INPUT_A, MENU_EXIT);

		switch (selection) {
			case MENU_INPUT_A:
				mat_a = populate_dense_matrix("A");
				a_loaded = true;
				break;

			case MENU_INPUT_B:
				mat_b = populate_dense_matrix("B");
				b_loaded = true;
				break;

			case MENU_PRINT:
				if (!a_loaded && !b_loaded) {
					printf("Matrices are empty.\n");
				} else {
					if (a_loaded) print_dense_matrix(&mat_a, "Matrix A");
					if (b_loaded) print_dense_matrix(&mat_b, "Matrix B");
				}
				break;

			case MENU_ADD:
				if (!a_loaded || !b_loaded) {
					printf("Missing one or both matrices.\n");
				} else if (add_dense_matrices(&mat_a, &mat_b, &mat_res)) {
					print_dense_matrix(&mat_res, "A + B");
				} else {
					printf("Dimension mismatch for addition.\n");
				}
				break;

			case MENU_SUBTRACT:
				if (!a_loaded || !b_loaded) {
					printf("Missing one or both matrices.\n");
				} else if (subtract_dense_matrices(&mat_a, &mat_b, &mat_res)) {
					print_dense_matrix(&mat_res, "A - B");
				} else {
					printf("Dimension mismatch for subtraction.\n");
				}
				break;

			case MENU_MULTIPLY:
				if (!a_loaded || !b_loaded) {
					printf("Missing one or both matrices.\n");
				} else if (multiply_dense_matrices(&mat_a, &mat_b, &mat_res)) {
					print_dense_matrix(&mat_res, "A * B");
				} else {
					printf("Dimension mismatch for multiplication.\n");
				}
				break;

			case MENU_TRANSPOSE_A:
				if (!a_loaded) {
					printf("Matrix A is empty.\n");
				} else {
					mat_res = transpose_dense_matrix(&mat_a);
					print_dense_matrix(&mat_res, "Transpose of A");
				}
				break;

			case MENU_TRANSPOSE_B:
				if (!b_loaded) {
					printf("Matrix B is empty.\n");
				} else {
					mat_res = transpose_dense_matrix(&mat_b);
					print_dense_matrix(&mat_res, "Transpose of B");
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
