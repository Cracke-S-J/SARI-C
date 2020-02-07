// Copyright (c) ssj. All rights reserved.

#include "symbol.h"
#include "logic.h"

Type* Rel::check(Type* p1, Type* p2) {
    if (p1 == p2) {
        return Types.Bool;
    }
    else {
        return nullptr;
    }
}
