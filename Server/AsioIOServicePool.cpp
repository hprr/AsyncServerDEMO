#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _ioServices(size), _works(size), _nextIOService(0), _threads(size) {
    for (std::size_t i = 0; i < size; ++i) {
        // 使用新的 executor_work_guard 来保持 io_context 处于工作状态
        _works[i] = std::make_unique<Work>(_ioServices[i].get_executor());
    }
    // 遍历多个 io_context，创建多个线程，每个线程内部启动 io_context
    for (std::size_t i = 0; i < _ioServices.size(); ++i) {
        _threads[i] = std::thread([this, i]() {
            _ioServices[i].run();
            });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    Stop();
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
    auto& service = _ioServices[_nextIOService++];
    if (_nextIOService >= _ioServices.size()) {
        _nextIOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop() {
    // 因为仅仅执行 work.reset 并不能让 io_context 从 run 的状态中退出
    // 当 io_context 已经绑定了读或写的监听事件后，还需要手动 stop 该服务。
    for (auto& work : _works) {
        // 重置 work 对象以允许 io_context 跳出 run 循环
        work.reset();
    }
    for (auto& ioService : _ioServices) {
        // 手动停止 io_context
        ioService.stop();
    }
    for (auto& t : _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
