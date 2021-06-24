k="`ls t/`"
# echo "$k"
# echo "${k:0:1}"
# printf "%d" \'"${k:1:1}"
# t=`printf "%x" 10`
# echo $t
# if [[ ${k:1:1} == $'\n' ]]
# then
#     echo "q"
# fi
# echo ${k: 1: 2}
echo "$k"
echo ${k:1:2}
SALVEIFS=$IFS
array=(`echo "$k" | tr '\n' ' '` )  
for file in "${array[@]}"
do
    echo "$file"
    echo 1
done
