# OS project multi-threading vs single-thread

This is my code for OS Course Project. 


## Project Description: 
I implemented a text search engine where its input is a file and a list of words. The search engine outputs a file containing the search results.  Since searching for large files is time-consuming, I used 4 threads to search in the file. Due to a large number of threads that can reduce the processing speed, the program creates a maximum of four threads. In multithreading to prevent the race condition, the program is implemented in two modes:  an implementation using i) Mutex Lock, and ii) semaphore. For comparing the processing time I also implemented the algorithm with a single thread.


## Input

A text file and a list of words entered by the user in the console.


## Output

A file containing the search results and processing time for the multi-threaded and single-threaded program.
