##Compile
 <pre>gcc -m64 -shared -fPIC -o /var/mqm/exits64/libsmsolog.so smsolog.c -I/opt/mqm/inc -L/opt/mqm/lib64 -Wl,-rpath=/opt/mqm/lib64 -Wl,-rpath=/usr/lib64 -lmqm_r</pre>