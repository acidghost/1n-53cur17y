# [Level01](https://exploit-exercises.com/nebula/level01/)
The vulnerable program runs `/usr/bin/env echo and now what?`, so if we modify
the `PATH` variable the `env` program will look into the modified path. We can
exploit this by forging an executable named `echo` so that the vulnerable program
will run the malware. See details in the script in this folder.
