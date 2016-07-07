# [Level00](https://exploit-exercises.com/nebula/level00/)
First find the executable files owned by user `flag00` with the command
`find / -user flag00 -executable`; you'll notice the file `/rofs/bin/.../flag00`.
Execute it and it will open a shell  owned by user `flag00`, run `getflag` and
you're done.
