##One more logs
###Compile
<pre>
gcc -m64 -shared -fPIC -o /var/mqm/exits64/Installation1/libsmsolog.so_r smsolog.c -I/opt/mqm/inc
</pre>

Recommended permissions for a _user exit_ is __r-xr-xr-x__
 
###qm.ini stanza
<pre>
ApiExitLocal:
   Sequence=100
   Name=smsolog
   Function=EntryPoint
   Module=/var/mqm/exits64/Installation1/libsmsolog.so
</pre>

#### Stop & compile & start
<pre>
endmqm DEFQM -w; gcc -m64 -shared -fPIC -o /var/mqm/exits64/Installation1/libsmsolog.so_r smsolog.c -I/opt/mqm/inc && echo "Compiled" && strmqm DEFQM
</pre>


