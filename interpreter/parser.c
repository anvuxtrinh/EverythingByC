#include "vector.h"
#include "parser.h"

BinExpr *tok2expr(vector *tok, u64 l, u64 r){
    if(l >= r){ return NULL; }

    
    BinExpr *bin_expr = (BinExpr *)malloc(sizeof(BinExpr));
    if(bin_expr == NULL){ return; }

    u64 mid = l + (r - l)/2;
    Token *current = vector_at(Token, tok, mid);
    switch (current->token)
    {
    case TK_PLUS:
        bin_expr->op.type = OPERATION;
        break;
    
    default:
        bin_expr->op.type = ATOM;
        break;
    }
    
    bin_expr->op.data = current->seminfo;
    bin_expr->lhs = tok2expr(tok, l, mid);
    bin_expr->rhs = tok2expr(tok, mid+1, r);

    return bin_expr;
}

BinExpr* parse(LexState* ls){
    BinExpr *ast = tok2expr(&ls->tok, 0, vector_size(&ls->tok));
    // current = vector_at(Token, &ls->tok, 0);
    // Expr lhs = {
    //     .type = ATOM,
    //     .data = current->seminfo
    // };

    // current = vector_at(Token, &ls->tok, 1);
    // Expr op = {
    //     .type = OPERATION,
    //     .data = current->seminfo
    // };

    // current = vector_at(Token, &ls->tok, 2);
    // Expr rhs = {
    //     .type = ATOM,
    //     .data = current->seminfo
    // };

    return ast;
}
