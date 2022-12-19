# TSLink2 

A fast timeseries record linkage algorithm. This program efficiently links patients to their ergometric tests based on timeseries data. 

## Requirements

Install the Boost Filesystem and System libraries

```shell
sudo apt install libboost-filesystem-dev
```

## Data preparation

A simulated dataset accompanies this repository.
The dataset could be downloaded from [figshare](https://doi.org/10.6084/m9.figshare.19224786.v1) or
fetched automatically using the supplied script as follows:

```shell
scripts/download_data.sh
```

Optionally, to prepare data for linking integer values, use the following command to drop the 
fractions from all heart rate values.

```shell
gawk 'BEGIN{FS=","} {if(NR==1) {print($0)} else {printf("%d,%d,%d\n",$1,$2,$3)}} END{}' data/sim_ergo_1600.csv > data/sim_ergo_1600_int.csv
gawk 'BEGIN{FS=","} {if(NR==1) {print($0)} else {printf("%d,%d,%d\n",$1,$2,$3)}} END{}' data/sim_pat_1600.csv > data/sim_pat_1600_int.csv
```

Optionally, to prepare data for linking categorical values, use the following command to convert
heart rate values into discrete levels. 

```shell
scripts/gen_categorical_dataset.py
```
This script loads the simulation dataset files, convert the values therein into series of characters,
and save the results as ``../data/sim_ergo_1600_cat.csv`` and ``../data/sim_pat_1600_cat.csv``.
The script converts the heart rate values as follows. First the values are divided by 10,
then converted into integer, and finally the integer values ``[0...25]``
are mapped into lowercase characters ``[ab...z]``
For example, the heart rate values ``[94.3, 100.5, 130.1, 110.5]`` would be mapped into 
characters ``[j,k,n,l]`` respectively.

## Data set description

This dataset constitutes two files (after decompression), namely: **sim_ergo_1600.csv** and **sim_pat_1600.csv**.

 - **sim_ergo_1600.csv** contains heart rate timeseries data for 1600 patients' ergometric tests. For each patient, 20 different ergometric tests were simulated. Each row in this file constitutes three field values: Ergo_ID, Heart Rate (BPM), and timestamp.

 - **sim_pat_1600.csv** contains only four sample readings from each of the patient's 20 ergometric tests. Each row contains three values: patient_ID, Heart Rate, and timestamp.

## Objective

The goal is to link patients (identified by their patient_ID in the pat.csv file) to their corresponding ergometric tests (identified by their Ergo_ID in the ergo.csv file). This is done solely on matching the timestamp-value pairs from both files.


## Performance evaluation

Data is simulated such that correctly linked/matched identifiers follow the following formula:

( |Ergo_ID - patient_ID| ) mod 10000 == 0

The above formula is useful in evaluating the linkage algorithm performance.

## How to build

To build just one target, use either
```shell
make tslink1
```
or
```shell
make tslink2
```
To build both implementations, use
```shell
make all
```

## How to use

Assuming that the datasets (the two csv files) have been downloaded and uncompressed into directory `data`.
Run `tslink1` or `tslink2` on these simulated datasets as follows: 
```shell
bin/tslink1 data/sim_pat_1600.csv data/sim_ergo_1600.csv out/link_results.csv
bin/tslink2 data/sim_pat_1600.csv data/sim_ergo_1600.csv out/link_results.csv
```
Both executables take three arguments in the following order:
`<patients dataset>`, `<ergometric dataset>`, and `<output path>` 
Each time an experiment is run,
the results are appended to the file designated by the output path (third argument).
In the example above, *out/link_results.csv* is the output path.
Any path could be used for reporting the results. 
If the file does not exist, it is automatically created and initialized with the header line.

## Results

Each row in the output csv file constitutes the following data:
```
algo,n_patients,n_ergos,t,t1,t2,t3,t4,t5,TP,FP
```
Where `algo` is the algorithm signature which is either *TSLink1_C++* or *TSLink2_C++*,
`n_patients` is the number of unique patients in the dataset,
`n_ergos` is the number of unique ergo tests,
`t` is the total run time in seconds,
`t1` is the date-to-epoch conversion time (N/A) (always = `-`),
`t2` is the time taken to form the first sequence `S1`,
`t3` is the time taken to form the `S2` sequence,
`t4` is the time taken to sort `S1`, `S2`, merge them into `S`, and sort the merged sequence `S`,
`t5` is the time taken to scan the sequence `S` and perform *pat-to-ergo associations* followed by linkage.
`TP` is the number of true positive links, and
`FP` is the number of false positive links.

