#include "index.h"

#define QUEUE_SIZE 4
#define LENGTH 1000000

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
static void
string_producer(thread_t *thread) {
    queue_t *queue = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        while (queue_is_full(queue)) {}

        assert(queue_push_back(queue, uint_to_string(count)));
        count++;
    }
}

static void
string_consumer(thread_t *thread) {
    queue_t *queue = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        while (queue_is_empty(queue)) {}

        char *x = queue_pop_front(queue);
        char *y = uint_to_string(count);
        assert(string_equal(x, y));
        string_destroy(&x);
        string_destroy(&y);
        count++;
    }
}

void
test_queue_multi_thread(void) {
    test_start();

    queue_t *queue = queue_new(QUEUE_SIZE);

    {
        who_printf("uint_producer vs. uint_consumer\n");

        thread_t *producer_thread = thread_start(uint_producer, queue);
        thread_t *consumer_thread = thread_start(uint_consumer, queue);

        thread_join(producer_thread);
        thread_join(consumer_thread);
    }

    {
        who_printf("string_producer vs. string_consumer\n");

        thread_t *producer_thread = thread_start(string_producer, queue);
        thread_t *consumer_thread = thread_start(string_consumer, queue);

        thread_join(producer_thread);
        thread_join(consumer_thread);
    }

    queue_destroy(&queue);

    test_end();
}
