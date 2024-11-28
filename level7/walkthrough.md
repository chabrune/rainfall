level7@RainFall:~$ ltrace ./level7 12 12
__libc_start_main(0x8048521, 3, 0xbffff6f4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                = 0x0804a008
malloc(8)                                = 0x0804a018
malloc(8)                                = 0x0804a028
malloc(8)                                = 0x0804a038
strcpy(0x0804a018, "12")                 = 0x0804a018
strcpy(0x0804a038, "12")                 = 0x0804a038
fopen("/home/user/level8/.pass", "r")    = 0
fgets( <unfinished ...>

(gdb) x/32wx 0x0804a008
0x804a008:      0x00000001      0x0804a018      0x00000000      0x00000011
0x804a018:      0x00004141      0x00000000      0x00000000      0x00000011
0x804a028:      0x00000002      0x0804a038      0x00000000      0x00000011
0x804a038:      0x00004141      0x00000000      0x00000000      0x00020fc1

Variable gloable : 0x8049960
fgets(var c(globale), n: 68, open(.pass), mode : read)

puts(08048703  data_8048703: ~~.)

(gdb) x/32w $esp
0xbffff610:     0x08049960      0x00000044      0x00000000      0xb7e5ee55
0xbffff620:     0xb7fed280      0x00000000      0x0804a028      0x0804a008
0xbffff630:     0x08048610      0x00000000      0x00000000      0xb7e454d3
0xbffff640:     0x00000003      0xbffff6d4      0xbffff6e4      0xb7fdc858
0xbffff650:     0x00000000      0xbffff61c      0xbffff6e4      0x00000000
0xbffff660:     0x0804825c      0xb7fd0ff4      0x00000000      0x00000000
0xbffff670:     0x00000000      0x0aae8655      0x3deb2245      0x00000000
0xbffff680:     0x00000000      0x00000000      0x00000003      0x08048440

Ecrire de 0x0804a038 a 0x8049960