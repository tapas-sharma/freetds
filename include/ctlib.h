/* FreeTDS - Library of routines accessing Sybase and Microsoft databases
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004  Brian Bruns
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _ctlib_h_
#define _ctlib_h_

/*
 * Internal (not part of the exposed API) prototypes and such.
 */

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

static const char rcsid_ctlib_h[] = "$Id: ctlib.h,v 1.19 2004-12-05 20:05:08 freddy77 Exp $";
static const void *const no_unused_ctlib_h_warn[] = { rcsid_ctlib_h, no_unused_ctlib_h_warn };

#include <tds.h>
/*
 * internal types
 */
struct _cs_config
{
	short cs_expose_formats;
};

/* Code changed for error handling */
/* Code changes starts here - CT_DIAG - 01 */

/* This structure is used in CT_DIAG */

struct cs_diag_msg_client
{
	CS_CLIENTMSG *clientmsg;
	struct cs_diag_msg_client *next;
};

struct cs_diag_msg_svr
{
	CS_SERVERMSG *servermsg;
	struct cs_diag_msg_svr *next;
};

/* Code changes ends here - CT_DIAG - 01 */

struct cs_diag_msg
{
	CS_CLIENTMSG *msg;
	struct cs_diag_msg *next;
};

struct _cs_context
{
	CS_INT date_convert_fmt;
	CS_INT cs_errhandletype;
	CS_INT cs_diag_msglimit;

	/* added for storing the maximum messages limit CT_DIAG */
	/* code changes starts here - CT_DIAG - 02 */

	CS_INT cs_diag_msglimit_client;
	CS_INT cs_diag_msglimit_server;
	CS_INT cs_diag_msglimit_total;
	struct cs_diag_msg_client *clientstore;
	struct cs_diag_msg_svr *svrstore;

	/* code changes ends here - CT_DIAG - 02 */

	struct cs_diag_msg *msgstore;
	CS_CSLIBMSG_FUNC _cslibmsg_cb;
	CS_CLIENTMSG_FUNC _clientmsg_cb;
	CS_SERVERMSG_FUNC _servermsg_cb;
	/* code changes start here - CS_CONFIG - 01*/
	void *userdata;
	int userdata_len;
	/* code changes end here - CS_CONFIG - 01*/
	TDSCONTEXT *tds_ctx;
	CS_CONFIG config;
};

/*
 * internal typedefs
 */
typedef struct _ct_colinfo
{
	TDS_SMALLINT *indicator;
}
CT_COLINFO;

struct _cs_connection
{
	CS_CONTEXT *ctx;
	TDSLOGIN *tds_login;
	TDSSOCKET *tds_socket;
	CS_CLIENTMSG_FUNC _clientmsg_cb;
	CS_SERVERMSG_FUNC _servermsg_cb;
	void *userdata;
	int userdata_len;
	CS_LOCALE *locale;
};

/*
 * Formerly CSREMOTE_PROC_PARAM, this structure can be used in other
 * places, too.
 */

typedef struct _cs_param
{
	struct _cs_param *next;
	char *name;
	int status;
	int type;
	CS_INT maxlen;
	CS_INT *datalen;
	CS_SMALLINT *ind;
	CS_BYTE *value;
	int param_by_value;
	CS_INT datalen_value;
	CS_SMALLINT indicator_value;
} CS_PARAM;

/*
 * Code added for RPC functionality - SUHA
 * RPC Code changes starts here
 */

typedef CS_PARAM CSREMOTE_PROC_PARAM;

typedef struct _csremote_proc
{
	char *name;
	CS_SMALLINT options;
	CSREMOTE_PROC_PARAM *param_list;
} CSREMOTE_PROC;

/*
 * Structure CS_COMMAND changed for RPC functionality -SUHA
 * Added CSREMOTE_PROC *rpc to CS_COMMAND structure
 */

/* values for cs_command.results_state */

#define _CS_RES_INIT            0
#define _CS_RES_RESULTSET_EMPTY 1
#define _CS_RES_RESULTSET_ROWS  2
#define _CS_RES_STATUS          3
#define _CS_RES_CMD_DONE        4
#define _CS_RES_CMD_SUCCEED     5

struct _cs_command
{
	CS_CHAR *query;
	CS_INT command_type;
	CS_CONNECTION *con;
	short dynamic_cmd;
	char *dyn_id;
	int row_prefetched;
	int results_state;
	int curr_result_type;
	/* Array Binding Code changes start here */
	int bind_count;
	/* Array Binding Code changes end here */
	int get_data_item;
	int get_data_bytes_returned;
	CS_IODESC *iodesc;
	CS_INT send_data_started;
	CSREMOTE_PROC *rpc;
	CS_PARAM *input_params;
	TDSCURSOR *cursor;
};

struct _cs_blkdesc
{
	CS_CONNECTION *con;
	CS_CHAR *tablename;
	CS_CHAR *insert_stmt;
	CS_INT direction;
	CS_INT identity_insert_on;
	CS_INT bind_count;
	CS_INT xfer_init;
	CS_INT var_cols;
	TDSRESULTINFO *bindinfo;
};


#define _CS_ERRHAND_INLINE 1
#define _CS_ERRHAND_CB     2

struct _cs_locale
{
	char *language;
	char *charset;
	char *time;
	char *collate;
};

/* internal defines for cursor processing */

#define _CS_CURS_TYPE_UNACTIONED 0
#define _CS_CURS_TYPE_REQUESTED  1
#define _CS_CURS_TYPE_SENT       2

/*
 * internal prototypes
 */
int _ct_handle_server_message(TDSCONTEXT * ctxptr, TDSSOCKET * tdsptr, TDSMESSAGE * msgptr);
int _ct_handle_client_message(TDSCONTEXT * ctxptr, TDSSOCKET * tdsptr, TDSMESSAGE * msgptr);
int _ct_get_server_type(int datatype);
int _ct_bind_data(CS_CONTEXT *ctx, TDSRESULTINFO * resinfo, TDSRESULTINFO *bindinfo, CS_INT offset);
int _ct_get_client_type(int datatype, int usertype, int size);
void _ctclient_msg(CS_CONNECTION * con, const char *funcname, int layer, int origin, int severity, int number,
		   const char *fmt, ...);
CS_INT _ct_diag_clearmsg(CS_CONTEXT * context, CS_INT type);

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif
