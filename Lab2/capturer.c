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

    pcap_if_t *device_list = NULL;
    int device_count = PrintDeviceList(&device_list);
    int selected_device_index = GetUserSelectedDeviceIndex(device_count);
    pcap_t *capture_handle = GetCaptureHandle(device_list, selected_device_index);

    if (argc == 2)
    {
        SetFilter(capture_handle, argv[1]);
    }

    FILE *output_file = PrepareOutputFile(capture_handle, argc == 2 ? argv[1] : NULL);

    return SUCCESS;
}

bool CheckArgs(const int argc, char *argv[])
{
    if (argc != 1 && argc != 2)
    {
        return false;
    }

    return true;
}