#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

class BlockingQueue {
public:
    void push(int value) {
        {
            std::lock_guard<std::mutex> guard(lock_);
            queue_.push(value);
        }
        ready_.notify_one();
    }

    std::optional<int> pop() {
        std::unique_lock<std::mutex> guard(lock_);
        ready_.wait(guard, [this] { return stopped_ || !queue_.empty(); });

        if (queue_.empty()) {
            return std::nullopt;
        }

        int value = queue_.front();
        queue_.pop();
        return value;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> guard(lock_);
            stopped_ = true;
        }
        ready_.notify_all();
    }

private:
    std::mutex lock_;
    std::condition_variable ready_;
    std::queue<int> queue_;
    bool stopped_ = false;
};

int main() {
    BlockingQueue queue;
    constexpr int items = 100;
    long sum = 0;

    std::thread consumer([&] {
        while (true) {
            std::optional<int> value = queue.pop();
            if (!value.has_value()) {
                break;
            }
            sum += *value;
        }
    });

    for (int i = 1; i <= items; ++i) {
        queue.push(i);
    }
    queue.stop();
    consumer.join();

    const long expected = (static_cast<long>(items) * (items + 1L)) / 2L;
    std::cout << "expected_sum=" << expected << '\n';
    std::cout << "actual_sum=" << sum << '\n';
    std::cout << "consumer_joined=yes\n";
    std::cout << "thread_safe_queue_verified=" << (sum == expected ? "yes" : "no") << '\n';

    return 0;
}
