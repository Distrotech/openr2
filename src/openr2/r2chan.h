/*
 * OpenR2 
 * MFC/R2 call setup library
 *
 * Moises Silva <moy@sangoma.com>
 * Copyright (C) 2008 Moises Silva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _OPENR2_CHAN_H_
#define _OPENR2_CHAN_H_

#include <stdarg.h>
#include "r2proto.h"
#include "r2log.h"
#include "r2context.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef __OR2_COMPILING_LIBRARY__
struct openr2_chan_s;
#define openr2_chan_t struct openr2_chan_s
struct openr2_context_s;
#define openr2_context_t struct openr2_context_s
#else
#ifndef OR2_CHAN_AND_CONTEXT_DEFINED
#define OR2_CHAN_AND_CONTEXT_DEFINED
typedef void* openr2_chan_t;
typedef void* openr2_context_t;
#endif
#endif

#define OR2_CHAN_READ_SIZE 160
/* callback for logging channel related info */
typedef void (*openr2_logging_func_t)(openr2_chan_t *r2chan, openr2_log_level_t level, const char *fmt, va_list ap);

openr2_chan_t *openr2_chan_new(openr2_context_t *r2context, int channo, void *mf_write_handle, void *mf_read_handle);
openr2_chan_t *openr2_chan_new_from_fd(openr2_context_t *r2context, int chanfd, void *mf_write_handle, void *mf_read_handle);
void openr2_chan_delete(openr2_chan_t *r2chan);
int openr2_chan_process_event(openr2_chan_t *r2chan);
int openr2_chan_accept_call(openr2_chan_t *r2chan, openr2_call_mode_t accept);
int openr2_chan_answer_call(openr2_chan_t *r2chan);
int openr2_chan_answer_call_with_mode(openr2_chan_t *r2chan, openr2_answer_mode_t mode);
int openr2_chan_disconnect_call(openr2_chan_t *r2chan, openr2_call_disconnect_cause_t cause);
int openr2_chan_make_call(openr2_chan_t *r2chan, const char *ani, const char *dnid, openr2_calling_party_category_t category);
openr2_direction_t openr2_chan_get_direction(openr2_chan_t *r2chan);
int openr2_chan_write(openr2_chan_t *r2chan, const unsigned char *buf, int len);
void openr2_chan_set_logging_func(openr2_chan_t *r2chan, openr2_logging_func_t logcallback);
int openr2_chan_get_fd(openr2_chan_t *r2chan);
int openr2_chan_get_number(openr2_chan_t *r2chan);
openr2_context_t *openr2_chan_get_context(openr2_chan_t *r2chan);
void openr2_chan_set_client_data(openr2_chan_t *r2chan, void *data);
void *openr2_chan_get_client_data(openr2_chan_t *r2chan);
int openr2_chan_get_time_to_next_event(openr2_chan_t *r2chan);
openr2_log_level_t openr2_chan_set_log_level(openr2_chan_t *r2chan, openr2_log_level_t level);
openr2_log_level_t openr2_chan_get_log_level(openr2_chan_t *r2chan);
void openr2_chan_enable_read(openr2_chan_t *r2chan);
void openr2_chan_disable_read(openr2_chan_t *r2chan);
int openr2_chan_get_read_enabled(openr2_chan_t *r2chan);
void openr2_chan_enable_call_files(openr2_chan_t *r2chan);
void openr2_chan_disable_call_files(openr2_chan_t *r2chan);
int openr2_chan_get_call_files_enabled(openr2_chan_t *r2chan);
const char *openr2_chan_get_dnis(openr2_chan_t *r2chan);
const char *openr2_chan_get_ani(openr2_chan_t *r2chan);
int openr2_chan_set_idle(openr2_chan_t *r2chan);
int openr2_chan_set_blocked(openr2_chan_t *r2chan);
int openr2_chan_handle_cas(openr2_chan_t *r2chan);
const char *openr2_chan_get_rx_cas_string(openr2_chan_t *r2chan);
const char *openr2_chan_get_tx_cas_string(openr2_chan_t *r2chan);
const char *openr2_chan_get_call_state_string(openr2_chan_t *r2chan);
const char *openr2_chan_get_r2_state_string(openr2_chan_t *r2chan);
const char *openr2_chan_get_mf_state_string(openr2_chan_t *r2chan);
const char *openr2_chan_get_mf_group_string(openr2_chan_t *r2chan);
int openr2_chan_get_tx_mf_signal(openr2_chan_t *r2chan);
int openr2_chan_get_rx_mf_signal(openr2_chan_t *r2chan);
int openr2_chan_set_dtmf_handles(openr2_chan_t *r2chan, void *dtmf_read_handle, void *dtmf_write_handle);

#ifdef __OR2_COMPILING_LIBRARY__
#undef openr2_chan_t
#undef openr2_context_t
#endif
#if defined(__cplusplus)
} /* endif extern "C" */
#endif

#endif /* endif defined _OPENR2_CHAN_H_ */

