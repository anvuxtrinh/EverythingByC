#include "vector.h"
#include "parser.h"

BinaryExpr parse(LexState *ls){
    Token *current;
    
    current = vector_at(Token, &ls->tok, 0);
    Expr lhs = {
        .type = ATOM,
        .data = current->seminfo
    };

    current = vector_at(Token, &ls->tok, 1);
    Expr op = {
        .type = OPERATION,
        .data = current->seminfo
    };

    current = vector_at(Token, &ls->tok, 2);
    Expr rhs = {
        .type = ATOM,
        .data = current->seminfo
    };

    return (BinaryExpr) {
        .rhs = rhs,
        .lhs = lhs,
        .op = op
    };
}
