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

#ifndef BOLT_UTIL_MACRO_TEMPLATE_H
    #define BOLT_UTIL_MACRO_TEMPLATE_H

    #include "macro_va.h"
    #include "macro_logic.h"
    #include "macro_eval.h"

    //--------------------------------------------------------------------------
    //
    //  Nested Namespace Generation
    //
    //--------------------------------------------------------------------------
    
    /** 
     * This macro definition creates nested namespaces that represent the arguments
     * passed. Each NS_START(...) should be ended with NS_END(...). For example:
     *   NS_START(bolt, util, macros) 
     *   {
     *     int a = 15;
     *   };
     *   NS_END(bolt, util, macros);
     * 
     * Will create the int a variable in the namespace bolt::util::macros, so bolt::util::macros::a
     * will resolve to 15.
     */
    #define NS_START(...) NS_START_(NS_T_, NS_T, __VA_ARGS__)

    #if defined(IS_MSVC)
        #define NS_START_(NEXT, END, ...) APPLY(NS_CHECK(__VA_ARGS__), NEXT, END, __VA_ARGS__)
        #define NS_CHECK(...) NS_CHECK_(GTE(VA_LENGTH(__VA_ARGS__), 2))               
        #define NS_CHECK_(INDEX) APPLY(IIF(INDEX), NS_1, NS_N)
        #define NS_1(NEXT, END, ...) APPLY(END, __VA_ARGS__)
        #define NS_N(NEXT, END, ...) APPLY(VA_ITERATE, NEXT, END, __VA_ARGS__)
    #else
        #define NS_START_(NEXT, END, ...) NS_CHECK(__VA_ARGS__)(NEXT, END, __VA_ARGS__)
        #define NS_CHECK(...) IIF(GTE(VA_LENGTH(__VA_ARGS__), 2))(NS_1, NS_N)
        #define NS_1(NEXT, END, ...) END(__VA_ARGS__)
        #define NS_N(NEXT, END, ...) VA_ITERATE(NEXT, END, __VA_ARGS__)
    #endif

    /**
     * This macro definition creates the closing braces for the nested namespaces. It must be called
     * after NS_START, and with the same arguments of NS_START. 
     */
    #if defined(IS_MSVC)
        #define NS_END(...) APPLY(MACRO_LOOKUP, NS_END, VA_DEC(__VA_ARGS__))
    #else
        #define NS_END(...) MACRO_LOOKUP(NS_END, VA_LENGTH_DEC(__VA_ARGS__))    
    #endif
    

    #define NS_T(T) namespace T
    #define NS_T_(T) NS_T(T) {

    //--------------------------------
    //  Namespace closing braces
    //--------------------------------
    #define NS_END0
    #define NS_END1 };
    #define NS_END2 }; };
    #define NS_END3 }; }; };
    #define NS_END4 }; }; }; };
    #define NS_END5 }; }; }; }; };
    #define NS_END6 }; }; }; }; }; };
    #define NS_END7 }; }; }; }; }; }; }; 
    #define NS_END8 }; }; }; }; }; }; }; };
    #define NS_END9 }; }; }; }; }; }; }; }; };

    //--------------------------------------------------------------------------
    //
    //  Template Generation
    //
    //--------------------------------------------------------------------------
    
    /**
     * This macro definition will output a template<> containing the parameters passed.
     * For example, a templated method can be defined:
     *
     *   generic(T, E) static T Map(E val) {
     *       T result = // conversion from E to T
     *       return T;
     *   }
     *
     * It is possible to pass default parameters:
     *
     *   generic(T=int, E=char) static T Map(E val);
     *
     * However, it is _NOT_ currently possible to pass specific typed template params:
     *
     *   generic(T, int E=10) static T* CreateArray() {
     *     return new T[E];
     *   }
     *
     * The above will fail. 
     */
    #define generic(...) template<TEMPLATE(CLASS_T_, CLASS_T, __VA_ARGS__)>
    #define typed_generic(...) template<__VA_ARGS__>

    /**
     * This macro generates a templated class using the symbols passed. For example:
     *
     *   class(T, U, V) Tuple {
     *       private: 
     *           T tValue;
     *           U uValue;
     *           V vValue;
     *
     *       public:
     *           Tuple(T tValue, U uValue, V vValue) 
     *               : tValue(tValue), uValue(uValue), vValue(vValue) { }
     *
     *           T GetValueOne() const { return tValue; }
     *           U GetValueTwo() const { return uValue; }
     *           V GetValueThree() const { return vValue; }
     *   };
     */
    #define class(...) generic(__VA_ARGS__) class
    
    /**
     * This macro performs the name templated definition as the class(...) define, but 
     * with a struct instead of class.
     */
    #define struct(...) generic(__VA_ARGS__) struct

    /**
     * Determines whether or not the variadic arguments have enough elements to be processed. 
     * If the length is 1, then only the END() macro is executed on the parameter. If the length
     * is two or more, the foreach is executed.
     */
    #if defined(IS_MSVC)
        #define TEMPLATE(NEXT, END, ...) APPLY(TEMPLATE_CHECK(__VA_ARGS__), NEXT, END, __VA_ARGS__)
        #define TEMPLATE_CHECK(...) TEMPLATE_CHECK_(GTE(VA_LENGTH(__VA_ARGS__), 2))
        #define TEMPLATE_CHECK_(INDEX) APPLY(IIF(INDEX), TEMPLATE_1, TEMPLATE_N)
        #define TEMPLATE_1(NEXT, END, ...) APPLY(END, __VA_ARGS__)
        #define TEMPLATE_N(NEXT, END, ...) APPLY(VA_ITERATE, NEXT, END, __VA_ARGS__)
    #else
        #define TEMPLATE(NEXT, END, ...) TEMPLATE_CHECK(__VA_ARGS__)(NEXT, END, ## __VA_ARGS__)
        #define TEMPLATE_CHECK(...) IIF(GTE(VA_LENGTH(__VA_ARGS__), 2))(TEMPLATE_1, TEMPLATE_N)
        #define TEMPLATE_1(NEXT, END, ...) END(__VA_ARGS__)
        #define TEMPLATE_N(NEXT, END, ...) VA_ITERATE(NEXT, END, __VA_ARGS__)
    #endif
    
    /**
     * Returns class T. Used in our for each macro as the END() defintion.
     */
    #define CLASS_T(T) class T

    /**
     * Returns class T with a comma. Used in our for each macro as the NEXT() definition.
     */
    #define CLASS_T_(T) CLASS_T(T),

#endif