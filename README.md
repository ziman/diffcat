# diffcat

SD cards (especially the cheaper ones) have frequently an okay read performance
but are slow to write to.  If you're restoring an image of a Raspberry Pi, for
example, chances are that most blocks will be untouched since the backup.

`diffcat` first *reads* blocks from the output file to determine if they need
overwriting. If they don't, it moves on to the next block.

## Usage

```bash
$ make
$ pixz -d image.xz | diffcat /dev/mmcblk0
```

## Debugging

* you can `#define DEBUG` to get a (very verbose) description of the process
* you can re-`#define BLOCKSIZE` to use non-4K blocks
