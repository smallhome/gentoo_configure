LG-Motion-4G-Computer-USB-Backup
================================

Backup all partitions of the LG Motion 4G via USB to a Linux Computer

Purpose?: Backs up all partitions, and does not require a micro sdcard.

These scripts have been tested/verified to work. The backup script performs a md5sum to verify that your backups are not corrupted. Location of the ADB binary should be modified in the script for your system.

Steps to backup:

1. Reboot device into "2ndinit CMW Recovery".
2. Run: backup.sh
3. Type in sudo password if needed.
4. Script will backup to a folder marked by date and time.

Steps to restore:

1. Reboot device into "2ndinit CMW Recovery".
2. Run: restore.sh /backup-directory 
3. Type in sudo password if needed.
4. Script will restore all partitions from within the given folder.
