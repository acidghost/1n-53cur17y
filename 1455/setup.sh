#!/usr/bin/env bash

lsts='11575'

test -e ${lsts} || mkdir ${lsts}

if [ ! -e ${lsts}/merged.lst ]; then
  wget "https://archive.org/download/mobywordlists03201gut/COMMON.TXT" -O ${lsts}/common.txt
  wget "https://archive.org/download/mobywordlists03201gut/CROSSWD.TXT" -O ${lsts}/crosswd.txt
  wget "https://archive.org/download/mobywordlists03201gut/CRSWD-D.TXT" -O ${lsts}/crswd-d.txt
  wget "https://archive.org/download/mobywordlists03201gut/FICTION.TXT" -O ${lsts}/fiction.txt
  wget "https://archive.org/download/mobywordlists03201gut/NAMES.TXT" -O ${lsts}/names.txt
  wget "https://duo.com/top250gawker.txt" -O ${lsts}/top250gawker.txt
  awk -F' ' '{ print $2 }' ${lsts}/top250gawker.txt > ${lsts}/top250gawker.txt
  cat ${lsts}/*.txt | LC_ALL=C uniq > ${lsts}/merged.lst
fi

if [ ! -e ${lsts}/10000.txt ]; then
  wget "https://github.com/danielmiessler/SecLists/raw/master/Passwords/10_million_password_list_top_10000.txt" -O ${lsts}/10000.txt
fi
