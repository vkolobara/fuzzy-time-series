# Use a foreach to loop through all the files in a directory.
# This method allows us to easily track the file name so we can report 
# our findings by file.
$fileDirectory = "D:\git\fuzzy-time-series\data\train"

foreach($file in Get-ChildItem $fileDirectory)
{
    if ($file.Name.StartsWith("ohlc")) {
        $filePath = $fileDirectory + "\" + $file
        C:\ProgramData\Anaconda3\python.exe create_parameters.py $filePath
    }
}