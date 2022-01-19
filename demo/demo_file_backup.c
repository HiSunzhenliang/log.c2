#include "demo.h"

#define LOG_FILE "demo.log"

int demo_file_backup_main(void){
    log_add_file_backup(LOG_FILE, 10, LOG_INFO);

    log_emergency("this is a log.");
    log_alert("this is a log.");
    log_critical("this is a log.");
    log_error("this is a log.");
    log_warn("this is a log.");
    log_notice("this is a log.");
    log_info("this is a log.");
    log_debug("this is a log.");

    log_emergency("this is a log.");
    log_alert("this is a log.");
    log_critical("this is a log.");
    log_error("this is a log.");
    log_warn("this is a log.");
    log_notice("this is a log.");
    log_info("this is a log.");
    log_debug("this is a log.");

    return 0;
}