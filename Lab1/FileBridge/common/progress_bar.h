#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <stdio.h>

#define PROGRESS_BAR_LENGTH 50
#define PROGRESS_BAR_FINISHED_CHAR '='
#define PROGRESS_BAR_UNFINISHED_CHAR '-'

void StartProgressBar();
// percentage: [0,1]
void UpdateProgressBar(double percentage);
// remain previous percentage, start a new line
void EndProgressBar();

#endif