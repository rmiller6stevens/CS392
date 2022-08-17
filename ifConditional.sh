CURR_PATH=$(pwd)
if [[ $CURR_PATH = "/home/robert/CS 392" ]]; then
echo "Yay!"
else 
echo "Nay!"
fi


if [[ $1>$2 ]]; then
echo $1
else 
echo $2
fi
#$echo $(( $1+$2 ))