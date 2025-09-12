// 1️⃣ Define a simple task queue
typedef struct Task {
    void (*callback)(void *);
    void *arg;
    struct Task *next;
} Task;

typedef struct TaskQueue {
    Task *head;
    Task *tail;
    pthread_mutex_t lock;
} TaskQueue;

void init_queue(TaskQueue *queue) {
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->lock, NULL);
}

// 2️⃣ Enqueue tasks (defer work)
void enqueue(TaskQueue *queue, void (*callback)(void *), void *arg) {
    Task *task = malloc(sizeof(Task));
    task->callback = callback;
    task->arg = arg;
    task->next = NULL;

    pthread_mutex_lock(&queue->lock);
    if (!queue->tail) {
        queue->head = task;
    } else {
        queue->tail->next = task;
    }
    queue->tail = task;
    pthread_mutex_unlock(&queue->lock);
}

// 3️⃣ Dequeue and process tasks
void process_queue(TaskQueue *queue) {
    while (1) {
        pthread_mutex_lock(&queue->lock);
        Task *task = queue->head;
        if (task) {
            queue->head = task->next;
            if (!queue->head) queue->tail = NULL;
        }
        pthread_mutex_unlock(&queue->lock);

        if (!task) break;

        // Execute the deferred work
        task->callback(task->arg);
        free(task);
    }
}

// 4️⃣ Example usage
// A mediator removal callback
void remove_mediator_task(void *arg) {
    char *name = (char *)arg;
    printf("Removing mediator: %s\n", name);
    free(name); // cleanup if dynamically allocated
}

void handle_notification() {
    // Instead of removing immediately:
    enqueue(&global_queue, remove_mediator_task, strdup("Mediator1"));
}

// Later, in main loop
process_queue(&global_queue);
