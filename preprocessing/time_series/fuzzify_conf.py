conf = {
    # List of tuples containing var name, the domain step and the type of the variable
    "in_var_names": [
		("Close", 0.01, "IN"),
		#("Volume", 0.01, "IN"),
		#("MA_7", 0.01, "IN"),
		#("BollingerB1_7", 0.01, "IN"),
		#("BollingerB2_7", 0.01, "IN"),
		#("SOk", 0.01, "IN"),
		#("SOd_7", 0.01, "IN"),
		#("MACD_15_26", 0.01, "IN"),
		#("MACDSign_15_26", 0.01, "IN"),
		#("MACDDiff_15_26", 0.01, "IN")		
	],
    # "out_var_name": ("pred", 1, "OUT"),
    "in_path": "../../data/SP500_prep_price.csv",
    "out_path": "../../conf/SP500_price.conf",
    "in_terms": 17,
    "out_terms": 7,
}
