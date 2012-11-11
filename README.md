## HOWTO BUILD ##
Actually, there are two versions of this project, one for xinetd and the other for independent use. 
- standalone:  
    _make_
- xinetd:  
    _make xinetd_

You should config it so that it can run via xinetd. For the xinetd version, an extra arg of root directory(absolute path) should be provided. And when it comes to standalone version, two extra args -- port and root directory should be provided.

## FEATURES ##
* Basic file server (directory access is also supported).
* All common MIME types are supported.
* Only GET is implemented (other HTTP headers are under construction :-))
* CGI is supported (PHP, Python, Shell etc.)

## AUTHOR ##
It's a project authored by [YuhangGe](https://github.com/YuhangGe) and [aegiryy](https://github.com/aegiryy). And it is released under the GPL v3 License.
