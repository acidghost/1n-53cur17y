# [Level07](https://exploit-exercises.com/nebula/level07/)
The script's `Host` parameter is vulnerable to code injection. To exploit this
it is not sufficient to call the script from command line as it will be executed
as `level07`. We need to find a service that is running as `flag07` and the file
`/home/flag07/thttpd.conf` hints that `thttpd` is running (we can verify with
`ps aux | grep -i thttpd`). In the configuration file we will find the port number
so that we can exploit the vulnerability with a `GET` or `POST` request.
