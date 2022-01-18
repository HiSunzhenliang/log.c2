#include "demo.h"

int demo_stdout_main(void) {
    log_stdout_verbose(LOG_DEBUG);
    log_emergency("this is a log.");
    log_alert("this is a log.");
    log_critical("this is a log.");
    log_error("this is a log.");
    log_warn("this is a log.");
    log_notice("this is a log.");
    log_info("this is a log.");
    log_debug("this is a log.");

    log_stdout_verbose(LOG_WARN);
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