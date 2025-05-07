#include "index.h"

#define QUEUE_SIZE 1024
#define LENGTH 10000000

static void
uint_producer(thread_t *thread) {
    queue_t *queue = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        while (queue_is_full(queue)) {}

        assert(queue_push_back(queue, (void *) count));
        count++;
    }
}

static void
uint_consumer(thread_t *thread) {
    queue_t *queue = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        while (queue_is_empty(queue)) {}

        assert(((size_t) queue_pop_front(queue)) == count);
        count++;
    }
}

void
test_queue_throughput(void) {
    test_start();

    queue_t *queue = queue_new(QUEUE_SIZE);

    double start_second = time_second();

    thread_t *producer_thread = thread_start(uint_producer, queue);
    thread_t *consumer_thread = thread_start(uint_consumer, queue);

    thread_join(producer_thread);
    thread_join(consumer_thread);

    double throughput = LENGTH / 1000 / time_passed_second(start_second);
    who_printf("throughput: %.f k/s\n", throughput);

    queue_destroy(&queue);

    test_end();
}
