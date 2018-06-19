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
    lst.append((vals[i], vals[i+1]))

ws = {}
for l in lst:
    if l in ws:
        ws[l] += 1
    else:
        ws[l] = 1

st = set(lst)

print(len(st))

dct = {}

for (x, y) in st:
    if x in dct:
        dct[x].append(y)
    else:
        dct[x] = [y]
        
print(dct)

values = df['Value'].values

mse = 0
mape = 0

print()

preds = []

for i in range(0, len(values)-1):
    pred = 0
    
    sum_ws = 0
    if vals[i] in dct:
        for j in dct[vals[i]]:
            sum_ws += ws[(vals[i],j)]
            pred += ws[(vals[i],j)] * (terms[j].b + terms[j].c) / 2 
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
plt.plot(df['Value'])
plt.plot(preds)
plt.legend(['Stvarna vrijednost', 'Predvidena vrijednost'])
plt.xlabel('Vremenski korak')
plt.ylabel('Broj zaklanih svinja')
plt.savefig('fuzzy_pigs.pdf')

#with open('pigs_rule_1.txt', 'w') as f:
#    for k in dct:
#        f.write(str(k) + "->" + ",".join([str(x) for x in dct[k]]) + "\n")
