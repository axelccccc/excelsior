/**
 * @file arch/i386/mem.h
 * @author axelccccc (github.com/axelccccc)
 * @brief i386-specific memory layout, management & definitions
 * @version 0.1
 * @date 2023-01-14
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _KERNEL_MEM_I386_H
#define _KERNEL_MEM_I386_H 1

// Start of extended memory (> 1mb)
#define MEM_EXT_START       0x100000

// Start of kernel memory (1mb)
#define MEM_KERNEL_START    0x100000

#endif