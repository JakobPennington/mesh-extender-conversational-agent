#!/bin/sh
echo "Please insert a USB you wish to configure."
echo "WARNING: This will erase everything on the USB."
while true; do
    read -p "Do you wish to continue? [y/n] " yn
    case $yn in
        y ) echo "Partitioning USB."
            # Discover which USB drive was plugged in
            dmesglog=$(dmesg | tail -n 15 | grep "Attached SCSI removable disk")
            for driveletter in a b c d; do
                if [ "$$(echo "$dmesglog" | grep sd$$driveletter | wc -l)" != 0 ]; then
                    attached="sd$$driveletter"
                    echo "USB is recognised as $$attached"
                    break
                fi
            done

            if [ ! -e /dev/$attached ]; then
                echo "No USB attached. Please insert a USB and try again."
                return 1
            fi

            # Check if the disk is mounted anywhere
            mounts=`mount | grep /dev/$attached | wc -l`
            if [[ $$mounts != 0 ]]; then
                echo "That disk is mounted.  Unmounting."
                umount /dev/$$attached"1"
                umount /dev/$$attached"2"
                umount /dev/$$attached"3"
            fi

           fdisk /dev/$$attached <<EOF
            d
            1
            d
            2
            d
            3
            d
            4
            n
            p
            1

            +1G
            n
            p
            2

            +1G
            n
            p
            3


            w
EOF
            echo
            echo
            echo
            echo "Finished fdisk: remove and re-insert memory stick and run populate-memory-stick."
            read -p "Please remove and re-insert the memory stick. Type y then [enter] to continue. $
            if [ "$$continue" == "y" ]; then
                echo "Formatting partitions."

                # Discover which USB drive was plugged in
                dmesglog=$(dmesg | tail -n 15 | grep "Attached SCSI removable disk")
                for driveletter in a b c d; do
                    if [ "$$(echo "$$dmesglog" | grep sd$$driveletter | wc -l)" != 0 ]; then
                        attached="sd$$driveletter"
                        echo "USB is recognised as $$attached"
                        break
                    fi
                done

                # Unmount and format partitions
                umount /dev/$$attached"1"
                umount /dev/$$attached"2"
                umount /dev/$$attached"3"
                mkfs.ext4 /dev/$$attached"1" -L dos
                mkfs.ext4 /dev/$$attached"2" -L serval
                mkfs.ext4 /dev/$$attached"3" -L serval-var

                # Mount the USB partitions
                mkdir /dos
                mkdir /serval
                mkdir /serval-var
                mount -t ext4 /dev/$$attached"1" /dos
                mount -t ext4 /dev/$$attached"2" /serval
                mount -t ext4 /dev/$$attached"3" /serval-var
            else
                echo "Cannot finish install without configuring USB."
                echo "Cancelling install"
                return 1
            fi
            return 0
            ;;
        n ) echo "Cancelling install."
            return 1
            ;;
        * ) echo "Please answer y or n."
            ;;
    esac
done
