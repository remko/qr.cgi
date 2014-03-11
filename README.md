# [qr.cgi: A CGI QR Code Generator](http://el-tramo.be/qr.cgi)

## About

A CGI binary for generating QR Codes. Put this on your webserver, point your
browser to it, drag the bookmarklet into your bookmarks bar, and create QR codes 
for the page you're currently browsing by clicking the bookmarklet (handy for 
transferring links to your smartphone to continue browsing a page on the go).

Because it is written in C++, it is fast and very light on resources.


## Dependencies

- A C++11-compatible compiler
- LibQREncode: `libqrencode` (Homebrew), `libqrencode-dev` (Debian Wheezy)
- LibPNG: `libpng` (Homebrew), `libpng12-dev` (Debian Wheezy)


## Building

To build, run
    
    scons

You can pass optional build flags via the command line, or put them in `config.py`:

    scons optimize=1 debug=0

I only tested building on Debian Wheezy and Mac OS X (Mavericks). If
you have a different setup, edit the `SConscript` file to reflect these.


## Installation

After building, put `qr.cgi` somewhere on your webserver, and make sure it is executable.

For Apache, putting the following in a .htaccess dir with the `qr.cgi` script should work
(assuming the right permissions to override):

        Options ExecCGI
        AddHandler cgi-script .cgi
        DirectoryIndex qr.cgi

On nginx with fcgiwrap, the following should work:

        location /path/to/qrcgi {
            index qr.cgi;
            location = /path/to/qrcgi {
                gzip off;
                fastcgi_pass unix:/var/run/fcgiwrap.socket;
                include fastcgi_params;
            }
        }


## Usage

Point your browser to `http://your.server.com/path/to/qrcgi/`.
This will show a page with a link to a bookmarklet. Drag this bookmarklet into your bookmarks,
and click it when on a page you want to generate a QR code for.


## License

Licensed under the GNU General Public License.
