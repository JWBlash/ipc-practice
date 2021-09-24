### Learnings

#### Closing pipe ends

Both the parent and the child have control of both ends of the pipe, from what I can tell. This means
it's super duper important to close the unused portion if we're doing unidirectional ipc.

#### Exit Bug

During the creation of this, I had been calling the nested() function from the child process of the first process.

Nested() would cause another fork -- the child would write again to stdout, and the parent process would
wait for the child to return. However, the parent process would then exit as opposed to returning.

Because of this, the OG child process would exit as well, since it was amidst the nested() routine. The parent
process would end as expected though.

Changing nested()'s parent process from exit -> return 0 made things much happier! The program actually does
what I want it to.

So, I suppose this isn't specifically about pipes, but moreso how processes exit. However it reinforced the
concept that a process forking BECOMES the parent process -- which is really important when nesting forks.

#### Makefile

The canonical approach for removing files on cleanup and not returning an error if they don't exist is just adding
the `-f` flag to remove.

**Reference:**
https://stackoverflow.com/questions/33608650/checking-if-file-exists-before-removing-it-in-makefile
