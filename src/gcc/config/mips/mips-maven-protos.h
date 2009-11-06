/*======================================================================*/
/* mips-maven-protos.h : Prototypes of target machine                   */
/*======================================================================*/
/* Copyright (C) 1989-2005, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by A. Lichnewsky (lich@inria.inria.fr).
   Changed by Michael Meissner	(meissner@osf.org).
   64-bit r4000 support by Ian Lance Taylor (ian@cygnus.com) and
   Brendan Eich (brendan@microunity.com).

   This file is part of GCC.
    
   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
    
   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_MIPS_PROTOS_H
#define GCC_MIPS_PROTOS_H

/*----------------------------------------------------------------------*/
/* mips_symbol_context                                                  */
/*----------------------------------------------------------------------*/
/* Describes how a symbol is used.

   SYMBOL_CONTEXT_CALL
       The symbol is used as the target of a call instruction.

   SYMBOL_CONTEXT_LEA
       The symbol is used in a load-address operation.

   SYMBOL_CONTEXT_MEM
       The symbol is used as the address in a MEM. */

enum mips_symbol_context 
{
  SYMBOL_CONTEXT_CALL,
  SYMBOL_CONTEXT_LEA,
  SYMBOL_CONTEXT_MEM
};

/*----------------------------------------------------------------------*/
/* mips_symbol_type                                                     */
/*----------------------------------------------------------------------*/
/* Classifies a SYMBOL_REF, LABEL_REF or UNSPEC address.

   SYMBOL_ABSOLUTE
       The symbol's value will be calculated using absolute relocations,
       such as %hi and %lo.

   SYMBOL_GP_RELATIVE
       The symbol's value will be calculated by adding a 16-bit offset
       from $gp.

   SYMBOL_PC_RELATIVE
       The symbol's value will be calculated using a MIPS16 PC-relative
       calculation.

   SYMBOL_FORCE_TO_MEM
       The symbol's value must be forced to memory and loaded from there.

   SYMBOL_GOT_PAGE_OFST
       The symbol's value will be calculated by loading an address
       from the GOT and then applying a 16-bit offset.

   SYMBOL_GOT_DISP
       The symbol's value will be loaded directly from the GOT.

   SYMBOL_GOTOFF_PAGE
       An UNSPEC wrapper around a SYMBOL_GOT_PAGE_OFST.  It represents the
       offset from _gp of the GOT entry.

   SYMBOL_GOTOFF_DISP
       An UNSPEC wrapper around a SYMBOL_GOT_DISP.  It represents the
       the offset from _gp of the symbol's GOT entry.

   SYMBOL_GOTOFF_CALL
       Like SYMBOL_GOTOFF_DISP, but used when calling a global function.
       The GOT entry is allowed to point to a stub rather than to the
       function itself.

   SYMBOL_GOTOFF_LOADGP
       An UNSPEC wrapper around a function's address.  It represents the
       offset of _gp from the start of the function.

   SYMBOL_TLS
       A thread-local symbol.

   SYMBOL_TLSGD
   SYMBOL_TLSLDM
   SYMBOL_DTPREL
   SYMBOL_GOTTPREL
   SYMBOL_TPREL
       UNSPEC wrappers around SYMBOL_TLS, corresponding to the
       thread-local storage relocation operators.

   SYMBOL_32_HIGH
       For a 32-bit symbolic address X, this is the value of %hi(X).

   SYMBOL_64_HIGH
       For a 64-bit symbolic address X, this is the value of
       (%highest(X) << 16) + %higher(X).

   SYMBOL_64_MID
       For a 64-bit symbolic address X, this is the value of
       (%higher(X) << 16) + %hi(X).

   SYMBOL_64_LOW
       For a 64-bit symbolic address X, this is the value of
       (%hi(X) << 16) + %lo(X).

   SYMBOL_HALF
       An UNSPEC wrapper around any kind of address.  It represents the
       low 16 bits of that address. */

enum mips_symbol_type 
{
  SYMBOL_ABSOLUTE,
  SYMBOL_GP_RELATIVE,
  SYMBOL_PC_RELATIVE,
  SYMBOL_FORCE_TO_MEM,
  SYMBOL_GOT_PAGE_OFST,
  SYMBOL_GOT_DISP,
  SYMBOL_GOTOFF_PAGE,
  SYMBOL_GOTOFF_DISP,
  SYMBOL_GOTOFF_CALL,
  SYMBOL_GOTOFF_LOADGP,
  SYMBOL_TLS,
  SYMBOL_TLSGD,
  SYMBOL_TLSLDM,
  SYMBOL_DTPREL,
  SYMBOL_GOTTPREL,
  SYMBOL_TPREL,
  SYMBOL_32_HIGH,
  SYMBOL_64_HIGH,
  SYMBOL_64_MID,
  SYMBOL_64_LOW,
  SYMBOL_HALF
};
#define NUM_SYMBOL_TYPES (SYMBOL_HALF + 1)

/*----------------------------------------------------------------------*/
/* mips_loadgp_style                                                    */
/*----------------------------------------------------------------------*/
/* Identifiers a style of $gp initialization sequence.

   LOADGP_NONE
	No initialization sequence is needed.

   LOADGP_OLDABI
	The o32 and o64 PIC sequence (the kind traditionally generated
	by .cpload).

   LOADGP_NEWABI
	The n32 and n64 PIC sequence (the kind traditionally generated
	by .cpsetup).

   LOADGP_ABSOLUTE
	The GNU absolute sequence, as generated by loadgp_absolute.

   LOADGP_RTP
	The VxWorks RTP PIC sequence, as generated by loadgp_rtp.  */

enum mips_loadgp_style 
{
  LOADGP_NONE,
  LOADGP_OLDABI,
  LOADGP_NEWABI,
  LOADGP_ABSOLUTE,
  LOADGP_RTP
};

/*----------------------------------------------------------------------*/
/* mips16e_save_restore_info                                            */
/*----------------------------------------------------------------------*/

struct mips16e_save_restore_info;

/*----------------------------------------------------------------------*/
/* mips_call_type                                                       */
/*----------------------------------------------------------------------*/
/* Classifies a type of call.

   MIPS_CALL_NORMAL
	A normal call or call_value pattern.

   MIPS_CALL_SIBCALL
	A sibcall or sibcall_value pattern.

   MIPS_CALL_EPILOGUE
	A call inserted in the epilogue. */

enum mips_call_type 
{
  MIPS_CALL_NORMAL,
  MIPS_CALL_SIBCALL,
  MIPS_CALL_EPILOGUE
};

/*----------------------------------------------------------------------*/
/* New maven prototypes                                                 */
/*----------------------------------------------------------------------*/

extern const char* 
mips_maven_output_vector_move( enum machine_mode, rtx, rtx );

/*----------------------------------------------------------------------*/
/* Previous mips prototypes                                             */
/*----------------------------------------------------------------------*/

extern bool mips_symbolic_constant_p( rtx, enum mips_symbol_context,
                                      enum mips_symbol_type* );
extern int mips_regno_mode_ok_for_base_p( int, enum machine_mode, bool );
extern bool mips_legitimate_address_p( enum machine_mode, rtx, bool );
extern bool mips_stack_address_p( rtx, enum machine_mode );
extern int mips_address_insns( rtx, enum machine_mode, bool );
extern int mips_const_insns( rtx );
extern int mips_split_const_insns( rtx );
extern int mips_load_store_insns( rtx, rtx );
extern int mips_idiv_insns( void );
extern rtx mips_emit_move( rtx, rtx );
extern rtx mips_pic_base_register( rtx );
extern rtx mips_got_load( rtx, rtx, enum mips_symbol_type );
extern bool mips_split_symbol( rtx, rtx, enum machine_mode, rtx* );
extern rtx mips_unspec_address( rtx, enum mips_symbol_type );
extern bool mips_legitimize_address( rtx*, enum machine_mode );
extern void mips_move_integer( rtx, rtx, unsigned HOST_WIDE_INT );
extern bool mips_legitimize_move( enum machine_mode, rtx, rtx );

extern int m16_uimm3_b( rtx, enum machine_mode );
extern int m16_simm4_1( rtx, enum machine_mode );
extern int m16_nsimm4_1( rtx, enum machine_mode );
extern int m16_simm5_1( rtx, enum machine_mode );
extern int m16_nsimm5_1( rtx, enum machine_mode );
extern int m16_uimm5_4( rtx, enum machine_mode );
extern int m16_nuimm5_4( rtx, enum machine_mode );
extern int m16_simm8_1( rtx, enum machine_mode );
extern int m16_nsimm8_1( rtx, enum machine_mode );
extern int m16_uimm8_1( rtx, enum machine_mode );
extern int m16_nuimm8_1( rtx, enum machine_mode );
extern int m16_uimm8_m1_1( rtx, enum machine_mode );
extern int m16_uimm8_4( rtx, enum machine_mode );
extern int m16_nuimm8_4( rtx, enum machine_mode );
extern int m16_simm8_8( rtx, enum machine_mode );
extern int m16_nsimm8_8( rtx, enum machine_mode );

extern rtx mips_subword( rtx, bool );
extern bool mips_split_64bit_move_p( rtx, rtx );
extern void mips_split_doubleword_move( rtx, rtx );
extern const char* mips_output_move( rtx, rtx );
extern void mips_restore_gp( rtx );
#ifdef RTX_CODE
extern bool mips_expand_scc( enum rtx_code, rtx );
extern void mips_expand_conditional_branch( rtx*, enum rtx_code );
extern void mips_expand_vcondv2sf( rtx, rtx, rtx, enum rtx_code, rtx, rtx );
extern void mips_expand_conditional_move( rtx* );
extern void mips_expand_conditional_trap( enum rtx_code );
#endif
extern bool mips_use_pic_fn_addr_reg_p( const_rtx );
extern rtx mips_expand_call( enum mips_call_type, rtx, rtx, rtx, rtx, bool );
extern void mips_split_call( rtx, rtx );
extern void mips_expand_fcc_reload( rtx, rtx, rtx );
extern void mips_set_return_address( rtx, rtx );
extern bool mips_expand_block_move( rtx, rtx, rtx );
extern void mips_expand_synci_loop( rtx, rtx );

extern void mips_init_cumulative_args( CUMULATIVE_ARGS*, tree );
extern void mips_function_arg_advance( CUMULATIVE_ARGS*, enum machine_mode,
                                       tree, int );
extern rtx mips_function_arg( const CUMULATIVE_ARGS*,
                              enum machine_mode, tree, int );
extern int mips_function_arg_boundary( enum machine_mode, tree );
extern bool mips_pad_arg_upward( enum machine_mode, const_tree );
extern bool mips_pad_reg_upward( enum machine_mode, tree );

extern bool mips_expand_ext_as_unaligned_load( rtx, rtx, HOST_WIDE_INT,
                                               HOST_WIDE_INT );
extern bool mips_expand_ins_as_unaligned_store( rtx, rtx, HOST_WIDE_INT,
                                                HOST_WIDE_INT );
extern bool mips_mem_fits_mode_p( enum machine_mode mode, rtx x );
extern void mips_override_options( void );
extern void mips_conditional_register_usage( void );
extern void mips_order_regs_for_local_alloc( void );
extern HOST_WIDE_INT mips_debugger_offset( rtx, HOST_WIDE_INT );

extern void mips_print_operand( FILE*, rtx, int );
extern void mips_print_operand_address( FILE*, rtx );
extern void mips_output_external( FILE*, tree, const char* );
extern void mips_output_filename( FILE*, const char* );
extern void mips_output_ascii( FILE*, const char*, size_t );
extern void mips_output_aligned_decl_common( FILE*, tree, const char*,
                                             unsigned HOST_WIDE_INT,
                                             unsigned int );
extern void mips_declare_common_object( FILE*, const char*,
                                        const char*, unsigned HOST_WIDE_INT,
                                        unsigned int, bool );
extern void mips_declare_object( FILE*, const char*, const char*,
                                 const char*, ...) ATTRIBUTE_PRINTF_4;
extern void mips_declare_object_name( FILE*, const char*, tree );
extern void mips_finish_declare_object( FILE*, tree, int, int );

extern bool mips_small_data_pattern_p( rtx );
extern rtx mips_rewrite_small_data( rtx );
extern bool mips_frame_pointer_required( void );
extern HOST_WIDE_INT mips_initial_elimination_offset( int, int );
extern rtx mips_return_addr( int, rtx );
extern enum mips_loadgp_style mips_current_loadgp_style( void );
extern void mips_expand_prologue( void );
extern void mips_expand_before_return( void );
extern void mips_expand_epilogue( bool );
extern bool mips_can_use_return_insn( void );
extern rtx mips_function_value( const_tree, enum machine_mode );

extern bool mips_cannot_change_mode_class( enum machine_mode,
                                           enum machine_mode, enum reg_class );
extern bool mips_dangerous_for_la25_p( rtx );
extern bool mips_modes_tieable_p( enum machine_mode, enum machine_mode );
extern enum reg_class mips_preferred_reload_class( rtx, enum reg_class );
extern enum reg_class mips_secondary_reload_class( enum reg_class,
                                                   enum machine_mode,
                                                   rtx, bool );
extern int mips_class_max_nregs( enum reg_class, enum machine_mode );
extern int mips_register_move_cost( enum machine_mode, enum reg_class,
                                    enum reg_class );

extern int mips_adjust_insn_length( rtx, int );
extern const char* mips_output_load_label( void );
extern const char* mips_output_conditional_branch( rtx, rtx*, const char*,
                                                   const char* );
extern const char* mips_output_order_conditional_branch( rtx, rtx*, bool );
extern const char* mips_output_sync_loop( const char* );
extern const char* mips_output_division( const char*, rtx* );
extern unsigned int mips_hard_regno_nregs( int, enum machine_mode );
extern bool mips_linked_madd_p( rtx, rtx );
extern bool mips_store_data_bypass_p( rtx, rtx );
extern rtx mips_prefetch_cookie( rtx, rtx );

extern void irix_asm_output_align( FILE*, unsigned );
extern const char* current_section_name( void );
extern unsigned int current_section_flags( void );
extern bool mips_use_ins_ext_p( rtx, HOST_WIDE_INT, HOST_WIDE_INT );

extern const char* mips16e_output_save_restore( rtx, HOST_WIDE_INT );
extern bool mips16e_save_restore_pattern_p( rtx, HOST_WIDE_INT,
                                            struct mips16e_save_restore_info* );

extern bool mask_low_and_shift_p( enum machine_mode, rtx, rtx, int );
extern int mask_low_and_shift_len( enum machine_mode, rtx, rtx );

union mips_gen_fn_ptrs
{
  rtx(*fn_6)( rtx, rtx, rtx, rtx, rtx, rtx );
  rtx(*fn_5)( rtx, rtx, rtx, rtx, rtx );
  rtx(*fn_4)( rtx, rtx, rtx, rtx );
};

extern void mips_expand_atomic_qihi( union mips_gen_fn_ptrs,
                                     rtx, rtx, rtx, rtx );

extern void mips_expand_vector_init( rtx, rtx );

#endif /* GCC_MIPS_PROTOS_H */