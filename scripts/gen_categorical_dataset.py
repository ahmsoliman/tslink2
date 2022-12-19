#!/usr/bin/env python
# coding: utf-8

import pandas as pd

def int2char(x):
    i2c = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n',
           'o','p','q','r','s','t','u','v','w','x','y','z']
    return i2c[x]

pat = pd.read_csv(r'../data/sim_pat_1600.csv')
pat['HR'] = pat['HR'].apply(lambda x : int2char(int(x/10)))
# pat['HR'] = pat['HR'].apply(lambda x : int(x/10))
pat.to_csv(r'../data/sim_pat_1600_cat.csv', index=False)

ergo = pd.read_csv(r'../data/sim_ergo_1600.csv')
ergo['HR'] = ergo['HR'].apply(lambda x : int2char(int(x/10)))
# ergo['HR'] = ergo['HR'].apply(lambda x : int(x/10))
ergo.to_csv(r'../data/sim_ergo_1600_cat.csv', index=False)

