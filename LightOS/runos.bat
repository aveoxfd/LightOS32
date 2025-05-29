
nasm -f bin boot\asm\boot.asm -o boot.bin
nasm -f bin boot\asm\partboot.asm -o partboot.bin
nasm -f bin boot\asm\end.asm -o end.bin


set GLOBAL_INCLUDES=-I kernel\int\C\headers -I kernel\drivers\C\headers -I kernel\tools\C\headers
set GLOBAL_C = -I kernel\int\C\main -I kernel\drivers\C\main -I kernel\tools\C\main


i686-elf-gcc -m32 -ffreestanding -O2 -nostdlib -nostartfiles %GLOBAL_INCLUDES% -c kernel\int\C\main\IDT.c -o IDT.o
i686-elf-gcc -m32 -ffreestanding -O2 -nostdlib -nostartfiles %GLOBAL_INCLUDES% -c kernel\drivers\C\main\pic.c -o pic.o
i686-elf-gcc -m32 -ffreestanding -O2 -nostdlib -nostartfiles %GLOBAL_INCLUDES% -c kernel\drivers\C\main\keyboard.c -o keyboard.o
i686-elf-gcc -m32 -ffreestanding -O2 -nostdlib -nostartfiles %GLOBAL_INCLUDES% -c kernel\tools\C\main\memory.c -o memory.o
i686-elf-gcc -ffreestanding -m32 -nostdlib %GLOBAL_INCLUDES% -c kernel\kernel.c -o kernel.o
i686-elf-gcc -ffreestanding -m32 -nostdlib %GLOBAL_INCLUDES% -c kernel\tools\C\main\out.c -o out.o
i686-elf-gcc -ffreestanding -m32 -nostdlib %GLOBAL_INCLUDES% -c kernel\drivers\C\main\vga.c -o vga.o
i686-elf-gcc -ffreestanding -m32 -nostdlib %GLOBAL_INCLUDES% -c kernel\tools\C\main\input.c -o input.o
i686-elf-gcc -ffreestanding -m32 -nostdlib %GLOBAL_INCLUDES% -c kernel\tools\C\main\screen.c -o screen.o


nasm -f elf32 kernel\int\asm\void_handler.asm -o void_handler.o
nasm -f elf32 kernel\int\asm\keyboard_interrupt_handler.asm -o keyboard_interrupt_handler.o
nasm -f elf32 kernel\int\asm\timer_interrupt_handler.asm -o timer_interrupt_handler.o
nasm -f elf32 kernel\drivers\asm\port.asm -o port.o


i686-elf-gcc -m32 -nostdlib -nostartfiles ^
  kernel.o ^
  void_handler.o ^
  keyboard_interrupt_handler.o ^
  timer_interrupt_handler.o ^
  IDT.o ^
  port.o ^
  pic.o ^
  keyboard.o ^
  memory.o ^
  vga.o ^
  out.o ^
  input.o ^
  screen.o ^
  -Wl,-Ttext=0x8C24 ^
  -Wl,--oformat=binary ^
  -o kernel.bin


copy /b boot.bin+partboot.bin+kernel.bin+end.bin os.img


qemu-system-i386 -drive format=raw,file=os.img -m 4096

del /Q *.bin
del /Q *.o


pause