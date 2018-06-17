import xml.etree.ElementTree as ET
import sys, os
import pandas as pd

tree = ET.parse('parameters.txt')
root = tree.getroot()

file_path = sys.argv[1]
set_name = os.path.splitext(os.path.basename(file_path))[0]

time_val = int(set_name.split("_")[3])

balance = pd.read_csv(file_path, index_col=False)['pred'].mean()*10

root[2][3].text = 'data/train/{}.csv'.format(set_name)
root[2][4].text = 'data/test/{}.csv'.format(set_name)
root[2][5].text = str(balance)

root[2][7].text = 'conf/{}.conf'.format(set_name)

for threshold in [0.7, 0.8, 0.9]:
    for num_rules in [5, 10]:
        for pop_size in [30, 100, 250]:
            for mut_prob in [0.2, 0.5, 0.9]:
                root[2][9].text = str(threshold)
                root[2][8].text = str(num_rules)
                root[2][10].text = str(pop_size)
                root[2][2].text = 'stats/{}_{}_{}_{}_{}.csv'.format(set_name,
                                                                  num_rules,
                                                                  pop_size,
                                                                  mut_prob,
                                                                  threshold)
                root[2][6].text = 'log/{}_{}_{}_{}_{}.csv'.format(set_name,
                                                                  num_rules,
                                                                  pop_size,
                                                                  mut_prob,
                                                                  threshold)
                root[2][12].text = 'milestones/{}_{}_{}_{}_{}.txt'.format(set_name, num_rules,
                                                       pop_size, mut_prob,
                                                       threshold)
                root[2][11].text = str(mut_prob)
                tree.write('../parameters/parameters_{}_{}_{}_{}_{}.txt'.format(set_name,
                                                                                num_rules,
                                                                                pop_size,
                                                                                mut_prob,
                                                                               threshold))
