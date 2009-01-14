This tool can be used to parse aspects of an input file:
Example file:
    foo
    %%begin one
    bar
    %%end one
    faa
'./generator file' will result in:
    foo
    faa
'./generator file one' will result in:
    foo
    bar
    faa
Aspects can be nested and may occur multiple times within a file


TODO:
- list aspects in file
- check file (start/end tag)
- parse file
- variable escape codes 

