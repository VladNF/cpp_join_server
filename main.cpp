#include "db.h"
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

using namespace std::chrono_literals;
namespace bio = boost::asio;
namespace bs = boost::system;

struct Session : std::enable_shared_from_this<Session> {

    explicit Session(
            bio::ip::tcp::socket socket
    ) : socket_{std::move(socket)} {}

    virtual ~Session() = default;

    void read() {
        async_read_until(
                socket_,
                buf_,
                '\n',
                [&, self = shared_from_this()](bs::error_code ec, std::size_t) {
                    if (ec) return;
                    std::istream is(&buf_);
                    std::string line;
                    std::getline(is, line);
                    self->write(db::execute(line));
                }
        );
    }

    void write(std::string response) {
        async_write(
                socket_,
                bio::buffer(response),
                [self = shared_from_this()](boost::system::error_code ec, std::size_t) {
                    if (ec) return;
                    self->read();
                });
    }

private:
    bio::ip::tcp::socket socket_;
    bio::streambuf buf_;
};

void serve(bio::ip::tcp::acceptor &acceptor) {
    acceptor.async_accept(
            [&acceptor](bs::error_code ec, bio::ip::tcp::socket socket) {
                serve(acceptor);
                if (ec) return;
                auto session = std::make_shared<Session>(std::move(socket));
                session->read();
            }
    );
}

int main(int argc, char const *argv[]) {
    std::size_t port = 9000;
    try {
        if (argc > 1) port = std::stol(argv[1]);
        std::cout << "Listening on port " << port << " ...\n";
    } catch (const std::exception &e) {
        std::cerr << "Invalid argument passed: " << e.what();
        exit(1);
    }

    try {
        bio::io_context io_context;
        bio::ip::tcp::acceptor acceptor{
                io_context,
                bio::ip::tcp::endpoint(bio::ip::tcp::v4(), port),
                true
        };
        serve(acceptor);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}