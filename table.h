#ifndef HW10_JOIN_SERVER_TABLE_H
#define HW10_JOIN_SERVER_TABLE_H
#include <mutex>
#include <shared_mutex>
#include "immer/map.hpp"

namespace db {

    struct row_t {
        int id;
        std::string name;
    };

    using table_t = immer::map<decltype(row_t::id), row_t>;

    class tables {
        std::shared_mutex mutex_A;
        std::shared_mutex mutex_B;
        table_t A_, B_;

        tables() = default;

        static tables &get_instance() {
            static tables t;
            return t;
        }

    public:
        static table_t get_table_A();

        static table_t get_table_B();

        static std::pair<table_t, table_t> get_tables_A_B();

        template<class Fn>
        static void mutate_table_A(Fn f) {
            auto &t = get_instance();
            std::unique_lock lock(t.mutex_A);
            t.A_ = f(t.A_);
        }

        template<class Fn>
        static void mutate_table_B(Fn f) {
            auto &t = get_instance();
            std::unique_lock lock(t.mutex_B);
            t.B_ = f(t.B_);
        }
    };

}

#endif
