ln -s ./helper/* ./
../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared  -g3 -std=gnu99  -c spinlock.c -o obj/spinlock.o
../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared  -g3 -std=gnu99  -c heaplib.c -o obj/heaplib.o
../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared  -g3 -std=gnu99  -c kernel.c -o obj/kernel.o
../mipsel-linux/bin/mipsel-linux-gcc -Wall  -O3 -static -mno-xgot -mno-abicalls -G 0 -Wa,-non_shared  -g3 -std=gnu99  -nostartfiles -nodefaultlibs -Wl,-T,kernel.x -o kernel obj/libgcc2.o obj/concurrency.o obj/mem.o obj/console.o obj/spinlock.o obj/hash.o obj/heaplib.o obj/printf.o obj/display.o obj/assert.o obj/kernel.o obj/intr.o obj/trap.o obj/machine.o
find ./ -maxdepth 1 -lname '*' -exec rm {} \;
