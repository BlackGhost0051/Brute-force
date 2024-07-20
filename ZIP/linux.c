#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <pthread.h>

#define FILE_NAME "secret.zip"
#define NUM_THREADS 8
#define PASSWORD_LENGTH 4
#define CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

struct ThreadData {
    const char *file_name;
    int start_index;
    int end_index;
    int *global_found;
    char found_password[PASSWORD_LENGTH + 1];
    pthread_mutex_t *mutex;
};

void generate_password(char *password, int num){
    int charset_size = strlen(CHARSET);
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        password[PASSWORD_LENGTH - 1 - i] = CHARSET[num % charset_size];
        num /= charset_size;
    }
    password[PASSWORD_LENGTH] = '\0';
}

int unzip(const char *file_name, const char *password) {
    int err;
    
    zip_t *zfile = zip_open(file_name, ZIP_RDONLY, &err);
    if (!zfile) {
        printf("Error: Unable to open file\n");
        return 0;
    }

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat_index(zfile, 0, 0, &st);

    printf("Try -> %s\n", password);
    zip_file_t *zfile_file = zip_fopen_encrypted(zfile, st.name, 0, password);
    if (!zfile_file) {
        zip_close(zfile);
        return 0;
    }

    char buffer[4096];
    int bytes_read = zip_fread(zfile_file, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        zip_fclose(zfile_file);
        zip_close(zfile);
        return 0;
    }

    zip_fclose(zfile_file);
    zip_close(zfile);
    return 1;
}

void *threadFunction(void *arg){
    struct ThreadData *data = (struct ThreadData *)arg;
    char password[PASSWORD_LENGTH + 1];

    for (int num = data->start_index; num < data->end_index; num++) {
        generate_password(password, num);

        pthread_mutex_lock(data->mutex);
        if (*(data->global_found)) {
            pthread_mutex_unlock(data->mutex);
            return NULL;
        }
        pthread_mutex_unlock(data->mutex);

        if (unzip(data->file_name, password)) {
            pthread_mutex_lock(data->mutex);
            *(data->global_found) = 1;
            strcpy(data->found_password, password);
            pthread_mutex_unlock(data->mutex);
            return NULL;
        }
    }

    return NULL;
}

int main(){
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int charset_size = strlen(CHARSET);
    int range = 1;
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        range *= charset_size;
    }

    int step = range / NUM_THREADS;
    int global_found = 0;
    char found_password[PASSWORD_LENGTH + 1] = "";

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].file_name = FILE_NAME;
        threadData[i].start_index = i * step;
        threadData[i].end_index = (i + 1) * step;
        threadData[i].global_found = &global_found;
        threadData[i].mutex = &mutex;
        threadData[i].found_password[0] = '\0';
        pthread_create(&threads[i], NULL, threadFunction, (void *)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        if (threadData[i].found_password[0] != '\0') {
            strcpy(found_password, threadData[i].found_password);
        }
    }

    if (global_found) {
        printf("The password was correct: %s\n", found_password);
    } else {
        printf("Password not found.\n");
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}