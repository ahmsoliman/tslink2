# TSLink2 

This is a time-series record linkage algorithm. In this project the program links patients with their ergometric tests. 

## Description

Data is split into two csv files: `pat.csv` and `ergo.csv`. `pat.csv` contains three columns: **patient_ID**, **HR**, and **timestamp**. There are exactly four rows for each ergometric tests. The `ergo.csv` file contains all timeseries readings for the entire dataset of ergometric tests. The `ergo.csv` file constitutes three columns: `Ergo_ID`, `HR` (Heart Rate in BPM), and `timestamp`. The goal is to link patients in the `pat.csv` file to their ergometric tests in the `ergo.csv` file.

