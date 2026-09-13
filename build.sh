#!/bin/bash

set -e

# ============================================================
# ROS 2 Jazzy - Bare Bones Installation
# Ubuntu 24.04 Noble / WSL2
# ============================================================

PASSWORD="1"

echo "================================================"
echo " ROS 2 Jazzy - Bare Bones Installation"
echo " Ubuntu 24.04 Noble"
echo "================================================"

# ------------------------------------------------------------
# Function: run sudo command
# ------------------------------------------------------------

sudo_cmd()
{
    printf '%s\n' "$PASSWORD" | sudo -S "$@"
}

# ------------------------------------------------------------
# 1. Check Ubuntu version
# ------------------------------------------------------------

echo ""
echo "[1/8] Checking Ubuntu version..."

source /etc/os-release

echo "OS       : $PRETTY_NAME"
echo "Codename : $VERSION_CODENAME"
echo "Arch     : $(dpkg --print-architecture)"

if [ "$VERSION_CODENAME" != "noble" ]; then
    echo ""
    echo "[ERROR] This script requires Ubuntu 24.04 Noble."
    exit 1
fi

# ------------------------------------------------------------
# 2. Test sudo password
# ------------------------------------------------------------

echo ""
echo "[2/8] Checking sudo access..."

printf '%s\n' "$PASSWORD" | sudo -S -v

echo "[OK] Sudo authentication successful."

# ------------------------------------------------------------
# 3. Update Ubuntu
# ------------------------------------------------------------

echo ""
echo "[3/8] Updating Ubuntu package database..."

sudo_cmd apt update

# ------------------------------------------------------------
# 4. Install prerequisites
# ------------------------------------------------------------

echo ""
echo "[4/8] Installing prerequisites..."

sudo_cmd apt install -y \
    software-properties-common \
    curl \
    locales

# ------------------------------------------------------------
# 5. Configure locale
# ------------------------------------------------------------

echo ""
echo "[5/8] Configuring UTF-8 locale..."

sudo_cmd locale-gen en_US en_US.UTF-8

sudo_cmd update-locale \
    LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8

export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

echo "[OK] Locale configured."

# ------------------------------------------------------------
# 6. Enable Ubuntu Universe repository
# ------------------------------------------------------------

echo ""
echo "[6/8] Enabling Ubuntu Universe repository..."

sudo_cmd add-apt-repository universe -y

sudo_cmd apt update

echo "[OK] Universe repository enabled."

# ------------------------------------------------------------
# 7. Install ROS 2 apt source
# ------------------------------------------------------------

echo ""
echo "[7/8] Installing ROS 2 apt repository..."

ROS_APT_SOURCE_VERSION=$(curl -s \
    https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest \
    | grep -F "tag_name" \
    | awk -F'"' '{print $4}')

if [ -z "$ROS_APT_SOURCE_VERSION" ]; then
    echo "[ERROR] Cannot determine ROS apt source version."
    exit 1
fi

echo "ROS apt source version: $ROS_APT_SOURCE_VERSION"

ROS_APT_PACKAGE="/tmp/ros2-apt-source.deb"

curl -L \
    -o "$ROS_APT_PACKAGE" \
    "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.${UBUNTU_CODENAME}_all.deb"

sudo_cmd dpkg -i "$ROS_APT_PACKAGE"

echo "[OK] ROS 2 apt repository installed."

# ------------------------------------------------------------
# 8. Install ROS 2 Jazzy Bare Bones
# ------------------------------------------------------------

echo ""
echo "[8/8] Installing ROS 2 Jazzy ROS-Base..."

sudo_cmd apt update

sudo_cmd apt install -y ros-jazzy-ros-base

echo ""
echo "================================================"
echo " ROS 2 Jazzy installation completed!"
echo "================================================"

# ------------------------------------------------------------
# Configure ROS 2 environment
# ------------------------------------------------------------

echo ""
echo "[INFO] Configuring ROS 2 environment..."

ROS_SETUP="source /opt/ros/jazzy/setup.bash"

if ! grep -Fxq "$ROS_SETUP" "$HOME/.bashrc"; then
    echo "$ROS_SETUP" >> "$HOME/.bashrc"
    echo "[OK] Added ROS 2 Jazzy to ~/.bashrc"
else
    echo "[OK] ROS 2 Jazzy already exists in ~/.bashrc"
fi

# Load ROS 2 for current script
source /opt/ros/jazzy/setup.bash

# ------------------------------------------------------------
# Verify installation
# ------------------------------------------------------------

echo ""
echo "================================================"
echo " Verification"
echo "================================================"

echo ""
echo "ROS_DISTRO:"
echo "$ROS_DISTRO"

echo ""
echo "ROS_VERSION:"
echo "$ROS_VERSION"

echo ""
echo "ROS 2 executable:"
which ros2

echo ""
echo "ROS 2 packages:"
ros2 pkg list | head -10

echo ""
echo "================================================"
echo " ROS 2 Jazzy Bare Bones is READY!"
echo "================================================"

echo ""
echo "Open a new terminal or run:"
echo ""
echo "    source ~/.bashrc"
echo ""
echo "Then test:"
echo ""
echo "    ros2 --help"
echo ""