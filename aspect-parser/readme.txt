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
- warning for nested tags of same type
- warning for incorrect types, even if !copyInput (keep tag tree + on/off variable)?
    -> give warning when: middle part selected but not used (optional?)
        disabled_tag
            enabled_tag
        disabled_tag

- negative tags?
