/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot MEMUSE or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to MEMUSE the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     QosPolicy.h
*  @brief    
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/07/29
*  @license  GNU General Public License (GPL)
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/07/29 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

/********************************************************************
QosPolicy.h
Qos相关结构体定义
********************************************************************/

#ifndef HIPERD_QOSPOLICY_H
#define HIPERD_QOSPOLICY_H  

#define LENGTH_UNLIMITED -1

/*******************************************************************/
/* Qos 相关数据结构定义 */
/*******************************************************************/

typedef struct  _Property
{
    UINT32 netcard_index;
    CHAR ipAddr[16];
	CHAR netCardName[256];

}Property;

typedef enum _DurabilityQosPolicyKind {
    VOLATILE_DURABILITY_QOS         ,    //不保存数据
    TRANSIENT_LOCAL_DURABILITY_QOS  ,    //当reader存在时保存数据
    TRANSIENT_DURABILITY_QOS        ,    //将数据保存在内存中
    PERSISTENT_DURABILITY_QOS            //将数据保存到硬盘上
}DurabilityQosPolicyKind;
typedef struct _DurabilityQosPolicy
{
    DurabilityQosPolicyKind kind    ;
}DurabilityQosPolicy;
/*******************************************************************/
typedef enum _HistoryQosPolicyKind{
    KEEP_LAST_HISTORY_QOS,
    KEEP_ALL_HISTORY_QOS
}HistoryQosPolicyKind;
typedef struct _DurabilityServiceQosPolicy
{
    Duration_t              service_cleanup_delay   ;
    HistoryQosPolicyKind    history_kind            ;
    EX_INT32                history_depth           ;
    EX_INT32                max_samples             ;
    EX_INT32                max_instances           ;
    EX_INT32                max_samples_per_instance;
}DurabilityServiceQosPolicy;
/*******************************************************************/
typedef struct _DeadlineQosPolicy
{
    Duration_t  period  ;
}DeadlineQosPolicy;
/*******************************************************************/
typedef struct _LatencyBudgetQosPolicy
{
    Duration_t  duration;
}LatencyBudgetQosPolicy;
/*******************************************************************/
typedef enum _LivelinessQosPolicyKind{
    AUTOMATIC_LIVELINESS_QOS            ,
    MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
    MANUAL_BY_TOPIC_LIVELINESS_QOS
}LivelinessQosPolicyKind;
typedef struct _LivelinessQosPolicy
{
    LivelinessQosPolicyKind kind            ;
    Duration_t              lease_duration  ;
}LivelinessQosPolicy;
/*******************************************************************/
typedef enum ReliabilityQosPolicyKind {
    BEST_EFFORT_RELIABILITY_QOS = 1,
    RELIABLE_RELIABILITY_QOS    = 2
}ReliabilityQosPolicyKind;
typedef struct _ReliabilityQosPolicy
{
    ReliabilityQosPolicyKind    kind                ;
    Duration_t                  max_blocking_time   ;
}ReliabilityQosPolicy;
/*******************************************************************/
/*QY_DDS用于控制可靠传输相关方式的扩展QoS*/
typedef struct _QY_CONTROL_QOS
{
    UINT	pack_per_herat;     //每多少包数据后会发送一个心跳
    UINT	period_per_heart;   //每隔一段时间检测writer的历史缓存，若缓存不为0则补发心跳
}QY_CONTROL_QOS;

typedef struct _ControllerQosPolicy
{
    QY_CONTROL_QOS controller;
}ControllerQosPolicy;
/*******************************************************************/
typedef struct _LifespanQosPolicy
{
    Duration_t  duration;
}LifespanQosPolicy;
/*******************************************************************/
typedef enum _OwnershipQosPolicyKind{
    SHARED_OWNERSHIP_QOS,
    EXCLUSIVE_OWNERSHIP_QOS
}OwnershipQosPolicyKind;
typedef struct OwnershipQosPolicy
{
    OwnershipQosPolicyKind  kind    ;
}OwnershipQosPolicy;
/*******************************************************************/
typedef struct _OwnershipStrengthQosPolicy
{
    EX_INT32    value   ;
}OwnershipStrengthQosPolicy;
/*******************************************************************/
typedef enum _DestinationOrderQosPolicyKind {
    BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
    BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
}DestinationOrderQosPolicyKind;
typedef struct _DestinationOrderQosPolicy
{
    DestinationOrderQosPolicyKind   kind    ;
}DestinationOrderQosPolicy;
/*******************************************************************/
typedef enum _PresentationQosPolicyAccessScopeKind{
    INSTANCE_PRESENTATION_QOS   ,
    TOPIC_PRESENTATION_QOS      ,
    GROUP_PRESENTATION_QOS
}PresentationQosPolicyAccessScopeKind;
typedef struct _PresentationQosPolicy
{
    PresentationQosPolicyAccessScopeKind access_scope   ;
    BOOL  coherent_access                         ;
    BOOL  ordered_access                          ;
}PresentationQosPolicy;
/*******************************************************************/
typedef struct _PartitionName
{
    BoundedString256 partitionValue;
    struct _PartitionName* pNext;
}PartitionName;
typedef struct _PartitionQosPolicy
{
    PartitionName*    pValue;
    EX_INT32          size;
}PartitionQosPolicy;

/********************************************************************
DCPS规范中UserDataQosPolicy对应的数据类型及相关处理机制
********************************************************************/
typedef struct _UserDataQosPolicy
{
	BoundedString256   value;

}UserDataQosPolicy;
/*
以下为该结构编解码工具宏
*/
#define SERIALIZE_USER_DATA_QOS_POLICY(locator , memBlock)                 \
    do                                                                     \
			{                                                                      \
        SERIALIZE_BOUNDED_STRING(locator.value, memBlock)                  \
			} while (0);


#define DESERIALIZE_USER_DATA_QOS_POLICY(locator , memBlock,exchange)      \
    do                                                                     \
			{                                                                      \
        DESERIALIZE_BOUNDED_STRING(locator.value, memBlock,exchange)       \
			} while (0);
/*******************************************************************/
typedef struct  _TopicDataQosPolicy
{
    BoundedString256    value   ;
}TopicDataQosPolicy;
/*******************************************************************/
typedef struct  _GroupDataQosPolicy
{
    BoundedString256    value   ;
}GroupDataQosPolicy;
/*******************************************************************/
typedef struct _TimeBasedFilterQosPolicy
{
    Duration_t  minimum_separation  ;
}TimeBasedFilterQosPolicy;
/*******************************************************************/
typedef struct _EntityFactoryQosPolicy
{
    BOOL  autoenable_created_entities ;
}EntityFactoryQosPolicy;
/*******************************************************************/
typedef struct _HistoryQosPolicy
{
    HistoryQosPolicyKind    kind    ;
    EX_INT32                depth   ;
}HistoryQosPolicy;
/*******************************************************************/
typedef struct _ResourceLimitsQosPolicy
{
    EX_INT32    max_samples             ;
    EX_INT32    max_instances           ;
    EX_INT32    max_samples_per_instance;
}ResourceLimitsQosPolicy;
/*******************************************************************/
typedef struct _TransportPriorityQosPolicy{
    EX_INT32    transport_priority  ;
}TransportPriorityQosPolicy;
/*******************************************************************/
typedef struct _WriterDataLifecycleQosPolicy
{
    BOOL  autodispose_unregistered_instances  ;
}WriterDataLifecycleQosPolicy;
/*******************************************************************/
typedef struct _ReaderDataLifecycleQosPolicy
{
    char        name[16];
    Duration_t  autopurge_nowriter_samples_delay;
    Duration_t  autopurge_disposed_samples_delay;
}ReaderDataLifecycleQosPolicy;
/*******************************************************************/
typedef struct  _DomainParticipantQos
{
    BoundedString256   value;
    Property property;
	LivelinessQosPolicy         liveliness; //LJM，规范中存活性QoS只配置在Topic\DW\DR中，DP中不含存活性QoS怎么判断失活呢：磐优参考实现是用定时器2s检测，三次没收到spdp报文则认为失活
}DDS_DomainParticipantQos;

/*******************************************************************/
typedef struct _DataWriterQos
{
    DurabilityQosPolicy         durability;
    DurabilityServiceQosPolicy  durability_service;
    DeadlineQosPolicy           deadline;
    LatencyBudgetQosPolicy      latency_budget;
    LivelinessQosPolicy         liveliness;
    ReliabilityQosPolicy        reliability;
    ControllerQosPolicy         controller;
    DestinationOrderQosPolicy   destination_order;
    HistoryQosPolicy            history;
    ResourceLimitsQosPolicy     resource_limits;
    TransportPriorityQosPolicy  transport_priority;
    LifespanQosPolicy           lifespan;
    UserDataQosPolicy           user_data;
    OwnershipQosPolicy          ownership;
    OwnershipStrengthQosPolicy  ownership_strength;
    WriterDataLifecycleQosPolicy writer_data_lifecycle;
}DDS_DataWriterQos;

/*******************************************************************/
typedef struct _DataReaderQos
{
    DurabilityQosPolicy         durability;
    DeadlineQosPolicy           deadline;
    LatencyBudgetQosPolicy      latency_budget;
    LivelinessQosPolicy         liveliness;
    ReliabilityQosPolicy        reliability;
    DestinationOrderQosPolicy   destination_order;
    HistoryQosPolicy            history;
    ResourceLimitsQosPolicy     resource_limits;
    UserDataQosPolicy           user_data;
    OwnershipQosPolicy          ownership;
    TimeBasedFilterQosPolicy    time_based_filter;
    ReaderDataLifecycleQosPolicy reader_data_lifecycle;
}DDS_DataReaderQos;
/*******************************************************************/
typedef struct _TopicQos
{
    TopicDataQosPolicy          topic_data;
    DurabilityQosPolicy         durability;
    DurabilityServiceQosPolicy  durability_service;
    DeadlineQosPolicy           deadline;
    LatencyBudgetQosPolicy      latency_budget;
    LivelinessQosPolicy         liveliness;
    ReliabilityQosPolicy        reliability;
    DestinationOrderQosPolicy   destination_order;
    HistoryQosPolicy            history;
    ResourceLimitsQosPolicy     resource_limits;
    TransportPriorityQosPolicy  transport_priority;
    LifespanQosPolicy           lifespan;
    OwnershipQosPolicy          ownership;

	/* 主题特有的QoS */
    PresentationQosPolicy       presentation;
    PartitionQosPolicy          partition;
    GroupDataQosPolicy          group_data;
    EntityFactoryQosPolicy      entity_factory;
}DDS_TopicQos;

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/
#define SERIALIZE_DURABILITY_QOS_POLICY(locator , memBlock)                 \
    do                                                                      \
    {                                                                       \
        PUT_INT(memBlock, locator.kind)                                     \
    } while (0);

#define DESERIALIZE_DURABILITY_QOS_POLICY(locator , memBlock,exchange)               \
    do                                                                      \
    {                                                                       \
        GET_INT(memBlock, locator.kind,exchange)                                     \
    } while (0);

#define SERIALIZE_DURABILITY_SERVICE_QOS_POLICY(locator , memBlock)         \
    do                                                                      \
    {                                                                       \
        PUT_INT(memBlock, locator.service_cleanup_delay.sec)                \
        PUT_INT(memBlock, locator.service_cleanup_delay.nanosec)            \
        PUT_INT(memBlock, locator.history_kind)                             \
        PUT_INT(memBlock, locator.history_depth)                            \
        PUT_INT(memBlock, locator.max_samples)                              \
        PUT_INT(memBlock, locator.max_instances)                            \
        PUT_INT(memBlock, locator.max_samples_per_instance)                 \
    } while (0);

#define DESERIALIZE_DURABILITY_SERVICE_QOS_POLICY(locator , memBlock,exchange)       \
    do                                                                      \
    {                                                                       \
        GET_INT(memBlock, locator.service_cleanup_delay.sec,exchange)                \
        GET_INT(memBlock, locator.service_cleanup_delay.nanosec,exchange)            \
        GET_INT(memBlock, locator.history_kind,exchange)                             \
        GET_INT(memBlock, locator.history_depth,exchange)                            \
        GET_INT(memBlock, locator.max_samples,exchange)                              \
        GET_INT(memBlock, locator.max_instances,exchange)                            \
        GET_INT(memBlock, locator.max_samples_per_instance,exchange)                 \
    } while (0);

#define SERIALIZE_CONTROLLER_QOS_POLICY(locator , memBlock)         \
    do                                                                      \
    {                                                                       \
        PUT_UNSIGNED_INT(memBlock, locator.controller.pack_per_herat)       \
        PUT_UNSIGNED_INT(memBlock, locator.controller.period_per_heart)     \
    } while (0);

#define DESERIALIZE_CONTROLLER_QOS_POLICY(locator , memBlock,exchange)         \
    do                                                                      \
    {                                                                       \
        GET_UNSIGNED_INT(memBlock, locator.controller.pack_per_herat,exchange)                  \
        GET_UNSIGNED_INT(memBlock, locator.controller.period_per_heart,exchange)                \
    } while (0);

#define SERIALIZE_DEADLINE_QOS_POLICY(locator , memBlock)                  \
    do                                                                     \
    {                                                                      \
        SERIALIZE_DURATION(locator.period, memBlock)                       \
    } while (0);

#define DESERIALIZE_DEADLINE_QOS_POLICY(locator , memBlock,exchange)                \
    do                                                                     \
    {                                                                      \
        DESERIALIZE_DURATION(locator.period, memBlock,exchange)                     \
    } while (0);

#define SERIALIZE_LATENCY_BUDGET_QOS_POLICY(locator , memBlock)            \
    do                                                                     \
    {                                                                      \
        SERIALIZE_DURATION(locator.duration, memBlock)                     \
    } while (0);

#define DESERIALIZE_LATENCY_BUDGET_QOS_POLICY(locator , memBlock,exchange)          \
    do                                                                     \
    {                                                                      \
        DESERIALIZE_DURATION(locator.duration, memBlock,exchange)                   \
    } while (0);

#define SERIALIZE_LIVELINESS_QOS_POLICY(locator , memBlock)                \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.kind)                                    \
        SERIALIZE_DURATION(locator.lease_duration, memBlock)               \
    } while (0);

#define DESERIALIZE_LIVELINESS_QOS_POLICY(locator , memBlock,exchange)              \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.kind,exchange)                                    \
        DESERIALIZE_DURATION(locator.lease_duration, memBlock,exchange)             \
    } while (0);

#define SERIALIZE_RELIABILITY_QOS_POLICY(locator , memBlock)               \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.kind)                                    \
        SERIALIZE_DURATION(locator.max_blocking_time, memBlock)            \
    } while (0);

#define DESERIALIZE_RELIABILITY_QOS_POLICY(locator , memBlock,exchange)             \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.kind,exchange)                                    \
        DESERIALIZE_DURATION(locator.max_blocking_time, memBlock,exchange)          \
    } while (0);

#define SERIALIZE_HISTORY_QOS_POLICY(locator , memBlock)                  \
    do                                                                     \
	{                                                                      \
		PUT_INT(memBlock, locator.kind)                            \
		PUT_INT(memBlock, locator.depth)                        \
	} while (0);

#define DESERIALIZE_HISTORY_QOS_POLICY(locator , memBlock,exchange)                \
    do                                                                     \
	{                                                                      \
		GET_INT(memBlock, locator.kind,exchange)                            \
		GET_INT(memBlock, locator.depth,exchange)                        \
	} while (0);

#define SERIALIZE_LIFESPAN_QOS_POLICY(locator , memBlock)                  \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.duration.sec)                            \
        PUT_INT(memBlock, locator.duration.nanosec)                        \
    } while (0);

#define DESERIALIZE_LIFESPAN_QOS_POLICY(locator , memBlock,exchange)                \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.duration.sec,exchange)                            \
        GET_INT(memBlock, locator.duration.nanosec,exchange)                        \
    } while (0);
        
#define SERIALIZE_OWNERSHIP_QOS_POLICY(locator , memBlock)                 \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.kind)                                    \
    } while (0);

#define DESERIALIZE_OWNERSHIP_QOS_POLICY(locator , memBlock,exchange)               \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.kind,exchange)                                    \
    } while (0);

#define SERIALIZE_OWNERSHIP_STRENGTH_QOS_POLICY(locator , memBlock)        \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.value)                                   \
    } while (0);

#define DESERIALIZE_OWNERSHIP_STRENGTH_QOS_POLICY(locator , memBlock,exchange)      \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.value,exchange)                                   \
    } while (0);

#define SERIALIZE_DESTINATION_ORDER_QOS_POLICY(locator , memBlock)         \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.kind)                                    \
    } while (0);

#define DESERIALIZE_DESTINATION_ORDER_QOS_POLICY(locator , memBlock,exchange)       \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.kind,exchange)                                    \
    } while (0);

#define SERIALIZE_PRESENTATION_QOS_POLICY(locator , memBlock)              \
    do                                                                     \
    {                                                                      \
        PUT_INT(memBlock, locator.access_scope)                            \
        PUT_BYTE(memBlock, locator.coherent_access)                        \
        PUT_BYTE(memBlock, locator.ordered_access)                         \
    } while (0);

#define DESERIALIZE_PRESENTATION_QOS_POLICY(locator , memBlock,exchange)            \
    do                                                                     \
    {                                                                      \
        GET_INT(memBlock, locator.access_scope,exchange)                            \
        GET_BYTE(memBlock, locator.coherent_access)                        \
        GET_BYTE(memBlock, locator.ordered_access)                         \
    } while (0);

#define SERIALIZE_PARTITION_QOS_POLICY(locator , memBlock)                 \
    do                                                                     \
    {                                                                      \
        SERIALIZE_BOUNDED_STRING(locator.value, memBlock)                  \
    } while (0);

#define DESERIALIZE_PARTITION_QOS_POLICY(locator , memBlock,exchange)               \
    do                                                                     \
    {                                                                      \
	    int i = 0 ;                                                        \
        GET_INT(memBlock, locator.size,exchange);                                   \
        for(i = 0 ; i < locator.size ; i ++)                               \
        {                                                                  \
            PartitionName* pstPartitionName = (PartitionName*)DDS_STATIC_MALLOC(sizeof(PartitionName));  \
            if (NULL == pstPartitionName)                                                                \
                return ;                                                                                 \
            pstPartitionName->pNext = NULL;                                                             \
            DESERIALIZE_BOUNDED_STRING(pstPartitionName->partitionValue, memBlock,exchange)                       \
            pstPartitionName->partitionValue.length = strlen(pstPartitionName->partitionValue.value) + 1; \
            LIST_INSERT_HEAD(locator.pValue, pstPartitionName);                                          \
        }                                                                                                \
    } while (0);

#define SERIALIZE_TOPIC_DATA_QOS_POLICY(locator , memBlock)                 \
    do                                                                      \
    {                                                                       \
        SERIALIZE_BOUNDED_STRING(locator.value, memBlock)                   \
    } while (0);

#define DESERIALIZE_TOPIC_DATA_QOS_POLICY(locator , memBlock,exchange)               \
    do                                                                      \
    {                                                                       \
        DESERIALIZE_BOUNDED_STRING(locator.value, memBlock,exchange)                 \
    } while (0);

#define SERIALIZE_GROUP_DATA_QOS_POLICY(locator , memBlock)                 \
    do                                                                      \
    {                                                                       \
        SERIALIZE_BOUNDED_STRING(locator.value, memBlock)                   \
    } while (0);

#define DESERIALIZE_GROUP_DATA_QOS_POLICY(locator , memBlock,exchange)               \
    do                                                                      \
    {                                                                       \
        DESERIALIZE_BOUNDED_STRING(locator.value, memBlock,exchange)                 \
    } while (0);

#define SERIALIZE_TIME_BASED_FILTER_QOS_POLICY(locator , memBlock)          \
    do                                                                      \
    {                                                                       \
        SERIALIZE_DURATION(locator.minimum_separation, memBlock)            \
    } while (0);

#define DESERIALIZE_TIME_BASED_FILTER_QOS_POLICY(locator , memBlock,exchange)        \
    do                                                                      \
    {                                                                       \
        DESERIALIZE_DURATION(locator.minimum_separation, memBlock,exchange)          \
    } while (0);

#endif
