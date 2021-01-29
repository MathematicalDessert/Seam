#pragma once

#include <iostream>
#include "parse/Lexer.h"
#include "Module.h"

namespace seam {
    struct Debug {
        static void output_module_tokens(Module* mod) {
            auto lex_state = LexState(mod->get_source());
            Lexer lexer = Lexer(lex_state);
            while (lexer.next() != TokenType::tkEOF) {
                std::cout << lexer.token().to_string() << " ";
            }
            std::cout << std::endl;
        }
    };
}