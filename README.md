# Notes

## General remarks

For some `i_`/`o-`/ pairs it is necessary for the `o_` program to be run first. For the others it doesn't matter, so if you run the `o_` program first in all cases it is always correct :)

## Realtime scheduling

Do not forget to use e.g. `chrt` to enable `SCHED_FIFO`. E.g.:

```
chrt -f 95 ./i_cond_shm 
```

```
chrt -f 95 ./o_cond_shm
```

Also unless you have frequency scaling completely disabled it probably helps to run e.g. `stress -c <number_of_threads_of_your_processor>` to put load onto the system that forces it to the highest clock frequency.

## Named pipes

for the `i_pipe` and `o_pipe` programs you need to create a named pipe with e.g.

```
mkfifo pipe
```

And then running

```
chrt -f 95 ./i_pipe pipe
```

and 

```
chrt -f 95 ./o_pipe pipe
```

## Condition variable in shared memory

You have to run the `i_cond_shm` before `o_cond_shm` because the former initializes the mutex and condition variable in the shared memory.

# Results

Here are some example results..

# AsRock N100DC-itx (Intel N100 CPU)

```
uname -a
Linux ogfx100 6.6.30-rt30 #1-NixOS SMP PREEMPT_RT Thu May  2 14:32:50 UTC 2024 x86_64 GNU/Linux
```

## Named pipes

## Condition variable in shared memory

## Named semaphore

## Unnamed semaphore

## Spinning on a bare int in shared memory

```
./o_bare_int
secs: 284881, nsecs: 624681713
secs: 284882, nsecs: 624790675
secs: 284883, nsecs: 624922829
secs: 284884, nsecs: 625064580
secs: 284885, nsecs: 625205841
secs: 284886, nsecs: 625302734
secs: 284887, nsecs: 625440672
^C
```

```
[nix-shell:~/src/flow]$ ./i_bare_int
secs: 284881, nsecs: 624688671
secs: 284882, nsecs: 624791383
secs: 284883, nsecs: 624925238
secs: 284884, nsecs: 625065518
secs: 284885, nsecs: 625206521
secs: 284886, nsecs: 625303560
secs: 284887, nsecs: 625441486
```

0.8 us - that's more like it :)

## Futex


