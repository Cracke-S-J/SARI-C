// Copyright (c) ssj. All rights reserved.

#include "stmt.h"

Break::Break() {
    if (Stmts.Enclosing == nullptr) {
        this->error("unenclosed bread");
    }
    this->stmt = Stmts.Enclosing;
}

void Seq::gen(int b, int a) {
    if (stmt1 == Stmts.Null) {
        if(stmt2 != nullptr) {
            stmt2->gen(b, a);
        }
        else {
            log_msg("set->gen()");
            set->gen(b, a);
        }
    }
    else if(stmt2 == Stmts.Null) {
        if(stmt1 != nullptr) {
            stmt1->gen(b, a);
        }
        else{
            set->gen(b, a);
        }
    }
    else {
        int label = this->newlable();
        this->stmt1->gen(b, label);
        this->emitlabel(label);
        this->stmt2->gen(label, a);
    }
}