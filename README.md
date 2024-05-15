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

## Unnamed semaphore
  wefewf
```
chrt -f 95 ./o_unnamed_sem
secs: 283400, nsecs: 964203046
secs: 283401, nsecs: 964399039
secs: 283402, nsecs: 964592270
secs: 283403, nsecs: 964785499
secs: 283404, nsecs: 964977858
secs: 283405, nsecs: 965115501
secs: 283406, nsecs: 965315677
```

```
chrt -f 95 ./i_unnamed_sem
secs: 283400, nsecs: 964254114
secs: 283401, nsecs: 964447141
secs: 283402, nsecs: 964639888
secs: 283403, nsecs: 964834067
secs: 283404, nsecs: 965028431
secs: 283405, nsecs: 965167046
secs: 283406, nsecs: 965363366
```

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

```
[nix-shell:~/src/flow]$ chrt -f 95 ./o_futex
secs: 84478, nsecs: 257347773
secs: 84479, nsecs: 257450959
secs: 84480, nsecs: 257483084
secs: 84481, nsecs: 257542892
secs: 84482, nsecs: 257571720
secs: 84483, nsecs: 257595752
secs: 84484, nsecs: 257620863
secs: 84485, nsecs: 257644488
secs: 84486, nsecs: 257678295
^C
```

```
[fps@ogfx100:~/src/flow]$ chrt -f 95 ./i_futex
secs: 84478, nsecs: 257390305
secs: 84479, nsecs: 257483606
secs: 84480, nsecs: 257513550
secs: 84481, nsecs: 257550645
secs: 84482, nsecs: 257614182
secs: 84483, nsecs: 257602979
secs: 84484, nsecs: 257653332
secs: 84485, nsecs: 257677168
secs: 84486, nsecs: 257685790
^C
```
