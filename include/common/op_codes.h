// SPDX-License-Identifier: MIT
/**
 * @file op_codes.h
 * @author lazypaws
 * @brief Defines all bytecode operation codes for MeowScript
 * @copyright Copyright(c) 2025 LazyPaws
 */

#pragma once

namespace meow::common {
    /**
     * @enum OpCode
     * @brief Represent all supported bytecode operation codes
     */
    enum class OpCode : unsigned char {
        LOAD_CONST, LOAD_NULL, LOAD_TRUE, LOAD_FALSE, LOAD_INT, MOVE,
        ADD, SUB, MUL, DIV, MOD, POW, EQ, NEQ, GT, GE, LT, LE,
        NEG, NOT,
        GET_GLOBAL, SET_GLOBAL, GET_UPVALUE, SET_UPVALUE, CLOSURE, CLOSE_UPVALUES,
        JUMP, JUMP_IF_FALSE, JUMP_IF_TRUE, CALL, RETURN, HALT,
        NEW_ARRAY, NEW_HASH, GET_INDEX, SET_INDEX, GET_KEYS, GET_VALUES,
        NEW_CLASS, GET_PROP, SET_PROP,
        SET_METHOD, INHERIT, GET_SUPER,
        BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, LSHIFT, RSHIFT,
        THROW, SETUP_TRY, POP_TRY,
        IMPORT_MODULE, EXPORT, GET_EXPORT, IMPORT_ALL,
        TOTAL_OPCODES
    };
};