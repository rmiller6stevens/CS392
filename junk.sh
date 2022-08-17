#!/bin/bash
# interface.sh
#Robert Miller
#I pledge my honor that I have abided by the Stevens Honor System
# Credit: https://www.computerhope.com/unix/bash/getopts.htm

readonly trash=~/.junk
mkdir -p $trash

usage() {                                 # Function: Print a help message.
cat << END
Usage: $0 [-hlp] [list of files]
  -h: Display help.
  -l: List junked files.
  -p: Purge all files.
  [list of files] with no other arguments to junk those files.
END
}

exit_abnormal() {                         # Function: Exit with error.
  usage
  exit 1
}

exit_normal() {
  exit 0
}

doH=0
doL=0
doP=0
tooMany=0
while getopts ":hlp" options; do         # Loop: Get the next option;
                                          # use silent error checking;  
  if [[ $# > 1 ]]; then #because the intake of getops is flags, 
  #I know that this number should be 1 or there are more flags

  tooMany=1
  fi

  case "${options}" in                    #
    h)                                    # If the option is h,
    doH=1
      ;;
    l)                                    # If the option is l,
    doL=1
      ;;
    p)                                    # IF the option is p,
    doP=1
      ;;
    *)                                    # If unknown (any other) option:
        echo "Error: Unknown option '-${OPTARG}'."
        exit_abnormal                       # Exit abnormally.
      ;;
  esac
done

let total=$doH+$doL+$doP
if [[ $total > 1 || $tooMany = 1 ]]; then
echo "Error: Too many options enabled."
exit_abnormal
fi

if [[ $doH = 1 ]]; then
usage
exit_normal
fi

if [[ $doL = 1 ]]; then 
ls -lAF $trash*
exit_normal
fi

if [[ $doP = 1 ]]; then
shopt -s dotglob
rm -rf $trash/*
exit_normal
fi

#now $# counts args/filenames, if none its an error
if [[ $# = 0 ]]; then
exit_abnormal
fi
  
#loop here
for name in $@
do
if [[ -f $name ]] || [[ -d $name ]]; then
mv $name $trash
else 
echo "Warning: '$name' not found."
fi
done

#for loop time

exit 0                                    # Exit normally.