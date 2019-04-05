Lab 4.1
=======

Mutexes
-------
**bug6.c** shows a solution to a common problem with mutexes when a thread does not lock a variable that all the other threads have access
to. The simple solution is enclosing the changing of the value in a pthread_mutex_lock and a pthread_mutex_unlock. This ensures that no other
threads will change the value of this variable when another thread is already changing it.

Condition Variables
-------------------
**bug1.c** and **bug4.c** show the solution to common problems when using condition variables.
The problem with **bug1.c** was that the signal to tell the threads that the condition was met was not being sent to all the threads. This means
that not everyone could see this to continue working.  
**bug4.c** had the value of the condition variable changing inside a loop that only stop when a threshold was met. It actually needed to be
changed after the loop was done and then count could be unlocked. Otherwise it would stay in that loop forever as all the threads would wait
for the condition variable to be met.  

Compiling and Running
---------------------
To compile these programs simply type `make` in your terminal. This will create the binary files **bug1**, **bug4**, and **bug6**. You can run each
of these programs individually by typing `./<bug1 || bug4 || bug6>`.  
To delete all binary files from your directory type `make clean`
