# ZIP

This program brute-forces passwords for ZIP file. ( **Linux** )


## How run?
1.
```
gcc -o linux linux.c -lzip -lpthread
```
```
./linux
```
## Data that the user can edit.
### FILE_NAME
File name - is the name of the file to be brute-forced.
```c
#define FILE_NAME "secret.zip"
```
### NUM_THREADS
Threads number - you have to change value for your own device.
```c
#define NUM_THREADS 8
```
### PASSWORD_LENGTH
Password length.
```c
#define PASSWORD_LENGTH 4
```
### CHARSET
Symbols used in brute force.
```c
#define CHARSET "0123456789abcde..."
```