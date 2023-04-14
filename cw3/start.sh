qemu-system-aarch64 -M virt -nographic \
	-m 128 \
	-cpu cortex-a57 -smp 2 \
	-kernel openwrt-dir/image -append "root=fe00" \
	-drive file=openwrt-dir/rootfs.ext4,if=none,format=raw,id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-nic user,hostfwd=tcp::5556-:22,hostfwd=tcp::5557-:80