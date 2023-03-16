#include "pcap_routines.h"

static void GetAddressString(
    char *buffer, const size_t maxlen, const struct sockaddr *address)
{
    switch (address->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET, &(((struct sockaddr_in *)address)->sin_addr), buffer, maxlen);
        break;

    case AF_INET6:
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)address)->sin6_addr), buffer, maxlen);
        break;

    default:
        sprintf(buffer, "<Unknown sa_family: %d>", address->sa_family);
        break;
    }
}

static void ClearStdInBuffer()
{
    char temp_char = 0;
    while ((temp_char = getchar()) != '\n' && temp_char != EOF)
        ;
}

void Initialize()
{
    int error_code = pcap_init(PCAP_CHAR_ENC_UTF_8, kErrorBuf);
    CHECK_ERROR;
}

int PrintDeviceList(pcap_if_t **device_list_ret)
{
    pcap_if_t *device_list = NULL;
    int error_code = pcap_findalldevs(&device_list, kErrorBuf);
    CHECK_ERROR;

    if (device_list == NULL)
    {
        puts("There are no devices detected on the system");
        exit(SUCCESS);
    }

    puts("-------- Detected Devices --------");

    pcap_if_t *current_device_ptr = device_list;
    int current_device_index = 0;
    while (current_device_ptr != NULL)
    {
        printf("[%d] %s\n  Address(es):  ", current_device_index, current_device_ptr->description);

        pcap_addr_t *current_address = current_device_ptr->addresses;

        if (current_address == NULL)
        {
            printf("None");
        }
        else
        {
            char address_string[50];
            while (current_address != NULL)
            {
                GetAddressString(address_string, 50, current_address->addr);
                printf("%s   ", address_string);
                current_address = current_address->next;
            }
        }

        puts("\n");

        current_device_ptr = current_device_ptr->next;
        current_device_index++;
    }

    *device_list_ret = device_list;
    return current_device_index;
}

int GetUserSelectedDeviceIndex(const int device_count)
{
    printf("Enter selected device index(0 to %d):", device_count - 1);

    const char *const kTryAgainPrompt = "Invalid input; try again:";

    while (true)
    {
        int entered_device_index = -1;
        int read_count = scanf("%d", &entered_device_index);
        ClearStdInBuffer();

        if (read_count != 1 || entered_device_index < 0 || entered_device_index >= device_count)
        {
            printf("%s", kTryAgainPrompt);
            continue;
        }

        return entered_device_index;
    }
}

pcap_t *GetCaptureHandle(pcap_if_t *device_list, const int device_index)
{
    pcap_if_t *curent_device_ptr = device_list;
    for (int i = 0; i < device_index; i++)
    {
        curent_device_ptr = curent_device_ptr->next;
    }

    pcap_t *capture_handle = pcap_open_live(
        curent_device_ptr->name, BUFSIZ, 0, -1, kErrorBuf);

    pcap_freealldevs(device_list);

    if (capture_handle == NULL)
    {
        puts(kErrorBuf);
        exit(FAILURE);
    }

    return capture_handle;
}

void SetFilter(pcap_t *capture_handle, const char *filter)
{
    struct bpf_program program;
    if (pcap_compile(capture_handle, &program, filter, 1, PCAP_NETMASK_UNKNOWN))
    {
        pcap_perror(capture_handle, NULL);
        pcap_close(capture_handle);
        exit(FAILURE);
    }

    int error_code = pcap_setfilter(capture_handle, &program);
    if (error_code == PCAP_ERROR_NOT_ACTIVATED)
    {
        pcap_close(capture_handle);
        puts("pcap_setfilter() failed: capture handle that has been created but not activated");
        exit(FAILURE);
    }
    else if (error_code == PCAP_ERROR)
    {
        pcap_perror(capture_handle, NULL);
        pcap_close(capture_handle);
        exit(FAILURE);
    }

    pcap_freecode(&program);
}