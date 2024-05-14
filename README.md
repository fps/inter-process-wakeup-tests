# Notes


## chrt
Do not forget to use e.g. `chrt` to enable `SCHED_FIFO`. E.g.:

```
chrt -f 95 ./i_cond_shm 
```

```
chrt -f 95 ./o_cond_shm
```

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

## Condition variable in SHM

You have to run the `i_cond_shm` before `o_cond_shm` because the former initializes the mutex and condition variable in the shared memory.

# Results

Here are some example results..

# AsRock N100DC-itx (Intel N100 CPU)

## Named pipes

```
chrt -f 95 ./o_pipe pipe1
secs: 248093, nsecs: 126818103
secs: 248094, nsecs: 126890207
secs: 248095, nsecs: 126945084
secs: 248096, nsecs: 127015588
secs: 248097, nsecs: 127142144
secs: 248098, nsecs: 127253128
secs: 248099, nsecs: 127326546
^C
```

```
chrt -f 95 ./i_pipe pipe1
secs: 248093, nsecs: 126866631
secs: 248094, nsecs: 126917198
secs: 248095, nsecs: 126997900
secs: 248096, nsecs: 127080546
secs: 248097, nsecs: 127187170
secs: 248098, nsecs: 127293099
secs: 248099, nsecs: 127393779
Done.
```

As we can see it takes around 50 - 60 microseconds for the `i_pipe` to wakeup after `o_pipe` wrote a byte to the named pipe.

