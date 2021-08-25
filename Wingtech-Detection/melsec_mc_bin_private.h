#ifndef __H_MELSEC_MC_BIN_PRIVATE_H__
#define __H_MELSEC_MC_BIN_PRIVATE_H__

#include "melsec_mc_comm.h"

int mc_read_response(int fd, byte_array_info* response);

//////////////////////////////////////////////////////////////////////////
bool read_bool_value(int fd, const char* address, int length, byte_array_info* out_bytes);
bool read_word_value(int fd, const char* address, int length, byte_array_info* out_bytes);
bool read_address_data(int fd, melsec_mc_address_data address_data, byte_array_info* out_bytes);

bool write_bool_value(int fd, const char* address, int length, bool_array_info in_bytes);
bool write_word_value(int fd, const char* address, int length, byte_array_info in_bytes);
bool write_address_data(int fd, melsec_mc_address_data address_data, byte_array_info in_bytes);

//////////////////////////////////////////////////////////////////////////
bool mc_remote_run(int fd);
bool mc_remote_stop(int fd);
bool mc_remote_reset(int fd);
char* mc_read_plc_type(int fd);

//��MCЭ��ĺ��ı��Ĵ����һ������ֱ�Ӷ�PLC���з��͵�ԭʼ����
byte_array_info pack_mc_command(byte_array_info* mc_core, byte network_number, byte station_number);
//��PLC��������������ȡ��ʵ�ʵ��������ݣ���Ҫ���뷴�����ݣ��Ƿ�λ��ȡ
void extract_actual_bool_data(byte_array_info* response);

plc_network_address g_network_address;

#endif//__H_MELSEC_MC_BIN_PRIVATE_H__
