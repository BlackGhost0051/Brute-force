#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <pthread.h>

#define NUM_THREADS 8
#define PASSWORD_LENGTH 4
#define CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

int unzip(const *file_name, const char *password){
    int err = 0;
    struct zip *za = zip_open(file_name, 0, &err);

    if(za == NULL){
        printf("Could not open the archive\n");
        return 0;
    }

    if (zip_set_default_password(za, password) != 0) {
        zip_close(za);
        return 0;
    }
}

int main(){
    return 0;
}