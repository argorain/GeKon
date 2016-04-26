#!/bin/bash

function usage {
    echo "USAGE: $0 <directory with *.log files>"
}

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    usage
    exit
fi

FILES=`ls "$1" | grep ".log"`

echo "Directory: $1"
echo "Files used:"
for i in $FILES; do
    echo $i;
done

# Print all files | filter only test and time lines | .. | remove every other newline
TIMES=`cat "$1"*.log | grep --regex="\(Test\|Time\)" | paste -d ' ' - -`

# for ((i = 0; i < ${#TIMES[@]}; i++))
# do
#     echo "${TIMES[$i]}"
# done

function grep_method {
    for ((i = 0; i < ${#TIMES[@]}; i++))
    do
        echo "${TIMES[$i]}" | grep $1
    done
}


function print_just_numbers {
    FIRST_ARG=$1
    for ((i = 0; i < ${#FIRST_ARG[@]}; i++))
    do
        echo "${FIRST_ARG[$i]}" | tr -d -c "\([:digit:]\|\n\)"
    done
}


# print_just_numbers "$SSIM_TIMES" | paste -sd+ | bc

function return_mean {
    FIRST_ARG=$1
    SUM=$(print_just_numbers "$1" | paste -sd+ | bc)
    # echo "array sum: $SUM"
    LEN=$(print_just_numbers "$1" | sed 's/^.*$/1/' | paste -sd+ | bc)
    # echo "array len: $LEN"
    MEAN=$(echo $SUM/$LEN | bc)
    # echo "array mean: $MEAN"
    echo "$MEAN"
}

SSIM_TIMES=$(grep_method "ssim")
return_mean "$SSIM_TIMES"

MSE_TIMES=$(grep_method "mse")
return_mean "$MSE_TIMES"


#SSIM_MEAN=$(return_mean $SSIM_TIMES)
#echo "ssim mean: $SSIM_MEAN"
