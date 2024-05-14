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

## Condition variable in shared memory

```
chrt -f 95 ./o_cond_shm
secs: 250128, nsecs: 112508338
secs: 250129, nsecs: 112701940
secs: 250130, nsecs: 112759480
secs: 250131, nsecs: 112969999
secs: 250132, nsecs: 113162833
secs: 250133, nsecs: 113204232
secs: 250134, nsecs: 113395527
^C
```

```
chrt -f 95 ./i_cond_shm
secs: 250128, nsecs: 112559155
secs: 250129, nsecs: 112752692
secs: 250130, nsecs: 112831952
secs: 250131, nsecs: 113018830
secs: 250132, nsecs: 113209081
secs: 250133, nsecs: 113247114
secs: 250134, nsecs: 113445567
```

Again around 50 us delay between the signal and the wakeup


## Named semaphore

```
chrt -f 95 ./o_named_sem
secs: 282642, nsecs: 777912350
secs: 282643, nsecs: 778104845
secs: 282644, nsecs: 778145842
secs: 282645, nsecs: 778326973
secs: 282646, nsecs: 778446322
secs: 282647, nsecs: 778503648
secs: 282648, nsecs: 778686387
^C
```

```
chrt -f 95 ./i_named_sem
secs: 282642, nsecs: 777957726
secs: 282643, nsecs: 778152272
secs: 282644, nsecs: 778182705
secs: 282645, nsecs: 778383635
secs: 282646, nsecs: 778508413
secs: 282647, nsecs: 778519117
secs: 282648, nsecs: 778728790
```

Not much better
