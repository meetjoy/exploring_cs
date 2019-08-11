# Part 5 Getting started with operating system principles: process management

## What is a process? Where are they? Process content? 

Hi guys. In this vedio we are going to look at some basic concepts.
Firstly, what is a process? A short answer is: a process is the instance of a program that is being executed. Let's open a linux terminal to see what we can get using **top** command. (demo)

Now we know, there are many processes existing at the same time in a running operating system. Each of these processes has a unique id, status, user, priority and time proper etc.. Then we can draw a table like this. We use this table to indicate a concept that each process takes a space inside of the entire memory space. Logically, we divide the memory into many space slots, each slot can be put in a process.

What's inside of each process? There is definitely code stuff inside of the process. Then when a process is running, it needs some space for the data. Currently we can image there are roughly two parts inside of a process space. Bottom part is the code, top part for the data. 

## What is a process group? What is a session? 

A process group denotes a collection of one or more processes.
A session denotes a collection of one or more process groups.
(demo: cat | grep 123 &;ps -o pid,ppid,pgrp,command;kill -9 -)

## Process cycle

We have introduced some concepts, now let us try to tell a stroy on how a simple operating system deals with its processes in the initialation stage. Let us image after the very beginning stage of the operating system initialization, the very first process created. The process id of the very first process is 0. If there's some function that can be called inside of the code of process 0 to generate another process, then we can use this function whenever a new process is required. Actually there is one, fork() is the right function we are looking for. Follow our thoughts that we write down the fork() in somewhere inside of process 0, then a new process is created and it takes another slot space. Even we now have two processes, at anything particular time, there is only one process using the CPU, which is running. All other processes is either waiting for its turn to run, sleeping, or in other status like stopping or in a zombie status.

Similarly, process 1 uses fork() to create another process let's give this child process a name: session leader. Yes, we want this process to be the leader of our very first session. But after this session leader was just created, we have to do something to make this process to be special: a session leader. Say there is a function called setsid() can be called to set this process to be a session leader. 

The experience of using terminal tells us that for some commands in a command line enviroment, they need us to input something before the program continues, let us think the content that the program waitting for can be called as input. Also, many programs print some characters onto the screen as a result or during the execution process. We call the content printed on the screen as output. 

Everytime a new process is created, the child process runs the same code area as the parent process. So if we would like run a new program inside of the child process, some function required to be invoked to load the new program inside of the child process and make it ready to run. The execve() is such function we are looking for. 

If the program is being loaded into the session leader process is somekind of bash, so far we successfuly bring the command line to us. 

Well wait a second, we forget something, the input and output of the session leader process. Now we image both the input and output as some kind of files. When we need to read something from the command line, we image this likes reading something from the input file. Similarly, printing something to the terminal likes writting to the output file. That's why we openned 3 files in process session leader. 

## User space and kernel space

By now we've mentioned serveral functions that help establish the system, like fork(), setsid() and execve() etc.. One question is where the content of these functions are. When the program is loaded into any process, the process slot space holds the code and data for the program. It's not proper to put all of functions, the fork(), setsid(), execve() I mean here, into each of the process. Ideally it's in a separate place other than the processes space we talked here. Now we image all the common functions can be put in one place, we call this kernel space. Correspondingly, we call the previous spaces for each process the user space. So when the CPU is executing the code inside of the process space, we say it's running in the user state ce of the process. Then we call the CPU is running in the kernel space when the code is executing the code inside of the kernel space.

## System call

So far we know every time one of the system call like fork() is invoked, the CPU moves from the user space to the kernel space. After the system call finishes, CPU moves back to the user space from the kernel space, some times. One chance for the system call is the system call can change the status of the process. If the status of process is not runable after the system call so we have to switch to some other process which is runnable. So it's not hard for us to understand that, some code needs to be put after each of the system call to check whether a sheduling of the process is required, and do so if it is. For now the flow the system call is roughly:
System call -> C handler -> schdule() if required.
That's not all, but we will disclose more shortly.

## Interprocess communication

In the above example we understand that the time CTRL + C pressed, the signal was sent to the processes in the front end group. When these signals can be handled? One good occasion would be after each of the system call. Actually it is a routine stage after the above steps of the system call. For now, we covered all the key steps of the whole system call:
system call -> C handler -> schedule() -> do signal().

## Interrupts



## Experiment: installing VirtualBox
## Experiment: installing Ubuntu Desktop 18.04