TESTS=('TEST00' 'TEST01' 'TEST02' 'TEST03' 'TEST04' 'TEST05' 'TEST06' 'TEST07' 'TEST08' 'TEST09' 'TEST10' 'TEST11' 'TEST12' 'TEST13' 'TEST14' 'TEST15' 'TEST16' 'TEST17' 'TEST18' 'TEST19' 'TEST20' 'TEST21' 'TEST22' 'TEST23' 'TEST24' 'TEST25')

CC1='../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared -D'
CC2='  -g3 -std=gnu99  -c kernel.c -o obj/kernel.o'
CC3='../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared  -g3 -std=gnu99  -nostartfiles -nodefaultlibs -Wl,-T,kernel.x -o kernel obj/libgcc2.o obj/concurrency.o obj/mem.o obj/console.o obj/spinlock.o obj/hash.o obj/heaplib.o obj/printf.o obj/display.o obj/assert.o obj/kernel.o obj/intr.o obj/trap.o obj/machine.o'

EXEC='../mips-sim/ksimulate64 -c 1 -m 12 ./kernel'

./compile.sh

ln -s ./helper/* ./

for T in "${TESTS[@]}"
do
    :
    cmd=$CC1$T$CC2
    echo $cmd
    eval $cmd
    eval $CC3
    eval $EXEC
done

find ./ -maxdepth 1 -lname '*' -exec rm {} \;
