# Phi^3 Simulations

```
gdb ./Simulation
```

```
(gdb) run 0.1 0.1 16 16 100 100
Starting program: /home/jovyan/work/Simulation 0.1 0.1 16 16 100 100
...
```

Get the backtrace:
```
(gdb) bt
#0  0x0000aaaaaaaa2550 in Lattice::metropolis (this=this@entry=0xaaaaaaac6eb0, site=0) at Lattice.cpp:167
#1  0x0000aaaaaaaa14ec in main (argc=<optimized out>, argv=<optimized out>) at Simulation.cpp:176
```

Show local variables:
```
(gdb) info locals
currentPhi = 2.2476763404026929
newValue = <optimized out>
tmp = -0.0130296740680933
curr = 0xaaaaaaac9760
difference = <optimized out>
```

Show the arguments passed to the function:
```
(gdb) info args
this = 0xaaaaaaac6eb0
site = 0
```

List the source code:
```
(gdb) list
162
163	    // Compute energy difference.
164	    siteNeighbours* curr = neighbours[site];
165	    double difference = (currentPhi - newValue)
166	        * (lattice[curr->nextX] + lattice[curr->nextY]
167	        +  lattice[curr->prevX] + lattice[curr->prevY] );
168
169	    newValue *= newValue;
170	    currentPhi *= currentPhi;
171	    difference += muSquared * (newValue - currentPhi);
```

```
(gdb) print curr->nextX
$1 = 1
(gdb) print curr->nextY
$2 = 0
(gdb) print curr->prevX
$3 = 4294967295
(gdb) print curr->prevY
$4 = 0
(gdb) print lattice[1]
$5 = -1.0112703738268465
```

We then proceded to add some printing to the `getHelicalNeighbours()` function and just happened to see that `xDim` and `yDim` were being set to 0.
Which turned out to be an issue of **shadowing** because we were doing the following:
```cpp
Lattice::Lattice(double mu, double lambda, unsigned int xDim, unsigned int yDim) {
    ...
    xDim = xDim;
    yDim = yDim;
    ...
}
```
