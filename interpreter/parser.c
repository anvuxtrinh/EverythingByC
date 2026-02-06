#include "parser.h"

I32 tok_next(Parser *parser);

Parser parser_init(LexState *ls){
    return (Parser) {
        .tok = ls->tok,
        .tok_size = ls->tok_size,
        .tok_capacity = ls->tok_capacity,
        .tok_idx = 0
    };
}

BinaryExpr parse(Parser *parser){
    Expr lhs = {
        .type = ATOM,
        .data = parser->tok->seminfo
    };

    tok_next(parser);
    Expr op = {
        .type = OPERATION,
        .data = parser->tok->seminfo
    };

    tok_next(parser);
    Expr rhs = {
        .type = ATOM,
        .data = parser->tok->seminfo
    };

    return (BinaryExpr) {
        .rhs = rhs,
        .lhs = lhs,
        .op = op
    };
}

I32 tok_next(Parser *parser){
    if(parser->tok_idx == parser->tok_size) { return 1; }
    ++parser->tok_idx;
    ++parser->tok;
    return 0;
}
