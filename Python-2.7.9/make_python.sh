
if test "$(arch)" = i686 ; then OS=i686-pc-linux-gnu; fi;
if test "$(arch)" = x86_64 ; then OS=x86_64-unknown-linux-gnu; fi;

(cp Makefile.pre.in_org Makefile.pre.in && \
./configure && \
make python Parser/pgen && \
mv python hostpython && \
mv Parser/pgen Parser/hostpgen && \
make distclean && \
patch -p1 < $PWD/Python-2.7.9-xcompile-patch && \
ac_cv_file__dev_ptc=no ac_cv_file__dev_ptmx=no ./configure --host=arm-linux --build=$OS --disable-ipv6 --enable-shared --prefix=$PWD/_install --with-pydebug && \
patch -p0 < $PWD/Python-2.7.9-removetest-patch && \
make -j2 HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen && \
make -j2 install HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen &&\
cp $PWD/build/lib.linux2-arm-2.7/*.so $PWD/_install/lib/python2.7/lib-dynload \
)

