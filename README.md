# Adjoint Graph Checker

Combinatorial algorithms in bioinformatics - student project.
This software includes some simple functions for checking if loaded graph is an adjoint/line graph and transforming them into their original graph.

## Getting started

Repository contains compiled version of ADG for linux, source code and several test graphs.

### Installing

Just clone repository to your computer.
You can run it by typing ./AGC on linux in AGC directory.

## Use

AGC supports text files (*.txt) in the following format:

```
5
1 2
2 3
3 4
3 5
```

* First line - number of vertices
* First column - starting vertex of edge
* Second column - end vertex edge

AGC supports graphs with vertices named by integers in range from 1 to C++ int length.

## Authors

* **Jakub Wajs** - All the work - [github](https://github.com/kubawajs)

## License

Creative Commons
