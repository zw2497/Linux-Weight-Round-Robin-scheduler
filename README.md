# Test Program

## Test 1

* `test/gen_cpu_proc.c`
* `test/gen_io_proc.c`

These two tests fork N children to test for both CPU-bound and I/O-bound processes.
 We ran the gen_io_proc test first, and then ran gen_cpu_proc immediately afterwards. 
 Based on the wrr_info outputted (especially when we set CHILD_PROC to a large number),
 we can see that load balancing works correctly, and CPU-bound & IO-bound processes can
 run concurrently.

## Test 2

* `test/fork_many.c`
* `test/weight_test.c`

To test the impact that changing task weights had, we created two tests--fork_many and weight_test.

fork_many spawns 100 child processes that find the factors of a large number, while weight_test just
finds the factors of a large number.

We ran fork_many first, and then timed weight_test with different weights passed in.

In our tests, with weight 1, weight_test finished after 9.125 seconds. However, when we changed the weight
to 100, weight_test finished in 6.857 seconds. Based on this, we see that assigning a higher weight allows
that process to finish more quickly because it has a large timeslice and is given more CPU time.
