#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_EXPRESSION_LENGTH 256

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    char elements[MAX_EXPRESSION_LENGTH];
    i32 top_index;
} CharStack;

typedef struct {
    char elements[MAX_EXPRESSION_LENGTH][MAX_EXPRESSION_LENGTH];
    i32 top_index;
} StringStack;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

void InitCharStack(CharStack* stack);
b32 IsEmptyCharStack(const CharStack* stack);
void PushChar(CharStack* stack, char value);
char PopChar(CharStack* stack);
char PeekChar(const CharStack* stack);

void InitStringStack(StringStack* stack);
b32 IsEmptyStringStack(const StringStack* stack);
void PushString(StringStack* stack, const char* value);
void PopString(StringStack* stack, char* output);

b32 IsOperator(char c);
i32 GetPrecedence(char c);
void InfixToPostfix(const char* infix, char* postfix);
void PrefixToInfix(const char* prefix, char* infix);
void PostfixToPrefix(const char* postfix, char* prefix);

void InitCharStack(CharStack* stack) {
    stack->top_index = -1;
}

b32 IsEmptyCharStack(const CharStack* stack) {
    return stack->top_index == -1;
}

void PushChar(CharStack* stack, char value) {
    if (stack->top_index < MAX_EXPRESSION_LENGTH - 1) {
        stack->elements[++(stack->top_index)] = value;
    }
}

char PopChar(CharStack* stack) {
    if (!IsEmptyCharStack(stack)) {
        return stack->elements[(stack->top_index)--];
    }
    return '\0';
}

char PeekChar(const CharStack* stack) {
    if (!IsEmptyCharStack(stack)) {
        return stack->elements[stack->top_index];
    }
    return '\0';
}

void InitStringStack(StringStack* stack) {
    stack->top_index = -1;
}

b32 IsEmptyStringStack(const StringStack* stack) {
    return stack->top_index == -1;
}

void PushString(StringStack* stack, const char* value) {
    if (stack->top_index < MAX_EXPRESSION_LENGTH - 1) {
        stack->top_index++;
        strncpy(stack->elements[stack->top_index], value, MAX_EXPRESSION_LENGTH - 1);
        stack->elements[stack->top_index][MAX_EXPRESSION_LENGTH - 1] = '\0';
    }
}

void PopString(StringStack* stack, char* output) {
    if (!IsEmptyStringStack(stack)) {
        strcpy(output, stack->elements[stack->top_index]);
        stack->top_index--;
    } else {
        output[0] = '\0';
    }
}

b32 IsOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

i32 GetPrecedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

void InfixToPostfix(const char* infix, char* postfix) {
    CharStack stack;
    InitCharStack(&stack);
    i32 i = 0, j = 0;
    code Code

    while (infix[i] != '\0') {
        if (isalnum((u8)infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            PushChar(&stack, infix[i]);
        } else if (infix[i] == ')') {
            while (!IsEmptyCharStack(&stack) && PeekChar(&stack) != '(') {
                postfix[j++] = PopChar(&stack);
            }
            PopChar(&stack);
        } else if (IsOperator(infix[i])) {
            while (!IsEmptyCharStack(&stack) && GetPrecedence(PeekChar(&stack)) >= GetPrecedence(infix[i])) {
                postfix[j++] = PopChar(&stack);
            }
            PushChar(&stack, infix[i]);
        }
        i++;
    }
    while (!IsEmptyCharStack(&stack)) {
        postfix[j++] = PopChar(&stack);
    }
    postfix[j] = '\0';

}

void PrefixToInfix(const char* prefix, char* infix) {
    StringStack stack;
    InitStringStack(&stack);
    i32 length = (i32)strlen(prefix);
    code Code

    for (i32 i = length - 1; i >= 0; i--) {
        if (isalnum((u8)prefix[i])) {
            char temp[2] = {prefix[i], '\0'};
            PushString(&stack, temp);
        } else if (IsOperator(prefix[i])) {
            char operand1[MAX_EXPRESSION_LENGTH], operand2[MAX_EXPRESSION_LENGTH], temp[MAX_EXPRESSION_LENGTH];
            PopString(&stack, operand1);
            PopString(&stack, operand2);
            snprintf(temp, sizeof(temp), "(%s%c%s)", operand1, prefix[i], operand2);
            PushString(&stack, temp);
        }
    }
    PopString(&stack, infix);

}

void PostfixToPrefix(const char* postfix, char* prefix) {
    StringStack stack;
    InitStringStack(&stack);
    i32 i = 0;
    code Code

    while (postfix[i] != '\0') {
        if (isalnum((u8)postfix[i])) {
            char temp[2] = {postfix[i], '\0'};
            PushString(&stack, temp);
        } else if (IsOperator(postfix[i])) {
            char operand1[MAX_EXPRESSION_LENGTH], operand2[MAX_EXPRESSION_LENGTH], temp[MAX_EXPRESSION_LENGTH];
            PopString(&stack, operand2);
            PopString(&stack, operand1);
            snprintf(temp, sizeof(temp), "%c%s%s", postfix[i], operand1, operand2);
            PushString(&stack, temp);
        }
        i++;
    }
    PopString(&stack, prefix);

}

i32 main(void) {
    b32 is_running = true;
    code Code

    printf("=================================================\n");
    printf("   Expression Conversion using Stack ADT\n");
    printf("=================================================\n");

    while (is_running) {
        printf("\nMenu:\n");
        printf("1. Infix to Postfix\n");
        printf("2. Prefix to Infix\n");
        printf("3. Postfix to Prefix\n");
        printf("4. Exit\n");
        printf("Selection: ");

        i32 selection = GetIntInRange(1, 4);

        switch (selection) {
            case 1: {
                printf("Enter Infix Expression (e.g. A+B*C): ");
                char* infix = GetString();
                char postfix[MAX_EXPRESSION_LENGTH] = {0};
                InfixToPostfix(infix, postfix);
                printf("-> Postfix Expression: %s\n", postfix);
                free(infix);
                break;
            }
            case 2: {
                printf("Enter Prefix Expression (e.g. +A*BC): ");
                char* prefix = GetString();
                char infix[MAX_EXPRESSION_LENGTH] = {0};
                PrefixToInfix(prefix, infix);
                printf("-> Infix Expression: %s\n", infix);
                free(prefix);
                break;
            }
            case 3: {
                printf("Enter Postfix Expression (e.g. ABC*+): ");
                char* postfix = GetString();
                char prefix[MAX_EXPRESSION_LENGTH] = {0};
                PostfixToPrefix(postfix, prefix);
                printf("-> Prefix Expression: %s\n", prefix);
                free(postfix);
                break;
            }
            case 4:
                printf("Exiting program...\n");
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
