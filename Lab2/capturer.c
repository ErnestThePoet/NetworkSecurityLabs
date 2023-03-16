#include "capturer.h"

int main(int argc, char *argv[])
{
    const char *const kUsageHelp = "Usage: capturer [filter-options]";
    if (!CheckArgs(argc, argv))
    {
        puts(kUsageHelp);
        return FAILURE;
    }

    Initialize();
}

bool CheckArgs(const int argc, char *argv[])
{
    if (argc != 1 && argc != 2)
    {
        return false;
    }

    return true;
}