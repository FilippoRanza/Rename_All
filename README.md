# Rename_All
automatic file rename program:

this program can rename all file inside a directory, optionally in sub-directories.

Every file will be renamed following this pattern:
  BASE-SEQ-EXT
  
Where:
  BASE is the common header for every renamed file.
  SEQ  is the unique serial number for every renamed file
  EXT file extension.
user can specify,as arguments,BASE , SEQ type and EXT.

Program Options:

    -d directory : specify working directory, by default "."
    -b name : set BASE, by default "Rename_"
    -e [.]ext  : set EXT only file with 'ext' as extension will be renamed, by default none so  every file is renamed.
    -h : generate SEQ in Hexadecimal ,by default Integer
    -c : generate SEQ in Charater,by default Integer
    -v : enable Verbose mode
    -r :enable recursion, so program will seach in sub directories.
    
Examples:
(binery name : rall)

rall -b Wien_ -e jpg -r

  will rename every .jpg file in corrent and all sub diretories to "Wien_#.jpg"
  where # is a number from 0 to number of jpg files.
  
rall -b School_ -e pdf -c

   will rename every .jpg file in corrent  diretories to "School_#.pdf"
  where # is a char serial from A to number of jpg files.
  
rall -b Music_ -d ~/Music -e mp3 -r

  will rename every .mp3 file in ~/Music and all its sub diretories to "Music_#.mp3"
  where # is a number from 0 to number of mp3 files.
  
rall -b Test_ -d ~/tests -r

  will rename every  file in ~/tests and all its sub diretories to "Test_#"
  where # is a number from 0 to number of files.
  note that original exansion is removed.
 
 
 
Installation:

  Download 'main.c' file, compile it and move the binary into a proper directory.
  That's all.
  
  
  
 ATTENTION:
 
    this program DOES NOT run in interacive mode, so once execute it will rename every corresponding file.
    if this program is run without care it can take serious damage to your system.
    
  
