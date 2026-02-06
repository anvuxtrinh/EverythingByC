#pragma once

#include "base_types.h"

Parser parser_init(LexState *ls);
BinaryExpr parse(Parser *parser);
