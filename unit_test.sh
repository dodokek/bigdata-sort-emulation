#!/bin/bash

# $1 - array length
# $2 - RAM limit
# $3 - amount of tests

for cnt in $(seq 1 $3)
do
    # Generate a random array of integers (range: 0-9999)
    array=()
    for ((i=0; i<($RANDOM % 1000); i++)); do
        array+=($RANDOM)
    done

    # Print the generated array to a file

    rm data.txt
    echo "${array[@]}" > data.txt

    # Sort the array using sort command in Bash
    sorted_bash=($(printf "%s\n" "${array[@]}" | sort -n))

    # Sort the array using a given executable (assuming it's named 'custom_sort' in the same directory)
    ./main data.txt output.txt $2 

    # Read and store the sorted result from the executable
    sorted_custom=($(<output.txt))

    # Compare the sorted results
    if [ "${sorted_bash[*]}" = "${sorted_custom[*]}" ]; then
        echo "[+] Unit test $cnt passed."
    else
        echo "[-] Unit test $cnt failed."
        echo ${sorted_bash[@]} > output_correct.txt
    fi

done