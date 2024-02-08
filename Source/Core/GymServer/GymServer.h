#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <thread>

namespace GymServer {
    class GymServer {
        public:
        static GymServer& Instance();
        virtual ~GymServer();
        virtual void Start(int port);
        virtual bool IsRunning();
        virtual void Step();

        private:
        sf::UdpSocket m_socket;
        std::thread m_socket_thread;
        bool m_running;
        int m_server_port;
    };

    struct MemoryWatch {
        uint32_t address;
        uint32_t value;

        MemoryWatch(uint32_t addr, uint32_t val) : address(addr), value(val) {}
    };

    struct ServerPacket {
        uint32_t size;
        char magic[4];
        MemoryWatch* watches;
    };

    const sf::IpAddress DEFAULT_HOST = "127.0.0.1";
    const int DEFAULT_SERVER_PORT = 9001;
    const int DEFAULT_CLIENT_PORT = 9002;
    const static char* SERVER_PACKET_MAGIC = "GYM";
}