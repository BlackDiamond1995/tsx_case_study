pagecacheFilePath="/home/lys/Documents/pagecachefiles"
#pagecacheFilePath="/home/liuweijie/Documents/pagecachefiles"

cd $pagecacheFilePath

dd if=/dev/zero of=lockfile2G bs=1G count=2

dd if=/dev/zero of=targetfile bs=1G count=2

for i in 0 1 2 3 4 5 6 7
do
    dd if=/dev/zero of=evictfile1G_$i bs=1G count=1
done
