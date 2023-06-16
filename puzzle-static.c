/*

 _____                                                    _    _____         _
/  __ \                                                  | |  /  ___|       | |
| /  \/ _ __   ___   ___  ___ __      __  ___   _ __   __| |  \ `--.   ___  | |__   __  ___  _ __
| |    | '__| / _ \ / __|/ __|\ \ /\ / / / _ \ | '__| / _` |   `--. \ / _ \ | |\ \ / / / _ \| '__|
| \__/\| |   | (_) |\__ \\__ \ \ V  V / | (_) || |   | (_| |  /\__/ /| (_) || | \ V / |  __/| |
 \____/|_|    \___/ |___/|___/  \_/\_/   \___/ |_|    \__,_|  \____/  \___/ |_|  \_/   \___||_|

Static Approach
E/19/063 - D M G S Dassanayake
E/19/264 - N.P.T. Nishantha

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORDS 50
#define MAX_WORD_LENGTH 50

typedef struct Crossword
{
    int rows;
    int cols;
    char grid[MAX_WORDS][MAX_WORDS];
} Crossword;

typedef struct Word
{
    char word[MAX_WORD_LENGTH];
    int length;
    int possibles[MAX_WORDS][MAX_WORDS];
    int count;
    int isAssigned;

} Word;

// Global Variables;

Word word_list[MAX_WORDS];

int word_count = 0;

// Functions

Crossword read_crossword(Crossword crossword)

{

    char string[MAX_WORD_LENGTH];
    crossword.cols = 0;
    crossword.rows = 0;

    while (strcmp(string, "\n"))
    {
        fflush(stdin);

        fgets(string, MAX_WORD_LENGTH, stdin);

        if (crossword.cols == 0)
        {
            crossword.cols = strlen(string);
        }

        // If the grid is not rectangular
        if (crossword.cols != strlen(string) && strlen(string) > 1)
        {

            printf("INVALID INPUT");
            exit(0);
        }

        for (int col = 0; col < strlen(string); col++)

        {

            // if input is valid add to grid
            if (string[col] == '#' || string[col] == '*' || (string[col] >= 'A' && string[col] <= 'Z') || (string[col] >= 'a' && string[col] <= 'z'))
            {
                crossword.grid[crossword.rows][col] = string[col];
            }

            // if you find a new line go to next row
            else if (string[col] == '\n')
            {
                crossword.rows++;
                continue;
            }

            // for Invalid inputs

            else
            {
                printf("INVALID INPUT");
                exit(0);
            }
        }
    }
    return crossword;
}

void read_word_list()
{

    Word current_word;
    current_word.isAssigned = 0;

    while (strcmp(current_word.word, "\n"))

    {
        fgets(current_word.word, MAX_WORD_LENGTH, stdin);
        current_word.length = strlen(current_word.word) - 1;

        for (int col = 0; col < strlen(current_word.word); col++)

        {

            // if input is valid add to grid
            if ((current_word.word[col] >= 'A' && current_word.word[col] <= 'Z') || (current_word.word[col] >= 'a' && current_word.word[col] <= 'z'))
            {
                continue;
            }

            // if you find a new line go to next word
            else if (current_word.word[col] == '\n')
            {
                word_count++;
                break;
            }

            // for Invalid inputs

            else
            {
                printf("INVALID INPUT");
                exit(0);
            }
        }
        // Capitaization -- For the moment capital and simple letters are kept as given and same uppercase letter and lowercase letter is considered different. Uncomment this to capitalize all the words
        /*
        for (int i = 0; i < current_word.length; i++)
        {
            if (current_word.word[i] >= 'a' && current_word.word[i] <= 'z')
            {
                current_word.word[i] = current_word.word[i] - 32;
            }
        }
        */

        word_list[word_count - 1] = current_word;
    }
}

Word get_occurances_horizontal(Crossword crossword, Word word)

{

    int count = 0;
    int start_check = 0;
    int temp_word_length = 0;
    int occur_start_row = -1;
    int occur_start_col = -1;

    for (int row = 0; row < crossword.rows; row++)
    {
        for (int col = 0; col < crossword.cols; col++)
        {

            if (crossword.grid[row][col] == '#' || crossword.grid[row][col] == word.word[0])
            {
                start_check = 1;
                occur_start_row = row;
                occur_start_col = col;

                col++;
                temp_word_length++;
            }
            if (start_check)
            {
                for (int k = 1; k < word.length; k++)
                {
                    if (crossword.grid[row][col] == '#' || crossword.grid[row][col] == word.word[k])
                    {

                        temp_word_length++;
                        col++;
                    }
                }
                start_check = 0;
            }
            if (temp_word_length == word.length && (crossword.grid[row][col] != '#' || col == crossword.cols))
            {
                word.possibles[count][0] = occur_start_row;
                word.possibles[count][1] = occur_start_col;
                count++;
            }
            temp_word_length = 0;
        }
    }
    word.count = count;
    return word;
}
Word get_occurances_vertical(Crossword crossword, Word word)
{
    int count = 0;
    int start_check = 0;
    int temp_word_length = 0;
    int occur_start_row = -1;
    int occur_start_col = -1;

    for (int col = 0; col < crossword.cols; col++)
    {
        for (int row = 0; row < crossword.rows; row++)
        {

            if (crossword.grid[row][col] == '#' || crossword.grid[row][col] == word.word[0])
            {
                start_check = 1;
                occur_start_row = row;
                occur_start_col = col;

                temp_word_length++;

                row++;
            }
            if (start_check)
            {
                for (int k = 1; k < word.length; k++)
                {
                    if (crossword.grid[row][col] == '#' || crossword.grid[row][col] == word.word[k])
                    {

                        temp_word_length++;

                        row++;
                    }
                }
                start_check = 0;
            }
            if (temp_word_length == word.length && (crossword.grid[row][col] != '#' || row == crossword.rows))
            {

                word.possibles[count][0] = occur_start_row;
                word.possibles[count][1] = occur_start_col;

                count++;
            }
            temp_word_length = 0;
        }
    }

    word.count = count;
    return word;
}
Crossword assign_to_grid(Crossword crossword, Word word, int row, int col, int dir, int num)
{
    if (row < 0 || col < 0)
    {
        return crossword;
    }

    // dir 0 - accross, 1 - down
    if (dir == 0)
    {
        for (int i = 0; i < word.length; i++)
        {

            crossword.grid[row][i + col] = word.word[i];
        }
    }
    if (dir == 1)
    {
        for (int i = 0; i < word.length; i++)
        {

            crossword.grid[i + row][col] = word.word[i];
        }
    }
    word_list[num].isAssigned = 1;
    return crossword;
}

int is_grid_complete(Crossword crossword)
{
    int is_complete = 1;
    for (int col = 0; col < crossword.cols; col++)
    {
        for (int row = 0; row < crossword.rows; row++)
        {
            if (crossword.grid[row][col] == '#')
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

int is_all_words_assigned()
{
    int all_assigned = 1;
    for (int i = 0; i < word_count; i++)
    {

        if (word_list[i].isAssigned == 0)
        {
            all_assigned = 0;
            break;
        }
    }
    return all_assigned;
}

void print_grid(Crossword crossword)
{
    for (int i = 0; i < crossword.rows; i++)
    {
        for (int j = 0; j < crossword.cols; j++)
        {
            printf("%c", crossword.grid[i][j]);
        }
        printf("\n");
    }
}

void solve_crossword(Crossword crossword, int num)
{

    // Base Case
    if (is_grid_complete(crossword) && is_all_words_assigned())
    {
        print_grid(crossword);
        exit(0);
    }

    // Recursion
    Word hori_upt_word = get_occurances_horizontal(crossword, word_list[num]);
    Word verti_upt_word = get_occurances_vertical(crossword, word_list[num]);

    for (int j = 0; j < hori_upt_word.count; j++)
    {
        if (num < word_count)
            solve_crossword(assign_to_grid(crossword, word_list[num], hori_upt_word.possibles[j][0], hori_upt_word.possibles[j][1], 0, num), num + 1);
    }
    for (int j = 0; j < verti_upt_word.count; j++)
    {
        if (num < word_count)
            solve_crossword(assign_to_grid(crossword, word_list[num], verti_upt_word.possibles[j][0], verti_upt_word.possibles[j][1], 1, num), num + 1);
    }
}
int main()
{
    Crossword crossword;
    crossword = read_crossword(crossword);

    fflush(stdin);

    read_word_list();

    // resolving new line input read issue with fgets
    crossword.rows--;
    crossword.cols--;
    word_count--;

    solve_crossword(crossword, 0);

    // if exit(0) was not executed, then we know that we cannot assign all the words to the grid
    printf("IMPOSSIBLE");

    return 0;
}