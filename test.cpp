#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>

#include "NoiseAlgorithms/genericNoise.h"
#include "NoiseAlgorithms/whiteNoise.h"
#include "NoiseAlgorithms/perlinNoise.h"

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

        if (payload == "whiteNoise") {
            // White noise
            whiteNoise wn;
            wn.create(128, 128, 0);

            genericNoise modNoise(wn, 0.5);
            std::string whiteJson = "{ \"type\": \"white\", \"payload\": " + modNoise.noiseToJson() + " }";

            m_server.send(hdl, whiteJson, websocketpp::frame::opcode::text);

            // perlin texture
            perlinNoise pn;
            pn.create(128, 128, 40.0f, 1337, PerlinMode::FBM, 6, 2.0f, 0.5f, 10.0f);

            std::string perlinJson = "{ \"type\": \"perlin\", \"payload\": " + pn.noiseToJson() + " }";

            m_server.send(hdl, perlinJson, websocketpp::frame::opcode::text);

            return;
        }

        std::string response = "{ \"type\": \"text\", \"payload\": \"Received: " + payload + "\" }";
        m_server.send(hdl, response, websocketpp::frame::opcode::text);
    }
};

int main() {
    WebSocketServer server;
    server.run(9002);
}
