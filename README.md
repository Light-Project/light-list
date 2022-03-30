# Light-list user manual 

Light-list is a linked list library implemented in C language. Although its code size is small, it provides rich traversal syntactic sugar.

## Compilation Instructions

It does not has any external dependency, so just make it directly in the project root directory.

```shell
cd light-list
make
```

### Run some simple tests

After you finish compiling above, you can run some simple tests.

```shell
./examples/benchmark
```

Here is the output of benchmark:

```shell
Generate 1000000 Node:
  real time: 0.030000
  user time: 0.020000
  kern time: 0.000000
List sort:
  real time: 0.170000
  user time: 0.170000
  kern time: 0.000000
List for each:
  real time: 0.070000
  user time: 0.070000
  kern time: 0.000000
Deletion All Node...
Done.
```

## Based on Development 

Light-rbtree library requires only two files to complete the migration.

```
src/sort.c
src/list.h
```

## License

This is an open source list library, which uses the GPLV2 protocol

## Discussion Group

The Tencent QQ Group number is: [763756024](https://jq.qq.com/?_wv=1027&k=UhogIfXA)

