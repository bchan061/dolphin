#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>

#include "Core/Core.h"
#include "Core/Debugger/PPCDebugInterface.h"

namespace GymServer {
    class GymServer {
        public:
        static GymServer& Instance();
        virtual ~GymServer();
        virtual void Start(int port);
        virtual bool IsRunning();
        virtual void Step();

        private:
        virtual void Listen(Core::CPUThreadGuard& guard,
                            PPCDebugInterface& debug_interface);
        virtual void SendServerPacket(Core::CPUThreadGuard& guard,
                                      PPCDebugInterface& debug_interface);
        sf::UdpSocket m_socket;
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

    struct MemoryPatch
    {
        uint32_t address;
        uint32_t value;

        MemoryPatch(uint32_t addr, uint32_t val) : address(addr), value(val) {}
    };

    struct ClientPacket {
        uint32_t size;
        char magic[4];
        MemoryPatch patch;
    };

    const sf::IpAddress DEFAULT_HOST = "127.0.0.1";
    const int DEFAULT_SERVER_PORT = 9001;
    const int DEFAULT_CLIENT_PORT = 9002;
    const static char* SERVER_PACKET_MAGIC = "GYM";
}
