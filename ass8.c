#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#define MAX_STACK_CAPACITY 256

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    i32 elements[MAX_STACK_CAPACITY];
    i32 top_index;
} ValueStack;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

void InitValueStack(ValueStack* stack);
b32 PushValue(ValueStack* stack, i32 value);
b32 PopValue(ValueStack* stack, i32* output);
b32 EvaluatePostfix(const char* expression, i32* result);

void InitValueStack(ValueStack* stack) {
    stack->top_index = -1;
}

b32 PushValue(ValueStack* stack, i32 value) {
    if (stack->top_index >= MAX_STACK_CAPACITY - 1) return false;
    stack->elements[++(stack->top_index)] = value;
    return true;
}

b32 PopValue(ValueStack* stack, i32* output) {
    if (stack->top_index < 0) return false;
    *output = stack->elements[(stack->top_index)--];
    return true;
}

b32 EvaluatePostfix(const char* expression, i32* result) {
    ValueStack stack;
    InitValueStack(&stack);
    u32 i = 0;
    code Code

    while (expression[i] != '\0') {
        if (isspace((u8)expression[i])) {
            i++;
            continue;
        }
        if (isdigit((u8)expression[i])) {
            i32 number = 0;
            while (isdigit((u8)expression[i])) {
                number = number * 10 + (expression[i] - '0');
                i++;
            }
            if (!PushValue(&stack, number)) return false;
            continue;
        }
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            i32 operand1, operand2;
            if (!PopValue(&stack, &operand2) || !PopValue(&stack, &operand1)) return false;
            i32 operation_result = 0;
            switch (expression[i]) {
                case '+': operation_result = operand1 + operand2; break;
                case '-': operation_result = operand1 - operand2; break;
                case '*': operation_result = operand1 * operand2; break;
                case '/':
                    if (operand2 == 0) return false;
                    operation_result = operand1 / operand2;
                break;
            }
            if (!PushValue(&stack, operation_result)) return false;
        }
        i++;
    }
    if (stack.top_index == 0) {
        return PopValue(&stack, result);
    }
    return false;

}

i32 main(void) {
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n1. Evaluate Postfix Expression\n2. Exit\nSelection: ");
        i32 selection = GetIntInRange(1, 2);

        switch (selection) {
            case 1: {
                printf("Enter postfix expression (operands and operators separated by space): ");
                char* expression = GetString();
                i32 evaluation_result = 0;
                if (EvaluatePostfix(expression, &evaluation_result)) {
                    printf("Result: %d\n", evaluation_result);
                } else {
                    printf("Error: Invalid postfix expression or division by zero.\n");
                }
                free(expression);
                break;
            }
            case 2:
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
