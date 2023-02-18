#include "../include/GlobalDefine.h"

SequenceNumber_t SEQUENCENUMBER_UNKNOWN = { -1, 0 };    // RTPS������SequenceNumber_tֵ
SequenceNumber_t SEQUENCENUMBER_MINIMUM = { -1, 0 };    // ��С��SequenceNumber_tֵ
SequenceNumber_t SEQUENCENUMBER_MAXIMUM = { 0x7fffffff, 0xffffffff };    // ��С��SequenceNumber_tֵ
SequenceNumber_t SEQUENCENUMBER_START = { 0, 1 }; //LJM���ǲ���Ӧ�øĳ�step����ʾÿ�μ�1?
SequenceNumber_t SEQUENCENUMBER_ZERO = { 0, 0 };

static SequenceNumber_t SEQUENCENUMBER_PUB_WRITER = { 0, 0 };  /* ����д����ʱseqNum */
static SequenceNumber_t SEQUENCENUMBER_SUB_READER = { 0, 0 };  /* ������ȡ��ʱseqNum */

/* �Ӽ����жϴ�С���� */
SequenceNumber_t Add_SequenceNumber(SequenceNumber_t left, SequenceNumber_t right)
{
	SequenceNumber_t result;
    unsigned int resultLow = left.low + right.low;
    unsigned int resultHigh = left.high + right.high + (resultLow < left.low ? 1 : 0);
	result.high = resultHigh;
	result.low = resultLow;
    return result;
}
SequenceNumber_t Sub_SequenceNumber(SequenceNumber_t left, SequenceNumber_t right)
{
	SequenceNumber_t result;
	unsigned int resultLow = (left.low >= right.low) ? (left.low - right.low) : (right.low - left.low);
    int resultHigh = left.low >= right.low ? left.high : left.high - 1;
	resultHigh = (left.high >= right.high) ? (left.high - right.high) : (-(right.high - left.high));
	result.high = resultHigh;
	result.low = resultLow;
    return result;
}

SequenceNumber_t GetPubWriterSequenceNumber()
{
    SEQUENCENUMBER_PUB_WRITER = Add_SequenceNumber(SEQUENCENUMBER_PUB_WRITER, SEQUENCENUMBER_START);
    return SEQUENCENUMBER_PUB_WRITER;
}

SequenceNumber_t GetSubReaderSequenceNumber()
{
    SEQUENCENUMBER_SUB_READER = Add_SequenceNumber(SEQUENCENUMBER_SUB_READER, SEQUENCENUMBER_START);
    return SEQUENCENUMBER_SUB_READER;
}