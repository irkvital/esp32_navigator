#pragma once

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"


sdmmc_card_t* sd_mount();
void sd_unmount(sdmmc_card_t* card);