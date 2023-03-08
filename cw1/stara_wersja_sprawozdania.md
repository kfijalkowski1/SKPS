## Przygotowanie stanowiska
Składamy płytkę zgodnie z instrukcją

## Pierwsze uruchomienie
**na host-cie** do terminala UART
`tio /dev/ttyUSB0`

podłączamy zasilanie do płytki

**na UART**
logujemy się jako root

przyznanie ip
`udhcpc`

sprawdzamy ip
`ifconfig`

sprawdzamy połączanie
`ping 192.168.9.113 -c 1`
sukces

**na host**
`ping 10.42.0.196`
sukces


## Obraz z initramfs
`make raspberrypi4_64_defconfig`
`make nconfig`

Toolchain --> Toolchain type: External toolchain
![[Pasted image 20230308175315.png|600]]

`make`
```
ERROR: file(rootfs.ext4): stat(/home/wojtek/Downloads/ramdisk/output/images/rootfs.ext4) failed: No such file or directory
ERROR: hdimage(sdcard.img): could not setup rootfs.ext4
make[1]: *** [Makefile:817: target-post-image] Error 1
make: *** [Makefile:84: _all] Error 2
```
pomimo tego błędu system działa


## Uruchomienie obrazu z initramfs
stawiamy serwer python
`python3 -m http.server`

ściągamy pliki cmdline.txt, bcm2711-rpi-4-b.dtb, Image
za pomocą wget do /root
`wget http://:8000/namefile`
``

mountujemy
`mount /dev/mmcblk0p1 /mnt`
pojawia się błąd z fsck mimo którego montowanie działa

przenosimy pliki pliki cmdline.txt, bcm2711-rpi-4-b.dtb, Image, do /mnt/usr
Image -> Kernel8.img

`reboot` 
przytrzymując SW4

uruchomił się system z ramdysku, na którym nie zostały zachowane pliki po ponownym uruchomieniu


## Obraz bez initramfs
`make raspberrypi4_64_defconfig`
`make nconfig`

Toolchain --> Toolchain type: External toolchain
![[Pasted image 20230308181642.png]]

`make`
```
ERROR: file(rootfs.ext4): stat(/home/wojtek/Downloads/buildroot-2022.02.10/output/images/rootfs.ext4) failed: No such file or directory
ERROR: hdimage(sdcard.img): could not setup rootfs.ext4
make[1]: *** [Makefile:817: target-post-image] Error 1
make: *** [Makefile:84: _all] Error 2
```
pomimo tego błędu system działa


## Uruchomienie obrazu bez initramfs
stawiamy serwer python
`python3 -m http.server`

ściągamy pliki cmdline.txt, bcm2711-rpi-4-b.dtb, Image, rootfs.ext2
za pomocą wget do /root
`wget http://:8000/namefile`

nagrywamy system plików
`dd if=rootfs.ext2 of=/dev/mmcblk0p2 bs=4096`

mountujemy
`mount /dev/mmcblk0p1 /mnt`
pojawia się błąd z fsck mimo którego działa

przenosimy pliki pliki cmdline.txt, bcm2711-rpi-4-b.dtb, Image, do /mnt/usr
Image -> Kernel8.img

`reboot` 
przytrzymując SW4

tworzymy plik `touch test.txt`

po ponownym uruchomieniu pozostał plik test.txt


