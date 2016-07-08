# [Level04](https://exploit-exercises.com/nebula/level04/)
The vulnerability resides in the fact that the check consists only in checking
the filename passed as parameter. We can exploit this by creating a symlink
pointing to the `/home/flag04/token` with a name different from `token`.
