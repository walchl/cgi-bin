# cgi-bin
CGI Demonstration

Lighttpd for Windows: http://lighttpd.dtech.hu/

lighttpd.conf:
  enable mod_cgi in server.modules
  disable all index-file.names
  cgi.assign = (".exe"=>"")
  
