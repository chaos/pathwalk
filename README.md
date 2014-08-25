## pathwalk

Path search is a common UNIX programming idiom and file system use
case.  It is used in the following cases, to name a few:

* shell or execvp(3) search for executables by iterating over PATH elements
* ld-linux.so search for shared libraries by iterating over /etc/ld.so.conf
or LD_LIBRARY_PATH elements (although this is mitigated somewhat by ld.so.cache)
* python dynamic linking and loading as explored by the Pynamic Benchmark
* perl module load path

### Path Search Example

Take for example a shell script that contains a line "hostname".  With
the system default path set to:

```
/usr/lib64/qt-3.3/bin:/usr/global/tools/totalview/m/hype/dflt/bin:\
  /usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin
```

In order to find `hostname`, the shell must perform a system call such as
stat(2) or execve(2) that will trigger path resolution, as described in
path_resolution(7), for each possible path starting with
`/usr/lib64/qt-3.3/bin/hostname` and ending with `/bin/hostname`, where
it is found.

### Cache Effects

The dcache assists with this process.   For each directory searched
unsuccessfully for hostname, a negative dentry is instantiated in
the dcache for hostname in that directory.  For /bin, a positive dentry
is instantiated for hostname in that directory.  The next time any process
on the system does a path lookup for hostname, it will still need to
walk each directory in the PATH but the path resolution for hostname
will be short circuited by the existance of the dcache entries, provided
the entries continue to meet the underlying file systems criteria for
avoiding revalidation.  In other words, the underlying file system will
not have to contact its network server (NFS, Lustre) or access the block
layer (ext3, other local file systems) for every hostname path search.

However, It is worth noting that the dcache caches names not directory
blocks.  Therefore the dcache cannot ever satisfy a request for a file
name that has not been requested before.  So even though only a handful
of files exist in `/usr/lib64/qt-3.3/bin` every path search for a new
name begins by triggering a lookup in the file system backing 
`/usr/lib64/qt-3.3/bin` for the new name.  For local file systems this
is not too bad because the block layer caches disk blocks backing the
directory, and the underlying file system will still likely be able to
satisfy the lookup via the buffer cache (in local memory).  For network
file systems, this lookup triggers an RPC to the network file system server.

### PATH Search for Executables on Clusters

On a cluster where PATH components are on network file systems, this
becomes pathalogical!  When a new name is resolved, the latency is
the sum of the RTTs of an RPC to each server (requests are not pipelined).
Scalability is further limited by the number of parallel requests that
must be handled by network file system servers, when nodes are all walking
the same paths.

### pathwalk benchmark

pathwalk simulates the file system load created by path search.
First it creates a directory tree containing by default, 16 sub-directories,
containing 10000 empty files each:
```
$ pathwalk -c /tmp/abc
pathwalk: Created 160001 objects in 2.381s
```
Then it measures the time to search for 10000 files trying each of
the 16 sub-directories in turn, and finding the file in the last directory.
```
$ pathwalk -t /tmp/abc
pathwalk: Found 10000/10000 files in 16 directories in 0.161s
```
Finally it can clean up the directory.
```
$ ./pathwalk -r /tmp/abc
pathwalk: Removed 160001 objects in 1.469s
```

The number of directories, the number of files, and other parameters
can be set on the command line:
```
Usage: pathwalk root-path [-c|-r|-t] [OPTIONS]
   -l,--length N          set number of directories to search dflt: 16)
   -f,--files N           set number of files to search for (dflt: 10000)
   -c,--create            create files and directories
   -r,--remove            remove files and directories
   -t,--test              run test
   -C,--primecache        run one path search before starting timing
   -F,--failsearch        arrange for search to be unsuccessful
```
