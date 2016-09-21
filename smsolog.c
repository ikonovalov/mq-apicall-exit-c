#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <syslog.h>

#include <cmqec.h>

#include <sys/time.h>

/********************************************************************/
/* Macros to find the max or min of a pair of numbers               */
/********************************************************************/
#ifndef max
#define max(a, b)        (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b)        (((a) < (b)) ? (a) : (b))
#endif

/*********************************************************************/
/* Definitions of data structures                                    */
/*********************************************************************/

typedef struct myExitUserArea
{
    MQLONG Options;

} MYEXITUSERAREA;

void MQStart() {
    ;
}


unsigned int excludeQueue(char* queueName, char *filter) {
    return strstr(queueName, filter) == NULL ? 1 : 0;
}

unsigned int excludeUser(PMQAXC context, char* excludedUser) {
    return strstr(context -> UserId, excludedUser) == NULL ? 1 : 0;
}

char* toHex(char *input, size_t size) {
    int i = 0;
    char* buf_str = (char*) malloc (2 * size + 1);
    for (i = 0; i < size; i++)
    {
        sprintf(buf_str + i*2, "%02X", input[i]);
    }
    return  buf_str;
}

char* toHex24(char* input) {
    return toHex(input, 24);
}

char* strSized(char *input, char *buffer, int sz) {
    memset(buffer, '\0', sz + 1);
    strncpy(buffer, input, sz);
    return buffer;
}

unsigned char isAllWhitespace(char* string, int sz) {
    // MQ str can't starts with empty
    if (*string == ' ' || *string == '\0')
        return 1;

    for (int z = 0; z < sz; z++) {
        char c = *(string + z);
        if (!isspace(c) && !isblank(c)) {
            return 0;
        }
    }
    return 1;
}


char *strMsgType(MQLONG type, char* buffer) {
    char  *ret = NULL;
    switch (type) {
        case MQMT_DATAGRAM:
            ret = "MQMT_DATAGRAM";
            break;
        case MQMT_REQUEST:
            ret = "MQMT_REQUEST";
            break;
        case MQMT_REPLY:
            ret = "MQMT_REPLY";
            break;
        case MQMT_REPORT:
            ret = "MQMT_REPORT";
            break;
        default:
            sprintf(buffer, "%s", buffer);
            ret = buffer;
    }
    return ret;
}


/*********************************************************************/
/*                                                                   */
/* Convert an appl type value into a string                          */
/*                                                                   */
/*********************************************************************/

char *strApplType(MQLONG ApplType, char *Buffer) {
    char *p = NULL;

    switch (ApplType) {
        case MQAT_UNKNOWN:
            p = "MQAT_UNKNOWN";
            break;
        case MQAT_NO_CONTEXT:
            p = "MQAT_NO_CONTEXT";
            break;
        case MQAT_CICS:
            p = "MQAT_CICS";
            break;
        case MQAT_MVS:
            p = "MQAT_MVS";
            break;
        case MQAT_IMS:
            p = "MQAT_IMS";
            break;
        case MQAT_OS2:
            p = "MQAT_OS2";
            break;
        case MQAT_DOS:
            p = "MQAT_DOS";
            break;
        case MQAT_UNIX:
            p = "MQAT_UNIX";
            break;
        case MQAT_QMGR:
            p = "MQAT_QMGR";
            break;
        case MQAT_OS400:
            p = "MQAT_OS400";
            break;
        case MQAT_WINDOWS:
            p = "MQAT_WINDOWS";
            break;
        case MQAT_CICS_VSE:
            p = "MQAT_CICS_VSE";
            break;
        case MQAT_WINDOWS_NT:
            p = "MQAT_WINDOWS_NT";
            break;
        case MQAT_VMS:
            p = "MQAT_VMS";
            break;
        case MQAT_NSK:
            p = "MQAT_NSK";
            break;
        case MQAT_VOS:
            p = "MQAT_VOS";
            break;
        case MQAT_IMS_BRIDGE:
            p = "MQAT_IMS_BRIDGE";
            break;
        case MQAT_XCF:
            p = "MQAT_XCF";
            break;
        case MQAT_CICS_BRIDGE:
            p = "MQAT_CICS_BRIDGE";
            break;
        case MQAT_NOTES_AGENT:
            p = "MQAT_NOTES_AGENT";
            break;
        case MQAT_BROKER:
            p = "MQAT_BROKER";
            break;
        case MQAT_JAVA:
            p = "MQAT_JAVA";
            break;
        case MQAT_DQM:
            p = "MQAT_DQM";
            break;
        case MQAT_SYSTEM_EXTENSION:
            p = "MQAT_SYSTEM_EXTENSION";
            break;
        default:
            sprintf(Buffer, "%d", ApplType);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an ExitId value into a string                             */
/*                                                                   */
/*********************************************************************/

char *strExitId(MQLONG ExitId, char *Buffer) {
    char *p = NULL;

    switch (ExitId) {
        case MQXT_API_EXIT:
            p = "MQXT_API_EXIT";
            break;
        case MQXT_CHANNEL_SEC_EXIT:
            p = "MQXT_CHANNEL_SEC_EXIT";
            break;
        case MQXT_CHANNEL_MSG_EXIT:
            p = "MQXT_CHANNEL_MSG_EXIT";
            break;
        case MQXT_CHANNEL_SEND_EXIT:
            p = "MQXT_CHANNEL_SEND_EXIT";
            break;
        case MQXT_CHANNEL_RCV_EXIT:
            p = "MQXT_CHANNEL_RCV_EXIT";
            break;
        case MQXT_CHANNEL_MSG_RETRY_EXIT:
            p = "MQXT_CHANNEL_MSG_RETRY_EXIT";
            break;
        case MQXT_CHANNEL_AUTO_DEF_EXIT:
            p = "MQXT_CHANNEL_AUTO_DEF_EXIT";
            break;
        case MQXT_CLUSTER_WORKLOAD_EXIT:
            p = "MQXT_CLUSTER_WORKLOAD_EXIT";
            break;
        case MQXT_PUBSUB_ROUTING_EXIT:
            p = "MQXT_PUBSUB_ROUTING_EXIT";
            break;
        default:
            sprintf(Buffer, "%d", ExitId);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an ExitReason into a string                               */
/*                                                                   */
/*********************************************************************/

char *strExitReason(MQLONG ExitReason, char *Buffer) {
    char *p = NULL;

    switch (ExitReason) {
        case MQXR_BEFORE:
            p = "MQXR_BEFORE";
            break;
        case MQXR_AFTER:
            p = "MQXR_AFTER";
            break;
        case MQXR_CONNECTION:
            p = "MQXR_CONNECTION";
            break;
        case MQXR_INIT:
            p = "MQXR_INIT";
            break;
        case MQXR_TERM:
            p = "MQXR_TERM";
            break;
        case MQXR_MSG:
            p = "MQXR_MSG";
            break;
        case MQXR_XMIT:
            p = "MQXR_XMIT";
            break;
        case MQXR_SEC_MSG:
            p = "MQXR_SEC_MSG";
            break;
        case MQXR_INIT_SEC:
            p = "MQXR_INIT_SEC";
            break;
        case MQXR_RETRY:
            p = "MQXR_RETRY";
            break;
        case MQXR_AUTO_CLUSSDR:
            p = "MQXR_AUTO_CLUSSDR";
            break;
        case MQXR_AUTO_RECEIVER:
            p = "MQXR_AUTO_RECEIVER";
            break;
        case MQXR_CLWL_OPEN:
            p = "MQXR_CLWL_OPEN";
            break;
        case MQXR_CLWL_PUT:
            p = "MQXR_CLWL_PUT";
            break;
        case MQXR_CLWL_MOVE:
            p = "MQXR_CLWL_MOVE";
            break;
        case MQXR_CLWL_REPOS:
            p = "MQXR_CLWL_REPOS";
            break;
        case MQXR_CLWL_REPOS_MOVE:
            p = "MQXR_CLWL_REPOS_MOVE";
            break;
        case MQXR_END_BATCH:
            p = "MQXR_END_BATCH";
            break;
        case MQXR_ACK_RECEIVED:
            p = "MQXR_ACK_RECEIVED";
            break;
        case MQXR_AUTO_SVRCONN:
            p = "MQXR_AUTO_SVRCONN";
            break;
        case MQXR_AUTO_CLUSRCVR:
            p = "MQXR_AUTO_CLUSRCVR";
            break;
        default:
            sprintf(Buffer, "%d", ExitReason);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an ExitResponse into a string                             */
/*                                                                   */
/*********************************************************************/

char *strExitResponse(MQLONG ExitResponse, char *Buffer) {
    char *p = NULL;

    switch (ExitResponse) {
        case MQXCC_OK:
            p = "MQXCC_OK";
            break;
        case MQXCC_SUPPRESS_FUNCTION:
            p = "MQXCC_SUPPRESS_FUNCTION";
            break;
        case MQXCC_SKIP_FUNCTION:
            p = "MQXCC_SKIP_FUNCTION";
            break;
        case MQXCC_SEND_AND_REQUEST_SEC_MSG:
            p = "MQXCC_SEND_AND_REQUEST_SEC_MSG";
            break;
        case MQXCC_SEND_SEC_MSG:
            p = "MQXCC_SEND_SEC_MSG";
            break;
        case MQXCC_SUPPRESS_EXIT:
            p = "MQXCC_SUPPRESS_EXIT";
            break;
        case MQXCC_CLOSE_CHANNEL:
            p = "MQXCC_CLOSE_CHANNEL";
            break;
        case MQXCC_REQUEST_ACK:
            p = "MQXCC_REQUEST_ACK";
            break;
        case MQXCC_FAILED:
            p = "MQXCC_FAILED";
            break;
        default:
            sprintf(Buffer, "%d", ExitResponse);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an ExitResponse2 into a string                            */
/*                                                                   */
/*********************************************************************/

char *strExitResponse2(MQLONG ExitResponse2, char *Buffer) {
    char *p = NULL;

    switch (ExitResponse2) {
        case MQXR2_DEFAULT_CONTINUATION:
            p = "MQXR2_DEFAULT_CONTINUATION";
            break;
        case MQXR2_CONTINUE_CHAIN:
            p = "MQXR2_CONTINUE_CHAIN";
            break;
        case MQXR2_SUPPRESS_CHAIN:
            p = "MQXR2_SUPPRESS_CHAIN";
            break;
        default:
            sprintf(Buffer, "%d", ExitResponse2);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an APICallerType into a string                            */
/*                                                                   */
/*********************************************************************/

char *strAPICallerType(MQLONG ApiCallerType, char *Buffer) {
    char *p = NULL;

    switch (ApiCallerType) {
        case MQXACT_EXTERNAL:
            p = "MQXACT_EXTERNAL";
            break;
        case MQXACT_INTERNAL:
            p = "MQXACT_INTERNAL";
            break;
        default:
            sprintf(Buffer, "%d", ApiCallerType);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an Function Id into a string                              */
/*                                                                   */
/*********************************************************************/

char *strFunctionId(MQLONG Function, char *Buffer) {
    char *p = NULL;

    switch (Function) {
        case MQXF_INIT:
            p = "MQXF_INIT";
            break;
        case MQXF_TERM:
            p = "MQXF_TERM";
            break;
        case MQXF_CONN:
            p = "MQXF_CONN";
            break;
        case MQXF_CONNX:
            p = "MQXF_CONNX";
            break;
        case MQXF_DISC:
            p = "MQXF_DISC";
            break;
        case MQXF_OPEN:
            p = "MQXF_OPEN";
            break;
        case MQXF_SUB:
            p = "MQXF_SUB";
            break;
        case MQXF_SUBRQ:
            p = "MQXF_SUBRQ";
            break;
        case MQXF_CB:
            p = "MQXF_CB";
            break;
        case MQXF_CTL:
            p = "MQXF_CTL";
            break;
        case MQXF_CALLBACK:
            p = "MQXF_CALLBACK";
            break;
        case MQXF_CLOSE:
            p = "MQXF_CLOSE";
            break;
        case MQXF_PUT1:
            p = "MQXF_PUT1";
            break;
        case MQXF_PUT:
            p = "MQXF_PUT";
            break;
        case MQXF_GET:
            p = "MQXF_GET";
            break;
        case MQXF_DATA_CONV_ON_GET:
            p = "MQXF_DATA_CONV_ON_GET";
            break;
        case MQXF_INQ:
            p = "MQXF_INQ";
            break;
        case MQXF_SET:
            p = "MQXF_SET";
            break;
        case MQXF_BEGIN:
            p = "MQXF_BEGIN";
            break;
        case MQXF_CMIT:
            p = "MQXF_CMIT";
            break;
        case MQXF_BACK:
            p = "MQXF_BACK";
            break;
        case MQXF_STAT:
            p = "MQXF_STAT";
            break;
        case MQXF_XACLOSE:
            p = "MQXF_XACLOSE";
            break;
        case MQXF_XACOMMIT:
            p = "MQXF_XACOMMIT";
            break;
        case MQXF_XACOMPLETE:
            p = "MQXF_XACOMPLETE";
            break;
        case MQXF_XAEND:
            p = "MQXF_XAEND";
            break;
        case MQXF_XAFORGET:
            p = "MQXF_XAFORGET";
            break;
        case MQXF_XAOPEN:
            p = "MQXF_XAOPEN";
            break;
        case MQXF_XAPREPARE:
            p = "MQXF_XAPREPARE";
            break;
        case MQXF_XARECOVER:
            p = "MQXF_XARECOVER";
            break;
        case MQXF_XAROLLBACK:
            p = "MQXF_XAROLLBACK";
            break;
        case MQXF_XASTART:
            p = "MQXF_XASTART";
            break;
        case MQXF_AXREG:
            p = "MQXF_AXREG";
            break;
        case MQXF_AXUNREG:
            p = "MQXF_AXUNREG";
            break;
        default:
            sprintf(Buffer, "%d", Function);
            p = Buffer;
    }

    return p;
}

/*********************************************************************/
/*                                                                   */
/* Convert an Feedback value into a string                           */
/*                                                                   */
/*********************************************************************/

char *strFeedback(MQLONG Feedback, char *Buffer) {
    char *p = NULL;

    switch (Feedback) {
        case MQFB_NONE:
            p = "MQFB_NONE";
            break;
        case MQFB_QUIT:
            p = "MQFB_QUIT";
            break;
        case MQFB_EXPIRATION:
            p = "MQFB_EXPIRATION";
            break;
        case MQFB_COA:
            p = "MQFB_COA";
            break;
        case MQFB_COD:
            p = "MQFB_COD";
            break;
        case MQFB_CHANNEL_COMPLETED:
            p = "MQFB_CHANNEL_COMPLETED";
            break;
        case MQFB_CHANNEL_FAIL_RETRY:
            p = "MQFB_CHANNEL_FAIL_RETRY";
            break;
        case MQFB_CHANNEL_FAIL:
            p = "MQFB_CHANNEL_FAIL";
            break;
        case MQFB_APPL_CANNOT_BE_STARTED:
            p = "MQFB_APPL_CANNOT_BE_STARTED";
            break;
        case MQFB_TM_ERROR:
            p = "MQFB_TM_ERROR";
            break;
        case MQFB_APPL_TYPE_ERROR:
            p = "MQFB_APPL_TYPE_ERROR";
            break;
        case MQFB_STOPPED_BY_MSG_EXIT:
            p = "MQFB_STOPPED_BY_MSG_EXIT";
            break;
        case MQFB_XMIT_Q_MSG_ERROR:
            p = "MQFB_XMIT_Q_MSG_ERROR";
            break;
        case MQFB_PAN:
            p = "MQFB_PAN";
            break;
        case MQFB_NAN:
            p = "MQFB_NAN";
            break;
        case MQFB_STOPPED_BY_CHAD_EXIT:
            p = "MQFB_STOPPED_BY_CHAD_EXIT";
            break;
        case MQFB_STOPPED_BY_PUBSUB_EXIT:
            p = "MQFB_STOPPED_BY_PUBSUB_EXIT";
            break;
        case MQFB_NOT_A_REPOSITORY_MSG:
            p = "MQFB_NOT_A_REPOSITORY_MSG";
            break;
        case MQFB_BIND_OPEN_CLUSRCVR_DEL:
            p = "MQFB_BIND_OPEN_CLUSRCVR_DEL";
            break;
        case MQFB_DATA_LENGTH_ZERO:
            p = "MQFB_DATA_LENGTH_ZERO";
            break;
        case MQFB_DATA_LENGTH_NEGATIVE:
            p = "MQFB_DATA_LENGTH_NEGATIVE";
            break;
        case MQFB_DATA_LENGTH_TOO_BIG:
            p = "MQFB_DATA_LENGTH_TOO_BIG";
            break;
        case MQFB_BUFFER_OVERFLOW:
            p = "MQFB_BUFFER_OVERFLOW";
            break;
        case MQFB_LENGTH_OFF_BY_ONE:
            p = "MQFB_LENGTH_OFF_BY_ONE";
            break;
        case MQFB_IIH_ERROR:
            p = "MQFB_IIH_ERROR";
            break;
        case MQFB_NOT_AUTHORIZED_FOR_IMS:
            p = "MQFB_NOT_AUTHORIZED_FOR_IMS";
            break;
        case MQFB_IMS_ERROR:
            p = "MQFB_IMS_ERROR";
            break;
        case MQFB_IMS_FIRST:
            p = "MQFB_IMS_FIRST";
            break;
        case MQFB_IMS_LAST:
            p = "MQFB_IMS_LAST";
            break;
        case MQFB_CICS_INTERNAL_ERROR:
            p = "MQFB_CICS_INTERNAL_ERROR";
            break;
        case MQFB_CICS_NOT_AUTHORIZED:
            p = "MQFB_CICS_NOT_AUTHORIZED";
            break;
        case MQFB_CICS_BRIDGE_FAILURE:
            p = "MQFB_CICS_BRIDGE_FAILURE";
            break;
        case MQFB_CICS_CORREL_ID_ERROR:
            p = "MQFB_CICS_CORREL_ID_ERROR";
            break;
        case MQFB_CICS_CCSID_ERROR:
            p = "MQFB_CICS_CCSID_ERROR";
            break;
        case MQFB_CICS_ENCODING_ERROR:
            p = "MQFB_CICS_ENCODING_ERROR";
            break;
        case MQFB_CICS_CIH_ERROR:
            p = "MQFB_CICS_CIH_ERROR";
            break;
        case MQFB_CICS_UOW_ERROR:
            p = "MQFB_CICS_UOW_ERROR";
            break;
        case MQFB_CICS_COMMAREA_ERROR:
            p = "MQFB_CICS_COMMAREA_ERROR";
            break;
        case MQFB_CICS_APPL_NOT_STARTED:
            p = "MQFB_CICS_APPL_NOT_STARTED";
            break;
        case MQFB_CICS_APPL_ABENDED:
            p = "MQFB_CICS_APPL_ABENDED";
            break;
        case MQFB_CICS_DLQ_ERROR:
            p = "MQFB_CICS_DLQ_ERROR";
            break;
        case MQFB_CICS_UOW_BACKED_OUT:
            p = "MQFB_CICS_UOW_BACKED_OUT";
            break;
        case MQFB_ACTIVITY:
            p = "MQFB_ACTIVITY";
            break;
        case MQFB_MAX_ACTIVITIES:
            p = "MQFB_MAX_ACTIVITIES";
            break;
        case MQFB_NOT_FORWARDED:
            p = "MQFB_NOT_FORWARDED";
            break;
        case MQFB_NOT_DELIVERED:
            p = "MQFB_NOT_DELIVERED";
            break;
        case MQFB_UNSUPPORTED_FORWARDING:
            p = "MQFB_UNSUPPORTED_FORWARDING";
            break;
        case MQFB_UNSUPPORTED_DELIVERY:
            p = "MQFB_UNSUPPORTED_DELIVERY";
            break;
        default:
            sprintf(Buffer, "%d", Feedback);
            p = Buffer;
    }

    return p;
}
/*********************************************************************/
/*                                                                   */
/* Convert a ChannelType into a string                               */
/*                                                                   */
/*********************************************************************/
char *strChannelType(MQLONG Type, char *Buffer) {
    char *p = NULL;

    switch (Type) {
        case MQCHT_SENDER:
            p = "MQCHT_SENDER";
            break;
        case MQCHT_SERVER:
            p = "MQCHT_SERVER";
            break;
        case MQCHT_RECEIVER:
            p = "MQCHT_RECEIVER";
            break;
        case MQCHT_REQUESTER:
            p = "MQCHT_REQUESTER";
            break;
        case MQCHT_ALL:
            p = "MQCHT_ALL";
            break;
        case MQCHT_CLNTCONN:
            p = "MQCHT_CLNTCONN";
            break;
        case MQCHT_SVRCONN:
            p = "MQCHT_SVRCONN";
            break;
        case MQCHT_CLUSRCVR:
            p = "MQCHT_CLUSRCVR";
            break;
        case MQCHT_CLUSSDR:
            p = "MQCHT_CLUSSDR";
            break;
        case MQCHT_MQTT:
            p = "MQCHT_MQTT";
            break;
        default:
            sprintf(Buffer, "%d", Type);
            p = Buffer;
    }

    return p;
}


/*********************************************************************/
/*                                                                   */
/* Check to see if a string is blank                                 */
/*                                                                   */
/*********************************************************************/

MQLONG myBlankCheck(void *DataPointer, size_t DataLength) {
    size_t i = 0;
    MQLONG result = 1;
    MQLONG more = 1;
    char *string = DataPointer;

    for (i = 0; more && (i < DataLength); i++) {
        switch (string[i]) {
            case 0:
                more = 0;
                break;

            case ' ':
                break;

            default:
                result = 0;
                more = 0;
                break;
        }
    }

    return result;
}

MQLONG myCheckPointer(void *DataPointer) {
    return DataPointer ? 1 : 0;
}

void myGetAbsoluteTime(char *StringBuffer, size_t StringBufferLength) {
    time_t long_time;
    struct tm *newtime;

    time(&long_time);
    newtime = localtime(&long_time);

    strftime(StringBuffer, StringBufferLength, "%Y-%m-%d  %H:%M:%S", newtime);
}

void myGetRelativeTime(char *StringBuffer) {
    struct timeval t1;
    gettimeofday(&t1, NULL);
    sprintf(StringBuffer, "%ld.%06ld", t1.tv_sec, t1.tv_usec);
}

/*********************************************************************/
/*                                                                   */
/* Format a block of data into hex and dump to a file                */
/*                                                                   */
/* Offset    Data                                                    */
/* XXXXXXXX: XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX   ................  */
/* 0         1         2         3         4         5         6         7   */
/* 012345678901234567890123456789012345678901234567890123456789012345678901  */
/*                                                                   */
/*********************************************************************/

char HEX[] = "0123456789ABCDEF";

void DumpHex(FILE *fp, void *DataPointer, size_t DataLength
) {
    if (myCheckPointer(DataPointer)) {
        char StringBuffer[100];
        unsigned char *pd;
        unsigned char *pa;
        unsigned char *p = DataPointer;
        size_t PointerSize = sizeof(void *);
        size_t DataOffset = (PointerSize * 2) + 2;
        size_t AsciiOffset = DataOffset + 38;
        size_t LineLength = AsciiOffset + 16;
        size_t index, i;
        int n;

        sprintf(&StringBuffer[LineLength], "\n");

        index = 0;
        while (index < DataLength) {
            pd = (unsigned char *) StringBuffer + DataOffset;
            pa = (unsigned char *) StringBuffer + AsciiOffset;

            memset(StringBuffer, ' ', LineLength);
            n = sprintf(StringBuffer, "%p", p);
            StringBuffer[n] = ':';

            for (i = 0; (i < 16) && (index < DataLength); i++) {
                if ((i % 4) == 0) pd++;

                *pd++ = HEX[*p / 16];
                *pd++ = HEX[*p % 16];
                *pa++ = isprint(*p) ? *p : '.';

                index++;
                p++;
            }

            fprintf(fp, "          %s", StringBuffer);
        }
    }
}

/*********************************************************************/
/*                                                                   */
/* Write out hex numnber                                             */
/*                                                                   */
/*********************************************************************/
void fprint_hex(FILE *fp, char *name, MQLONG *pValue) {
    if (pValue) fprintf(fp, "  %14s: (%p) 0x%X\n", name, pValue, *pValue);
    else fprintf(fp, "  %14s: (NULL)\n", name);
}
/*********************************************************************/
/*                                                                   */
/* Write out hex numnber                                             */
/*                                                                   */
/*********************************************************************/
void fprint_phex(FILE *fp, char *name, MQLONG **pValue) {
    if (pValue) fprint_hex(fp, name, *pValue);
    else fprintf(fp, "  %14s: (NULL)\n", name);
}

MQ_BACK_EXIT BackAfter;

void MQENTRY BackAfter(PMQAXP pExitParms, PMQAXC pExitContext, PMQHCONN pHconn, PMQLONG pCompCode, PMQLONG pReason)
{
    syslog(LOG_INFO, "MQBACK");
    return;
}

MQ_BEGIN_EXIT BeginAfter;

void MQENTRY BeginAfter(PMQAXP pExitParms, PMQAXC pExitContext, PMQHCONN pHconn, PPMQBO ppBeginOptions, PMQLONG pCompCode, PMQLONG pReason)
{
    syslog(LOG_INFO, "MQBEGIN");
    return;
}

MQ_CLOSE_EXIT CloseAfter;

void MQENTRY CloseAfter(PMQAXP pExitParms, PMQAXC pExitContext, PMQHCONN pHconn, PPMQHOBJ ppHobj, PMQLONG pOptions, PMQLONG pCompCode, PMQLONG pReason)
{
    syslog(LOG_INFO, "MQCLOSE");
    return;
}

MQ_CMIT_EXIT CmitAfter;

void MQENTRY CmitAfter(PMQAXP pExitParms, PMQAXC pExitContext, PMQHCONN pHconn, PMQLONG pCompCode, PMQLONG pReason)
{
    syslog(LOG_INFO, "MQCMIT PID:%d", pExitContext -> ProcessId);
    return;
}

MQ_CONNX_EXIT ConnAfter;

void MQENTRY ConnAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        MQCHAR48 QMgrName,
        PPMQCNO ppConnectOpts,
        PPMQHCONN ppHconn,
        PMQLONG pCompCode,
        PMQLONG pReason) {

    char applicationType[64], userId[13], application[16], queueManager[50];

    syslog(LOG_INFO, "MQCONNX PID:%d, Caller:%s(%d), User:%s, AppName:%s, Qmrg:%s",
           pExitContext->ProcessId,
           strAPICallerType(pExitParms->APICallerType, applicationType),
           pExitParms->APICallerType,
           strSized(pExitContext->UserId, userId, 12),
           strSized(pExitContext->ApplName, application, 12),
           strSized(QMgrName, queueManager, 48)
    );
    return;
}

MQ_CONNX_EXIT ConnxAfter;

void MQENTRY ConnxAfter(PMQAXP pExitParms, PMQAXC pExitContext, MQCHAR48 QMgrName, PPMQCNO ppConnectOpts, PPMQHCONN ppHconn, PMQLONG pCompCode, PMQLONG pReason)
{
    char applicationType[64], userId[13], application[16], queueManager[50];

    syslog(LOG_INFO, "MQCONNX PID:%d, Caller:%s(%d), User:%s, AppName:%s, Qmrg:%s",
           pExitContext->ProcessId,
           strAPICallerType(pExitParms->APICallerType, applicationType),
           pExitParms->APICallerType,
           strSized(pExitContext->UserId, userId, 12),
           strSized(pExitContext->ApplName, application, 12),
           strSized(QMgrName, queueManager, 48)
    );

    return;
}

MQ_DISC_EXIT DiscAfter;

void MQENTRY DiscAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PPMQHCONN ppHconn,
        PMQLONG pCompCode,
        PMQLONG pReason) {
    syslog(LOG_INFO, "MQDISC PID:%d",
           pExitContext->ProcessId
    );
    return;
}

MQ_GET_EXIT GetAfter;

void MQENTRY GetAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQHOBJ pHobj,
        PPMQMD ppMsgDesc,
        PPMQGMO ppGetMsgOpts,
        PMQLONG pBufferLength,
        PPMQVOID ppBuffer,
        PPMQLONG ppDataLength,
        PMQLONG pCompCode,
        PMQLONG pReason) {


    // this is MQGET for empty queue
    if (strlen((*ppMsgDesc) -> MsgId) == 0)
        return;

    char resQName[50], userId[13], msgType[32];

    char *resolvedQName = (*ppGetMsgOpts)->ResolvedQName;
    if (excludeQueue(resolvedQName, "SYSTEM")) {
        syslog(LOG_INFO, "MQGET ResolvedQName:%s, UserId:%s, MsgId:%s, MsgType:%s",
               strSized(resolvedQName, resQName, 48),
               strSized(pExitContext->UserId, userId, 12),
               toHex24((*ppMsgDesc)->MsgId),
               strMsgType((*ppMsgDesc)->MsgType, msgType)
        );
    }

    return;
}


MQ_OPEN_EXIT OpenAfter;

void MQENTRY OpenAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PPMQOD ppObjDesc,
        PMQLONG pOptions,
        PPMQHOBJ ppHobj,
        PMQLONG pCompCode,
        PMQLONG pReason) {

    PMQOD pObjDesc = *ppObjDesc;
    char* objectName = pObjDesc -> ObjectName;

    if (excludeQueue(objectName, "SYSTEM") && !isAllWhitespace(objectName, 48)) {
        char objectNameTrim[50], queueManager[50];
        syslog(LOG_INFO, "MQOPEN ObjectName:%s, QM:%s",
               strSized(objectName, objectNameTrim, 48),
               strSized(pObjDesc -> ObjectQMgrName, queueManager, 48)
        );
    }

    return;
}

MQ_PUT_EXIT PutAfter;

void MQENTRY PutAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQHOBJ pHobj,
        PPMQMD ppMsgDesc,
        PPMQPMO ppPutMsgOpts,
        PMQLONG pBufferLength,
        PPMQVOID ppBuffer,
        PMQLONG pCompCode,
        PMQLONG pReason) {

    char *resolvedQName = (*ppPutMsgOpts)->ResolvedQName;
    char userId[13], msgType[32], resQName[50];
    if (excludeQueue(resolvedQName, "SYSTEM")) {
        syslog(LOG_INFO, "MQPUT UserId:%s, MsgId:%s, MsgType:%s, ResolvedQName:%s",
               strSized(pExitContext->UserId, userId, 12),
               toHex24((*ppMsgDesc)->MsgId),
               strMsgType((*ppMsgDesc)->MsgType, msgType),
               strSized(resolvedQName, resQName, 48)
        );
    }

    return;
}

MQ_PUT1_EXIT Put1After;

void MQENTRY Put1After(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PPMQOD ppObjDesc,
        PPMQMD ppMsgDesc,
        PPMQPMO ppPut1MsgOpts,
        PMQLONG pBufferLength,
        PPMQVOID ppBuffer,
        PMQLONG pCompCode,
        PMQLONG pReason) {

    char* objectName = (*ppObjDesc) ->ObjectName;
    char userId[13], msgType[32], resQName[50];
    if (excludeQueue(objectName, "SYSTEM")) {
        syslog(LOG_INFO, "MQPUT1 UserId:%s, MsgId:%s, MsgType:%s, ResolvedQName:%s",
               strSized(pExitContext->UserId, userId, 12),
               toHex24((*ppMsgDesc)->MsgId),
               strMsgType((*ppMsgDesc)->MsgType, msgType),
               strSized(objectName, resQName, 48)
        );
    }
    return;
}

XA_COMMIT_EXIT XACommitBefore;

void MQENTRY XACommitBefore(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQPTR ppXID,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}

XA_COMMIT_EXIT XACommitAfter;

void MQENTRY XACommitAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQPTR ppXID,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}


XA_OPEN_EXIT XAOpenBefore;

void MQENTRY XAOpenBefore(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PPMQCHAR ppXa_info,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}

XA_OPEN_EXIT XAOpenAfter;

void MQENTRY XAOpenAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PPMQCHAR ppXa_info,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}

XA_ROLLBACK_EXIT XARollbackBefore;

void MQENTRY XARollbackBefore(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQPTR ppXID,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}

XA_ROLLBACK_EXIT XARollbackAfter;

void MQENTRY XARollbackAfter(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQHCONN pHconn,
        PMQPTR ppXID,
        PMQLONG pRmid,
        PMQLONG pFlags,
        PMQLONG pXARetCode) {
    return;
}

MQ_TERM_EXIT Terminate;

void MQENTRY Terminate(
        PMQAXP pExitParms,
        PMQAXC pExitContext,
        PMQLONG pCompCode,
        PMQLONG pReason) {
    return;
}

MQ_INIT_EXIT EntryPoint;

void MQENTRY EntryPoint(PMQAXP pExitParms, PMQAXC pExitContext, PMQLONG pCompCode, PMQLONG pReason
) {
    MQLONG rc = MQRC_NONE;
    MYEXITUSERAREA **ppExitUserArea = (void *) &pExitParms->ExitUserArea;
    MYEXITUSERAREA *pExitUserArea = NULL;
    char *env = NULL;

    /*******************************************************************/
    /* Make sure that the Hconfig supplied contains the interface      */
    /* entry points.                                                   */
    /*******************************************************************/
    if (memcmp(pExitParms->Hconfig->StrucId, MQIEP_STRUC_ID, 4)) {
        rc = MQRC_API_EXIT_ERROR;
        pExitParms->ExitResponse = MQXCC_FAILED;
    }

    openlog("smsolog", LOG_PID, LOG_USER);
    //syslog(LOG_INFO, "SoLog entryPoint");

    /*******************************************************************/
    /* Malloc storage for the ExitUserArea                             */
    /*******************************************************************/

    if (rc == MQRC_NONE) {
        pExitUserArea = calloc(1, sizeof(MYEXITUSERAREA));

        if (pExitUserArea)
            *ppExitUserArea = pExitUserArea;
        else {
            rc = MQRC_API_EXIT_ERROR;
            pExitParms->ExitResponse = MQXCC_FAILED;
        }
    }

    /*******************************************************************/
    /* Write to the logfile                                            */
    /*******************************************************************/

    if (rc == MQRC_NONE) {
        /***************************************************************/
        /* Register the MQBACK entrypoints                             */
        /***************************************************************/
        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_BACK, (PMQFUNC) BackAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQBEGIN entrypoints                            */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_BEGIN, (PMQFUNC) BeginAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQCLOSE entrypoints                            */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_CLOSE, (PMQFUNC) CloseAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQCMIT entrypoints                             */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_CMIT, (PMQFUNC) CmitAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQCONN entrypoints                             */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_CONN, (PMQFUNC) ConnAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQCONNX entrypoints                            */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_CONNX, (PMQFUNC) ConnxAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQDISC entrypoints                             */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_DISC, (PMQFUNC) DiscAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQGET entrypoints                              */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_GET, (PMQFUNC) GetAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQOPEN entrypoints                             */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_OPEN, (PMQFUNC) OpenAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQPUT entrypoints                              */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_PUT, (PMQFUNC) PutAfter, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the MQPUT1 entrypoints                             */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_PUT1, (PMQFUNC) Put1After, NULL,
                                            pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the xa_commit entrypoints                          */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_BEFORE, MQXF_XACOMMIT,
                                            (PMQFUNC) XACommitBefore, NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_XACOMMIT, (PMQFUNC) XACommitAfter,
                                            NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the xa_open entrypoints                            */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_BEFORE, MQXF_XAOPEN, (PMQFUNC) XAOpenBefore,
                                            NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_XAOPEN, (PMQFUNC) XAOpenAfter,
                                            NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the xa_rollback entrypoints                        */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_BEFORE, MQXF_XAROLLBACK,
                                            (PMQFUNC) XARollbackBefore, NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_AFTER, MQXF_XAROLLBACK,
                                            (PMQFUNC) XARollbackAfter, NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }

        /***************************************************************/
        /* Register the termination entrypoint                         */
        /***************************************************************/

        if (rc == MQRC_NONE) {
            pExitParms->Hconfig->MQXEP_Call(pExitParms->Hconfig, MQXR_CONNECTION, MQXF_TERM, (PMQFUNC) Terminate,
                                            NULL, pCompCode, pReason
            );

            if (*pReason != MQRC_NONE) {
                rc = *pReason;
                pExitParms->ExitResponse = MQXCC_FAILED;
            }
        }
    }

    return;
}
