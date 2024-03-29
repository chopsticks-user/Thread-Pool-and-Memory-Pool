#ifndef USHI_SOURCE_THREAD_POOL_HPP
#define USHI_SOURCE_THREAD_POOL_HPP

#include "Base.hpp"
#include "ContractManager.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"
#include "WorkerManager.hpp"

namespace ushi {

class ThreadPool {
public:
  static u32 nConcurrentThreads() noexcept {
    return std::thread::hardware_concurrency();
  }

public:
  ThreadPool() = delete;
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) noexcept = default;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) noexcept = default;
  ~ThreadPool() noexcept { this->clear(); }

  explicit ThreadPool(u64 nThreads) noexcept
      : mContractManager{std::make_unique<ContractManager>()},
        mWorkerManager{std::make_unique<WorkerManager>(nThreads)}, mMutex{} {}

  u64 nThreads() const noexcept { return this->mWorkerManager->nWorkers(); }

  u64 nAvailableThreads() const noexcept {
    return this->mWorkerManager->nAvailableWorkers();
  }

  bool busy() const noexcept {
    return this->mWorkerManager->nAvailableWorkers() == 0;
  }

  ContractStatusPtr submitJob(JobType job) noexcept;

  // TODO: submitJob with a user-defined contract status
  // template <typename CustomStatusType> //
  // ContractStatusPtr
  // submitJob(std::function<void(CustomStatusType &)> job) noexcept;

  void clear() noexcept {
    this->mWorkerManager->clear();
    this->mContractManager->clear();
  }

private:
  std::unique_ptr<ContractManager> mContractManager;
  std::unique_ptr<WorkerManager> mWorkerManager;
  std::mutex mMutex;
};

} // namespace ushi

#endif // USHI_SOURCE_THREAD_POOL_HPP