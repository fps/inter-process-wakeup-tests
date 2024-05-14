Do not forget to use e.g. `chrt` to enable `SCHED_FIFO`. E.g.:

```
chrt -f 95 ./i_cond_shm 
```

```
chrt -f 95 ./o_cond_shm
```
