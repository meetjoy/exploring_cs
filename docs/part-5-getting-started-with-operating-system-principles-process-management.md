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
## System call


===================
## From process 0 to session process
## interrupts
## Creation and termination of a process
fork(), execve() and _exit()
===================

## Experiment: installing VirtualBox
## Experiment: installing Ubuntu Desktop 18.04


