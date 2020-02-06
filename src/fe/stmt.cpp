// Copyright (c) ssj. All rights reserved.

#include "stmt.h"

Break::Break() {
    if (Stmts.Enclosing == nullptr) {
        this->error("unenclosed bread");
    }
    this->stmt = Stmts.Enclosing;
}

void Seq::gen(int b, int a) {
    log_msg("Seq->gen()" + std::to_string(b)+std::to_string(a));
    if (nxt == Stmts.Null && 
        nxt->getClazz() == Inter::STMT) {
        if(stmt->getClazz() == Inter::SET) {
            Set* set = (Set*)stmt;
            set->gen(b, a);
        }
        else if(stmt->getClazz() == Inter::SEQ) {
            Seq* seq = (Seq*)stmt;
            seq->gen(b, a);
        }
        else if(stmt->getClazz() == Inter::IF) {
            If* _if = (If*)stmt;
            _if->gen(b, a);
        }
        else {
            stmt->gen(b, a);
        }
    }
    else if (stmt == Stmts.Null && 
             stmt->getClazz() == Inter::STMT) {
        if(nxt->getClazz() == Inter::SET) {
            Set* set = (Set*)nxt;
            set->gen(b, a);
        }
        else if(nxt->getClazz() == Inter::SEQ) {
            Seq* seq = (Seq*)nxt;
            seq->gen(b, a);
        }
        else if(nxt->getClazz() == Inter::IF) {
            If* _if = (If*)nxt;
            _if->gen(b, a);
        }
        else {
            nxt->gen(b, a);
        }
    }
    else {
        int label = this->newlable();
        if(stmt->getClazz() == Inter::SET) {
            Set* set = (Set*)stmt;
            set->gen(b, label);
        }
        else if(stmt->getClazz() == Inter::IF) {
            If* _if = (If*)stmt;
            _if->gen(b, label);
        }
        else {
            this->stmt->gen(b, label);
        }
        this->emitlabel(label);
        if(nxt->getClazz() == Inter::SEQ) {
            Seq* seq = (Seq*)nxt;
            seq->gen(label, a);
        }
        else {
            this->nxt->gen(label, a);
        }
    }
}

void If::gen(int b, int a) {
    log_msg("if->gen()");
    int label  = this->newlable();
    int expr_t = this->expr->getClazz();
    if(expr_t == Inter::ID) {
        Id* id = (Id*)expr;
        log_msg("id->jumping");
        id->jumping(0, a);
    }
    else {
        this->expr->jumping(0, a);
    }
    this->emitlabel(label);
    if(stmt->getClazz() == Inter::SET) {
        Set* set = (Set*)stmt;
        set->gen(label, a);
    }
    else {
        this->stmt->gen(label, a);
    }
    return;
}
