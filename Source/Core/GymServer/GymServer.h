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

        private:
        virtual void Run();
        sf::UdpSocket m_socket;
        std::thread m_socket_thread;
        bool m_running;
    };

    const int DEFAULT_PORT = 9001;
}