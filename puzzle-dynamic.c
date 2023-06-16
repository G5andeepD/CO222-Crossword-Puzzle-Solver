/*

 _____                                                    _    _____         _
/  __ \                                                  | |  /  ___|       | |
| /  \/ _ __   ___   ___  ___ __      __  ___   _ __   __| |  \ `--.   ___  | |__   __  ___  _ __
| |    | '__| / _ \ / __|/ __|\ \ /\ / / / _ \ | '__| / _` |   `--. \ / _ \ | |\ \ / / / _ \| '__|
| \__/\| |   | (_) |\__ \\__ \ \ V  V / | (_) || |   | (_| |  /\__/ /| (_) || | \ V / |  __/| |
\____/|_|    \___/ |___/|___/  \_/\_/   \___/ |_|    \__,_|  \____/  \___/ |_|  \_/   \___||_|

Dynamic Approach

E/19/063 - D M G S Dassanayake
E/19/264 - R.P.T. Nishantha

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 30

// GLOBAL VARIABLES

int rows = 0;
int cols = 0;
int word_count = 0;
char **grid;
char **words;

// STRUCTURES

typedef struct occurances
{
    int count;
    int possibles[MAX_WORD_SIZE][2];
} Occurances;

// FUNCTIONS

void read_grid()
{
    // Allocate space for the global grid
    grid = (char **)malloc(MAX_WORD_SIZE * sizeof(char *));

    // Getting input for the grid
    while (1)
    {
        // Allocate Space for a row

        char *str = (char *)malloc(MAX_WORD_SIZE * sizeof(char));
        fgets(str, MAX_WORD_SIZE, stdin);
        if (rows == 0)
        {
            cols = strlen(str) - 1; // since fgets take a '\n' at the end
        }

        // finding end of inputting grid
        if (!strcmp(str, "\n"))
        {
            free(str);
            break;
        }

        // remove taking '\n' character taken by fgets
        str[strlen(str) - 1] = '\0';

        // Input Validation
        for (int i = 0; i < strlen(str); i++)
        {
            if (!(*(str + i) == '#' || *(str + i) == '*' || (*(str + i) >= 'A' && *(str + i) <= 'Z') || (*(str + i) >= 'a' && *(str + i) <= 'z')))
            {
                printf("INVALID INPUT");

                // freeing allocated variables
                free(str);
                for (int i = 0; i < rows; i++)
                {
                    free(*(grid + i));
                }
                free(grid);
                for (int i = 0; i < word_count; i++)
                {
                    free(*(words + i));
                }
                free(words);

                exit(0);
            }
        }

        // dynamically allocating space for just the length.
        *(grid + rows) = malloc((strlen(str) + 1) * sizeof(char));
        memcpy(*(grid + rows), str, (strlen(str) + 1) * sizeof(char));
        free(str);

        rows++;
    }
    // freeing extra allocated space
    char **new_grid = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++)
    {
        new_grid[i] = (char *)malloc(sizeof(char) * (cols + 1));
        memcpy(new_grid[i], grid[i], sizeof(char) * (cols + 1));
    }

    for (int i = 0; i < rows; i++)
    {
        free(grid[i]);
    }
    free(grid);
    grid = new_grid;

    return;
}

void read_word_list()
{
    // allocating space for global word list

    words = (char **)malloc(MAX_WORD_SIZE * sizeof(char *));

    while (1)
    {
        // initially allocating space for a word to receive input
        char *str = (char *)malloc(MAX_WORD_SIZE * sizeof(char));
        fgets(str, MAX_WORD_SIZE, stdin);

        if (!strcmp(str, "\n"))
        {
            free(str);

            break;
        }
        str[strlen(str) - 1] = '\0';

        // Input Validation
        for (int i = 0; i < strlen(str); i++)
        {
            if (!((*(str + i) >= 'A' && *(str + i) <= 'Z') || (*(str + i) >= 'a' && *(str + i) <= 'z')))
            {
                printf("INVALID INPUT");

                free(str);
                for (int i = 0; i < rows; i++)
                {
                    free(*(grid + i));
                }
                free(grid);
                for (int i = 0; i < word_count; i++)
                {
                    free(*(words + i));
                }
                free(words);
                exit(0);
            }
        }
        // dynamically allocating space for just the word length
        *(words + word_count) = malloc((strlen(str) + 1) * sizeof(char));
        memcpy(*(words + word_count), str, (strlen(str) + 1) * sizeof(char));
        free(str);

        word_count++;

        fflush(stdin);
    }
    // freeing extra allocated space
    char **new_words = (char **)malloc(word_count * sizeof(char *));
    for (int i = 0; i < word_count; i++)
    {
        new_words[i] = (char *)malloc(sizeof(char) * (strlen(words[i]) + 1));
        memcpy(new_words[i], words[i], sizeof(char) * (strlen(words[i]) + 1));
    }

    for (int i = 0; i < word_count; i++)
    {
        free(words[i]);
    }
    free(words);
    words = new_words;
    return;
}

// function that finds where a word can fit in the grid horizontally
Occurances get_occurences_horizontal(char *word)
{
    Occurances horizontal_occuraces;

    int count = 0;
    int start_check = 0;
    int temp_word_length = 0;
    int occur_start_row = -1;
    int occur_start_col = -1;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (*(*(grid + i) + j) == '#' || *(*(grid + i) + j) == *word)
            {
                if (j + strlen(word) <= cols)
                {
                    start_check = 1;
                    occur_start_row = i;
                    occur_start_col = j;

                    temp_word_length++;

                    j++;
                }
            }
            if (start_check)
            {
                for (int k = 1; k < strlen(word); k++)
                {

                    if (grid[i][j] == '#' || grid[i][j] == *(word + k))
                    {

                        temp_word_length++;

                        j++;
                        if (j == cols)
                            break;
                    }
                }
                start_check = 0;
            }
            if (temp_word_length == strlen(word) && (j == cols || *(*(grid + i) + j) == '*'))
            {

                horizontal_occuraces.possibles[count][0] = occur_start_row;
                horizontal_occuraces.possibles[count][1] = occur_start_col;

                count++;
            }
            temp_word_length = 0;
        }
    }
    horizontal_occuraces.count = count;
    return horizontal_occuraces;
}

// function that finds where a word can fit in the grid vertically
Occurances get_occurences_vertical(char *word)
{

    Occurances vertical_occurances;

    int count = 0;
    int start_check = 0;
    int temp_word_length = 0;
    int occur_start_row = -1;
    int occur_start_col = -1;

    for (int col = 0; col < cols; col++)
    {
        for (int row = 0; row < rows; row++)
        {

            if (*(*(grid + row) + col) == '#' || *(*(grid + row) + col) == *word)
            {

                if (row + strlen(word) <= rows)
                {
                    start_check = 1;
                    occur_start_row = row;
                    occur_start_col = col;

                    temp_word_length++;

                    row++;
                }
            }
            if (start_check)
            {

                for (int k = 1; k < strlen(word); k++)
                {

                    if (grid[row][col] == *(word + k) || grid[row][col] == '#')
                    {

                        temp_word_length++;

                        row++;
                        if (row == rows)
                        {

                            break;
                        }
                    }
                }
                start_check = 0;
            }
            if (temp_word_length == strlen(word) && (row == rows || *(*(grid + row) + col) == '*'))
            {

                vertical_occurances.possibles[count][0] = occur_start_row;
                vertical_occurances.possibles[count][1] = occur_start_col;

                count++;
            }
            temp_word_length = 0;
        }
    }
    vertical_occurances.count = count;
    return vertical_occurances;
}

// function that assigns a word to the grid in a given location
void assign_to_grid(Occurances occurance, int index, char *word, int dir)
{
    // dir 0-vertical 1-horizontal

    int row = occurance.possibles[index][0];
    int col = occurance.possibles[index][1];

    // checking whether valid row and columns is provided
    if (!(row > 0 || row < rows || col < cols || col > 0))
    {
        return;
    }
    // Horizontal Assignment
    if (dir == 1)
    {

        for (int i = 0; i < strlen(word); i++)
        {

            grid[row][i + col] = *(word + i);
        }
    }
    // Vertical Assignment
    if (dir == 0)
    {

        for (int i = 0; i < strlen(word); i++)
        {

            grid[i + row][col] = *(word + i);
        }
    }
}

// function that prints the global grid
void print_grid()
{
    for (int i = 0; i < rows; i++)
    {

        printf("%s\n", *(grid + i));
    }
}

// function that checks whether there are any # s left in the grid
int is_grid_complete()
{
    int is_complete = 1;
    for (int col = 0; col < cols; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            if (grid[row][col] == '#')
            {
                is_complete = 0;
                break;
            }
        }
        if (is_complete == 0)
        {
            break;
        }
    }
    return is_complete;
}

// Recursive function which solves the crossword

void solve_crossword(int word_start)
{
    // usually word_start = 0 -> to start from beginning of the word list

    // if all words are assigned and grid is completed
    if (word_start == word_count && is_grid_complete())
    {

        print_grid();

        // free global grid
        for (int i = 0; i < rows; i++)
        {
            free(*(grid + i));
        }
        free(grid);

        // free global words
        for (int i = 0; i < word_count; i++)
        {
            free(*(words + i));
        }
        free(words);

        exit(0);
    }
    // Obtaining Occurances of the word for the current state of the grid
    Occurances vertical_occurances = get_occurences_vertical(*(words + word_start));

    Occurances horizontal_occurances = get_occurences_horizontal(*(words + word_start));
    /*
    Creating a copy of the current state of the grid.
    This is used to restore a previous grid by backtracking
     if the path of the recursion does not yield a correct answer.
    */
    char new_grid[rows][cols + 1];
    for (int i = 0; i < rows; i++)
    {

        strcpy(new_grid[i], *(grid + i));
    }

    // try assigning the word to the possible spaces and calling the function again for the next word

    for (int i = 0; i < vertical_occurances.count; i++)
    {
        assign_to_grid(vertical_occurances, i, *(words + word_start), 0);

        solve_crossword(word_start + 1);
        // restoring the previous state of the grid
        for (int j = 0; j < rows; j++)
        {

            strcpy(*(grid + j), new_grid[j]);
        }
    }

    for (int i = 0; i < horizontal_occurances.count; i++)
    {
        assign_to_grid(horizontal_occurances, i, *(words + word_start), 1);
        solve_crossword(word_start + 1);
        // restoring the previous state of the grid
        for (int j = 0; j < rows; j++)
        {

            strcpy(*(grid + j), new_grid[j]);
        }
    }
}

int main()
{

    read_grid();
    read_word_list();
    solve_crossword(0);
    printf("IMPOSSIBLE");
    return 0;
}