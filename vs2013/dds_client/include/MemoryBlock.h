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
*  @file     MemoryBlock.h
*  @brief    MemoryBlock
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

/*
MemoryBlock.h
MemoryBlock�ṹ����ض��弰������
*/

#ifndef HIPERD_MEMORYBLOCK_H
#define HIPERD_MEMORYBLOCK_H

typedef struct _MemoryBlock
{
    char *          base        ; /* �ڴ���׵�ַ                                           */
    unsigned int    totalSize   ; /* �ڴ���ܴ�С                                           */
    unsigned int    readIndex   ; /* ��ָ��                                                 */
    char *          rdPtr       ;
    unsigned int    writeIndex  ; /* дָ��                                                 */
    char *          wrPtr       ;
}MemoryBlock;

#define INITIAL_MEMORYBLOCK(memBlock, basePtr, totalsize)   \
    do                                                      \
    {                                                       \
        memBlock.base = basePtr;                            \
        memBlock.totalSize = totalsize;                     \
        memBlock.readIndex = 0;                             \
        memBlock.rdPtr = basePtr;                           \
        memBlock.writeIndex = 0;                            \
        memBlock.wrPtr = basePtr;                           \
    } while (0);
//��ʼ�����ڴ��
#define INITIAL_READ_MEMORYBLOCK(memBlock, basePtr, totalsize)   \
    do                                                      \
    {                                                       \
        memBlock.base = basePtr;                            \
        memBlock.totalSize = totalsize;                     \
        memBlock.readIndex = 0;                             \
        memBlock.rdPtr = basePtr;                           \
        memBlock.writeIndex = totalsize;                    \
        memBlock.wrPtr = basePtr + totalsize;               \
    } while (0);
//��ָ������ƫ��length������������length
#define MEMORYBLOCK_SKIP_READER(memBlock, length)           \
    do                                                      \
    {                                                       \
        memBlock.readIndex += length;                       \
        memBlock.rdPtr += length;                           \
     } while (0);

//дָ������ƫ��length��д��������length ???��ʲô���壿
#define MEMORYBLOCK_SKIP_WRITE(memBlock, length)            \
    do                                                      \
    {                                                       \
        memBlock.writeIndex += length;                      \
        memBlock.wrPtr += length;                           \
    } while (0);
#endif