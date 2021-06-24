#!/bin/bash

src=$1
des=$2
dcnt=0
fcnt=0
if [ -e "$2" ]
then
	rm "$2"
	touch "$2"
else
	touch "$2"
fi

array=
a=0
b=1
array[0]="$1"
SALVEIFS=$IFS
IFS=$(echo -en "\n\b")
while [ $a -lt $b ]
do
    dir=${array[$a]}
    echo $dir
    dir_name=$(basename "${array[$a]}")
    abseach=$(cd "$(dirname "${array[$a]}")";pwd)
	echo "[$dir_name]" >>"$2"
    a=$(($a+1))
	for each in `ls $dir`
	do 
		
		echo "$abseach/${dir_name}/${each}" >>"$2"
		if [ -d "${dir}/$each" ]
		then
            echo $each
			dcnt=$(($dcnt+1))
			
			array[b]="${dir}/${each}"
            b=$(($b+1))
		elif [ -f "${dir}/$each" ]
		then
			fcnt=$(($fcnt+1))
		fi
	done
    echo >>"$2"
done
IFS=$SAVEIFS
echo [Directories Count]:$dcnt >> "$2"
echo [Files Count]:$fcnt >> "$2"
