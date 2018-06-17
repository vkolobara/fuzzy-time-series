import pandas as pd
import sys,os

file_path = sys.argv[1]
set_name = os.path.splitext(os.path.basename(file_path))[0]

df = pd.read_csv(file_path, index_col=False)
df = df.drop('pred', axis=1)

in_var_names = [(x, 0.01, "IN", False) for x in list(df)]
in_path = file_path
out_path = "../../conf/{}.conf".format(set_name)
in_terms = 7
out_terms = 1

conf = dict()

conf['in_var_names'] = in_var_names
conf['in_path'] = file_path
conf['out_path'] = out_path
conf['in_terms'] = in_terms
conf['out_terms'] = out_terms

with open("fuzzify_conf.py", 'w') as f:
    f.write("conf={}".format(str(conf)))

os.system(r"python fuzzify_dataset.py")
