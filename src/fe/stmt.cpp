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
        else if(stmt->getClazz() == Inter::ELSE) {
            Else* _else = (Else*)stmt;
            _else->gen(b, a);
        }
        else if(stmt->getClazz() == Inter::WHIL) {
            While* _while = (While*)stmt;
            _while->gen(b, a);
        }
        else if(stmt->getClazz() == Inter::DO) {
            Do* _do = (Do*)stmt;
            _do->gen(b, a);
        }
        else if(stmt->getClazz() == Inter::BREA) {
            Break* _break = (Break*)stmt;
            _break->gen(b, a);
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
        else if(nxt->getClazz() == Inter::ELSE) {
            Else* _else = (Else*)stmt;
            _else->gen(b, a);
        }
        else if(nxt->getClazz() == Inter::WHIL) {
            While* _while = (While*)nxt;
            _while->gen(b, a);
        }
        else if(nxt->getClazz() == Inter::DO) {
            Do* _do = (Do*)stmt;
            _do->gen(b, a);
        }
        else if(nxt->getClazz() == Inter::BREA) {
            Break* _break = (Break*)stmt;
            _break->gen(b, a);
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
        else if(stmt->getClazz() == Inter::ELSE) {
            Else* _else = (Else*)stmt;
            _else->gen(b, label);
        }
        else if(stmt->getClazz() == Inter::WHIL) {
            While* _while = (While*)stmt;
            _while->gen(b, label);
        }
        else if(stmt->getClazz() == Inter::DO) {
            Do* _do = (Do*)stmt;
            _do->gen(b, label);
        }
        else if(stmt->getClazz() == Inter::BREA) {
            Break* _break = (Break*)stmt;
            _break->gen(b, label);
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
    else if(expr_t == Inter::CONS) {
        Constant* cons = (Constant*)expr;
        log_msg("constant->jumping");
        cons->jumping(0, a);
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

void Else::gen(int b, int a) {
    int label1 = this->newlable();
    int label2 = this->newlable();
    int expr_t = this->expr->getClazz();
    if(expr_t == Inter::ID) {
        Id* id = (Id*)expr;
        log_msg("id->jumping");
        id->jumping(0, label2);
    }
    else if(expr_t == Inter::CONS) {
        Constant* cons = (Constant*)expr;
        log_msg("constant->jumping");
        cons->jumping(0, label2);
    }
    else {
        this->expr->jumping(0, label2);
    }
    this->emitlabel(label1);
    if(stmt1->getClazz() == Inter::SET) {
        Set* set = (Set*)stmt1;
        set->gen(label1, a);
    }
    else {
        this->stmt1->gen(label1, a);
    }
    this->emit("goto L" + std::to_string(a));
    this->emitlabel(label2);
    if(stmt2->getClazz() == Inter::SET) {
        Set* set = (Set*)stmt2;
        set->gen(label2, a);
    }
    else {
        this->stmt2->gen(label2, a);
    }
}

void While::gen(int b, int a) {
    this->setAfter(a);
    int expr_t = this->expr->getClazz();
    if(expr_t == Inter::ID) {
        Id* id = (Id*)expr;
        log_msg("id->jumping");
        id->jumping(0, a);
    }
    else if(expr_t == Inter::CONS) {
        Constant* cons = (Constant*)expr;
        log_msg("constant->jumping");
        cons->jumping(0, a);
    }
    else {
        this->expr->jumping(0, a);
    }
    int label = this->newlable();
    this->emitlabel(label);
    if(stmt->getClazz() == Inter::SET) {
        Set* set = (Set*)stmt;
        set->gen(label, a);
    }
    else {
        this->stmt->gen(label, a);
    }
    this->emit("goto L" + std::to_string(b));
}

void Do::gen(int b, int a) {
    this->setAfter(a);
    int label = this->newlable();
    if(stmt->getClazz() == Inter::SET) {
        Set* set = (Set*)stmt;
        set->gen(b, label);
    }
    else {
        this->stmt->gen(b, label);
    }
    this->emitlabel(label);
    int expr_t = this->getClazz();
    if(expr_t == Inter::ID) {
        Id* id = (Id*)expr;
        log_msg("id->jumping");
        id->jumping(b, 0);
    }
    else if(expr_t == Inter::CONS) {
        Constant* cons = (Constant*)expr;
        log_msg("constant->jumping");
        cons->jumping(b, 0);
    }
    else {
        this->expr->jumping(b, 0);
    }
}
