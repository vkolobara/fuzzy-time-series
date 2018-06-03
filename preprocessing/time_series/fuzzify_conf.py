conf = {
    # List of tuples containing var name, the domain step and the type of the variable
    "in_var_names": [
		#("Close", 0.01, "IN", False),
		#("pct_change", 0.01, "IN", False),
		#("Volume", 0.01, "IN", False),
		#("MA_7", 0.01, "IN"),
		#("BollingerB1_7", 0.01, "IN"),
		#("BollingerB2_7", 0.01, "IN"),
		#("SOk", 0.01, "IN", False),
		#("RSI_20", 0.01, "IN", False),
		#("SOd_7", 0.01, "IN"),
		#("MACD_15_26", 0.01, "IN"),
		#("MACDSign_15_26", 0.01, "IN"),
		#("MACDDiff_15_26", 0.01, "IN")		
                ('Value', 1, 'IN', False)
                #('sepal_length', 0.01, 'IN', False),
                #('sepal_width', 0.01, 'IN', False),
                #('petal_length', 0.01, 'IN', False),
                #('petal_width', 0.01, 'IN', False)
	],
    #"out_var_name": ("pred", 1, "OUT", False),
    "in_path": "../../data/train/pigs.csv",
    "out_path": "../../conf/pigs.conf",
    "in_terms": 7,
    "out_terms": 1,
}
