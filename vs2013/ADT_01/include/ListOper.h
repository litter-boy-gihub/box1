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
*  @file     ListOper.h
*  @brief    ListOper
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

#ifndef __LIST_OPER_H_INCLUDED__
#define __LIST_OPER_H_INCLUDED__

#define   FUNC_NAME    __FUNCTION__ //(__func__)

/* ָ��У��� */
//���޷���ֵ��retVal��RETURN_VOID
#define RETURN_VOID
#define CHECK_SINGLE_POINTER(ptr1, retVal) do{\
    if(NULL == (ptr1)) \
    {   \
        \
            printf("[%s(%d)]Null Pointer: "#ptr1"!\n\r", FUNC_NAME, __LINE__); \
            return retVal; \
    } \
}while (0)

#define CHECK_DOUBLE_POINTER(ptr1, ptr2, retVal) do{\
    if((NULL == (ptr1)) || (NULL == (ptr2))) \
        { \
        printf("[%s(%d)]Null Pointer: "#ptr1"(%p), "#ptr2"(%p)!\n\r", FUNC_NAME, __LINE__, ptr1, ptr2); \
        return retVal; \
        } \
        }while(0)

#define CHECK_TRIPLE_POINTER(ptr1, ptr2, ptr3, retVal) do{\
    if((NULL == (ptr1)) || (NULL == (ptr2)) || (NULL == (ptr3))) \
        { \
        printf("[%s(%d)]Null Pointer: "#ptr1"(%p), "#ptr2"(%p), "#ptr3"(%p)!\n\r", FUNC_NAME, __LINE__, ptr1, ptr2, ptr3); \
        return retVal; \
        } \
        }while(0)

// �������Ϊ��Ϊ����ͷ������˫��ѭ��������
#define LIST_INIT_NODE(pNode) do{              \
    (pNode)->pNext = (pNode)->pPrev = (pNode); \
        }while(0)

//"����"�����㣬����ͨ���ý�������ǰ���ͺ�̽��(������������)
#define LIST_ISOL_NODE(pNode) do{           \
    (pNode)->pNext = (pNode)->pPrev = NULL; \
        }while(0)

//�ж������Ƿ����ͷ���
#define LIST_WITH_HEAD(pHeadNode) do{                                           \
    (((pHeadNode)->pPrev == (pHeadNode)) && ((pHeadNode->pNext == pHeadNode))); \
        }while(0)

//����������
#define LIST_INSERT_NODE(prevNode, insertNode) do{ \
    (insertNode)->pNext      = (prevNode)->pNext;  \
    (insertNode)->pPrev      = (prevNode);         \
    (prevNode)->pNext->pPrev = (insertNode);       \
    (prevNode)->pNext        = (insertNode);       \
        }while(0)

//ɾ��˫������
#define LIST_REMOVE_NODE(removeNode) do{ \
    (removeNode)->pPrev->pNext = (removeNode)->pNext;  \
    (removeNode)->pNext->pPrev = (removeNode)->pPrev;  \
        }while(0)

//��ȡ�����㼰������(������ȫ�Լ��)
#define GET_NODE_NUM(pList)      ((pList)->dwNodeNum)
#define GET_HEAD_NODE(pList)     ((pList)->pHead)
#define GET_TAIL_NODE(pList)     ((pList)->pTail)
#define GET_PREV_NODE(pNode)     ((pNode)->pPrev)
#define GET_NEXT_NODE(pNode)     ((pNode)->pNext)
#define GET_NODE_DATA(pNode)     ((pNode)->pvNodeData)

//˫��ѭ���������У���
#define LIST_ITER_CHECK(pList, retVal) do{               \
    CHECK_SINGLE_POINTER((pList), retVal);               \
    CHECK_SINGLE_POINTER((pList)->pHead, retVal);        \
    CHECK_SINGLE_POINTER((pList)->pHead->pNext, retVal); \
        }while(0)

//˫��ѭ�����������
//pList: ����ָ�룻pLoopNode: �����㣬����ѭ����������
//pTmpNode: �����㣬����ɾ��pLoopNodeʱ��ʱ����pLoopNode->pNext
//ĳЩ����²��ñ��������OmciLocateListNode��OmciTraverseListNode���������ִ��Ч�ʡ�
//���ⲿ���ݺͽ�������谴��ͬ�Ĺ���ת��ʱ�����ñ������ʹ�ⲿ���ݲ����ظ�ת����
#define LIST_ITER_LOOP(pList, pLoopNode) \
  for(pLoopNode = (pList)->pHead->pNext; \
      pLoopNode != (pList)->pHead;       \
      pLoopNode = pLoopNode->pNext)

#define LIST_ITER_LOOP_SAFE(pList, pLoopNode, pTmpNode)               \
  for(pLoopNode = (pList)->pHead->pNext, pTmpNode = pLoopNode->pNext; \
      pLoopNode != (pList)->pHead;                                    \
      pLoopNode = pTmpNode, pTmpNode = pLoopNode->pNext)

//��ȫ�ͷ��ڴ�
#define SAFE_FREE(pointer) do{ \
    if(pointer != NULL){         \
        free(pointer);           \
        pointer = NULL;          \
    }while(0);

//������ͷ��
#define LIST_INSERT_HEAD(headNode, insertNode)      \
    do{                                             \
        insertNode->pNext = headNode;                \
        headNode = insertNode;                        \
    }while(0)

//������β��
#define LIST_INSERT_TAIL(headNode, insertNode, tempNode)          \
    do{                                                           \
        tempNode = headNode;                                      \
        insertNode->pNext = NULL;                                 \
        if(NULL == tempNode)                                      \
        {                                                         \
            headNode = insertNode;                                \
        }                                                         \
        else                                                      \
        {                                                          \
            while (NULL != tempNode->pNext)                       \
            {                                                     \
                tempNode = tempNode->pNext;                       \
            }                                                     \
            tempNode->pNext = insertNode;                         \
        }                                                          \
    }while(0)

//������ɾ��
#define LIST_DELETE_ALL(headNode, tempNode)                       \
    do{                                                           \
        while (NULL != headNode)                                  \
        {                                                         \
            tempNode = headNode;                                  \
            headNode = headNode->pNext;                           \
            tempNode->pNext = NULL;                               \
            DDS_STATIC_FREE(tempNode);                            \
        }                                                         \
    }while(0)

//������ɾ��
#define LIST_DELETE_GUID(headNode, tempNode)                      \
    do{                                                           \
        while (NULL != headNode)                                  \
        {                                                         \
            tempNode = headNode;                                  \
            headNode = headNode->pNext;                           \
            tempNode->pNext = NULL;                               \
            DDS_STATIC_FREE(tempNode);                            \
        }                                                         \
    }while(0)

//����ͷ����β������β��
#define DOUBLE_INSERT_TAIL(headNode, tailNode, insertNode)  \
    do{                                                 \
        if(NULL == headNode || NULL == tailNode)        \
        {                                               \
            headNode = insertNode;                      \
            tailNode = insertNode;                      \
        }                                               \
        else                                            \
        {                                               \
            tailNode->pNext = insertNode;               \
			tailNode = insertNode;                      \
	    }                                               \
        insertNode->pNext =NULL;                        \
    }while(0)                                           

#endif

