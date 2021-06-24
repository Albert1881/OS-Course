#!/bin/bash

unprint_arr=
unprint_down_index=
unprint_up_index=
dir_count=
file_count=
PrintInfo(){
    
    path=$(cd "$(dirname "$1")";pwd)
    dir=$(basename "$1")
    
    echo "[$dir]" >> "$2"
    # echo $SHELL_FOLDER
    SALVEIFS=$IFS
    IFS=$(echo -en "\n\b")
    set -f 

    if [ "$path" == "/" ]
    then
        path=""
    fi
    for file in `ls $1/`
    do  
        # echo $file
        if [ -e "$1/$file" ]
        then
            echo "$path/$dir/${file}" >> "$2"
            if [ -f "$1/$file" ]
            then
                # echo 'f'
                file_count=$(($file_count+1))
            elif [ -d "$1/$file" ]
            then
                # echo "$1/$file"
                dir_count=$(($dir_count+1))
                unprint_arr[$unprint_up_index]="$1/$file"
                unprint_up_index=$(($unprint_up_index+1))
            fi
        fi
    done
    echo >>"$2"
    set +f 
    IFS=$SAVEIFS
}
PrintBase(){
    if [ -e "$1" ]
    then
        path=$(cd "$1";pwd)
        dir=$(basename "$path")
        
        echo "[$dir]" >> "$2"
        # echo $SHELL_FOLDER
        
        SALVEIFS=$IFS
        IFS=$(echo -en "\n\b")
        set -f 

        if [ "$path" == "/" ]
        then
            path=""
        fi
        for file in `ls $1/`
        do  
            if [ -e "$1/$file" ]
            then
                echo "$path/${file}" >> "$2"
                if [ -f "$1/$file" ]
                then
                    # echo 'f'
                    file_count=$(($file_count+1))
                elif [ -d "$1/$file" ]
                then
                    # echo "$1/$file"
                    dir_count=$(($dir_count+1))
                    unprint_arr[$unprint_up_index]="$1/$file"
                    unprint_up_index=$(($unprint_up_index+1))
                fi
            fi
        done
        echo >>"$2"
        set +f 
        IFS=$SAVEIFS
    fi
}

if [ -f "$2" ]
then
    rm "$2"
fi
touch "$2"

dir_count=0
file_count=0
unprint_arr[1]="$1"
unprint_down_index=1
unprint_up_index=2

PrintBase "${unprint_arr[$unprint_down_index]}" "$2"
unprint_down_index=$(($unprint_down_index+1))

while [ $unprint_down_index -lt $unprint_up_index ]
do
    PrintInfo "${unprint_arr[$unprint_down_index]}" "$2"
    unprint_down_index=$(($unprint_down_index+1))
done
echo [Directories Count]:$dir_count >> "$2"
echo [Files Count]:$file_count >> "$2"