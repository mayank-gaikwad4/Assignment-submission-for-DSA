#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_DIMENSION 10

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    u32 row_count;
    u32 col_count;
    i32 elements[MAX_DIMENSION][MAX_DIMENSION];
} Matrix;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

Matrix PopulateMatrix(const char* name);
void PrintMatrix(const Matrix* matrix, const char* name);
b32 AddMatrices(const Matrix* a, const Matrix* b, Matrix* result);
b32 SubtractMatrices(const Matrix* a, const Matrix* b, Matrix* result);
b32 MultiplyMatrices(const Matrix* a, const Matrix* b, Matrix* result);
Matrix TransposeMatrix(const Matrix* source);

Matrix PopulateMatrix(const char* name) {
    Matrix result = {0};
    printf("Matrix %s Total Rows: ", name);
    result.row_count = (u32)GetIntInRange(1, MAX_DIMENSION);
    printf("Matrix %s Total Columns: ", name);
    result.col_count = (u32)GetIntInRange(1, MAX_DIMENSION);
    for (u32 row = 0; row < result.row_count; row++) {
        for (u32 col = 0; col < result.col_count; col++) {
            printf("Element [%u][%u]: ", row, col);
            result.elements[row][col] = GetInt();
        }
    }
    return result;
}

void PrintMatrix(const Matrix* matrix, const char* name) {
    printf("\n%s (%ux%u):\n", name, matrix->row_count, matrix->col_count);
    for (u32 row = 0; row < matrix->row_count; row++) {
        printf(" | ");
        for (u32 col = 0; col < matrix->col_count; col++) {
            printf("%6d ", matrix->elements[row][col]);
        }
        printf("|\n");
    }
}

b32 AddMatrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->row_count != b->row_count || a->col_count != b->col_count) {
        return false;
    }
    result->row_count = a->row_count;
    result->col_count = a->col_count;
    for (u32 row = 0; row < a->row_count; row++) {
        for (u32 col = 0; col < a->col_count; col++) {
            result->elements[row][col] = a->elements[row][col] + b->elements[row][col];
        }
    }
    return true;
}

b32 SubtractMatrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->row_count != b->row_count || a->col_count != b->col_count) {
        return false;
    }
    result->row_count = a->row_count;
    result->col_count = a->col_count;
    for (u32 row = 0; row < a->row_count; row++) {
        for (u32 col = 0; col < a->col_count; col++) {
            result->elements[row][col] = a->elements[row][col] - b->elements[row][col];
        }
    }
    return true;
}

b32 MultiplyMatrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->col_count != b->row_count) {
        return false;
    }
    result->row_count = a->row_count;
    result->col_count = b->col_count;
    for (u32 row = 0; row < a->row_count; row++) {
        for (u32 col = 0; col < b->col_count; col++) {
            result->elements[row][col] = 0;
            for (u32 k = 0; k < a->col_count; k++) {
                result->elements[row][col] += a->elements[row][k] * b->elements[k][col];
            }
        }
    }
    return true;
}

Matrix TransposeMatrix(const Matrix* source) {
    Matrix result = {0};
    result.row_count = source->col_count;
    result.col_count = source->row_count;
    for (u32 row = 0; row < source->row_count; row++) {
        for (u32 col = 0; col < source->col_count; col++) {
            result.elements[col][row] = source->elements[row][col];
        }
    }
    return result;
}

i32 main(void) {
    Matrix matrix_a = {0};
    Matrix matrix_b = {0};
    Matrix matrix_result = {0};
    b32 is_a_loaded = false;
    b32 is_b_loaded = false;
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n1. Input A\n2. Input B\n3. Print Matrices\n4. Add\n5. Subtract\n6. Multiply\n7. Transpose A\n8. Transpose B\n9. Exit\nSelection: ");
        i32 selection = GetIntInRange(1, 9);

        switch (selection) {
            case 1:
                matrix_a = PopulateMatrix("A");
                is_a_loaded = true;
                break;
            case 2:
                matrix_b = PopulateMatrix("B");
                is_b_loaded = true;
                break;
            case 3:
                if (!is_a_loaded && !is_b_loaded) {
                    printf("Matrices are empty.\n");
                } else {
                    if (is_a_loaded) PrintMatrix(&matrix_a, "Matrix A");
                    if (is_b_loaded) PrintMatrix(&matrix_b, "Matrix B");
                }
                break;
            case 4:
                if (!is_a_loaded || !is_b_loaded) {
                    printf("Missing one or both matrices.\n");
                } else if (AddMatrices(&matrix_a, &matrix_b, &matrix_result)) {
                    PrintMatrix(&matrix_result, "A + B");
                } else {
                    printf("Dimension mismatch for addition.\n");
                }
                break;
            case 5:
                if (!is_a_loaded || !is_b_loaded) {
                    printf("Missing one or both matrices.\n");
                } else if (SubtractMatrices(&matrix_a, &matrix_b, &matrix_result)) {
                    PrintMatrix(&matrix_result, "A - B");
                } else {
                    printf("Dimension mismatch for subtraction.\n");
                }
                break;
            case 6:
                if (!is_a_loaded || !is_b_loaded) {
                    printf("Missing one or both matrices.\n");
                } else if (MultiplyMatrices(&matrix_a, &matrix_b, &matrix_result)) {
                    PrintMatrix(&matrix_result, "A * B");
                } else {
                    printf("Dimension mismatch for multiplication.\n");
                }
                break;
            case 7:
                if (!is_a_loaded) {
                    printf("Matrix A is empty.\n");
                } else {
                    matrix_result = TransposeMatrix(&matrix_a);
                    PrintMatrix(&matrix_result, "Transpose of A");
                }
                break;
            case 8:
                if (!is_b_loaded) {
                    printf("Matrix B is empty.\n");
                } else {
                    matrix_result = TransposeMatrix(&matrix_b);
                    PrintMatrix(&matrix_result, "Transpose of B");
                }
                break;
            case 9:
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
