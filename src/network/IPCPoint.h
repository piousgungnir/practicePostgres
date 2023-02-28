#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

namespace ipc {

class Communication;

class Process
{
public:
    Process() = default;
#if __cplusplus >= 201402L
    virtual ~Process() = default;
#else
    virtual ~Process() {}
#endif

    virtual void onData(int32_t typecode, const std::vector<uint8_t> &data) = 0;

    void setCommunication(Communication* ipc) noexcept {m_communication = ipc;}
    void send(int32_t typecode, const uint8_t* data, std::size_t size) const;

    void unsetCommunication() noexcept { m_communication = nullptr; }

private:
    Communication* m_communication = nullptr;
};


class Communication
{
public:
    Communication() = default;
#if __cplusplus >= 201402L
    virtual ~Communication() = default;
#else
    virtual ~Communication() {}
#endif

    virtual void send(int32_t typecode, const uint8_t* data, std::size_t size) = 0;

    void addProcess(Process* ipcPoint) noexcept
    {
        if (ipcPoint == nullptr)
            return;

        bool isConnected = std::any_of(processes.begin(), processes.end(), [&ipcPoint](auto proc)
        {
            return proc == ipcPoint;
        });
        if (isConnected)
            return;

        processes.push_back(ipcPoint);
    }

    void removeProcess(Process* ipcPoint) noexcept
    {
        processes.erase(std::remove(processes.begin(), processes.end(), ipcPoint), processes.end());
    }

    void onData(int32_t typecode, const std::vector<uint8_t> &data);

private:
    std::vector<Process*> processes;
};

inline void Process::send(int32_t typecode, const uint8_t *data, std::size_t size) const
{
    if (m_communication)
    {
        m_communication->send(typecode, data, size);
    }
}

inline void Communication::onData(int32_t typecode, const std::vector<uint8_t> &data)
{
    for (auto & proc : processes)
    {
        if (proc)
        {
            proc->onData(typecode, data);
        }
    }
}

enum class ConnectionWay
{
    Dual/*SendAndReceive*/, OnlySend, OnlyReceive
};

inline void connect(Process* process, Communication* communication,
                    ConnectionWay way = ConnectionWay::Dual) noexcept
{
    if (communication == nullptr)
        return;
    if (process == nullptr)
        return;
    if (way == ConnectionWay::Dual or way == ConnectionWay::OnlySend)
        process->setCommunication(communication);
    if (way == ConnectionWay::Dual or way == ConnectionWay::OnlyReceive)
        communication->addProcess(process);
}

} // end of namespace ipc
