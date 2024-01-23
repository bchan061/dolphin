#define _WEBSOCKETPP_NO_EXCEPTIONS_

#include <iostream>
#include <chrono>

#include "GymServer.h"

namespace GymServer {
    GymServer& GymServer::Instance() {
        static GymServer instance;
        return instance;
    }

    GymServer::~GymServer() {
        m_running = false;
        m_socket_thread.join();
    }

    void GymServer::Start(int port) {
        std::cout << "Setting up GymServer at port " << port << std::endl;
        if (m_socket.bind(port) != sf::Socket::Done) {
            std::cerr << "Unable to bind GymServer UDP socket to " << port << std::endl;
            return;
        }

        m_socket_thread = std::thread(&GymServer::Run, this);

        m_running = true;
    }

    void GymServer::Run() {
        while (m_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));

            char test[5] = "TEST";
            sf::IpAddress recipient = "127.0.0.1";
            m_socket.send(test, 5, recipient, 9002);
        }
    }

    bool GymServer::IsRunning() {
        return m_running;
    }
}
