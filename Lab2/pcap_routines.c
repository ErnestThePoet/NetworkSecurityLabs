#include "pcap_routines.h"

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