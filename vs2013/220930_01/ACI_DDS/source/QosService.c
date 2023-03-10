#include "../include/GlobalDefine.h"

DDS_DLL QosService g_default_Qos;
DDS_DLL DDS_DomainParticipantQos       DDS_PARTICIPANT_QOS_DEFAULT;
DDS_DLL DDS_TopicQos                   DDS_TOPIC_QOS_DEFAULT;
DDS_DLL DDS_DataWriterQos              DDS_DATAWRITER_QOS_DEFAULT;
DDS_DLL DDS_DataReaderQos              DDS_DATAREADER_QOS_DEFAULT;

void InitialDefaultQos()
{
    /* 根据协议给默认的QoS赋值    */
    g_default_Qos.defaultUserDataQosPolicy.value.length                                         = 0;
    g_default_Qos.defaultTopicDataQosPolicy.value.length                                        = 0;
    g_default_Qos.defaultGroupDataQosPolicy.value.length                                        = 0;
    g_default_Qos.defaultTransportPriorityQosPolicy.transport_priority                          = 0;
    g_default_Qos.defaultLifespanQosPolicy.duration.sec                                         = DURATION_INFINITE_SEC;
    g_default_Qos.defaultLifespanQosPolicy.duration.nanosec                                     = DURATION_INFINITE_NSEC;
    g_default_Qos.defaultDurabilityQosPolicy.kind                                               = VOLATILE_DURABILITY_QOS;
    g_default_Qos.defaultDurabilityServiceQosPolicy.service_cleanup_delay.sec                   = DURATION_ZERO_SEC;
    g_default_Qos.defaultDurabilityServiceQosPolicy.service_cleanup_delay.nanosec               = DURATION_ZERO_NSEC;
    g_default_Qos.defaultDurabilityServiceQosPolicy.history_kind                                = KEEP_LAST_HISTORY_QOS;
    g_default_Qos.defaultDurabilityServiceQosPolicy.history_depth                               = 1;
    g_default_Qos.defaultDurabilityServiceQosPolicy.max_samples                                 = LENGTH_UNLIMITED;
    g_default_Qos.defaultDurabilityServiceQosPolicy.max_instances                               = LENGTH_UNLIMITED;
    g_default_Qos.defaultDurabilityServiceQosPolicy.max_samples_per_instance                    = LENGTH_UNLIMITED;
    g_default_Qos.defaultPresentationQosPolicy.access_scope                                     = INSTANCE_PRESENTATION_QOS;
    g_default_Qos.defaultPresentationQosPolicy.coherent_access                                  = FALSE;
    g_default_Qos.defaultPresentationQosPolicy.ordered_access                                   = FALSE;
    g_default_Qos.defaultDeadlineQosPolicy.period.sec                                           = 0x7fffffff;
    g_default_Qos.defaultDeadlineQosPolicy.period.nanosec                                       = 0xffffffff;
    g_default_Qos.defaultLatencyBudgetQosPolicy.duration.sec                                    = DURATION_ZERO_SEC;
    g_default_Qos.defaultLatencyBudgetQosPolicy.duration.nanosec                                = DURATION_ZERO_NSEC;
    g_default_Qos.defaultOwnershipQosPolicy.kind                                                = SHARED_OWNERSHIP_QOS;
    g_default_Qos.defaultOwnershipStrengthQosPolicy.value                                       = 0;
    g_default_Qos.defaultLivelinessQosPolicy.kind                                               = AUTOMATIC_LIVELINESS_QOS;
    g_default_Qos.defaultLivelinessQosPolicy.lease_duration.sec                                 = DURATION_INFINITE_SEC;
    g_default_Qos.defaultLivelinessQosPolicy.lease_duration.nanosec                             = DURATION_INFINITE_NSEC;
    g_default_Qos.defaultTimeBasedFilterQosPolicy.minimum_separation.sec                        = DURATION_ZERO_SEC;
    g_default_Qos.defaultTimeBasedFilterQosPolicy.minimum_separation.nanosec                    = DURATION_ZERO_NSEC;
    g_default_Qos.defaultPartitionQosPolicy.pValue                                              = NULL;
    g_default_Qos.defaultPartitionQosPolicy.size                                                = 0;
    g_default_Qos.defaultReliabilityQosPolicy.kind                                                = BEST_EFFORT_RELIABILITY_QOS;//RELIABLE_RELIABILITY_QOS;
    g_default_Qos.defaultReliabilityQosPolicy.max_blocking_time.sec                             = DURATION_INFINITE_SEC;
    g_default_Qos.defaultReliabilityQosPolicy.max_blocking_time.nanosec                         = DURATION_INFINITE_NSEC;
    g_default_Qos.defaultDestinationOrderQosPolicy.kind                                         = BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
    //g_default_Qos.defaultHistoryQosPolicy.kind                                                  = KEEP_ALL_HISTORY_QOS;
	g_default_Qos.defaultHistoryQosPolicy.kind = KEEP_LAST_HISTORY_QOS;
    g_default_Qos.defaultHistoryQosPolicy.depth                                                 = 1;
    g_default_Qos.defaultResourceLimitsQosPolicy.max_samples                                    = LENGTH_UNLIMITED;
    g_default_Qos.defaultResourceLimitsQosPolicy.max_instances                                  = LENGTH_UNLIMITED;
    g_default_Qos.defaultResourceLimitsQosPolicy.max_samples_per_instance                       = LENGTH_UNLIMITED;
    g_default_Qos.defaultEntityFactoryQosPolicy.autoenable_created_entities                     = TRUE;
    g_default_Qos.defaultWriterDataLifecycleQosPolicy.autodispose_unregistered_instances        = TRUE;
    g_default_Qos.defaultReaderDataLifecycleQosPolicy.autopurge_nowriter_samples_delay.sec      = DURATION_INFINITE_SEC;
    g_default_Qos.defaultReaderDataLifecycleQosPolicy.autopurge_nowriter_samples_delay.nanosec  = DURATION_INFINITE_NSEC;
    g_default_Qos.defaultReaderDataLifecycleQosPolicy.autopurge_disposed_samples_delay.sec      = DURATION_INFINITE_SEC;
    g_default_Qos.defaultReaderDataLifecycleQosPolicy.autopurge_disposed_samples_delay.nanosec  = DURATION_INFINITE_NSEC;

    /*  模特参与者qos */
    memset(DDS_PARTICIPANT_QOS_DEFAULT.property.ipAddr, 0, 16);
	memset(DDS_PARTICIPANT_QOS_DEFAULT.property.netCardName, 0, 256);
    DDS_PARTICIPANT_QOS_DEFAULT.property.netcard_index = 0;
	DDS_PARTICIPANT_QOS_DEFAULT.liveliness.kind = AUTOMATIC_LIVELINESS_QOS;
	DDS_PARTICIPANT_QOS_DEFAULT.liveliness.lease_duration.sec = 0;
	DDS_PARTICIPANT_QOS_DEFAULT.liveliness.lease_duration.nanosec = 0;

    /*  默认主题qos */
    DDS_TOPIC_QOS_DEFAULT.topic_data         = g_default_Qos.defaultTopicDataQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.durability         = g_default_Qos.defaultDurabilityQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.durability_service = g_default_Qos.defaultDurabilityServiceQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.deadline           = g_default_Qos.defaultDeadlineQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.latency_budget     = g_default_Qos.defaultLatencyBudgetQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.liveliness         = g_default_Qos.defaultLivelinessQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.reliability        = g_default_Qos.defaultReliabilityQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.destination_order  = g_default_Qos.defaultDestinationOrderQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.history            = g_default_Qos.defaultHistoryQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.resource_limits    = g_default_Qos.defaultResourceLimitsQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.transport_priority = g_default_Qos.defaultTransportPriorityQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.lifespan           = g_default_Qos.defaultLifespanQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.ownership          = g_default_Qos.defaultOwnershipQosPolicy;
    DDS_TOPIC_QOS_DEFAULT.partition          = g_default_Qos.defaultPartitionQosPolicy;

    /* 默认写入器qos */
    DDS_DATAWRITER_QOS_DEFAULT.durability            = g_default_Qos.defaultDurabilityQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.durability_service    = g_default_Qos.defaultDurabilityServiceQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.deadline              = g_default_Qos.defaultDeadlineQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.latency_budget        = g_default_Qos.defaultLatencyBudgetQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.liveliness            = g_default_Qos.defaultLivelinessQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.reliability           = g_default_Qos.defaultReliabilityQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.controller.controller.pack_per_herat = 5;
    DDS_DATAWRITER_QOS_DEFAULT.controller.controller.period_per_heart = 2000;
    DDS_DATAWRITER_QOS_DEFAULT.destination_order     = g_default_Qos.defaultDestinationOrderQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.history               = g_default_Qos.defaultHistoryQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.history.depth         = 1000;
    DDS_DATAWRITER_QOS_DEFAULT.resource_limits       = g_default_Qos.defaultResourceLimitsQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.transport_priority    = g_default_Qos.defaultTransportPriorityQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.lifespan              = g_default_Qos.defaultLifespanQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.user_data             = g_default_Qos.defaultUserDataQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.ownership             = g_default_Qos.defaultOwnershipQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.ownership_strength    = g_default_Qos.defaultOwnershipStrengthQosPolicy;
    DDS_DATAWRITER_QOS_DEFAULT.writer_data_lifecycle = g_default_Qos.defaultWriterDataLifecycleQosPolicy;

    /* 默认阅读器qos */
    DDS_DATAREADER_QOS_DEFAULT.durability            = g_default_Qos.defaultDurabilityQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.deadline              = g_default_Qos.defaultDeadlineQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.latency_budget        = g_default_Qos.defaultLatencyBudgetQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.liveliness            = g_default_Qos.defaultLivelinessQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.reliability           = g_default_Qos.defaultReliabilityQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.destination_order     = g_default_Qos.defaultDestinationOrderQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.history               = g_default_Qos.defaultHistoryQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.history.depth         = 500;
    DDS_DATAREADER_QOS_DEFAULT.resource_limits       = g_default_Qos.defaultResourceLimitsQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.user_data             = g_default_Qos.defaultUserDataQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.time_based_filter     = g_default_Qos.defaultTimeBasedFilterQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.ownership             = g_default_Qos.defaultOwnershipQosPolicy;
    DDS_DATAREADER_QOS_DEFAULT.reader_data_lifecycle = g_default_Qos.defaultReaderDataLifecycleQosPolicy;
}

/* Nephalem 释放Partition空间 */
void UnInitTopicPartitionQoS(DDS_TopicQos* pstTopicQoS)
{
    PartitionName* pstPartitionName = pstTopicQoS->partition.pValue;
    while (NULL != pstPartitionName)
    {
        PartitionName* pstDelPartitionName = pstPartitionName;

        pstPartitionName = pstPartitionName->pNext;

        DDS_STATIC_FREE(pstDelPartitionName);
        pstDelPartitionName = NULL;
    }

    pstTopicQoS->partition.pValue = NULL;
    pstTopicQoS->partition.size = 0;
}
/* Nephalem DataWriter QoS检查设置是否正确 */
int DataWriterQoSCheck(DDS_DataWriterQos* pstWriterQos)
{
    if (LENGTH_UNLIMITED != pstWriterQos->resource_limits.max_samples)
    {
        /* Nephalem resource_limits 负值非法 */
        /* Nephame max_samples有限，而max_samples_per_instance无限，非法 */
        if (LENGTH_UNLIMITED > pstWriterQos->resource_limits.max_samples ||
            LENGTH_UNLIMITED >= pstWriterQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : resource_limits.max_samples || max_samples_per_instance set err \n");
            return -1;
        }
            

        /* Nephalem max_samples必须大于等于 max_samples_per_instance */
        if (pstWriterQos->resource_limits.max_samples < pstWriterQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : max_samples <  max_samples_per_instance\n");
            return -1;
        }
            
    }

    if (KEEP_LAST_HISTORY_QOS == pstWriterQos->history.kind)
    {
        /* Nephalem 负值非法 */
        if (0 > pstWriterQos->history.depth)
        { 
            printf("Nephalem Error : history.depth < 0 \n");
            return -2;
        }
            

        /* Nephalem max_samples_per_instance有限时，必须大于等于depth */
        if (LENGTH_UNLIMITED != pstWriterQos->resource_limits.max_samples_per_instance &&
            pstWriterQos->history.depth > pstWriterQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : history.depth > resource_limits.max_samples_per_instance \n");
            return -2;
        }

        /* Nephalem ownership_strength非法值 */
        if (EXCLUSIVE_OWNERSHIP_QOS == pstWriterQos->ownership.kind && pstWriterQos->ownership_strength.value < 0)
        {
            printf("Nephalem Error : ownership_strength.depth < 0 \n");
            return -3;
        }
            
    }

    return 0;
}


/* Nephalem DataReader QoS检查设置是否正确*/
int DataReaderQoSCheck(DDS_DataReaderQos* pstReaderQos)
{
    if (LENGTH_UNLIMITED != pstReaderQos->resource_limits.max_samples)
    {
        /* Nephalem resource_limits 负值非法*/
        /* Nephame max_samples有限，而max_samples_per_instance无限，非法 */
        if (LENGTH_UNLIMITED > pstReaderQos->resource_limits.max_samples ||
            LENGTH_UNLIMITED >= pstReaderQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : resource_limits.max_samples || max_samples_per_instance set err \n");
            return -1;
        }

        /* Nephalem max_samples必须大于等于 max_samples_per_instance */
        if (pstReaderQos->resource_limits.max_samples < pstReaderQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : max_samples <  max_samples_per_instance\n");
            return -1;
        }
    }

    if (KEEP_LAST_HISTORY_QOS == pstReaderQos->history.kind)
    {
        /* Nephalem 负值非法 */
        if (0 >= pstReaderQos->history.depth)
        {
            printf("Nephalem Error : history.depth < 0 \n");
            return -2;
        }

        /* Nephalem max_samples_per_instance有限时，必须大于等于depth */
        if (LENGTH_UNLIMITED != pstReaderQos->resource_limits.max_samples_per_instance &&
            pstReaderQos->history.depth > pstReaderQos->resource_limits.max_samples_per_instance)
        {
            printf("Nephalem Error : history.depth > resource_limits.max_samples_per_instance \n");
            return -2;
        }
    }

    return 0;
}
