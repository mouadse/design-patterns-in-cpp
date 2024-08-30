#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 64

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

void add_token(Token tokens[], int *token_count, TokenType type, const char *value) {
    if (*token_count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens\n");
        exit(EXIT_FAILURE);
    }
    tokens[*token_count].type = type;
    strncpy(tokens[*token_count].value, value, MAX_TOKEN_LENGTH - 1);
    tokens[*token_count].value[MAX_TOKEN_LENGTH - 1] = '\0';
    (*token_count)++;
}

void lex(const char *input, Token tokens[], int *token_count) {
    const char *ptr = input;
    char buffer[MAX_TOKEN_LENGTH];
    int buffer_index = 0;

    while (*ptr != '\0') {
        if (isspace(*ptr)) {
            ptr++;
            continue;
        }

        if (*ptr == '|') {
            add_token(tokens, token_count, TOKEN_PIPE, "|");
            ptr++;
        } else if (*ptr == '<') {
            add_token(tokens, token_count, TOKEN_REDIRECT_IN, "<");
            ptr++;
        } else if (*ptr == '>') {
            if (*(ptr + 1) == '>') {
                add_token(tokens, token_count, TOKEN_REDIRECT_APPEND, ">>");
                ptr += 2;
            } else {
                add_token(tokens, token_count, TOKEN_REDIRECT_OUT, ">");
                ptr++;
            }
        } else {
            buffer_index = 0;
            while (*ptr != '\0' && !isspace(*ptr) && *ptr != '|' && *ptr != '<' && *ptr != '>') {
                buffer[buffer_index++] = *ptr++;
                if (buffer_index >= MAX_TOKEN_LENGTH - 1) {
                    fprintf(stderr, "Error: Token too long\n");
                    exit(EXIT_FAILURE);
                }
            }
            buffer[buffer_index] = '\0';
            add_token(tokens, token_count, TOKEN_WORD, buffer);
        }
    }
    add_token(tokens, token_count, TOKEN_END, "");
}

int main() {
    const char *input = "echo hello | grep world > output.txt";
    Token tokens[MAX_TOKENS];
    int token_count = 0;

    lex(input, tokens, &token_count);

    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type %d, Value '%s'\n", i, tokens[i].type, tokens[i].value);
    }

    return 0;
}