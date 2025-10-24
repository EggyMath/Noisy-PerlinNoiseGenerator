#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> server;

class WebSocketServer {
public:
    WebSocketServer() {
        m_server.set_access_channels(websocketpp::log::alevel::all);
        m_server.clear_access_channels(websocketpp::log::elevel::all);

        m_server.init_asio();

        m_server.set_reuse_addr(true);

        using websocketpp::lib::bind;
        using websocketpp::lib::placeholders::_1;
        using websocketpp::lib::placeholders::_2;

        m_server.set_open_handler(bind(&WebSocketServer::on_open, this, _1));
        m_server.set_close_handler(bind(&WebSocketServer::on_close, this, _1));
        m_server.set_message_handler(bind(&WebSocketServer::on_message, this, _1, _2));
    }

    void run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        std::cout << "WebSocket++ server running on port " << port << std::endl;
        m_server.run();
    }

private:
    typedef websocketpp::connection_hdl connection_hdl;
    server m_server;
    std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections;

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
        std::cout << "Client connected!" << std::endl;
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
        std::cout << "Client disconnected." << std::endl;
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        std::string payload = msg->get_payload();
        std::cout << "Message received: " << payload << std::endl;

        std::string response = "Recieved message: " + payload;
        m_server.send(hdl, response, msg->get_opcode());
    }
};

int main() {
    WebSocketServer server;
    server.run(9002);
}
