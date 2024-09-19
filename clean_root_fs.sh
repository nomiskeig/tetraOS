
echo "Removing auto generated files from rootFS"
rm -rf rootFS/usr/include/*
mv rootFS/usr/lib/libc.a rootFS/
rm -rf rootFS/usr/lib/*
mv rootFS/libc.a rootFS/usr/lib/libc.a
rm -rf rootFS/usr/bin/*
