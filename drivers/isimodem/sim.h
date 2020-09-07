/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2009-2010  Nokia Corporation and/or its subsidiary(-ies).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __ISIMODEM_SIM_H
#define __ISIMODEM_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

#define PN_SIM							0x09
#define SIM_TIMEOUT						5
#define PN_SECURITY						0x08
#define SIM_MAX_IMSI_LENGTH					15

enum sim_isi_cause {
	SIM_SERV_NOT_AVAIL =					0x00,
	SIM_SERV_OK =						0x01,
	SIM_SERV_PIN_VERIFY_REQUIRED =				0x02,
	SIM_SERV_PIN_REQUIRED =					0x03,
	SIM_SERV_SIM_BLOCKED =					0x04,
	SIM_SERV_SIM_PERMANENTLY_BLOCKED =			0x05,
	SIM_SERV_SIM_DISCONNECTED =				0x06,
	SIM_SERV_SIM_REJECTED =					0x07,
	SIM_SERV_LOCK_ACTIVE =					0x08,
	SIM_SERV_AUTOLOCK_CLOSED =				0x09,
	SIM_SERV_AUTOLOCK_ERROR =				0x0A,
	SIM_SERV_INIT_OK =					0x0B,
	SIM_SERV_INIT_NOT_OK =					0x0C,
	SIM_SERV_WRONG_OLD_PIN =				0x0D,
	SIM_SERV_PIN_DISABLED =					0x0E,
	SIM_SERV_COMMUNICATION_ERROR =				0x0F,
	SIM_SERV_UPDATE_IMPOSSIBLE =				0x10,
	SIM_SERV_NO_SECRET_CODE_IN_SIM =			0x11,
	SIM_SERV_PIN_ENABLE_OK =				0x12,
	SIM_SERV_PIN_DISABLE_OK =				0x13,
	SIM_SERV_WRONG_UNBLOCKING_KEY =				0x15,
	SIM_SERV_ILLEGAL_NUMBER =				0x2E,
	SIM_SERV_NOT_OK =					0x1C,
	SIM_SERV_PN_LIST_ENABLE_OK =				0x1E,
	SIM_SERV_PN_LIST_DISABLE_OK =				0x1F,
	SIM_SERV_NO_PIN =					0x20,
	SIM_SERV_PIN_VERIFY_OK =				0x21,
	SIM_SERV_PIN_BLOCKED =					0x22,
	SIM_SERV_PIN_PERM_BLOCKED =				0x23,
	SIM_SERV_DATA_NOT_AVAIL =				0x24,
	SIM_SERV_IN_HOME_ZONE =					0x25,
	SIM_SERV_STATE_CHANGED =				0x27,
	SIM_SERV_INF_NBR_READ_OK =				0x28,
	SIM_SERV_INF_NBR_READ_NOT_OK =				0x29,
	SIM_SERV_IMSI_EQUAL =					0x2A,
	SIM_SERV_IMSI_NOT_EQUAL =				0x2B,
	SIM_SERV_INVALID_LOCATION =				0x2C,
	SIM_SERV_STA_SIM_REMOVED =				0x35,
	SIM_SERV_SECOND_SIM_REMOVED_CS =			0x36,
	SIM_SERV_CONNECTED_INDICATION_CS =			0x37,
	SIM_SERV_SECOND_SIM_CONNECTED_CS =			0x38,
	SIM_SERV_PIN_RIGHTS_LOST_IND_CS =			0x39,
	SIM_SERV_PIN_RIGHTS_GRANTED_IND_CS =			0x3A,
	SIM_SERV_INIT_OK_CS =					0x3B,
	SIM_SERV_INIT_NOT_OK_CS =				0x3C,
	SIM_FDN_ENABLED =					0x19,
	SIM_FDN_DISABLED =					0x1A,
	SIM_SERV_INVALID_FILE =					0x45,
	SIM_SERV_DATA_AVAIL =					0x4F,
	SIM_SERV_ICC_EQUAL =					0x49,
	SIM_SERV_ICC_NOT_EQUAL =				0x4A,
	SIM_SERV_SIM_NOT_INITIALISED =				0x4B,
	SIM_SERV_SERVICE_NOT_AVAIL =				0x50,
	SIM_SERV_FDN_STATUS_ERROR =				0x57,
	SIM_SERV_FDN_CHECK_PASSED =				0x58,
	SIM_SERV_FDN_CHECK_FAILED =				0x59,
	SIM_SERV_FDN_CHECK_DISABLED =				0x5A,
	SIM_SERV_FDN_CHECK_NO_FDN_SIM =				0x5B,
	SIM_STA_ISIM_AVAILEBLE_PIN_REQUIRED =			0x5C,
	SIM_STA_ISIM_AVAILEBLE =				0x5D,
	SIM_STA_USIM_AVAILEBLE =				0x5E,
	SIM_STA_SIM_AVAILEBLE =					0x5F,
	SIM_STA_ISIM_NOT_INITIALIZED =				0x60,
	SIM_STA_IMS_READY =					0x61,
	SIM_STA_APP_DATA_READ_OK =				0x96,
	SIM_STA_APP_ACTIVATE_OK =				0x97,
	SIM_STA_APP_ACTIVATE_NOT_OK =				0x98,
	SIM_SERV_NOT_DEFINED =					0xF9,
	SIM_SERV_NOSERVICE =					0xFA,
	SIM_SERV_NOTREADY =					0xFB,
	SIM_SERV_ERROR =					0xFC,
	SIM_SERV_CIPHERING_INDICATOR_DISPLAY_REQUIRED =		0x30,
	SIM_SERV_CIPHERING_INDICATOR_DISPLAY_NOT_REQUIRED =	0x31,
	SIM_SERV_FILE_NOT_AVAILABLE =				0x4D
};

enum sim_subblock {
	SIM_PB_INFO_REQUEST =					0xE4,
	SIM_PB_STATUS =						0xFB,
	SIM_PB_LOCATION =					0xFE,
	SIM_PB_LOCATION_SEARCH =				0xFF,
};

enum sim_pb_type {
	SIM_PB_ADN =						0xC8,
};

enum sim_pb_tag {
	SIM_PB_ANR =						0xCA,
	SIM_PB_EMAIL =						0xDD,
	SIM_PB_SNE =						0xF7,
};

enum sim_message_id {
	SIM_NETWORK_INFO_REQ =					0x19,
	SIM_NETWORK_INFO_RESP =					0x1A,
	SIM_IMSI_REQ_READ_IMSI =				0x1D,
	SIM_IMSI_RESP_READ_IMSI =				0x1E,
	SIM_SERV_PROV_NAME_REQ =				0x21,
	SIM_SERV_PROV_NAME_RESP =				0x22,
	SIM_DYNAMIC_FLAGS_REQ =					0x29,
	SIM_DYNAMIC_FLAGS_RESP =				0x2A,
	SIM_READ_FIELD_REQ =					0xBA,
	SIM_READ_FIELD_RESP =					0xBB,
	SIM_SMS_REQ =						0xBC,
	SIM_SMS_RESP =						0xBD,
	SIM_STATUS_REQ =					0xC0,
	SIM_STATUS_RESP =					0xC1,
	SIM_PB_REQ_SIM_PB_READ =				0xDC,
	SIM_PB_RESP_SIM_PB_READ =				0xDD,
	SIM_SERVER_READY_IND =					0xED,
	SIM_IND =						0xEF,
};

enum sim_service_type {
	SIM_ST_CARD_STATUS =					0x00,
	SIM_ST_PIN =						0x01,
	SIM_ST_ALL_SERVICES =					0x05,
	SIM_ST_INFO =						0x0D,
	SIM_PB_READ =						0x0F,
	SIM_ST_CAT_SUPPORT_ENABLE =				0x15,
	SIM_ST_CAT_SUPPORT_DISABLE =				0x16,
	SIM_ST_READ_SERV_PROV_NAME =				0x2C,
	READ_IMSI =						0x2D,
	READ_HPLMN =						0x2F,
	READ_DYN_FLAGS =					0x35,
	READ_PARAMETER =					0x52,
	UPDATE_PARAMETER =					0x53,
	ICC =							0x66,
};

#define SEC_CODE_MAX_LENGTH		0x0A

enum sec_message_id {
	SEC_CODE_STATE_REQ =		0x01,
	SEC_CODE_STATE_OK_RESP =	0x02,
	SEC_CODE_STATE_FAIL_RESP =	0x03,
	SEC_CODE_CHANGE_REQ =		0x04,
	SEC_CODE_CHANGE_OK_RESP =	0x05,
	SEC_CODE_CHANGE_FAIL_RESP =	0x06,
	SEC_CODE_VERIFY_REQ =		0x07,
	SEC_CODE_VERIFY_OK_RESP =	0x08,
	SEC_CODE_VERIFY_FAIL_RESP =	0x09,
	SEC_STATE_REQ =			0x11,
	SEC_STATE_RESP =		0x12,
};

enum sec_code_id_info {
	SEC_CODE_PIN =			0x02,
	SEC_CODE_PUK =			0x03,
	SEC_CODE_PIN2 =			0x04,
	SEC_CODE_PUK2 =			0x05,
};

enum sec_code_state_info {
	SEC_CODE_DISABLE =		0x00,
	SEC_CODE_ENABLE =		0x01,
	SEC_CODE_STATE_QUERY =		0x04,
};

enum sec_state_cause_info {
	SEC_CAUSE_PIN_REQUIRED =	0x02,
	SEC_CAUSE_PUK_REQUIRED =	0x03,
	SEC_STARTUP_OK =		0x05,
	SEC_STARTUP_ONGOING =		0x07,
	SEC_CAUSE_CODE_BLOCKED =	0x08,
	SEC_CAUSE_NO_SIM =		0x16,
	SEC_CAUSE_SIM_REJECTED =	0x1A,
	SEC_CAUSE_INVALID_SIM =		0x1E,
};

#ifdef __cplusplus
};
#endif

#endif /* __ISIMODEM_SIM_H */
