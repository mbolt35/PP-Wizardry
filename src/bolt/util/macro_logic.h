////////////////////////////////////////////////////////////////////////////////
//
//  MATTBOLT.BLOGSPOT.COM
//  Copyright(C) 2013 Matt Bolt
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BOLT_UTIL_MACRO_LOGIC_H
	#define BOLT_UTIL_MACRO_LOGIC_H

	#include "macro_va.h"
	#include "macro_eval.h"
	#include "macro_template.h"

	//--------------------------------------------------------------------------
    //
    //  Bit Logic
    //
    //--------------------------------------------------------------------------

	/**
	 * Returns the complement of the 0 or 1 input.
	 */
	#define NOT(C) MACRO_LOOKUP(NOT, C)()
	#define NOT0() 1
	#define NOT1() 0

	//--------------------------------------------------------------------------
    //
    //  Combinational Logic
    //
    //--------------------------------------------------------------------------
	
	/** 
	 * This macro definition returns the bitwise AND of A and B.
	 */
	#define AND(A, B) MACRO_LOOKUP(AND, A ## B)()
	#define AND00() 0
	#define AND01() 0
	#define AND10() 0
	#define AND11() 1

	/**
	 * This macro definition returns bitwise NAND of A and B.
	 */
	#define NAND(A, B) NOT(AND(A, B))

	/**
	 * This macro definition returns the bitwise OR of A and B.
	 */
	#define OR(A, B) MACRO_LOOKUP(OR, A ## B)() 
	#define OR00() 0
	#define OR01() 1
	#define OR10() 1
	#define OR11() 1

	/**
	 * This macro definition returns the bitwise NOR of A and B.
	 */
	#define NOR(A, B) NOT(OR(A, B))

	/**
	 * This macro definition returns the bitwise XOR of A and B.
	 */
	#define XOR(A, B) MACRO_LOOKUP(XOR, A ## B)()
	#define XOR00() 0
	#define XOR01() 1
	#define XOR10() 1
	#define XOR11() 0
	
	/**
	 * This macro definition returns the bitwise XNOR of A and B.
	 */
	#define XNOR(A, B) NOT(XOR(A, B))


	//--------------------------------------------------------------------------
    //
    //  Comparison Logic
    //
    //--------------------------------------------------------------------------

	#define EQ(A, B) DELAYED_APPLY(MACRO_LOOKUP(VA_AT, A), MACRO_LOOKUP(EQ, B))
	#define NEQ(A, B) NOT(EQ(A, B))

	#define GT(LHS, RHS) DELAYED_APPLY(MACRO_LOOKUP(VA_AT, LHS), MACRO_LOOKUP(GT, RHS))
	#define GTE(LHS, RHS) APPLY(OR, GT(LHS, RHS), EQ(LHS, RHS))

	#define LT(LHS, RHS) APPLY(NOT, GTE(LHS, RHS))
	#define LTE(LHS, RHS) APPLY(OR, LT(LHS, RHS), EQ(LHS, RHS))	

	//--------------------------------
    //  EQ Mapping for 10 Params
	//--------------------------------
	#define EQ0()  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	#define EQ1()  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
	#define EQ2()  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	#define EQ3()  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
	#define EQ4()  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	#define EQ5()  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
	#define EQ6()  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0
	#define EQ7()  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
	#define EQ8()  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0
	#define EQ9()  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
	#define EQ10() 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1

	//--------------------------------
    //  GT Mapping for 10 Params
	//--------------------------------
	#define GT0()  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	#define GT1()  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
	#define GT2()  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1
	#define GT3()  0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1
	#define GT4()  0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1
	#define GT5()  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1
	#define GT6()  0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1
	#define GT7()  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1
	#define GT8()  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1
	#define GT9()  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
	#define GT10() 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0


#endif