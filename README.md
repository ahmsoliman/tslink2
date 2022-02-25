# TSLink2 

A fast timeseries record linkage algorithm. This program efficiently links patients to their ergometric tests based on timeseries data. 

## Data preparation

A simulated dataset accompanies this repository. The dataset could be downloaded from [figshare](https://doi.org/10.6084/m9.figshare.19224786.v1). This dataset constitutes two files (after decompression), namely: **sim_ergo_1600.csv** and **sim_pat_1600.csv**.

 - **sim_ergo_1600.csv** contains heart rate timeseries data for 1600 patients' ergometric tests. For each patient, 20 different ergometric tests were simulated. Each row in this file constitutes three field values: Ergo_ID, Heart Rate (BPM), and timestamp.

 - **sim_pat_1600.csv** contains only four sample readings from each of the patient's 20 ergometric tests. Each row contains three values: patient_ID, Heart Rate, and timestamp.

## Objective

The goal is to link patients (identified by their patient_ID in the pat.csv file) to their corresponding ergometric tests (identified by their Ergo_ID in the ergo.csv file). This is done solely on matching the timestamp-value pairs from both files.


## Performance evaluation

Data is simulated such that correctly linked/matched identifiers follow the following formula:

( |Ergo_ID - patient_ID| ) mod 10000 == 0

The above formula is useful in evaluating the linkage algorithm performance.

## How to build

```shell
make tslink2
```

## How to run 

```shell
build/tslink2 data/sim_pat_1600.csv data/sim_ergo_1600.csv out/link_results.csv
```

The results are logged by appending a line at the end of the *link_results.csv* file.
