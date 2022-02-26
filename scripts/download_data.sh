#!/bin/bash

# Author: Ahmed Soliman
# Name: download_data.sh
# Description:
#   Fetch ergometric tests' timeseries datasets from Figshare


# Create `data` directory and cd into it
parent_dir=$(cd -P -- "$(dirname -- "$0")"/.. && pwd -P)
cd "$parent_dir" || exit
mkdir -p "data"
data_dir="$parent_dir"/data
cd "$data_dir" || exit
echo switching to "$data_dir"

# Preparing dataset files
function prepare() {
file=$1
url=$2
if [ ! -f "$file".csv ]
then
  if [ ! -f "$file".zip ]
  then
    echo downloading "$file".zip ...
    wget "$url" -O "$file".zip -nv --show-progress
  fi
  unzip "$file".zip
  if [ -f "$file".csv ]
  then
    rm "$file".zip
    echo "$file".csv .... OK
  fi
fi
}

prepare sim_pat_1600 https://ndownloader.figshare.com/files/34149021
prepare sim_ergo_1600 https://figshare.com/ndownloader/files/34149018
