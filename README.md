To isolate commands we use immutable data structure from `immer` library. 
Each mutation on a container produces a new version of it, one can read concurrently without any lock.
See details [Immer](https://github.com/arximboldi/immer).

Use `join_server` binary to test with the input from `tests/test_cmd.txt`:
- `bulk_server 9000`, and then
- `cat tests/test_cmd.txt | nc localhost 9000`

you should get the following

```shell
OK
ERR duplicate 0
OK
OK
OK
OK
OK
OK
OK
OK
OK
OK
OK
3,violation,proposal
4,quality,example
5,precision,lake
0,lean,
1,sweater,
2,frank,
6,,flour
7,,wonder
8,,selection
OK
OK

```