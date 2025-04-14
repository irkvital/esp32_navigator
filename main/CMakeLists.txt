idf_component_register(PRIV_REQUIRES spi_flash fatfs
                    INCLUDE_DIRS "include"
                    SRCS "sd_card.c" "button_fix_bounce.c" "display_buttons.c"
                    
                    # PRIV_REQUIRES spi_flash fatfs
                    WHOLE_ARCHIVE)
