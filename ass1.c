#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_ROWS 10
#define MAX_COLS 10

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
