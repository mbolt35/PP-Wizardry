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

#ifndef BOLT_UTIL_MACRO_VA_H
    #define BOLT_UTIL_MACRO_VA_H    

    #if _MSC_VER && !__INTEL_COMPILER
        #define IS_MSVC
        #define MSVC_HACK(FUNC, ARGS) FUNC ARGS
    #endif

    #include "macro_logic.h"
    #include "macro_eval.h"
    #include "macro_template.h"

    //--------------------------------------------------------------------------
    //
    //  Macro Lookups
    //
    //--------------------------------------------------------------------------

    /**
     * This macro "looks up" another macro in the form <MACRO><INDEX>.
     *
     * For instance, there exists the following macro defintions: 
     *   #define LOG_ERR(FORMAT, ...)    fprintf(stderr, FORMAT, __VA_ARGS__)
     *   #define LOG_WARN(FORMAT, ...)   fprintf(stdout, FORMAT, __VA_ARGS__)
     *   
     * We could create a new macro, LOG, which could delegate between the 
     * two based on a new LEVEL input:
     *
     *   #define L_ERROR _ERR
     *   #define L_WARN _WARN
     *
     *   #define LOG(LEVEL, FORMAT, ...) MACRO_LOOKUP(LOG, LEVEL)(FORMAT, __VA_ARGS__)
     *
     * And we could then call it like so:
     *
     *   if (error) {
     *       LOG(L_ERROR, "There was an error: %s\n", error)
     *       return false;
     *   }
     */
    #define MACRO_LOOKUP(FUNC, INDEX) MACRO_CONCAT(FUNC, INDEX)

    /**
     * This macro definition just concatenates the two arguments together as a single entity. Most
     * often used to combine a macro name and index for lookups based on value.
     */
    #define MACRO_CONCAT(FUNC, INDEX) FUNC ## INDEX

    /**
     * This macro defintion concatenates the first argument with the variadic arguments following it.
     * It is deferred once to preserve the concatenation.
     */
    #define VA_CONCAT(ARG, ...) VA_CONCAT(ARG, __VA_ARGS__)
    #define VA_CONCAT_(ARG, ...) ARG ## __VA_ARGS__

    /** 
     * This macro definition returns the second argument in the list, which is a 0 unless the variadic
     * arguments contain elements
     */
    #if defined(IS_MSVC)
        #define VA_CHECK(...) APPLY(VA_CHECK_, __VA_ARGS__, 0)
    #else
        #define VA_CHECK(...) VA_CHECK_(__VA_ARGS__, 0,)
    #endif
    
    #define VA_CHECK_(ARG, N, ...) N

    //--------------------------------------------------------------------------
    //
    //  Variadic Helper Macros
    //
    //--------------------------------------------------------------------------
    
    /**
     * Defines a macro for determining the number of arguments in a variadic macro.
     * The VA_LENGTH_ macro is the complement to the counter, which aligns our arguments 
     * up to a specific count.
     */
    #if defined(IS_MSVC)
        #define VA_LENGTH(...) APPLY(VA_LENGTH_, 0, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
    #else
        #define VA_LENGTH(...) VA_LENGTH_(0, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
    #endif
    #define VA_LENGTH_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
    
    /**
     * This macro looks up another macro definition using the first argument and the length
     * variadic arguments.
     */
    #define GET_MACRO(FUNC, ...) MACRO_LOOKUP(FUNC, VA_LENGTH(__VA_ARGS__))

    /** 
     * This macro returns VA_LENGTH(__VA_ARGS__) + 1
     */
    #if defined(IS_MSVC)
        #define VA_LENGTH_INC(...) APPLY(VA_INC, __VA_ARGS__)
    #else
        #define VA_LENGTH_INC(...) VA_INC(__VA_ARGS__)
    #endif
    #define VA_INC(...) VA_LENGTH(0, __VA_ARGS__)

    /**
     * This macro returns VA_LENGTH(__VA_ARGS__) - 1
     */
    #if defined(IS_MSVC)
        #define VA_LENGTH_DEC(...) APPLY(VA_DEC, __VA_ARGS__)
    #else
        #define VA_LENGTH_DEC(...) VA_DEC(__VA_ARGS__)
    #endif
    #define VA_DEC(_0, ...) VA_LENGTH(__VA_ARGS__)

    /**
     * This macro returns the first argument in a list of arguments.
     */
    #if defined(IS_MSVC)
        #define VA_HEAD(...) APPLY(VA_AT, 0, __VA_ARGS__)
    #else
        #define VA_HEAD(...) VA_AT(0, __VA_ARGS__)
    #endif
    

    /**
     * This macro returns the last argument in a list of arguments.
     */
    #if defined(IS_MSVC)
        #define VA_TAIL(...) VA_TAIL_(VA_LENGTH_DEC(__VA_ARGS__), __VA_ARGS__)
        #define VA_TAIL_(VAL, ...) APPLY(VA_AT, VAL, __VA_ARGS__)    
    #else
        #define VA_TAIL(...) VA_AT(VA_LENGTH_DEC(__VA_ARGS__), __VA_ARGS__)
    #endif

    /**
     * This macro returns the single argument at an index position in the variadict 
     * arguments. It is upon the *CALLER* to pass arguments that meet the minimum
     * index requirements (LENGTH-1)
     */
    #if defined(IS_MSVC)
        #define VA_AT(INDEX, ...) APPLY(MACRO_CONCAT(VA_AT, INDEX), __VA_ARGS__)
    #else
        #define VA_AT(INDEX, ...) MACRO_CONCAT(VA_AT, INDEX)(__VA_ARGS__)
    #endif

    /**
     * This macro extracts the first argument through INDEX within an argument list.
     */
    #if defined(IS_MSVC)
        #define VA_SLICE_AT(INDEX, ...) APPLY(MACRO_CONCAT(VA_SLICE_AT, INDEX), __VA_ARGS__)
    #else
        #define VA_SLICE_AT(INDEX, ...) MACRO_CONCAT(VA_SLICE_AT, INDEX)(__VA_ARGS__)
    #endif
    

    //--------------------------------
    //  Indexed AT
    //--------------------------------
    #define VA_AT0(N, ...) N
    #define VA_AT1(X0, N, ...) N
    #define VA_AT2(X0, X1, N, ...) N
    #define VA_AT3(X0, X1, X2, N, ...) N
    #define VA_AT4(X0, X1, X2, X3, N, ...) N
    #define VA_AT5(X0, X1, X2, X3, X4, N, ...) N
    #define VA_AT6(X0, X1, X2, X3, X4, X5, N, ...) N
    #define VA_AT7(X0, X1, X2, X3, X4, X5, X6, N, ...) N
    #define VA_AT8(X0, X1, X2, X3, X4, X5, X6, X7, N, ...) N
    #define VA_AT9(X0, X1, X2, X3, X4, X5, X6, X7, X8, N, ...) N
    #define VA_AT10(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, N, ...) N

    //--------------------------------
    //  Indexed VA_SLICE_AT
    //--------------------------------
    #define VA_SLICE_AT0(X0, ...) 
    #define VA_SLICE_AT1(X0, X1, ...) X0
    #define VA_SLICE_AT2(X0, X1, X2, ...) X0, X1
    #define VA_SLICE_AT3(X0, X1, X2, X3, ...) X0, X1, X2
    #define VA_SLICE_AT4(X0, X1, X2, X3, X4, ...) X0, X1, X2, X3
    #define VA_SLICE_AT5(X0, X1, X2, X3, X4, X5, ...) X0, X1, X2, X3, X4
    #define VA_SLICE_AT6(X0, X1, X2, X3, X4, X5, X6, ...) X0, X1, X2, X3, X4, X5
    #define VA_SLICE_AT7(X0, X1, X2, X3, X4, X5, X6, X7, ...) X0, X1, X2, X3, X4, X5, X6
    #define VA_SLICE_AT8(X0, X1, X2, X3, X4, X5, X6, X7, X8, ...) X0, X1, X2, X3, X4, X5, X6, X7
    #define VA_SLICE_AT9(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, ...) X0, X1, X2, X3, X4, X5, X6, X7, X8
    #define VA_SLICE_AT10(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, ...) X0, X1, X2, X3, X4, X5, X6, X7, X8, X9

    //--------------------------------------------------------------------------
    //
    //  Preprocessor Loops
    //
    //--------------------------------------------------------------------------

    /**
     * This macro will execute the NEXT macro on the first N-1 arguments and the 
     * END macro on the final argument. It requires at least two initial arguments,
     * ARG1 and ARG2. 
     */
    #if defined(IS_MSVC)
        #define VA_ITERATE(NEXT, END, ARG1, ARG2, ...) APPLY(VA_ITERATE_, NEXT, END, ARG1, ARG2, __VA_ARGS__)
        #define VA_ITERATE_(NEXT, END, ...) \
            VA_EACH_EXEC(NEXT, VA_DEC(__VA_ARGS__), __VA_ARGS__) \
            END(VA_TAIL(__VA_ARGS__))
    #else
        #define VA_ITERATE(NEXT, END, ARG1, ARG2, ...) VA_ITERATE_(NEXT, END, ARG1, ARG2, ## __VA_ARGS__)
        #define VA_ITERATE_(NEXT, END, ...) \
            VA_EACH_EXEC(NEXT, VA_LENGTH_DEC(__VA_ARGS__), __VA_ARGS__) \
            END(VA_TAIL(__VA_ARGS__))
    #endif

    /**
     * This macro will execute the NEXT macro on all of the arguments. It requires
     * at least one argument ARG1.
     */
    #if defined(IS_MSVC)
        #define VA_EACH(NEXT, ARG1, ...) APPLY(VA_EACH_EXEC__, NEXT, ARG1, __VA_ARGS__)
    #else
        #define VA_EACH(NEXT, ARG1, ...) VA_EACH_(NEXT, ARG1, __VA_ARGS__)
        #define VA_EACH_(NEXT, ...) VA_EACH_EXEC(NEXT, VA_LENGTH(__VA_ARGS__), __VA_ARGS__)
    #endif


    #if defined(IS_MSVC)
        #define VA_EACH_EXEC(NEXT, NUM, ...) VA_EACH_EXEC_(NEXT, VA_SLICE_AT(NUM, __VA_ARGS__)) 
        #define VA_EACH_EXEC_(NEXT, ...) APPLY(VA_EACH_EXEC__, NEXT, __VA_ARGS__)
        #define VA_EACH_EXEC__(NEXT, ...) APPLY(_EACH, NEXT, __VA_ARGS__)
    #else
        #define VA_EACH_EXEC(NEXT, NUM, ...) VA_EACH_EXEC_(NEXT, VA_SLICE_AT(NUM, __VA_ARGS__))
        #define VA_EACH_EXEC_(NEXT, ...) _EACH(NEXT, __VA_ARGS__)
    #endif

#endif