# Use a foreach to loop through all the files in a directory.
# This method allows us to easily track the file name so we can report 
# our findings by file.
$fileDirectory = "D:\git\fuzzy-time-series\parameters"

foreach($file in Get-ChildItem $fileDirectory)
{
    if ($file.Name.StartsWith("parameters_ohlc_bitfinex_btcusd")) {
		$filePath = $fileDirectory + "\" + $file
		./cmake-build-debug/fts-financial.exe $filePath
	}
    
}