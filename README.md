# p3a-format

This repository documents the "**P3A**" file format we use in newer PH3 game ports.  
P3A is a basic archive file with the following properties:

* Supports uncompressed, [lz4](https://github.com/lz4/lz4) and [zstd](https://github.com/facebook/zstd) files, with a per-file decision
* Supports zstd *[dictionaries](https://facebook.github.io/zstd/#small-data)* useful for large numbers of small, similar files (e.g. shaders) 
* Files are accessible and can be extracted indivdually
* The compression tool can include arbitrary padding to support smaller updates on digital platforms (at a small space efficiency hit)
* The file access library (not included here) supports overlay replacements e.g. for modding 

The `p3atool.exe` application is shipped with every game that uses this file format, and can extract, compress, inspect and verify p3a files.

However, since we currently have not released the source for that application (it is part of our larger library of tools), we offer a header specifying the file format for **p3a** in this repo -- for the off-case that you need to interact with these files in a way not supported by `p3atool`.
