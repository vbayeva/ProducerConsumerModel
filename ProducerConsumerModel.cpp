// ProducerConsumerModel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "ThreadSafeQueue.hpp"

void producer(ThreadSafeQueue<int>& queue, int producer_id) {
    for (int i = 0; i < 10; ++i) {
        int item = producer_id + i;
        queue.enqueue(item);

        std::cout << "Producer " << producer_id << " produced item: " << item << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(ThreadSafeQueue<int>& queue, int consumer_id) {
    for (int i = 0; i < 10; ++i) { // Consume 10 items
        int item = queue.dequeue();

        std::cout << "Consumer " << consumer_id << " processed item: " << item << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main()
{
    ThreadSafeQueue<int> queue(5); 

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 1; i <= 2; ++i) {
        producers.emplace_back(producer, std::ref(queue), i);
    }

    for (int i = 1; i <= 2; ++i) {
        consumers.emplace_back(consumer, std::ref(queue), i);
    }

    for (auto& producer : producers) {
        producer.join();
    }

    for (auto& consumer : consumers) {
        consumer.join();
    }

    std::cout << "All producers and consumers have finished" << std::endl;

	return 0;
}
