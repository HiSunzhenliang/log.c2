#include "demo.h"

#define LOG_FILE "demo.log"

int demo_file_main(void){
    FILE* fp = fopen(LOG_FILE, "a");
    if (!fp) {
        perror("launcher create log file failed");
        return -1;
    }
    log_add_fp(fp, LOG_INFO);

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