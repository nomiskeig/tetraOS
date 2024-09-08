cd $(dirname $0)

echo "Installing shell"
cd shell 
make
cp shell ../../rootFS/usr/bin/shell
