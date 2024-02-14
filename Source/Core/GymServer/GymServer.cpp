#define _WEBSOCKETPP_NO_EXCEPTIONS_

#include <iostream>
#include <chrono>

#include "GymServer.h"
#include "Core/Core.h"
#include "Core/System.h"
#include "Core/PowerPC/PowerPC.h"
#include "Core/Debugger/PPCDebugInterface.h"
#include "VideoCommon/VideoEvents.h"

namespace GymServer {
    GymServer& GymServer::Instance() {
        static GymServer instance;
        return instance;
    }

    GymServer::~GymServer() {
        m_running = false;
    }

    void GymServer::Start(int port) {
        m_server_port = port;

        std::cout << "Setting up GymServer at port " << port << std::endl;
        if (m_socket.bind(port) != sf::Socket::Done) {
            std::cerr << "Unable to bind GymServer UDP socket to " << port << std::endl;
            return;
        }

        m_running = true;
    }

    void GymServer::Step() {
        if (!m_running) {
            return;
        }

        auto& system = Core::System::GetInstance();
        auto& debug_interface = system.GetPowerPC().GetDebugInterface();

        Core::CPUThreadGuard guard(system);
        const size_t num_watches = debug_interface.GetWatches().size();
        std::vector<Common::Debug::Watch> debug_watches = debug_interface.GetWatches();
        std::vector<MemoryWatch> memory_watches;
        for (size_t i = 0; i < debug_watches.size(); i++) {
            const auto& debug_watch = debug_watches[i];
            const uint32_t watch_value = debug_interface.ReadMemory(guard, debug_watch.address);
            memory_watches.emplace_back(debug_watch.address, watch_value);
        }

        // packet_size: 4 bytes
        // magic: 4 bytes
        // each watch: 8 bytes
        const size_t packet_size = 4 + 4 + sizeof(MemoryWatch) * num_watches;
        std::vector<uint8_t> packet_buffer_vector(packet_size);
        uint8_t* packet_buffer = packet_buffer_vector.data();

        ServerPacket* packet = reinterpret_cast<ServerPacket*>(packet_buffer);
        packet->size = static_cast<uint32_t>(packet_size);
        strcpy(packet->magic, SERVER_PACKET_MAGIC);
        memcpy(packet_buffer + 8, memory_watches.data(), sizeof(MemoryWatch) * num_watches);

        m_socket.send(packet_buffer, packet_size, DEFAULT_HOST, DEFAULT_CLIENT_PORT);
    }

    bool GymServer::IsRunning() {
        return m_running;
    }
}
