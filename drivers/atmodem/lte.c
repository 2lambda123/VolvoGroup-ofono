/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2017  Intel Corporation. All rights reserved.
 *  Copyright (C) 2018 Gemalto M2M
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <glib.h>

#include <ofono/modem.h>
#include <ofono/gprs-context.h>
#include <ofono/log.h>
#include <ofono/lte.h>

#include "gatchat.h"
#include "gatresult.h"

#include "atmodem.h"
#include "vendor.h"

struct lte_driver_data {
	GAtChat *chat;
	unsigned int vendor;
};

struct lte_cb_data {
	const struct ofono_lte_default_attach_info *info;
	ofono_lte_cb_t cb;
	const struct ofono_lte *lte;
	void *data;
};

static void at_lte_set_default_auth_info_cb(gboolean ok, GAtResult *result,
							gpointer user_data)
{

	struct lte_cb_data *lcbd = user_data;
	struct ofono_error error;
	ofono_lte_cb_t cb = lcbd->cb;
	void *data = lcbd->data;

	DBG("ok %d", ok);

	decode_at_error(&error, g_at_result_final_response(result));
	cb(&error, data);
}

static void gemalto_at_lte_set_default_auth_info(const struct ofono_lte *lte,
			const struct ofono_lte_default_attach_info *info,
			ofono_lte_cb_t cb, void *data)
{
	struct lte_cb_data *lcbd = data;
	void* ud = lcbd->data;
	struct lte_driver_data *ldd = ofono_lte_get_data(lte);
	char buf[32 + OFONO_GPRS_MAX_USERNAME_LENGTH +
					OFONO_GPRS_MAX_PASSWORD_LENGTH +1];
	guint buflen = sizeof(buf);

	if (ldd->vendor==OFONO_VENDOR_GEMALTO_CGAUTH) {
		snprintf(buf, buflen, "AT+CGAUTH=");
	} else {
		snprintf(buf, buflen, "AT^SGAUTH=");
	}

	buflen-=strlen(buf);

	if (info->auth_type == OFONO_LTE_AUTH_METHOD_NONE) {
		snprintf(buf+strlen(buf), buflen, "1,0");
		if(g_at_chat_send(ldd->chat, buf, NULL,
			  at_lte_set_default_auth_info_cb, lcbd, g_free) > 0) {
			return;
		}
		goto set_info_failure;
	}

	if (!*info->username || !*info->password) {
		g_free(lcbd);
		goto set_info_failure;
	}

	if (info->auth_type == OFONO_LTE_AUTH_METHOD_PAP) {
		snprintf(buf+strlen(buf), buflen, "1,1,\"%s\",\"%s\"",
						info->username, info->password);

		if(g_at_chat_send(ldd->chat, buf, NULL,
			at_lte_set_default_auth_info_cb, lcbd, g_free) > 0) {
			return;
		}

	} else if (info->auth_type == OFONO_LTE_AUTH_METHOD_CHAP) {
		snprintf(buf+strlen(buf), buflen, "1,2,\"%s\",\"%s\"",
						info->username, info->password);

		if(g_at_chat_send(ldd->chat, buf, NULL,
			at_lte_set_default_auth_info_cb, lcbd, g_free) > 0) {
			return;
		}

	} else {
		g_free(lcbd);
	}

set_info_failure:
	CALLBACK_WITH_FAILURE(cb, ud);
}

static void at_lte_set_default_attach_info_cb(gboolean ok, GAtResult *result,
							gpointer user_data)
{
	struct lte_cb_data *lcbd = user_data;
	struct lte_driver_data *ldd = ofono_lte_get_data(lcbd->data);
	struct ofono_error error;

	DBG("ok %d", ok);

	if (ok) {
		switch (ldd->vendor) {
		case OFONO_VENDOR_GEMALTO:
			gemalto_at_lte_set_default_auth_info(lcbd->lte,
				lcbd->info, lcbd->cb, user_data);
			return;
			break;
		default:
			break;
		}
	}

	decode_at_error(&error, g_at_result_final_response(result));
	lcbd->cb(&error, lcbd->data);
}

static void at_lte_set_default_attach_info(const struct ofono_lte *lte,
			const struct ofono_lte_default_attach_info *info,
			ofono_lte_cb_t cb, void *data)
{
	struct lte_driver_data *ldd = ofono_lte_get_data(lte);
	char buf[32 + OFONO_GPRS_MAX_APN_LENGTH  +1];
	struct lte_cb_data *lcbd = g_new0(struct lte_cb_data, 1);

	lcbd->data = data;
	lcbd->info = info;
	lcbd->cb = cb;
	lcbd->lte = lte;

	DBG("LTE config with APN: %s", info->apn);

	/* We can't do much in case of failure so don't check any response */

	if(ldd->vendor==OFONO_VENDOR_GEMALTO || ldd->vendor == OFONO_VENDOR_GEMALTO_CGAUTH) {

		if (strlen(info->apn) > 0)
			snprintf(buf, sizeof(buf), "AT+CGDCONT=1,\"IP\",\"%s\"",
						info->apn);
		else
			snprintf(buf, sizeof(buf), "AT+CGDCONT=1,\"IP\"");

		if (g_at_chat_send(ldd->chat, buf, NULL,
			  at_lte_set_default_attach_info_cb, lcbd, NULL) > 0)
			return;
	} else {

		if (strlen(info->apn) > 0)
			snprintf(buf, sizeof(buf), "AT+CGDCONT=0,\"IP\",\"%s\"",
					info->apn);
		else
			snprintf(buf, sizeof(buf), "AT+CGDCONT=0,\"IP\"");

		if (g_at_chat_send(ldd->chat, buf, NULL,
			  at_lte_set_default_attach_info_cb, lcbd, NULL) > 0)
			return;

	}

	CALLBACK_WITH_FAILURE(cb, data);
}

static gboolean lte_delayed_register(gpointer user_data)
{
	ofono_lte_register(user_data);

	return FALSE;
}

static int at_lte_probe(struct ofono_lte *lte, unsigned int vendor, void *data)
{
	GAtChat *chat = data;
	struct lte_driver_data *ldd;

	DBG("at lte probe");

	ldd = g_try_new0(struct lte_driver_data, 1);
	if (!ldd)
		return -ENOMEM;

	ldd->chat = g_at_chat_clone(chat);
	ldd->vendor = vendor;

	ofono_lte_set_data(lte, ldd);

	g_idle_add(lte_delayed_register, lte);

	return 0;
}

static void at_lte_remove(struct ofono_lte *lte)
{
	struct lte_driver_data *ldd = ofono_lte_get_data(lte);

	DBG("at lte remove");

	g_at_chat_unref(ldd->chat);

	ofono_lte_set_data(lte, NULL);

	g_free(ldd);
}

static struct ofono_lte_driver driver = {
	.name				= "atmodem",
	.probe				= at_lte_probe,
	.remove				= at_lte_remove,
	.set_default_attach_info	= at_lte_set_default_attach_info,
};

void at_lte_init(void)
{
	ofono_lte_driver_register(&driver);
}

void at_lte_exit(void)
{
	ofono_lte_driver_unregister(&driver);
}
