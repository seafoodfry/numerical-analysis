# Ising Model Simulations

```
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 64 64 100000 100000 10
0.100000	5.670887	-1.999256	0.010658	0.999814	0.000001	0.070779	0.000442	0.999814	0.000000

real	1m27.622s
user	1m27.603s
sys	0m0.018s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 128 128 100000 100000 10
0.100000	30293.461682	-0.951324	1.266202	0.475691	0.388705	1634137.499800	40858.425248	0.475691	0.249380

real	2m55.105s
user	2m55.099s
sys	0m0.009s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 256 256 100000 100000 10
0.100000	5610.638493	-0.164995	0.644181	0.082499	0.092160	1984145.131848	49605.142137	0.082499	0.075691

real	2m41.963s
user	2m41.960s
sys	0m0.005s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 512 512 100000 100000 10
0.100000	1170.738598	-0.033920	0.300172	0.016960	0.019758	1748202.238853	43705.321587	0.016960	0.016672

real	1m38.856s
user	1m38.854s
sys	0m0.005s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 1024 1024 100000 100000 10
0.100000	40.290652	-0.001160	0.055897	0.000580	0.000683	243127.608153	6078.201797	0.000580	0.000580

real	1m19.380s
user	1m19.367s
sys	0m0.009s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 2024 2024 100000 100000 10
0.100000	131.566380	-0.003800	0.101079	0.001900	0.002234	3107479.270033	77687.019679	0.001900	0.001896

real	4m12.237s
user	4m12.226s
sys	0m0.018s
(base) jovyan@1549756b159e:~/work$ time ./Metropolis 2024 2024 1000000 100000 10
0.100000	131.566380	-0.003800	0.101079	0.001900	0.002234	3107479.270033	77687.019679	0.001900	0.001896

real	5m19.628s
user	5m19.608s
sys	0m0.020s
```