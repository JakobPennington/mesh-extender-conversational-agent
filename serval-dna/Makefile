include $(TOPDIR)/rules.mk

PKG_NAME:=serval-dna
PKG_VERSION:=MESHEXTENDER-20160504.1051.39
PKG_RELEASE:=45

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/servalproject/serval-dna.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=c78ee668d5a08ca4f528a4260de384aaa2a06fed
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_FIXUP:=autoreconf

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/serval-dna
	TITLE:=Serval DNA mesh networking daemon
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=Mesh networking
	DEPENDS:=+librt +fdisk +libext2fs +e2fsprogs +libcom_err +uhttpd +dnsmasq
	MAINTAINER:=Serval DNA Maintainer <serval-dna@servalproject.org>
endef

define Package/serval-dna/description
  Serval DNA is a daemon that implements the Serval Mesh network protocol as an
  overlay network encapsulated within UDP/IP packets over Wi-Fi.  The daemon,
  when running, joins the OpenWRT router to the local Serval Mesh Wi-Fi
  network.  This allows it to participate in dynamic mesh routing (using
  Serval's own mesh routing protocol), file distribution (using the Rhizome
  store-and-forward protocol), secure text messaging (using the MeshMS
  protocol), and to relay secure voice calls (using the VoMP voice-over-mesh
  protocol) and other secure Serval traffic (such as tunnelled SOCKS proxy
  connections) between Android devices running the Serval Mesh app.

  The Serval Mesh Extender device is essentially a Wi-Fi router running the
  Serval DNA daemon on OpenWRT.
endef

define Package/serval-dna/preinst
#!/bin/sh
echo "Please insert a USB configured with partition-memory-stick and populate-memory-stick"
echo "WARNING: This will erase everything on the USB."
while true; do
  read -p "Do you wish to continue? [y/n] " continue
  case $$continue in
    y )
      # Discover which USB drive was plugged in
      dmesglog=$$(dmesg | tail -n 15 | grep "Attached SCSI removable disk")
      device=0
      for driveletter in a b c d; do
        if [ "$$(echo "$$dmesglog" | grep sd$${driveletter} | wc -l)" != 0 ]; then
          device="sd$${driveletter}"
          echo "USB is recognised as $$device"
          break
        fi
      done

      if [ ! -e /dev/$$device ]; then
        echo "No USB attached. Please insert a USB and try again."
        return 1
      fi

      if [ $$device == 0 ]; then
        echo "No USB attached recently. Please re-insert a USB and try again."
        return 1
      fi

      # Kill all serval related processes
      echo "runservald pid: $$(ps | grep runservald | grep -v grep | awk '{print $$1}')"
      ps | grep runservald | grep -v grep | awk '{print $$1}' | xargs kill
      echo "testing pid: $$(ps | grep testing | grep -v grep | awk '{print $$1}')"
      ps | grep testing | grep -v grep | awk '{print $$1}' | xargs kill
      killall servald

      # Do check if the disk is mounted anywhere
      mounts=$$(mount | grep /dev/$${device} | wc -l)
      echo "${mounts}"
      if [[ $$mounts != 0 ]]; then
        echo "That disk is mounted. Unmounting."
        umount -lv /dev/$${device}1
        umount -lv /dev/$${device}2
        umount -lv /dev/$${device}3
	umount -lv /mnt/$${device}1
	umount -lv /mnt/$${device}2
	umount -lv /mnt/$${device}3
      fi

      # For strange cases when non-existent devices are mounted to these directories
      umount -lv /dos
      umount -lv /serval
      umount -lv /serval-var

      echo "***************"
      mount
      echo "***************"

      rm -rf /dos
      rm -rf /serval
      rm -rf /serval-var
      sync
      mkdir /dos
      mkdir /serval
      mkdir /serval-var
      mount -t vfat /dev/$${device}1 /dos
      mount -t ext4 /dev/$${device}2 /serval
      mount -t ext4 /dev/$${device}3 /serval-var

      return 0
    ;;
    n )
      echo "Cancelling Install"
      return 1
    ;;
    * )
      echo "Please answer with y or n."
    ;;
  esac
done
exit 0
endef

define Package/serval-dna/postinst
#!/bin/sh
# Check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
        echo "Enabling rc.d symlink for servald"
        /etc/init.d/servald enable
	echo "Enabling rc.d symlink for wirelesssetup"
	/etc/init.d/wirelesssetup enable
	echo "Enabling rc.d symlink for captiveportal"
	/etc/init.d/captiveportal enable
fi
exit 0
endef

define Package/serval-dna/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
        echo "Removing rc.d symlink for servald"
        /etc/init.d/servald disable
	echo "Removing rc.d symlink for wirelesssetup"
	/etc/init.d/wirelesssetup disable
	echo "Removing rc.d symlink for captiveportal
	/etc/init.d/captiveportal disabe
fi
exit 0
endef

define Build/Configure
	echo "$(PKG_VERSION)" >$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/$(strip $(3))/VERSION.txt
	$(call Build/Configure/Default,$(1),$(2),$(3))
endef

define Package/serval-dna/install
	$(INSTALL_DIR) $(1)/serval
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/servald $(1)/serval

	$(INSTALL_DIR) $(1)/serval/www
	$(INSTALL_DATA) ./files/serval/www/index* $(1)/serval/www

	$(INSTALL_DIR) $(1)/serval/www/css
	$(INSTALL_DATA) ./files/serval/www/css/* $(1)/serval/www/css

	$(INSTALL_DIR) $(1)/serval/www/fonts
	$(INSTALL_DATA) ./files/serval/www/fonts/* $(1)/serval/www/fonts

	$(INSTALL_DIR) $(1)/serval/www/images
	$(INSTALL_DATA) ./files/serval/www/images/* $(1)/serval/www/images

	$(INSTALL_DIR) $(1)/serval/www/js
	$(INSTALL_DATA) ./files/serval/www/js/* $(1)/serval/www/js

	$(INSTALL_DIR) $(1)/serval/www/software/android
	$(INSTALL_DATA) ./files/serval/www/software/android/* $(1)/serval/www/software/android

	$(INSTALL_DIR) $(1)/serval/www/software/ios
	$(INSTALL_DATA) ./files/serval/www/software/ios/* $(1)/serval/www/software/ios

	$(INSTALL_DIR) $(1)/serval/www/software/linux
	$(INSTALL_DATA) ./files/serval/www/software/linux/* $(1)/serval/www/software/linux

	$(INSTALL_DIR) $(1)/serval/www/software/osx
	$(INSTALL_DATA) ./files/serval/www/software/osx/* $(1)/serval/www/software/osx

	$(INSTALL_DIR) $(1)/serval/www/software/source
	$(INSTALL_DATA) ./files/serval/www/software/source/index.html $(1)/serval/www/software/source

	$(INSTALL_DIR) $(1)/serval/www/software/windows
	$(INSTALL_DATA) ./files/serval/www/software/windows/* $(1)/serval/www/software/windows

	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/etc/profile $(1)/etc
	$(INSTALL_DATA) ./files/etc/banner $(1)/etc
	
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/fstab $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/network.template $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/system $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/wireless.template $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/dhcp $(1)/etc/config
	$(INSTALL_DATA) ./files/etc/config/uhttpd $(1)/etc/config

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/servald $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/dropbear $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/wirelesssetup $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/captiveportal $(1)/etc/init.d

	$(INSTALL_DIR) $(1)/etc/serval
	$(INSTALL_BIN) ./files/etc/serval/runservald $(1)/etc/serval
	$(INSTALL_BIN) ./files/etc/serval/otaupdate $(1)/etc/serval/otaupdate
	$(INSTALL_BIN) ./files/etc/serval/meshmsresponder $(1)/etc/serval/meshmsresponder
	$(INSTALL_BIN) ./files/etc/serval/testing $(1)/etc/serval/testing

	$(INSTALL_DIR) $(1)/serval-var
	$(INSTALL_DATA) ./files/serval-var/serval.conf $(1)/serval-var

	$(INSTALL_DIR) $(1)/dos
	$(INSTALL_DATA) ./files/dos/switch_values $(1)/dos
	$(INSTALL_DATA) ./files/dos/supported_objects $(1)/dos
endef

$(eval $(call BuildPackage,serval-dna))
