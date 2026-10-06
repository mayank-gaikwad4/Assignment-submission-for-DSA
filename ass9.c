#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_CAPACITY 10

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    i32 elements[MAX_QUEUE_CAPACITY];
    i32 front_index;
    i32 rear_index;
} JobQueue;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

void InitQueue(JobQueue* queue);
void EnqueueJob(JobQueue* queue, i32 job_id);
void DequeueJob(JobQueue* queue);
void PrintQueue(const JobQueue* queue);

void InitQueue(JobQueue* queue) {
    queue->front_index = -1;
    queue->rear_index = -1;
}

void EnqueueJob(JobQueue* queue, i32 job_id) {
    if (queue->rear_index == MAX_QUEUE_CAPACITY - 1) {
        printf("Queue Overflow\n");
        return;
    }
    if (queue->front_index == -1) {
        queue->front_index = 0;
    }
    queue->rear_index++;
    queue->elements[queue->rear_index] = job_id;
    printf("Job added successfully\n");
}

void DequeueJob(JobQueue* queue) {
    if (queue->front_index == -1 || queue->front_index > queue->rear_index) {
        printf("Queue Underflow\n");
        return;
    }
    printf("Processing Job: %d\n", queue->elements[queue->front_index]);
    queue->front_index++;
    if (queue->front_index > queue->rear_index) {
        queue->front_index = -1;
        queue->rear_index = -1;
    }
}

void PrintQueue(const JobQueue* queue) {
    if (queue->front_index == -1 || queue->front_index > queue->rear_index) {
        printf("Queue is empty\n");
        return;
    }
    printf("Current Queue: ");
    for (i32 i = queue->front_index; i <= queue->rear_index; i++) {
        printf("%d ", queue->elements[i]);
    }
    printf("\n");
}

i32 main(void) {
    JobQueue active_queue;
    InitQueue(&active_queue);
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n--- Job Queue Simulation ---\n");
        printf("1. Add Job\n");
        printf("2. Delete/Process Job\n");
        printf("3. Display Jobs\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        i32 selection = GetIntInRange(1, 4);

        switch (selection) {
            case 1:
                printf("Enter Job ID: ");
                i32 job_id = GetInt();
                EnqueueJob(&active_queue, job_id);
                break;
            case 2:
                DequeueJob(&active_queue);
                break;
            case 3:
                PrintQueue(&active_queue);
                break;
            case 4:
                printf("Exiting system.\n");
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
