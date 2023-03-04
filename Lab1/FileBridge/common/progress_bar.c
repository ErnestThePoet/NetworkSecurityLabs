#include "progress_bar.h"

static void ClearConsoleLine(){
    printf("\33[2K\r");
}

void StartProgressBar()
{
    putchar('[');
    for (int i = 0; i < PROGRESS_BAR_LENGTH; i++)
    {
        putchar(PROGRESS_BAR_UNFINISHED_CHAR);
    }
    printf("] 0%%");
}

void UpdateProgressBar(double percentage)
{
    ClearConsoleLine();
    putchar('[');
    for (int i = 0; i < PROGRESS_BAR_LENGTH; i++)
    {
        putchar(i <= PROGRESS_BAR_LENGTH * percentage
                    ? PROGRESS_BAR_FINISHED_CHAR
                    : PROGRESS_BAR_UNFINISHED_CHAR);
    }
    printf("] %d%%", (int)(100 * percentage));
}

void EndProgressBar(){
    putchar('\n');
}