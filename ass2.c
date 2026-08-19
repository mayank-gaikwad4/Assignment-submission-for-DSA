#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TERMS 101

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

bool is_valid_coord(int r, int c, int max_r, int max_c) {
	return is_in_range(r, 0, max_r - 1) && is_in_range(c, 0, max_c - 1);
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

bool parse_three_ints(const char *buf, int *v1, int *v2, int *v3) {
	if (!is_valid_input(buf)) {
		return false;
	}
	char trail;
	return sscanf(buf, "%d %d %d %c", v1, v2, v3, &trail) == 3;
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

void get_three_ints(int *v1, int *v2, int *v3) {
	char buf[256];
	while (true) {
		if (read_line(buf, sizeof(buf)) && parse_three_ints(buf, v1, v2, v3)) {
			return;
		}
		printf("Requires exactly three integers.\nRetry: ");
	}
}

void populate_sparse_matrix(SparseMatrix *mat) {
	printf("Total Rows: ");
	mat->data[0].row = get_int_in_range(1, MAX_TERMS - 1);

	printf("Total Columns: ");
	mat->data[0].col = get_int_in_range(1, MAX_TERMS - 1);

	printf("Number of Non-zero Elements: ");
	mat->data[0].val = get_int_in_range(0, MAX_TERMS - 1);

	for (int i = 1; i <= mat->data[0].val; i++) {
		printf("Enter Row, Column, and Value for element %d: ", i);
		while (true) {
			int r, c, v;
			get_three_ints(&r, &c, &v);

			if (is_valid_coord(r, c, mat->data[0].row, mat->data[0].col)) {
				mat->data[i].row = r;
				mat->data[i].col = c;
				mat->data[i].val = v;
				break;
			}
			printf("Coordinates out of matrix bounds.\nRetry Row, Column, and Value: ");
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
