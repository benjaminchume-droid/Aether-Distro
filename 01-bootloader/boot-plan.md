# Aether Boot Path

Firmware initializes hardware; the bootloader loads Linux and the Aether initramfs; Linux initializes the platform; Aether init becomes PID 1; early userspace mounts required filesystems; capability discovery runs; core services start; a debug shell is exposed before graphics.
