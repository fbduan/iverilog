#ifndef __veriuser_H
#define __veriuser_H
/*
 * Copyright (c) 2002 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: veriuser.h,v 1.27 2003/05/28 03:38:05 steve Exp $"
#endif

/*
 * This header file contains the definitions and declarations needed
 * by an Icarus Verilog user using tf_ routines.
 *
 * NOTE 1: Icarus Verilog does not directly support tf_ routines. This
 * header file defines a tf_ compatibility later. The functions that
 * are implemented here are actually implemented using VPI routines.
 *
 * NOTE 2: The routines and definitions of the tf_ library were
 * clearly not designed to account for C++, or even ANSI-C. This
 * header file attempts to fix these problems in a source code
 * compatible way. In the end, though, it is not completely
 * possible. Instead, users should not use this or the acc_user.h
 * header files or functions in new applications, and instead use the
 * more modern vpi_user.h and VPI functions.
 *
 * This API is provided by Icarus Verilog only to support legacy software.
 */


#ifdef __cplusplus
# define EXTERN_C_START extern "C" {
# define EXTERN_C_END }
#else
# define EXTERN_C_START
# define EXTERN_C_END
#endif

#ifndef __GNUC__
# undef  __attribute__
# define __attribute__(x)
#endif

EXTERN_C_START

# include  "_pli_types.h"

/*
 * defines for tf_message
 */
#define ERR_MESSAGE 1
#define ERR_WARNING 2
#define ERR_ERROR   3
#define ERR_INTERNAL 4
#define ERR_SYSTEM   5

/*
 * These are some defines for backwards compatibility. They should not
 * be used in new code.
 */
#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif
#ifndef __cplusplus
# ifndef true
#  define true 1
# endif
# ifndef false
#  define false 0
# endif
# ifndef bool
#  define bool int
# endif
#endif

/*
 * structure for veriusertfs array
 */
typedef struct t_tfcell
{
      short type;               /* usertask|userfunction|userrealfunction */
      short data;               /* data passed to user routine */
      int   (*checktf)(int user_data, int reason);
      int   (*sizetf)(int user_data, int reason);
      int   (*calltf)(int user_data, int reason);
      int   (*misctf)(int user_data, int reason, int paramvc);
      char  *tfname;            /* name of the system task/function */
      int   forwref;            /* usually set to 1 */
      char  *tfveritool;        /* usually ignored */
      char  *tferrmessage;      /* usually ignored */
      char  reserved[20];            /* reserved */
} s_tfcell, *p_tfcell;

/*
 * Normal PLI1.0 modules export a veriusertfs array that contains all
 * the function definitions for use by the simulator. The user code is
 * expected to supply that array. The method of export varies amongst
 * Verilog simulators, but at least one vendor gets a pointer to the
 * veriusertfs array by calling a boot function that takes no
 * arguments and returns a pointer to the table.
 *
 * The Icarus Verilog bootstrap process is a little bit different, and
 * is described in the vpi_user.h header file. However, a fairly
 * simple adaptation to your application fixes it so that it
 * automatically boots with Icarus Verilog.
 *
 * The trick is to write a vlog_startup_routine that calls the
 * veriusertfs_register_table function. A simple example:
 *
 *   static struct s_tfcell my_veriusertfs_table[] = {
 *          [... table entries, null terminated ...]
 *   };
 *
 *   // Cadence compatibility
 *   struct s_tfcell* my_bootstrap(void)
 *   { return my_veriusertfs_table; }
 *
 *   // Icarus Verilog compatibility
 *   static void veriusertfs_register(void)
 *   {
 *       veriusertfs_register_table(my_veriusertfs_table);
 *   }
 *   void (*vlog_startup_routines[])() = { &veriusertfs_register, 0 };
 *
 * The veriusertfs_register function and vlog_startup_routines table
 * are specific to Icarus Verilog, and arrange for automatic loading
 * of the PLI1 application. The vvp program will treat this as any
 * other VPI module.
 *
 * By structuring the bootstrap process in this manner, it is
 * plausible to make source code compatibility with a variety of
 * Verilog simulators.
 *
 * NOTE: The cadpli module of Icarus Verilog also makes it possible to
 * be *binary* compatible with other simulators. Create the
 * my_bootstrap function and leave out the vlog_startup_routines, then
 * use the "-mcadpli" module to load it in compatibility mode.
 */
extern s_tfcell veriusertfs[];
extern void veriusertfs_register_table(p_tfcell vtable);

#define usertask 1
#define userfunction 2
#define userrealfunction 3

/* callback reasons */
#define reason_checktf 1
#define reason_calltf 3
#define reason_paramvc 7
#define reason_synch  8
#define REASON_SYNCH  reason_synch
#define reason_finish 9
#define reason_endofcompile 16

/* These are values returned by tf_typep */
#define tf_nullparam    0
#define TF_NULLPARAM    tf_nullparam
#define tf_string       1
#define TF_STRING       tf_string
#define tf_readonly     10
#define TF_READONLY     tf_readonly
#define tf_readwrite    11
#define TF_READWRITE    tf_readwrite
#define tf_rwbitselect  12
#define TF_RWBITSELECT  tf_rwbitselect
#define tf_rwpartselect 13
#define TF_RWPARTSELECT tf_rwpartselect
#define tf_rwmemselect  14
#define TF_RWMEMSELECT  tf_rwmemselect
#define tf_readonlyreal 15
#define TF_READONLYREAL tf_readonlyreal
#define tf_readwritereal 16
#define TF_READWRITEREAD tf_readwritereal

typedef struct t_tfnodeinfo {
      PLI_INT16 node_type;
      PLI_INT16 padding;
      union {
	    struct t_vecval *vecval_p;
	    struct t_strengthval *strengthval_p;
	    PLI_BYTE8 *memoryval_p;
	    double *read_value_p;
      } node_value;
      char* node_symbol;
      PLI_INT32 node_ngroups;
      PLI_INT32 node_vec_size;
      PLI_INT32 node_sign;
      PLI_INT32 node_ms_index;
      PLI_INT32 node_ls_index;
      PLI_INT32 node_mem_size;
      PLI_INT32 node_lhs_element;
      PLI_INT32 node_rhs_element;
      PLI_INT32*node_handle;
} s_tfnodeinfo, *p_tfnodeinfo;

/* values used in the node_type of the tfnodeinfo structure. */
#define tf_null_node    100
#define TF_NULL_NODE    tf_null_node
#define tf_reg_node     101
#define TF_REG_NODE     tf_reg_node
#define tf_integer_node 102
#define TF_INTEGER_NODE tf_integer_node
#define tf_time_node    103
#define TF_TIME_NODE    tf_time_node
#define tf_netvector_node 104
#define TF_NETVECTOR_NODE tf_netvector_node
#define tf_netscalar_node 105
#define TF_NETSCALAR_NODE tf_netscalar_node
#define tf_memory_node    106
#define TF_MEMORY_NODE    tf_memory_node
#define tf_real_node      107
#define TF_REAL_NODE      tf_real_node

/* Extern functions from the library. */
extern void io_printf (const char *, ...)
      __attribute__((format (printf,1,2)));
extern char* mc_scan_plusargs(char*plusarg);

extern int tf_asynchoff(void);
extern int tf_asynchon(void);

extern int tf_dofinish(void);

extern int tf_dostop(void);

extern void tf_error(const char*, ...)
      __attribute__((format (printf,1,2)));

extern char* tf_getcstringp(int nparam);

extern PLI_BYTE8* tf_getinstance(void);

extern int tf_getlongp(int*aof_highvalue, int pnum);

extern int tf_getp(int pnum);

extern char *tf_strgettime(void);
extern PLI_INT32 tf_gettime(void);

extern PLI_INT32 tf_getlongtime(PLI_INT32*);
extern PLI_INT32 tf_igetlongtime(PLI_INT32*, void*);

extern PLI_INT32 tf_gettimeprecision(void);
extern PLI_INT32 tf_igettimeprecision(void*);

extern PLI_INT32 tf_gettimeunit(void);
extern PLI_INT32 tf_igettimeunit(void*);

extern PLI_BYTE8* tf_getworkarea(void);

extern PLI_INT32 tf_message(PLI_INT32 level, char*facility,
			    char*messno, char*fmt, ...)
      __attribute__((format (printf,4,5)));

extern void tf_multiply_long(PLI_INT32*aof_low1, PLI_INT32*aof_high1,
			     PLI_INT32 aof_low2, PLI_INT32 aof_high2);

extern int tf_nump(void);
extern int tf_inump(void*);

/* IEEE1364 NOTE: tf_putlongp is listed as returning in in the header
   file shown in the standard, but as returning void in the detailed
   description of the function. So I call it void. Same for tf_putp. */
extern void tf_putlongp(int pnum, int lowvalue, int highvalue);

extern void tf_putp(int pnum, int value);

/* Activate the misctf function after a delay. The units are of the
   current scope. The tf_isetdelay variant specifies a particular
   system task instance to use as the context for the
   units. tf_getinstance gets that value. */
extern int tf_setdelay(PLI_INT32 delay);
extern int tf_isetdelay(PLI_INT32 delay, void* sys);

/* IEEE1364 NOTE: tf_setworkarea is listed as taking a PLI_BYTE8*, but
   that is silly, it really takes any kind of pointer. Taking void* is
   compatible with those who pass a PLI_BYTE8*. */
extern PLI_INT32 tf_setworkarea(void*workarea);

/* Return the complete, hierarchical name of the current scope. The
   current scope is the scope containing the currently running system
   task call. */
extern char* tf_spname(void);

extern PLI_INT32 tf_synchronize(void);
extern PLI_INT32 tf_isynchronize(void* sys);

extern PLI_INT32 tf_typep(PLI_INT32 narg);

extern void tf_warning(const char*, ...)
      __attribute__((format (printf,1,2)));

EXTERN_C_END

/*
 * $Log: veriuser.h,v $
 * Revision 1.27  2003/05/28 03:38:05  steve
 *  Implement tf_inump
 *
 * Revision 1.26  2003/05/28 03:14:20  steve
 *  Missing time related declarations.
 *
 * Revision 1.25  2003/05/18 00:16:35  steve
 *  Add PLI_TRACE tracing of PLI1 modules.
 *
 *  Add tf_isetdelay and friends, and add
 *  callback return values for acc_vcl support.
 *
 * Revision 1.24  2003/04/23 15:01:29  steve
 *  Add tf_synchronize and tf_multiply_long.
 *
 * Revision 1.23  2003/04/12 18:57:13  steve
 *  More acc_ function stubs.
 *
 * Revision 1.22  2003/03/13 04:35:09  steve
 *  Add a bunch of new acc_ and tf_ functions.
 *
 * Revision 1.21  2003/02/26 01:25:51  steve
 *  Document new PLI1 bootstrap interface.
 *
 * Revision 1.20  2003/02/17 06:39:47  steve
 *  Add at least minimal implementations for several
 *  acc_ functions. Add support for standard ACC
 *  string handling.
 *
 *  Add the _pli_types.h header file to carry the
 *  IEEE1364-2001 standard PLI type declarations.
 *
 * Revision 1.19  2003/02/16 02:23:22  steve
 *  Change the IV veriusertfs_register to accept table pointers.
 *
 * Revision 1.18  2002/12/19 21:37:04  steve
 *  Add tf_message, tf_get/setworkarea, and
 *  ty_typep functions, along with defines
 *  related to these functions.
 *
 * Revision 1.17  2002/08/12 01:35:01  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.16  2002/06/11 03:29:14  steve
 *  Get tf_asynchon/off name right.
 *
 * Revision 1.15  2002/06/07 16:21:12  steve
 *  Add tf_putlongp and tf_putp.
 */
#endif
