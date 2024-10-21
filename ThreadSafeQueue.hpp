#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @brief A thread-safe queue implementation using templates.
 *
 * @tparam T The type of elements stored in the queue.
 */
template <typename T>
class ThreadSafeQueue {
public:
	/**
    * @brief Constructs a ThreadSafeQueue with a maximum size.
    *
    * @param max_size The maximum number of elements the queue can hold.
    */
    ThreadSafeQueue(size_t max_size) : max_size(max_size) {}

    /**
     * @brief Adds an item to the queue.
     *
     * If the queue is full, the method blocks until space becomes available.
     *
     * @param item The item to add to the queue.
     */
    void enqueue(const T& item) {
        std::unique_lock<std::mutex> lock(mutex);

        cond_var_full.wait(lock, [this]() {return safe_queue.size() < max_size; });

        safe_queue.push(item);
        cond_var_empty.notify_one();
    }

    /**
     * @brief Removes and returns an item from the queue.
     *
     * If the queue is empty, the method blocks until an item becomes available.
     *
     * @return The item removed from the queue.
     */
    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex);

        cond_var_empty.wait(lock, [this]() {return !safe_queue.empty(); });

        T item = safe_queue.front();
        safe_queue.pop();
        cond_var_full.notify_one();

        return item;
    }

private:
    std::queue<T> safe_queue;               /**< The underlying queue */
    size_t max_size;                        /**< Maximum capacity of the queue */
    std::mutex mutex;                       /**< Mutex for synchronizing access */
    std::condition_variable cond_var_full;  /**< Condition variable for full queue */
    std::condition_variable cond_var_empty; /**< Condition variable for empty queue */
};

