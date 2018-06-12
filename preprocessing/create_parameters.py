import xml.etree.ElementTree as ET
import sys, os
import pandas as pd

tree = ET.parse('parameters.txt')
root = tree.getroot()

file_path = sys.argv[1]
set_name = os.path.splitext(os.path.basename(file_path))[0]

balance = pd.read_csv(file_path, index_col=False)['pred'].mean()*10

root[2][0].text = 'data/train/{}.csv'.format(set_name)
root[2][1].text = 'data/test/{}.csv'.format(set_name)
root[2][2].text = str(balance)
root[2][3].text = 'log/{}.csv'.format(set_name)

root[2][4].text = 'conf/{}.conf'.format(set_name)

for num_rules in [5, 10, 20]:
    for pop_size in [30, 100, 250]:
        for mut_prob in [0.2, 0.5, 0.9]:
            root[2][5].text = str(num_rules)
            root[2][6].text = str(pop_size)
            root[2][7].text = str(mut_prob)
            tree.write('../parameters/parameters_{}_{}_{}.txt'.format(set_name, pop_size, mut_prob))