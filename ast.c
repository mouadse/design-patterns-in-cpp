#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

// code the lexer or tokenizer here
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


//
typedef struct s_ast_node
{
    char *value;
    struct s_ast_node *left;
    struct s_ast_node *right;
} t_ast_node;

t_ast_node *create_node(char *value)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_ast(t_ast_node *root)
{
    if (!root)
        return;
    free_ast(root->left);
    free_ast(root->right);
    free(root->value);
    free(root);
}

typedef t_ast_node *(*nud_fn)(Token **tokens);
typedef t_ast_node *(*led_fn)(t_ast_node *left, Token **tokens);

t_ast_node *nud_word(Token **tokens)
{
    t_ast_node *node = create_node((*tokens)->value);
    (*tokens)++;
    return node;
}

t_ast_node *parse_expression(Token **tokens, int rbp);

t_ast_node *led_pipe(t_ast_node *left, Token **tokens)
{
    t_ast_node *node = create_node("|");
    (*tokens)++;
    node->left = left;
    node->right = parse_expression(tokens, 1);
    return node;
}

t_ast_node *parse_expression(Token **tokens, int rbp);

t_ast_node *nud(Token **tokens)
{
    switch ((*tokens)->type)
    {
    case TOKEN_WORD:
        return nud_word(tokens);
    default:
        fprintf(stderr, "Unexpected token: %s\n", (*tokens)->value);
        exit(EXIT_FAILURE);
    }
}

t_ast_node *led(t_ast_node *left, Token **tokens)
{
    switch ((*tokens)->type)
    {
    case TOKEN_PIPE:
        return led_pipe(left, tokens);
    default:
        fprintf(stderr, "Unexpected token: %s\n", (*tokens)->value);
        exit(EXIT_FAILURE);
    }
}

int lbp(TokenType type)
{
    switch (type)
    {
    case TOKEN_PIPE:
        return 1;
    default:
        return 0;
    }
}

t_ast_node *parse_expression(Token **tokens, int rbp)
{
    t_ast_node *left = nud(tokens);
    while (rbp < lbp((*tokens)->type))
    {
        left = led(left, tokens);
    }
    return left;
}

t_ast_node *parse_input(char *input)
{
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    lex(input, tokens, &token_count);
    Token *token_ptr = tokens;
    return parse_expression(&token_ptr, 0);
}

void print_ast(t_ast_node *root, int depth)
{
    if (!root)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", root->value);
    print_ast(root->left, depth + 1);
    print_ast(root->right, depth + 1);
}


void execute_command(t_ast_node *root)
{
    if (!root)
        return;

    if (strcmp(root->value, "|") == 0)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid1 = fork();
        if (pid1 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid1 == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execute_command(root->left);
            exit(EXIT_SUCCESS);
        }

        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execute_command(root->right);
            exit(EXIT_SUCCESS);
        }

        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
    else
    {
        char *args[MAX_TOKENS];
        int i = 0;
        t_ast_node *current = root;
        while (current)
        {
            args[i++] = current->value;
            current = current->right;
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return EXIT_FAILURE;
    }

    t_ast_node *root = parse_input(argv[1]);
    
    print_ast(root, 0);
    
    
    
    
    execute_command(root);
    free_ast(root);

    return EXIT_SUCCESS;
}