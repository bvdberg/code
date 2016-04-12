from http://www.linuxjournal.com/node/1005818

file.tar created with:

```bash
cd files
tar cf - . > ../payload/files.tar
```

Creating whole thing:

```bash
./build
```

Extract again:

```bash
./selfextract.bsx
```

