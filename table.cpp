#include "table.h"

namespace db {
    table_t tables::get_table_A() {
        auto &t = get_instance();
        std::shared_lock lock(t.mutex_A);
        return t.A_;
    }

    table_t tables::get_table_B() {
        auto &t = get_instance();
        std::shared_lock lock(t.mutex_B);
        return t.B_;
    }

    std::pair<table_t, table_t> tables::get_tables_A_B() {
        auto &t = get_instance();
        std::shared_lock lock_a(t.mutex_A), lock_b(t.mutex_B);
        return {t.A_, t.B_};
    }
}
