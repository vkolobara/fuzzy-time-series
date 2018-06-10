# Use a foreach to loop through all the files in a directory.
# This method allows us to easily track the file name so we can report 
# our findings by file.
$fileDirectory = "D:\crypto_data"

foreach($file in Get-ChildItem $fileDirectory)
{
    $filePath = $fileDirectory + "\" + $file
    C:\ProgramData\Anaconda3\python.exe create_dataset.py $filePath
}