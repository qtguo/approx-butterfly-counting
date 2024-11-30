
# Effective Approximation Algorithms for Butterfly Counting in Bipartite Graph Streams

*This README awaits to be supplemented with more details.*

This repository implements vertex-sparsification-based algorithms that are first proposed in our paper for approximate butterfly counting in different streaming models, including the two-pass model (`VS-2PASS`), the random arrival model (`VS-1PASS`) and the time-based sliding window model (`VS-TSW`). For the time-based sliding window model, we also offer implementation of our basic method `B-TSW`. In summary, we implement the following methods: 

-  Two-Pass Model
   -  `VS-2PASS`
-  Random Arrival Model
   -  `VS-1PASS`
-  Sliding Window Model
   -  `B-TSW`
   -  `VS-TSW`

## Compile

**Two-Pass Model**

In directory `./code/`

```shell
g++ VS_2pass.cpp Ams.cpp -o vs2pass -std=c++11
```

**Random Arrival Model**

In directory `./code/`

```shell
g++ VS_1pass.cpp Ams.cpp -o vs1pass -std=c++11
```

**Sliding Window Model**

In directory `./code/TSW/`

```shell
cmake .
make
```

## File Description

`./code/`:

-  `AMS.*`: Implementation of the AMS sketch.
-  `VS_1pass.cpp`: Implementation of VS-1PASS algorithm.
-  `VS_2pass.cpp`: Implementation of VS-2PASS algorithm.

`./code/TSW:`

- `def.h`: Managing parameter setting, including algorithm version and key parameters for each algorithm version.
- `main.cpp`: Program entrance.
- `Sample.*`: Proceeding the sample maintaining steps.
- `SampleTable.*`: Abstraction of the sampled graph, with multiple methods for manipulation.

## Parameters

For the two-pass model and the random arrival model, the parameters are set in the execution command as follows:

```
./[name of the executable file] [path to the target dataset] [path to the result file] 0.5 [ratio of the size of AMS sketch to the space for sampled edges] [ground truth]
```

For the sliding window model, there are some parameters to be set in `./code/TSW/def.h`

-  `ALG [0|2]`: Choice of algorithm version
   -  `0` means `B-TSW`
   -  `2` means `VS-TSW`
-  `left_size`: Number of vertices on the `L` side of the target graph.

## Input & Output

The graph must be stored in the **edge list** format. The vertices on both sides are numbered independently. For example:

```
1 1
1 2
1 3
2 3
```

The result will be outputed to the designated file according to the defined result file path.

