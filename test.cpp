#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>

#include "NoiseAlgorithms/genericNoise.h"
#include "NoiseAlgorithms/whiteNoise.h"
#include "NoiseAlgorithms/perlinNoise.h"
#include "NoiseAlgorithms/simplexNoise.h"

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
        
        std::string command = getValue(payload, "CMD");
        std::cout<<"Commnad: "<<command<<std::endl;

        if (command == "whiteNoise") {
            std::cout<<"White Noise Requested"<<std::endl;
            whiteNoise wn;
            wn.create(128, 128, 0);

            std::string whiteJson = "{ \"type\": \"white\", \"payload\": " + wn.noiseToJson() + " }";

            m_server.send(hdl, whiteJson, websocketpp::frame::opcode::text);
        } else if (command == "perlinNoise") {
            std::cout<<"Perlin Noise Requested"<<std::endl;
            perlinNoise pn;
            pn.create(
                128, 128,
                12.0f,
                1337,
                PerlinMode::FBM,
                6,
                2.2f,
                0.5f,
                0.0f
            );

            std::string perlinJson = "{ \"type\": \"perlin\", \"payload\": " + pn.noiseToJson() + " }";
            m_server.send(hdl, perlinJson, websocketpp::frame::opcode::text);
        } else if (command == "simplexNoise") {
            std::cout<<"Simplex Noise Requested"<<std::endl;
            simplexNoise simplex;
            simplex.create(256, 
                         256, 
                         40.0f, 
                         1337, 
                         6, 
                         2.0f, 
                         0.5f
            );

            std::string simplexJson = "{ \"type\": \"simplex\", \"payload\": " + simplex.noiseToJson() + " }";
            m_server.send(hdl, simplexJson, websocketpp::frame::opcode::text);
        } else if (command == "terrainNoise") {
            perlinNoise pn;
            pn.create(
                128, 128,
                12.0f,
                1337,
                PerlinMode::FBM,
                6,
                2.2f,
                0.5f,
                0.0f
            );

            std::string perlinJson = "{ \"type\": \"terrain\", \"payload\": " + pn.noiseToJson() + " }";
            m_server.send(hdl, perlinJson, websocketpp::frame::opcode::text);
        } else if (command == "cloudsNoise") {
            perlinNoise clouds;
            clouds.create(
                256, 256,
                40.0f,
                2024,
                PerlinMode::BILLOW,
                6,
                2.0f,
                0.5f,
                0.0f
            );

            std::string cloudJson = "{ \"type\": \"clouds\", \"payload\": " + clouds.noiseToJson() + " }";
            m_server.send(hdl, cloudJson, websocketpp::frame::opcode::text);
        } else if (command == "waterNoise") {
            simplexNoise water;
            water.create(256, 
                         256, 
                         40.0f, 
                         1337, 
                         6, 
                         2.0f, 
                         0.5f
            );

            std::string waterJson = "{ \"type\": \"water\", \"payload\": " + water.noiseToJson() + " }";
            m_server.send(hdl, waterJson, websocketpp::frame::opcode::text);
        } else if (command == "gasNoise") {
            perlinNoise gas;
            gas.create(
                512, 512,
                150.0f,
                9999,
                PerlinMode::DOMAIN_WARP,
                6,
                2.5f,
                0.45f,
                4.0f
            );

            std::string gasJson = "{ \"type\": \"gas\", \"payload\": " + gas.noiseToJson() + " }";
            m_server.send(hdl, gasJson, websocketpp::frame::opcode::text);
        } else {
            std::string response = "{ \"type\": \"text\", \"payload\": \"Received: " + payload + "\" }";
            m_server.send(hdl, response, websocketpp::frame::opcode::text);
        }
    }

    std::string getValue(const std::string& msg, const std::string& key) {
        size_t start = msg.find(key + ":");
        if (start == std::string::npos) 
            return "";
        start += key.size() + 1;

        size_t end = msg.find(";", start);
        if (end == std::string::npos)
            end = msg.size();

        return msg.substr(start, end - start);
    }
};

int main() {
    WebSocketServer server;
    server.run(9002);
}
