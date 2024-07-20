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
    int found;
    char found_password[PASSWORD_LENGTH + 1];
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

}

int main(){
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];
    int charset_size = strlen(CHARSET);
    int range = 1;

    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        range *= charset_size;
    }

    for(int t = 0; t < NUM_THREADS; t++){
        pthread_create(&threads[t], NULL, threadFunction, (void *)&threadData[t]);
    }

    return 0;
}