
# Shagohod


Shagohod is a free open source tool designed to be used with the game Metal Gear Solid 3. It allows the user to extract Slot and Stage archive files packaged with the game

This project uses [Zlib](https://github.com/madler/zlib) by [Mark Adler](https://github.com/madler)

### To Do
 - Add multithreaded extract
 - Add error handling
 - Create GUI variant
 - Clean up the code

##  Usage

Currently only a CLI version of the application exists. The program is primitive and just takes the filename as the only required argument. An optional output directory can also be added. It is also possible to just drag the file you wish to extract on the executable. A dictionary file is also included in order to resolve a lot of the hashes, the result is incomplete.

If you wish to use the dictionary to resolve filenames place it alonside Shagohod.exe, please note this program is designed to work with the PS2 version of MGS3.

```
Shagohod.exe "path\to\STAGE.DAT"
```
The above instruction will extract all files from STAGE.DAT to the current directory.

```
Shagohod.exe "path\to\SLOT.DAT" "path\to\output"
```
The same can be used for Slot files. An optional output path can be added at the end, if it is not included then it will extract to the directory of the file being extracted.

## License
[MIT](LICENSE.md)
This project falls under the MIT license.