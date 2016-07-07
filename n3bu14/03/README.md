# [Level03](https://exploit-exercises.com/nebula/level03/)
The vulnerability is that there is a crontab that calls a script
(`/home/flag03/writable.sh`) that executes  bash scripts in the
`/home/flag03/writable.d` folder, which in turn is writable by everyone. The
exploit consists in creating a script in that folder and wait for the crontab to
trigger the execution.
