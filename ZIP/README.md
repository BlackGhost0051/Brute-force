# ZIP
## Brute force


## How run?
```
gcc -o linux linux.c -lzip -lpthread
./linux
```
## Data that the user can edit.
### FILE_NAME
File name
```c
#define FILE_NAME "secret.zip"
```
### NUM_THREADS
Threads number
```c
#define NUM_THREADS 8
```
### PASSWORD_LENGTH
Password length
```c
#define PASSWORD_LENGTH 4
```
### CHARSET
Symbols used in brute force
```c
#define CHARSET "0123456789abcde..."
```