// This file is part of MLDB. Copyright 2015 Datacratic. All rights reserved.

/* guard.h                                                         -*- C++ -*-
   Jeremy Barnes, 13 February 2007
   Copyright (c) 2007 Jeremy Barnes.  All rights reserved.



   ---
*/

#ifndef __utils__guard_h__
#define __utils__guard_h__

#include "mldb/compiler/compiler.h"
#include <functional>

namespace ML {

struct Call_Guard {
    
    typedef std::function<void ()> Fn;

    Call_Guard(const Fn & fn, bool condition = true)
        : fn(condition ? fn : Fn())
    {
    }

    Call_Guard()
    {
    }
    
#if JML_HAS_RVALUE_REFERENCES
    Call_Guard(Call_Guard && other)
        : fn(other.fn)
    {
        other.clear();
    }

    Call_Guard & operator = (Call_Guard && other)
    {
        if (fn) fn();
        fn = other.fn;
        other.clear();
        return *this;
    }
#endif

    ~Call_Guard()
    {
        if (fn) fn();
    }

    void clear() { fn = Fn(); }

    void set(const Fn & fn) { this->fn = fn; }

    std::function<void ()> fn;

private:
    Call_Guard(const Call_Guard & other);
    void operator = (const Call_Guard & other);
};

#if JML_HAS_RVALUE_REFERENCES
template<typename Fn>
Call_Guard call_guard(Fn fn, bool condition = true)
{
    return Call_Guard(fn, condition);
}
#endif


} // namespace ML


#endif /* __utils__guard_h__ */
