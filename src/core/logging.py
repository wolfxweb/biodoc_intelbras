import logging
import os
from logging.handlers import TimedRotatingFileHandler

LOG_DIR = "/log"
LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

def setup_logging():
    if not os.path.exists(LOG_DIR):
        try:
            os.makedirs(LOG_DIR)
        except OSError:
            # Fallback for local testing se /log não for gravável
            os.makedirs("./log", exist_ok=True)
            log_path = "./log/app.log"
        else:
            log_path = os.path.join(LOG_DIR, "app.log")
    else:
        log_path = os.path.join(LOG_DIR, "app.log")

    logger = logging.getLogger("biodoc_intelbras")
    logger.setLevel(logging.INFO)

    if not logger.handlers:
        handler = TimedRotatingFileHandler(
            log_path,
            when="midnight",
            interval=1,
            backupCount=30,
            encoding="utf-8"
        )
        handler.suffix = "%Y-%m-%d"
        formatter = logging.Formatter(LOG_FORMAT)
        handler.setFormatter(formatter)
        logger.addHandler(handler)
        
        console_handler = logging.StreamHandler()
        console_handler.setFormatter(formatter)
        logger.addHandler(console_handler)

    return logger

logger = setup_logging()
