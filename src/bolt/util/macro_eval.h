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

#ifndef BOLT_UTIL_MACRO_EVAL_H
	#define BOLT_UTIL_MACRO_EVAL_H

	#include "macro_va.h"
	#include "macro_logic.h"
    #include "macro_template.h"

	//--------------------------------------------------------------------------
    //
    //  Macro Deference and Evaluation
    //
    //--------------------------------------------------------------------------
	
	/** 
	 * This macro expands each call 3 times to avoid the disabling context:
	 * "However, a disabling context only exists during one scan, so by deferring 
	 * an expansion we can prevent our macros from becoming painted blue. We will 
	 * just need to apply more scans to the expression. We can do that using this 
	 * EVAL macro"
	 * 
	 * Thanks to Paul, (pfultz2 on github) for this method: 
	 * http://stackoverflow.com/users/375343/paul
	 */
	#define B_EVAL(...)  B_EVAL0(B_EVAL0(B_EVAL0(__VA_ARGS__)))
	#define B_EVAL0(...) B_EVAL1(B_EVAL1(B_EVAL1(__VA_ARGS__)))
	#define B_EVAL1(...) B_EVAL2(B_EVAL2(B_EVAL2(__VA_ARGS__)))
	#define B_EVAL2(...) B_EVAL3(B_EVAL3(B_EVAL3(__VA_ARGS__)))
	#define B_EVAL3(...) B_EVAL4(B_EVAL4(B_EVAL4(__VA_ARGS__)))
	#define B_EVAL4(...) __VA_ARGS__

	
	//--------------------------------------------------------------------------
    //
    //  Macro Conditional Evaluation
    //
    //--------------------------------------------------------------------------

	#define IF_THEN(COND, THEN, ...) IF_THEN_ELSE_(COND, THEN, EMPTY)(__VA_ARGS__)
	#define IF_THEN_ELSE(COND, THEN, ELSE, ...) IF_THEN_ELSE_(COND, THEN, ELSE)(__VA_ARGS__)
	#define IF_THEN_ELSE_(COND, THEN, ELSE) IIF(COND)(ELSE, THEN)

	/**
	 * This method concatenates the condition with the IIF statement and returns either
	 * a macro that returns the first element (on false) or the rest of the elements (on true).
	 * 
	 * Thanks again to Paul, (pfultz2 on github) for this method: 
	 * http://stackoverflow.com/users/375343/paul
	 */
	#define IIF(COND) VA_CONCAT_(IIF_, COND)
	#define IIF_0(ARG, ...) ARG
	#define IIF_1(ARG, ...) __VA_ARGS__

	/**
	 * This macro definition represents an empty token argument.
	 */
	#define EMPTY_TOKEN

	/**
	 * This macro definition returns nothing given a variadic set of arguments.
	 */
	#define EMPTY(...)

	/**
	 * Expands the arguments to create another indirection
	 */
	#define EXPAND(...) __VA_ARGS__

	/**
	 * This macro definition attempts to execute a macro containing one argument.
	 */
	#define TOKEN(I) I(~)

	/**
	 * This macro definition returns the first argument with a 1 constant.
	 */
	#define SENTINEL(X) X, 1

	//--------------------------------------------------------------------------
    //
    //  Delayed Evaluation
    //
    //--------------------------------------------------------------------------

	#define DELAYED_APPLY(FUNC, PROP) APPLY(FUNC, PROP())
	#if defined(IS_MSVC)
		#define APPLY(FUNC, ...) MSVC_HACK(FUNC, (__VA_ARGS__))
	#else
		#define APPLY(FUNC, ...) FUNC(__VA_ARGS__)
	#endif

	//--------------------------------------------------------------------------
    //
    //  Macro Loop Evaluation
    //
    //--------------------------------------------------------------------------

	// Does our T token match our sentinel?
	#define IS_NEXT_END(T) IIF(VA_CHECK(TOKEN(T)))(0, 0, EMPTY)

	/**
	 * Loops through each of the variadict arguments and executes the FUNC parameter
	 * passing the argument. We use two "ping-ponging" macros, _EACH1 and _EACH0 to
	 * simulate recursive calls. NEXT is used to extract the next argument unless it
	 * finds the terminating value, which is in this case: SENTINEL. 
	 *
	 * On each of the _EACH0 and _EACH1 executions, the X represents the current node
	 * while the P represents the item that is next. P is checked by _NEXT and will return
	 * the next _EACH method if P does not match our sentinel. 
	 */
	#if defined(IS_MSVC)
		#define _EACH(FUNC, ...) B_EVAL(_EACH0(FUNC, __VA_ARGS__, SENTINEL, 0))
		#define _EACH0(F, X, P, ...) F(X) _EACH_(_NEXT(P, _EACH1), F, P, __VA_ARGS__)
		#define _EACH1(F, X, P, ...) F(X) _EACH_(_NEXT(P, _EACH0), F, P, __VA_ARGS__)
		#define _EACH_(NEXT, F, P, ...) APPLY(NEXT, F, P, __VA_ARGS__)

		#define _NEXT(I, N) APPLY(_NEXT0, _NEXT_(I, N))
		#define _NEXT_(I, N) IS_NEXT_END(I), N
		#define _NEXT0(I, N, ...) APPLY(_NEXT1, I, N, 0)
		#define _NEXT1(I, N, ...) N EMPTY_TOKEN
	#else
		#define _EACH(FUNC, ...) B_EVAL(_EACH0(FUNC, __VA_ARGS__, SENTINEL, 0))
		#define _EACH0(F, X, P, ...) F(X) _NEXT(P, _EACH1)(F, P, __VA_ARGS__)
		#define _EACH1(F, X, P, ...) F(X) _NEXT(P, _EACH0)(F, P, __VA_ARGS__)

		#define _NEXT(I, N) _NEXT0(IS_NEXT_END(I), N)
		#define _NEXT0(I, N) _NEXT1(I, N, 0) 
		#define _NEXT1(I, N, ...) N EMPTY_TOKEN
		
	#endif
	
#endif