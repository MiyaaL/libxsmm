/******************************************************************************
* Copyright (c) Friedrich Schiller University Jena - All rights reserved.     *
*               Intel Corporation - All rights reserved                       *
* This file is part of the LIBXSMM library.                                   *
*                                                                             *
* For information on the license, see the LICENSE file.                       *
* Further information: https://github.com/libxsmm/libxsmm/                    *
* SPDX-License-Identifier: BSD-3-Clause                                       *
******************************************************************************/
/* Alexander Breuer (Univ. Jena), Alexander Heinecke, Evangelos Georganas (Intel Corp.)
******************************************************************************/
#include "generator_gemm_common.h"
#include "generator_rv64_instructions.h"
#include "generator_gemm_common_rv64.h"
#include "generator_common.h"
#include "generator_mateltwise_unary_binary_rv64.h"
#include "generator_common_rv64.h"
#include "generator_mateltwise_rv64.h"

#if 0
#include "generator_mateltwise_transform_common.h"
#include "generator_mateltwise_transform_rv64_sve.h"
#endif

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_getval_stack_var_rv64( libxsmm_generated_code*             io_generated_code,
                                                      libxsmm_gemm_stack_var              stack_var,
                                                      unsigned int                        i_gp_reg ) {
#if 0
  int offset = libxsmm_generator_gemm_get_rbp_relative_offset(stack_var);
  /* make sure we requested a legal stack var */
  if (offset == 0) {
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_GENERAL );
    return;
  }
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_I, LIBXSMM_RV64_GP_REG_X29, i_gp_reg, -offset, 0 );
  libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF, i_gp_reg, LIBXSMM_RV64_GP_REG_UNDEF, 0, i_gp_reg );
#endif
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_setval_stack_var_rv64( libxsmm_generated_code*             io_generated_code,
                                                      libxsmm_gemm_stack_var              stack_var,
                                                      unsigned int                        i_aux_reg,
                                                      unsigned int                        i_gp_reg ) {
#if 0
  int offset = libxsmm_generator_gemm_get_rbp_relative_offset(stack_var);
  /* make sure we requested to set  a legal stack var */
  if (offset >= 0) {
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_GENERAL );
    return;
  }
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_I, LIBXSMM_RV64_GP_REG_X29, i_aux_reg, -offset, 0 );
  libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_STR_I_OFF, i_aux_reg, LIBXSMM_RV64_GP_REG_UNDEF, 0, i_gp_reg );
#endif
}

#if 0
LIBXSMM_API_INTERN
void libxsmm_generator_gemm_setup_stack_frame_fill_ext_gemm_stack_vars_rv64( libxsmm_generated_code*            io_generated_code,
    const libxsmm_gemm_descriptor*      i_xgemm_desc,
    libxsmm_micro_kernel_config*        i_micro_kernel_config,
    const libxsmm_gp_reg_mapping*       i_gp_reg_mapping ) {

  int is_stride_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_STRIDE) > 0) ? 1 : 0;
  int is_offset_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_OFFSET) > 0) ? 1 : 0;
  int is_address_brgemm = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_ADDRESS) > 0) ? 1 : 0;
  int is_brgemm         = ((is_stride_brgemm == 1) || (is_offset_brgemm == 1) || (is_address_brgemm == 1)) ? 1 : 0;
  int has_scf           = ((LIBXSMM_DATATYPE_I8 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype )) && (LIBXSMM_DATATYPE_F32 == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ))) ? 1 : 0;
  int has_A_pf_ptr      = (i_xgemm_desc->prefetch == LIBXSMM_GEMM_PREFETCH_AL2_AHEAD || i_xgemm_desc->prefetch == LIBXSMM_GEMM_PREFETCH_AL2BL2_VIA_C_AHEAD) ? 1 : 0;
  int has_B_pf_ptr      = (i_xgemm_desc->prefetch == LIBXSMM_GEMM_PREFETCH_BL2_VIA_C || i_xgemm_desc->prefetch == LIBXSMM_GEMM_PREFETCH_AL2BL2_VIA_C_AHEAD ||
      i_xgemm_desc->prefetch == LIBXSMM_GEMM_PREFETCH_AL2BL2_VIA_C /*|| i_xgemm_desc->prefetch == LIBXSMM_PREFETCH_AL2CL2BL2_VIA_C*/) ? 1 : 0;
  unsigned int struct_reg = i_gp_reg_mapping->gp_reg_help_1;
  unsigned int val_reg    = i_gp_reg_mapping->gp_reg_help_0;
  unsigned int aux_reg    = i_gp_reg_mapping->gp_reg_help_2;

  if (has_scf == 1) {
    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 112, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_INT8_SCF, aux_reg, val_reg );
  }

  if (has_A_pf_ptr == 1) {
    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 56, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_PFA_PTR, aux_reg, val_reg );
  }

  if (has_B_pf_ptr == 1) {
    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 88, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_PFB_PTR, aux_reg, val_reg );
  }

  if ((is_brgemm == 1) && ((libxsmm_cpuid_arm_mmla_gemm_pack_b_to_vnnit_on_stack() > 0) || (i_micro_kernel_config->decompress_A == 1) || ((LIBXSMM_GEMM_FLAG_TRANS_A & i_xgemm_desc->flags) > 0))) {
    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        i_gp_reg_mapping->gp_reg_reduce_count, LIBXSMM_RV64_GP_REG_UNDEF, 0, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, aux_reg, val_reg );
  }

  if (is_offset_brgemm == 1) {
    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 40, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_A_OFFS_BRGEMM_PTR, aux_reg, val_reg );

    libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
        struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 72, val_reg);
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_B_OFFS_BRGEMM_PTR, aux_reg, val_reg );
  }

  if (i_micro_kernel_config->fused_eltwise == 1) {
    if (i_micro_kernel_config->has_colbias_act_fused == 1) {
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 128, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_BIAS_PTR, aux_reg, val_reg );
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 104, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_OUTPUT_PTR, aux_reg, val_reg );
    }
    if (i_micro_kernel_config->decompress_A == 1) {
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 48, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_BITMAP_PTR, aux_reg, val_reg );
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 160, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_DECOMPRESS_BUF, aux_reg, val_reg );
    }
    if (i_micro_kernel_config->vnni_cvt_output_ext_buf == 1) {
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 104, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_OUTPUT_PTR, aux_reg, val_reg );
    }
    if (i_micro_kernel_config->fused_relu_bwd == 1) {
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 104, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_ELT_RELU_BITMASK_PTR, aux_reg, val_reg );
    }
    if (i_micro_kernel_config->norm_to_normT_B_ext_buf == 1) {
      libxsmm_rv64_instruction_alu_move( io_generated_code, LIBXSMM_RV64_INSTR_GP_LDR_I_OFF,
          struct_reg, LIBXSMM_RV64_GP_REG_UNDEF, 192, val_reg);
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_TRANS_EXT_BUF_B, aux_reg, val_reg );
    }
  }
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_setup_stack_frame_allocate_scratch_rv64( libxsmm_generated_code*            io_generated_code,
    const libxsmm_gemm_descriptor*      i_xgemm_desc,
    const libxsmm_gp_reg_mapping*       i_gp_reg_mapping,
    libxsmm_micro_kernel_config*        i_micro_kernel_config ) {
  unsigned int gemm_scratch_size      = 0;
  unsigned int scratch_pad_size       = 0;
  unsigned int temp_reg = i_gp_reg_mapping->gp_reg_help_1;
  unsigned int temp_reg2 = i_gp_reg_mapping->gp_reg_help_0;

  /* Allocate scratch for stashing 32 zmms */
  if ( ((LIBXSMM_GEMM_FLAG_USE_XGEMM_EXT_ABI & i_xgemm_desc->flags) == LIBXSMM_GEMM_FLAG_USE_XGEMM_EXT_ABI) ) {
    gemm_scratch_size = 32 * 64;
  }
  if (i_micro_kernel_config->vnni_format_C > 0) {
    gemm_scratch_size = 32 * 64 + i_xgemm_desc->n * i_xgemm_desc->m * 4;
  }

  scratch_pad_size  = (gemm_scratch_size % 64 == 0) ? 0 : ((gemm_scratch_size + 63)/64) * 64 - gemm_scratch_size;
  gemm_scratch_size += scratch_pad_size;

  /* Allocate scratch for the A transpose */
  if ( (LIBXSMM_GEMM_FLAG_TRANS_A & i_xgemm_desc->flags) > 0 ) {
    int is_stride_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_STRIDE) > 0) ? 1 : 0;
    int is_offset_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_OFFSET) > 0) ? 1 : 0;
    int is_address_brgemm = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_ADDRESS) > 0) ? 1 : 0;
    int is_brgemm         = ((is_stride_brgemm == 1) || (is_offset_brgemm == 1) || (is_address_brgemm == 1)) ? 1 : 0;
    unsigned int inp_dtype_size = ((LIBXSMM_GEMM_FLAG_TRANS_A & i_xgemm_desc->flags) > 0) ?  LIBXSMM_TYPESIZE(LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC(i_xgemm_desc->datatype)) : 4;
    unsigned int a_size  = (i_xgemm_desc->m * i_xgemm_desc->k) * inp_dtype_size;
    unsigned int b_size  = (i_xgemm_desc->k * i_xgemm_desc->n) * inp_dtype_size;
    unsigned int a_pad  = (a_size % 64 == 0) ? 0 : ((a_size + 63)/64) * 64 - a_size;
    unsigned int b_pad  = (b_size % 64 == 0) ? 0 : ((b_size + 63)/64) * 64 - b_size;
    a_size += a_pad;
    b_size += b_pad;

    if (is_brgemm == 0) {
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg, 0, 0 );
      libxsmm_rv64_instruction_alu_compute_imm64( io_generated_code, LIBXSMM_RV64_INSTR_GP_META_SUB, temp_reg, temp_reg2, temp_reg, a_size );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_TRANSPOSE_PTR, temp_reg2, temp_reg );
    } else {
      libxsmm_generator_gemm_getval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, temp_reg);
      libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg2, a_size );
      libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_MUL, temp_reg, temp_reg2, temp_reg, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
      libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_TRANSPOSE_PTR, temp_reg, temp_reg2 );
      if (is_offset_brgemm > 0 || is_address_brgemm > 0) {
        libxsmm_generator_gemm_getval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, temp_reg);
        libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg2, b_size );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_MUL, temp_reg, temp_reg2, temp_reg, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
        libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_B_EMU_PTR, temp_reg, temp_reg2 );
      }
    }
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg, 0, 0 );
    libxsmm_rv64_instruction_alu_compute_imm64( io_generated_code, LIBXSMM_RV64_INSTR_GP_META_SUB, temp_reg, temp_reg2, temp_reg, 128 );
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_MELTW_STRUCT_PTR, temp_reg2, temp_reg );
  }

  if (libxsmm_cpuid_arm_mmla_gemm_pack_b_to_vnnit_on_stack() > 0) {
    int is_stride_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_STRIDE) > 0) ? 1 : 0;
    int is_offset_brgemm  = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_OFFSET) > 0) ? 1 : 0;
    int is_address_brgemm = ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_BATCH_REDUCE_ADDRESS) > 0) ? 1 : 0;
    int is_brgemm         = ((is_stride_brgemm == 1) || (is_offset_brgemm == 1) || (is_address_brgemm == 1)) ? 1 : 0;
    unsigned int inp_dtype_size =  LIBXSMM_TYPESIZE(LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC(i_xgemm_desc->datatype));
    unsigned int a_size  = (i_xgemm_desc->m * i_xgemm_desc->k) * inp_dtype_size;
    unsigned int b_size  = (i_xgemm_desc->k * i_xgemm_desc->n) * inp_dtype_size;
    unsigned int a_pad  = (a_size % 64 == 0) ? 0 : ((a_size + 63)/64) * 64 - a_size;
    unsigned int b_pad  = (b_size % 64 == 0) ? 0 : ((b_size + 63)/64) * 64 - b_size;
    a_size += a_pad;
    b_size += b_pad;

    if (is_brgemm == 0) {
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg, 0, 0 );
      libxsmm_rv64_instruction_alu_compute_imm64( io_generated_code, LIBXSMM_RV64_INSTR_GP_META_SUB, temp_reg, temp_reg2, temp_reg, b_size );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_B_EMU_PTR, temp_reg2, temp_reg );
    } else {
      libxsmm_generator_gemm_getval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, temp_reg);
      libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg2, b_size );
      libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_MUL, temp_reg, temp_reg2, temp_reg, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
      libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
      libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_B_EMU_PTR, temp_reg, temp_reg2 );
      if (is_offset_brgemm > 0 || is_address_brgemm > 0) {
        libxsmm_generator_gemm_getval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, temp_reg);
        libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg2, a_size );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_MUL, temp_reg, temp_reg2, temp_reg, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
        libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_A_EMU_PTR, temp_reg, temp_reg2 );

        libxsmm_generator_gemm_getval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_BRCOUNT, temp_reg);
        libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg2, b_size );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_MUL, temp_reg, temp_reg2, temp_reg, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
        libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
        libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
        libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_B_EMU_PTR, temp_reg, temp_reg2 );
      }
    }
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg, 0, 0 );
    libxsmm_rv64_instruction_alu_compute_imm64( io_generated_code, LIBXSMM_RV64_INSTR_GP_META_SUB, temp_reg, temp_reg2, temp_reg, 128 );
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_MELTW_STRUCT_PTR, temp_reg2, temp_reg );
  }

  if (gemm_scratch_size > 0) {
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg, 0, 0 );
    libxsmm_rv64_instruction_alu_compute_imm64( io_generated_code, LIBXSMM_RV64_INSTR_GP_META_SUB, temp_reg, temp_reg2, temp_reg, gemm_scratch_size );
    libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
    libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_GEMM_SCRATCH_PTR, temp_reg2, temp_reg );
  }

}
#endif

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_setup_stack_frame_rv64( libxsmm_generated_code*            io_generated_code,
    const libxsmm_gemm_descriptor*      i_xgemm_desc,
    const libxsmm_gp_reg_mapping*       i_gp_reg_mapping,
    libxsmm_micro_kernel_config*        i_micro_kernel_config ) {
#if 0
  unsigned int temp_reg = i_gp_reg_mapping->gp_reg_help_1;
  unsigned int temp_reg2 = i_gp_reg_mapping->gp_reg_help_0;

  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, LIBXSMM_RV64_GP_REG_X29, 0, 0 );
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_SUB_I, LIBXSMM_RV64_GP_REG_XSP, LIBXSMM_RV64_GP_REG_XSP, 168, 0 );

  /* The stack now looks like this:
   *      10th param (if applicable)                <-- RBP+80
   *      9th param (if applicable)                 <-- RBP+72
   *      8th param (if applicable)                 <-- RBP+64
   *      7th param (if applicable)                 <-- RBP+56
   *      Return address                            <-- RBP+48
   *      Calle SAVED-regs                          <-- RBP[+8,+16,+24,+32,+40]
   *      Entry/saved RBP                           <-- RBP
   *      prefetch A ptr                            <-- RBP-8
   *      prefetch B ptr                            <-- RBP-16
   *      Offset A array ptr                        <-- RBP-24
   *      Offset B array ptr                        <-- RBP-32
   *      Int8 scaling factor                       <-- RBP-40
   *      GEMM_scratch ptr in stack (to be filled)  <-- RBP-48
   *      Eltwise bias ptr                          <-- RBP-56
   *      Eltwise output_ptr                        <-- RBP-64
   *      Eltwise buf1_ptr                          <-- RBP-72
   *      Eltwise buf2_ptr                          <-- RBP-80
   *      Batch-reduce count                        <-- RBP-88,
   *      Transpose A ptr                           <-- RBP-96,
   *      AVX2 Mask                                 <-- RBP-104,
   *      AVX2 low precision helper                 <-- RBP-112, RSP
   */
  if ( ((LIBXSMM_GEMM_FLAG_USE_XGEMM_EXT_ABI & i_xgemm_desc->flags) == LIBXSMM_GEMM_FLAG_USE_XGEMM_EXT_ABI) || (libxsmm_cpuid_arm_mmla_gemm_pack_b_to_vnnit_on_stack() > 0) || ((LIBXSMM_GEMM_FLAG_TRANS_A & i_xgemm_desc->flags) > 0) ) {
    libxsmm_generator_gemm_setup_stack_frame_fill_ext_gemm_stack_vars_rv64( io_generated_code, i_xgemm_desc, i_micro_kernel_config, i_gp_reg_mapping );
  } else {
    int has_scf = ((LIBXSMM_DATATYPE_I8 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype )) && (LIBXSMM_DATATYPE_F32 == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ))) ? 1 : 0;
    if (has_scf == 1) {
      libxsmm_generator_gemm_setval_stack_var_rv64( io_generated_code, LIBXSMM_GEMM_STACK_VAR_INT8_SCF, temp_reg, i_gp_reg_mapping->gp_reg_scf );
    }
  }

  /* Now align RSP to 64 byte boundary */
  libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, temp_reg, 0xFFFFFFFFFFFFFFC0 );
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_XSP, temp_reg2, 0, 0 );
  libxsmm_rv64_instruction_alu_compute_shifted_reg( io_generated_code, LIBXSMM_RV64_INSTR_GP_AND_SR, temp_reg2, temp_reg, temp_reg2, 0, LIBXSMM_RV64_SHIFTMODE_LSL );
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, temp_reg2, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );

  /* Now alllocate in stack required GEMM scratch if necessary*/
  libxsmm_generator_gemm_setup_stack_frame_allocate_scratch_rv64( io_generated_code, i_xgemm_desc, i_gp_reg_mapping, i_micro_kernel_config );

  /* The stack at exit of setup looks like this:
   *
   *      10th param (if applicable)            <-- RBP+80
   *      9th param (if applicable)             <-- RBP+72
   *      8th param (if applicable)             <-- RBP+64
   *      7th param (if applicable)             <-- RBP+56
   *      Return address                        <-- RBP+48
   *      Calle SAVED-regs                      <-- RBP[+8,+16,+24,+32,+40]
   *      Entry/saved RBP                       <-- RBP
   *      prefetch A ptr                        <-- RBP-8
   *      prefetch B ptr                        <-- RBP-16
   *      Offset A array ptr                    <-- RBP-24
   *      Offset B array ptr                    <-- RBP-32
   *      Int8 scaling factor                   <-- RBP-40
   *      GEMM_scratch ptr in stack             <-- RBP-48
   *      Eltwise bias ptr                      <-- RBP-56
   *      Eltwise output_ptr                    <-- RBP-64
   *      Eltwise buf1_ptr                      <-- RBP-72
   *      Eltwise buf2_ptr                      <-- RBP-80
   *      Batch-reduce count                    <-- RBP-88
   *      Transpose A ptr                       <-- RBP-96
   *      AVX2 Mask                             <-- RBP-104
   *      AVX2 low precision helper             <-- RBP-112, RSP
   *      [ Potential pad for 64b align ]
   *      AV2 mask, 64b aligned                 <-- (RBP-104) contains this address
   *      AV2 low precision helper, 64b aligned <-- (RBP-112) contains this address
   *      GEMM scratch, 64b aligned             <-- (RBP-48) contains this address
   */
#endif
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_destroy_stack_frame_rv64( libxsmm_generated_code* io_generated_code) {
#if 0
  libxsmm_rv64_instruction_alu_compute_imm12( io_generated_code, LIBXSMM_RV64_INSTR_GP_ADD_I, LIBXSMM_RV64_GP_REG_X29, LIBXSMM_RV64_GP_REG_XSP, 0, 0 );
#endif
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_init_micro_kernel_config_rv64( libxsmm_micro_kernel_config*   io_micro_kernel_config,
                                                              const unsigned int             i_arch,
                                                              const libxsmm_gemm_descriptor* i_xgemm_desc ) {
  memset(io_micro_kernel_config, 0, sizeof(*io_micro_kernel_config)); /* avoid warning "maybe used uninitialized" */
  libxsmm_generator_gemm_setup_fusion_microkernel_properties(i_xgemm_desc, io_micro_kernel_config);
  if ( i_arch == LIBXSMM_RV64 ) {
    io_micro_kernel_config->instruction_set = i_arch;
    io_micro_kernel_config->vector_reg_count = 32;
    io_micro_kernel_config->use_masking_a_c = 0;
    io_micro_kernel_config->vector_name = 'v';
    if ( LIBXSMM_DATATYPE_F64 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) {
      io_micro_kernel_config->vector_length = libxsmm_cpuid_mvl_rv64()/64;
      io_micro_kernel_config->datatype_size_in = 8;
      io_micro_kernel_config->datatype_size_out = 8;
      io_micro_kernel_config->a_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE64_V;
      io_micro_kernel_config->b_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE64_V;
      io_micro_kernel_config->b_shuff_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->c_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VSE64_V;
      io_micro_kernel_config->c_vmove_nts_instruction = LIBXSMM_RV64_INSTR_GP_VSE64_V;
      io_micro_kernel_config->vxor_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->vmul_instruction = LIBXSMM_RV64_INSTR_GP_VFMACC_VV;
      io_micro_kernel_config->vadd_instruction = LIBXSMM_RV64_INSTR_UNDEF;
    } else if ( LIBXSMM_DATATYPE_F32 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) {
      io_micro_kernel_config->vector_length = libxsmm_cpuid_mvl_rv64()/32;
      io_micro_kernel_config->datatype_size_in = 4;
      io_micro_kernel_config->datatype_size_out = 4;
      io_micro_kernel_config->a_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE32_V;
      io_micro_kernel_config->b_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE32_V;
      io_micro_kernel_config->b_shuff_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->c_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VSE32_V;
      io_micro_kernel_config->c_vmove_nts_instruction = LIBXSMM_RV64_INSTR_GP_VSE32_V;
      io_micro_kernel_config->vxor_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->vmul_instruction = LIBXSMM_RV64_INSTR_GP_VFMACC_VV;
      io_micro_kernel_config->vadd_instruction = LIBXSMM_RV64_INSTR_UNDEF;
    } else if ( LIBXSMM_DATATYPE_BF16 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) { /* TODO (MMLA): do a proper integration; right now just assumes A in MMLA format, rest col-major */
      io_micro_kernel_config->vector_length = libxsmm_cpuid_mvl_rv64()/16;
      io_micro_kernel_config->datatype_size_in = 2;
      if ( LIBXSMM_DATATYPE_F32 == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) {
        io_micro_kernel_config->datatype_size_out = 4;
      } else if ( LIBXSMM_DATATYPE_BF16 == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) {
        io_micro_kernel_config->datatype_size_out = 2;
      } else {
        /* Should not happen */
      }
      io_micro_kernel_config->a_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE16_V;
      io_micro_kernel_config->b_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VLE16_V;
      io_micro_kernel_config->b_shuff_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->c_vmove_instruction = LIBXSMM_RV64_INSTR_GP_VSE16_V;
      io_micro_kernel_config->c_vmove_nts_instruction = LIBXSMM_RV64_INSTR_GP_VSE16_V;
      io_micro_kernel_config->vxor_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->vmul_instruction = LIBXSMM_RV64_INSTR_UNDEF;
      io_micro_kernel_config->vadd_instruction = LIBXSMM_RV64_INSTR_UNDEF;
    } else {
      /* should not happend */
    }
  } else {
      /* should not happend */
  }
}

LIBXSMM_API_INTERN
unsigned int libxsmm_generator_gemm_rv64_get_max_n_blocking( const libxsmm_micro_kernel_config*  i_micro_kernel_config,
                                                                const libxsmm_gemm_descriptor*      i_xgemm_desc,
                                                                const unsigned int                  i_arch ) {
  LIBXSMM_UNUSED( i_micro_kernel_config );
  LIBXSMM_UNUSED( i_xgemm_desc );

  if ( i_arch == LIBXSMM_RV64 ) {
    /* return libxsmm_cpuid_mvl_rv64(); */
    return 10;
  } else {
    return 0;
  }
}

LIBXSMM_API_INTERN
unsigned int libxsmm_generator_gemm_rv64_get_initial_m_blocking( libxsmm_micro_kernel_config*    io_micro_kernel_config,
                                                                    const libxsmm_gemm_descriptor*  i_xgemm_desc,
                                                                    const unsigned int              i_arch ) {
  unsigned int l_m_blocking = 1;

  if ( ( i_arch == LIBXSMM_RV64 ) && ( ( LIBXSMM_DATATYPE_F32  == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) ||
                                                                                           ( LIBXSMM_DATATYPE_I32  == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) ||
                                                                                           ( LIBXSMM_DATATYPE_BF16 == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) )    ) ) {
    /* Remark switching ti OUT datatype check here to cover BF16 in, Fp32/Int32 out kernel with the same logic */
    /* TODO: check if there is a better blocking strategy */
    if ( i_xgemm_desc->m >= 32 ) {
      l_m_blocking = 32;
    } else if ( i_xgemm_desc->m >= 16 ) {
      l_m_blocking = 16;
    } else if ( i_xgemm_desc->m >= 8 ) {
      l_m_blocking = 8;
    } else{
      l_m_blocking = i_xgemm_desc->m;
    }
  } else if ( ( i_arch == LIBXSMM_RV64 ) && ( LIBXSMM_DATATYPE_F64 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) ) {
    /* TODO: check if there is a better blocking strategy */
    if ( i_xgemm_desc->m >= 16 ) {
      l_m_blocking = 16;
    } else
    if ( i_xgemm_desc->m >= 8 ) {
      l_m_blocking = 8;
    } else {
      l_m_blocking = i_xgemm_desc->m;
    }
  } else {
    /* we should never end up here, if we do let the user know */
    /*LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_M_BLOCK );
    return 0;*/
  }

  libxsmm_generator_gemm_init_micro_kernel_config_rv64( io_micro_kernel_config, i_arch, i_xgemm_desc );

  return l_m_blocking;
}

LIBXSMM_API_INTERN
unsigned int libxsmm_generator_gemm_rv64_update_m_blocking( libxsmm_micro_kernel_config*   io_micro_kernel_config,
                                                            const libxsmm_gemm_descriptor* i_xgemm_desc,
                                                            const unsigned int             i_arch,
                                                            const unsigned int             i_current_m_blocking ) {
  unsigned int l_m_blocking = 0;

  if ( ( i_arch == LIBXSMM_RV64 ) && (( LIBXSMM_DATATYPE_F32  == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) || ( LIBXSMM_DATATYPE_I32  == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) ) || ( LIBXSMM_DATATYPE_BF16  == LIBXSMM_GEMM_GETENUM_C_PREC( i_xgemm_desc->datatype ) )) ) {
    /* Remark switching ti OUT datatype check here to cover BF16 in, Fp32 out kernel with the same logic */
    if (i_current_m_blocking == 32 ) {
      if ((i_xgemm_desc->m % 32) >= 16){
        l_m_blocking = 16;
      }
      else if ((i_xgemm_desc->m % 16) >= 8){
        l_m_blocking = 8;
      }
      else {
        l_m_blocking = i_xgemm_desc->m % 16;
      }
    } else if (i_current_m_blocking == 16 ) {
      if ((i_xgemm_desc->m % 16) >= 8){
        l_m_blocking = 8;
      }
      else {
        l_m_blocking = i_xgemm_desc->m % 16;
      }
      /* we are done with m_blocking */
    } else if (i_current_m_blocking == 8 ) {
      l_m_blocking = i_xgemm_desc->m % 8;
      /* we are done with m_blocking */
    }
  } else if ( ( i_arch == LIBXSMM_RV64 ) && ( LIBXSMM_DATATYPE_F64 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) ) {
    if (i_current_m_blocking == 8) {
      l_m_blocking = i_xgemm_desc->m % 8;
    } else {
      /* we are done with m_blocking */
    }
  } else {
    /* we should never end up here, if we do let the user know */
    /*LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_M_BLOCK );
    return 0;*/
  }

  libxsmm_generator_gemm_init_micro_kernel_config_rv64( io_micro_kernel_config, i_arch, i_xgemm_desc );

  return l_m_blocking;
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_rv64_setup_n_blocking( libxsmm_generated_code*        io_generated_code,
                                                   libxsmm_micro_kernel_config*   io_micro_kernel_config,
                                                   const libxsmm_gemm_descriptor* i_xgemm_desc,
                                                   const unsigned int             i_arch,
                                                   unsigned int*                  o_n_N,
                                                   unsigned int*                  o_n_n) {
  unsigned int max_n_blocking = libxsmm_generator_gemm_rv64_get_max_n_blocking( io_micro_kernel_config, i_xgemm_desc, i_arch );
  const unsigned int init_m_blocking = libxsmm_generator_gemm_rv64_get_initial_m_blocking( io_micro_kernel_config, i_xgemm_desc, i_arch );
  unsigned int init_m_blocks = 0;

  /* check for valid values */
  if ( max_n_blocking == 0 || io_micro_kernel_config->vector_length == 0 ) {
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_N_BLOCK );
    return;
  }

  init_m_blocks = LIBXSMM_UPDIV(init_m_blocking, io_micro_kernel_config->vector_length);

  /* increment m register blocking in case of 2 remainder registers */
  if (init_m_blocking > 0) {
    if ( (init_m_blocking % io_micro_kernel_config->vector_length == 3) || ((i_xgemm_desc->m % init_m_blocking) % io_micro_kernel_config->vector_length == 3) ) {
      init_m_blocks++;
    }
  }

  while ((init_m_blocks * max_n_blocking + init_m_blocks + 1) > io_micro_kernel_config->vector_reg_count) {
    max_n_blocking--;
  }

  if ((i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_VNNI_A) > 0 ) {
    max_n_blocking = 16;
    if (i_xgemm_desc->n < 16) {
      o_n_N[0] = i_xgemm_desc->n;
      o_n_n[0] = i_xgemm_desc->n;
      o_n_N[1] = 0;
      o_n_n[1] = 0;
    } else {
      o_n_N[0] = i_xgemm_desc->n - i_xgemm_desc->n % max_n_blocking;
      o_n_n[0] = max_n_blocking;
      o_n_N[1] = i_xgemm_desc->n % max_n_blocking;
      o_n_n[1] = i_xgemm_desc->n % max_n_blocking;
    }
  } else {
    libxsmm_compute_equalized_blocking( i_xgemm_desc->n, max_n_blocking, &(o_n_N[0]), &(o_n_n[0]), &(o_n_N[1]), &(o_n_n[1]) );
  }
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_rv64_setup_k_strides( libxsmm_generated_code*            io_generated_code,
                                                     const libxsmm_gp_reg_mapping*      i_gp_reg_mapping,
                                                     const libxsmm_micro_kernel_config* i_micro_kernel_config,
                                                     const libxsmm_gemm_descriptor*     i_xgemm_desc,
                                                     const unsigned int                 i_m_blocking,
                                                     const unsigned int                 i_n_blocking ) {
  /* temp variable for b-offset to handle no-trans/trans B */
  int l_b_offset = 0;
  /* register blocking counter in n */
  libxsmm_blasint l_rv64_bfdot = (libxsmm_blasint)libxsmm_cpuid_arm_use_bfdot();
  libxsmm_blasint l_rv64_i8dot = (libxsmm_blasint)libxsmm_cpuid_arm_use_i8dot();

  /* preload offset of B */
  if ( (io_generated_code->arch >= LIBXSMM_RV64) ) {
    if ( (i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_TRANS_B) > 0 ) {
      l_b_offset = (i_xgemm_desc->ldb * i_micro_kernel_config->datatype_size_in) - (i_micro_kernel_config->datatype_size_in*i_n_blocking);
    } else {
      l_b_offset = i_xgemm_desc->ldb * i_micro_kernel_config->datatype_size_in;
      libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_2, l_b_offset );

      l_b_offset = i_xgemm_desc->ldb - i_n_blocking;
      l_b_offset *= i_micro_kernel_config->datatype_size_in;;
    }
  } else {
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_ARCH );
    return;
  }

  libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_1, l_b_offset );

  /* preload offset of A */
  if ( (l_rv64_bfdot != 0) && ( LIBXSMM_DATATYPE_BF16 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) ) {
    libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_0,
                                             ((long long)i_xgemm_desc->lda - i_m_blocking) * 2*i_micro_kernel_config->datatype_size_in );
  } else if ( (l_rv64_i8dot != 0) && ( LIBXSMM_DATATYPE_I8 == LIBXSMM_GEMM_GETENUM_AB_COMMON_PREC( i_xgemm_desc->datatype ) ) ) {
    libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_0,
                                             ((long long)i_xgemm_desc->lda - i_m_blocking) * 4*i_micro_kernel_config->datatype_size_in );
  } else {
    libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_0,
                                             ((long long)i_xgemm_desc->lda - i_m_blocking) * i_micro_kernel_config->datatype_size_in );
  }

  /* load b offsets */
#if 0
  if ( io_generated_code->arch == LIBXSMM_RV64 ) {
    if ( i_n_blocking < 7 ) {
      for ( l_n = 1; l_n < i_n_blocking; l_n++ ) {
        /* handle trans B */
        if ( (i_xgemm_desc->flags & LIBXSMM_GEMM_FLAG_TRANS_B) > 0 ) {
          l_b_offset = l_n * i_micro_kernel_config->datatype_size_in;
        } else {
          l_b_offset = i_xgemm_desc->ldb * l_n * i_micro_kernel_config->datatype_size_in;
        }
        libxsmm_rv64_instruction_alu_set_imm64( io_generated_code, i_gp_reg_mapping->gp_reg_help_2 + (l_n - 1), l_b_offset );
      }
    }
  }
#endif
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_apply_relu_fusion_2dregblock_rv64(  libxsmm_generated_code*         io_generated_code,
                                                                const libxsmm_gemm_descriptor*  i_xgemm_desc,
                                                                libxsmm_micro_kernel_config*    io_micro_kernel_config,
                                                                const unsigned int              i_gp_reg_scratch0,
                                                                const unsigned int              i_gp_reg_scratch1,
                                                                const unsigned int              i_vec_length,
                                                                const unsigned int              i_vec_reg_count,
                                                                const unsigned int              i_m_blocking,
                                                                const unsigned int              i_n_blocking ) {
  /* register blocking counter in n */
  unsigned int l_n = 0;
  /* register blocking counter in m */
  unsigned int l_m = 0;
  unsigned int l_m_blocks[2] = { 0 }; /* 0: #full vector stores, 1: #predicate stores (0 or 1) */
  unsigned int l_m_total_blocks = 0;
  unsigned int l_vec_reg_acc_start = 0;
  unsigned int l_remainder_size = 0;

  l_m_blocks[0] = i_m_blocking / i_vec_length;
  l_remainder_size = i_m_blocking % i_vec_length;
  l_m_blocks[1] = (l_remainder_size > 0);
  l_m_total_blocks = l_m_blocks[0] + l_m_blocks[1];

  /* start register of accumulator */
  l_vec_reg_acc_start = i_vec_reg_count - (i_n_blocking * l_m_total_blocks);

  libxsmm_rv64_instruction_alu_compute( io_generated_code, LIBXSMM_RV64_INSTR_GP_FMV_W_X,
                                        LIBXSMM_RV64_GP_REG_X0, LIBXSMM_RV64_GP_REG_X0,
                                        LIBXSMM_RV64_GP_REG_F31);

  for ( l_n = 0; l_n < i_n_blocking; l_n++ ) {
    /* this is the jump size to be performed after a m-block is complete */
    for ( l_m = 0; l_m < l_m_total_blocks; l_m++ ) {
      unsigned int cur_vreg = l_vec_reg_acc_start + l_m_total_blocks * l_n + l_m;

      libxsmm_rv64_instruction_rvv_compute( io_generated_code, LIBXSMM_RV64_INSTR_GP_VFMAX_VF,
                                               LIBXSMM_RV64_GP_REG_F31, cur_vreg, cur_vreg, 1);
    }
  }
}

LIBXSMM_API_INTERN
void libxsmm_generator_gemm_apply_fusion_2dregblock_rv64(  libxsmm_generated_code*         io_generated_code,
                                                           const libxsmm_gemm_descriptor*  i_xgemm_desc,
                                                           libxsmm_micro_kernel_config*    io_micro_kernel_config,
                                                           const unsigned int              i_gp_reg_scratch0,
                                                           const unsigned int              i_gp_reg_scratch1,
                                                           const unsigned int              i_vec_length,
                                                           const unsigned int              i_vec_reg_count,
                                                           const unsigned int              i_m_blocking,
                                                           const unsigned int              i_n_blocking ) {
  if ((io_micro_kernel_config->fused_relu_nobitmask > 0) || (io_micro_kernel_config->fused_relu > 0)) {
    libxsmm_generator_gemm_apply_relu_fusion_2dregblock_rv64( io_generated_code, i_xgemm_desc, io_micro_kernel_config, i_gp_reg_scratch0, i_gp_reg_scratch1, i_vec_length, i_vec_reg_count, i_m_blocking, i_n_blocking );
  }
}