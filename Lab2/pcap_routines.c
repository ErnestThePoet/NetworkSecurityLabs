#include "pcap_routines.h"

static void GetAddressString(
    char *buffer, const size_t maxlen, const struct sockaddr *address)
{
    switch (address->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET,
                  &(((struct sockaddr_in *)address)->sin_addr),
                  buffer,
                  maxlen);
        break;

    case AF_INET6:
        inet_ntop(AF_INET6,
                  &(((struct sockaddr_in6 *)address)->sin6_addr),
                  buffer,
                  maxlen);
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
        printf("[%d] %s\n  Address(es):  ",
               current_device_index,
               current_device_ptr->description);

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
                GetAddressString(address_string,
                                 sizeof(address_string),
                                 current_address->addr);
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

FILE *PrepareOutputFile(pcap_t *capture_handle, const char *filter)
{
    char file_name[50];

    time_t current_time = time(NULL);
    struct tm *current_time_tuple = localtime(&current_time);
    strftime(file_name,
             sizeof(file_name),
             "capture_%Y_%m_%d_%H_%M_%S.txt",
             current_time_tuple);

    FILE *output_file = fopen(file_name, "w");
    if (output_file == NULL)
    {
        printf("Failed to open %s\n", file_name);
        pcap_close(capture_handle);
        exit(FAILURE);
    }

    fprintf(output_file,
            "---Packet Capture Results---\nFilter: %s\n",
            filter == NULL ? "<None>" : filter);

    return output_file;
}

static void HandlePacket(
    uint8_t *user, const struct pcap_pkthdr *h, const uint8_t *bytes)
{
    FILE *output_file = (FILE *)user;

    EthernetHeader *ethernet_header = (EthernetHeader *)bytes;
    uint16_t ethernet_payload_type = ntohs(ethernet_header->type);
    if (ethernet_payload_type != ETHERNET_PAYLOAD_TYPE_IP4)
    {
        printf("Ethernet frame payload is not IPv4 (type=%#06x). Frame discarded\n",
               ethernet_payload_type);
        return;
    }

    Ip4Header *ip4_header = (Ip4Header *)(bytes + ETHERNET_HEADER_LENGTH);

    if (ip4_header->protocol != IP4_PAYLOAD_TYPE_TCP && ip4_header->protocol != IP4_PAYLOAD_TYPE_UDP)
    {
        printf("IPv4 packet payload is neither TCP nor UDP (type=%#04x). Packet discarded\n",
               ip4_header->protocol);
        return;
    }

    PrintEthernetHeader(output_file, ethernet_header);
    PrintIp4Header(output_file, ip4_header);

    // DF=1 or Offset=0
    if (ip4_header->foff & 0x4000U || !(ip4_header->foff & 0x1FFFU))
    {
        uint8_t *transport_layer_header = ((uint8_t *)ip4_header) + ((ip4_header->hlv) & 0x0FU) * 4;

        switch (ip4_header->protocol)
        {
        case IP4_PAYLOAD_TYPE_TCP:
            PrintTcpHeader(output_file, (TcpHeader *)transport_layer_header);
            break;
        case IP4_PAYLOAD_TYPE_UDP:
            PrintUdpHeader(output_file, (UdpHeader *)transport_layer_header);
            break;
        }
    }
}

void StartCaptureLoop(pcap_t *capture_handle, FILE *output_file)
{
    puts("Capture loop started");

    int error_code = pcap_loop(capture_handle, 0, HandlePacket, (uint8_t *)output_file);

    switch (error_code)
    {
    case PCAP_ERROR_BREAK:
        puts("Capture loop FAILED: "
             "loop terminated due to a call to pcap_breakloop() before any packets were processed");
        CleanUp(capture_handle, output_file);
        exit(FAILURE);
    case PCAP_ERROR_NOT_ACTIVATED:
        puts("Capture loop failed: capture handle that has been created but not activated");
        CleanUp(capture_handle, output_file);
        exit(FAILURE);
    case PCAP_ERROR:
        pcap_perror(capture_handle, NULL);
        CleanUp(capture_handle, output_file);
        exit(FAILURE);
    }
}

void CleanUp(pcap_t *capture_handle, FILE *output_file)
{
    pcap_close(capture_handle);
    fclose(output_file);
}