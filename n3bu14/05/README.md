# [Level05](https://exploit-exercises.com/nebula/level05/)
The vulnerability is in a backup folder in the flag05 home directory, which is
misconfigured to be readable and executable by others. It is the backup of the
`.ssh` folder (which is protected), so we can extract the archive in our home
directory and ssh into localhost as user flag05.
