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

echo "SSIM"
SSIM_TIMES=$(grep_method "ssim")
return_mean "$SSIM_TIMES"

echo "mse"
mse_TIMES=$(grep_method "mse")
return_mean "$mse_TIMES"

echo "tournament"
tournament_TIMES=$(grep_method "tournament")
return_mean "$tournament_TIMES"


echo "rank"
rank_TIMES=$(grep_method "rank")
return_mean "$rank_TIMES"


echo "roulette"
roulette_TIMES=$(grep_method "roulette")
return_mean "$roulette_TIMES"

echo "simple"
simple_TIMES=$(grep_method "simple")
return_mean "$simple_TIMES"


echo "convex"
convex_TIMES=$(grep_method "convex")
return_mean "$convex_TIMES"


echo "blx"
blx_TIMES=$(grep_method "blx")
return_mean "$blx_TIMES"


echo "swap"
swap_TIMES=$(grep_method "swap")
return_mean "$swap_TIMES"


echo "dynamic"
dynamic_TIMES=$(grep_method "dynamic")
return_mean "$dynamic_TIMES"


#SSIM_MEAN=$(return_mean $SSIM_TIMES)
#echo "ssim mean: $SSIM_MEAN"
