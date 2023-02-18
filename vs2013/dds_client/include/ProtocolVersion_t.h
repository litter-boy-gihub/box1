/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot use or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to use the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     ProtocolVersion_t.h
*  @brief    ProtocolVersion_t
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/08
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/08 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

/********************************************************************
ProtocolVersion_t.h
RTPS�淶��ProtocolVersion_t��Ӧ���������ͼ���ش������
********************************************************************/

#ifndef HIPERD_PROTOCOLVERSION_T_H
#define HIPERD_PROTOCOLVERSION_T_H

/* ProtocolVersion_t �ṹ�����л���Parameterʱռ�ÿռ�Ĵ�С ��Ҫ�����ֽڶ���*/
#define SIZE_OF_PROTOCOL_VERSION 2

typedef struct _ProtocolVersion_t
{
    EX_UINT8 major; /* ���汾�� */
    EX_UINT8 minor; /* ���汾�� */
}ProtocolVersion_t;

/* ������ProtocolVersion_tֵ */
extern ProtocolVersion_t PROTOCOLVERSION_1_0;    /* 1.0�汾                            */
extern ProtocolVersion_t PROTOCOLVERSION_1_1;    /* 1.1�汾                            */
extern ProtocolVersion_t PROTOCOLVERSION_2_0;    /* 2.0�汾                            */
extern ProtocolVersion_t PROTOCOLVERSION_2_1;    /* 2.1�汾                            */
extern ProtocolVersion_t PROTOCOLVERSION;    /* PROTOCOLVERSION�����°汾�ı���    */

#define VERSION_IS_EQUAL(leftVersion , rightVersion) \
    (## leftVersion ##.major == ## rightVersion ##.major) && \
    (## leftVersion ##.minor == ## rightVersion ##.minor)

/* ����Ϊ�ýṹ����빤�ߺ� */
#define SERIALIZE_PROTOCOL_VERSION(version,memBlock)     \
    do                                                   \
        {                                                \
        PUT_CHAR(memBlock, version.major)                \
        PUT_CHAR(memBlock, version.minor)                \
    } while (0);

#define DESERIALIZE_PROTOCOL_VERSION(version,memBlock)   \
    do                                                   \
    {                                                    \
        GET_CHAR(memBlock, version.major)                \
        GET_CHAR(memBlock, version.minor)                \
    } while (0);

#endif