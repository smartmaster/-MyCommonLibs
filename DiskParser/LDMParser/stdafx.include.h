#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <iostream>
#include <fcntl.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <linux/fs.h>
#endif
