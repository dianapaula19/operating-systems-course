# Operating Systems

> **Original version:** this README and some fixes were added in 2026. To see the project exactly as it was first built, browse commit [`3048e80`](https://github.com/dianapaula19/operating-systems-course/tree/3048e8045d50b8ff456b7327fb1c0d7867928da3) (2021-10-02).

C homework for the *Operating Systems* course at the University of Bucharest:
UNIX system programming with low-level file I/O, processes, pipes, signals and directories.
Each file starts with the assignment (in Romanian).

| Folder | Program |
|---|---|
| `dfwseet_a5` | `getint(fd)`: read a decimal integer from a file descriptor using only `read` (a low-level `fscanf("%d")`) |
| `dfwseet_a9` | Several `fork`ed children reading from the keyboard at the same time: who gets the character? |
| `dfwseet_a13` | Parallel divide-and-conquer search: each half of the array is searched by a child process, recursively |
| `dfwseet_a22` | Level-order traversal of a binary tree using an anonymous **pipe** as the queue |
| `ietud2_d2` | Total size of a directory tree (`opendir` / `readdir` / `stat`) |
| `sn6d_2`, `sn6d_4` | Signals: sending signals between processes, `alarm` + `SIGALRM` to break out of an infinite loop |
| `teme_f16` | A `wc` clone with the `-c`, `-l` and `-w` options |
| `teme_f2` | Primality check from the command line |

## Examples

```
$ gcc dfwseet_a22/bfs.c -o bfs && ./bfs 50 30 70 20 40 60 80
50 30 20 40 70 60 80        <- pre-order
50 30 70 20 40 60 80        <- level order, using a pipe as the queue

$ gcc teme_f16/wc.c -o mywc && ./mywc -lw teme_f16/test.txt
# lines: 3
# words: 6
```

Build any program with `gcc path/to/file.c -o program` (Linux or macOS).
