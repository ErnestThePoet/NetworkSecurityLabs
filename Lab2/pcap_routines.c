#include "pcap_routines.h"

void Initialize()
{
    int error_code = pcap_init(PCAP_CHAR_ENC_UTF_8, kErrorBuf);
    CHECK_ERROR;
}