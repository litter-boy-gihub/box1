#ifndef _DDSDRV_H
#define _DDSDRV_H
//#define _TM1
//#define _TM3
typedef enum  _DDS_INIT_MASK
{
	DDS_INIT_TOPIC = 0,
	DDS_INIT_SEND,
	DDS_INIT_RECV,
	DDS_INIT_SENDRECV
}DDS_INIT_MASK;

typedef struct
{
	unsigned int major_code;
	unsigned int minor_code;
	unsigned int release;
	unsigned int build;

	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
}Ver_Info;

#if defined _TM1 || defined _TM3

typedef void ( *DDS_RECV_CALLBACK_FUNC)(const void* buf, long length, int domain_id, const char* topic_name);

extern  int ddsdrv_init_domain(int domain_id);

extern  int ddsdrv_destory_domain(int domain_id);

extern  void ddsdrv_destory_domains();

extern  int ddsdrv_init_topic(int domain_id, const char* topic_name, long max_length, DDS_INIT_MASK init_mask);

extern  int ddsdrv_send(int domain_id, const char* topic_name, const void* buf ,long length);

extern  int ddsdrv_recv(int domain_id, const char* topic_name, void* buf, long length, unsigned int* src_ip);

extern  int ddsdrv_setcb(int domain_id, const char* topic_name, DDS_RECV_CALLBACK_FUNC func);

extern  Ver_Info ddsdrv_info_report();


#else

typedef void (STACALL *DDS_RECV_CALLBACK_FUNC)(const void* buf, long length, int domain_id, const char* topic_name);

extern DDS_DLL int ddsdrv_init_domain(int domain_id);

extern DDS_DLL int ddsdrv_destory_domain(int domain_id);

extern DDS_DLL void ddsdrv_destory_domains();

extern DDS_DLL int ddsdrv_init_topic(int domain_id, const char* topic_name, long max_length, DDS_INIT_MASK init_mask);

extern DDS_DLL int ddsdrv_send(int domain_id, const char* topic_name, const void* buf ,long length);

extern DDS_DLL int ddsdrv_recv(int domain_id, const char* topic_name, void* buf, long length, unsigned int* src_ip);

extern DDS_DLL  int ddsdrv_setcb(int domain_id, const char* topic_name, DDS_RECV_CALLBACK_FUNC func);

extern DDS_DLL Ver_Info ddsdrv_info_report();

#endif



#endif // endif _DDSDRV_H
