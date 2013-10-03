#include "thread_lib.h"

shared_buffer_t * thread_new_shared_buffer()
{
    
    shared_buffer_t * shared;
    shared = calloc(1, sizeof(shared_buffer_t));
    shared->buffer = calloc(MAX_BUFFER_LEN, sizeof(char));
    return shared;
}

void thread_free_shared_buffer(shared_buffer_t * shared)
{
    free(shared->buffer);
    free(shared);
}

void thread_copy_to_buffer(shared_buffer_t * shared, const char * source)
{
    /* lock */
    pthread_mutex_lock(&(shared->mutex));
    
    /* copy string to thread_buffer */
    
    memcpy(shared->buffer, source, sizeof(char) * MAX_BUFFER_LEN);
    
    /* mark the msg as updated */
    shared->is_changed = 1;
    
    /* unlock */
    pthread_mutex_unlock(&(shared->mutex));
}
int thread_copy_from_buffer(shared_buffer_t * shared, char * destination)
{
    int result;
    result = 0;//lets the caller know a copy was performed
    
    /* lock */
    pthread_mutex_lock(&(shared->mutex));
    
    if(shared->is_changed)
    {
        /* copy string to thread_buffer */
        memcpy(destination,shared->buffer, sizeof(char) * MAX_BUFFER_LEN);
        /* mark the msg as updated */
        shared->is_changed = 0;
        result = 1;
    }
    
    /* unlock */
    pthread_mutex_unlock(&(shared->mutex));
    
    return result;
}