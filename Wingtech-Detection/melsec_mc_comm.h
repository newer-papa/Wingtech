#ifndef __H_MELSC_MC_COMM_H__
#define __H_MELSC_MC_COMM_H__
#include "utill.h"

typedef struct _tag_melsec_mc_data_type {
	byte	data_code;	// ���͵Ĵ���ֵ
	byte	data_type;	// ���ݵ����ͣ�0�����֣�1����λ
	char	ascii_code[10];	// ����ASCII��ʽͨѶʱ����������
	int		from_base;	// ָʾ��ַ��10���ƣ�����16���Ƶ�
}melse_mc_data_type;

typedef struct _tag_melsec_mc_address_data {
	melse_mc_data_type data_type;	// ��������ݵ�ַ��Ϣ
	int	address_start;				// ���ֵ���ʼ��ַ��Ҳ����ƫ�Ƶ�ַ
	int length;						// ��ȡ�����ݳ���
}melsec_mc_address_data;

int mc_convert_string_to_int(const char* address, int frombase);
melse_mc_data_type mc_create_data_type(byte code, byte type, const char* ascii_code, int from_base);
melsec_mc_address_data mc_analysis_address(const char* address, int length);

#endif//__H_MELSC_MC_COMM_H__