#pragma once

#include <memory>
#include <vector>

#include "../ASTNode.h"

namespace seam::ast::statement {
    class Statement : ASTNode {  };

    using StatementList = std::vector<std::unique_ptr<Statement>>;
}