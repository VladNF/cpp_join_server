#include "db.h"
#include "command.h"

std::string db::execute(const std::string &stmt) {
    auto cmd = build_command(stmt);
    return cmd->execute();
}

