## HOWTO BUILD ##
Actually, there are two versions of this project, one for xinetd and the other for independent use. 
- Individual:
    make
- XINETD:
    make xinetd
You should config it so that it can run via xinetd. For the XINETD version, an extra arg of root directory(absolute path) should be provided. And when it comes to Individual Version, two extra
    args -- port and root directory should be provided.

## FEATURES ##
    * Basic file server (directory access is also supported).
    * All common MIME types are supported.
    * Only GET is implemented (other HTTP headers are under construction :-))
    * CGI is supported (PHP, Python, Shell etc.)

## AUTHOR ##
    It's a project authored by [YuhangGe](https://github.com/YuhangGe) and [aegiryy](https://github.com/aegiryy). And it is released under the GPL v3 License.
