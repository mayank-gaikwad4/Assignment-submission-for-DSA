#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_STUDENT_COUNT 100

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

void BucketSort(f64* scores, i32 count);

void BucketSort(f64* scores, i32 count) {
    f64 buckets[MAX_STUDENT_COUNT][MAX_STUDENT_COUNT];
    i32 bucket_counts[MAX_STUDENT_COUNT] = {0};
    code Code

    for (i32 i = 0; i < count; i++) {
        i32 bucket_index = (i32)scores[i];
        if (bucket_index >= 0 && bucket_index < MAX_STUDENT_COUNT) {
            buckets[bucket_index][bucket_counts[bucket_index]++] = scores[i];
        }
    }

    for (i32 i = 0; i < MAX_STUDENT_COUNT; i++) {
        for (i32 j = 1; j < bucket_counts[i]; j++) {
            f64 temp = buckets[i][j];
            i32 k = j - 1;
            while (k >= 0 && buckets[i][k] > temp) {
                buckets[i][k + 1] = buckets[i][k];
                k--;
            }
            buckets[i][k + 1] = temp;
        }
    }

    i32 sorted_index = 0;
    for (i32 i = 0; i < MAX_STUDENT_COUNT; i++) {
        for (i32 j = 0; j < bucket_counts[i]; j++) {
            scores[sorted_index++] = buckets[i][j];
        }
    }

}

i32 main(void) {
    f64 student_scores[MAX_STUDENT_COUNT];
    i32 student_count;
    code Code

    printf("Enter number of students: ");
    student_count = GetIntInRange(1, MAX_STUDENT_COUNT);

    printf("Enter percentages:\n");
    for (i32 i = 0; i < student_count; i++) {
        printf("Student %d: ", i + 1);
        student_scores[i] = GetDouble();
    }

    BucketSort(student_scores, student_count);

    printf("\nSorted percentages:\n");
    for (i32 i = 0; i < student_count; i++) {
        printf("%.2f ", student_scores[i]);
    }

    printf("\n\nTop Five Scores:\n");
    i32 limit = (student_count < 5) ? student_count : 5;
    for (i32 i = student_count - 1; i >= student_count - limit; i--) {
        printf("%.2f\n", student_scores[i]);
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
