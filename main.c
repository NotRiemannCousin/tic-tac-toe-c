#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define ACCEPTED_MOVEMENT 200
#define PLAYER_WIN 201
#define DRAW 202
#define OVERWRITE_ATTEMPT 203
#define INCORRECT_INPUT 204

char grid_board[3][3];

int player, game_movement_count, game_status;
bool is_game_ended;
char try_again;
const char player_icon[2] = {'X', 'O'};

const char *COLOR_RED = "\x1b[31m";
const char *COLOR_GREEN = "\x1b[32m";
const char *COLOR_RESET = "\x1b[0m";
const char *COLOR_GRAY = "\x1b[90m";

void clear();
void change_color(const char *color);
void initial_setup();
bool check_position(int a, int b, int c);
const char *get_player_icon_color(char pos);
void print_board();
int validate_input(int x, int y, int count);
void finish();

int main(void)
{
    int y_input, x_input, input_count;

    clear();
    change_color(COLOR_GREEN);
    printf("|------|TIC-TAC-TOE|------|\n");
    printf("Bem-vindo ao jogo da velha!\n\n\n");

    change_color(COLOR_RESET);

    for (;;)
    {
        initial_setup();
        print_board();

        while (!is_game_ended)
        {
            change_color(COLOR_GRAY);

            printf("\n Jogador %d (%c, movimento %d)\n",
                   player + 1, player_icon[player], game_movement_count);
            printf("\n Insira as coordenadas (horizontal e vertical): \n");

            input_count = scanf("%d%d", &y_input, &x_input);
            y_input--;
            x_input--;

            clear();

            game_status = validate_input(x_input, y_input, input_count);
            print_board();

            change_color(COLOR_RED);

            switch (game_status)
            {
            case ACCEPTED_MOVEMENT:
                game_movement_count++;
                player = (player + 1) % 2;
                break;
            case PLAYER_WIN:
                is_game_ended = true;
                change_color(COLOR_RED);
                printf("\n O jogador %d (%c) foi o vencedor da partida em %d movimentos!\n", player + 1, player_icon[player], game_movement_count);
                break;
            case DRAW:
                is_game_ended = true;
                printf("\n Deu velha!\n");
                break;
            case INCORRECT_INPUT:
                while (getchar() != '\n')
                    ;
                printf("\n Coordenadas incorretas!\n");
                break;
            case OVERWRITE_ATTEMPT:
                printf("\n Sem sobrescrever!\n");
                break;
            default:
                printf("\n Erro! Caso nao previsto.\n");
                finish();
            }

            change_color(COLOR_RESET);
        }

        for (;;)
        {
            printf("\n\n Deseja jogar novamente? (Y)- sim (N)- Nao\n\n");
            scanf(" %c", &try_again);
            try_again = tolower(try_again);

            clear();

            if (try_again == 'y')
                break;
            else if (try_again == 'n')
                finish();
            else
                printf("Erro! Digite apenas Y ou N.\n");
        }
    }
}

void clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void change_color(const char *color)
{
    printf("\x1b[0m%s", color);
}

void initial_setup()
{
    memset(grid_board, ' ', sizeof(grid_board));

    player = 0;
    game_movement_count = 1;
    game_status = 0;
    is_game_ended = false;
    try_again = ' ';
}

bool check_position(char a, char b, char c)
{
    return a == b && a == c && a != ' ';
}

const char *get_player_icon_color(char pos)
{
    if (pos == ' ')
        return " ";

    const char *color = (pos == 'X') ? COLOR_RED : COLOR_GREEN;

    size_t color_len = strlen(color);
    size_t pos_len = 1;
    size_t reset_len = strlen(COLOR_RESET);

    size_t result_len = color_len + pos_len + reset_len + 1;

    char *result = (char *)malloc(result_len);
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation error while trying to get player icon color.\n");
        return " ";
    }

    snprintf(result, result_len, "%s%c%s", color, pos, COLOR_RESET);
    return result;
}

void print_board()
{
    const char *icon[3][3];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            icon[i][j] = get_player_icon_color(grid_board[i][j]);

    change_color(COLOR_RESET);
    printf("   |1||2||3|\n");
    printf("   ---------\n");
    printf(" 1-|%s||%s||%s|\n", icon[0][0], icon[0][1], icon[0][2]);
    printf(" 2-|%s||%s||%s|\n", icon[1][0], icon[1][1], icon[1][2]);
    printf(" 3-|%s||%s||%s|\n", icon[2][0], icon[2][1], icon[2][2]);
    printf("   ---------\n");

    // for (int i = 0; i < 3; i++)
    //     for (int j = 0; j < 3; j++)
    //         free((void *)icon[i][j]);
}

int validate_input(int x, int y, int count)
{
    if ((x < 0 || x >= 3) ||
        (y < 0 || y >= 3) ||
        count != 2)
        return INCORRECT_INPUT;

    if (grid_board[x][y] != ' ')
        return OVERWRITE_ATTEMPT;

    grid_board[x][y] = player_icon[player];

    for (int i = 0; i < 3; i++)
        if (check_position(grid_board[i][0], grid_board[i][1], grid_board[i][2]) || check_position(grid_board[0][i], grid_board[1][i], grid_board[2][i]))
            return PLAYER_WIN;
    if (check_position(grid_board[0][0], grid_board[1][1], grid_board[2][2]) || check_position(grid_board[2][0], grid_board[1][1], grid_board[0][2]))
        return PLAYER_WIN;

    if (game_movement_count == 9)
        return DRAW;

    return ACCEPTED_MOVEMENT;
}

void finish()
{
    change_color(COLOR_RESET);
    exit(EXIT_FAILURE);
}