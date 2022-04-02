#ifndef HW10_JOIN_SERVER_COMMAND_H
#define HW10_JOIN_SERVER_COMMAND_H

#include "table.h"
#include <memory>
#include <string>

namespace db {

    class cmd_t {
    public:
        cmd_t() = default;

        virtual ~cmd_t() = default;

        virtual std::string execute() = 0;
    };

    using cmd_ptr = std::unique_ptr<cmd_t>;

    class unknown_cmd : public cmd_t {
    public:
        std::string execute() override;
    };

    class insert_cmd : public cmd_t {
        std::string table_;
        row_t row_;
    public:
        insert_cmd(std::string &t, row_t r): table_(t), row_(std::move(r)) {};
        std::string execute() override;
    };

    class truncate_cmd : public cmd_t {
        std::string table_;
    public:
        explicit truncate_cmd(std::string &t): table_(t) {};
        std::string execute() override;
    };

    class and_cmd : public cmd_t {
    public:
        std::string execute() override;
    };

    class xor_cmd : public cmd_t {
    public:
        std::string execute() override;
    };

    cmd_ptr build_command(const std::string &stmt);
}

#endif
