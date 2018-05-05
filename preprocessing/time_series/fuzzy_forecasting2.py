#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  4 11:44:23 2018

@author: vkolobara
"""

from fuzzify_dataset import LambdaTerm, TrapezoidalTerm, LanguageVariable, generate_lang_term, generate_trapeizodal_lang_var, fuzzify_vars
from fuzzify_conf import conf
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

lang_var = fuzzify_vars()[0]

df = pd.read_csv(conf['in_path'])

df['term'] = df['Value'].apply(lambda x: np.argmax([term.membership(x) for term in lang_var.language_terms]))

terms = lang_var.language_terms

[term.name for term in terms]

vals = df['term'].values
lst = []

for i in range(0, len(vals)*3//4):
    lst.append((vals[i], vals[i+1], vals[i+2]))

ws = {}
for l in lst:
    if l in ws:
        ws[l] += 1
    else:
        ws[l] = 1

st = set(lst)

print(len(st))

dct = {}

for (x, y, z) in st:
    if x in dct:
        dct[(x,y)].append(z)
    else:
        dct[(x,y)] = [z]
        
values = df['Value'].values

mse = 0
mape = 0

print()
preds = []

for i in range(1, len(values)-1):
    pred = 0
    
    sum_ws = 0
    
    (x,y) = (vals[i-1], vals[i])
    
    if (x,y) in dct:
        for j in dct[(x,y)]:
            sum_ws += ws[(x, y, j)]
            pred += ws[(x, y, j)] * (terms[j].b + terms[j].c) / 2 
        pred /= sum_ws
    else:
        pred = (terms[vals[i]].b + terms[vals[i]].c) / 2
    
    preds.append(pred)
    print(pred, values[i+1])
    mse += pow(pred-values[i+1], 2)
    mape += abs((pred - values[i+1]) / (values[i+1]))
    
print()
    
print("MAPE:" + str(100*mape/(len(values)-1)))
print("MSE:" + str(mse / (len(values) - 1) ))
print(dct)
print(len(dct))

plt.plot(preds)
plt.plot(values[2:])