#include "command.h"
#include <algorithm>
#include <set>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace db {

    namespace bs = boost::algorithm;

    cmd_ptr build_command(const std::string &stmt) {
        std::vector<std::string> tokens;
        bs::split(tokens, stmt, bs::is_space());
        auto command = tokens.empty() ? "" : tokens[0];

        if (command == "INSERT" && tokens.size() == 4) {
            auto table = tokens[1];
            int id = std::stoi(tokens[2]);
            auto name = tokens[3];
            return std::make_unique<insert_cmd>(table, row_t{id, name});
        } else if (command == "TRUNCATE" && tokens.size() == 2) {
            auto table = tokens[1];
            return std::make_unique<truncate_cmd>(table);
        } else if (command == "INTERSECTION") {
            return std::make_unique<and_cmd>();
        } else if (command == "SYMMETRIC_DIFFERENCE") {
            return std::make_unique<xor_cmd>();
        }
        return std::make_unique<unknown_cmd>();
    }

    std::string unknown_cmd::execute() {
        return "Unknown command or arguments passed\n";
    }

    std::string insert_cmd::execute() {
        std::string msg = "OK\n";
        auto script = [&](const table_t &t) {
            if (t.find(row_.id)) {
                msg = "ERR duplicate " + std::to_string(row_.id) + "\n";
                return t;
            } else {
                return t.insert({row_.id, row_});
            }
        };

        if (table_ == "A") {
            tables::mutate_table_A(script);
        } else if (table_ == "B") {
            tables::mutate_table_B(script);
        } else msg = "ERR unknown table\n";

        return msg;
    }

    std::string truncate_cmd::execute() {
        std::string msg = "OK\n";
        auto script = [](const table_t &) {
            return table_t();
        };

        if (table_ == "A") {
            tables::mutate_table_A(script);
        } else if (table_ == "B") {
            tables::mutate_table_B(script);
        } else msg = "ERR unknown table\n";

        return msg;
    }

    std::string and_cmd::execute() {
        auto[t1, t2] = tables::get_tables_A_B();
        std::set<decltype(row_t::id)> k1, k2, keys;
        auto key_extractor = [](const table_t::value_type &it) { return it.first; };

        std::transform(t1.begin(), t1.end(), std::inserter(k1, k1.begin()), key_extractor);
        std::transform(t2.begin(), t2.end(), std::inserter(k2, k2.begin()), key_extractor);
        std::set_intersection(k1.begin(), k1.end(), k2.begin(), k2.end(), std::inserter(keys, keys.begin()));

        std::stringstream msg;
        std::for_each(
                keys.begin(),
                keys.end(),
                [&, t1 = t1, t2 = t2](auto k) { msg << k << "," << t1[k].name << "," << t2[k].name << "\n"; }
        );

        return msg.str();
    }

    std::string xor_cmd::execute() {
        auto[t1, t2] = tables::get_tables_A_B();
        std::set<decltype(row_t::id)> k1, k2, keys;
        auto key_extractor = [](const table_t::value_type &it) { return it.first; };

        std::transform(t1.begin(), t1.end(), std::inserter(k1, k1.begin()), key_extractor);
        std::transform(t2.begin(), t2.end(), std::inserter(k2, k2.begin()), key_extractor);
        std::set_symmetric_difference(
                k1.begin(), k1.end(),
                k2.begin(), k2.end(),
                std::inserter(keys, keys.begin())
        );

        std::stringstream msg;
        std::for_each(
                keys.begin(),
                keys.end(),
                [&, t1 = t1, t2 = t2](auto k) {
                    msg << k << "," << (t1.find(k) ? t1[k].name : "") << "," << (t2.find(k) ? t2[k].name : "") << "\n";
                }
        );

        return msg.str();
    }
}