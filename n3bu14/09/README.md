# [Level09](https://exploit-exercises.com/nebula/level09/)
The vulnerability is in the fact that we can use the variable `$use_me` in the
php script by crafting a file like `[email {$use_me}]` and passing it as the first
argument of `/home/flag09/flag09`. The second argument (as noted by experimentation)
is passed as the second argument of the php script (the `$use_me` variable). If
we pass something like `exec`, a php function like C's `system()`, the php script
will execute the argument passed to it. The final exploit is `[email {$use_me(getflag)}]`.
