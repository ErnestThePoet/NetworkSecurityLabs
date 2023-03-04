#ifndef TRANS_H_
#define TRANS_H_

#define FAILURE_RETURN_FREE(P, M)     \
    do                                \
    {                                 \
        free(P);                      \
        strcpy(result.error_info, M); \
        return result;                \
    } while (false)

#endif