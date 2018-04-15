import pandas as pd
from fuzzify_conf import conf
from abc import ABC, abstractmethod
import json

class JSONable(ABC):
    @abstractmethod
    def to_json(self):
        pass

class Domain(JSONable):
    def __init__(self, lb, step, ub):
        self.lb = lb
        self.step = step
        self.ub = ub
        
    def to_json(self):
        json_dict = dict()
        json_dict['min'] = self.lb
        json_dict['step'] = self.step
        json_dict['max'] = self.ub
        return json_dict
    
    def __str__(self):
        return ",".join([str(self.lb), str(self.step), str(self.ub)])

class LanguageVariable(JSONable):
    def __init__(self, name, language_terms, domain, var_type='IN'):
        self.name = name
        self.var_type = var_type
        self.domain = domain
        self.language_terms = list(language_terms)
        
    def to_json(self):
        json_dict = dict()
        json_dict['name'] = self.name
        json_dict['type'] = self.var_type
        json_dict['lang_terms'] = [term.to_json() for term in self.language_terms]
        json_dict['domain'] = self.domain.to_json()
        return json_dict
    
    def __str__(self):
        ret = "{0}\t{1}\t{2}".format(self.var_type, self.name, self.domain)
        lst = [ret] + [str(x) for x in self.language_terms]
        return "\n".join(lst)
    
class TrapezoidalTerm(JSONable):
    def __init__(self, name, a, b, c, d):
        self.name = name
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        
    def to_json(self):
        json_dict = dict()
        json_dict['name'] = self.name
        json_dict['shape'] = 'PI'
        json_dict['alpha'] = self.a
        json_dict['beta'] = self.b
        json_dict['gamma'] = self.c
        json_dict['delta'] = self.d
        return json_dict
        
    def __str__(self):
        return "{4}\tPI\t{0},{1},{2},{3}".format(self.a,self.b,self.c,self.d,self.name)

def generate_trapeizodal_lang_var(df, name, domain_step, var_type, n=7):    

    df_col = df[name]
    df_std = df_col.std()
    df_min = df_col.min()
    df_max = df_col.max()
    
    u_min = int(df_min - df_std)
    u_max = int(df_max + df_std)
    
    u_range = u_max - u_min
    
    u_S = int(u_range / (2*n + 1))
    
    domain = Domain(u_min, domain_step, u_max)
    terms = [generate_lang_term("L{0}".format(i), u_min, u_max, u_S, i) for i in range(n)]
    
    return LanguageVariable(name, terms, domain, var_type)
        
def generate_lang_term(name, u_min, u_max, u_S, i):
    a = u_min + 2 * i * u_S
    b = a + u_S
    c = b + u_S
    d = c + u_S
    return TrapezoidalTerm(name, a, b, c, d)

def main():
    df = pd.read_csv(conf["in_path"])

    lang_vars = [generate_trapeizodal_lang_var(df, name, step, var_type) for (name, step, var_type) in conf["var_names"]]

    out_string = "\n\n".join([str(var) for var in lang_vars])

    with open(conf["out_path"], 'w') as f:
        f.write(out_string)
        
    with open(conf["out_path"] + ".json", 'w') as f:
        json_dict = dict()
        json_dict['lang_vars'] = [var.to_json() for var in lang_vars]
        json.dump(json_dict, f)

if __name__ == '__main__':
    main()